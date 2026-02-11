#include "Setup.h"
#include "Vardef.h"
#include "FTL.h"
#include "IO.h"

#include <string.h>
#include <stdlib.h>


#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)


#if YMTC_JGS
#define SLC_PREFIX() FPU_CMD(0xDA)
#define TLC_PREFIX() FPU_CMD(0xDF)
#define FOURTH_COL_ADDR_DIE(x) FPU_ADDR_DAT(1, x)
#elif Hynix3DV6 || Hynix3DV5
#define SLC_PREFIX() FPU_CMD(0xA2)
#define TLC_PREFIX() (0x0000)
#define FOURTH_COL_ADDR_DIE(x) (0x0000)
#else

#endif
#if BURNER
#define IRAMCONTENT_SECTION (".FPU.data")
#else
#define IRAMCONTENT_SECTION (".ICODE.data")
#endif


__attribute__((section(IRAMCONTENT_SECTION))) FPUSEQs_T gubIRAMContent = {
	//	UWORD FPU_PTR_CFF[2];
	{
		FPU_CMD(0xFF), FPU_END()
	},
	//	UWORD FPU_PTR_C70_POL_MK40[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_END()
	},
	//	UWORD FPU_PTR_C70_POL_MK40_C00[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x00), FPU_END()
	},
	//UWORD FPU_PTR_C70_POL_MK20[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20), FPU_END()
	},
	//UWORD FPU_PTR_C70_POL_MK20_C00[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20), FPU_CMD(0x00), FPU_END()
	},
	//UWORD FPU_PTR_C70_DAT_MK40[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x40), FPU_END()
	},
	//UWORD FPU_PTR_C70_DAT_MK20[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x20), FPU_END()
	},
	//UWORD FPU_PTR_C71_DAT_MK40[8];
	{
		FPU_CMD(0x71), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x40), FPU_END()
	},
	//UWORD FPU_PTR_C71_DAT_MK20[8];
	{
		FPU_CMD(0x71), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x20), FPU_END()
	},
	//UWORD FPU_PTR_C73_DAT_MK40[8];
	{
		FPU_CMD(0x73), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x40), FPU_END()
	},
	//UWORD FPU_PTR_C73_DAT_MK20[8];
	{
		FPU_CMD(0x73), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x20), FPU_END()
	},
	//UWORD FPU_PTR_C70_DAT_MK0F[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x0F), FPU_END()
	},
	//UWORD FPU_PTR_C71_DAT_MK1F[8];
	{
		FPU_CMD(0x71), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x1F), FPU_END()
	},
	//UWORD FPU_PTR_C73_DAT_MK1F[8];
	{
		FPU_CMD(0x73), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x1F), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C60_A3_CD0[8] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},
	//UWORD FPU_PTR_AG_C60_A3_CD0[7] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0[17] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	//UWORD FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0[16] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0[11] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	//UWORD FPU_PTR_AG_C60_A3_AG_C60_A3_CD0[10] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C80_A5_DW[9] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_END()
	},

	//UWORD FPU_PTR_AG_C80_A5_DW[8] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C80_A5_DW_C11[10] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x11), FPU_END()
	},

	//UWORD FPU_PTR_AG_C80_A5_DW_C11[7] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W()
	},
	//UWORD FPU_PTR_C11[2] =
	{
		FPU_CMD(0x11), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C80_A5_DW_C15[10] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//UWORD FPU_PTR_AG_C80_A5_DW_C15[9] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C80_A5_DW_C10[10] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},

	//UWORD FPU_PTR_AG_C80_A5_DW_C10[7] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W()
	},

	//UWORD FPU_PTR_C10[2] =
	{
		FPU_CMD(0x10), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C81_A5_DW_C11[8] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x11), FPU_END()
	},

	//UWORD FPU_PTR_AG_C81_A5_DW_C11[7] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x11), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C81_A5_DW_C15[8] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//UWORD FPU_PTR_AG_C81_A5_DW_C15[7] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C81_A5_DW_C10[8] =

	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},

	//UWORD FPU_PTR_AG_C81_A5_DW_C10[7] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15[12] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},


	//UWORD FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15[1] =
	{
		FPU_ADR_GEN()
	},
	//UWORD FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15[10] =
	{
		FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW[11] =

	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},

	//UWORD FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW[1] =
	{
		FPU_ADR_GEN()
	},

	//UWORD FPU_PTR_C11_C70_POL_MK40_C81_A5_DW[9] =
	{
		FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(1, 1, 0x81), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C00_A5_C30[8] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	//UWORD FPU_PTR_AG_C00_A5_C30[1] =
	{
		FPU_ADR_GEN()
	},
	//UWORD FPU_PTR_C00_A5_C30[6] =
	{
		FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[29] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	//UWORD FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[28] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},
	//UWORD FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[15] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	//UWORD FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[14] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C00_A5_C31[6] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x31), FPU_END()
	},
	//UWORD FPU_PTR_AG_C00_A5_C31[5] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x31), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[27] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x31), FPU_END()
	},

	//UWORD FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[26] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x31), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[13] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x31), FPU_END()
	},

	//UWORD FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[12] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40),
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x31), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C00_A5_C32[8] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_END()
	},

	//UWORD FPU_PTR_AG_C00_A5_C32[7] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_END()
	},
	//UWORD FPU_PTR_CA2_C31[1] =
	{
		SLC_PREFIX(), FPU_CMD(0x31), FPU_END()
	},
	//UWORD FPU_PTR_C31[2] =
	{
		TLC_PREFIX(), FPU_CMD(0x31), FPU_END()
	},

	////UWORD FPU_PTR_CA2_C3F[1] =
	{
		SLC_PREFIX(), FPU_CMD(0x3F), FPU_END()
	},

	////UWORD FPU_PTR_C3F[2] =
	{
		TLC_PREFIX(), FPU_CMD(0x3F), FPU_END()
	},

	////UWORD FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30[15] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	////UWORD FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30[14] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	////UWORD FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30[9] =
	{
		FPU_ADR_GEN(), SLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	////UWORD FPU_PTR_AG_C60_A3_AG_C60_A3_C30[8] =
	{
		FPU_ADR_GEN(), TLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_END()
	},

	////UWORD FPU_PTR_AG_DR[3] =
	{
		FPU_ADR_GEN(), FPU_DMA_R(), FPU_END()
	},

	////UWORD FPU_PTR_AG_C00_DR[5] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x00), FPU_DLY(0x50), FPU_DMA_R(), FPU_END()
	},

	////UWORD FPU_PTR_AG_C00_A5_C05_A2_CE0_DR[1] =
	{
		FPU_ADR_GEN()
	},
	////UWORD FPU_PTR_C00_A5_C05_A2_CE0_DR[8] =
	{
		FPU_CMD(0x00), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x05), FPU_ADDR(0x02), FPU_CMD(0xE0), FPU_DLY(0x50), FPU_DMA_R(), FPU_END()
	},

	////UWORD FPU_PTR_NOP[2] =
	{
		FPU_NOP(), FPU_END()
	},

	////UWORD FPU_PTR_AG_IBF_CORRECT[3] =
	{
		FPU_ADR_GEN(), FPU_IBF_CORRECT(), FPU_END()
	},

	////UWORD FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION[2] =
	{
		FPU_BR(0, 0, 0, 0), FPU_END()
	},

	////UWORD FPU_PTR_AG_RAW[3] =
	{
		FPU_ADR_GEN(), FPU_RAW(), FPU_END()
	},

	////UWORD FPU_PTR_SOFTBIT_CORRECTION[2] =
	{
		FPU_SBC(), FPU_END()
	},

	////UWORD FPU_PTR_CC6_AG_C60_A3_CD0[8] =
	{
		FPU_ADR_GEN(), FPU_CMD(0xC6), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	////UWORD FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0[9] =
	{
		FPU_ADR_GEN(), FPU_CMD(0xC6), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	////UWORD FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0[16] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	////UWORD FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0[15] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), TLC_PREFIX(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xD0), FPU_END()
	},

	////UWORD FPU_PTR_CFC[2] =
	{
		FPU_CMD(0xFC), FPU_END()
	},

	//UWORD FPU_PTR_AG_CA2_C80_A5_C10[11] =
	{
		FPU_ADR_GEN(), FPU_CMD_DQS(1, 1, 0x78), FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), SLC_PREFIX(), FPU_CMD_DQS(1, 1, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x50), FPU_CMD(0x10), FPU_END()
	},


};

#else


#if TLC
#if(!BURNER)
__attribute__((section(".ICODE.data")))	UBYTE gubIRAMContent[FPU_Length] =
#else
__attribute__((section(".FPU.data")))	UBYTE gubIRAMContent[FPU_Length] =
#endif
#else
#if(RDT)
UBYTE gubIRAMContent[FPU_Length] =
#else
#if(!BURNER)
__attribute__((section(".ICODE.data")))	UBYTE gubIRAMContent[FPU_Length] =
#else
__attribute__((section(".FPU.data")))	UBYTE gubIRAMContent[FPU_Length] =
#endif
#endif
#endif
{
#if Hynix
	0xFF, 0x20, 0x00, 0xF0,																								//0x0000    cmdFF-end
	0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0xF0,																					//0x0004	    cmd70-dly20-poll with Mask 0x40-end
	0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x20, 0x00, 0xF0,																			//0x000C    cmd70-dly20-poll with Mask 0x40-cmd00-end
	0x70, 0x20, 0x20, 0x10, 0x20, 0x51, 0x00, 0xF0,																					//0x0016	    cmd70-dly20-poll with Mask 0x20-end
	0x70, 0x20, 0x20, 0x10, 0x20, 0x51, 0x00, 0x20, 0x00, 0xF0,																			//0x001E    cmd70-dly20-poll with Mask 0x20-cmd00-end
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x40, 0x40, 0x00, 0xF0,																			//0x0028    cmd70-dly20-data(status) with Mask 0x40-end
	0x00, 0x00,																													//0x0032    empty
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x20, 0x40, 0x00, 0xF0,																			//0x0034    cmd70-dly20-data(status) with Mask 0x20-end
	0x71, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x40, 0x40, 0x00, 0xF0,																			//0x003E    cmd71-dly20-data(status) with Mask 0x40-end
	0x71, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x20, 0x40, 0x00, 0xF0,																			//0x0048    cmd71-dly20-data(status) with Mask 0x20-end
	0x00, 0x00,																													//0x0052    empty
	0x73, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x40, 0x40, 0x00, 0xF0,																			//0x0054    cmd73-dly20-data(status) with Mask 0x40-end
	0x73, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x20, 0x40, 0x00, 0xF0,																			//0x005E    cmd73-dly20-data(status) with Mask 0x20-end
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x0F, 0x40, 0x00, 0xF0,																			//0x0068    cmd70-dly20-data(status) with Mask 0x0F-end
	0x00, 0x00, 																													//0x0072    empty
	0x71, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x1F, 0x40, 0x00, 0xF0,																			//0x0074    cmd71-dly20-data(status) with Mask 0x1F-end
	0x73, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x1F, 0x40, 0x00, 0xF0,																			//0x007E    cmd73-dly20-data(status) with Mask 0x1F-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,	//0x0088    cmdA2-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,							//0x0098    adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,					//0x00A6 cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,					//0x00BC adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,					//0x00D8 cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,							//0x00EA adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x00, 0xF0,																	//0x00FA    cmdA2-adr_gen-cmd80-adr5-dly50-dma-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x00, 0xF0,																	//0x0108    adr_gen-cmd80-adr5-dly50-dma-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x11, 0x20, 0x00, 0xF0,	             													//0x0114    cmdA2-adr_gen-cmd80-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x11, 0x20, 0x00, 0xF0,	             													//0x0124    adr_gen-cmd80-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x15, 0x20, 0x00, 0xF0,															//0x0132    cmdA2-adr_gen-cmd80-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x15, 0x20, 0x00, 0xF0,															//0x0142    adr_gen-cmd80-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x10, 0x20, 0x00, 0xF0,															//0x0150    cmdA2-adr_gen-cmd80-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0x78, 0x23, 0x00, 0x33, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x10, 0x20, 0x00, 0xF0,															//0x0160    adr_gen-cmd80-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0xA2, 0x20, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x11, 0x20, 0x00, 0xF0,															//0x016E    cmdA2-adr_gen-cmd81-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x11, 0x20, 0x00, 0xF0,															//0x017E    adr_gen-cmd81-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0xA2, 0x20, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x15, 0x20, 0x00, 0xF0,															//0x0018C    cmdA2-adr_gen-cmd81-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x15, 0x20, 0x00, 0xF0,															//0x019C    adr_gen-cmd81-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0xA2, 0x20, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x10, 0x20, 0x00, 0xF0,															//0x01AA    cmdA2-adr_gen-cmd81-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x63, 0x10, 0x20, 0x00, 0xF0,															//0x01BA    adr_gen-cmd81-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0xA2, 0x20, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x63, 0x15, 0x20, 0x00, 0xF0, 									//0x01C8   cmdA2-adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-cmd15-end
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x63, 0x15, 0x20, 0x00, 0xF0, 									//0x01E0    adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-cmd15-end
	0x00, 0x70, 0xA2, 0x20, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x63, 0x00, 0xF0, 											//0x01F6   cmdA2-adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-end
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x63, 0x00, 0xF0, 										//0x020C    adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,																		//0x0220   cmdA2-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,																		//0x022C    adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,	//0x0236   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,	//0x026C   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      									//0x02A0   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      								//0x02C8   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      																			//0x02EE   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      																			//0x0308   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,																															//0x0320   cmdA2-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,																																//0x032C   adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,	//0x0336   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,	//0x036C   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      									//0x03A0   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      								//0x03C8   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      																			//0x03EE   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      																			//0x0408   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x00, 0xF0,																		//0x0420  cmdA2-adr_gen-cmd00-adr5-cmd32-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x00, 0xF0,																		//0x042C  adr_gen-cmd00-adr5-cmd32-end
	0xA2, 0x20, 0x31, 0x20, 0x00, 0xF0,																						//0x0436   cmdA2-cmd31-end
	0xA2, 0x20, 0x3F, 0x20, 0x00, 0xF0,																						//0x043C   cmdA2-cmd3F-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,					//0x0442  cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,					//0x0460  adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,									//0x047C  cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,									//0x0494  adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,														//0x04AA	  cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,														//0x04BC  adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x00, 0x60, 0x00, 0xF0,																						//0x04CC  adr_gen-dma-end
	0x00, 0x70, 0x00, 0x20, 0x50, 0x10, 0x00, 0x60, 0x00, 0xF0,																					//0x04D2  adr_gen-cmd00-dly50-dma-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x05, 0x20, 0x00, 0x32, 0xE0, 0x20, 0x50, 0x10, 0x00, 0x60, 0x00, 0xF0,														//0x04DC  adr_gen-cmd00-adr5-cmd05-adr2-cmdE0-dly50-dma-end
	0x00, 0x00, 0x00, 0xF0,																								//0x04EE  NOP
	0x00, 0x70, 0x00, 0xB0, 0x00, 0xF0,																								//0x04F2      adr_gen-FCOR
	0x00, 0xD0, 0x00, 0xF0,																								//0x04F8	Customize IBF restore/backup fpu
	0x00, 0x70, 0x00, 0xA0, 0x00, 0xF0,																						//0x04FC  	adr_gen-raw-end
	0x00, 0xC0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00,																								//0x0502	Soft bit correction
	0x00, 0x70, 0xC6, 0x20,/* 0x00, 0x00,*/ 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,												//0x050A
	0x00, 0x70, 0xC6, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,							//0x0516 cmdC6-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0xD1, 0x20, 0x20, 0x10, 0x70, 0x20, 0x20, 0x10, 0x20, 0x51, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,     // 0x0528    A2 60h ADDR(3) 60h D1h   60h ADDR(3) 60h D0h
	0x00, 0x70, 0x78, 0x20, 0x00, 0x33, 0x60, 0x20, 0x00, 0x33, 0xD1, 0x20, 0x20, 0x10, 0x70, 0x20, 0x20, 0x10, 0x20, 0x51, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,     // 0x0528    A2 60h ADDR(3) 60h D1h   60h ADDR(3) 60h D0h
	0xFC, 0x20, 0x00, 0xF0,             // 0x0544 cmd FC
#else
	0xFF, 0x20, 0x00, 0xF0,																								//0x0000    cmdFF-end
	0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0xF0,																					//0x0004	    cmd70-dly20-poll with Mask 0x40-end
	0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x20, 0x00, 0xF0,																			//0x000C    cmd70-dly20-poll with Mask 0x40-cmd00-end
	0x70, 0x20, 0x20, 0x10, 0x20, 0x51, 0x00, 0xF0,																					//0x0016	    cmd70-dly20-poll with Mask 0x20-end
	0x70, 0x20, 0x20, 0x10, 0x20, 0x51, 0x00, 0x20, 0x00, 0xF0,																			//0x001E    cmd70-dly20-poll with Mask 0x20-cmd00-end
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x40, 0x40, 0x00, 0xF0,																			//0x0028    cmd70-dly20-data(status) with Mask 0x40-end
	0x00, 0x00,																													//0x0032    empty
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x20, 0x40, 0x00, 0xF0,																			//0x0034    cmd70-dly20-data(status) with Mask 0x20-end
	0x71, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x40, 0x40, 0x00, 0xF0,																			//0x003E    cmd71-dly20-data(status) with Mask 0x40-end
	0x71, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x20, 0x40, 0x00, 0xF0,																			//0x0048    cmd71-dly20-data(status) with Mask 0x20-end
	0x00, 0x00,																													//0x0052    empty
	0x73, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x40, 0x40, 0x00, 0xF0,																			//0x0054    cmd73-dly20-data(status) with Mask 0x40-end
	0x73, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x20, 0x40, 0x00, 0xF0,																			//0x005E    cmd73-dly20-data(status) with Mask 0x20-end
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x0F, 0x40, 0x00, 0xF0,																			//0x0068    cmd70-dly20-data(status) with Mask 0x0F-end
	0x00, 0x00, 																													//0x0072    empty
	0x71, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x1F, 0x40, 0x00, 0xF0,																			//0x0074    cmd71-dly20-data(status) with Mask 0x1F-end
	0x73, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x1F, 0x40, 0x00, 0xF0,																			//0x007E    cmd73-dly20-data(status) with Mask 0x1F-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,																//0x0088    cmdA2-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,																		      //0x0094    adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,					//0x009E cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,					//0x00BC adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,					//0x00D8 cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,							//0x00EA adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
	0x00, 0x70, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x00, 0xF0,																	//0x00FA    cmdA2-adr_gen-cmd80-adr5-dly50-dma-end
	0x00, 0x70, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x00, 0xF0,																	//0x0108    adr_gen-cmd80-adr5-dly50-dma-end
	0x00, 0x70, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x11, 0x20, 0x00, 0xF0,	             													//0x0114    cmdA2-adr_gen-cmd80-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x11, 0x20, 0x00, 0xF0,	             													//0x0124    adr_gen-cmd80-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0,															//0x0132    cmdA2-adr_gen-cmd80-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0,															//0x0142    adr_gen-cmd80-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0xA2, 0x20, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x10, 0x20, 0x00, 0xF0,															//0x0150    cmdA2-adr_gen-cmd80-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x10, 0x20, 0x00, 0xF0,															//0x0160    adr_gen-cmd80-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0xA2, 0x20, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x11, 0x20, 0x00, 0xF0,															//0x016E    cmdA2-adr_gen-cmd81-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x11, 0x20, 0x00, 0xF0,															//0x017E    adr_gen-cmd81-adr5-dly50-dma-cmd11-end
	0x00, 0x70, 0xA2, 0x20, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0,															//0x0018C    cmdA2-adr_gen-cmd81-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0,															//0x019C    adr_gen-cmd81-adr5-dly50-dma-cmd15-end
	0x00, 0x70, 0xA2, 0x20, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x10, 0x20, 0x00, 0xF0,															//0x01AA    cmdA2-adr_gen-cmd81-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0x81, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0x10, 0x20, 0x00, 0xF0,															//0x01BA    adr_gen-cmd81-adr5-dly50-dma-cmd10-end
	0x00, 0x70, 0xA2, 0x20, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0, 									//0x01C8   cmdA2-adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-cmd15-end
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0, 									//0x01E0    adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-cmd15-end
	0x00, 0x70, 0xA2, 0x20, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x00, 0xF0, 											//0x01F6   cmdA2-adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-end
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x81, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x00, 0xF0, 										//0x020C    adr_gen-cmd11-cmd70-dly20-poll with Mask 0x40-cmd81-adr5-dma-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,																		//0x0220   cmdA2-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,																		//0x022C    adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,	//0x0236   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,	//0x026C   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      									//0x02A0   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      								//0x02C8   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      																			//0x02EE   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,                      																			//0x0308   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,																															//0x0320   cmdA2-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,																																//0x032C   adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,	//0x0336   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,	//0x036C   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      									//0x03A0   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      								//0x03C8   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      																			//0x03EE   cmdA2-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,                      																			//0x0408   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd31-end
	0x00, 0x70, 0xA2, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x00, 0xF0,																		//0x0420  cmdA2-adr_gen-cmd00-adr5-cmd32-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x00, 0xF0,																		//0x042C  adr_gen-cmd00-adr5-cmd32-end
	0xA2, 0x20, 0x31, 0x20, 0x00, 0xF0,																						//0x0436   cmdA2-cmd31-end
	0xA2, 0x20, 0x3F, 0x20, 0x00, 0xF0,																						//0x043C   cmdA2-cmd3F-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,					//0x0442  cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,					//0x0460  adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,									//0x047C  cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,									//0x0494  adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0xA2, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,														//0x04AA	  cmdA2-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0x30, 0x20, 0x00, 0xF0,														//0x04BC  adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmd30-end
	0x00, 0x70, 0x00, 0x60, 0x00, 0xF0,																						//0x04CC  adr_gen-dma-end
	0x00, 0x70, 0x00, 0x20, 0x50, 0x10, 0x00, 0x60, 0x00, 0xF0,																					//0x04D2  adr_gen-cmd00-dly50-dma-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x05, 0x20, 0x00, 0x32, 0xE0, 0x20, 0x50, 0x10, 0x00, 0x60, 0x00, 0xF0,														//0x04DC  adr_gen-cmd00-adr5-cmd05-adr2-cmdE0-dly50-dma-end
	0x00, 0x00, 0x00, 0xF0,																								//0x04EE  NOP
	0x00, 0x70, 0x00, 0xB0, 0x00, 0xF0,																								//0x04F2      adr_gen-FCOR
	0x00, 0xD0, 0x00, 0xF0,																								//0x04F8	Customize IBF restore/backup fpu
	0x00, 0x70, 0x00, 0xA0, 0x00, 0xF0,																						//0x04FC  	adr_gen-raw-end
#if TLC
	0x00, 0xC0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00,																								//0x0502	Soft bit correction
	0x00, 0x70, 0xC6, 0x20,/* 0x00, 0x00,*/ 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,												//0x050A
	0x00, 0x70, 0xC6, 0x20, 0x60, 0x20, 0x00, 0x33, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,							//0x0516 cmdC6-adr_gen-cmd60-adr3-adr_gen-cmd60-adr3-cmdD0-end
#if B0KB
	//Micron
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x00, 0xF0, 					//0x0528    adr_gen-cmd11-cmd70-dly18-poll with Mask 0x40-cmd80-adr5-dma-end
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0, 		//0x53C		adr_gen-cmd11-cmd70-dly18-poll with Mask 0x40-cmd80-adr5-dma-cmd15-end
	0x00, 0x00,																									// 0x552	dummy
	0xFC, 0x20, 0x00, 0xF0,																						// 0x554	cmdFC-end
	0xDA, 0x20, 0x00, 0xF0,																						// 0x558	cmdDA-end
	0xDF, 0x20, 0x00, 0xF0,																						// 0x55C	cmdDF-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x35, 0x20, 0x00, 0xF0,													// 0x560    adr_gen-cmd00-adr5-cmd35-end
	0x00, 0x70, 0x85, 0x23, 0x00, 0x35, 0x50, 0x10, 0x10, 0x20, 0x00, 0xF0,										// 0x56A    adr_gen-cmd85-adr5-dly50-cmd10-end
	0x00, 0x70, 0x85, 0x23, 0x00, 0x35, 0x50, 0x10, 0x11, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0xF0,	// 0x576    adr_gen-cmd85-adr5-dly50-cmd11-cmd70-dly18-poll with Mask 0x40-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x35, 0x20, 0x00, 0xF0,	 //0x0588   adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd35-end
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x10, 0x20, 0x00, 0xF0, 		//0x5BC		adr_gen-cmd11-cmd70-dly18-poll with Mask 0x40-cmd80-adr5-dma-cmd10-end
	0x00, 0x70, 0x33, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,														//0x5D2		adr_gen-cmd33-adr5-cmd30-end
	0x00, 0x70, 0x06, 0x20, 0x00, 0x35, 0xE0, 0x20, 0x00, 0xF0,														//0x5DC		adr_gen-cmd06-adr5-cmdE0-end

	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0x35, 0x20, 0x00, 0xF0,	//0x5E6		adr_gen-cmd00-adr5-cmd32-cmd70-dly20-poll with Mask 0x40-adr_gen-cmd00-adr5-cmd35-end
	//0x5FE
#if FORCE_AUTOPOL_TIMEOUT
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x00, 0x40, 0x00, 0xF0,										// 0x5FE cmd70-dly20-data(status) with Mask 0x00-end	(fot debug auto pol timeout)
	0x00, 0x70, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0xFC, 0x20, 0x00, 0xF0,							// 0x608 adr_gen-cmd80-adr5-dly50-dma-cmdFC-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0xFC, 0x20, 0x00, 0xF0										// 0x616 adr_gen-cmd00-adr5-cmdFC-end
	//0x620
#endif

#else
#if Hynix
	0xA2, 0x20, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD1, 0x20, 0x20, 0x10, 0x70, 0x20, 0x20, 0x10, 0x20, 0x51, 0x00, 0x70, 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0,     // 0x0528    A2 60h ADDR(3) 60h D1h   60h ADDR(3) 60h D0h
	0xFC, 0x20, 0x00, 0xF0,             // 0x0544 cmd FC
#else
	0x00, 0x20, 0x00, 0x35, 0xAE, 0x20, 0x00, 0xF0,						//Toshiba	 Only  0x0528  cmd00-adr5-cmdAE-end
	0x3D, 0x20, 0x00, 0xF0,									//Toshiba	 Only  0x0530  cmd3D-end
	0x05, 0x20, 0x00, 0x35, 0xE0, 0x20, 0x50, 0x10, 0x00, 0xA0, 0x00, 0xF0 				//Toshiba	 Only  0x0534  cmd05-adr5-cmdE0-dly50-RAW-end
	//0x0540 empty
#endif
#endif
#else
	0x00, 0xC0, 0x00, 0xF0, 																								//0x0502	Soft bit correction
#if MicronFlashOnly
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x00, 0xF0, 					//0x0506    adr_gen-cmd11-cmd70-dly18-poll with Mask 0x40-cmd80-adr5-dma-end
	0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0, 		//0x51A		adr_gen-cmd11-cmd70-dly18-poll with Mask 0x40-cmd80-adr5-dma-cmd15-end
	0xFC, 0x20, 0x00, 0xF0,																						// 0x530	cmdFC-end
	0xDA, 0x20, 0x00, 0xF0,																						// 0x534	cmdDA-end
	0xDF, 0x20, 0x00, 0xF0,																						// 0x538	cmdDF-end
#if FORCE_AUTOPOL_TIMEOUT
	0x70, 0x20, 0x20, 0x10, 0x00, 0x4E, 0x00, 0x40, 0x00, 0xF0,													// 0x53C cmd70-dly20-data(status) with Mask 0x00-end	(fot debug auto pol timeout)
	0x00, 0x70, 0x80, 0x23, 0x00, 0x35, 0x50, 0x10, 0x00, 0x61, 0xFC, 0x20, 0x00, 0xF0,							// 0x546 adr_gen-cmd80-adr5-dly50-dma-cmdFC-end
	0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0xFC, 0x20, 0x00, 0xF0													// 0x554 adr_gen-cmd00-adr5-cmdFC-end
#else
	0x00, 0x00, 0x00, 0x00																						//0x053C    empty
#endif
#else
	0x00, 0x20, 0x00, 0x35, 0xAE, 0x20, 0x00, 0xF0,						//Toshiba	 Only  0x0506  cmd00-adr5-cmdAE-end
	0x3D, 0x20, 0x00, 0xF0,									//Toshiba	 Only  0x050E  cmd3D-end
	0x05, 0x20, 0x00, 0x35, 0xE0, 0x20, 0x50, 0x10, 0x00, 0xA0, 0x00, 0xF0 				//Toshiba	 Only  0x0512  cmd05-adr5-cmdE0-dly50-RAW-end
	//0x051E empty
#endif
#endif
#endif
};

#endif

#if TLC_BICS2 || YMTC
#define WRITE_CMD 0x80
#else
#define WRITE_CMD 0x85
#endif

#if (!BURNER)
__attribute__((section(".ICODE.data"))) RamSEQs_T guwFpuEntry =
#else
__attribute__((section(".FPU2.data"))) RamSEQs_T guwFpuEntry =
#endif
{
	//===== Polling =====
	//Auto Poll 8bytes align(DAT, DAT RDY , DAT FAIL)
	//FPU_C70_DAT_MK40_MF0F[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x18), FPU_NOP(), FPU_NOP(), FPU_DAT_RD(), FPU_DAT_RDY(0x40), FPU_DAT_FAIL(0x0F),  FPU_END()
	},
	//FPU_C70_DAT_MK20_MF0F[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x18), FPU_NOP(), FPU_NOP(), FPU_DAT_RD(), FPU_DAT_RDY(0x20), FPU_DAT_FAIL(0x0F), FPU_END()
	},


	//FPU_C70_DAT_MK01[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x01), FPU_END(), FPU_END(), FPU_END(), FPU_END()
	},
	//FPU_C70_DAT_MK04[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x04), FPU_END(), FPU_END(), FPU_END(), FPU_END()
	},
	//FPU_C70_DAT_MK03[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x03), FPU_END(), FPU_END(), FPU_END(), FPU_END()
	},
	//FPU_C70_DAT_MK0C[8];
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_DAT_RD(), FPU_DAT_FAIL(0x0C), FPU_END(), FPU_END(), FPU_END(), FPU_END()
	},

	// ===== Base =====

	// ===== Erase =====


	// ===== Write =====
	// =====D3 Write (2P)=====
	// ===== First =====
	//DMA
	//FPU_C09_C01_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C01_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x01) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C09_C02_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C02_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x02) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C09_C03_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C15[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C10[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},


	// ===== Foggy =====
	//FPU_C0D_C01_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x01) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C0D_C02_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x02) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C0D_C03_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C15[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C10[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},


	// ===== Fine =====
	//FPU_NOP_C01_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x01) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_NOP_C02_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x02) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_NOP_C03_AG_C85_A5_DW[8];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W()  , FPU_END()
	},

	//FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C15[13];//帶Auto Poll
	{
#if TLC_BICS2
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
#else
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
#endif
	},

	//FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C10[13];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},

	// ===== Write =====
	// =====D3 Write (1P)=====
	// ===== First =====
	//DMA
	//FPU_C09_C01_AG_C85_A5_DW_C1A[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C09_C02_AG_C85_A5_DW_C1A[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C09_C03_AG_C85_A5_DW_C15[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//FPU_C09_C03_AG_C85_A5_DW_C10[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},


	// ===== Foggy =====
	//FPU_C0D_C01_AG_C85_A5_DW_C1A[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C0D_C02_AG_C85_A5_DW_C1A[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C0D_C03_AG_C85_A5_DW_C15[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},

	//FPU_C0D_C03_AG_C85_A5_DW_C10[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},


	// ===== Fine =====
	//FPU_NOP_C01_AG_C85_A5_DW_C1A[9];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_NOP_C02_AG_C85_A5_DW_C1A[9];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_NOP_C03_AG_C85_A5_DW_C15[9];
	{
#if TLC_BICS2
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
#else
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
#endif
	},

	//FPU_NOP_C03_AG_C85_A5_DW_C10[9];
	{
		FPU_ADR_GEN(), FPU_NOP()	  , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, WRITE_CMD), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},

	//Non-DMA
	//FPU_C09_C01_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C01_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x01) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C09_C02_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C02_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x02) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C09_C03_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x09), FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C15[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x15), FPU_END()
	},

	//FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C10[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x09), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x10), FPU_END()
	},

	// ===== Foggy =====
	//FPU_C0D_C01_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x01) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C0D_C02_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x02) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_C0D_C03_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x0D), FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C15[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x15), FPU_END()
	},

	//FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C10[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x0D), FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x10), FPU_END()
	},


	// ===== Fine =====
	//FPU_NOP_C01_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x01), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x01) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_NOP_C02_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x02), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x02) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x1A), FPU_END()
	},

	//FPU_NOP_C03_AG_C85_A5_NOP[7];
	{
		FPU_ADR_GEN(), FPU_NOP()	 , FPU_CMD(0x03), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_END()
	},

	//FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C15[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x15), FPU_END()
	},

	//FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C10[12];//帶Auto Poll
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_NOP()	  , FPU_CMD(0x03) , FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x10), FPU_END()
	},
#if  MicronFlashOnly || YMTC
	//FPU_CA2_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw[2]
	{
		FPU_DLY(0xF0) , FPU_CMD(0xDA)
	},
#else
	//FPU_CA2_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw[2]
	{
		FPU_DLY(0xF0) , FPU_CMD(0xA2)
	},
#endif

#if YMTC
	//FPU_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw[13]
	{
		FPU_CMD(0x00) , FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x30), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20), FPU_CMD(0x05) , FPU_ADDR(0x02), FPU_CMD(0xE0), FPU_DLY(0x20), FPU_RAW(), FPU_END()
		//FPU_ADR_GEN(),FPU_CMD(0x00) ,FPU_ADDR(0x05),FPU_CMD(0x30),FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),FPU_ADR_GEN(),FPU_CMD(0x05) , FPU_ADDR(0x05), FPU_CMD(0xE0),FPU_DLY(0x20),FPU_END()
	},
#else
	//FPU_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw[13]
	{
		FPU_DLY(0xF0), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20), FPU_CMD(0x05) , FPU_ADDR(0x05), FPU_CMD(0xE0), FPU_DLY(0x20), FPU_RAW(), FPU_END()
		//FPU_ADR_GEN(),FPU_CMD(0x00) ,FPU_ADDR(0x05),FPU_CMD(0x30),FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),FPU_ADR_GEN(),FPU_CMD(0x05) , FPU_ADDR(0x05), FPU_CMD(0xE0),FPU_DLY(0x20),FPU_END()

	},
#endif

#if YMTC
	//UWORD FPU_CDF_AG_C80_A6_DW[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0xDF), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},

	//UWORD FPU_CDF_AG_C85_A6_NOP[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0xDF), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_NOP(), FPU_END()
	},

	//UWORD FPU_C11_POL_MK40_AG_C80_A6_DW_C1A[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A) , FPU_END()
	},

	//UWORD FPU_C11_POL_MK40_AG_C80_A6_DW_C10[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10) , FPU_END()
	},

	//UWORD FPU_C11_POL_MK40_AG_C85_A6_NOP_C1A[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_NOP(), FPU_CMD(0x1A) , FPU_END()
	},

	//UWORD FPU_C11_POL_MK40_AG_C85_A6_NOP_C10[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x85), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_NOP(), FPU_CMD(0x10) , FPU_END()
	},

	//UWORD FPU_CDF_AG_C80_A5_DW_C1A[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0xDF), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x1A), FPU_END()
	},

	//UWORD FPU_CDF_AG_C80_A5_DW_C10[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0xDF), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x10), FPU_END()
	},

	// Softbit
	//FPU_AG_C00_A6_C05_A2_CE0[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0),	FPU_CMD(0x05) , FPU_ADDR(0x02), FPU_CMD(0xE0), FPU_END()
	},
	//FPU_AG_C00_A6_C34[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0),	FPU_CMD(0x34) , FPU_END()
	},
	//UWORD FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_CA2_C00_A5_C35[20];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0xDA) , FPU_CMD(0x00) , FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05), FOURTH_COL_ADDR_DIE(0), FPU_CMD(0x35), FPU_END()
	},

	//UWORD FPUEND[1];
	{
		FPU_END()
	}


#elif Hynix
	// ===== Read =====


	//0x00, 0x70, 0x05, 0x20, 0x00, 0x35, 0xE0, 0x20, 0x3C, 0x10, 0x00, 0x60, 0x00, 0xF0,
	//FPU_AG_C05_A5_CE0_DR[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x05) , FPU_ADDR(0x05), FPU_CMD(0xE0), FPU_DLY(0x3C), FPU_DMA_R(), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C30[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C30[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C03_AG_C00_A5_C30[8];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C31[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C31[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0
	//FPU_C03_AG_C00_A5_C31[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

#if Hynix
	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35[15];
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0xA2) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x20), FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x35), FPU_END()
	},
#endif

	// ===== Soft Bit ====
	//0x00, 0x70, 0x05, 0x20, 0x00, 0x35, 0xE0, 0x20, 0x00, 0xF0,
	//FPU_AG_C05_A5_CE0[5];
	{
		FPU_ADR_GEN(), FPU_CMD(0x05) , FPU_ADDR(0x05), FPU_CMD(0xE0), FPU_END()
	},
	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x3C, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C3C[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x3C, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C3C[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x3C, 0x20, 0x00, 0xF0
	//FPU_C03_AG_C00_A5_C3C[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},
	//0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0xAE, 0x20, 0x00, 0xF0
	//FPU_AG_C00_A5_CAE[5];
	{
		FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0xAE), FPU_END()
	},
#if Hynix
	//FPU_AG_C00_A5_C05_A2_CE0[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05),  FPU_CMD(0x05) , FPU_ADDR(0x02), FPU_CMD(0xE0), FPU_END()
	},
#endif
	// ===== Write =====
	//0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0,
	//FPU_AG_C11_C70_POL_MK40_C80_A5_DW_C15[11];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},
	//0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x00, 0xF0,
	//FPU_AG_C11_C70_POL_MK40_C80_A5_DW[10];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
#if Hynix3DV6 || Hynix3DV5
	//UWORD	FPU_C01_AG_C80_A5_DW;
	{
		FPU_ADR_GEN(), FPU_CMD(0x01), FPU_CMD(0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C01_AG_C81_A5_DW;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x01), FPU_CMD(0x81), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C01_AG_C81_A5_DW_C22;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x01), FPU_CMD(0x81), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x22), FPU_END()
	},
	//UWORD FPU_C01_AG_C80_A5_DW_C22;
	{
		FPU_ADR_GEN(), FPU_CMD(0x01), FPU_CMD(0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x22), FPU_END()
	},
	//UWORD			 FPU_C02_AG_C80_A5_DW;
	{
		FPU_ADR_GEN(), FPU_CMD(0x02), FPU_CMD(0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C02_AG_C81_A5_DW;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x02), FPU_CMD(0x81), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C02_AG_C81_A5_DW_C22;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x02), FPU_CMD(0x81), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x22), FPU_END()
	},
	//UWORD FPU_C02_AG_C80_A5_DW_C22;
	{
		FPU_ADR_GEN(), FPU_CMD(0x02), FPU_CMD(0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x22), FPU_END()
	},
	//UWORD			 FPU_C03_AG_C80_A5_DW;
	{
		FPU_ADR_GEN(), FPU_CMD(0x03), FPU_CMD(0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C03_AG_C81_A5_DW;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x03), FPU_CMD(0x81), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C03_AG_C81_A5_DW_C23;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x03), FPU_CMD(0x81), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x23), FPU_END()
	},
	//UWORD FPU_C03_AG_C80_A5_DW_C23;
	{
		FPU_ADR_GEN(), FPU_CMD(0x03), FPU_CMD(0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x23), FPU_END()
	},

	//UWORD			 FPU_C01_AG_C85_A5_NOP;
	{
		FPU_ADR_GEN(), FPU_CMD(0x01), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C01_AG_C85_A5_NOP;
	{

		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x01), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C01_AG_C85_A5_NOP_C22;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x01), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_CMD(0x22), FPU_END()
	},
	//UWORD			 FPU_C02_AG_C85_A5_NOP;
	{
		FPU_ADR_GEN(), FPU_CMD(0x02), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C02_AG_C85_A5_NOP;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x02), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_END()
	},
	//UWORD FPU_C11_POL_MK40_C02_AG_C85_A5_NOP_C22;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x02), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_CMD(0x22), FPU_END()
	},
	//UWORD			 FPU_C03_AG_C85_A5_NOP;
	{
		FPU_ADR_GEN(), FPU_CMD(0x03), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_END()
	},

	//UWORD FPU_C11_POL_MK40_C03_AG_C85_A5_NOP;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x03), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_END()
	},

	//UWORD FPU_C11_POL_MK40_C03_AG_C85_A5_NOP_C23;
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD(0x03), FPU_CMD(0x85), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_NOP(), FPU_CMD(0x23), FPU_END()
	},

	//UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30;
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},


	//UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30;
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30;
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31;
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31;
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31;
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_NOP_AG_C00_A5_C32_C70_POL_MK40_NOP_AG_C00_A5_C32_C70_POL_MK40_AG_NOP_C00_A5_C35
	{
		FPU_ADR_GEN(), FPU_CMD(0x78) , FPU_ADDR(0x03), FPU_CMD(0xA2) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_NOP() , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_NOP() , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40),        FPU_ADR_GEN(), FPU_NOP() ,     FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x35), FPU_END()
	},

	//UWORD FPU_C78_A3_C60_A3_CD3[7] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD3), FPU_END()
	},

	//UWORD FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD3[15] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD3), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD3), FPU_END()
	},

	//UWORD FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD3[25] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD3), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD3), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD3), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD3), FPU_END()
	},

	//UWORD FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD0[25] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD0), FPU_END()
	},

	//UWORD FPU_AG_C78_A3_CA2_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD0[] =
	{
		FPU_ADR_GEN(), FPU_CMD(0x78), FPU_ADDR(0x03), FPU_CMD(0xA2), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD1), FPU_DLY(0x20), FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_ADR_GEN(), FPU_CMD(0x60), FPU_ADDR(0x03), FPU_CMD(0xD0), FPU_END()
	},


#endif

	// ===== END =====
	//FPUEND[1]
	{
		FPU_END()
	}
#else
	// ===== Read =====


	//0x00, 0x70, 0x05, 0x20, 0x00, 0x35, 0xE0, 0x20, 0x3C, 0x10, 0x00, 0x60, 0x00, 0xF0,
	//FPU_AG_C05_A5_CE0_DR[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x05) , FPU_ADDR(0x05), FPU_CMD(0xE0), FPU_DLY(0x3C), FPU_DMA_R(), FPU_END()
	},

	//0x00, 0x70, 0x05, 0x20, 0xFF, 0x10, 0xFF, 0x10, 0xFF, 0x10, 0xFF, 0x10, 0xFF, 0x10, 0x00, 0x35, 0xE0, 0x20, 0x3C, 0x10, 0x00, 0x60, 0x00, 0xF0,
	//FPU_AG_DLY_C05_A5_CE0_DR[12];
	{
		FPU_ADR_GEN(), FPU_DLY(0xFF) , FPU_DLY(0xFF) , FPU_DLY(0xFF),  FPU_DLY(0xFF),  FPU_DLY(0xFF), FPU_CMD(0x05), FPU_ADDR(0x05), FPU_CMD(0xE0), FPU_DLY(0x3C), FPU_DMA_R(), FPU_END()
	},

	//0x00, 0x70, 0x05, 0x20, 0xFF, 0x10, 0xFF, 0x10, 0xFF, 0x10, 0xFF, 0x10, 0xFF, 0x10, 0x00, 0x35, 0xE0, 0x20, 0x3C, 0x10, 0x00, 0x60, 0x00, 0xF0,
	//FPU_AG_DLY_C00_A5_C05_A2_CE0_DR[14];
	{
		FPU_ADR_GEN(), FPU_DLY(0xFF) , FPU_DLY(0xFF) , FPU_DLY(0xFF),  FPU_DLY(0xFF),  FPU_DLY(0xFF), FPU_CMD(0x00), FPU_ADDR(0x05), FPU_CMD(0x05), FPU_ADDR(0x02), FPU_CMD(0xE0), FPU_DLY(0x3C), FPU_DMA_R(), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C30[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C30[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_C03_AG_C00_A5_C30[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C31[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C31[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x31, 0x20, 0x00, 0xF0
	//FPU_C03_AG_C00_A5_C31[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

#if (TLC_BICS2 & (!BURNER))
	//FPU_C5D_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C01_C00_A5_C30[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_C5D_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C02_C00_A5_C30[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_C5D_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C03_C00_A5_C30[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_C5D_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C01_C00_A5_C31[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//FPU_C5D_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C02_C00_A5_C31[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//FPU_C5D_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C03_C00_A5_C31[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//FPU_C5D_C01_AG_C00_A5_C30[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_C5D_C02_AG_C00_A5_C30[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_C5D_C03_AG_C00_A5_C30[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_C5D_C01_AG_C00_A5_C31[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//FPU_C5D_C02_AG_C00_A5_C31[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//FPU_C5D_C03_AG_C00_A5_C31[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},
#endif

#if Hynix
	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x32, 0x20, 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x30, 0x20, 0x00, 0xF0,
	//FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35[13];
	{
		FPU_ADR_GEN(), FPU_CMD(0xA2) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x20), FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x35), FPU_END()
	},
#endif

	// ===== Soft Bit ====
	//0x00, 0x70, 0x05, 0x20, 0x00, 0x35, 0xE0, 0x20, 0x00, 0xF0,
	//FPU_AG_C05_A5_CE0[5];
	{
		FPU_ADR_GEN(), FPU_CMD(0x05) , FPU_ADDR(0x05), FPU_CMD(0xE0), FPU_END()
	},
	//0x00, 0x70, 0x01, 0x20, 0x00, 0x20, 0x00, 0x35, 0x3C, 0x20, 0x00, 0xF0,
	//FPU_C01_AG_C00_A5_C3C[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},

	//0x00, 0x70, 0x02, 0x20, 0x00, 0x20, 0x00, 0x35, 0x3C, 0x20, 0x00, 0xF0,
	//FPU_C02_AG_C00_A5_C3C[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},

	//0x00, 0x70, 0x03, 0x20, 0x00, 0x20, 0x00, 0x35, 0x3C, 0x20, 0x00, 0xF0
	//FPU_C03_AG_C00_A5_C3C[6];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},
	//FPU_C01_AG_CC2_C00_A5_C3C[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x01) , FPU_CMD(0xC2) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},
	//FPU_C02_AG_CC2_C00_A5_C3C[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x02) , FPU_CMD(0xC2) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},
	//FPU_C03_AG_CC2_C00_A5_C3C[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x03) , FPU_CMD(0xC2) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x3C), FPU_END()
	},
	//0x00, 0x70, 0x00, 0x20, 0x00, 0x35, 0xAE, 0x20, 0x00, 0xF0
	//FPU_AG_C00_A5_CAE[5];
	{
		FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0xAE), FPU_END()
	},
#if Hynix
	//FPU_AG_C00_A5_C05_A2_CE0[7];
	{
		FPU_ADR_GEN(), FPU_CMD(0x00) , FPU_ADDR(0x05),  FPU_CMD(0x05) , FPU_ADDR(0x02), FPU_CMD(0xE0), FPU_END()
	},
#endif
	// ===== Write =====
	//0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x15, 0x20, 0x00, 0xF0,
	//FPU_AG_C11_C70_POL_MK40_C80_A5_DW_C15[11];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_CMD(0x15), FPU_END()
	},
	//0x00, 0x70, 0x11, 0x20, 0x70, 0x20, 0x20, 0x10, 0x40, 0x51, 0x80, 0x23, 0x00, 0x35, 0x3C, 0x10, 0x00, 0x61, 0x00, 0xF0,
	//FPU_AG_C11_C70_POL_MK40_C80_A5_DW[10];
	{
		FPU_ADR_GEN(), FPU_CMD(0x11) , FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x40), FPU_CMD_DQS(0x01, 0x01, 0x80), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_DMA_W(), FPU_END()
	},
	//FPU_AG_CA2_C80_A5_C10[9];
	{
		FPU_ADR_GEN(), FPU_CMD(0xA2), FPU_DLY(0x18), FPU_CMD(0x80), FPU_DLY(0x18), FPU_ADDR(0x05), FPU_DLY(0x3C), FPU_CMD(0x10), FPU_END()
	},

#if ENABLE_RESET_RETRY_LEVEL
	// FPU_CEF_A89_D00_D00_D00_D00_C70_POL_MK20_CEF_A8A_D00_D00_D00_D00_C70_POL_MK20[24]
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_CMD(0xEF), FPU_ADDR_DAT(0x01, 0x89),  FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00),
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_CMD(0xEF), FPU_ADDR_DAT(0x01, 0x8A),  FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00),
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_END()
	},
	// FPU_CEF_A12_D00_D00_D00_D00_C70_POL_MK20_CEF_A13_D00_D00_D00_D00_C70_POL_MK20[24]
	{
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_CMD(0xEF), FPU_ADDR_DAT(0x01, 0x12),  FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00),
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_CMD(0xEF), FPU_ADDR_DAT(0x01, 0x13),  FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00), FPU_DAT_WR(0x00),
		FPU_CMD(0x70), FPU_DLY(0x20), FPU_POL(0x20),
		FPU_END()
	},
	//FPU_C5C_CC5_C55_A04_D00_C55_A05_D00_C55_A06_D00_C55_A07_D00_C55_A0D_D00[18]
	{
		FPU_CMD(0x5C), FPU_CMD(0xC5), FPU_CMD(0x55), FPU_ADDR_DAT(0x01, 0x04), FPU_RTY_DAT(0x00), FPU_CMD(0x55), FPU_ADDR_DAT(0x01, 0x05), FPU_RTY_DAT(0x00), FPU_CMD(0x55), FPU_ADDR_DAT(0x01, 0x06), FPU_RTY_DAT(0x00),
		FPU_CMD(0x55), FPU_ADDR_DAT(0x01, 0x07), FPU_RTY_DAT(0x00), FPU_CMD(0x55), FPU_ADDR_DAT(0x01, 0x0D), FPU_RTY_DAT(0x00), FPU_END()
	},

#if (!TLC)
	//FPU_C26_C5D_AG_C00_A5_C32_C70_POL_MK40_AG_C26_C5D_C00_A5_C30[16];
	{
		FPU_ADR_GEN(), FPU_CMD(0x26), FPU_CMD(0x5D) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x26), FPU_CMD(0x5D) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_PTR_AG_C26_C5D_C00_A5_C30[7]
	{
		FPU_ADR_GEN(), FPU_CMD(0x26), FPU_CMD(0x5D), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_PTR_AG_C26_C5D_C00_A5_C31[7]
	{
		FPU_ADR_GEN(), FPU_CMD(0x26), FPU_CMD(0x5D), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

	//FPU_C5D_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C00_A5_C30[14];
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x32), FPU_CMD(0x70), FPU_DLY(0x18), FPU_POL(0x40), FPU_ADR_GEN(), FPU_CMD(0x5D) , FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_PTR_AG_C5D_C00_A5_C30[6]
	{
		FPU_ADR_GEN(), FPU_CMD(0x5D), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x30), FPU_END()
	},

	//FPU_PTR_AG_C5D_C00_A5_C31[6]
	{
		FPU_ADR_GEN(),  FPU_CMD(0x5D), FPU_CMD(0x00) , FPU_ADDR(0x05), FPU_CMD(0x31), FPU_END()
	},

#endif
#endif
	// ===== END =====
	//FPUEND[1]
	{
		FPU_END()
	}

#endif

};

#if 0
void flaPrintPC()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	asm("mov %0, a0" : "+a" (VT->gulErrorDumpPC));
	UartString("\nPC:");
	UartLongHex(VT->gulErrorDumpPC);
}
#endif

UWORD flaReadFailStatusFPUPointer(UBYTE ubgFQLinkIndex)
{
	UWORD uwFPU = FPU_PTR_C70_DAT_MK0F;

#if (MicronFlashOnly)
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	if (FQ->ubFJob == BYTE_FJOB_WRITE) {
		if (FQ->ubCheckStatusPhase != STATUS_CHECK_ONE_FQ) {
			uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK03);
		}
		else {
			uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK01);
		}
		//UartString("\r\nC W ");
	}
	else if ((FQ->ubFJob == BYTE_FJOB_ERASE_BLOCK) || (FQ->ubFJob == BYTE_FJOB_ERASE_UNIT)) {
		uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK01);
		//UartString("\r\nC E ");
	}
	//UartWordHex(uwFPU);
#elif (YMTC)
	//Using Original FPU
#elif (Hynix)
	//Using Original FPU
#else
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];

	if (FQ->ubFJob == BYTE_FJOB_WRITE) {
		UBYTE ubD1Unit = 0;

		if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
			ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
		}

		if (TLC & ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK0C);
			}
			else {
				uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK03);
			}
		}
		else {
			uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK03);
		}
	}
	else if ((FQ->ubFJob == BYTE_FJOB_ERASE_BLOCK) || (FQ->ubFJob == BYTE_FJOB_ERASE_UNIT)) {
		if (TLC & ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0)))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK04);
			}
			else {
				uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK01);
			}
		}
		else {
			uwFPU = RAM_SEQ_PTR16(FPU_C70_DAT_MK01);
		}
	}
#endif
	return uwFPU;
}

void flaDumpError(UBYTE ubForceBootCode, UBYTE ubSaveVTSerialNumber)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (ubForceBootCode) {
		VT->gubSaveVTSerialNumber = ubSaveVTSerialNumber;
		mForceSramToFlash((U32)VT, (U32)gubPlanesPerVT * gub4kEntrysPerPlane * 4096, 0x111111); // VT, D1 Link, Free Block Table
		mForceSramToFlash(0x5c0c8000, 65536, 0x222222); // DCCM
#if ENABLE_PFAIL && DEBUG_ULTRAMLC_POWER_FAIL
		mForceSramToFlash((U32)trace_arr, 4096, 0x333333);
#else
		mForceSramToFlash((U32)L2PTable, (U32)gub4kEntrysPerPlane * 4096, 0x333333); // L2P
#endif
		mForceSramToFlash((U32)gulGRTable, (GRTableSize + TempGRTableSize), 0x555555); // GR
		mForceSramToFlash(DUMP_SRAM0_BASE, DUMP_SRAM0_LENGTH, 0x666666);
		mForceSramToFlash((U32)guwRUTBASE, RUTSize, 0x777777);
		mForceSramToFlash((U32)guwL2P_InRamIndex, 120 * 1024, 0x888888);
		mForceSramToFlash((U32)gulL2P_InRamInverse, 64 * 1024, 0x999999);
		flaLEDBlink(1);
		//Vender_Isp_Jump(0xFF);
	}
	else {

		if (gubMPRW) {
			APU_Reset();
			VT->gulMPRWCE_FailMap = guoCE_FailMap;
			VT->gubSaveVTSerialNumber = ubSaveVTSerialNumber;
			mForceSramToFlash((U32)VT, (U32)gubPlanesPerVT * gub4kEntrysPerPlane * 4096, 0x111111); // VT, D1 Link, Free Block Table
			mForceSramToFlash(0x5c0c8000, 65536, 0x222222); // DCCM
#if ENABLE_PFAIL && DEBUG_ULTRAMLC_POWER_FAIL
			mForceSramToFlash((U32)trace_arr, 4096, 0x333333);
#else
			mForceSramToFlash((U32)L2PTable, (U32)gub4kEntrysPerPlane * 4096, 0x333333); // L2P
#endif
			mForceSramToFlash((U32)gulGRTable, (GRTableSize + TempGRTableSize), 0x555555); // GR
			mForceSramToFlash(DUMP_SRAM0_BASE, DUMP_SRAM0_LENGTH, 0x666666);
			mForceSramToFlash((U32)guwRUTBASE, RUTSize, 0x777777);

		}
		//================= CLEAR ALL MTQ =================
		UBYTE ubChannel;

		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
			// force empty to avoid program parity clear interrupt vector enable bit bug
			gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
		}


		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
				gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
			}

			// Clear all MTQ in channel
			gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
			gFREG[FCTLL_MT_TRIG].B.B1 = 0;

			gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug
		}

		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
			// force empty to avoid program parity clear interrupt vector enable bit bug
			gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
			gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
		}

		//=================== FW RESET  ====================
		FCONL[FCONL_FW_RESET] = 0;
		while (FCONL[FCONL_FW_RESET] != 0);
		FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;
		//=================== RESET EACH CE ====================
		UBYTE ubCE;
		for (ubCE = 0; ubCE < gubCENumber; ubCE++) {
			UBYTE ubTrueCE = ubCE;
			ubChannel = (ubCE & gubPlanesPerBurstMask);
			volatile REG_t *FREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
			//Enable CE
			M_VirtualToPhysicalCE(ubTrueCE);
			FCONL[FCONL_FCE_ENB] = 0x00000000;
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][(ubTrueCE / gubPlanesPerBurst)]);
			}
			else {
				FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubTrueCE / gubPlanesPerBurst); // Enable selected CE
			}

			//reset
#if MicronFlashOnly || YMTC
			if ((guoFlashUseType.B.btNVDDR2 || guoFlashUseType.B.btNVDDR)) {
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFC;
			}
			else {
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
			}
#elif Hynix
			if (guoFlashUseType.B.btToggle) {
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFC;
			}
			else {
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
			}
#else
			FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
#endif

			FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((FREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			//wait buy
			//FREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			FREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
			FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x6F_FPU_POL_flaDumpError, FREG, CHECK_FPU_POL_TIMEOUT);
			}
			else {
				while ((FREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}
		}
		FCONL[FCONL_FCE_ENB] = 0x00000000;
		if (gubSSD_Init_State.B.ubState >= BYTE_INIT_VT_RUT_VTC_DONE) {
			ftlSaveVT(1, ubSaveVTSerialNumber);
		}
		if ((ubSaveVTSerialNumber == VT_ASSERT_TABLEUNIT_READUNC_0x6F) ||
		        (ubSaveVTSerialNumber == VT_ASSERT_SEQUENCE_MARK_STATUS_BAD_0xB2)) {
			M_DebugGPIO8_OFF_LED();
			Vender_Isp_Jump(0xFF);
		}
		else {
			Vender_Isp_Jump(0xFE);
		}
	}
}

void flaLEDBlink(U32 ulFailCondition)
{
#if(!RDT)
	gulLEDBlinkConditon = ulFailCondition;
	if (ENABLE_DEBUG_NO_HANG) {
		gubTest = 0;
	}
	else {
		gubTest = 1;
	}
	UartString("\nError!");
	while (gubTest) {
		if (gubTest == 2) {
			M_SwitchTask();
		}
		else if (gubTest == 0xCC) {
#if(!RDT)
			if (!BURNER) {
				gubTemp = BIT_Load_P2L_Check_VC | BIT_Scan_L2P_Check_VC;
				ftlCheckVC(gubTemp);
			}
#endif
			gubTest = 1;
		}
		//M_DebugGPIO8_High();
		M_DebugGPIO8_OFF_LED();
		idle(0x400000);
		//M_DebugGPIO8_Low();
		M_DebugGPIO8_ON_LED();
		idle(0x400000);
	}
#endif
}
#if !BURNER
__attribute__((section(".ICODE.text"))) __attribute__ ((always_inline)) inline SBYTE TemperatureRead()
{

	SBYTE sbTemperature = 33;
	UBYTE ubAddrSelect = 0x48;    /// TMP101 slave addr
	UBYTE ubTryThermoSensorCnt = 0;
	UBYTE ubMode = 0;

	if (ubMode) {
		sbTemperature = THEMAL_SENSOR_NOT_EXIST;
	}
	SYS0L[SYS0L_SYS_MUX_CTRL0] &= ~(BIT27);
	SYS0L[SYS0L_SYS_MUX_CTRL0] |= BIT26;



	SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] &= 0xFFFFFF00;
	SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] |= (U32)(ubAddrSelect);

MARK_RetryI2CThermoSensor:
	SYS1L[SYS1L_I2C_BUF0_REG] = 0; //clear buffer 0
	SYS1L[SYS1L_I2C_GP1_OP_REG] = 0;//read only

	////the master device must issue an Hs-mode master
	////code (00001XXX) as the first byte after a START condition
	////to switch the bus to high-speed operation

	SYS1L[SYS1L_I2C_GP1_CTRL] = ((1 << 16) | (2 << 8) | (0 << 4) | I2C_READ); // OP_CNT=1  DATA_CNT=2 BUFF=0 DIR=read | SPEED;
	SYS1L[SYS1L_I2C_GP1_CTRL] |= I2C_START; //Trigger

	while ((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1); //Victor: if there is no I2C slave, maybe it will stuck at here.
	if ((SYS1L[SYS1L_I2C_GP1_NACK] & GP1_ANACK) == 0) { //GP1_ANACK indicates the device does not response to address ACK
		if (ubMode) {
			sbTemperature = THEMAL_SENSOR_EXIST; // 0x01 mean thermal sensor exist
		}
		else {
			U32 ulNACKTimer = gulOperationTime;
			U32 ulCTimer;
			while (SYS1L[SYS1L_I2C_GP1_NACK] & GP1_DNACK) { // GP1_DNACK indicates the device does not response to data ACK.
				//Timeout Issue
				xthal_dcache_line_invalidate(&gulOperationTime); //Avoid Compiler Optimize
				ulCTimer = gulOperationTime;
				if (ulCTimer < ulNACKTimer) {
					ulCTimer += (0xFFFFFFFF - ulNACKTimer);
				}
				else {
					ulCTimer -= ulNACKTimer;
				}
				if (ulCTimer > TEMPERATURE_NACK_TIMEOUT) {
					//NACK Timeout -> Record VT & Set Temp Fail Flag
					gubGetTempFailFlag = 1;
#if (!BURNER)
					if (gubSSD_Init_State.B.ubState >= BYTE_INIT_VT_RUT_VTC_DONE) {
						VT->guwTempNACKTimeoutCount++;
					}
					else {
						//VT->guwTempNACKTimeoutCount++ after gubSSD_Init_State.B.ubState >= BYTE_INIT_VT_RUT_VTC_DONE
						gubGetTempFailFlag = 2;
					}
#endif
					//return Error Temp
					return TEMPERATURE_NACK_FAIL_RETURN_VALUE;  // -128
				}
			}
			sbTemperature = SYS1B[SYS1L_I2C_BUF0_REG << 2];
		}
	}
	else {
		if ( ubTryThermoSensorCnt == 0) {
			ubTryThermoSensorCnt++;
			ubAddrSelect = 0x4A;      ///LM75BD  slave addr
			SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] &= 0xFFFFFF00;
			SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] |= (U32)(ubAddrSelect);
			goto MARK_RetryI2CThermoSensor;
		}
		else {
			if (ubMode) {
				sbTemperature = THEMAL_SENSOR_NOT_EXIST; // 0x00 mean thermal sensor doesn't exist
			}
			else {
				sbTemperature = 33;//if there is no thermal sensor, return 33C
			}
		}
	}
	return sbTemperature;
}

__attribute__((section(".ICODE.text"))) __attribute__ ((always_inline)) inline void TemperatureControl(void)
{
	// if (gFQI.ubFQLinkNumber >= 4)
	// {
	SBYTE sbTemperature;
	if (guwTemperatureDetectCnt > (gubTemperatureCooling ? 100 : 100)) { // Detect Temperature 100 times, if the temperature is too high, and it would start temperaturecooling to make the performance slow.
		if (gubGetTempFailFlag) {
#if (!BURNER)
			if (gubGetTempFailFlag == 2 && gubSSD_Init_State.B.ubState >= BYTE_INIT_VT_RUT_VTC_DONE) {
				VT->guwTempNACKTimeoutCount++;
				gubGetTempFailFlag = 1;
			}
#endif
			idle(gulDelayCnt[1]);  // Get Temp Fail -> Slow down the speed (Stage2)
		}
		else {
			sbTemperature = TemperatureRead();
			if (sbTemperature >= (SBYTE)gubDropSpeedTemperature[0]) {
				gubTemperatureCooling = 1;
				if (sbTemperature < (SBYTE)gubDropSpeedTemperature[1]) {
					idle(gulDelayCnt[0]);  // Slow down the speed (Stage1)
				}
				else {
					idle(gulDelayCnt[1]);  // Slow down the speed (Stage2)
				}
			}
			else {
				gubTemperatureCooling = 0;
			}
		}
		guwTemperatureDetectCnt = 0;
	}
	else {
		guwTemperatureDetectCnt++;
	}
}
#endif

void flaTask()
{
#if(!RDT)
	// initial phase...
	UBYTE ubCEIndex, ubi, ubCFQIndex, ub4kIndex, ubgFQLinkIndex;
	UBYTE ubPreviousFQIndex, ubgPreviousFQLinkIndex;
	UBYTE ubNextFQIndex, ubgNextFQLinkIndex;
	UBYTE ubBurstBank;
	UBYTE ubCEXferPhase;
	UBYTE ubRemoveFQCount;
	UBYTE ubFQIndex;
	UBYTE ubDebugCFQIndex;
	UBYTE ubRemainQCnt;
	UBYTE ubMTQOverLimit;
	FlashQueue_t *FQ;
	FlashQueue_t *PreviousFQ;

	//U32 ubFQDoingNumber[MAX_CE + 1];
	UBYTE ubWaitFQCnt[MAX_CE];
	UBYTE ubSequentialFQNumBackup[MAX_CE];
	UBYTE ubBurstBankBackup[MAX_CE];
	UBYTE ubPreviousFQIndexBackup[MAX_CE];
	UBYTE ubgPreviousFQLinkIndexBackup[MAX_CE];
	UBYTE ubCFQIndexBackup[MAX_CE];
	UBYTE ubgFQLinkIndexBackup[MAX_CE];
	UBYTE ubCEXferPhaseBackup[MAX_CE];
	U32 ulFEntry;
	UWORD uwD3SuspendFPU;
	UBYTE usD3SuspendPlane;

#if ENABLE_FQ_ALIGNMENT
	U32 ulLastEntry[MAX_CE + 1];
#endif

#if (!B0KB)
	// For support 2 plane for 5CE/10CE, CE number will be scanned in early stage, so B0KB don't need to set gubCENumber here.
	gubCENumber = MAX_CE; // 怕變數沒有做好initial就switch task, 先給max CE
#endif

	memset(&gFQI, 0, sizeof(gFQI));

	for (ubCEIndex = 0; ubCEIndex < (MAX_CE + 1); ubCEIndex++) {
#if ENABLE_FQ_ALIGNMENT
		ulLastEntry[ubCEIndex] = 0;
#endif
		if (ubCEIndex == MAX_CE) {
			continue;
		}
		gubFQNeedExecuteRightNow[ubCEIndex] = 0;
		gubFQNotSequential[ubCEIndex] = 0;
		gubToCheckNextCmdSequence[ubCEIndex] = 0;
		gulNotFinishPhase[ubCEIndex] = 0;
		gubNoFQIn[ubCEIndex] = 0;
		gubMTDepthPushNumber[ubCEIndex] = 0;
		ubWaitFQCnt[ubCEIndex] = 0;
		ubSequentialFQNumBackup[ubCEIndex] = 0;
		ubBurstBankBackup[ubCEIndex] = 0;
		ubPreviousFQIndexBackup[ubCEIndex] = 0;
		ubgPreviousFQLinkIndexBackup[ubCEIndex] = 0;
		ubCEXferPhaseBackup[ubCEIndex] = 0;
		gubMicronMode[ubCEIndex] = MICRON_MLC;
		gFQI.ulPreviousEntry[ubCEIndex] = 0xFFFFFFFF;


	}
	for (ubi = 0; ubi < gFQ_NUM; ubi++) {
		gFQI.gFQLink[ubi].ubNext = (ubi + 1)&gFQ_NUM_MASK;
		gFQI.gFQLink[ubi].ubPrevious = (ubi - 1)&gFQ_NUM_MASK;
		gFQI.gFQLink[ubi].ubRelativeDepth = 0xFF;
		gFQI.gFQLink[ubi].ubRemainderDepth = 0xFF;
		gFQI.gFQLink[ubi].ubRedundantDepth = 0xFF;
#if MicronFlashOnly
		gFQI.gFQLink[ubi].ubInExitSLCDepth = 0xFF;
#endif
#if ENABLE_RESET_RETRY_LEVEL
		gFQI.gFQLink[ubi].ubBicsRetryLevelDepth = 0xFF;
#endif
		gFQI.gFQLink[ubi].ubWritePhaseRedoCmd = MARK_FPU_C80_C10C15;
		gFQI.gFQLink[ubi].btGenFail = (ENABLE_DEBUG_HW_FORCEREADFAIL || ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL);
		gFQI.gFQLink[ubi].ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;
	}
	M_SwitchTask();

	while (1) {
#if (!BURNER)
		if (gubEnableTemperatureControl) {
			TemperatureControl();
		}

#endif
		UBYTE ubFQCount = 0;
	Mark_AssignFQToEachCE:

		for (ubgFQLinkIndex = gFQI.ubFQLinkFirst; ubFQCount < gFQI.ubFQLinkNumber; (ubgFQLinkIndex = FQ->ubNext), ubFQCount++) {
			FQ = &gFQI.gFQLink[ubgFQLinkIndex];
			ubCEIndex = FQ->ubCEIndex ;
			M_VirtualToPhysicalCE(ubCEIndex);       //FQ->ubCEIndex : Virtual CE ; ubCEIndex(Control QueueIndex) :Physical CE

			//if (FQ->btUNServed && (ubFQDoingNumber[ubCEIndex] < FQ_NUM)) {
			UBYTE ubCELimit = FQ_NUM;
#if Hynix3DV6
			if (gubCENumber > 8) {
				ubCELimit = 8;
			}
#endif

#if ENABLE_FQ_ALIGNMENT
			if ((FQ->btUNServed && ((FQ->ubFQPhase == BYTE_WAIT_DMA) || (gFQI.ubFQDoingNumber[ubCEIndex] < ubCELimit)) && ((ulLastEntry[ubCEIndex] & DOING_FQ_ALIGNMENT) == 0)))
#else
			if ((FQ->btUNServed && ((FQ->ubFQPhase == BYTE_WAIT_DMA) || (gFQI.ubFQDoingNumber[ubCEIndex] < ubCELimit))))
#endif
			{
				M_AssertError((FQ->ubFQPhase != 0) && (FQ->btNeedStopDMA == 0));
				if (FQ->ubFJob == BYTE_FJOB_READ) {
#if ENABLE_FQ_ALIGNMENT
					if (ulLastEntry[ubCEIndex] & CHECK_FQ_ALIGNMENT) {
						if (((ulLastEntry[ubCEIndex] & (~CHECK_FQ_ALIGNMENT)) + gub4kEntrysPerPlane) == FQ->ulFEntry) {
							if (((FQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask) == gubBurstsPerBank - 1) {
								// last non-aligned seq burst bank, lock local FQ of this CE
								ulLastEntry[ubCEIndex] = DOING_FQ_ALIGNMENT;
							}
							else {
								// keep checking alignment
								ulLastEntry[ubCEIndex] = (CHECK_FQ_ALIGNMENT | FQ->ulFEntry);
							}
						}
						else {
							// not seq FQ, doesn't need to Check
							ulLastEntry[ubCEIndex] = 0;
						}
					}
#endif
					if (FQ->FQPara.B.btUserData) {
						ULLONG uoBufferFlag = 0;
						UBYTE ubIsBreakByCheckOrder = 0;

						for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
							UBYTE ub4kBufferIndex = (FQ->ubBufferInedex_R + ub4kIndex) & gulBuffer2Read4KMask;
							if (gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] != FQ->ubUseBufferOrder[ub4kIndex]) {
								//叫到號碼才能trigger Data!!
								ubIsBreakByCheckOrder = 1;
								break;
							}

							if (gubCheckBufferFlag && guoFWRead4kBufferFlag & ((ULLONG)BIT0 << ub4kBufferIndex)) {
								//ubIsBreakByFWRead4kBufferFlag = 1;
								break;
							}
							uoBufferFlag |= ((ULLONG)BIT0 << ub4kBufferIndex);
						}
						if (ub4kIndex != FQ->ubL4kNum) {
#if (!BURNER)
							if (ENABLE_PREFETCH_FQ && gubSeqR && (gubFQOutOfOrder == 0) && ((VT->gulFTLState.All & FTL_STATE_CleanGR_GC_WL) == 0)) {
								if (FQ->btNeedStopDMA == 0) {	// 還不能DMA的FQ只在第一次新增
									gFQI.ubFQIndex[ubCEIndex][(gFQI.ubFQFirst[ubCEIndex] + gFQI.ubFQDoingNumber[ubCEIndex])&FQ_NUM_MASK] = ubgFQLinkIndex;
									gFQI.ubFQDoingNumber[ubCEIndex] ++;
									M_AssertError(gFQI.ubFQDoingNumber[ubCEIndex] > FQ_NUM );
									FQ->btNeedStopDMA = 1;
									M_AssertError((FQ->ubFQPhase != 0) &&  (FQ->ubFQPhase != BYTE_WAIT_DMA));
#if B0KB
									if ( FQ->ubD3WriteMode & D3_SUSPEND_EN ) {
										gubD1D3CEPushFQ[FQ->ubCEIndex]--;
									}
#endif
								}
							}
#endif
							gubTemp = 0;
							continue;
						}
						else {
							if (gubCheckBufferFlag) {
								guoFWRead4kBufferFlag |= uoBufferFlag;
							}
							if (ENABLE_PREFETCH_FQ) {
								if (FQ->btNeedStopDMA) {	// 本來不能DMA的FQ不再被卡住
									FQ->btUNServed = 0;
									FQ->btNeedStopDMA = 0;
									continue;
								}
							}
							//FQ->btNeedStopDMA = 0;
						}

					}
					else if (FQ->FQPara.B.btHandleTableFlag && gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btTableInRAMFlag) {
						continue;
					}
				}
				else if (ENABLE_FIX_2PARITIES_CROSS_CH_BUG && (FQ->ubFJob == BYTE_FJOB_WRITE) && (FQ->btParity)) {
					if (FQ->ubRSFrameIndex < Table_RSFrameStartIndex) {
						if (guwParityDepth[FQ->ubRSFrameIndex] != 0xFFFF) {
							UBYTE ubNeedWaitTheOtherParityDone;
							ubNeedWaitTheOtherParityDone = 1;
							ubBurstBank = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
							if (ENABLE_PLANE_CH_CE) {
								if ( FQ->btRSProg2ndParity) {
									if (ubBurstBank) {    //  2nd parity落在planeA才會跟1st parity不同CH
										ubNeedWaitTheOtherParityDone = 0;
									}
								}
								else {
									if (ubBurstBank != (gubBurstsPerBank - 1) ) {
										ubNeedWaitTheOtherParityDone = 0; // 1st partiy落在last plane才會跟2nd parity不同CH
									}
								}
							}
							if (ubNeedWaitTheOtherParityDone) {
								if ((gubINTTable[guwParityDepth[FQ->ubRSFrameIndex]].B.btAll_Done == 0) && (guwRSFrameDoneCount[FQ->ubRSFrameIndex] == 0)) {
									continue;
								}
							}
							guwParityDepth[FQ->ubRSFrameIndex] = 0xFFFF;
						}
						else {
							guwParityDepth[FQ->ubRSFrameIndex] = FQ->ubRelativeDepth;
						}
					}
				}
				else if (FQ->ubFJob == BYTE_FJOB_SETVALUE) {
					if (FQ->FQPara.B.btRWTableUnit) {
						if (FQ->FQPara.B.btHandleTableFlag && gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btTableInRAMFlag) {
							continue;
						}
					}
					else if (FQ->FQPara.B.btUserData) {
						ULLONG uoBufferFlag = 0;
						for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
							UBYTE ub4kBufferIndex = (FQ->ubBufferInedex_R + ub4kIndex) & gulBuffer2Read4KMask;
							if (gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] != FQ->ubUseBufferOrder[ub4kIndex]) {
								//叫到號碼才能trigger Data!!
								break;
							}
							if (gubCheckBufferFlag  && (guoFWRead4kBufferFlag & ((ULLONG)BIT0 << ub4kBufferIndex))) {
								break;
							}
							uoBufferFlag |= ((ULLONG)BIT0 << ub4kBufferIndex);
						}
						if (ub4kIndex != FQ->ubL4kNum) {
							continue;
						}
						if (gubCheckBufferFlag) {
							guoFWRead4kBufferFlag |= uoBufferFlag;
						}
					}
				}
				else if (FQ->ubFJob == BYTE_FJOB_COPYFROMDRAM) {
					if (FQ->FQPara.B.btHandleTableFlag) {
						if (gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btTableInRAMFlag) {
							continue;
						}
					}
					else if (FQ->FQPara.B.btUserData) {
						ULLONG uoBufferFlag = 0;
						for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
							UBYTE ub4kBufferIndex = (FQ->ubBufferInedex_R + ub4kIndex) & gulBuffer2Read4KMask;
							if (FQ->FQPara.B.btUserData && (gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] != FQ->ubUseBufferOrder[ub4kIndex])) {
								//叫到號碼才能trigger Data!!
								break;
							}
							if (gubCheckBufferFlag && (FQ->FQPara.B.btPartialFromFlash == 0) && (guoFWRead4kBufferFlag & ((ULLONG)BIT0 << ub4kBufferIndex))) {
								break;
							}
							uoBufferFlag |= ((ULLONG)BIT0 << ub4kBufferIndex);
						}
						if (ub4kIndex != FQ->ubL4kNum) {
							continue;
						}
						if (gubCheckBufferFlag && (FQ->FQPara.B.btPartialFromFlash == 0)) {
							guoFWRead4kBufferFlag |= uoBufferFlag;
						}
					}
				}
				gFQI.ubFQIndex[ubCEIndex][(gFQI.ubFQFirst[ubCEIndex] + gFQI.ubFQDoingNumber[ubCEIndex])&FQ_NUM_MASK] = ubgFQLinkIndex;
				gFQI.ubFQDoingNumber[ubCEIndex] ++;
				M_AssertError(gFQI.ubFQDoingNumber[ubCEIndex] > FQ_NUM );
				FQ->btUNServed = 0;
				M_AssertError((FQ->ubFQPhase != 0) &&  (FQ->ubFQPhase != BYTE_WAIT_DMA));
#if B0KB
				if ( FQ->ubD3WriteMode & D3_SUSPEND_EN ) {
					gubD1D3CEPushFQ[FQ->ubCEIndex]--;
				}
#endif
			}
		}

#if ENABLE_FQ_ALIGNMENT
		for (ubCEIndex = 0; ubCEIndex < (MAX_CE + 1); ubCEIndex++) {
			ulLastEntry[ubCEIndex] = 0;
		}
#endif
		UBYTE ubCEScanCount;
		ubCEIndex = gubMTQTotalPhyCE;
		for (ubCEScanCount = 0; ubCEScanCount < (gubMTQTotalPhyCE + 1); ubCEScanCount++) {
			//M_DebugToggleGPIO2();
			if (gFQI.ubFQDoingNumber[ubCEIndex]) {
				//M_DebugGPIO6_High();
				if (ubCEIndex != gubMTQTotalPhyCE) {
					gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubCEIndex & gubPlanesPerBurstMask) * 0x200));
					gubChannel = ubCEIndex & gubPlanesPerBurstMask;
				}
				ubCFQIndex = gFQI.ubFQFirst[ubCEIndex];
				ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
				FQ = &gFQI.gFQLink[ubgFQLinkIndex];
				UBYTE ubFQServeCount;
				UBYTE ubSequentialFQNum;
				UBYTE ubUnServedCount;
				UBYTE ubNewFQExecute;
				UBYTE ubLastCFQIndex;
				UBYTE ubRemainFQDoingNum;
				UBYTE ubEnterContinuePhase;
				UBYTE ubEnterContinuePhaseBackup;

				ubSequentialFQNum = 0;
				ubNewFQExecute = 0;
			Mark_ScanFQQueueAndExecuteFQ:
				for (ubFQServeCount = 0; ubFQServeCount < gFQI.ubFQDoingNumber[ubCEIndex]; ubFQServeCount++) {
					if ((FQ->ubFQPhase == 0) || (FQ->ubFQPhase == BYTE_WAIT_DMA) || (FQ->ubFQPhase == BYTE_WAIT_MTQFREE)) {
						if (ubCEIndex != gubMTQTotalPhyCE) {
							if (ENABLE_DEBUG_MTREMAINCNT_REG) {
								if (ubCEIndex < 8) {
									ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
								}
								else {
									ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
								}
								if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
									goto Mark_CheckRemoveCodition;
								}
							}
							else {
								if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
									goto Mark_CheckRemoveCodition;
								}
							}

							ubEnterContinuePhase = 0;
							ubEnterContinuePhaseBackup = ubEnterContinuePhase;

							if ((FQ->ubFQPhase == BYTE_WAIT_DMA) || (FQ->ubFQPhase == BYTE_WAIT_MTQFREE)) {
								if (FQ->btNeedStopDMA) {
									goto Mark_CheckRemoveCodition;
								}
								else {
									ubSequentialFQNum = ubSequentialFQNumBackup[ubCEIndex];
									ubBurstBank = ubBurstBankBackup[ubCEIndex];
									ubPreviousFQIndex = ubPreviousFQIndexBackup[ubCEIndex];
									ubgPreviousFQLinkIndex = ubgPreviousFQLinkIndexBackup[ubCEIndex];
									ubCFQIndex = ubCFQIndexBackup[ubCEIndex];
									ubgFQLinkIndex = ubgFQLinkIndexBackup[ubCEIndex];
									ubCEXferPhase = ubCEXferPhaseBackup[ubCEIndex];

									if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
										ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
									}
									ubEnterContinuePhase = 1;
									ubEnterContinuePhaseBackup = ubEnterContinuePhase;

									ubNewFQExecute = 1;
									ubWaitFQCnt[ubCEIndex] = 0;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
									// Refresh autopol start time, prevent to wait DMA too long to trig timeout handle
									if (FQ->btUsingAutoPol) {
										FQ->ulAutoPolStartTime = gulOperationTime;
									}
#endif
									//gulNotFinishPhase直接跳所存的Label位置
									if (gulNotFinishPhase[ubCEIndex] != 0) {
										goto *gulNotFinishPhase[ubCEIndex];
									}
									else {
										M_AssertError(1);
									}
								}

							}

							//M_DebugGPIO5_High();
						Mark_ReEnter:
							if (ENABLE_DEBUG_MTREMAINCNT_REG) {
								if (ubCEIndex < 8) {
									ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
								}
								else {
									ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
								}
								if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
									goto Mark_CheckRemoveCodition;
								}
							}
							else {
								if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
									goto Mark_CheckRemoveCodition;
								}
							}
							M_AssertError(gFQI.ubFQDoingNumber[ubCEIndex] < ubFQServeCount);
							ubUnServedCount = gFQI.ubFQDoingNumber[ubCEIndex] - ubFQServeCount;
							M_AssertError(ubUnServedCount == 0);

							if (ENABLE_DEBUG_RW) {
								ubLastCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + gFQI.ubFQDoingNumber[ubCEIndex] - 1) & FQ_NUM_MASK;
								if (ubLastCFQIndex < ubCFQIndex) {
									ubRemainFQDoingNum = (ubLastCFQIndex + FQ_NUM - ubCFQIndex + 1);
								}
								else {
									ubRemainFQDoingNum = ubLastCFQIndex - ubCFQIndex + 1;
								}
								M_AssertError(ubRemainFQDoingNum != ubUnServedCount);
							}

						Mark_CheckFQContinue:
							M_AssertError(gubFQNeedExecuteRightNow[ubCEIndex]);
							M_AssertError(gubFQNotSequential[ubCEIndex]);
							//M_AssertError(gubNoFQIn[ubCEIndex]);
							gubFQNeedExecuteRightNow[ubCEIndex] = 0;
							gubFQNotSequential[ubCEIndex] = 0;

							if (ENABLE_MULTIPLANE_READ || ENABLE_MULTIPLANE_PROGRAM) {
								ubSequentialFQNum = flaCheckFQContinueNum(ubCEIndex, ubCFQIndex, ubUnServedCount);

								// 如果還沒等到所需的FQ個數or sequence沒有被中斷就繼續等FQ
								if ((ubSequentialFQNum != 0) && (gubFQNeedExecuteRightNow[ubCEIndex] == 0)) {
									if ((FQ->ubFJob == BYTE_FJOB_WRITE) && (ubSequentialFQNum < 2)) {
										if ((gFQI.ubXferPhase[ubCEIndex] == BYTE_CEW_4PLANE_DMA2_CONTINUOUS) || (gFQI.ubXferPhase[ubCEIndex] == BYTE_CEW_4PLANE_DMA6_CONTINUOUS)) {
											gubTemp = 0;
											goto Mark_ContinueWaitFQ;
										}
									}
									if ((FQ->ubFJob == BYTE_FJOB_READ) && (ubSequentialFQNum < gubBurstsPerBank) && (gubFQNotSequential[ubCEIndex] == 0))  {
										gubTemp = 0;
										goto Mark_ContinueWaitFQ;
									}
								}

								gubToCheckNextCmdSequence[ubCEIndex] = 0;
								ubNewFQExecute = 1;
								ubWaitFQCnt[ubCEIndex] = 0;


								/*if((FQ->ubFJob==BYTE_FJOB_READ) && (ubSequentialFQNum==4) && (gubFQNeedExecuteRightNow[ubCEIndex]==0) && (gubFQNotSequential[ubCEIndex] == 0)) {
									ubWaitFQCnt[ubCEIndex] = 100;
								}*/

								// 當沒有FQ或後面的FQ無法再接下去做cache operation時, 必須將之前的cmd sequence先結束
								if (ubSequentialFQNum == 0) {
								Mark_NoFQIn:
									if (gFQI.ubXferPhase[ubCEIndex]) {
										if ((gFQI.ubXferPhase[ubCEIndex] > 20) && (gFQI.ubXferPhase[ubCEIndex] < 30))  { // previous state is read relative phase
											goto Mark_NextRead;
										}

										if (gFQI.ubXferPhase[ubCEIndex] < 20)  { // previous state is write relative phase
											goto Mark_NextWrite;
										}
									}
								}
							}
							else {
								ubSequentialFQNum = 1;
							}
						}

					Mark_JobSel:
						switch (FQ->ubFJob) {
							case BYTE_FJOB_READ:
							Mark_NextRead:
								if (ENABLE_DEBUG_MTREMAINCNT_REG) {	// 做完一個phase後還有sequential FQ, 可能再進Mark_NextRead, 需再check MTQ是否快滿
									if (ubCEIndex < 8) {
										ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
									}
									else {
										ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
									}
									if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
										goto Mark_CheckRemoveCodition;
									}
								}
								else {
									if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
										goto Mark_CheckRemoveCodition;
									}
								}

								if (ENABLE_DEBUG_RW) {
									M_AssertError(ubCFQIndex != ( ( gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount)& FQ_NUM_MASK) );
								}
								//M_DebugGPIO5_Low();
								if (ENABLE_MULTIPLANE_READ) {
									ubEnterContinuePhase = 0;
									ubEnterContinuePhaseBackup = ubEnterContinuePhase;
									// CaseA => 有足夠的sequential FQs做2p/4p operation
									if (ubSequentialFQNum >= gubBurstsPerBank) {
										guwDebug[gFQI.ubXferPhase[ubCEIndex]]++;
										if (ENABLE_CY_DEBUG) {
											if (gFQI.ubXferPhase[ubCEIndex]) {
												ubPreviousFQIndex = gFQI.ubFQFirst[ubCEIndex];
												ubPreviousFQIndex = ((ubPreviousFQIndex + gFQI.ubFQDoingNumber[ubCEIndex] - 1) & FQ_NUM_MASK);
												ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
												PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

												while (PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE) {
													ubPreviousFQIndex = ((ubPreviousFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
													ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
													PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
												}
												M_AssertError(ubCFQIndex != ((ubPreviousFQIndex + 1)&FQ_NUM_MASK));
												//ubCFQIndex = (ubPreviousFQIndex + 1)&FQ_NUM_MASK);
											}
										}
										switch (gFQI.ubXferPhase[ubCEIndex]) {
											case 0:
												if (ENABLE_iFSA == 0) {
													//設定第一組cmd的FSAs
													flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
													for (ubBurstBank = 1; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
														ubNextFQIndex = ((ubCFQIndex + ubBurstBank)&FQ_NUM_MASK);
														ubgNextFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubNextFQIndex];
														flaSetFlashSourceAddrCE_CH(ubgNextFQLinkIndex);
													}
												}
#if MicronFlashOnly
												flaSwitchMicronSLCMode(ubgFQLinkIndex, ubCEIndex);

#endif
												if (guoFlashUseType.B.btFourPlane) {	// 60h xxx 60h xxx 60h xxx 60h xxx 30h
#if MicronFlashOnly
#if B0KB
													if ((gFQI.gFQLink[ubgFQLinkIndex].ubD3WriteMode & D3_SUSPEND_EN) && (gFQI.gFQLink[ubgFQLinkIndex].uwFJobInfo & BIT_FJOBI_NON_DMA)) {
														// for Internal copy back
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex , FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
													}
													else
#endif
													{
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex , FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
													}
#else
													flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex , FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
#endif
													//flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex ,FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, USE_SINGLE_MT|GET_EXTRA_DEPTH, WAIT_TRUE_READY);
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_4PLANE_BUSY_START;
												}
												else if (guoFlashUseType.B.btTwoPlane) {	// 60h xxx 60h xxx 30h
#if MicronFlashOnly
#if B0KB
													if ((gFQI.gFQLink[ubgFQLinkIndex].ubD3WriteMode & D3_SUSPEND_EN) && (gFQI.gFQLink[ubgFQLinkIndex].uwFJobInfo & BIT_FJOBI_NON_DMA)) {
														// for Internal copy back
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
													}
													else

#endif
													{
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
													}
#else
#if TLC
#if Hynix
													flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C60_A3_AG_C60_A3_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
#else
													flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C60_A3_AG_C60_A3_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, (FQ->ubD3WriteMode & D3_SUSPEND_EN) ? WAIT_CACHE_READY/*D3 suspend*/ : WAIT_TRUE_READY);
#endif
#else
													flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C60_A3_AG_C60_A3_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
#endif
#endif
													//flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, USE_SINGLE_MT|GET_EXTRA_DEPTH, WAIT_TRUE_READY);
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_2PLANE_BUSY_START;
												}
												else { 	// 00h xxxxx 30h
													flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_1PLANE_BUSY_START;
												}

#if MicronFlashOnly
												gubMicronModeSet = 0;
#endif
												// 修改相關FQ的FQPhase
												for (ubBurstBank = 1; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
													ubFQServeCount++;
													ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													FQ = &gFQI.gFQLink[ubgFQLinkIndex];
													FQ->ubFQPhase = BYTE_CMD_ADDR_DONE;
												}


												if (( (Hynix3DV6 || Hynix3DV5 || YMTC_JGS) && (FQ->ubD3WriteMode & D3_SUSPEND_EN) ) || (ENABLE_CACHEREAD == 0) || (ENABLE_PSEUDO_CE && gubExpandCE))	{	// read without cache需要在這邊將剩下的DMA都做完
													ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - (gubBurstsPerBank - 1))&FQ_NUM_MASK); //Need  minus 1 cause ubCFQIndex is not updated yet
													ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
													PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
													for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
														ubMTQOverLimit = 0;
														if (ENABLE_DEBUG_MTREMAINCNT_REG) {
															if (ubCEIndex < 8) {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
															}
															else {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
															}
															if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														else {
															if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
															M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
															if (PreviousFQ->btNeedStopDMA ) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
															}
															else {
																PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
															}
															gulNotFinishPhase[ubCEIndex] = && Mark_Phase_Continue_Disable_Cache_Read_DMA_Continuous;
															if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																M_AssertError(1);
															}
															goto Mark_Backup;
														}
													Mark_Phase_Continue_Disable_Cache_Read_DMA_Continuous:
#if TLC
														if (gFQI.gFQLink[ubgPreviousFQLinkIndex].uwFJobInfo & BIT_FJOBI_NON_DMA) {
#if Hynix || YMTC
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_C70_POL_MK20 , USE_SINGLE_MT, 0xFF);
#else
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, (gFQI.gFQLink[ubgPreviousFQLinkIndex].ubD3WriteMode & D3_SUSPEND_EN) ? FPU_PTR_C70_POL_MK40 : FPU_PTR_C70_POL_MK20 , USE_SINGLE_MT, 0xFF);
#endif
														}
														else {
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_SINGLE_MT, 0xFF);
														}
#else
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_SINGLE_MT, 0xFF);
#endif

														if (ubBurstBank < (gubBurstsPerBank - 1)) {
															if (ubEnterContinuePhase) {     //last burstbank dont need plus 1
																ubFQServeCount++;
															}
															ubPreviousFQIndex = ((ubPreviousFQIndex + 1)&FQ_NUM_MASK);
															ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
															PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
														}
													}
													gFQI.ubXferPhase[ubCEIndex] = 0;

													gulNotFinishPhase[ubCEIndex] = 0;	// pseudo CE or 不開cache operation時的新flow, 若沒有清為0可能會導致下面MTQ full直接跳掉造成之後M_AssertError(gubFQNeedExecuteRightNow[ubCEIndex]);

													ubEnterContinuePhase = 0; // 開始另一段DMA前, 先歸0, 避免後續FQServeCount多加
												}

												ubSequentialFQNum -= gubBurstsPerBank;

												if (ubSequentialFQNum) {
													if (ENABLE_DEBUG_MTREMAINCNT_REG) {
														if (ubCEIndex < 8) {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
														}
														else {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
														}
														if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													else {
														if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													// for 4p, 1, 1, 1, 1, 1, 2, 3, 則作完1111後,  立即重進入read flow, 會因為CFQIndex還停在4th個1, 變成準備DMA時會算成1st 1前的FQ
													ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													ubFQServeCount++;
													goto Mark_NextRead;
												}
												break;
											case BYTE_CER_2PLANE_BUSY_START:
											case BYTE_CER_2PLANE_CACHE_DMA_CONTINUOUS:

											case BYTE_CER_4PLANE_BUSY_START:
											case BYTE_CER_4PLANE_CACHE_DMA_CONTINUOUS:
												ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - gubBurstsPerBank)&FQ_NUM_MASK);
												ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
												PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

												if (ENABLE_iFSA == 0) {
													//設定後一組cmd的FSAs
													flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
													for (ubBurstBank = 1; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
														ubNextFQIndex = ((ubCFQIndex + ubBurstBank)&FQ_NUM_MASK);
														ubgNextFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubNextFQIndex];
														flaSetFlashSourceAddrCE_CH(ubgNextFQLinkIndex);
													}
												}

												// 31h (busy)  00h xxxxx 05h xx E0h DMA
												//2 ================== 可能還要加個wait true ready or cache ready option for 30h/31h =================
												flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_C31, USE_REUSE_MT, WAIT_CACHE_READY);
												for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
													ubMTQOverLimit = 0;
													if (ENABLE_DEBUG_MTREMAINCNT_REG) {
														if (ubCEIndex < 8) {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
														}
														else {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
														}
														if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
															ubMTQOverLimit = 1;
														}
													}
													else {
														if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
															ubMTQOverLimit = 1;
														}
													}
													if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
														M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
														if (PreviousFQ->btNeedStopDMA ) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
															PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
														}
														else {
															PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
														}
														gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous;
														if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
															M_AssertError(1);
														}
														goto Mark_Backup;
													}
												Mark_Phase_MultiPlane_DMA_Continuous:
													flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, (ubBurstBank ? USE_SINGLE_MT : USE_REUSE_MT), 0xFF);
													if (ubEnterContinuePhase) {
														ubFQServeCount++;
													}

													if (ubBurstBank < (gubBurstsPerBank - 1)) {
														ubPreviousFQIndex = ((ubPreviousFQIndex + 1)&FQ_NUM_MASK);
														ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
														PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
													}
												}

												// 修改相關FQ的FQPhase
												for (ubBurstBank = 1; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
													ubFQServeCount++;
													ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													FQ = &gFQI.gFQLink[ubgFQLinkIndex];
													FQ->ubFQPhase = BYTE_CMD_ADDR_DONE;
												}

												if (guoFlashUseType.B.btFourPlane) {
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_4PLANE_CACHE_DMA_CONTINUOUS;
												}
												else if (guoFlashUseType.B.btTwoPlane) {
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_2PLANE_CACHE_DMA_CONTINUOUS;
												}

												gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

												ubSequentialFQNum -= gubBurstsPerBank;

												if (ubSequentialFQNum) {
													if (ENABLE_DEBUG_MTREMAINCNT_REG) {
														if (ubCEIndex < 8) {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
														}
														else {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
														}
														if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													else {
														if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													// for 4p, 1, 1, 1, 1, 1, 2, 3, 則作完1111後,  立即重進入read flow, 會因為CFQIndex還停在4th個1, 變成準備DMA時會算成1st 1前的FQ
													ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													ubFQServeCount++;
													goto Mark_NextRead;
												}
												break;
											case BYTE_CER_1PLANE_BUSY_START:
											case BYTE_CER_1PLANE_CACHE_DMA_CONTINUOUS:
											case BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS:
												ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
												ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
												PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

												if (guoFlashUseType.B.btFourPlane || guoFlashUseType.B.btTwoPlane) {
													if (gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_1PLANE_BUSY_START) {
														if (PreviousFQ->btNeedStopDMA) {
															M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
															PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
															gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_CONTINUOUS;
															if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																M_AssertError(1);
															}
															goto Mark_Backup;
														}
													Mark_Phase_OnePlane_DMA_CONTINUOUS:
#if (TLC && (B0KB == 0))
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_DR, USE_SINGLE_MT, 0xFF);
#else
#if MicronFlashOnly
														if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
														if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
														{
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_DR, USE_SINGLE_MT, 0xFF);
														}
														else {
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_SINGLE_MT, 0xFF);
														}
#endif
														if (ubEnterContinuePhase) {
															ubFQServeCount++;
														}
													}
													else {	// BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgPreviousFQLinkIndex, FPU_PTR_C3F, USE_REUSE_MT | GET_EXTRA_DEPTH, WAIT_CACHE_READY);
#if ENABLE_FQ_ALIGNMENT
														if (((PreviousFQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask) != gubBurstsPerBank - 1) {
															ulLastEntry[PreviousFQ->ubCEIndex] = (CHECK_FQ_ALIGNMENT | PreviousFQ->ulFEntry);
														}
#endif
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_0x3F_CMD_DONE;
														ubMTQOverLimit = 0;
														if (ENABLE_DEBUG_MTREMAINCNT_REG) {
															if (ubCEIndex < 8) {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
															}
															else {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
															}
															if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														else {
															if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
															M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
															if (PreviousFQ->btNeedStopDMA ) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
															}
															else {
																PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
															}
															gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_CONTINUOUS2;
															if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																M_AssertError(1);
															}
															goto Mark_Backup;
														}
													Mark_Phase_OnePlane_DMA_CONTINUOUS2:
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
														if (ubEnterContinuePhase) {
															ubFQServeCount++;
														}
													}
													ubCEXferPhase = 0;

													gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]
												}
												else { // one plane
													//M_AssertError(1);	// one plane之後要修NeedStopDMA/MTQFree ... 問題
													// BYTE_CER_1PLANE_BUSY_START || BYTE_CER_1PLANE_CACHE_DMA_CONTINUOUS || BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS
													if (ENABLE_iFSA == 0) {
														flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
													}
													flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_C31, USE_REUSE_MT, WAIT_CACHE_READY);

#if ENABLE_ONEPLANE_CACHEREAD
													ubMTQOverLimit = 0;
													if (ENABLE_DEBUG_MTREMAINCNT_REG) {
														if (ubCEIndex < 8) {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
														}
														else {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
														}
														if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
															ubMTQOverLimit = 1;
														}
													}
													else {
														if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
															ubMTQOverLimit = 1;
														}
													}
													if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
														M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
														if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
															PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
														}
														else {
															PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
														}
														gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_A;
														if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
															M_AssertError(1);
														}
														goto Mark_Backup;
													}

												Mark_Phase_OnePlane_DMA_A:
#endif
													flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
													ubCEXferPhase = BYTE_CER_1PLANE_CACHE_DMA_CONTINUOUS;
#if ENABLE_ONEPLANE_CACHEREAD
													if (ubEnterContinuePhase) {
														ubFQServeCount++;
													}
													gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]
#endif
												}
												gFQI.ubXferPhase[ubCEIndex] = ubCEXferPhase;

												//  special case: for 1p random cache read接4p read
												if ((gFQI.ubXferPhase[ubCEIndex] == 0) && ubSequentialFQNum) {
													if (ENABLE_DEBUG_MTREMAINCNT_REG) {
														if (ubCEIndex < 8) {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
														}
														else {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
														}
														if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													else {
														if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													goto Mark_NextRead;
												}
#if ENABLE_ONEPLANE_CACHEREAD
												ubSequentialFQNum -= gubBurstsPerBank;

												if (ubSequentialFQNum) {
													if (ENABLE_DEBUG_MTREMAINCNT_REG) {
														if (ubCEIndex < 8) {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
														}
														else {
															ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
														}
														if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													else {
														if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
															goto Mark_CheckRemoveCodition;
														}
													}
													// for 4p, 1, 1, 1, 1, 1, 2, 3, 則作完1111後,  立即重進入read flow, 會因為CFQIndex還停在4th個1, 變成準備DMA時會算成1st 1前的FQ

													ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													ubFQServeCount++;

													goto Mark_NextRead;
												}
#endif
												break;
											default:
												M_AssertError(1);
												break;
										}
									}
									else {
										// CaseB => 等不到FQ or 等到其他job type的FQ, 必須結束目前的cmd sequence
										//               flow: 處理前一段cmd sequence, 然後再以1p random cache read處理完
										if (gubFQNeedExecuteRightNow[ubCEIndex]) {
											UBYTE ubMPlaneCache; // 1: multiplane cache read phase, 0: multiplane read phase
											guwDebug[gFQI.ubXferPhase[ubCEIndex]]++;
											if (ENABLE_CY_DEBUG) {
												if (gFQI.ubXferPhase[ubCEIndex]) {
													ubPreviousFQIndex = gFQI.ubFQFirst[ubCEIndex];
													ubPreviousFQIndex = ((ubPreviousFQIndex + gFQI.ubFQDoingNumber[ubCEIndex] - 1) & FQ_NUM_MASK);
													ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
													PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

													while (PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE) {
														ubPreviousFQIndex = ((ubPreviousFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
														ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
														PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
													}
													M_AssertError(ubCFQIndex != ((ubPreviousFQIndex + 1)&FQ_NUM_MASK));
													//ubCFQIndex = (ubPreviousFQIndex + 1)&FQ_NUM_MASK);
												}
											}
											switch (gFQI.ubXferPhase[ubCEIndex]) {
												case 0:

												case BYTE_CER_2PLANE_BUSY_START:
												case BYTE_CER_4PLANE_BUSY_START:

												case BYTE_CER_2PLANE_CACHE_DMA_CONTINUOUS:
												case BYTE_CER_4PLANE_CACHE_DMA_CONTINUOUS:

													if ((ENABLE_CACHEREAD == 0) || (ENABLE_PSEUDO_CE && gubExpandCE))	{
														M_AssertError(gFQI.ubXferPhase[ubCEIndex]);	// 不開cache或開pseudo CE時不會有cache xferPhase的狀況進入
													}

													ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - gubBurstsPerBank)&FQ_NUM_MASK);
													ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
													PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

													if (gFQI.ubXferPhase[ubCEIndex]) {
														if ((gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_2PLANE_BUSY_START) ||
														        (gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_4PLANE_BUSY_START)) {
															ubMPlaneCache = 0;
														} //BYTE_CER_2PLANE_CACHE_DMA_CONTINUOUS || BYTE_CER_4PLANE_CACHE_DMA_CONTINUOUS
														else {
															// 3Fh (busy) 00h xxxxx 05h xx E0h DMA
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgPreviousFQLinkIndex, FPU_PTR_C3F, USE_REUSE_MT | GET_EXTRA_DEPTH, WAIT_CACHE_READY);
#if ENABLE_FQ_ALIGNMENT
															if (((PreviousFQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask) != gubBurstsPerBank - 1) {
																ulLastEntry[PreviousFQ->ubCEIndex] = (CHECK_FQ_ALIGNMENT | PreviousFQ->ulFEntry);
															}
#endif
															gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_0x3F_CMD_DONE;
															ubMPlaneCache = 1;
														}

														// 00h xxxxx 05h xx E0h DMA
														for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous_Break;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_Backup;
															}
														Mark_Phase_MultiPlane_DMA_Continuous_Break:
															// no backup ubMPlaneCache, 所以再判斷一次
															if ((gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_2PLANE_BUSY_START) ||
															        (gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_4PLANE_BUSY_START)) {
																ubMPlaneCache = 0;
															}
															else {
																ubMPlaneCache = 1;
															}
															if (ubMPlaneCache) {
																flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, (ubBurstBank ? USE_SINGLE_MT : USE_REUSE_MT), 0xFF);
																if (ubBurstBank == 0) {
																	gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_CACHE_REMAINDER_DMA; //如果繼續停在BYTE_CER_0x3F_CMD_DONE, 在FlashToSram會沒有update Para0x1C
																}
															}
															else {
#if TLC
																if (gFQI.gFQLink[ubgPreviousFQLinkIndex].uwFJobInfo & BIT_FJOBI_NON_DMA) {
#if Hynix
																	flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_C70_POL_MK20, USE_SINGLE_MT, 0xFF);
#else
																	flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, (gFQI.gFQLink[ubgPreviousFQLinkIndex].ubD3WriteMode & D3_SUSPEND_EN) ? FPU_PTR_C70_POL_MK40 : FPU_PTR_C70_POL_MK20 , USE_SINGLE_MT, 0xFF);
#endif
																}
																else {
																	flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_SINGLE_MT, 0xFF);
																}
#else
																flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_SINGLE_MT, 0xFF);
#endif
															}

															if (ubBurstBank < (gubBurstsPerBank - 1)) {
																ubPreviousFQIndex = ((ubPreviousFQIndex + 1)&FQ_NUM_MASK);
																ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
																PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
															}
															if (ubEnterContinuePhase) {
																ubFQServeCount++;	// fix prefetch bug
															}
														}
													}

													gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

													//3  ===== for NeedStopDMA method ====
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													FQ = &gFQI.gFQLink[ubgFQLinkIndex];

													// NoFQIn = 1: 因等不到FQ而結束cmd sequence, 沒有new FQ, 所以不進Mark_ReEnter
													// NoFQIn = 0: 因等到non-read FQ結束cmd sequence, 進Mark_ReEnter進行新的cmd sequence
													if (ubSequentialFQNum == 0) { // 一串read後接write/erase or沒有cmd
														gFQI.ubXferPhase[ubCEIndex] = 0;
														gubFQNeedExecuteRightNow[ubCEIndex] = 0;
														if (gubNoFQIn[ubCEIndex] == 0) {
															if (ubEnterContinuePhase) { // fix prefetch bug (CDM test)
																//ubFQServeCount++;
																//M_AssertError(1);
															}
															goto Mark_ReEnter;
														}
														gubNoFQIn[ubCEIndex] = 0;

														if ((ubFQServeCount + 1) < gFQI.ubFQDoingNumber[ubCEIndex]) { // fix prefetch bug (iometer test)
															//ubFQServeCount++;
															//M_AssertError(1);
															goto Mark_ReEnter;
														}
													}
													else if (ubSequentialFQNum == 1) {//斷掉
														if (ENABLE_CY_DEBUG) {
															ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
															M_AssertError(ubDebugCFQIndex != ubCFQIndex);
															M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
														}
														if (ENABLE_iFSA == 0) {
															flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
														}

														ubMTQOverLimit = 0;
														if (ENABLE_DEBUG_MTREMAINCNT_REG) {
															if (ubCEIndex < 8) {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
															}
															else {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
															}
															if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														else {
															if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}

#if MicronFlashOnly

														flaSwitchMicronSLCMode(ubgFQLinkIndex, ubCEIndex);


#endif
														if (FQ->btNeedStopDMA || ubMTQOverLimit) {
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_REUSE_MT, WAIT_TRUE_READY);
#if MicronFlashOnly
															if (gubMicronModeSet) {
																gubMicronModeSet = 0;
															}
#endif
															M_AssertError(FQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
															if (FQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																FQ->ubFQPhase = BYTE_WAIT_DMA;
															}
															else {
																FQ->ubFQPhase = BYTE_WAIT_MTQFREE;
															}
															gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous_Break2;
															if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																M_AssertError(1);
															}
															goto Mark_Backup;
														Mark_Phase_MultiPlane_DMA_Continuous_Break2:
#if (TLC && (B0KB == 0))
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_DR, USE_REUSE_MT, 0xFF);
#else
#if MicronFlashOnly
															if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
															if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
															{
																flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_DR, USE_REUSE_MT, 0xFF);
															}
															else {
																flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
															}
#endif
														}
														else {
															flaFlashToSram_0030(BYTE_FQR_ADDRCMD, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_REUSE_MT);
#if MicronFlashOnly
															if (gubMicronModeSet) {
																gubMicronModeSet = 0;
															}
#endif
														}

														gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

														if (ENABLE_CY_DEBUG) {
															ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
															M_AssertError(ubDebugCFQIndex != ubCFQIndex);
															M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
														}

														/*if (ubEnterContinuePhase) {
															//ubFQServeCount++; //留在外層加
														}*/
														M_AssertError(ubSequentialFQNum == 0);
														ubSequentialFQNum--;
													}
													else if (ubSequentialFQNum > 1) {
#if MicronFlashOnly

														flaSwitchMicronSLCMode(ubgFQLinkIndex, ubCEIndex);

#endif
														// special path when random cache read is disable
														if (guoFlashUseType.B.btRandCacheRead == FALSE) {
															goto Mark_DisableRandomCacheRead;
														}

														if (ENABLE_CY_DEBUG) {
															ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
															M_AssertError(ubDebugCFQIndex != ubCFQIndex);
															M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
														}
														if (ENABLE_iFSA == 0) {
															flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
														}
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
#if MicronFlashOnly
														if (gubMicronModeSet) {
															gubMicronModeSet = 0;
														}
#endif
														ubSequentialFQNum--;

														ubEnterContinuePhase = 0; // 開始另一段DMA前, 先歸0, 避免後續FQServeCount多加

														do  {
															ubPreviousFQIndex = ubCFQIndex; // for 一致性, 不加目前不影響code
															ubgPreviousFQLinkIndex = ubgFQLinkIndex;
															PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
															ubFQServeCount++;
															ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
															ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];

															if (ENABLE_iFSA == 0) {
																flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
															}
															/*
															Mark_Phase_MultiPlane_DMA_Continuous_Break3:
															flaFlashToSramRandCache_CH(ubgFQLinkIndex, ubgPreviousFQLinkIndex);	// (A2) 00 31 / DMA
															if (PreviousFQ->btNeedStopDMA) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																ubSequentialFQNumBackup[ubCEIndex] = ubSequentialFQNum;
																ubBurstBankBackup[ubCEIndex] = ubBurstBank;
																ubPreviousFQIndexBackup[ubCEIndex] = ubPreviousFQIndex;
																ubgPreviousFQLinkIndexBackup[ubCEIndex] = ubgPreviousFQLinkIndex;
																ubCFQIndexBackup[ubCEIndex] = ubCFQIndex;
																ubgFQLinkIndexBackup[ubCEIndex] = ubgFQLinkIndex;
																ubCEXferPhaseBackup[ubCEIndex] = ubCEXferPhase;
																gubNotFinishPhase[ubCEIndex] = PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_BREAK3;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_CheckRemoveCodition;
															}
															*/
															//
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C31, USE_REUSE_MT, WAIT_CACHE_READY);
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous_Break3;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_Backup;
															}
														Mark_Phase_MultiPlane_DMA_Continuous_Break3:
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
															//
															ubSequentialFQNum--;
															if (ubEnterContinuePhase) {
																//ubFQServeCount++;
															}
														}
														while (ubSequentialFQNum);

														gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

														if (ubEnterContinuePhase) {
															ubFQServeCount++;
														}

														//3 ===== for NeedStopDMA method ====
														FQ = &gFQI.gFQLink[ubgFQLinkIndex];

														if (ENABLE_CY_DEBUG) {
															ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
															M_AssertError(ubDebugCFQIndex != ubCFQIndex);
															M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
														}
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_C3F, USE_REUSE_MT | GET_EXTRA_DEPTH, WAIT_CACHE_READY);
#if ENABLE_FQ_ALIGNMENT
														if (((FQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask) != gubBurstsPerBank - 1) {
															ulLastEntry[FQ->ubCEIndex] = (CHECK_FQ_ALIGNMENT | FQ->ulFEntry);
														}
#endif
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_0x3F_CMD_DONE;
														ubMTQOverLimit = 0;
														if (ENABLE_DEBUG_MTREMAINCNT_REG) {
															if (ubCEIndex < 8) {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
															}
															else {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
															}
															if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														else {
															if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														if (FQ->btNeedStopDMA || ubMTQOverLimit) {
															M_AssertError(FQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
															if (FQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																FQ->ubFQPhase = BYTE_WAIT_DMA;
															}
															else {
																FQ->ubFQPhase = BYTE_WAIT_MTQFREE;
															}
															gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous_Break4;
															if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																M_AssertError(1);
															}
															goto Mark_Backup;
														}
													Mark_Phase_MultiPlane_DMA_Continuous_Break4:
														if (ENABLE_CY_DEBUG) {
															ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
															M_AssertError(ubDebugCFQIndex != ubCFQIndex);
															M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
														}
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
														if (ubEnterContinuePhase) {
															//ubFQServeCount++; //留在外層加
														}
														gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]
													}
													break;
												case BYTE_CER_1PLANE_BUSY_START:
												case BYTE_CER_1PLANE_CACHE_DMA_CONTINUOUS:
												case BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS:
													if (guoFlashUseType.B.btFourPlane || guoFlashUseType.B.btTwoPlane) {

														M_AssertError(guoFlashUseType.B.btRandCacheRead == FALSE);

														while (ubSequentialFQNum) {
															ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
															ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
															PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
															if (ENABLE_iFSA == 0) {
																flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
															}
															/*
															Mark_Phase_OnePlane_DMA_Continuous_Break:
															flaFlashToSramRandCache_CH(ubgFQLinkIndex, ubgPreviousFQLinkIndex);	// (A2) 00 31 / DMA
															if (PreviousFQ->btNeedStopDMA) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																ubSequentialFQNumBackup[ubCEIndex] = ubSequentialFQNum;
																ubBurstBankBackup[ubCEIndex] = ubBurstBank;
																ubPreviousFQIndexBackup[ubCEIndex] = ubPreviousFQIndex;
																ubgPreviousFQLinkIndexBackup[ubCEIndex] = ubgPreviousFQLinkIndex;
																ubCFQIndexBackup[ubCEIndex] = ubCFQIndex;
																ubgFQLinkIndexBackup[ubCEIndex] = ubgFQLinkIndex;
																ubCEXferPhaseBackup[ubCEIndex] = ubCEXferPhase;
																gubNotFinishPhase[ubCEIndex] = PHASE_CONTINUE_ONEPLANE_DMA_CONTINUOUS_BREAK;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_CheckRemoveCodition;
															}
															*/
															//
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C31, USE_REUSE_MT, WAIT_CACHE_READY);
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_Continuous_Break;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_Backup;
															}
														Mark_Phase_OnePlane_DMA_Continuous_Break:
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
															//
															ubSequentialFQNum--;

															if (ubSequentialFQNum) {
																ubFQServeCount++;
																ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
																ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
															}

														}

														gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

														if (ubEnterContinuePhase) {
															ubFQServeCount++;
														}

														//3 ===== for NeedStopDMA method ====
														ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
														FQ = &gFQI.gFQLink[ubgFQLinkIndex];

														if (ENABLE_CY_DEBUG) {
															ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
															M_AssertError(ubDebugCFQIndex != ubCFQIndex);
															M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
														}
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_C3F, USE_REUSE_MT | GET_EXTRA_DEPTH, WAIT_CACHE_READY);
#if ENABLE_FQ_ALIGNMENT
														if (((FQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask) != gubBurstsPerBank - 1) {
															ulLastEntry[FQ->ubCEIndex] = (CHECK_FQ_ALIGNMENT | FQ->ulFEntry);
														}
#endif
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_0x3F_CMD_DONE;
														ubMTQOverLimit = 0;
														if (ENABLE_DEBUG_MTREMAINCNT_REG) {
															if (ubCEIndex < 8) {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
															}
															else {
																ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
															}
															if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														else {
															if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																ubMTQOverLimit = 1;
															}
														}
														if (FQ->btNeedStopDMA || ubMTQOverLimit) {
															M_AssertError(FQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
															if (FQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																FQ->ubFQPhase = BYTE_WAIT_DMA;
															}
															else {
																FQ->ubFQPhase = BYTE_WAIT_MTQFREE;
															}
															gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_Continuous_Break2;
															if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																M_AssertError(1);
															}
															goto Mark_Backup;
														}
													Mark_Phase_OnePlane_DMA_Continuous_Break2:
														if (ENABLE_CY_DEBUG) {
															ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
															M_AssertError(ubDebugCFQIndex != ubCFQIndex);
															M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
														}
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);

														gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

														if (ubEnterContinuePhase) {
															//ubFQServeCount++; //留在外層加
														}
													}
													else {
														//1 ===================================
														//M_AssertError(1); // one plane之後要修NeedStopDMA/MTQFree ... 問題
														ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
														ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
#if ENABLE_ONEPLANE_CACHEREAD
														PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
														if (ENABLE_iFSA == 0) {
															flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
														}
#endif

														if (gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_1PLANE_BUSY_START) {

#if ENABLE_ONEPLANE_CACHEREAD
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA ) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_B;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}

																goto Mark_Backup;
															}
														Mark_Phase_OnePlane_DMA_B:
#endif

															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_SINGLE_MT, 0xFF);
														}
														else {	// BYTE_CER_1PLANE_CACHE_DMA_CONTINUOUS || BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgPreviousFQLinkIndex, FPU_PTR_C3F, USE_REUSE_MT | GET_EXTRA_DEPTH, WAIT_CACHE_READY);
#if ENABLE_FQ_ALIGNMENT
															PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
															if (((PreviousFQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask) != gubBurstsPerBank - 1) {
																ulLastEntry[PreviousFQ->ubCEIndex] = (CHECK_FQ_ALIGNMENT | PreviousFQ->ulFEntry);
															}
#endif
															gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_0x3F_CMD_DONE;
#if ENABLE_ONEPLANE_CACHEREAD
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA ) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_B2;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}

																goto Mark_Backup;
															}
														Mark_Phase_OnePlane_DMA_B2:
#endif

															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
														}

#if ENABLE_ONEPLANE_CACHEREAD
														if (ubEnterContinuePhase) {
															ubFQServeCount++;
														}
														gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

														ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
														FQ = &gFQI.gFQLink[ubgFQLinkIndex];
														if (ubSequentialFQNum == 0) { // 一串read後接write/erase or沒有cmd
															gFQI.ubXferPhase[ubCEIndex] = 0;
															gubFQNeedExecuteRightNow[ubCEIndex] = 0;
															if (gubNoFQIn[ubCEIndex] == 0) {
																if (ubEnterContinuePhase) { // fix prefetch bug (CDM test)
																	//ubFQServeCount++;
																	//M_AssertError(1);
																}
																goto Mark_ReEnter;
															}
															gubNoFQIn[ubCEIndex] = 0;

															if ((ubFQServeCount + 1) < gFQI.ubFQDoingNumber[ubCEIndex]) { // fix prefetch bug (iometer test)
																//ubFQServeCount++;
																//M_AssertError(1);
																goto Mark_ReEnter;
															}
														}
#endif

													}
													break;
												default:
													M_AssertError(1);
													break;
											}
											gFQI.ubXferPhase[ubCEIndex] = 0;
											gubFQNeedExecuteRightNow[ubCEIndex] = 0;
										}
										// CaseC => 等到non-sequential read FQ, 轉換成1p random cache read flow
										//               flow: 處理前一段cmd sequence, 再以1p random cache read延續 (to set gubToCheckNextCmdSequence[ubCEIndex])
										else if (gubFQNotSequential[ubCEIndex]) {
											guwDebug[gFQI.ubXferPhase[ubCEIndex]]++;
											if (ENABLE_CY_DEBUG) {
												if (gFQI.ubXferPhase[ubCEIndex]) {
													ubPreviousFQIndex = gFQI.ubFQFirst[ubCEIndex];
													ubPreviousFQIndex = ((ubPreviousFQIndex + gFQI.ubFQDoingNumber[ubCEIndex] - 1) & FQ_NUM_MASK);
													ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
													PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

													while (PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE) {
														ubPreviousFQIndex = ((ubPreviousFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
														ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
														PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
													}
													M_AssertError(ubCFQIndex != ((ubPreviousFQIndex + 1)&FQ_NUM_MASK));
													//ubCFQIndex = (ubPreviousFQIndex + 1)&FQ_NUM_MASK);
												}
											}
											switch (gFQI.ubXferPhase[ubCEIndex]) {
												case BYTE_CER_2PLANE_BUSY_START:
												case BYTE_CER_4PLANE_BUSY_START:

												case BYTE_CER_2PLANE_CACHE_DMA_CONTINUOUS:
												case BYTE_CER_4PLANE_CACHE_DMA_CONTINUOUS:

													if ((gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_2PLANE_BUSY_START) ||
													        (gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_4PLANE_BUSY_START)) {
														ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - gubBurstsPerBank)&FQ_NUM_MASK);
														ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
														PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

														// 00h xxxxx 05h xx E0h DMA
														for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous_NotSeq;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_Backup;
															}
														Mark_Phase_MultiPlane_DMA_Continuous_NotSeq:
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_SINGLE_MT, 0xFF);

															if (ubEnterContinuePhase) {
																ubFQServeCount++;
															}

															if (ubBurstBank < (gubBurstsPerBank - 1)) {
																ubPreviousFQIndex = ((ubPreviousFQIndex + 1)&FQ_NUM_MASK);
																ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
																PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
															}
														}
													}

													else if ((gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_2PLANE_CACHE_DMA_CONTINUOUS) ||
													         (gFQI.ubXferPhase[ubCEIndex] == BYTE_CER_4PLANE_CACHE_DMA_CONTINUOUS)) {
														ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - gubBurstsPerBank)&FQ_NUM_MASK);
														ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
														PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

														// 3Fh (busy) 00h xxxxx 05h xx E0h DMA
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgPreviousFQLinkIndex, FPU_PTR_C3F, USE_REUSE_MT | GET_EXTRA_DEPTH, WAIT_CACHE_READY);
#if ENABLE_FQ_ALIGNMENT
														if (((PreviousFQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask) != gubBurstsPerBank - 1) {
															ulLastEntry[PreviousFQ->ubCEIndex] = (CHECK_FQ_ALIGNMENT | PreviousFQ->ulFEntry);
														}
#endif
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_0x3F_CMD_DONE;
														for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous_NotSeq2;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_Backup;
															}
														Mark_Phase_MultiPlane_DMA_Continuous_NotSeq2:
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, (ubBurstBank ? USE_SINGLE_MT : USE_REUSE_MT), 0xFF);
															if (ubBurstBank == 0) {
																gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_CACHE_REMAINDER_DMA;  //如果繼續停在BYTE_CER_0x3F_CMD_DONE, 在FlashToSram會沒有update Para0x1C
															}

															if (ubEnterContinuePhase) {
																ubFQServeCount++;
															}

															if (ubBurstBank < (gubBurstsPerBank - 1)) {
																ubPreviousFQIndex = ((ubPreviousFQIndex + 1)&FQ_NUM_MASK);
																ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
																PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
															}
														}
													}

													gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

													//3 ===== for NeedStopDMA method ====
													ubEnterContinuePhase = 0; // 開始另一段DMA前, 先歸0, 避免後續FQServeCount多加
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													FQ = &gFQI.gFQLink[ubgFQLinkIndex];

													gFQI.ubXferPhase[ubCEIndex] = 0;

													if (ubSequentialFQNum) {
														if (guoFlashUseType.B.btFourPlane || guoFlashUseType.B.btTwoPlane) {
															if (ENABLE_iFSA == 0) {
																flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
															}
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
															ubSequentialFQNum--;
															if (ubSequentialFQNum == 0) {
																ubCEXferPhase = BYTE_CER_1PLANE_BUSY_START;
															}
															else {
																ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
																ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
																ubFQServeCount++;
																ubCEXferPhase = BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS;
															}
															while (ubSequentialFQNum) {
																ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
																ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
																PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
																if (ENABLE_iFSA == 0) {
																	flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
																}
																/*
																Mark_Phase_MultiPlane_DMA_Continuous_NotSeq3:
																flaFlashToSramRandCache_CH(ubgFQLinkIndex, ubgPreviousFQLinkIndex);	// (A2) 00 31 / DMA
																if (PreviousFQ->btNeedStopDMA) {
																	M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																	ubSequentialFQNumBackup[ubCEIndex] = ubSequentialFQNum;
																	ubBurstBankBackup[ubCEIndex] = ubBurstBank;
																	ubPreviousFQIndexBackup[ubCEIndex] = ubPreviousFQIndex;
																	ubgPreviousFQLinkIndexBackup[ubCEIndex] = ubgPreviousFQLinkIndex;
																	ubCFQIndexBackup[ubCEIndex] = ubCFQIndex;
																	ubgFQLinkIndexBackup[ubCEIndex] = ubgFQLinkIndex;
																	ubCEXferPhaseBackup[ubCEIndex] = ubCEXferPhase;
																	gubNotFinishPhase[ubCEIndex] = PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_NOTSEQ3;
																	if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																		M_AssertError(1);
																	}
																	goto Mark_CheckRemoveCodition;
																}
																*/
																//
																flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C31, USE_REUSE_MT, WAIT_CACHE_READY);
																ubMTQOverLimit = 0;
																if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																	if (ubCEIndex < 8) {
																		ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																	}
																	else {
																		ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																	}
																	if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																		ubMTQOverLimit = 1;
																	}
																}
																else {
																	if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																		ubMTQOverLimit = 1;
																	}
																}
																if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																	M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																	if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																		PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																	}
																	else {
																		PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																	}
																	gulNotFinishPhase[ubCEIndex] = && Mark_Phase_MultiPlane_DMA_Continuous_NotSeq3;
																	if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																		M_AssertError(1);
																	}
																	goto Mark_Backup;
																}
															Mark_Phase_MultiPlane_DMA_Continuous_NotSeq3:
																flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
																//
																ubSequentialFQNum--;

																if (ubSequentialFQNum) {
																	ubFQServeCount++;
																	ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
																	ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
																}
															}
															if (ubEnterContinuePhase) {
																ubFQServeCount++;
															}
														}
														else {
															//1 ===================================
															M_AssertError(1);	// one plane之後要修NeedStopDMA/MTQFree ... 問題
															ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
															ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
															if (ENABLE_iFSA == 0) {
																flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
															}
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C31, USE_REUSE_MT, WAIT_CACHE_READY);
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
															ubSequentialFQNum--;
															ubCEXferPhase = BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS;
														}
														gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]
														gFQI.ubXferPhase[ubCEIndex] = ubCEXferPhase;
														gubFQNotSequential[ubCEIndex] = 0;
														gubToCheckNextCmdSequence[ubCEIndex] = 1;
													}
													else { // 當一段sequential read要接成另一段時, 會先得到SeqentialFQNum=0結束前面的read, 才重啟新read sequence
														gubFQNotSequential[ubCEIndex] = 0;
														gubToCheckNextCmdSequence[ubCEIndex] = 1;
														goto Mark_ReEnter;
													}
													break;

												case 0:
												case BYTE_CER_1PLANE_BUSY_START:
												case BYTE_CER_1PLANE_CACHE_DMA_CONTINUOUS:
												case BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS:
													if (guoFlashUseType.B.btFourPlane || guoFlashUseType.B.btTwoPlane) {
														//M_DebugGPIO7_High();
														if (gFQI.ubXferPhase[ubCEIndex] == 0) {
															if (ENABLE_iFSA == 0) {
																flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
															}
#if MicronFlashOnly

															flaSwitchMicronSLCMode(ubgFQLinkIndex, ubCEIndex);


#endif
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_SINGLE_MT | GET_EXTRA_DEPTH, WAIT_TRUE_READY);
#if MicronFlashOnly
															if (gubMicronModeSet) {
																gubMicronModeSet = 0;
															}
#endif
															ubSequentialFQNum--;
															if (ubSequentialFQNum == 0) {
																ubCEXferPhase = BYTE_CER_1PLANE_BUSY_START;
															}
															else {
																ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
																ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
																ubFQServeCount++;
																ubCEXferPhase = BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS;
															}
														}
														else {
															ubCEXferPhase = BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS;
														}
														while (ubSequentialFQNum) {
															ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
															ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
															PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
															if (ENABLE_iFSA == 0) {
																flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
															}
															/*
															Mark_Phase_OnePlane_DMA_Continuous_NotSeq:
															flaFlashToSramRandCache_CH(ubgFQLinkIndex, ubgPreviousFQLinkIndex);	// (A2) 00 31 / DMA
															if (PreviousFQ->btNeedStopDMA) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																ubSequentialFQNumBackup[ubCEIndex] = ubSequentialFQNum;
																ubBurstBankBackup[ubCEIndex] = ubBurstBank;
																ubPreviousFQIndexBackup[ubCEIndex] = ubPreviousFQIndex;
																ubgPreviousFQLinkIndexBackup[ubCEIndex] = ubgPreviousFQLinkIndex;
																ubCFQIndexBackup[ubCEIndex] = ubCFQIndex;
																ubgFQLinkIndexBackup[ubCEIndex] = ubgFQLinkIndex;
																ubCEXferPhaseBackup[ubCEIndex] = ubCEXferPhase;
																gubNotFinishPhase[ubCEIndex] = PHASE_CONTINUE_ONEPLANE_DMA_CONTINUOUS_NOTSEQ;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_CheckRemoveCodition;
															}
															*/
															//
															flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C31, USE_REUSE_MT, WAIT_CACHE_READY);
															ubMTQOverLimit = 0;
															if (ENABLE_DEBUG_MTREMAINCNT_REG) {
																if (ubCEIndex < 8) {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
																}
																else {
																	ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
																}
																if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															else {
																if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
																	ubMTQOverLimit = 1;
																}
															}
															if (PreviousFQ->btNeedStopDMA || ubMTQOverLimit) {
																M_AssertError(PreviousFQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
																if (PreviousFQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
																	PreviousFQ->ubFQPhase = BYTE_WAIT_DMA;
																}
																else {
																	PreviousFQ->ubFQPhase = BYTE_WAIT_MTQFREE;
																}
																gulNotFinishPhase[ubCEIndex] = && Mark_Phase_OnePlane_DMA_Continuous_NotSeq;
																if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
																	M_AssertError(1);
																}
																goto Mark_Backup;
															}
														Mark_Phase_OnePlane_DMA_Continuous_NotSeq:
															flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
															//
															ubSequentialFQNum--;

															//if (ubEnterContinuePhase) {
															//ubFQServeCount++; //留在外層加
															//}
															if (ubSequentialFQNum) {
																ubFQServeCount++;
																ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
																ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
															}
														}
														if (ubEnterContinuePhase) {
															ubFQServeCount++;
														}
														//M_DebugGPIO7_Low();

													}
													else {
														//1 ===================================
														M_AssertError(1);	// one plane之後要修NeedStopDMA/MTQFree ... 問題
														ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
														ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
														if (ENABLE_iFSA == 0) {
															flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
														}
														flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C31, USE_REUSE_MT, WAIT_CACHE_READY);
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgPreviousFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
														ubSequentialFQNum--;
													}

													gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]

													gFQI.ubXferPhase[ubCEIndex] = ubCEXferPhase;
													gubFQNotSequential[ubCEIndex] = 0;
													gubToCheckNextCmdSequence[ubCEIndex] = 1;
													break;
												default:
													M_AssertError(1);
													break;
											}
										}
										else {	// FQNotSequential = 0, FQNeedExecuteRightNot = 0
											// 執行完2p/4p read operation後, 剩下的SequentialFQNum不夠執行2p/4p read operaton, 則繼續等FQ
											goto Mark_ContinueWaitFQ;
										}
										if (0) {
										Mark_DisableRandomCacheRead:
											while (ubSequentialFQNum) {
												if (ENABLE_CY_DEBUG) {
													ubDebugCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
													M_AssertError(ubDebugCFQIndex != ubCFQIndex);
													M_AssertError(ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]);
												}
												if (ENABLE_iFSA == 0) {
													flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
												}

												ubMTQOverLimit = 0;
												if (ENABLE_DEBUG_MTREMAINCNT_REG) {
													if (ubCEIndex < 8) {
														ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
													}
													else {
														ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
													}
													if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
														ubMTQOverLimit = 1;
													}
												}
												else {
													if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
														ubMTQOverLimit = 1;
													}
												}
												if (FQ->btNeedStopDMA || ubMTQOverLimit) {
													flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_REUSE_MT, WAIT_TRUE_READY);
#if MicronFlashOnly
													if (gubMicronModeSet == 1) {
														gubMicronModeSet = 0;
													}
#endif
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CER_1PLANE_BUSY_START;
													M_AssertError(FQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
													if (FQ->btNeedStopDMA) {	// NeedStopDMA且MTQ over limit時設定為BYTE_WAIT_DMA
														FQ->ubFQPhase = BYTE_WAIT_DMA;
													}
													else {
														FQ->ubFQPhase = BYTE_WAIT_MTQFREE;
													}
													gulNotFinishPhase[ubCEIndex] = && Mark_Phase_Continue_Disable_Random_Cache_Read_DMA_Continuous;
													if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
														M_AssertError(1);
													}
													goto Mark_Backup;
												Mark_Phase_Continue_Disable_Random_Cache_Read_DMA_Continuous:
#if (TLC && (B0KB == 0))
													flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_DR, USE_REUSE_MT, 0xFF);
#else
#if MicronFlashOnly
													if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
													if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
													{
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_DR, USE_REUSE_MT, 0xFF);
													}
													else {
														flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
													}
#endif
												}
												else {
													flaFlashToSram_0030(BYTE_FQR_ADDRCMD, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_REUSE_MT);
#if MicronFlashOnly
													if (gubMicronModeSet == 1) {
														gubMicronModeSet = 0;
													}
#endif
												}

												gFQI.ubXferPhase[ubCEIndex] = 0;
												ubSequentialFQNum--;
												if (ubSequentialFQNum) {
													ubFQServeCount++;
													ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
													ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
													FQ = &gFQI.gFQLink[ubgFQLinkIndex];
												}
											}
											gulNotFinishPhase[ubCEIndex] = 0;	// 到此階段已無因Wait_DMA or Wait_MTQFree的FQ, 若這邊之後會馬上到Mark_CheckRemoveCodition, 可以清掉gubFQNeedExecuteRightNow[ubCEIndex]
										}
									}

									gubNoFQIn[ubCEIndex] = 0;

									if (ubSequentialFQNum == 0) {
										if (gubFQNeedExecuteRightNow[ubCEIndex]) {
											gubFQNeedExecuteRightNow[ubCEIndex] = 0;
										}
										if (gubFQNotSequential[ubCEIndex]) {
											gubFQNotSequential[ubCEIndex] = 0;
										}
									}
								}
								else {
									if (ENABLE_iFSA == 0) {
										flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
									}
#if MicronFlashOnly

									flaSwitchMicronSLCMode(ubgFQLinkIndex, ubCEIndex);
#endif
#if B0KB
									if ((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA)) {
										// for B0KB internal copy back
										flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C35, USE_SINGLE_MT | GET_EXTRA_DEPTH, (FQ->ubD3WriteMode & D3_SUSPEND_EN) ? WAIT_CACHE_READY/*D3 suspend*/ : WAIT_TRUE_READY);
									}
									else
#endif
									{
										flaFlashToSram_CH(BYTE_CMD_ADDR_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C30, USE_REUSE_MT, WAIT_TRUE_READY);
									}
#if MicronFlashOnly
									if (gubMicronModeSet) {
										gubMicronModeSet = 0;
									}
#endif
									ubMTQOverLimit = 0;
									if (ENABLE_DEBUG_MTREMAINCNT_REG) {
										if (ubCEIndex < 8) {
											ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
										}
										else {
											ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
										}
										if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
											ubMTQOverLimit = 1;
										}
									}
									else {
										if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {	//以下最多一次推兩個MT
											ubMTQOverLimit = 1;
										}
									}
									if (FQ->btNeedStopDMA || ubMTQOverLimit) {
										M_AssertError(FQ->ubFQPhase != BYTE_CMD_ADDR_DONE);
										if (FQ->btNeedStopDMA) {
											FQ->ubFQPhase = BYTE_WAIT_DMA;
										}
										else {
											FQ->ubFQPhase = BYTE_WAIT_MTQFREE;
										}
										gulNotFinishPhase[ubCEIndex] = && Mark_Phase_Normal_Read_DMA_Continuous;
										if (ubgFQLinkIndex != gFQI.ubFQIndex[ubCEIndex][ubCFQIndex]) {
											M_AssertError(1);
										}
										goto Mark_Backup;
									}
								Mark_Phase_Normal_Read_DMA_Continuous:
#if (TLC && (B0KB == 0))
									flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_DR, USE_REUSE_MT, 0xFF);
#else
#if MicronFlashOnly
									if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
									if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
									{
										flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_DR, USE_REUSE_MT, 0xFF);
									}
									else {
										flaFlashToSram_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, USE_REUSE_MT, 0xFF);
									}

									gulNotFinishPhase[ubCEIndex] = 0;
#endif
									M_AssertError(ubSequentialFQNum == 0);
									ubSequentialFQNum--;
									if (gubFQNeedExecuteRightNow[ubCEIndex]) {
										gubFQNeedExecuteRightNow[ubCEIndex] = 0;
									}
									if (gubFQNotSequential[ubCEIndex]) {
										gubFQNotSequential[ubCEIndex] = 0;
									}
									if (gubNoFQIn[ubCEIndex]) {
										gubNoFQIn[ubCEIndex] = 0;
									}
								}
								break;
							case BYTE_FJOB_WRITE:
							Mark_NextWrite:
								gulNotFinishPhase[ubCEIndex] = 0;	// only for read use

								if (ENABLE_DEBUG_RW) {
									M_AssertError(ubCFQIndex != ( ( gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount)& FQ_NUM_MASK) );
								}
								// 找還沒下true program cmd or 需要等到true ready的FQ
								if (gFQI.ubXferPhase[ubCEIndex] != 0) {
									ubPreviousFQIndex = gFQI.ubFQFirst[ubCEIndex];
									ubPreviousFQIndex = ((ubPreviousFQIndex + gFQI.ubFQDoingNumber[ubCEIndex] - 1) & FQ_NUM_MASK);
									ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
									PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

									while (PreviousFQ->ubFQPhase == 0) {
										ubPreviousFQIndex = ((ubPreviousFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
										ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
										PreviousFQ  = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
									}

									/*
									ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
									ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
									PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];
									*/
								}

								if (ubSequentialFQNum == 0) {
									// 沒有sequential write FQ, 需要補下10h or wait true ready
									guwDebug[gFQI.ubXferPhase[ubCEIndex]]++;
									switch (gFQI.ubXferPhase[ubCEIndex]) {
										case 0:
											M_AssertError(1);
											break;
										case BYTE_CEW_1PLANE_CACHE_START:
										case BYTE_CEW_1PLANE_CACHE_CONTINUOUS:

										case BYTE_CEW_2PLANE_CACHE_START:
										case BYTE_CEW_2PLANE_CACHE_CONTINUOUS:

										case BYTE_CEW_4PLANE_CACHE_START:
										case BYTE_CEW_4PLANE_CACHE_CONTINUOUS:

#if TLC
											if (PreviousFQ->ubD3WriteMode & D3_SUSPEND_EN) {
												//D3 SUSPEND 只等Cache RDY, True RDY暫時只用Auto Poll
											}
											else {
#else
											{
#endif
												PreviousFQ->ubRedundantDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

#if MicronFlashOnly || YMTC_JGS
												if (gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
												if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
												{
													flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, FPU_PTR_AG_C60_A3_CD0, 1, 1, WAIT_TRUE_READY);
												}
												else {
													flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, FPU_PTR_C70_POL_MK20, 0, 1, 0xFF);
												}
											}

											if ((gFQI.ubXferPhase[ubCEIndex] == BYTE_CEW_1PLANE_CACHE_CONTINUOUS) ||
											        (gFQI.ubXferPhase[ubCEIndex] == BYTE_CEW_2PLANE_CACHE_CONTINUOUS) ||
											        (gFQI.ubXferPhase[ubCEIndex] == BYTE_CEW_4PLANE_CACHE_CONTINUOUS)) {
												PreviousFQ->ubRemainderDepth = PreviousFQ->ubDepth;	// ubRemainderDepth為81h xxxxx 15h 後的1st poll fail MT (for previous 2p cache program), 可考慮不要用remainderDepth, 也用redundantDepth
											}

											PreviousFQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];  // ubDepth為81h xxxxx 15h 後2nd poll fail MT (中間經歷1st poll fail MT, WaitTrueReady)

											if ((ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) && gubGenProgramFailDebug && PreviousFQ->btGenFail) {
												gulRandomValue = getIntRandValue(100);
												if (PreviousFQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) {
													if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_SPECAILCASE_RATIO) {
														gulPara0x00.BitMap.btForceWriteFail = 1;
													}
												}
												else if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_RATIO) {
													gulPara0x00.BitMap.btForceWriteFail = 1;
												}
											}

											//若真的發生fail, 之後需要issue 71h check planeA/B, 73h check planeC/D
#if (TLC || ENABLE_ULTRAMLC)
											if (PreviousFQ->ubD3WriteMode & D3_SUSPEND_EN) {
#if B0KB
												flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, ((PreviousFQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? FPU_PTR_C70_POL_MK40 : flaReadFailStatusFPUPointer(ubgPreviousFQLinkIndex)), 0, 1, 0xFF); // pol fail
#else
												//只有(FINE & UPPER) Poll Fail, 其它只等Cache RDY
												//flaSendCmd_CH(PreviousFQ->ubL4kIndex, ubCEIndex, ((PreviousFQ->ubD3WriteMode & D3_WRITE_FINE) && (PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER | D3_WRITE_UPPER_END))) ? FPU_PTR_C70_DAT_MK0F /*FINE & UPPER Polling Fail*/ : FPU_PTR_C70_POL_MK40 , 0, 1, 0xFF); // pol fail
												// 改成 First , Foggy, Fine 都要 Poll
												flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, ((PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER | D3_WRITE_UPPER_END))) ? flaReadFailStatusFPUPointer(ubgPreviousFQLinkIndex) /*FINE & UPPER Polling Fail*/ : FPU_PTR_C70_POL_MK40 , 0, 1, 0xFF); // pol fail
#endif
											}
											else {
												flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgPreviousFQLinkIndex), 0, 1, 0xFF); // pol fail
											}
#else//FPU_PTR_C70_DAT_MK0F
											flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgPreviousFQLinkIndex), 0, 1, 0xFF); // pol fail
#endif
											if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
#if TLC
												if (PreviousFQ->ubD3WriteMode & D3_SUSPEND_EN) {
													PreviousFQ->ubMTDepthCnt += 1;
												}
												else {
#else
												{
#endif
													PreviousFQ->ubMTDepthCnt += 2;
												}
											}

											M_AssertError(PreviousFQ->FQPara.B.btCanRemove);

											M_AssertError(PreviousFQ->ubFJob != BYTE_FJOB_WRITE);

											PreviousFQ->FQPara.B.btCanRemove = 1; //需等最後一個CheckWrite推完才改, 不然會變拿前一個CacheReady的result判斷

											gFQI.ubXferPhase[ubCEIndex] = 0;

											if (gubFQNeedExecuteRightNow[ubCEIndex]) {
												gubFQNeedExecuteRightNow[ubCEIndex] = 0;
											}

											if (gubNoFQIn[ubCEIndex] == 0) {
												goto Mark_ReEnter;
											}
											gubNoFQIn[ubCEIndex] = 0;
											break;
											//2 20160118 Fix 3plane program with 4-plane write command
										case BYTE_CEW_4PLANE_DMA3_CONTINUOUS:
										case BYTE_CEW_4PLANE_DMA7_CONTINUOUS:
											M_AssertError(gFQI.ubXferPhase[ubCEIndex]);
										case BYTE_CEW_MULTIPLANE_DMA_START:
										case BYTE_CEW_2PLANE_DMA_CONTINUOUS:

										case BYTE_CEW_4PLANE_DMA2_CONTINUOUS:
										case BYTE_CEW_4PLANE_DMA5_CONTINUOUS:
										case BYTE_CEW_4PLANE_DMA6_CONTINUOUS:

										Mark_ForceCmd10ToAvoidThreePlaneWrite:

											PreviousFQ->ubRedundantDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
											//2 因為準備autopolling前就會送完ALL_DONE, 但HW已記錄autopolling FPU, 所以MTP已經可release
#if (TLC &&(!B0KB))
											if (PreviousFQ->ubD3WriteMode & D3_SUSPEND_EN) {
												//D3 SUSPEND 只等Cache RDY, True RDY暫時只用Auto Poll
												flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, FPU_PTR_C70_POL_MK20, 0, 1, 0xFF);
											}
											else
#endif
											{
												flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, FPU_PTR_C10, 1, 1, WAIT_TRUE_READY);
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
												gubDebugSendCmdFQIndex = PreviousFQ->ubL4kIndex;
#endif
											}

											PreviousFQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
											if ((ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) && gubGenProgramFailDebug &&  PreviousFQ->btGenFail) {
												gulRandomValue = getIntRandValue(100);
												if (PreviousFQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) {
													if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_SPECAILCASE_RATIO) {
														gulPara0x00.BitMap.btForceWriteFail = 1;
													}
												}
												else if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_RATIO) {
													gulPara0x00.BitMap.btForceWriteFail = 1;
												}
											}
											flaSendCmd_CH(ubgPreviousFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgPreviousFQLinkIndex), 0, 1, 0xFF); // for 4 plane, 若有fail, 再下71h/73h去看哪個plane fail

											if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
												PreviousFQ->ubMTDepthCnt += 2;
											}

											M_AssertError(PreviousFQ->FQPara.B.btCanRemove);

											M_AssertError(PreviousFQ->ubFJob != BYTE_FJOB_WRITE);

											PreviousFQ->FQPara.B.btCanRemove = 1;

											gFQI.ubXferPhase[ubCEIndex] = 0;

											if (gubFQNeedExecuteRightNow[ubCEIndex]) {
												gubFQNeedExecuteRightNow[ubCEIndex] = 0;
											}

											if (gubNoFQIn[ubCEIndex] == 0) {
												goto Mark_ReEnter;
											}
											gubNoFQIn[ubCEIndex] = 0;
											break;
										default:
											M_AssertError(1);
											break;
									}

								}
								else {
									// 有sequential write FQ可以繼續接
									if (ENABLE_iFSA == 0) {
										flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
									}
									guwDebug[gFQI.ubXferPhase[ubCEIndex]]++;
									ulFEntry = FQ->ulFEntry;
									if (ENABLE_PLANE_CH_CE) {
										ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
									}
									else {
										ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
									}
									switch (gFQI.ubXferPhase[ubCEIndex]) {
										case 0:
#if MicronFlashOnly
											flaSwitchMicronSLCMode(ubgFQLinkIndex, ubCEIndex);
#elif ENABLE_RESET_RETRY_LEVEL
											if (((BIT0 << ubCEIndex) & gulKeepRetryLevelCE_BITMAP) && ((guoFlashUseType.B.btSandisk && TLC_BICS2) || ((!TLC) && (!guoFlashUseType.B.btSandisk)))) {
												flaSwitchDefaultRetryLevel(ubgFQLinkIndex, ubCEIndex);
											}
#endif
											if (ENABLE_MULTIPLANE_PROGRAM) {
												//UWORD uwFEntry = FQ->uwFEntry;
												//UBYTE ubBurstBank = (uwFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
												if ((ubBurstBank != 0)) { //2  增加直接normal program的方式for preformat, RDT
#if (TLC&&(!B0KB))
													if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
#if Hynix3DV6 || YMTC_JGS
														flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 1, WAIT_TRUE_READY);
#else
														flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 1, ((PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END)) ? WAIT_TRUE_READY : WAIT_CACHE_READY));
#endif
													}
													else
#endif
													{
														flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C80_A5_DW_C10, 1, WAIT_TRUE_READY);
													}

													FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

													if ((ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) && gubGenProgramFailDebug &&  FQ->btGenFail) {
														gulRandomValue = getIntRandValue(100);
														if (FQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) {
															if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_SPECAILCASE_RATIO) {
																gulPara0x00.BitMap.btForceWriteFail = 1;
															}
														}
														else if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_RATIO) {
															gulPara0x00.BitMap.btForceWriteFail = 1;
														}
													}
													FQ->ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;
													flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgFQLinkIndex), 0, 1, 0xFF); // pol fail

													FQ->FQPara.B.btCanRemove = 1;
													gFQI.ubXferPhase[ubCEIndex] = 0;
													if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
														FQ->ubMTDepthCnt ++;
													}
												}
												else {
													//2 ===== one plane =====
													if ((guoFlashUseType.B.btFourPlane == 0) && (guoFlashUseType.B.btTwoPlane == 0)) {
														// WaitCacheReady

														if ((ENABLE_CACHEPROGRAM == 0) || (ENABLE_PSEUDO_CE && gubExpandCE)) { //If CE is expanded, must disable Cache RW
															flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C80_A5_DW, 0, 0xFF);
															FQ->ubWritePhaseRedoCmd = MARK_FPU_C80_NonC10C15;
															FQ->ubRedundantDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
															flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, FPU_PTR_C10, 1, 1, WAIT_TRUE_READY);
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
															gubDebugSendCmdFQIndex = FQ->ubL4kIndex;
#endif
															FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

															flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, flaReadFailStatusFPUPointer(FQ->ubL4kIndex), 0, 1, 0xFF); // for 4 plane, 若有fail, 再下71h/73h去看哪個plane fail

															if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
																FQ->ubMTDepthCnt += 2;
															}

															M_AssertError(FQ->FQPara.B.btCanRemove);

															M_AssertError(FQ->ubFJob != BYTE_FJOB_WRITE);

															FQ->FQPara.B.btCanRemove = 1;

															//FQ->ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;	// 需要support one plane時要再check
															gFQI.ubXferPhase[ubCEIndex] = 0;
														}
														else {
															flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C80_A5_DW_C15, 1, WAIT_CACHE_READY);
															//FQ->ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;	// 需要support one plane時要再check
															gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_1PLANE_CACHE_START;
														}
													}
													//2 ===== multi planes =====
													else {
#if TLC
														if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
#if B0KB
															flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, (FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? FPU_PTR_AG_C85_A5_C11_C70_POL_MK40 : FPU_PTR_AG_C80_A5_DW, 0, 0xFF);
#else
															if (FQ->btIsMixPlaneUnit || FQ->btTLCProgramCodeBlock) {
#if Hynix3DV6 || YMTC_JGS
																flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 0, WAIT_TRUE_READY);
#else
																flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 0, ((PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END)) ? WAIT_TRUE_READY : WAIT_CACHE_READY));
#endif
															}
															else {
																if (guoFlashUseType.B.btFourPlane) {
																	uwD3SuspendFPU = flaFourPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEA);
																}
																else {
																	uwD3SuspendFPU = flaTwoPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEA);
																}

																flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, uwD3SuspendFPU, 0, 0xFF);
															}
#endif
														}
														else {
#else
														{
#endif
															flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C80_A5_DW, 0, 0xFF);
														}

														FQ->ubWritePhaseRedoCmd = MARK_FPU_C80_NonC10C15;
														FQ->ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_MULTIPLANE_DMA_START;
													}
												}
											}
											else {
#if (TLC&&(!B0KB))
												if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
#if Hynix3DV6 || YMTC_JGS
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 1, WAIT_TRUE_READY);
#else
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 1, ((PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END)) ? WAIT_TRUE_READY : WAIT_CACHE_READY));
#endif
												}
												else
#endif
												{

													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C80_A5_DW_C10, 1, WAIT_TRUE_READY);
												}




												FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

												if ((ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) && gubGenProgramFailDebug && FQ->btGenFail) {
													gulRandomValue = getIntRandValue(100);
													if (FQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) {
														if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_SPECAILCASE_RATIO) {
															gulPara0x00.BitMap.btForceWriteFail = 1;
														}
													}
													else if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_RATIO) {
														gulPara0x00.BitMap.btForceWriteFail = 1;
													}
												}
												flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgFQLinkIndex), 0, 1, 0xFF);
												if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
													FQ->ubMTDepthCnt++;
												}
												FQ->FQPara.B.btCanRemove = 1;
												FQ->ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;
											}
#if MicronFlashOnly
											if (gubMicronModeSet) {
												gubMicronModeSet = 0;
											}
#endif
											break;
										case BYTE_CEW_1PLANE_CACHE_START:
										case BYTE_CEW_1PLANE_CACHE_CONTINUOUS:

#if Hynix
#else
											// WaitCacheReady
											flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C80_A5_DW_C15, 1, WAIT_CACHE_READY);
#endif

											PreviousFQ->FQPara.B.btCanRemove = 1;

											// 要綁CheckWrite
											FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

											if ((ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) && gubGenProgramFailDebug && FQ->btGenFail) {
												gulRandomValue = getIntRandValue(100);
												if (PreviousFQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) {
													if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_SPECAILCASE_RATIO) {
														gulPara0x00.BitMap.btForceWriteFail = 1;
													}
												}
												else if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_RATIO) {
													gulPara0x00.BitMap.btForceWriteFail = 1;
												}
											}
											flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgFQLinkIndex), 0, 1, 0xFF);
											if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
												FQ->ubMTDepthCnt++;
											}
											//FQ->ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;	// 需要support one plane時要再check
											gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_1PLANE_CACHE_CONTINUOUS;
											break;

											//2 20160118 Fix 3plane program with 4-plane write command
										case BYTE_CEW_4PLANE_DMA2_CONTINUOUS:
										case BYTE_CEW_4PLANE_DMA6_CONTINUOUS:
											if (ubSequentialFQNum < 2) { //80 DMA + 11 81 DMA + 1 sequential = 3sequential commands
												if (gubFQNeedExecuteRightNow[ubCEIndex]) { // 4P已做了2個plane, 雖然有3rd plane, 但再下一個就已確定不是write or 沒有FQ了, 所以要直接補10, 3rd plane用80 DMA 10做掉
													goto Mark_ForceCmd10ToAvoidThreePlaneWrite;
												}
												else {	// 4P 已做了2個plane, 目前確定有3rd plane, 在還沒gubFQNeedExecuteRightNow[ubCEIndex] == 0 下代表可以再繼續等FQ
													goto Mark_ContinueWaitFQ;	//Need check more FQ in
												}
											}
											// else ubSequentialFQNum >= 2
											// 若BYTE_CEW_4PLANE_DMA2_CONTINUOUS/BYTE_CEW_4PLANE_DMA6_CONTINUOUS 有2個以上的SeqFQ, 就往下跟BYTE_CEW_MULTIPLANE_DMA_START/BYTE_CEW_4PLANE_DMA5_CONTINUOUS做一樣的事
										case BYTE_CEW_MULTIPLANE_DMA_START:
										case BYTE_CEW_4PLANE_DMA5_CONTINUOUS:

											if (guoFlashUseType.B.btFourPlane) {
												// WaitCacheReady
#if MicronFlashOnly
#if B0KB
												if ((FQ->ubD3WriteMode & D3_SUSPEND_EN) && (FQ->uwFJobInfo & BIT_FJOBI_NON_DMA)) {
													// For internal copy back
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C85_A5_C11_C70_POL_MK40, 0, 0xFF);
												}
												else
#endif
												{
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW, 0, 0xFF);
												}

#elif Hynix3DV6 || Hynix3DV5

												if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
													uwD3SuspendFPU = flaFourPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEB);
												}
												else {
													uwD3SuspendFPU = FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW;
												}
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, uwD3SuspendFPU, 0, 0xFF);
#else
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW, 0, 0xFF);
#endif
												FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_NonC10C15;
												PreviousFQ->FQPara.B.btCanRemove = 1;	// for 80h xxxxx DMA, 11h WaitDummyReady 81h xxxxx DMA,
												switch (gFQI.ubXferPhase[ubCEIndex]) {
													case BYTE_CEW_MULTIPLANE_DMA_START:
														FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_4PLANE_DMA2_CONTINUOUS;
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_4PLANE_DMA2_CONTINUOUS;
														break;
													case BYTE_CEW_4PLANE_DMA2_CONTINUOUS:
														FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_4PLANE_DMA3_CONTINUOUS;
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_4PLANE_DMA3_CONTINUOUS;
														break;
													case BYTE_CEW_4PLANE_DMA5_CONTINUOUS:
														FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_4PLANE_DMA6_CONTINUOUS;
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_4PLANE_DMA6_CONTINUOUS;
														break;
													case BYTE_CEW_4PLANE_DMA6_CONTINUOUS:
														FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_4PLANE_DMA7_CONTINUOUS;
														gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_4PLANE_DMA7_CONTINUOUS;
														break;
													default:
														M_AssertError(1);
														break;
												}
											}
											else if (guoFlashUseType.B.btTwoPlane) {
												// WaitCacheReady

												if ((ENABLE_CACHEPROGRAM == 0) || (ENABLE_PSEUDO_CE && gubExpandCE) || (B0KB && (FQ->ubD3WriteMode & D3_SUSPEND_EN))) {	// program without cache時, 這邊直接補10做掉
#if TLC
													if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
#if B0KB
														flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, (FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? FPU_PTR_AG_C85_A5_C10 : FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C10, 1, WAIT_TRUE_READY);
														FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_C10C15;
														goto Mark_2Plane_True_Program_CheckFail;
#else
														if (FQ->btIsMixPlaneUnit || FQ->btTLCProgramCodeBlock) {
#if Hnyix3DV6 || YMTC_JGS
															flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 0, WAIT_TRUE_READY);
#else
															flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaOnePlaneD3WriteChangFPU(FQ->ubD3WriteMode), 0, ((PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END)) ? WAIT_TRUE_READY : WAIT_CACHE_READY));
#endif

														}
														else {
															flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaTwoPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEB), 1, \
															                  ((PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
														}

														//((PreviousFQ->ubD3WriteMode & D3_WRITE_FINE) && (PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
#endif
													}
													else {
#else
													{
#endif
														flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW, 0, 0xFF);
													}

													FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_NonC10C15;
													FQ->ubRedundantDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

													flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, FPU_PTR_C10, 1, 1, WAIT_TRUE_READY);
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
													gubDebugSendCmdFQIndex = FQ->ubL4kIndex;
#endif
												Mark_2Plane_True_Program_CheckFail:
													FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

													flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, flaReadFailStatusFPUPointer(FQ->ubL4kIndex), 0, 1, 0xFF); // for 4 plane, 若有fail, 再下71h/73h去看哪個plane fail

													if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
														if (B0KB && (FQ->ubD3WriteMode & D3_SUSPEND_EN)) {
															FQ->ubMTDepthCnt += 1;	// only push checkFail MT
														}
														else {
															FQ->ubMTDepthCnt += 2;
														}
													}
													M_AssertError(FQ->FQPara.B.btCanRemove);
													M_AssertError(FQ->ubFJob != BYTE_FJOB_WRITE);

													FQ->FQPara.B.btCanRemove = 1;

													PreviousFQ->FQPara.B.btCanRemove = 1;	// for 80h xxxxx DMA

													FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START;

													gFQI.ubXferPhase[ubCEIndex] = 0;
												}
												else {
#if MicronFlashOnly
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15, 1, WAIT_CACHE_READY);
#else
#if TLC
													if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
														flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, flaTwoPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEB), 1, \
														                  ((PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
														// ((PreviousFQ->ubD3WriteMode & D3_WRITE_FINE) && (PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
													}
													else {
#else
													{
#endif
														flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15, 1, WAIT_CACHE_READY);
													}
#endif

													FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_C10C15;
													PreviousFQ->FQPara.B.btCanRemove = 1;	// for 80h xxxxx DMA
													FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START;
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_2PLANE_CACHE_START;
												}
											}
											break;
										case BYTE_CEW_2PLANE_CACHE_START:
										case BYTE_CEW_2PLANE_CACHE_CONTINUOUS:

										case BYTE_CEW_4PLANE_CACHE_START:
										case BYTE_CEW_4PLANE_CACHE_CONTINUOUS	:
#if TLC
											if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
#if B0KB
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, (FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? FPU_PTR_AG_C85_A5_C11_C70_POL_MK40 : FPU_PTR_AG_C80_A5_DW, 0, 0xFF);
#else

												if (guoFlashUseType.B.btFourPlane) {
													uwD3SuspendFPU = flaFourPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEA);
												}
												else {
													uwD3SuspendFPU = flaTwoPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEA);
												}

												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, uwD3SuspendFPU, 0, 0xFF);
#endif
											}
											else {
#else
											{
#endif
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C80_A5_DW, 0, 0xFF);
											}

											FQ->ubWritePhaseRedoCmd = MARK_FPU_C80_NonC10C15;
											PreviousFQ->FQPara.B.btCanRemove = 1;	// for 81h xxxxx DMA 15h
											switch (gFQI.ubXferPhase[ubCEIndex]) {
												case BYTE_CEW_2PLANE_CACHE_START:
												case BYTE_CEW_2PLANE_CACHE_CONTINUOUS:
													FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_2PLANE_DMA_CONTINUOUS;
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_2PLANE_DMA_CONTINUOUS;
													break;
												case BYTE_CEW_4PLANE_CACHE_START:
												case BYTE_CEW_4PLANE_CACHE_CONTINUOUS:
													FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_4PLANE_DMA5_CONTINUOUS;
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_4PLANE_DMA5_CONTINUOUS;
													break;
												default:
													M_AssertError(1);
													break;
											}
											break;
										case BYTE_CEW_2PLANE_DMA_CONTINUOUS:
										case BYTE_CEW_4PLANE_DMA7_CONTINUOUS:

											if ((ENABLE_CACHEPROGRAM == 0) || (ENABLE_PSEUDO_CE && gubExpandCE)) {
												M_AssertError(1);	// 不開cache或開pseudo CE時不會有cache continue xferPhase的狀況進入
											}
											// WaitCacheReady
#if MicronFlashOnly
#if B0KB
											if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, (FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? FPU_PTR_AG_C85_A5_C10 : FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C10, 1, WAIT_TRUE_READY);
											}
											else
#endif
											{
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15, 1, WAIT_CACHE_READY);
											}

#else
#if TLC
											if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {

												if (guoFlashUseType.B.btFourPlane) {
													uwD3SuspendFPU = flaFourPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANED);
												}
												else {
													uwD3SuspendFPU = flaTwoPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANEB);
												}
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, uwD3SuspendFPU, 1, \
												                  ( (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)  || (PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
												//((PreviousFQ->ubD3WriteMode & D3_WRITE_FINE) && (PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
											}
											else {
#else
											{
#endif
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15, 1, WAIT_CACHE_READY);
											}
#endif


											FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_C10C15;
											PreviousFQ->FQPara.B.btCanRemove = 1;

											// 要綁CheckWrite
											FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];  // 2p/4p cache 開始後, 11h 81h xxxxx DMA 15h CW若後續waitFQ fail, 就立即再等到true ready

											if ((ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) && gubGenProgramFailDebug && FQ->btGenFail) {
												gulRandomValue = getIntRandValue(100);
												if (PreviousFQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) {
													if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_SPECAILCASE_RATIO) {
														gulPara0x00.BitMap.btForceWriteFail = 1;
													}
												}
												else if (gulRandomValue < ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_RATIO) {
													gulPara0x00.BitMap.btForceWriteFail = 1;
												}
											}

#if BICS2_GC_TO_D3
											if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
												flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, ((FQ->ubD3WriteMode & D3_WRITE_FINE) && (FQ->ubD3WriteMode & (D3_WRITE_UPPER | D3_WRITE_UPPER_END))) ? flaReadFailStatusFPUPointer(ubgFQLinkIndex) /*FINE & UPPER Polling Fail*/ : FPU_PTR_C70_POL_MK40 , 0, 1, 0xFF);
											}
											else {
												flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgFQLinkIndex), 0, 1, 0xFF);  // plane A/B/C/D
											}
#else /* BICS2_GC_TO_D3 */
											flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgFQLinkIndex), 0, 1, 0xFF);  // plane A/B/C/D
#endif
											if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
												FQ->ubMTDepthCnt++;
											}
											switch (gFQI.ubXferPhase[ubCEIndex]) {
												case BYTE_CEW_2PLANE_DMA_CONTINUOUS:
													FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_CONTINUOUS;
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_2PLANE_CACHE_CONTINUOUS;
													break;
												case BYTE_CEW_4PLANE_DMA7_CONTINUOUS:
													FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_CONTINUOUS;
													gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_4PLANE_CACHE_CONTINUOUS;
													break;
												default:
													M_AssertError(1);
													break;
											}
											break;
										case BYTE_CEW_4PLANE_DMA3_CONTINUOUS:
											// WaitCacheReady
											if ((ENABLE_CACHEPROGRAM == 0) || (ENABLE_PSEUDO_CE && gubExpandCE) || (B0KB && (FQ->ubD3WriteMode & D3_SUSPEND_EN))) {	// program without cache時, 這邊直接補10做掉
#if MicronFlashOnly
#if B0KB
												if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, (FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? FPU_PTR_AG_C85_A5_C10 : FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C10, 1, WAIT_TRUE_READY);
													FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_C10C15;
													goto Mark_4Plane_True_Program_CheckFail;
												}
												else
#endif
												{
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW, 0, 0xFF);
												}

#else
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW, 0, 0xFF);
#endif
												FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_NonC10C15;
												FQ->ubRedundantDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
												flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, FPU_PTR_C10, 1, 1, WAIT_TRUE_READY);
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
												gubDebugSendCmdFQIndex = FQ->ubL4kIndex;
#endif
											Mark_4Plane_True_Program_CheckFail:
												FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

												flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, flaReadFailStatusFPUPointer(FQ->ubL4kIndex), 0, 1, 0xFF); // for 4 plane, 若有fail, 再下71h/73h去看哪個plane fail

												if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
													if (B0KB && (FQ->ubD3WriteMode & D3_SUSPEND_EN)) {
														FQ->ubMTDepthCnt += 1;	// only push checkFail MT
													}
													else {
														FQ->ubMTDepthCnt += 2;
													}
												}
												M_AssertError(FQ->FQPara.B.btCanRemove);
												M_AssertError(FQ->ubFJob != BYTE_FJOB_WRITE);

												FQ->FQPara.B.btCanRemove = 1;

												PreviousFQ->FQPara.B.btCanRemove = 1;	// for 81h xxxxx DMA

												FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START;

												gFQI.ubXferPhase[ubCEIndex] = 0;
											}
											else {
#if MicronFlashOnly
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15, 1, WAIT_CACHE_READY);
#elif (Hynix3DV6 || Hynix3DV5)
												if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {

													uwD3SuspendFPU = flaFourPlaneD3WriteChangFPU(((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) ? 1 : 0 ), FQ->ubD3WriteMode, PLANED);
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, uwD3SuspendFPU, 1, \
													                  ( (Hynix3DV6 || Hynix3DV5) || (PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
													//((PreviousFQ->ubD3WriteMode & D3_WRITE_FINE) && (PreviousFQ->ubD3WriteMode & (D3_WRITE_UPPER_END))) ? WAIT_TRUE_READY /* FINE UPPER END */ : WAIT_CACHE_READY);
												}
												else {
													flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15, 1, WAIT_CACHE_READY);
												}




#else
												flaSramToFlash_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15, 1, WAIT_CACHE_READY);
#endif
												FQ->ubWritePhaseRedoCmd = MARK_FPU_NonC80_C10C15;
												PreviousFQ->FQPara.B.btCanRemove = 1;	// for 81h xxxxx DMA
												FQ->ubCheckStatusPhase = STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START;
												gFQI.ubXferPhase[ubCEIndex] = BYTE_CEW_4PLANE_CACHE_START;
											}
											break;
										default:
											M_AssertError(1);
											break;
									}
									ubSequentialFQNum--;
									if (ubSequentialFQNum) {
										ubMTQOverLimit = 0;
										if (ENABLE_DEBUG_MTREMAINCNT_REG) {
											if (ubCEIndex < 8) {
												ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
											}
											else {
												ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
											}
											if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
												ubMTQOverLimit = 1;
											}
										}
										else {
											if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
												ubMTQOverLimit = 1;
											}
										}
										if (ubMTQOverLimit) {
											if (gubFQNeedExecuteRightNow[ubCEIndex]) {
												gubFQNeedExecuteRightNow[ubCEIndex] = 0;
											}
											goto Mark_CheckRemoveCodition;
										}

										ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
										ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
										FQ = &gFQI.gFQLink[ubgFQLinkIndex];
										ubFQServeCount++;
										M_AssertError(FQ->ubFJob != BYTE_FJOB_WRITE);
										goto Mark_NextWrite;
									}
									else {
										if (gubFQNeedExecuteRightNow[ubCEIndex]) {
											gubFQNeedExecuteRightNow[ubCEIndex] = 0;
											if (gFQI.ubXferPhase[ubCEIndex]) { // non-initial phase才需要補cmd, 若等到幾個連續的FQ, 但不是從planeA開始, 則都只下80 10而CE phase=0

												ubMTQOverLimit = 0;
												if (ENABLE_DEBUG_MTREMAINCNT_REG) {
													if (ubCEIndex < 8) {
														ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
													}
													else {
														ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
													}
													if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
														goto Mark_CheckRemoveCodition;
													}
												}
												else {
													if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
														goto Mark_CheckRemoveCodition;
													}
												}
												ubLastCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + gFQI.ubFQDoingNumber[ubCEIndex] - 1) & FQ_NUM_MASK;
												if (ubCFQIndex == ubLastCFQIndex) {
													gubNoFQIn[ubCEIndex] =  1; // flaCheckFQContinueNum加入ubNeedFQNoWait後, write要當下處理, 這邊要當成跟NoFQIn進來一樣的處理
												}
												ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
												ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
												FQ = &gFQI.gFQLink[ubgFQLinkIndex];
												ubFQServeCount++;
												goto Mark_NextWrite;
											}
										}
									}
								}
								break;
							case BYTE_FJOB_ERASE_BLOCK:
								if (ENABLE_DEBUG_RW) {
									M_AssertError(ubCFQIndex != ( ( gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount)& FQ_NUM_MASK) );
								}
								if (ENABLE_iFSA == 0) {
									flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
								}
#if MicronFlashOnly
								gubMicronModeSet = 0;
								if ( ENABLE_MICRON_SLC && (guoFlashUseType.B.btIntelMicron && guoFlashUseType.B.btA2Cmd)) {
									//Check SLC or MLC mode
									UWORD uwSLCAccess = 0;
									UWORD uwFPUPointer;
#if B0KB
									if ( (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (FQ->btD3 == 0) ) {
										uwSLCAccess = BIT_FJOBI_FASTPAGE;
									}
#else
									uwSLCAccess = FQ->uwFJobInfo;

#endif
#if Force_MLC_Erase
									uwSLCAccess &= (~BIT_FJOBI_FASTPAGE);
#endif
									// Micron flash mode change
									gubMicronModeSet = 1;
									FQ->ubInExitSLCDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
									if ((uwSLCAccess & BIT_FJOBI_FASTPAGE) == 0) {
										//switch to MLC
										gubMicronMode[ubCEIndex] = MICRON_MLC;
										uwFPUPointer = FPU_PTR_CDF;
									}
									else {
										//switch to SLC
										gubMicronMode[ubCEIndex] = MICRON_SLC;
										uwFPUPointer = FPU_PTR_CDA;
									}
									flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, uwFPUPointer, 0, 0, 0xFF);
									FQ->ubMTDepthCnt ++;	// for add DA&DF MTQ number
								}
#elif ENABLE_RESET_RETRY_LEVEL
								if (((BIT0 << ubCEIndex) & gulKeepRetryLevelCE_BITMAP) && ((guoFlashUseType.B.btSandisk && TLC_BICS2) || ((!TLC) && (!guoFlashUseType.B.btSandisk)))) {
									flaSwitchDefaultRetryLevel(ubgFQLinkIndex, ubCEIndex);
								}
#endif
#if TLC
								flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, 1);
#else
								flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C60_A3_CD0);
#endif
								FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

								flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgFQLinkIndex), 0, 1, 0xFF);
#if MicronFlashOnly
								if (gubMicronModeSet) {
									gubMicronModeSet = 0;
								}
#endif
								FQ->FQPara.B.btCanRemove = 1;
								if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
									FQ->ubMTDepthCnt++;
								}
								break;
							case BYTE_FJOB_ERASE_UNIT:


								if (ENABLE_DEBUG_RW) {
									M_AssertError(ubCFQIndex != ( ( gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount)& FQ_NUM_MASK) );
								}
								if (ENABLE_iFSA == 0) {
									flaSetFlashSourceAddrCE_CH(ubgFQLinkIndex);
								}
#if MicronFlashOnly
								gubMicronModeSet = 0;
								if ( ENABLE_MICRON_SLC && (guoFlashUseType.B.btIntelMicron && guoFlashUseType.B.btA2Cmd)) {
									//Check SLC or MLC mode
									UWORD uwSLCAccess = 0;
									UWORD uwFPUPointer;
#if B0KB
									if ( (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (FQ->btD3 == 0) ) {
										uwSLCAccess = BIT_FJOBI_FASTPAGE;
									}
#else
									uwSLCAccess = FQ->uwFJobInfo;
#endif
#if Force_MLC_Erase
									uwSLCAccess &= (~BIT_FJOBI_FASTPAGE);
#endif
									// Micron flash mode change
									gubMicronModeSet = 1;
									FQ->ubInExitSLCDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
									if ((uwSLCAccess & BIT_FJOBI_FASTPAGE) == 0) {
										//switch to MLC
										gubMicronMode[ubCEIndex] = MICRON_MLC;
										uwFPUPointer = FPU_PTR_CDF;
									}
									else {
										//switch to SLC
										gubMicronMode[ubCEIndex] = MICRON_SLC;
										uwFPUPointer = FPU_PTR_CDA;
									}
									flaSendCmd_CH(FQ->ubL4kIndex, ubCEIndex, uwFPUPointer, 0, 0, 0xFF);
									FQ->ubMTDepthCnt ++;	// for add DA&DF MTQ number
								}
#elif ENABLE_RESET_RETRY_LEVEL
								if (((BIT0 << ubCEIndex) & gulKeepRetryLevelCE_BITMAP) && ((guoFlashUseType.B.btSandisk && TLC_BICS2) || ((!TLC) && (!guoFlashUseType.B.btSandisk)))) {
									flaSwitchDefaultRetryLevel(ubgFQLinkIndex, ubCEIndex);
								}
#endif
								if (guoFlashUseType.B.btFourPlane) {
#if TLC
									flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, 4);
#else
									flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0);
#endif
								}
								else if (guoFlashUseType.B.btTwoPlane) {
#if TLC
									flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, 2);
#else
									flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C60_A3_AG_C60_A3_CD0);
#endif
								}
								else {
#if TLC
									flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, 1);
#else
									flaEraseTarget_CH(BYTE_TRIG_DMA_DONE, ubgFQLinkIndex, FPU_PTR_AG_C60_A3_CD0);
#endif
								}
								FQ->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

								if ((ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL) && gubGenEraseFailDebug && FQ->btGenFail) {
									gulRandomValue = getIntRandValue(100);
#if (TLC && (ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL))
									UBYTE ubGenForceEraseFail = 0;
									switch (ENABLE_DEBUG_HW_FORCEERASEFAIL_DATA_REGION) {	//0: Default, 1: D1 Only, 2: D3 Only
										case 1: /* D1 Only */
											ubGenForceEraseFail = ubGenForceEraseFail = (FQ->btD3 == 0);
											break;
										case 2: /* D3 Only (Including D3 with A2) */
											ubGenForceEraseFail = (FQ->btD3 == 1);
											break;
										case 0: /* Default */
										default: /* Default */
											ubGenForceEraseFail = 1;
											break;
									}

									if (ubGenForceEraseFail && (gulRandomValue < ENABLE_DEBUG_HW_FORCEERASEFAIL_RATIO))
#else
									if (gulRandomValue < ENABLE_DEBUG_HW_FORCEERASEFAIL_RATIO)
#endif
									{
										/* start if ubGenForceEraseFail */
										gulPara0x00.BitMap.btForceWriteFail = 1;
									} /* end if ubGenForceEraseFail */
								}

								flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, flaReadFailStatusFPUPointer(ubgFQLinkIndex), 0, 1, 0xFF);
#if MicronFlashOnly
								if (gubMicronModeSet) {
									gubMicronModeSet = 0;
								}
#endif
								FQ->FQPara.B.btCanRemove = 1;
								if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
									FQ->ubMTDepthCnt++;
								}
								break;
							case BYTE_FJOB_SETVALUE:
								if (FQ->FQPara.B.btRWTableUnit) {
									mDMAC_SV(4, FQ->ulRAMAddr, 512 << gubSectorsPerPlaneLog, 0, gulTable_Invalid, 0, DMA_EN_SETCQ, 1);
									if (FQ->FQPara.B.btHandleTableFlag) {
										gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btTableInRAMFlag = 1; //not to change valid at flash layer
									}
								}
								else {
									for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
										UBYTE ub4kBufferIndex = (FQ->ubBufferInedex_R + ub4kIndex) & gulBuffer2Read4KMask;
										UBYTE ubSectorIndex;
										if (((FQ->ulDataMask >> (ub4kIndex * 8)) & 0xFF) == 0xFF) {
											mDMAC_SV(1, (U32)&gubBuffer2[(ub4kBufferIndex * 8) * 512], 4096, ((FQ->ulSpare + ub4kIndex) << SectorsPer4KLog), 0, 0, 0, 1);
										}
										else {
											for (ubSectorIndex = 0; ubSectorIndex < 8; ubSectorIndex++) {
												if ((FQ->ulDataMask >> (ub4kIndex * 8)) & (BIT0 << ubSectorIndex)) {
													mDMAC_SV(1, (U32)&gubBuffer2[((ub4kBufferIndex * 8) + ubSectorIndex) * 512], 512, ((FQ->ulSpare + ub4kIndex) << SectorsPer4KLog) + ubSectorIndex, 0, 0, 0, 1);
												}
											}
										}
										if (FQ->FQPara.B.btUserData) {
											if (gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] == 0xFF) {
												gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] = 0;
											}
											else {
												gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] ++;
											}
										}
									}
									if (FQ->FQPara.B.btUserData) {
										gubRFQCnt --;
									}
								}
								if (FQ->uwFJobInfo & BIT_FJOBI_WAITRESULT) {
									FQ->ubFQPhase = BYTE_FQ_WAITCHECK;
								}
								else {
									FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
								}
								break;
							case BYTE_FJOB_COPYFROMDRAM:
								if (FQ->FQPara.B.btHandleTableFlag) {
									mDMAC_COPY((U32)&gulL2PBase[guwL2PEntrysPerL2PGroup * FQ->uwSourceTableInRAMIndex], (U32)&gulL2PBase[guwL2PEntrysPerL2PGroup * FQ->uwDestTableInRAMIndex], guwL2PEntrysPerL2PGroup * 4, 0, 0, 0, 0, 1);
									if (ENABLE_DEBUG_RW) {
										if (gulL2PBase[guwL2PEntrysPerL2PGroup * FQ->uwSourceTableInRAMIndex].All != gulL2PBase[guwL2PEntrysPerL2PGroup * FQ->uwDestTableInRAMIndex].All) {
											/*UartString("\nMTFail2");
											UartWordHex(FQ->uwSourceTableInRAMIndex);
											UartWordHex(FQ->uwDestTableInRAMIndex);*/
											M_AssertError(1);
										}
									}
									gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btTableInRAMFlag = 1;
									gulL2P_InRamInverse[FQ->uwSourceTableInRAMIndex].All &= ~(BIT_FLAG_L2P | BIT_SAVING_L2P);
								}
								else {
									UBYTE ub4kIndex;
									for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
										U32 ulMask = ((FQ->ulDataMask >> (ub4kIndex * 8)) & 0xFF);
										if (ulMask != 0) {
											UBYTE ub4kBufferIndex = (FQ->ubBufferInedex_R + ub4kIndex) & gulBuffer2Read4KMask;
											if (FQ->btCopyFromWB) {
												UBYTE ubWriteBufferPTR = FQ->uwDataInSDRIndex[ub4kIndex] + BUFFER2_WRITE_4K_START_INDEX;
												UBYTE ubFlagBackUp = AXIB[ubWriteBufferPTR];
												UBYTE ubStartPTR;
												UBYTE ubIndex;
												UBYTE ubFirst = 0;
												UBYTE ubSectorCount = 0;
												for (ubIndex = 0; ubIndex < SectorsPer4K; ubIndex++) {
													if (ulMask >> (BIT0 * ubIndex) & 0x01) {
														if (ubFirst == 0) {
															ubStartPTR = ubIndex;
															ubFirst = 1;
														}
														ubSectorCount++;
													}
												}
												mDMAC_COPY(((U32)gubBuffer2 + (ubWriteBufferPTR * 4096)), (U32)(gubBuffer2 + (ub4kBufferIndex * 4096)), 4096, ((FQ->ulSpare + ub4kIndex) << SectorsPer4KLog), (0xFFFFFF00 | ulMask), 1, 0, 1);
												AXIB[ubWriteBufferPTR] = ubFlagBackUp;
											}
											else {
												mDMAC_COPY((U32)&gubSDRCache[FQ->uwDataInSDRIndex[ub4kIndex] * 4 * 1024], (U32)&gubBuffer2[(ub4kBufferIndex * 8) * 512], 4096, ((FQ->ulSpare + ub4kIndex) << SectorsPer4KLog), (0xFFFFFF00 | ulMask), 1, 0, 1);
											}
										}
									}
									if (FQ->FQPara.B.btUserData) {
										for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
											UBYTE ub4kBufferIndex = (FQ->ubBufferInedex_R + ub4kIndex) & gulBuffer2Read4KMask;
											if (gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] == 0xFF) {
												gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] = 0;
											}
											else {
												gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] ++;
											}
										}
										gubRFQCnt --;
									}
								}
								if (FQ->uwFJobInfo & BIT_FJOBI_WAITRESULT) {
									FQ->ubFQPhase = BYTE_FQ_WAITCHECK;
								}
								else {
									FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
								}
								break;
							default:
								M_AssertError(1);
								break;
						}
					}

					// send cmd done, currentFQIndex +1
					ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
					ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
					FQ = &gFQI.gFQLink[ubgFQLinkIndex];


					if (ENABLE_DEBUG_MTREMAINCNT_REG) {
						if (ubCEIndex < 8) {
							ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
						}
						else {
							ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
						}
						if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
							break;
						}
					}
					else {
						if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
							break;
						}
					}

				}
				// cache program/read 期間, 若一定次數進來都沒有FQ符合條件, 就中斷cmd sequence
				if ((ubCEIndex != gubMTQTotalPhyCE) && gFQI.ubFQDoingNumber[ubCEIndex] && (gFQI.ubXferPhase[ubCEIndex]) && (ubNewFQExecute == 0) && (ubFQServeCount >= gFQI.ubFQDoingNumber[ubCEIndex])) {
				Mark_ContinueWaitFQ:
					ubWaitFQCnt[ubCEIndex]++;
					UBYTE ubWaitCnt;
					if (gubFTLNoWait) {
						ubWaitCnt = 0;
					}
					else if ((gFQI.ubXferPhase[ubCEIndex] > 0) && (gFQI.ubXferPhase[ubCEIndex] < 20)) { //W
#if B0KB
						ubWaitCnt = (gubCENumber < 6) ? 200 : (1200 / gubCENumber);
#else
						if (enumWriteWaitCntMode == WaitCnt_Default) {
							ubWaitCnt = (gubCENumber >= 16) ? WaitCnt_16CE : WaitCnt_Less16CE;
							enumWriteWaitCntMode = ubWaitCnt;
						}
						else {
							ubWaitCnt = enumWriteWaitCntMode;
						}
#endif
					}
					else if ((gFQI.ubXferPhase[ubCEIndex] > 23) && (gFQI.ubXferPhase[ubCEIndex] < 28)) { //R
#if ENABLE_FQ_ALIGNMENT
						ubWaitCnt = (gubCENumber <= 4) ? 15 : 5;			// Former setting (<) will cause B0KB 2Die 4CE speed down to 4xx.(SR)
#elif (!TLC)
						UBYTE ubDieNumber = (gubExpandCE) ? (gubDieNumber * gubExpandCE_Ratio) : gubDieNumber;
						ubWaitCnt = ((guoFlashUseType.B.btFourPlane) && (ubDieNumber == 4)) ? 20 : (gubCENumber < 4) ? (40 / gubCENumber) : 5;
#else																		// Change to "<=" can let the speed up to normal performance. (5xx)
						ubWaitCnt = (gubCENumber < 4) ? (40 / gubCENumber) : 5;
#endif
					}
					else {
						ubWaitCnt = 0;
					}

					if (ubWaitFQCnt[ubCEIndex] > ubWaitCnt) {

						if (ENABLE_DEBUG_MTREMAINCNT_REG) {
							if (ubCEIndex < 8) {
								ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
							}
							else {
								ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
							}
							if (ubRemainQCnt >= gubMTEVENT_CNT_LIMIT) {
								goto Mark_CheckRemoveCodition;
							}
						}
						else {
							if (gubMTDepthPushNumber[ubCEIndex] >= gubMTEVENT_CNT_LIMIT) {
								goto Mark_CheckRemoveCodition;
							}
						}

						if (ubSequentialFQNum) { //等不滿的狀況
							ubNewFQExecute = 1;
							ubWaitFQCnt[ubCEIndex] = 0;
						}
						gubFQNeedExecuteRightNow[ubCEIndex] = 1; // 等不滿全部 or 等不到
						gubNoFQIn[ubCEIndex] = 1; // 等不滿 or 等不到都要設定
						goto Mark_NoFQIn;
					}
				}
			Mark_CheckRemoveCodition:
				// 當MTQ full時會直接跳到這, 原本flaCheckFQContinueNum得到的結果要在這清掉, 等下次輪到這CE時再重新決定gubFQNeedExecuteRightNow，gubFQNotSequential
				if (gulNotFinishPhase[ubCEIndex] == 0) {
					gubFQNeedExecuteRightNow[ubCEIndex] = 0;
				}
				else if (0) {
				Mark_Backup:
					ubSequentialFQNumBackup[ubCEIndex] = ubSequentialFQNum;
					ubBurstBankBackup[ubCEIndex] = ubBurstBank;
					ubPreviousFQIndexBackup[ubCEIndex] = ubPreviousFQIndex;
					ubgPreviousFQLinkIndexBackup[ubCEIndex] = ubgPreviousFQLinkIndex;
					ubCFQIndexBackup[ubCEIndex] = ubCFQIndex;
					ubgFQLinkIndexBackup[ubCEIndex] = ubgFQLinkIndex;
					ubCEXferPhaseBackup[ubCEIndex] = ubCEXferPhase;
				}
#if (FLUSHD1_PARTIAL_CE_ADVANCE && B0KB)
				//to check DMA done all FQ in this CE first when flushD1
				if (VT->gulFTLState.B.btDoingFlushD1) {
					UBYTE ubMaxCEInGroup;
					ubMaxCEInGroup  = (gubCENumber >> 1);
					ubMaxCEInGroup += (gubCENumber & BIT0) ? 1 : 0; //modified for odd CE case
					for (ubFQServeCount = 0; ubFQServeCount < gFQI.ubFQDoingNumber[ubCEIndex]; ubFQServeCount++) {
						ubCFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubFQServeCount) & FQ_NUM_MASK;
						ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
						FQ = &gFQI.gFQLink[ubgFQLinkIndex];
						if ((FQ->btLastProgram) && gubINTTable[FQ->ubRelativeDepth].B.btDMA_Done) {
							if (gubCENumber < FLUSHD1_PARTIAL_CENUM) { // 1group case
								gubFlushD1Flag |= (BIT0 << FQ->ubCEIndex);
							}
							else {// 2 group case
								M_AssertError(gubFlushD1Flag & (BIT0 << (FQ->ubCEIndex % ubMaxCEInGroup)));
								gubFlushD1Flag |= (BIT0 << (FQ->ubCEIndex % ubMaxCEInGroup));
							}
							FQ->btLastProgram = 0;
						}
					}
				}
#endif


				ubRemoveFQCount = 0;
				ubFQIndex = gFQI.ubFQFirst[ubCEIndex];
				while (1) {
					ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubFQIndex];
					gubDebugFQLinkIndex = ubgFQLinkIndex;
					FQ = &gFQI.gFQLink[ubgFQLinkIndex];
					UWORD uwCheckID = FQ->ubDepth;
					if ((FQ->ubDepth == 0xFF) && (FQ->ubFQPhase == BYTE_FQ_CHECKDONE) && (ubRemoveFQCount == 0)) {
#if FORCE_AUTOPOL_TIMEOUT
						// Do not clear force auto pol in this FQ
						gubDebugAutoPolDirectRemoveFQ = 1;
#endif
						goto Mark_Remove_FQ;
					}

#if FLUSHD1_PARTIAL_CE_ADVANCE
#if B0KB  				// Prevent  not check done before FQ removed

					if (VT->gulFTLState.B.btDoingFlushD1) {
						UBYTE ubMaxCEInGroup;
						ubMaxCEInGroup  = (gubCENumber >> 1);
						ubMaxCEInGroup += (gubCENumber & BIT0) ? 1 : 0; //modified for odd CE case
						if ((FQ->btLastProgram) && gubINTTable[FQ->ubRelativeDepth].B.btDMA_Done) {
							if (gubCENumber < FLUSHD1_PARTIAL_CENUM) { // 1group case
								gubFlushD1Flag |= (BIT0 << FQ->ubCEIndex);
							}
							else {// 2 group case
								M_AssertError(gubFlushD1Flag & (BIT0 << (FQ->ubCEIndex % ubMaxCEInGroup)));
								gubFlushD1Flag |= (BIT0 << (FQ->ubCEIndex % ubMaxCEInGroup));
							}
							FQ->btLastProgram = 0;
						}
					}
#else
					if ((FQ->ubD3WriteMode & D3_SUSPEND_EN)
					        && (FQ->ubD3WriteMode & (D3_WRITE_UPPER | D3_WRITE_UPPER_END))
					        && (FQ->ubFJob == BYTE_FJOB_WRITE)
					        && (!(gubFlushD1Flag & (BIT0 << (FQ->ubCEIndex % gubPartialDoingCENum))))) {

						if (ENABLE_PLANE_CH_CE) {
							ubBurstBank = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
						}
						else {
							ubBurstBank = (FQ->ulFEntry >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
						}

						if (((ubBurstBank == gubBurstsPerBankMask) || (FQ->btIsMixPlaneUnit)) && (gubINTTable[FQ->ubRelativeDepth].B.btAll_Done)) {
							// Write DMA done by CE
							gubFlushD1Flag |= BIT0 << (FQ->ubCEIndex % gubPartialDoingCENum);
						}
					}
#endif

#endif
#if TLC && (!Hynix3DV6) && (!Hynix3DV5) && (!YMTC_JGS)
					if (gubINTTable[uwCheckID].B.btAll_Done && (gubINTTable[uwCheckID].B.btAbort_Done == 0) && (FQ->ubDepth != 0xFF) && FQ->FQPara.B.btCanRemove && flaCheckD3CopyBackMTDone(ubgFQLinkIndex))
#else
					if (gubINTTable[uwCheckID].B.btAll_Done && (gubINTTable[uwCheckID].B.btAbort_Done == 0) && (FQ->ubDepth != 0xFF) && FQ->FQPara.B.btCanRemove)
#endif
					{
						if ((FQ->ubFQPhase == BYTE_TRIG_DMA_DONE) || (FQ->ubFQPhase == BYTE_FQW_ADDRDMA) || (FQ->ubFQPhase == BYTE_FQR_READFAIL)) {
							//M_DebugGPIO5_High();

							if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY) || (gubINTTable[FQ->ubDepth].B.btSTA)) {

								UBYTE ubSTAFailCnt = 0;
								UBYTE ubSTAFQLinkIndex[2] = {0xFF};

								UBYTE ubREDQSMissMatch = 0;
								// Timeout...due to DQS missmatch
								if ((gFREG[FCTLL_RD_CNT].W.W0 > gFREG[FCTLL_RD2_CNT].W.W0) && (gubINTTable[FQ->ubDepth].All & 0xF0) && (FQ->ubFJob == BYTE_FJOB_READ) && ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT)) {
									ubREDQSMissMatch = 1;
								}

								//2 MT stop 後, 不能馬上 讀 相關 CH Register 的值, 需等 channel idle

								// force empty to avoid program parity clear interrupt vector enable bit bug
								if (!ubREDQSMissMatch) {
									gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
								}

								if (ubREDQSMissMatch) {
								MARK_DQSCNT_MISMATCH:
									flaReadDQSREMismatchHandling(ubgFQLinkIndex);
								}
								else {
									// wait the channel idle for read retry
									while ((gFREG[FCTLL_FPU_TRIG].L & (TIMEOUT | SIGNOFF_BUSY | FPU_TRIG)) != 0) {
										if ((gubINTTable[FQ->ubRelativeDepth].All & 0xF8) && (FQ->ubFJob == BYTE_FJOB_WRITE) && ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT)) {
											goto MARK_PROGRAM_ERROR;
										}
										else if ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
											flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x70_FQ_flaTask, gFREG, CHECK_FQ_TIMEOUT);
											break;
										}
									}
									// wait the channel true idle when force empty
									gFREG[FCTLL_DBG_INF].L = (0x0C);
									while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));
									gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug
								}


								if ((gFREG[FCTLL_MAP_CFG].L & (CURRENT_FIFO_HAS_NEW | NEXT_FIFO_HAS_NEW))) {
									//出現有兩張 map, 需要事先存起來到對應 FQ 內,防止過程中被清掉, 以利後續查找
									// 發生 Error 使 MT Stop 不一定會 update FIFO Map: Timeout, Abort(w/o UNC), Over ECC(disable)
									// Ultra DMA Case: 一筆 UNC + 一筆 Abort. 其中 Abort 那筆除非也有 UNC 不然只會 update UNC 那筆 Error Map
									UBYTE ubFIFOInfoCnt = (gFREG[FCTLL_MAP_CFG].L & NEXT_FIFO_HAS_NEW) ? 2 : 1;
									UBYTE ubTempFIFOScanIndex = 0;

									if (ENABLE_DEBUG_UART_CC) {
										UartString("\n FIFO Cnt : ");
										Uart_Tx_DataHex(ubFIFOInfoCnt);
									}

									for (ubTempFIFOScanIndex = 0; ubTempFIFOScanIndex < ubFIFOInfoCnt; ubTempFIFOScanIndex++) {

										UBYTE ubTempIntVctDepth = 0;
										UBYTE ubMaxErrorFrameNum = 0;
										UBYTE ubSignOffMaskFail = 0;
										UBYTE ubNeedToCheckSTA = 0;
										UWORD uwSignOffMaskFailMap = 0;

										FlashQueue_t *FQTemp;

										// Get INT Depth for error FQ
										gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
										gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(INT_VCT_MAP);
										ubTempIntVctDepth = gFREG[FCTLL_MAP_INF].B.B0;

										if (gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(SIGNOFFMASK_MAP);
											uwSignOffMaskFailMap = gFREG[FCTLL_MAP_INF].W.W0;
											ubSignOffMaskFail = 1;
										}

										if (gFREG[FCTLL_INT_INF].L & STA_ERR_INF) { // 1st priority due to other map also setup at the same time of HW bug
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(STA_ERR_MAP);
											ubMaxErrorFrameNum = 1; // STA

											ubNeedToCheckSTA = 1;

											if (ENABLE_DEBUG_UART_CC && 0) {
												UartString("\n FIFO STA Err.");
												if (gubPreformat) {
													UartString(" @P");
												}
											}
										}
										else if (gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(LCA_FW_CMP_FAIL_MAP);
											ubMaxErrorFrameNum = 16; // Logi
										}
										else if (gFREG[FCTLL_INT_INF].L & DECOMPRESSION_FAIL_INF) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(DECOMPRESSION_FAIL_MAP);
											ubMaxErrorFrameNum = 16; // Logi
										}
										else if (gFREG[FCTLL_INT_INF].L & CRC16_ERROR) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC16_ERR_MAP);
											ubMaxErrorFrameNum = 32; // Logi
										}
										else if (gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(FW_SETTING_ERR_MAP);
											ubMaxErrorFrameNum = 6; // Zip
										}
										else if (gFREG[FCTLL_INT_INF].L & UNC_MAP_INF) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);
											ubMaxErrorFrameNum = 4; // Phy

											if (ENABLE_DEBUG_UART_CC && 0) {
												UartString("\n FIFO UNC Err.");
												if (gubINTTable[ubTempIntVctDepth].B.btAbort_Done) {
													UartString(" Abort!");
												}
											}
										}
										else if (gFREG[FCTLL_INT_INF].L & CRC32_ERROR) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC32_ERR_MAP);
											ubMaxErrorFrameNum = 4; // Phy
										}
										else if (gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
											ubMaxErrorFrameNum = 16; // Logi
										}
										else if (gFREG[FCTLL_INT_INF].L & ERASE_PAGE) {
											//no error frame num info
										}
										else if (gFREG[FCTLL_INT_INF].L & E3D_ERR_INF) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(E3D_ERR_MAP);
											ubMaxErrorFrameNum = 16; // Logi
										}
										else if (gFREG[FCTLL_INT_INF].L & COMPRESSION_FAIL_INF) {
											gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
											gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(COMPRESSION_FAIL_MAP);
											ubMaxErrorFrameNum = 16; // Logi
										}
										else {
											// over ecc, PFA not turn on now
											if (ENABLE_WP_DEBUG) {
												flaLEDBlink(0xEE);
											}
											else {
												M_AssertError(1);
												gubWP_DEBUG = 0xEE;
											}
										}

										UBYTE ubFirstError4KIndex = 0;
										UBYTE ubTempFQCount = 0;
										UBYTE ubTempFoundFQ = 0;
										UBYTE ubTempFQLinkIndex = ubgFQLinkIndex;

										M_Get_Error_4K_Index(ubFirstError4KIndex, ubMaxErrorFrameNum, gFREG[FCTLL_MAP_INF].L);

										if ((FQ->ubDepth == ubTempIntVctDepth) || (FQ->ubRemainderDepth == ubTempIntVctDepth) || (FQ->ubRelativeDepth == ubTempIntVctDepth)) {
											// 先用目前的 FQ 比對, 沒 match 再掃該 CE 所有 FQ
											// [NOTE]: Relative Depth 有可能會有 compression error, FW setting error, E3D error
											//	Remainder Depth for Cache Program
											FQTemp = FQ;
											ubTempFoundFQ = 1;
											ubTempFQLinkIndex = ubgFQLinkIndex;
											if (ENABLE_DEBUG_UART_CC && 0) {
												UartString("\n CurFQ FIFO");
											}
										}
										else {
											for (ubTempFQLinkIndex = gFQI.ubFQLinkFirst, ubTempFQCount = 0; ubTempFQCount < gFQI.ubFQLinkNumber; (ubTempFQLinkIndex = FQTemp->ubNext)) {
												FQTemp = &gFQI.gFQLink[ubTempFQLinkIndex];
												if ((FQTemp->ubDepth == ubTempIntVctDepth) || (FQTemp->ubRemainderDepth == ubTempIntVctDepth) || (FQTemp->ubRelativeDepth == ubTempIntVctDepth)) {
													ubTempFoundFQ = 1;
													break;
												}

												ubTempFQCount++;
											}
										}

										if (ubTempFoundFQ) {
											// Remainder Depth 的不用記FIP Error, 後面會處理直接 free, 待 Depth 判斷時再判斷是 previous or current page fail
											if ((FQTemp->ubDepth == ubTempIntVctDepth) || (FQTemp->ubRelativeDepth == ubTempIntVctDepth)) {
												FQTemp->ulFIPError.All = gFREG[FCTLL_INT_INF].L;
												FQTemp->btUncCheckInRSTwoCh = 1;
												if (ubSignOffMaskFail) { // For SignOffMask error map (user data)
													FQTemp->ulFIPError.W.uwSignoffMaskFailLog = uwSignOffMaskFailMap;
												}
												FQTemp->ubFirstUncFrameNoMapInRSTwoCh = ubFirstError4KIndex;
											}
											else {
												if (ENABLE_DEBUG_UART_CC && 0) {
													UartString("\n Remainder FQ FIFO");
												}
											}

											if (ubNeedToCheckSTA && gubINTTable[FQTemp->ubDepth].B.btSTA) {
												ubSTAFQLinkIndex[ubSTAFailCnt] = ubTempFQLinkIndex; //record STA FQ Index and handle later
												ubSTAFailCnt++;	//record STA count and handle later

												if (ENABLE_DEBUG_UART_CC) {
													UartString("\n Check STA: 0x");
													Uart_Tx_DataHex(ubTempFQLinkIndex);
												}
											}
										}
										else {
											if (ENABLE_DEBUG_UART_CC) {
												UartString("\n FIFO FQ not found.");
											}

											M_AssertError(1);
										}

										// Use switch method to change FIFO point to another map.
										// Also reset pointer after 1 switching(for 1 map) / 2nd switching(for 2 maps).
										if ((ubFIFOInfoCnt == 2) && (ubTempFIFOScanIndex == 0)) {
											gFREG[FCTLL_MAP_CFG].L |= SWITCH_PINGPONG_FIFO_TO_NEXT_FRAME_MAP;
										}

									}

									// Make sure the FIFO pointer been reset
									//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
									gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);

								}

								//2 ====== Handle STA Fail Here ========

								if ((ubSTAFailCnt != 0) || (gubINTTable[FQ->ubDepth].B.btSTA)) {

									UBYTE ubTempSTACheckIndex = 0;
									UBYTE ubTempSTAFQLinkIndex = 0;
									UBYTE ubNoMapSTA = 0;
									FlashQueue_t *CheckStatusFQ;

									if (ubSTAFailCnt == 0) {
										ubNoMapSTA = 1;
										ubSTAFailCnt = 1;
									}

									for (ubTempSTACheckIndex = 0; ubTempSTACheckIndex < ubSTAFailCnt; ubTempSTACheckIndex++) {
										UBYTE ubCheckCE = 0;
										UBYTE ubTempChannel = 0;
										UBYTE ubCheckBurstBank = 0;
										UBYTE ubTempFailBurstBank = 0;
										UBYTE ubTempStatus = 0;

										if (ubNoMapSTA) {
											ubTempSTAFQLinkIndex = ubgFQLinkIndex;
											CheckStatusFQ = FQ;
										}
										else {
											ubTempSTAFQLinkIndex = ubSTAFQLinkIndex[ubTempSTACheckIndex];
											CheckStatusFQ = &gFQI.gFQLink[ubTempSTAFQLinkIndex];
										}

										if (ENABLE_DEBUG_UART_CC && 0) {
											UartString("\n Handle STA, 0x");
											Uart_Tx_DataHex(ubTempSTAFQLinkIndex);
											if (ubNoMapSTA) {
												UartString("\n No Map");
											}
										}

										ubCheckCE = CheckStatusFQ->ubCEIndex;
										M_VirtualToPhysicalCE(ubCheckCE);

										ubTempChannel = CheckStatusFQ->ubCEIndex & gubPlanesPerBurstMask;

#if FixShallowErase
										if (CheckStatusFQ->ubFJob == BYTE_FJOB_WRITE) {
											if (gubSkipProgramFail) {
												//CheckStatusFQ->ubFQPhase = BYTE_FQ_CHECKDONE; // Bug: 如果是cache program fail, 這邊不應該提早改CHECKDONE (此時ubDepth是後續的ubRemainderDepth)，
												// 導致後續POL fail自己這筆FQ時又出現STA沒有處理，被Assert
												if (gMTQ->Depth[CheckStatusFQ->ubDepth].Para0x00.BitMap.btStaNoStop == 0) {
													//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
													gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
													if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
														gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
													}
												}
												if (CheckStatusFQ->FQPara.B.btUserData) {
													//gulProgramUserDataCount --;
												}
												gubINTTable[CheckStatusFQ->ubDepth].All &= 0x0F;
												//break;
												continue;
											}
										}
#endif
										gubPlaneABDebug = 0;

										CheckStatusFQ->btPEFail = 1;
										if (ENABLE_PLANE_CH_CE) {
											ubCheckBurstBank = (CheckStatusFQ->ulFEntry  >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
										}
										else {
											ubCheckBurstBank = (CheckStatusFQ->ulFEntry  >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
										}

										if (CheckStatusFQ->ubFJob == BYTE_FJOB_WRITE) {

											if (ENABLE_DEBUG_UART_CC && 0) {
												UartString("\n Handle W");
											}
#if TLC
											// TLC
											UBYTE ubTempD1LinkUnit = Default_D1_Unit;

											if (CheckStatusFQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) {
												ubTempD1LinkUnit = CheckStatusFQ->ubFSAIndex;
											}
											else {
												ubTempD1LinkUnit = gubD1LinkTable[CheckStatusFQ->uwFUnit].B.ubD1Link;
											}

											if (CheckStatusFQ->btD3 == D3) {
												VT->gulProgramFailCnt_D3++;

											}
											else {
												if (ubTempD1LinkUnit != Default_D1_Unit) {
													VT->gulProgramFailCnt_D1++;
												}
												else {
													VT->gulProgramFailCnt_D3++;
												}
											}
#else
											// MLC
											VT->gulProgramFailCnt_D3++;
#endif

											flaCheckWriteStatus (ubTempSTAFQLinkIndex);

											//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
											gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
											if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
												gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
											}
											gubINTTable[CheckStatusFQ->ubDepth].All &= 0x0F;
										}
										else if (CheckStatusFQ->ubFJob == BYTE_FJOB_ERASE_UNIT) {

											if (ENABLE_DEBUG_UART_CC && 0) {
												UartString("\n Handle E U");
											}

											if (guoFlashUseType.B.btFourPlane || guoFlashUseType.B.btTwoPlane) {

												// force empty to avoid program parity clear interrupt vector enable bit bug
												gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);

												// wait the channel idle for read retry
												while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

												// wait the channel true idle when force empty
												gFREG[FCTLL_DBG_INF].L = (0x0C);
												while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));

												gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug

												FCONB[FCONB_FCE_ENB_CH0 + ubTempChannel] = 0;
#if(UFS)
												if (1)
#else
												if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
												{
													FCONB[FCONB_FCE_ENB_CH0 + ubTempChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubTempChannel][(ubCheckCE / gubPlanesPerBurst)]); // Enable selected CE
												}
												else {
													FCONL[FCONL_FCE_SET] = (ubTempChannel * 8) + (ubCheckCE / gubPlanesPerBurst); // Enable selected CE
												}
												//Mark_Check_PlaneA_PlaneB_Erase:
												//Get Status
#if MicronFlashOnly
												//Check Plane A and Plane B status
												ubTempStatus = flaMicronCheckWriteStatus(ubTempSTAFQLinkIndex, 0);

#elif  Hynix
												ubTempStatus = FlaHynixCheckStatusPerPlane(ubTempSTAFQLinkIndex, 0);
#else
												gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
#if TLC
												if (CheckStatusFQ->ubALUIndex == ALU_A2) {
													gFREG[FCTLL_PIO_CMD].L = 0x72;
												}
												else {
													gFREG[FCTLL_PIO_CMD].L = 0x71;
												}
#else
												gFREG[FCTLL_PIO_CMD].L = 0x71;
#endif
												UWORD uwCnt;
												for (uwCnt = 0; uwCnt < 5; uwCnt++) {	// 1000, 500, 200, 150 140 ok, 120 fail => 100 ok
													// need dummy delay tWHR (add a dummy read register)。
													ubTempStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;	// for dealy tWHR
												}
												ubTempStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;
#endif


												if (ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL) {
													UBYTE ubFailMode;	// 0: 不造fail, 1: 造bit1,0 fail, 2: 造bit2,0 fail, 3: 造bit2,1,0 fail

													// planeA, B是否造fail
													ubFailMode = getIntRandValue(3);
													if (guoFlashUseType.B.btTwoPlane) { // 2-plane sample至少要造planeA/B其中一個fail, 4-plane sample 可以不造planeA/B fail
														ubFailMode++;
													}

													if (ubFailMode == 1) {
														ubTempStatus |= (BIT1 | BIT0);	// planeA fail
													}
													if (ubFailMode == 2) {
														ubTempStatus |= (BIT2 | BIT0);	// planeB fail
													}
													if (ubFailMode == 3) {
														ubTempStatus |= (BIT2 | BIT1 | BIT0);	// planeA/B fail
													}
												}

												if (ubTempStatus & BIT0) {
													if (ubTempStatus & BIT1) {	// planeA fail
														ubTempFailBurstBank = 0;
														CheckStatusFQ->ubCheckStatusPhase |= BIT0;
														flaMarkFail(ubTempSTAFQLinkIndex, 0, ubTempFailBurstBank - ubCheckBurstBank);
													}
													if (ubTempStatus & BIT2) {	// planeB fail
														ubTempFailBurstBank = 1;
														CheckStatusFQ->ubCheckStatusPhase |= BIT1;
														flaMarkFail(ubTempSTAFQLinkIndex, 0, ubTempFailBurstBank - ubCheckBurstBank);
													}
													gubPlaneABDebug = 1;	// PlaneA/B已經有發生error
												}
												else if (guoFlashUseType.B.btTwoPlane) {

													CheckStatusFQ->btPEFail = 0;
													ftlRecordFailLog(FLHErrLogType, VT_ERASE_STATUS_MISMATCH_0xB0 , FQ->ubL4kIndex , DONTCARE , DONTCARE, BIT_NormalErr);
												}
											}
											else {
												//one plane fail
												flaMarkFail(ubTempSTAFQLinkIndex, 0, 0); // swap RUT過程又發生fail的case, 例如program RUT or SaveVT也會在這MarkFail
											}
											if (guoFlashUseType.B.btFourPlane) {
												//Mark_Check_PlaneC_PlaneD_Erase:
												//Get Status
#if MicronFlashOnly
												//Check Plane C and Plane D status
												ubTempStatus = flaMicronCheckWriteStatus(ubTempSTAFQLinkIndex, 2);

#elif  Hynix
												ubTempStatus = FlaHynixCheckStatusPerPlane(ubgFQLinkIndex, 1);
#else
												gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
												gFREG[FCTLL_PIO_CMD].L = 0x73;
												UWORD uwCnt;
												for (uwCnt = 0; uwCnt < 5; uwCnt++) {	// 1000, 500, 200, 150 140 ok, 120 fail => 100 ok
													// need dummy delay tWHR (add a dummy read register)。
													ubTempStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;	// for dealy tWHR
												}
												ubTempStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;
#endif

												if (ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL) {
													UBYTE ubFailMode;	// 0: 不造fail, 1: 造bit1,0 fail, 2: 造bit2,0 fail, 3: 造bit2,1,0 fail

													// planeC, D是否造fail
													ubFailMode = getIntRandValue(3);
													if (gubPlaneABDebug == 0) { // 若planeA/B沒有發生fail, 但這邊進來就代表4 plane裡有fail, 所以這邊就一定要造planeC/D其中一個fail
														ubFailMode++;
													}

													if (ubFailMode == 1) {
														ubTempStatus |= (BIT1 | BIT0);	// planeC fail
													}
													if (ubFailMode == 2) {
														ubTempStatus |= (BIT2 | BIT0);	// planeD fail
													}
													if (ubFailMode == 3) {
														ubTempStatus |= (BIT2 | BIT1 | BIT0);	// planeC/D fail
													}
												}

												if (((ubTempStatus & BIT0) == 0) && (gubPlaneABDebug == 0)) {	// 不應該Cmd70後, BIT0 有爆fail, 結果all planes都沒有錯
													M_AssertError(1);
												}

												if (ubTempStatus & BIT1) {	// planeC fail
													ubTempFailBurstBank = 2;
													CheckStatusFQ->ubCheckStatusPhase |= BIT2;
													flaMarkFail(ubTempSTAFQLinkIndex, 0, ubTempFailBurstBank - ubCheckBurstBank);

												}
												if (ubTempStatus & BIT2) {	// planeD fail
													ubTempFailBurstBank = 3;
													CheckStatusFQ->ubCheckStatusPhase |= BIT3;
													flaMarkFail(ubTempSTAFQLinkIndex, 0, ubTempFailBurstBank - ubCheckBurstBank);
												}
												if (((ubTempStatus & 0x07) == 0) && (gubPlaneABDebug == 0)) {
													CheckStatusFQ->btPEFail = 0;
													ftlRecordFailLog(FLHErrLogType, VT_ERASE_STATUS_MISMATCH_0xB0 , FQ->ubL4kIndex, DONTCARE , DONTCARE, BIT_NormalErr);
												}
											}

											//2 one plane fail
											//M_AssertError(1);	// one plane sample
											//flaMarkFail(ubgFQLinkIndex, 0, 0); // swap RUT過程又發生fail的case, 例如program RUT or SaveVT也會在這MarkFail


										}
										else {	// BYTE_FJOB_ERASE_BLOCK
											flaMarkFail(ubTempSTAFQLinkIndex, 0, 0); //swap RUT過程又發生fail的case, 例如program RUT or SaveVT會走上面.
										}

										if ((CheckStatusFQ->ubFJob == BYTE_FJOB_ERASE_BLOCK) || (CheckStatusFQ->ubFJob == BYTE_FJOB_ERASE_UNIT)) {

											if (ENABLE_DEBUG_UART_CC && 0) {
												UartString("\n Handle E B&U");
											}

#if(UFS)
											if (1)
#else
											if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
											{
												FCONB[FCONB_FCE_ENB_CH0 + ubTempChannel] = 0;
											}
											else {
												FCONL[FCONL_FCE_CLR] = (ubTempChannel * 8) + (ubCheckCE / gubPlanesPerBurst); // Disable selected CE
											}
											//gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);//把模擬狀態復原


											// Must Send Reset CMD(0xFF) After Erase Fail
											if (CheckStatusFQ->uwFJobInfo & BIT_FJOBI_ERASE_FAIL_RESET) {
												flaEraseFailHandle(ubTempSTAFQLinkIndex);
											}

										}

										UBYTE ubFailType = DONTCARE;
										if (CheckStatusFQ->FQPara.B.btRWTableUnit) {
											if (CheckStatusFQ->ubFJob == BYTE_FJOB_WRITE) {
												ubFailType = BYTE_PF_WRITETABLE;
											}
											else {
												ubFailType = BYTE_EF_TABLE;
											}
										}
										else {
											if (CheckStatusFQ->ubFJob == BYTE_FJOB_WRITE) {
												ubFailType = BYTE_PF_XFERDATA;
											}
											else {
												ubFailType = BYTE_EF_GR;
											}
										}
										ftlRecordFailLog(FLHErrLogType, 0x1, ubTempSTAFQLinkIndex, ubFailType, DONTCARE, BIT_NormalErr);

										if (gubMPRW) {
											guoCE_FailMap |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(gFQI.gFQLink[ubTempSTAFQLinkIndex].ubCEIndex)));
											if (ubFailType == BYTE_PF_WRITETABLE || ubFailType == BYTE_PF_XFERDATA) {
												if ( (GetRTT_milis() - gulMPRWCMDInStartTime) > MPRW_CMDTIMEOUT) {
													flaDumpError(0, VT_ASSERT_MPRW_WRITEFAIL_0x96);
												}
											}
											else {
												if ( (GetRTT_milis() - gulMPRWCMDInStartTime) > MPRW_CMDTIMEOUT) {
													flaDumpError(0, VT_ASSERT_MPRW_ERASEFAIL_0x97);
												}
											}
										}


									}

								}

								//2  === Finish Handle STA Error ===


								//if (gubEnterReadFailRetryCondition != DQSCNT_MISMATCH) {
								// Refer to previous FORCE EMPTY for channel idle used, so we have to clear here.
								//}
								if (!ubREDQSMissMatch) {
									gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
								}
							}

							switch (FQ->ubFJob) {
								case BYTE_FJOB_READ:
#if TLC_NES_TEST_SB_FLOW
#if MicronFlashOnly || YMTC_JGS
									if ((gubNESReadIBF == 0x01) && gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
									if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
									{
										flaNES_SB_TESTFlow( gubDebugFQLinkIndex );
									}
#endif
									//FQ->ubECCStatus = (gFREG[FCTLL_ECC_INFO].B.B0 & 0xFF); // AUTION: ultra dma will change 3FF to zero (TODO: move to later)
									if ((ENABLE_READRETRY && (gubINTTable[FQ->ubDepth].All & 0xF0)) || ((BURNER || gubPreformat || gubExpectErasePage) && (gFREG[FCTLL_MAP_CFG].L & CURRENT_FIFO_HAS_NEW))) {

										gubEnterReadFailRetryCondition = 0xFE;
										gubRSclearMTQRebuildFQ = 0;

										if  ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {

											gubEnterReadFailRetryCondition = DQSCNT_MISMATCH;

#if (!BURNER)
											if (ENABLE_DEBUG_UART_DQS_MISSMATCH) {
												UartString("\nDQSmiss ");
											}

											// Clear all MTQ in channel
											gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
											gFREG[FCTLL_MT_TRIG].B.B1 = 0;

											//UBYTE ubTimeoutChannel = (FQ->ubCEIndex & gubPlanesPerBurstMask);

											if (ENABLE_DEBUG_UART_DQS_MISSMATCH) {
												UartString("\nFW_RST! ");
											}

											flaFIPTimeoutErrorHandle(FQ->ubCEIndex);

											if (ENABLE_DEBUG_UART_DQS_MISSMATCH) {
												UartString("\nTrig: ");
												UartLongHex(gFREG[FCTLL_FPU_TRIG].L);
											}

											if (ENABLE_DEBUG_UART_DQS_MISSMATCH) {
												UartString("\nEnter Retry");
											}
#endif

											VT->gulReadDQSREMismatchCnt++;
											gubUNCandLCAfailCondition = 0;
											if (ENABLE_DEBUG_BB_ITEM_CROSS_FRAME_ISSUE && ((gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) == LCA_FW_CMP_ERR_INF)) {
												//if (ENABLE_WP_DEBUG) {
												VT->gsReadFailDebugCount.gulUNCandLCAfail_DebugCount++;
												//}
												gubUNCandLCAfailCondition = 1;
											}
											if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
												flaLEDBlink(1);
											}
											else {
												M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
											}
											SYS1L[SYS1L_INT_EN3] &= (~(FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
											flaReadFailRetry(ubgFQLinkIndex);
											SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing FPU COR
											if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
												flaLEDBlink(1);
											}
											else {
												M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
											}
											SYS1L[SYS1L_INT_EN3] |= (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);
											gubINTTable[FQ->ubDepth].All &= 0x0F;
										}
										else if (((gFREG[FCTLL_INT_INF].L & ERASE_PAGE) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & ERASE_PAGE))) {
											gubEnterReadFailRetryCondition = ERASE_PAGE_ERR;
											if (FQ->btCheckEmpty) {
												FQ->btThisIsErasePage = 1;
											}
											if (!BURNER) {
												if (FQ->uwFJobInfo & BIT_FJOBI_RETRY) {
													if (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) {
														VT->gulReadErasePageErrorCnt++;
														ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Read_ErasePageErr);
													}
													if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
														flaLEDBlink(1);
													}
													else {
														M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
													}
													SYS1L[SYS1L_INT_EN3] &= (~(FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
#if  0 //Hynix Debug
													if ((gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE)) {
														flaLEDBlink(ERASE_PAGE_ERR);
													}
#endif
													flaReadFailRetry(ubgFQLinkIndex);

													SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing FPU COR
													if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
														flaLEDBlink(1);
													}
													else {
														M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
													}
													SYS1L[SYS1L_INT_EN3] |= (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);
												}
												else {
													VT->gulReadFailNotRetryJobDebugCnt++;
													flaRebuildFQPhase(ubgFQLinkIndex);
												}
											}
											else {
												gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
												gFREG[FCTLL_MT_TRIG].B.B1 = 0;
												//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
												gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
												if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
													gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
												}
											}
											gubINTTable[FQ->ubDepth].All &= 0x0F;
										}
										else {
											if (((gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & LCA_FW_CMP_ERR_INF))) {
												VT->gulReadLCAErrorCnt++;
												gubEnterReadFailRetryCondition = LCA_FW_CMP_FAIL_MAP;
												ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Read_LCAErr);
											}
											else if (((gFREG[FCTLL_INT_INF].L & DECOMPRESSION_FAIL_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & DECOMPRESSION_FAIL_INF))) {
												VT->gulDeCompressionErrorDebugCnt++;
												gubEnterReadFailRetryCondition = DECOMPRESSION_FAIL_MAP;
												ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Read_DecompressionErr);
											}
											else if (((gFREG[FCTLL_INT_INF].L & CRC16_ERROR) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & CRC16_ERROR))) {
												VT->gulCRC16ErrorDebugCnt++;
												gubEnterReadFailRetryCondition = CRC16_ERR_MAP;
												ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Read_CRC16Err);
											}
											else if (((gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & FW_SETTING_ERR_INF))) {
												VT->gulReadFWSettingErrorCnt++;
												gubEnterReadFailRetryCondition = FW_SETTING_ERR_MAP;
												ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Read_FWSettingErr);
											}
											else if (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & SIGNOFFMASK_INF)) {
												// 一開始就signoffmask fail
												if (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & UNC_MAP_INF)) {
													if (flaCheckMTFirstReadFailL4KDuetoFakeSignoffMaskFailorNot(ubgFQLinkIndex)) {
														// Fake signoffmask fail
														goto MARK_MT_UNC;
													}
												}
											}
											else if (gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
												// 一開始就signoffmask fail
												if (gFREG[FCTLL_INT_INF].L & UNC_MAP_INF) {
													if (flaCheckMTFirstReadFailL4KDuetoFakeSignoffMaskFailorNot(ubgFQLinkIndex)) {
														// Fake signoffmask fail
														goto MARK_MT_UNC;
													}
												}
											}
											else if (((gFREG[FCTLL_INT_INF].L & UNC_MAP_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & UNC_MAP_INF))) {
											MARK_MT_UNC:
												gubUNCandLCAfailCondition = 0;
												if (ENABLE_DEBUG_BB_ITEM_CROSS_FRAME_ISSUE && ((gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) == LCA_FW_CMP_ERR_INF)) {
													//if (ENABLE_WP_DEBUG) {
													VT->gsReadFailDebugCount.gulUNCandLCAfail_DebugCount++;
													//}
													gubUNCandLCAfailCondition = 1;
												}

												gubEnterReadFailRetryCondition = UNC_MAP;
											}
											else if (((gFREG[FCTLL_INT_INF].L & CRC32_ERROR) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & CRC32_ERROR))) {
												VT->gulReadCRC32ErrorCnt++;
												ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Read_CRC32Err);
												gubEnterReadFailRetryCondition = CRC32_ERR_MAP;
#if 0 //Hynix Debug
												flaLEDBlink(CRC32_ERR_MAP);
#endif
											}

											if (!BURNER) {
												if ((FQ->uwFJobInfo & BIT_FJOBI_RETRY) && (gubEnterReadFailRetryCondition != 0xFE)) {
													if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
														flaLEDBlink(1);
													}
													else {
														M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
													}
													SYS1L[SYS1L_INT_EN3] &= (~(FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
													flaReadFailRetry(ubgFQLinkIndex);

													SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing FPU COR
													if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
														flaLEDBlink(1);
													}
													else {
														M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
													}
													SYS1L[SYS1L_INT_EN3] |= (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);
													if (FQ->btSpareInvalid == 0) {
														gubINTTable[FQ->ubDepth].All &= 0x0F;
													}
												}
												else if (gubEnterReadFailRetryCondition != 0xFE) {
													VT->gulReadFailNotRetryJobDebugCnt++;
													FQ->btReadUNC = 1;//WP add
													flaRebuildFQPhase(ubgFQLinkIndex);
													gubINTTable[FQ->ubDepth].All &= 0x0F;
												}
											}
											else {

												if ((FQ->uwFJobInfo & BIT_FJOBI_RETRY) && (gubEnterReadFailRetryCondition != 0xFE) && (gubReadHBRetry_Enable)) {
													UartString("\nUNC gen ");
													UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
													UBYTE ubFrameNum = gMTQ->Depth[uwCheckID].Para0x24.BitMap.ubFrameNum;
													UBYTE ubIsFastPage = (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;
													UBYTE ubDepth;
													if (ENABLE_BURNER_FORCEGENFAIL) {
														gFREG[FCTLL_UNC_CFG].L = 0;
														gMTQ->Depth[FQ->ubDepth].Para0x00.BitMap.btForceReadFail = 0; // can remove
													}
													M_GetMTDepth(ubDepth);
													gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
													gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
													//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
													gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
													gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
													gubINTTable[ubDepth].All = 0;
													gFREG[FCTLL_INT_CFG].L &= (~ABORT_DONE_INTR_EN); // follow FLH RegSpec 4.19 MTQ Manual Abort
													gulReadFail_4KMAP = 0;


													FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
#if(UFS)
													if (1)
#else
													if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
													{
														FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
													}
													else {
														FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
													}
													// Wait flash interface ready
													gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
													gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
													if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
														flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x71_FPU_POL_flaTask, gFREG, CHECK_FPU_POL_TIMEOUT);
													}
													else {
														while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
													}

													// MT preload + FPU trig = Auto clear CE, so it need to set CE again
#if(UFS)
													if (1)
#else
													if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
													{
														FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
													}
													else {
														FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
													}

#if TLC
													// Copy these stuff from flaReadFailRetry
													UBYTE ubD1Unit = Default_D1_Unit;
													if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
														ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
													}
													if (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
														ubIsFastPage = 1;
													}
													gulIRAM_L4K_TABLE_OFFSET_HR = (L4K_TABLE_OFFSET + (FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize);
													gubStartFrameNo_HR = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
													gubFrameNum_HR = ubFrameNum;
													gubIsFastPage_HR = ubIsFastPage;
													gubDepth_HR = ubDepth;
													gubChannel_HR = ubChannel;
													gubALUIndex_HR = FQ->ubALUIndex;
													gubCEIndex_HR = FQ->ubCEIndex;
#if B0KB
													gubD3LMU_HR = Micron_CheckPage((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#else
													gubD3LMU_HR = ( (M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) % 3 ) + 1;
#endif
													if (!flaReadFailHBRetry()) {
#else
													gulFQEntry_HR = FQ->ulFEntry;
													if (!(flaReadFailHBRetry((L4K_TABLE_OFFSET + (FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize), (FQ->ulFEntry & gub4kEntrysPerPlaneMask), ubFrameNum, ubIsFastPage, ubDepth, ubChannel, FQ->ubALUIndex))) {
#endif
														FQ->btReadUNC = 1;
													}
#if(UFS)
													if (1)
#else
													if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
													{
														FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
													}
													else {
														FCONL[FCONL_FCE_CLR] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Disable selected CE
													}
													gubINTTable[ubDepth].All = 0;
													M_AddMTDepth(ubDepth);
													gFREG[FCTLL_INT_CFG].L |= ABORT_DONE_INTR_EN;
													//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
													gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);

													if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
														gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
													}
													//if (FQ->btSpareInvalid == 0) {
													gubINTTable[FQ->ubDepth].All &= 0x0F;
													//}
												}
												else if (gubEnterReadFailRetryCondition != 0xFE) {
													FQ->btReadUNC = 1;
													gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
													gFREG[FCTLL_MT_TRIG].B.B1 = 0;
													//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
													gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
													if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
														gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
													}
													gubINTTable[FQ->ubDepth].All &= 0x0F;
												}

											}
										}

										// read retry後為signoffmask fail or 一開始就signoffmask fail
										if ((gubINTTable[FQ->ubDepth].All & 0xF0) && ((gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & SIGNOFFMASK_INF)))) {
											flaReadSignoffmaskFail(ubgFQLinkIndex);
											gubINTTable[FQ->ubDepth].All &= 0x0F;
											//M_AssertError(1);
										}
										gubEnterReadFailRetryCon_Debug = gubEnterReadFailRetryCondition;//For last condition Debugging
										gubEnterReadFailRetryCondition = 0xFE;//Clear After Read Fail Flow
									}

									if (FQ->ubFQPhase >= BYTE_TRIG_DMA_DONE) {
										if (FQ->uwFJobInfo & BIT_FJOBI_WAITRESULT) {
											FQ->ubFQPhase = BYTE_FQ_WAITCHECK;
										}
										else {
											FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
										}
									}

#if ENABLE_TABLE_VERIFY
									if (FQ->ubFQPhase >= BYTE_TRIG_DMA_DONE) {
										if (FQ->btTableVerify && !FQ->btReadUNC) {
											P4KTable16B *pP4KTablePtr = (P4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize));
											if (pP4KTablePtr->Para0x04.BitMap.ubL4K_SPRV != 0xFF) {
												VT->guwTableProgramE3DCnt++;
												flaMarkFail(FQ->ubL4kIndex, 0xFF, 0);
											}
										}
									}
#endif

									if (FQ->FQPara.B.btHandleTableFlag) {
										gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btTableInRAMFlag = 1; //not to change valid at flash layer
									}

									if (FQ->FQPara.B.btUserData && FQ->ubFQPhase >= BYTE_TRIG_DMA_DONE) {
										for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
											UBYTE ub4kBufferIndex = (FQ->ubBufferInedex_R + ub4kIndex) & gulBuffer2Read4KMask;
											if (gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] == 0xFF) {
												gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] = 0;
											}
											else {
												gubNowItsYourTernToLoadDataToThis4kBuffer[ub4kBufferIndex] ++;
											}
										}

										gubRFQCnt --;

									}
									break;
								case BYTE_FJOB_WRITE:
								case BYTE_FJOB_ERASE_BLOCK:
								case BYTE_FJOB_ERASE_UNIT:
									if (gubINTTable[FQ->ubDepth].B.btSTA) {

										if (ENABLE_DEBUG_UART_CC) {
											UartString("\n @STA : 0x");
											Uart_Tx_DataHex(FQ->ubDepth);
											//已經移到前面去處理, 原則上這裡不會再有 STA Error
											//M_AssertError(1);
										}

									}

									if (FQ->uwFJobInfo & BIT_FJOBI_WAITRESULT) {
										FQ->ubFQPhase = BYTE_FQ_WAITCHECK;
									}
									else {
										FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
									}
									if (FQ->ubFJob == BYTE_FJOB_WRITE) {
										if (FQ->btNeedToClearWrite4kBufferFlag && (FQ->uwDataInSDRMap || (FQ->uwFJobInfo & BIT_FJOBI_BUFFERMODE))) {
											for (ub4kIndex = 0; ub4kIndex < FQ->ubL4kNum; ub4kIndex++) {
												UWORD uw4kIndexBit = (BIT0 << ub4kIndex);
												if (FQ->uw4KInFQMap & uw4kIndexBit) {
													if (FQ->uwDataInSDRMap & uw4kIndexBit) {
														SDR_DataEntryTable[(FQ->uwDataInSDRIndex[0] + ub4kIndex)&SDRAM_CACHE_4KNUM_MASK].btDataValid = 0;
													}
												}
											}
											FQ->uwDataInSDRMap = 0;
											FQ->btNeedToClearWrite4kBufferFlag = 0;
											gFQI.ubFQLinkNumber_W--;
										}
										else if (FQ->FQPara.B.btHandleTableFlag) {
											gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btTableInRAMFlag = 0; //not to change valid at flash layer
											gulL2P_InRamInverse[FQ->uwDestTableInRAMIndex].B.btSaving = 0;
											FQ->FQPara.B.btHandleTableFlag = 0;
										}
										else if (FQ->btRWVCTable) {
											FQ->btRWVCTable = 0;
											gubProgramVCTable = 0;
										}
										else if (FQ->FQPara.B.btRWTable_P2L) {
											FQ->FQPara.B.btRWTable_P2L = 0;
											gubProgramTable_P2L = 0;
										}
										else if (FQ->btParity) {
											if (FQ->uwFUnit != VT->guwInitInfoTarget.B.uwTarget) {

												if (FQ->ubRSFrameIndex < TOTAL_PARITY_GROUP_NUM) {
													guwRSFrameDoneCount[FQ->ubRSFrameIndex]++;
													gubflaRSFrameReuseCount[FQ->ubRSFrameIndex] ++;
												}

											}
											FQ->btParity = 0;
										}
										if (FQ->FQPara.B.btUserData) {
#if FixShallowErase
											if (!gubSkipProgramFail) {
												gulProgramUserDataCount --;
											}
#else
											gulProgramUserDataCount --;
#endif
										}
									}
									break;
								default:
									M_AssertError(1);
									break;
							}
							//M_DebugGPIO5_Low();
						}


						// 每個CE需FIFO進行remove FQ, 前面的FQ 若是BYTE_FQ_WAITCHECK, 需要等ftlTask改BYTE_FQ_CHECKDONE
						// 而後面已完成的FQ (BYTE_FQ_CHECKDONE)還不能先被remove
						if ((FQ->ubFQPhase == BYTE_FQ_CHECKDONE) && (ubRemoveFQCount == 0)) {
#if (FORCE_AUTOPOL_TIMEOUT && UART_AUTOPOL_TIMEOUT_HANDLING)
							gubDebugAutoPolDirectRemoveFQ = 0;
							if ((FQ->ubL4kIndex == gubDebugAutoPolFQIndex) && gubDebugAutoPolFPUSeqInUse) {
								UartString("\nRemove Autopol FQ:");
								UartWordHex((UWORD) gubDebugAutoPolFQIndex);
								UartString(" ,");
								UartWordHex((UWORD) gubDebugAutoPolMTDepth);
							}
#endif
							//M_DebugGPIO6_High();
							// clear INTTable of relative MT
							if (gubINTTable[uwCheckID].All & 0xF0) {
							Mark_FreeFailInterrupt_ClearINTTable:
#if (TLC || ENABLE_ULTRAMLC)
								if (gubPreformat && ((FQ->ubFJob == BYTE_FJOB_ERASE_BLOCK) || (FQ->ubFJob == BYTE_FJOB_ERASE_UNIT))) {
#else
								if (gubPreformat && (FQ->ubFJob == BYTE_FJOB_ERASE_BLOCK)) {
#endif

									if (ENABLE_DEBUG_UART_CC) {
										UartString("\n@Preformat E F");

									}
									if (FQ->uwFJobInfo & BIT_FJOBI_ERASE_FAIL_RESET) {
										flaEraseFailHandle(ubgFQLinkIndex);
									}

								}
								else if (FQ->btThisIsErasePage && FQ->btCheckEmpty) {
									//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
									gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
									if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
										gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
									}
								}
								else if ((gFREG[FCTLL_INT_INF].L & UNC_MAP_INF) == UNC_MAP_INF) {
									if (FQ->uwFJobInfo & BIT_FJOBI_RETRY && (!BURNER)) {
										if (ENABLE_WP_DEBUG) {
											flaLEDBlink(1);
										}
										else {
											M_AssertError(1);
										}
									}
									else {
										VT->gsReadFailDebugCount.gulPreformatUNCNoRetry_LogCount++;
										//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
										gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
										if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
											gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
										}
									}
								}
								else if ((gFREG[FCTLL_INT_INF].L & OVER_ECC_THRESHOLD_INF) == OVER_ECC_THRESHOLD_INF) {
									VT->gsReadFailDebugCount.gulECCOverTH_LogCount++;

									if (!ENABLE_DEBUG_FIX_MT_EOT_BUG) {
										if (ENABLE_WP_DEBUG) {
											flaLEDBlink(1);
										}
										else {
											M_AssertError(1);
										}
									}
									if (ENABLE_DEBUG_OVER_ECC_STOP) {
										//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
										gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
										if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
											gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
										}
									}
									else {
										if (ENABLE_WP_DEBUG && (gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
											flaLEDBlink(1);
										}
										else {
											M_AssertError(gFREG[FCTLL_INT_INF].L & INTR_BUSY);
										}
										//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
										gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
									}
								}
								else if ((gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) == SIGNOFFMASK_INF) {
									if (ENABLE_WP_DEBUG) {
										flaLEDBlink(1);
									}
									else {
										M_AssertError(1);
									}
									//flaReadSignoffmaskFail(ubgFQLinkIndex);
								}
								else if (ENABLE_DEBUG_RSDECODE) {}
								else if (gubExpectBadkBlock && ((gFREG[FCTLL_INT_INF].L & CRC32_ERROR) == CRC32_ERROR)) { //maybe CRC fail when scanning bad block
									//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
									gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
									if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
										gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
									}
								}
								else {
									if (((gFREG[FCTLL_INT_INF].L & CRC16_ERROR) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & CRC16_ERROR))) {
										VT->gulCRC16ErrorDebugCnt++;
									}
									if (((gFREG[FCTLL_INT_INF].L & DECOMPRESSION_FAIL_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & DECOMPRESSION_FAIL_INF))) {
										VT->gulDeCompressionErrorDebugCnt++;
									}
									if (ENABLE_WP_DEBUG) {
										flaLEDBlink(1);
									}
									else {
										M_AssertError(1);
									}
									if (ENABLE_FIP_NO_HANG && (!ENABLE_DEBUG_RW) && (gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
										// no hang
										VT->gulFIPNoHangDebugCnt++;
										flaRebuildFQPhase(ubgFQLinkIndex);
									}
								}
							}

							gubINTTable[uwCheckID].All = 0;
							M_AddMTDepth(FQ->ubDepth);

							if (FQ->ubRelativeDepth != 0xFF) {
								if (FQ->ubRelativeDepth != FQ->ubDepth) {
									gubINTTable[FQ->ubRelativeDepth].All = 0;
									M_AddMTDepth(FQ->ubRelativeDepth);
								}
								FQ->ubRelativeDepth = 0xFF;
							}

							if (FQ->ubRemainderDepth != 0xFF) {
								M_AssertError(FQ->ubRemainderDepth == FQ->ubDepth);
								M_AssertError(FQ->ubRemainderDepth == FQ->ubRelativeDepth);
								gubINTTable[FQ->ubRemainderDepth].All = 0;
								M_AddMTDepth(FQ->ubRemainderDepth);
								FQ->ubRemainderDepth = 0xFF;
							}

							if (FQ->ubRedundantDepth != 0xFF) {
								M_AssertError(FQ->ubRedundantDepth == FQ->ubDepth);
								M_AssertError(FQ->ubRedundantDepth == FQ->ubRelativeDepth);
								M_AssertError(FQ->ubRedundantDepth == FQ->ubRemainderDepth);
								gubINTTable[FQ->ubRedundantDepth].All = 0;
								M_AddMTDepth(FQ->ubRedundantDepth);
								FQ->ubRedundantDepth = 0xFF;
							}

#if MicronFlashOnly
							if (FQ->ubInExitSLCDepth != 0xFF) {
								gubINTTable[FQ->ubInExitSLCDepth].All = 0;
								M_AddMTDepth(FQ->ubInExitSLCDepth);
								FQ->ubInExitSLCDepth = 0xFF;
							}
#endif

#if ENABLE_RESET_RETRY_LEVEL
							if (FQ->ubBicsRetryLevelDepth != 0xFF && ((guoFlashUseType.B.btSandisk && TLC_BICS2) || ((!TLC) && (!guoFlashUseType.B.btSandisk)))) {
								gubINTTable[FQ->ubBicsRetryLevelDepth].All = 0;
								M_AddMTDepth(FQ->ubBicsRetryLevelDepth);
								FQ->ubBicsRetryLevelDepth = 0xFF;
							}
#endif
						Mark_Remove_FQ:
							FQ->ubFQPhase = 0;
							FQ->FQPara.All = 0;
							FQ->btNoWait = 0;
							FQ->btD3 = 0;
							FQ->ubWritePhaseRedoCmd = MARK_FPU_C80_C10C15;
							FQ->btNeedSetCEWhenTriggerDMA = 0;
							M_AssertError(FQ->btNeedStopDMA);
							FQ->btUncCheckInRSTwoCh = 0;
							FQ->btRSPatchUpWritePhaseCmd = 0;
							FQ->btRedoFQ = 0;
							FQ->btSpareInvalid = 0;
							FQ->btDirectHandling = 0;
							FQ->btReadUNC = 0;
							FQ->btReadMove = 0;
							FQ->btThisIsErasePage = 0;
							FQ->btGenFail = (ENABLE_DEBUG_HW_FORCEREADFAIL || ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL);
							FQ->btCopyFromWB = 0;
							FQ->btRecordFailLogByMyself = 0;
							FQ->ulFIPError.All = 0;
							FQ->btPEFail = 0;
							FQ->btTLCProgramCodeBlock = 0;
#if ENABLE_TABLE_VERIFY
							FQ->btTableVerify = 0;
#endif


#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
							FQ->FQPara.B.btConvBypass = 0;
#endif
							FQ->ubCheckStatusPhase = STATUS_CHECK_ONE_FQ;
#if TLC
							FQ->ubD3WriteMode = 0;
#endif
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
#if ENABLE_READ_AUTO_POL_TIMEOUT_HANDLING
							FQ->btPolFailTimeoutCnt = 0;
#endif
							FQ->btUsingAutoPol = 0;
#if FORCE_AUTOPOL_TIMEOUT
							if ((FQ->ubL4kIndex == gubDebugAutoPolFQIndex) && gubDebugAutoPolFPUSeqInUse && (gubDebugAutoPolDirectRemoveFQ == 0)) {
								gubDebugAutoPolFQIndex = 0xFF;
								gubDebugAutoPolFPUSeqInUse = 0;
								gMTQ->Depth[gubDebugAutoPolMTDepth].Para0x1C.BitMap.ubPolSeqSel = WAIT_TRUE_READY;
#if UART_AUTOPOL_TIMEOUT_HANDLING
								UartString("\n");
								UartWordHex((UWORD)gFREG[FCTLL_FPU_TRIG].B.B0);
								UartString("	");
								UartWordHex((UWORD)gFREG[FCTLL_INT_RDY].B.B0);
								UartString("	");
								UartLongHex(gFREG[FCTLL_FPU_ENTRY].L);
#endif
							}
#endif
#endif
							if (gubFQOutOfOrder) {
								if (FQ->btOutOfOrder) {
									gubFQOutOfOrderCnt--;
									FQ->btOutOfOrder = 0;
									M_AssertError(gubFQOutOfOrderCnt > gFQI.ubFQLinkNumber);
								}
								if ((gubNeedWaitCount == 0) && (gubFQOutOfOrderCnt == 0)) {
									gubFQOutOfOrder = 0;
								}
							}

							//remove FQLink
							if (ubgFQLinkIndex == gFQI.ubFQLinkFirst) {
								gFQI.ubFQLinkFirst = FQ->ubNext;
							}
							else {
								UBYTE ubFirstFQ = gFQI.ubFQLinkFirst;
								gFQI.gFQLink[FQ->ubPrevious].ubNext = FQ->ubNext;
								gFQI.gFQLink[FQ->ubNext].ubPrevious = FQ->ubPrevious;

								FQ->ubNext = ubFirstFQ;
								FQ->ubPrevious = gFQI.gFQLink[ubFirstFQ].ubPrevious;
								gFQI.gFQLink[gFQI.gFQLink[ubFirstFQ].ubPrevious].ubNext = ubgFQLinkIndex;
								gFQI.gFQLink[ubFirstFQ].ubPrevious = ubgFQLinkIndex;

								if (gFQI.ubFQLinkNumber == gFQ_NUM) {
									gFQI.ubFQLinkFree = ubgFQLinkIndex;
								}
							}
							gFQI.ubFQLinkNumber--;
							gFQI.ubFQFirst[ubCEIndex] = (gFQI.ubFQFirst[ubCEIndex] + 1)&FQ_NUM_MASK;
							gFQI.ubFQDoingNumber[ubCEIndex]--;
							if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
								gubMTDepthPushNumber[ubCEIndex] -= FQ->ubMTDepthCnt;
								FQ->ubMTDepthCnt = 0;
							}

							if (ENABLE_DEBUG_RW && (ENABLE_DEBUG_MTREMAINCNT_REG == 0) && (ubCEIndex != gubMTQTotalPhyCE)) {
								UBYTE ubMTDepthCntSum;
								ubMTDepthCntSum = 0;
								ubCFQIndex = gFQI.ubFQFirst[ubCEIndex];
								ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
								FQ = &gFQI.gFQLink[ubgFQLinkIndex];

								for (ubFQServeCount = 0; ubFQServeCount < gFQI.ubFQDoingNumber[ubCEIndex]; ubFQServeCount++) {
									ubMTDepthCntSum += FQ->ubMTDepthCnt;

									ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
									ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
									FQ = &gFQI.gFQLink[ubgFQLinkIndex];
								}
								M_AssertError(gubMTDepthPushNumber[ubCEIndex] != ubMTDepthCntSum);
							}

							//M_DebugGPIO6_Low();

							if (gFQI.ubFQDoingNumber[ubCEIndex]) {
								ubFQIndex = gFQI.ubFQFirst[ubCEIndex];
							}
							else {
								break;
							}
						}
						else if ((FQ->ubFQPhase == BYTE_FQ_WAITCHECK) || (FQ->ubFQPhase == BYTE_FQ_CHECKDONE)) {
							ubRemoveFQCount++;
							if (ubRemoveFQCount < gFQI.ubFQDoingNumber[ubCEIndex]) {
								ubFQIndex = (gFQI.ubFQFirst[ubCEIndex] + ubRemoveFQCount)&FQ_NUM_MASK;
							}
							else {
								break;
							}
						}
					}
					else if ((FQ->ubRemainderDepth != 0xFF) && (gubINTTable[FQ->ubRemainderDepth].B.btAll_Done && gubINTTable[FQ->ubRemainderDepth].B.btSTA)) {

						// cache program中的check write status為FQ->ubDepth, 若有轉成FQ->ubRemainderDepth, 代表其後面還有一個等完true ready後的FQ->ubDepth (pol fail)
						// 所以這邊不用處理fail plane check, 可以用最後的FQ->ubDepth一併check current program and previous program result

						//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
						gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
						if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
							gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
						}
						gubINTTable[FQ->ubRemainderDepth].All &= 0x0F;
						VT->gulPreviousFQCacheProgramFailDebugCnt++;
					}
					else if (ENABLE_FIP_NO_HANG && (!ENABLE_DEBUG_RW) && ((FQ->ubRelativeDepth != 0xFF) && (gubINTTable[FQ->ubRelativeDepth].All & 0xF8))) {
						// program error
					MARK_PROGRAM_ERROR:
						if (((gFREG[FCTLL_INT_INF].L & COMPRESSION_FAIL_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & COMPRESSION_FAIL_INF))) {
							VT->gulProgramCompressionFailCnt++;
							ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Program_CompressionErr);

						}
						else if (((gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & FW_SETTING_ERR_INF))) {
							VT->gulProgramFWSettingErrorCnt++;
							ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Program_FWSettingErr);
						}
						else if (((gFREG[FCTLL_INT_INF].L & E3D_ERR_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & E3D_ERR_INF))) {
							VT->gulProgramE3DErrorDebugCnt++;
						}

#if ENABLE_DO_AUTOPOL_TIMEOUT_HANDLING
						else if ((gMTQ->Depth[FQ->ubRelativeDepth].Para0x1C.BitMap.uwFPUPtr == FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15)
						         || (gMTQ->Depth[FQ->ubRelativeDepth].Para0x1C.BitMap.uwFPUPtr == FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15)
						         || (gMTQ->Depth[FQ->ubRelativeDepth].Para0x1C.BitMap.uwFPUPtr == FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15)
						         || (gMTQ->Depth[FQ->ubRelativeDepth].Para0x1C.BitMap.uwFPUPtr == FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW)
						         || (gMTQ->Depth[FQ->ubRelativeDepth].Para0x1C.BitMap.uwFPUPtr == FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW)
						         || (gMTQ->Depth[FQ->ubRelativeDepth].Para0x1C.BitMap.uwFPUPtr == FPU_PTR_C11_C70_POL_MK40_C81_A5_DW)) {
							// Timeout
							gubTimeoutEnterRebuildFQ = 1;
							ftlRecordFailLog(FLHErrLogType, 0x5, FQ->ubL4kIndex, BYTE_TIMEOUT_AUTOPOL, DONTCARE, BIT_NormalErr);
							VT->gulMTFPUPolTimeoutCnt++;
						}
#endif
						else {
							VT->gulDmaWriteLack8BytesDebugCnt++;
							ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_Program_Lack8ByteTimeoutErr);
						}

						FQ->btPEFail = 1;

						// doesn't mark bad but need to do copy unit
						flaMarkFail(ubgFQLinkIndex, AUTOPOL_TIMEOUT_FAIL, 0);

						// let it be All_Done & DMA_Done
						gubINTTable[FQ->ubRelativeDepth].All = 0x05;

						if (gFREG[FCTLL_INT_INF].L & INTR_BUSY) {
							// no hang
							if (((gFREG[FCTLL_INT_INF].L & E3D_ERR_INF) && (!FQ->btUncCheckInRSTwoCh)) || (FQ->btUncCheckInRSTwoCh && (FQ->ulFIPError.All & E3D_ERR_INF))) {
								/*if (ENABLE_DEBUG_PROGRAM_E3D_FAIL_FIP_CHECK) {
									UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
									if (ubChannel == 0) {
										FCONL[FCONL_FW_RESET] = 0x7FFEFEFE;
												FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;
									}
									else {
										FCONL[FCONL_FW_RESET] = 0x7FFDFDFD;
												FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;
									}
								}*/
							}
							VT->gulFIPNoHangDebugCnt++;
							flaRebuildFQPhase(ubgFQLinkIndex);
						}
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
						gubTimeoutEnterRebuildFQ = 0;
#endif
					}
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
					// check that FQ had used auto pol and this FQ wait toooooo long
					else if ((FQ->btUsingAutoPol) &&
					         (((gulOperationTime >= FQ->ulAutoPolStartTime) && ((gulOperationTime - FQ->ulAutoPolStartTime) > AUTOPOL_TIMEOUT_MILLISEC)) ||
					          ((gulOperationTime < FQ->ulAutoPolStartTime) && ((0xFFFFFFFF - FQ->ulAutoPolStartTime + gulOperationTime) > AUTOPOL_TIMEOUT_MILLISEC)))) {
						U32 ulStartTime = gulOperationTime;
						xthal_dcache_line_invalidate(&gulOperationTime);
						while (1) {
							// Check autopol timeout FPU sequence
#if FORCE_AUTOPOL_TIMEOUT
							if (gFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK00)
#else
							if ((gFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK20) && (gFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK40))
#endif
							{
								goto Mark_NotAutoPolTimeout;
							}

							// if it is not autopol CE / not first FQ of current CE => not autopol timecout FQ, and should not be INTR busy
							if ((!flaMTCheckIsAutoPollTimeout(ubgFQLinkIndex)) || (gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
								goto Mark_NotAutoPolTimeout;
							}

							// FPU is busy and still auto polling
							if (((gFREG[FCTLL_FPU_TRIG].L & (ANY_BUSY | FPU_TRIG)) != 0) && ((gFREG[FCTLL_INT_RDY].B.B0 & (BIT0 << (FQ->ubCEIndex >> 1))) == 0)) {
								if (((gulOperationTime - ulStartTime) > AUTOPOL_TIMEOUT_MILLISEC) || ((gulOperationTime < ulStartTime) && ((0xFFFFFFFF - ulStartTime + gulOperationTime) > AUTOPOL_TIMEOUT_MILLISEC))) {
									flaMTAutoPollTimeoutHandling(ubgFQLinkIndex);
									break;
								}
							}
							else {
								goto Mark_NotAutoPolTimeout;
							}
						}
						if (0) {
						Mark_NotAutoPolTimeout:
							gulNotPolTimeoutCnt++;
							break;
						}
					}
#endif
					else if ((BURNER == 0) && gubINTTable[FQ->ubDepth].B.btAbort_Done) {

						if (gFREG[FCTLL_INT_INF].L & INTR_BUSY) {

							// 檢查error是同CH的其他CE
							if ((gFREG[FCTLL_MAP_CFG].L & (CURRENT_FIFO_HAS_NEW | NEXT_FIFO_HAS_NEW))) {
								gubTest = (gFREG[FCTLL_MAP_CFG].L & NEXT_FIFO_HAS_NEW) ? 2 : 1; // 不要看第二筆, 避免FIFO PTR亂掉

								// Get INT Depth for error FQ
								gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
								gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(INT_VCT_MAP);

								UBYTE ubCheckCEIndex = FQ->ubCEIndex;
								M_VirtualToPhysicalCE(ubCheckCEIndex);
#if(UFS)
								if (1)
#else
								if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
								{
									ubCheckCEIndex = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[(ubCheckCEIndex & gubPlanesPerBurstMask)][(ubCheckCEIndex / gubPlanesPerBurst)]);
								}

								if (ubCheckCEIndex != gMTQ->Depth[gFREG[FCTLL_MAP_INF].B.B0].Para0x1C.BitMap.ubCEValue) {
									// 合理現象
									gubAbortDone_Case = 1;
								}
								else {
									ftlRecordFailLog(FLHErrLogType, VT_ASSERT_ABNORMAL_ABORTDONE_0x92, FQ->ubL4kIndex, BYTE_INTERR_ABNORMAL_ABORTDONE, DONTCARE, BIT_NormalErr);
									M_AssertResetError(gubAbortDone_Case, VT_ASSERT_ABNORMAL_ABORTDONE_0x92);
								}

							}
							else {
								// 合理現象
								// Check一筆FQ當下可能其他人跳error, 就會先把FIFO error map記下來並清掉FIFO, 但還沒處理那筆error的handle就先流進這裡
								gubAbortDone_Case = 2;
							}

						}
						else {
							ftlRecordFailLog(FLHErrLogType, VT_ASSERT_ABNORMAL_ABORTDONE_0x93, FQ->ubL4kIndex, BYTE_INTERR_ABNORMAL_ABORTDONE, DONTCARE, BIT_NormalErr);
							M_AssertResetError(gubAbortDone_Case, VT_ASSERT_ABNORMAL_ABORTDONE_0x93);
						}
						break; // 檢查下一個CE first FQ狀態
					}
					else {
						break;
					}
				}
			}
			ubCEIndex = (ubCEIndex + 1) % (gubMTQTotalPhyCE + 1);
		}

		if (HL[HL_RD_UNC_ADDR] != 0) { // ReadUNC multi-error handler

			UBYTE ubFindTarget = 0;
			UBYTE ubUncArrayPtr = gubUncPtr;
			while (ubUncArrayPtr > 0) {
				if (gulSATAUncAddr[ubUncArrayPtr - 1] == (BUFFER2_BASE + HL[HL_BUF_CUR_OFST] * 512)) {
					HL[HL_RD_UNC_ADDR] = gulSATAUncAddr[ubUncArrayPtr - 1];
					ubFindTarget = 1;
					break;
				}
				ubUncArrayPtr--;
			}

			gulReadUNCWaitCount++;
			if (ubFindTarget == 0) {
				if ((gulReadUNCWaitCount % 1000) == 0) {
					if (gubUncPtr > ((gulReadUNCWaitCount / 1000) - 1)) {
						HL[HL_RD_UNC_ADDR] = gulSATAUncAddr[((gulReadUNCWaitCount / 1000) - 1)];
					}
				}
			}

			if (gulReadUNCWaitCount == (FLA_READUNC_MAXLOG_CNT * 1000)) {
				gulReadUNCWaitCount = 0;
			}
		}


#if (ENABLE_RESET_RETRY_LEVEL && (!TLC))
		if (!guoFlashUseType.B.btSandisk) {

			xthal_dcache_line_invalidate(&gulOperationTime);

			if (((gulOperationTime - gul26CMDStartTime) > 1000) || (gul26CMDStartTime > gulOperationTime)) {
				gul26CMDStartTime = gulOperationTime;
				gub26CMDflag = 0;
				gulMLCHBPassCnt = 0;
			}
			if (gulMLCHBPassCnt > MLCHBPassCntThreshold) {
				gub26CMDflag = 1;
			}

		}
#endif



		M_SwitchTask();
	}
#endif
}

void flaSetFlashSourceAddrCE_CH(UBYTE ubgFQLinkIndex) {
	// flaSetFlashSourceAddrCE_CH 對IRAM有read-modify-write的行為, 但因為現在都用iFSA, 所以這邊先不修
#if (!ENABLE_iFSA )
	FlashQueue_t *FQ;
	FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	U32 ulFEntry = FQ->ulFEntry;
	UBYTE ubBurstBank, ubPhyPlane;
	UWORD uwFBlock;
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];
	UBYTE ubD1Unit = Default_D1_Unit;

#if (TLC)
	if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
		ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
		if ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
			ulFEntry = FQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
		}
	}
#endif

	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
	}

	if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
		uwFBlock = FQ->uwFUnit;
		ubPhyPlane = ubBurstBank;
	}
	else {
		if ((TLC == 0) || (FQ->btD3 || (ubD1Unit == Default_D1_Unit))) {
			uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
		}
		else {
			uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
		}
		if (gubRUTMixPlaneEnable == 0) {
			ubPhyPlane = ubBurstBank;
		}
		else {
			ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
			uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
		}
	}


	if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
		if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
			uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
		}
	}

	M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

	if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
		if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
			FQ->ubALUIndex = ALU_A2;
			goto Mark_NormalSectorAddress;
		}
		else {
			FQ->ubALUIndex = ALU_NORMAL;
			pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | (flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			//pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubBurstBank ? BIT15 : 0)  | (flaFastPage((U32)(((uwFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) + (uwFEntry & gub4kEntrysPerPlaneMask)))); //20150323之前長這樣
		}
	}
	else {
		FQ->ubALUIndex = ALU_NORMAL;
	Mark_NormalSectorAddress:
#if TLC
		if (TLC) {
			if (FQ->ubALUIndex == ALU_NORMAL) { // D3 Read
				pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask)); // 3:LMU
			}
			else {
				pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask));
			}
		}
		else {
#else
		{
#endif
			pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask));
		}
	}
	pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwFBlock & 0xFFFF;
	pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;

	if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
		FQ->ubFSAIndex = gubFSAIndex[ubChannel];
	}

	gubFSAIndex[ubChannel] = (gubFSAIndex[ubChannel] + 1) & 127;

	if (FQ->ubFJob == BYTE_FJOB_ERASE_UNIT) {
		for (ubBurstBank = 1; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
			if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
				if ((TLC == 0) || FQ->btD3) {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
				}
				else {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (FQ->uwFUnit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
				}
				if (gubRUTMixPlaneEnable == 0) {
					ubPhyPlane = ubBurstBank;
				}
				else {
					ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
					uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
				}
			}
			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE)) {
				if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
					uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
				}
			}
			M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);
			pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15));
			pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwFBlock & 0xFFFF;
			pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;
			gubFSAIndex[ubChannel] = (gubFSAIndex[ubChannel] + 1) & 127;
		}
	}

	M_AssertCriticalError((((uwFBlock >= gulTotalPhyUnitNum) && ((gubDieNumber == 1) && (gubExpandCE == 0))) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog))), VT_ASSERT_SETFLASHSOURCEADDRCE_CH_0x42); // SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum
#endif
}

UBYTE flaCheckFQContinueNum(UBYTE ubCEIndex, UBYTE ubCFQIndex,  UBYTE ubUnServedCount) {
	// 此筆FQ跟前一筆是否接得起來, CEXferPhase=0且為planeB, C, D時算fail
	// 遇到不同type的FQ跳出
	// 考慮flash busy status, 仍busy就可以再等, 已經ready就要評估還要再等幾次
	// 保留一次等1, 2 , 4個FQ的彈性

	FlashQueue_t *FQ, *PreviousFQ;
	UBYTE ubPhase;
	UBYTE ubSequentialFQNum;
	UBYTE ubgFQLinkIndex, ubgPreviousFQLinkIndex;
	UBYTE ubBurstBank, ubPreviousBurstBank, ubBurstBankOrder, ubLastBurstBank;
	UBYTE ubPreviousFQIndex;
	UBYTE ubCompareFromPreviousDoneFQ;
	U32 ul4kEntrysDiff;
	UBYTE ubNeedFQNoWait = 0;
#if TLC
	UWORD uwD1UnitIndex;
	UWORD uwPreviousD1UnitIndex;
#endif

	UBYTE ubPreviousBurstBankTemp;
	UWORD uwPrevoursFBlockTemp;
	UBYTE ubBurstBankTemp;
	UWORD uwFBlockTemp;
	U32 ulFEntry;
	U32 ulPreviousFEntry;
	UBYTE ubD1Unit = Default_D1_Unit;
	UBYTE ubPreviousD1Unit = Default_D1_Unit;

	if ((gubToCheckNextCmdSequence[ubCEIndex] == 0) ||
	        ((guoFlashUseType.B.btFourPlane == 0) && (guoFlashUseType.B.btTwoPlane == 0)) ) {
		ubCompareFromPreviousDoneFQ = 1;
	}
	else {
		ubCompareFromPreviousDoneFQ = 0;
	}

	ubSequentialFQNum = 0;
	ubBurstBankOrder = 0;
	ubLastBurstBank = gubBurstsPerBank - 1;

	ubPhase = gFQI.ubXferPhase[ubCEIndex];
	ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
	FQ = &gFQI.gFQLink[ubgFQLinkIndex];

	if (ubPhase && ubCompareFromPreviousDoneFQ)  {
		ubPreviousFQIndex = ((ubCFQIndex + FQ_NUM - 1)&FQ_NUM_MASK);
		ubgPreviousFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubPreviousFQIndex];
		PreviousFQ = &gFQI.gFQLink[ubgPreviousFQLinkIndex];

		if (FQ->ubFJob == BYTE_FJOB_READ) {
			if (gubRUTMixPlaneEnable) {
				ulPreviousFEntry = PreviousFQ->ulFEntry;
#if TLC
				ubPreviousD1Unit = Default_D1_Unit;
				if ((PreviousFQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
					ubPreviousD1Unit = ((PreviousFQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? PreviousFQ->ubFSAIndex : gubD1LinkTable[PreviousFQ->uwFUnit].B.ubD1Link);
					if ((PreviousFQ->btD3 == 0) && (ubPreviousD1Unit != Default_D1_Unit)) {
						ulPreviousFEntry = PreviousFQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
					}
				}
#endif
				if (ENABLE_PLANE_CH_CE) {
					ubPreviousBurstBankTemp = (ulPreviousFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
				}
				else {
					ubPreviousBurstBankTemp = (ulPreviousFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
				}

				if (PreviousFQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
					ubPreviousBurstBank = ubPreviousBurstBankTemp;
				}
				else {
					if ((TLC == 0) || (PreviousFQ->btD3 || (ubPreviousD1Unit == Default_D1_Unit)) ) {
						uwPrevoursFBlockTemp = M_RUT2DTo1D(PreviousFQ->ubCEIndex , (PreviousFQ->uwFUnit << gubBurstsPerBankLog) + ubPreviousBurstBankTemp);
					}
					else {
						uwPrevoursFBlockTemp = M_RUT2DTo1D(PreviousFQ->ubCEIndex , ((guwFWTotalUnitNum + (ubPreviousD1Unit * D1_UNIT_NUM_PER_LINK) + (PreviousFQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubPreviousBurstBankTemp);
					}
					ubPreviousBurstBank = (uwPrevoursFBlockTemp & gubBurstsPerBankMask);
				}
			}
			else {
				if (ENABLE_PLANE_CH_CE) {
					ubPreviousBurstBank = (PreviousFQ->ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
				}
				else {
					ubPreviousBurstBank = (PreviousFQ->ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
				}
			}

			if (ubPreviousBurstBank == ubLastBurstBank) {	// 之前是full plane operation
				ubBurstBankOrder = ubLastBurstBank;
			}
		}
	}
	else { // initial phase or gubToCheckNextCmdSequence[ubCEIndex] = 1
		if ((FQ->ubFJob != BYTE_FJOB_WRITE) && (FQ->ubFJob != BYTE_FJOB_READ)) {
			return ubSequentialFQNum;
		}

		if (FQ->btNoWait) {
			ubNeedFQNoWait = 1;
		}

		if (ubUnServedCount > 1) {
			PreviousFQ = FQ;

			ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
			ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
			FQ = &gFQI.gFQLink[ubgFQLinkIndex];
		}
		ubSequentialFQNum = 1;
		ubUnServedCount--;
	}


	while (ubUnServedCount) {
#if TLC
		if (((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) && ((FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit))) {
			uwD1UnitIndex = (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit);
			uwPreviousD1UnitIndex = (PreviousFQ->ulFEntry  / gulFastPage4kEntrysPerUnit);
		}
#endif

		if (gubRUTMixPlaneEnable) {
			ulFEntry = FQ->ulFEntry;
			ulPreviousFEntry = PreviousFQ->ulFEntry;
#if TLC
			ubD1Unit = Default_D1_Unit;
			if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
				ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
				if ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
					ulFEntry = FQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
				}
			}

			ubPreviousD1Unit = Default_D1_Unit;
			if ((PreviousFQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
				ubPreviousD1Unit = ((PreviousFQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? PreviousFQ->ubFSAIndex : gubD1LinkTable[PreviousFQ->uwFUnit].B.ubD1Link);
				if ((PreviousFQ->btD3 == 0) && (ubPreviousD1Unit != Default_D1_Unit)) {
					ulPreviousFEntry = PreviousFQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
				}
			}
#endif
			if (ENABLE_PLANE_CH_CE) {
				ubBurstBankTemp = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
				ubPreviousBurstBankTemp = (ulPreviousFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
			}
			else {
				ubBurstBankTemp = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
				ubPreviousBurstBankTemp = (ulPreviousFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
			}

			if (PreviousFQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
				ubBurstBank = ubBurstBankTemp;
				ubPreviousBurstBank = ubPreviousBurstBankTemp;
			}
			else {
				if ((TLC == 0) || (FQ->btD3 || (ubD1Unit == Default_D1_Unit))) {
					uwFBlockTemp =  M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBankTemp);
				}
				else {
					uwFBlockTemp = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBankTemp);
				}

				if ((TLC == 0) || (PreviousFQ->btD3 || (ubPreviousD1Unit == Default_D1_Unit))) {
					uwPrevoursFBlockTemp = M_RUT2DTo1D(PreviousFQ->ubCEIndex , (PreviousFQ->uwFUnit << gubBurstsPerBankLog) + ubPreviousBurstBankTemp);
				}
				else {
					uwPrevoursFBlockTemp = M_RUT2DTo1D(PreviousFQ->ubCEIndex , ((guwFWTotalUnitNum + (ubPreviousD1Unit * D1_UNIT_NUM_PER_LINK) + (PreviousFQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubPreviousBurstBankTemp);
				}

				ubBurstBank = (uwFBlockTemp & gubBurstsPerBankMask);
				ubPreviousBurstBank = (uwPrevoursFBlockTemp & gubBurstsPerBankMask);
			}
		}
		else {
			if (ENABLE_PLANE_CH_CE) {
				ubBurstBank = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
				ubPreviousBurstBank = (PreviousFQ->ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
			}
			else {
				ubBurstBank = (FQ->ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
				ubPreviousBurstBank = (PreviousFQ->ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
			}
		}

#if TLC
		if ((FQ->ubFJob != PreviousFQ->ubFJob) || ((FQ->ubFJob == BYTE_FJOB_WRITE) && (FQ->uwFUnit != PreviousFQ->uwFUnit)) || \
		        ((TLC && ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0)) && (FQ->ubFJob == BYTE_FJOB_WRITE)
		         && ((FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit))
		         && (uwD1UnitIndex != uwPreviousD1UnitIndex)) ) {
#else
		if ((FQ->ubFJob != PreviousFQ->ubFJob) || ((FQ->ubFJob == BYTE_FJOB_WRITE) && (FQ->uwFUnit != PreviousFQ->uwFUnit))) {
#endif
			gubFQNeedExecuteRightNow[ubCEIndex] = 1;
			if (gubFQNotSequential[ubCEIndex] == 1) {
				gubFQNotSequential[ubCEIndex] = 0;
			}
			break;
		}
		if (FQ->btNoWait) {
			ubNeedFQNoWait = 1;
		}

		if (PreviousFQ->ubFJob == BYTE_FJOB_READ) {
			if (1) {	//ENABLE_ONEPLANE_CACHEREAD
				//if (guoFlashUseType.B.btFourPlane || guoFlashUseType.B.btTwoPlane) {
#if TLC
				if ((ubPreviousBurstBank != ubBurstBankOrder) || (FQ->ulFEntry < PreviousFQ->ulFEntry) || (FQ->uwFUnit != PreviousFQ->uwFUnit) || \
				        ((TLC && ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0)) && ((FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit)) && (uwD1UnitIndex != uwPreviousD1UnitIndex))) {
#else
				if ((ubPreviousBurstBank != ubBurstBankOrder) || (FQ->ulFEntry < PreviousFQ->ulFEntry) || (FQ->uwFUnit != PreviousFQ->uwFUnit)) {
#endif
					if (guoFlashUseType.B.btRandCacheRead ) {
#if MicronFlashOnly
						if (guoFlashUseType.B.btA2Cmd) {
							//Micron SLC mode operation limitation, device cannot mix SLC/MLC/TLC modes during any single operation.
							UBYTE CurrentSLC = ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || ((FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit)));
							UBYTE PreviousSLC = ((PreviousFQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || ((PreviousFQ->btD3 == 0) && ((PreviousFQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? PreviousFQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[PreviousFQ->uwFUnit].B.ubD1Link != Default_D1_Unit)));
							if (CurrentSLC != PreviousSLC) {
								gubFQNeedExecuteRightNow[ubCEIndex] = 1;
								break;
							}
						}
						if (guoFlashDefaultType.B.btMultiDie && FQ->uwFUnit != PreviousFQ->uwFUnit) {
							gubFQNeedExecuteRightNow[ubCEIndex] = 1;
						}
						else {
							gubFQNotSequential[ubCEIndex] = 1;
						}
#else
						gubFQNotSequential[ubCEIndex] = 1;
#endif
					}
					else {
						gubFQNeedExecuteRightNow[ubCEIndex] = 1;
					}
					break;
				}
			}
		}

		if (ENABLE_CY_DEBUG && (FQ->ubFJob == BYTE_FJOB_WRITE)) { 	//read cmd不成立
			M_AssertError(FQ->ulFEntry % gub4kEntrysPerPlane);
			M_AssertError(PreviousFQ->ulFEntry >= FQ->ulFEntry);
		}

		//2  絛件可再修好一點, 目前為pA->pB, pB->pC, pC->pD, pD(pageN->pA (pageN+1)
		UBYTE ub4kEntrysDiffForContinuousPlane;	// for pA接pB, pB接pC, pC接pD (同page不同plane)
		UWORD uw4kEntrysDiffForContinuousPage; // only for pD (page N) 接pA (page N+1)

		if (gubRUTMixPlaneEnable) {
			if (ENABLE_PLANE_CH_CE) {
				ub4kEntrysDiffForContinuousPlane = gub4kEntrysPerPlane;
				uw4kEntrysDiffForContinuousPage = guw4kEntrysPerPage - gub4kEntrysPerPlane * ubPreviousBurstBankTemp;
			}
			else {
				ub4kEntrysDiffForContinuousPlane = gub4kEntrysPerBurst;
				uw4kEntrysDiffForContinuousPage = guw4kEntrysPerPage - gub4kEntrysPerBurst * ubPreviousBurstBankTemp;
			}
		}
		else {
			if (ENABLE_PLANE_CH_CE) {
				ub4kEntrysDiffForContinuousPlane = gub4kEntrysPerPlane;
				uw4kEntrysDiffForContinuousPage = guw4kEntrysPerPage - gub4kEntrysPerPlane * ubPreviousBurstBank;
			}
			else {
				ub4kEntrysDiffForContinuousPlane = gub4kEntrysPerBurst;
				uw4kEntrysDiffForContinuousPage = guw4kEntrysPerPage - gub4kEntrysPerBurst * ubPreviousBurstBank;
			}
		}

		ul4kEntrysDiff = FQ->ulFEntry - PreviousFQ->ulFEntry;

		if ((ubBurstBank == ((ubPreviousBurstBank + 1) & gubBurstsPerBankMask))
		        && ((ul4kEntrysDiff == ub4kEntrysDiffForContinuousPlane) || ((ubPreviousBurstBank == ubLastBurstBank) && (ul4kEntrysDiff == uw4kEntrysDiffForContinuousPage))) ) {
#if MicronFlashOnly
			if ( (FQ->ubFJob == BYTE_FJOB_READ)  && ((guoFlashUseType.B.btA2Cmd == 0) && (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE)) && (ubSequentialFQNum == 3) ) {
				if ( (FQ->ubFJob == BYTE_FJOB_READ) ) {
					goto NOT_SEQUENTIAL;
				}
			}
#endif
			ubSequentialFQNum++;
			ubBurstBankOrder = (ubBurstBankOrder + 1) & gubBurstsPerBankMask;
		}
		else {
		NOT_SEQUENTIAL:
			if (FQ->ubFJob == BYTE_FJOB_WRITE) {
				if (gubFQNotSequential[ubCEIndex] == 1) {
					gubFQNotSequential[ubCEIndex] = 0;
				}
				gubFQNeedExecuteRightNow[ubCEIndex] = 1;
			}
			else if (FQ->ubFJob == BYTE_FJOB_READ) {
				if (guoFlashUseType.B.btRandCacheRead ) {
					gubFQNotSequential[ubCEIndex] = 1;
				}
				else {
					gubFQNeedExecuteRightNow[ubCEIndex] = 1;
				}
			}
			break;
		}

		PreviousFQ = FQ;
		ubCFQIndex = ((ubCFQIndex + 1)&FQ_NUM_MASK);
		ubgFQLinkIndex = gFQI.ubFQIndex[ubCEIndex][ubCFQIndex];
		FQ = &gFQI.gFQLink[ubgFQLinkIndex];
		ubUnServedCount--;
	}
	if (ubNeedFQNoWait && (ubUnServedCount == 0)) {
		gubFQNeedExecuteRightNow[ubCEIndex] = 1;
	}

	return ubSequentialFQNum;
}

UBYTE flaSendCmd_CH(UBYTE ubgFQLinkIndex, UBYTE ubCEIndex, UWORD uwFPU, UBYTE ubAutoPoll, UBYTE ubSendINT, UBYTE ubPolSeqSel) {
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
#endif
	UBYTE ubDepth;
	UBYTE ubChannel;
	UBYTE ubRemainQCnt;
	UBYTE ubRemainQCnt2;

	M_AssertError(gMTDepth.ubLinkNum == 0);
	M_GetMTDepth(ubDepth);

	if ((ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL) && gulPara0x00.BitMap.btForceWriteFail) {
		/* 因為ForceWriteFail當初是走PFA (program fail autopolling)的flow去做的, 但PFA有問題所以用ForceWriteFail也會有其他不預期的問題,
		    改用pol true ready的mask來造STA fail (作到這邊一定已經true ready, 所以pol true ready bit一定符合interrupt絛件, 可以造STA fail)
		*/
		gulPara0x00.BitMap.btForceWriteFail = 0;
		uwFPU = FPU_PTR_C70_DAT_MK40;
	}

	//[CC]: 不可用 btForceWriteFail 可能會有問題
	if (gulPara0x00.BitMap.btForceWriteFail) {
		// 這邊攔住上面沒攔到的情況, 把 btForceWriteFail 清掉
		//UartString("\n Clear WF");
		gulPara0x00.BitMap.btForceWriteFail = 0;
	}

	if (ENABLE_DEBUG_RW) {
		ubRemainQCnt = 0;
		ubRemainQCnt2 = 0;
		ubChannel = ubCEIndex & gubPlanesPerBurstMask;
		gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
	}
	M_VirtualToPhysicalCE(ubCEIndex);

	gulPara0x24.BitMap.ubFrameNum = 0;
	gulPara0x04.BitMap.btIntVctEn = ubSendINT;
#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		ubChannel = ubCEIndex & gubPlanesPerBurstMask;
		gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][ubCEIndex / gubPlanesPerBurst]);
		gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
	}
	else {
		gulPara0x1C.BitMap.ubCEValue = ubCEIndex;
	}
	gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
	gulPara0x00.BitMap.btSetInternalBusy  = ubAutoPoll;
	gulPara0x00.BitMap.btQueryCheck = 0;
#if MicronFlashOnly || YMTC_JGS
	if (gulParamPageInfo.ONFI.btNandSupplyByEmulator && (uwFPU == FPU_PTR_AG_C60_A3_CD0))
#else
	if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator && (uwFPU == FPU_PTR_AG_C60_A3_CD0))
#endif
	{
		U32 uliFSA;
		uliFSA = 0x00001040;
		gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = uliFSA;
		gulPara0x04.BitMap.btiFSAEn = 1;
		gulPara0x04.BitMap.ubALUSel = ALU_NORMAL;
	}

	gulPara0x00.BitMap.btStaNoStop  = 0;

#if FixShallowErase
	if (gubSkipProgramFail) {
		//gulPara0x00.BitMap.btStaNoStop  = 1;
	}
#endif
	if (ubAutoPoll) {
		gulPara0x1C.BitMap.ubPolSeqSel  = ubPolSeqSel;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
		FQ->btUsingAutoPol = 1;
		FQ->ulAutoPolStartTime = gulOperationTime;
#endif
#if FORCE_AUTOPOL_TIMEOUT
		if ((gubDebugAutoPolFPUSeqInUse == 0) && (gubPreformat == 0) && (VT->gulFTLState.B.btNeedRUTReplace == 0)) {
			gulRandomValue = getIntRandValue(FORCE_AUTOPOL_TIMEOUT_DENOMINATOR);
			if (gulRandomValue < 0) {//FORCE_AUTOPOL_TIMEOUT_NUMERATOR) {
				gulPara0x1C.BitMap.ubPolSeqSel = WAIT_AUTOPOL_TIMEOUT;
				gubDebugAutoPolFPUSeqInUse = 1;
				gubDebugAutoPolFQIndex = gubDebugSendCmdFQIndex;
				gubDebugAutoPolMTDepth = ubDepth;
#if UART_AUTOPOL_TIMEOUT_HANDLING
				UartString("\nSendCmdPol Timeout");
#endif
			}
		}
#endif
	}
	gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
	gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
	gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
	gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
#if FORCE_AUTOPOL_TIMEOUT
	if (ubAutoPoll) {
		// restore PolSeqSel
		gulPara0x1C.BitMap.ubPolSeqSel = ubPolSeqSel;
	}
#endif
#pragma flush
	if (ENABLE_DEBUG_RW) {
		while (1) {
			if (ubCEIndex < 8) {
				ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
			}
			else {
				ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
			}
			if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
				M_AssertError(1);
			}
			if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
				break;
			}
		}
	}
	FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | REUSEMT_STOP | QUEUE_INDEX(ubCEIndex));
	gubMTDepthPushNumber[ubCEIndex]++;

	return 0;
}
UBYTE flaFlashToSram_CH(UBYTE ubPhase, UBYTE ubgFQLinkIndex, UWORD uwFPU, UBYTE ubMode, UBYTE ubPolSeqSel) {
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubDepth;
	U32 ulFEntry = FQ->ulFEntry;
	UBYTE ubBurstBank, ubPhyPlane;
	UBYTE ubChannel;
	UBYTE ubRemainQCnt;
	UBYTE ubRemainQCnt2;
	UWORD uwFBlock;
	U32 uliFSA;
#if ( TLC && (B0KB == 0))
	UBYTE ubD3LMU;
	UBYTE ubiFSACnt_8LC;
#endif
	UBYTE ubD1Unit = Default_D1_Unit;

	if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
#if MicronFlashOnly
		if (!gubMicronModeSet) {
			M_AssertError((FQ->ubFQPhase == 0) && FQ->ubMTDepthCnt);
		}
		else {
			M_AssertError((FQ->ubFQPhase == 0) && (FQ->ubMTDepthCnt != 1));
		}
#else
		M_AssertError((FQ->ubFQPhase == 0) && FQ->ubMTDepthCnt);
#endif
	}

	if (ENABLE_DEBUG_RW) {
		ubRemainQCnt = 0;
		ubRemainQCnt2 = 0;
		ubChannel = ubCEIndex & gubPlanesPerBurstMask;
		gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
	}

#if TLC
	if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
		ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
		if ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
			ulFEntry = FQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
		}
	}
#endif

	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
	}

	M_VirtualToPhysicalCE(ubCEIndex);

	M_AssertError(FQ->ubFJob != BYTE_FJOB_READ);

	if (ENABLE_iFSA) {

		if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
			uwFBlock = FQ->uwFUnit;
			ubPhyPlane = ubBurstBank;
		}
		else {
			if ((TLC == 0) || (FQ->btD3 || (ubD1Unit == Default_D1_Unit))) {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex, (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
			}
			else {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
			}
			if (gubRUTMixPlaneEnable == 0) {
				ubPhyPlane = ubBurstBank;
			}
			else {
				ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
				uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
			}
		}

		if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
			if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
				uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
			}
		}

		M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);
		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				FQ->ubALUIndex = ALU_A2;
				goto Mark_NormalSectorAddress;
			}
			else {
				FQ->ubALUIndex = ALU_NORMAL;
#if (B0KB || YMTC_JGS)
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
			}
		}
		else {
			FQ->ubALUIndex = ALU_NORMAL;
		Mark_NormalSectorAddress:
#if TLC
			if (TLC) {
#if (B0KB || YMTC_JGS)
				uliFSA =  (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				if (FQ->ubALUIndex == ALU_NORMAL) { // D3 Read
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask)); // 3:LMU
				}
				else {
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask));
				}
#endif
			}
			else {
#else
			{
#endif
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			}
		}

		uliFSA |= (uwFBlock & 0xFFFF) << 16;

		if ((((uwFBlock >= gulTotalPhyUnitNum) && ((gubDieNumber == 1) && (gubExpandCE == 0))) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog)))) {
			VT->gulDebugInfo[0] = uwFBlock;
			VT->gulDebugInfo[1] = gulTotalPhyUnitNum;
			VT->gulDebugInfo[2] = ulFEntry;
			VT->gulDebugInfo[3] = (gulPlanesPerUnit << gub4kEntrysPerPlaneLog);
			VT->gulDebugInfo[4] = ubgFQLinkIndex;
			M_AssertCriticalError(1, VT_ASSERT_FLASHTOSRAM_CH_0x43); // SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum
		}


		gulPara0x04.BitMap.btiFSAEn = 1;
	}
#if ( TLC && (B0KB == 0))
	if (TLC) {
		ubiFSACnt_8LC = 1;

		if ( (uwFPU == FPU_PTR_AG_C00_DR) || ( uwFPU == FPU_PTR_AG_C00_A5_C05_A2_CE0_DR)  ) {
#if Hynix || YMTC
			if (guoFlashDefaultType.B.btHynix || guoFlashDefaultType.B.btYMTC) {
				uwFPU = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
			}
#else
			if (!gulDQSREMismatchMap) {
				uwFPU = RAM_SEQ_PTR16(FPU_AG_C05_A5_CE0_DR);
			}
			else {
				uwFPU = RAM_SEQ_PTR16(FPU_AG_DLY_C05_A5_CE0_DR);
			}
#endif
		}

		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if ( uwFPU == FPU_PTR_C31 ) {

				if (guoFlashUseType.B.btFourPlane) {
					ubiFSACnt_8LC = 4;
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31;
				}
				else {
					ubiFSACnt_8LC = 2;
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31;

				}
				//D1 SLC or D3 SLC
			}
#if (Hynix3DV6 || Hynix3DV5)
			else if ( uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30) {
				ubiFSACnt_8LC = 4;
				if ((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA)) {
					uwFPU = RAM_SEQ_PTR16(FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_NOP_AG_C00_A5_C32_C70_POL_MK40_NOP_AG_C00_A5_C32_C70_POL_MK40_AG_NOP_C00_A5_C35);
				}
				else {
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30;
				}


			}
#endif
			else if ( uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30 ) {
				ubiFSACnt_8LC = 2;
#if Hynix
#if (Hynix3DV6 || Hynix3DV5)
				if ((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA)) {
					uwFPU = RAM_SEQ_PTR16(FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_CA2_C00_A5_C35);
				}
				else {
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30;
				}
#else
				if (gubDoFlushD1) {
					uwFPU = RAM_SEQ_PTR16(FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_CA2_C00_A5_C35);
				}
				else {
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30;
				}
#endif

#elif YMTC
				if ((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA)) {
					uwFPU = RAM_SEQ_PTR16(FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_CA2_C00_A5_C35);
				}
				else {
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30;
				}
#else
				uwFPU = FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30;
#endif
				//D1 SLC or D3 SLC
			}
		}
		else {
			ubD3LMU = ( ( (M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) % 3 ) + 1 );
			if ( uwFPU == FPU_PTR_C31 ) {
#if Hynix3DV6
				if (guoFlashUseType.B.btFourPlane) {

					ubiFSACnt_8LC = 4;
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31);
					}
				}
				else
#endif
				{
					ubiFSACnt_8LC = 2;
#if (TLC_BICS2 & (!BURNER))
					if (guoFlashUseType.B.btSandisk) {
						if ( ubD3LMU == 1) {
							uwFPU = RAM_SEQ_PTR16(FPU_C5D_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C01_C00_A5_C31);
						}
						else if ( ubD3LMU == 2) {
							uwFPU = RAM_SEQ_PTR16(FPU_C5D_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C02_C00_A5_C31);
						}
						else {
							uwFPU = RAM_SEQ_PTR16(FPU_C5D_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C03_C00_A5_C31);
						}
					}
					else
#endif
					{
#if YMTC_JGS
						uwFPU = FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31;
#else
						if ( ubD3LMU == 1) {
							uwFPU = RAM_SEQ_PTR16(FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31);
						}
						else if ( ubD3LMU == 2) {
							uwFPU = RAM_SEQ_PTR16(FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31);
						}
						else {
							uwFPU = RAM_SEQ_PTR16(FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31);
						}

#endif


						//D3 TLC
					}
				}
			}
#if (Hynix3DV6 || Hynix3DV5)
			else if ( uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30 ) {
				ubiFSACnt_8LC = 4;

				if ( ubD3LMU == 1) {
					uwFPU = RAM_SEQ_PTR16(FPU_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30);
				}
				else if ( ubD3LMU == 2) {
					uwFPU = RAM_SEQ_PTR16(FPU_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30);
				}
				else {
					uwFPU = RAM_SEQ_PTR16(FPU_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30);
				}

			}
#endif
			else if ( uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30 ) {
				ubiFSACnt_8LC = 2;

#if (TLC_BICS2 & (!BURNER))
				if (guoFlashUseType.B.btSandisk) {
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C01_C00_A5_C30);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C02_C00_A5_C30);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C03_C00_A5_C30);
					}
				}
				else
#endif
				{

#if YMTC_JGS
					uwFPU = FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30;
#else
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30);
					}

#endif
					//D3 TLC
				}
			}
			else if ( uwFPU == FPU_PTR_AG_C00_A5_C30 ) {
#if (TLC_BICS2 & (!BURNER))
				if (guoFlashUseType.B.btSandisk) {
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C01_AG_C00_A5_C30);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C02_AG_C00_A5_C30);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C03_AG_C00_A5_C30);
					}
				}
				else
#endif
				{

#if YMTC_JGS

#else
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C01_AG_C00_A5_C30);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C02_AG_C00_A5_C30);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C03_AG_C00_A5_C30);
					}
#endif

				}
				//D3 TLC
			}
			else if ( uwFPU == FPU_PTR_AG_C00_A5_C31 ) {
#if (TLC_BICS2 & (!BURNER))
				if (guoFlashUseType.B.btSandisk) {
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C01_AG_C00_A5_C31);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C02_AG_C00_A5_C31);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C5D_C03_AG_C00_A5_C31);
					}
				}
				else
#endif
				{

#if YMTC_JGS

#else
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C01_AG_C00_A5_C31);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C02_AG_C00_A5_C31);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C03_AG_C00_A5_C31);
					}
#endif
				}
				//D3 TLC
			}
		}
	}
#else//MLC

#if HV// To avoid read out fail
	if (uwFPU == FPU_PTR_AG_C00_DR || uwFPU == FPU_PTR_AG_C00_A5_C05_A2_CE0_DR) {
		if (!gulDQSREMismatchMap) {
			uwFPU = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
		}
		else {
			uwFPU = RAM_SEQ_PTR16(FPU_AG_DLY_C00_A5_C05_A2_CE0_DR);
		}
	}
#else
	//DQS delay for toshiba
	if ((uwFPU == FPU_PTR_AG_C00_DR || uwFPU == FPU_PTR_AG_C00_A5_C05_A2_CE0_DR) && gulDQSREMismatchMap && guoFlashUseType.B.btToshiba) {
		uwFPU = RAM_SEQ_PTR16(FPU_AG_DLY_C00_A5_C05_A2_CE0_DR);
	}

#endif

#endif
	//if (ENABLE_CACHEREAD && (gul4KRandomCacheRead == 0)) {
	if (ENABLE_MULTIPLANE_READ) {
#pragma frequency_hint NEVER
		if (ubPhase == BYTE_CMD_ADDR_DONE) {
			gulPara0x00.BitMap.btQueryCheck  = 0;
			gulPara0x00.BitMap.btSetInternalBusy = 1;
			gulPara0x00.BitMap.btStaNoStop  = 0;
			gulPara0x04.BitMap.btIntVctEn = 1;
			gulPara0x04.BitMap.btUltraDMADisable = 0;

			if (ubMode & GET_EXTRA_DEPTH) {	// 1st cmd MT or last 3Fh MT
				M_AssertError(gMTDepth.ubLinkNum == 0);
				M_GetMTDepth(ubDepth);
				FQ->ubRelativeDepth = ubDepth;

				if (uwFPU != FPU_PTR_C3F) {
					FQ->btNeedSetCEWhenTriggerDMA = 1;
				}
			}
			else {
#pragma frequency_hint FREQUENT
				ubDepth = FQ->ubDepth;
			}

			if (uwFPU != FPU_PTR_C3F) {
#pragma frequency_hint FREQUENT
				M_AssertError(FQ->ubFQPhase != 0);	// issue 0x3F時, FQ->ubFQphase已是BYTE_CMD_ADDR_DONE
				gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex;
				if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
					gulPara0x10.BitMap.ubFSASel = FQ->ubFSAIndex;
				}
			}

			if (ENABLE_iFSA) {
				UBYTE ubiFSACnt = 1;

				guliFSA[0] = uliFSA;

#if MicronFlashOnly
#if B0KB
				if ( (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30) || ( uwFPU == FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30 )
				        || (uwFPU == FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35) )
#else
				if ( (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30) || ( uwFPU == FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30 ) )

#endif

#else
				if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30)
#endif
				{
					ubiFSACnt = 4;

				}
#if MicronFlashOnly
#if B0KB
				else if ( (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30) || (uwFPU == FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30 )
				          || (uwFPU == FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35) )
#else
				else if ( (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30) || (uwFPU == FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30) )
#endif
#else
				else if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30)
#endif
				{
					ubiFSACnt = 2;

				}

#if ( TLC && (B0KB == 0))
				if (TLC) {
					ubiFSACnt = ubiFSACnt_8LC;
				}
#endif
				if (ENABLE_READ_CHECK_FUNCTION) {
					if (uwFPU != FPU_PTR_C3F) {
						UBYTE ubA2Mode;
#if TLC
						ubA2Mode = ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) ?  1 : 0;
#else
						ubA2Mode = (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;
#endif
						if (uwFPU == FPU_PTR_C31) {
							flaGetUnitReadCheckInfo(ubgFQLinkIndex, gubBurstsPerBank , ubA2Mode); //依照Phycial Plane 數
						}
						else {
							flaGetUnitReadCheckInfo(ubgFQLinkIndex, ubiFSACnt, ubA2Mode); //ubiFSACnt 等同Plane數
						}
					}
				}
				for (ubBurstBank = 1; ubBurstBank < ubiFSACnt; ubBurstBank++) {

					if (ENABLE_PLANE_CH_CE) {
						ulFEntry +=  gub4kEntrysPerPlane;
					}
					else {
						ulFEntry +=  gub4kEntrysPerBurst;// flaCheckFQContinueNum 已經先卡好會是planeA ~ planeD
					}

					if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
						uwFBlock = FQ->uwFUnit;
						uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
					}
					else {
						if ((TLC == 0) || FQ->btD3 || (ubD1Unit == Default_D1_Unit)) {
							uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
						}
						else {
							uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
						}
						if (gubRUTMixPlaneEnable == 0) {
							ubPhyPlane = ubBurstBank;
						}
						else {
							ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
							uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
						}
					}

					if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
						if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
							uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
						}
					}
					M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

					if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
						if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
							FQ->ubALUIndex = ALU_A2;
#if (B0KB || YMTC_JGS)
							uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
							uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
						}
						else {
							FQ->ubALUIndex = ALU_NORMAL;
#if (B0KB || YMTC_JGS)
							uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
							uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
						}
					}
					else {
						FQ->ubALUIndex = ALU_NORMAL;
#if TLC
						if (TLC) {
#if (B0KB || YMTC_JGS)
							uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
							uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask)); // 3:LMU
#endif
						}
						else {
#else
						{
#endif
							uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
						}
					}

					uliFSA |= (uwFBlock & 0xFFFF) << 16;

					M_AssertError(((uwFBlock >= gulTotalPhyUnitNum) && (!ENABLE_MULTI_DIE)) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog)));	// SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum

					guliFSA[ubBurstBank] = uliFSA;

				}

				gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = guliFSA[0];
				gMTQ->Depth[ubDepth].uliFSA1 = guliFSA[1];
				gMTQ->Depth[ubDepth].uliFSA2 = guliFSA[2];
				gMTQ->Depth[ubDepth].uliFSA3 = guliFSA[3];

				if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
					FQ->ubFSAIndex = 0xFF;
				}
			}
			else {
				// FSA ring回FSA 0的處理
				gulPara0x10.BitMap.ubFSA2Cnt = 0;
				if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30) {
					if (FQ->ubFSAIndex > 124) {
						gulPara0x10.BitMap.ubFSA2Cnt = 4 - (128 - FQ->ubFSAIndex);
					}
				}
				else if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30) {
					if (FQ->ubFSAIndex == 127) {
						gulPara0x10.BitMap.ubFSA2Cnt = 3;
					}
				}
			}

			M_AssertError((guoFlashUseType.B.btRandCacheRead == 0) && (uwFPU == FPU_PTR_AG_C00_A5_C31));


#if (ENABLE_RESET_RETRY_LEVEL && (!TLC))
			if (((BIT0 << ubCEIndex) & gulKeepRetryLevelCE_BITMAP) && (FQ->ubALUIndex != ALU_A2) && (!guoFlashUseType.B.btSandisk)) {

				if ( uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30 ) {
					uwFPU = (gub26CMDflag) ? RAM_SEQ_PTR16(FPU_C26_C5D_AG_C00_A5_C32_C70_POL_MK40_AG_C26_C5D_C00_A5_C30) : RAM_SEQ_PTR16(FPU_C5D_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C00_A5_C30);
				}
				else if ( uwFPU == FPU_PTR_AG_C00_A5_C30 ) {
					uwFPU = (gub26CMDflag) ? RAM_SEQ_PTR16(FPU_PTR_AG_C26_C5D_C00_A5_C30) : RAM_SEQ_PTR16(FPU_PTR_AG_C5D_C00_A5_C30);
				}
				else if ( uwFPU == FPU_PTR_AG_C00_A5_C31 ) {
					uwFPU = (gub26CMDflag) ? RAM_SEQ_PTR16(FPU_PTR_AG_C26_C5D_C00_A5_C31) : RAM_SEQ_PTR16(FPU_PTR_AG_C5D_C00_A5_C31);
				}
			}
#endif

#if MicronFlashOnly
			if ((FQ->ubALUIndex == ALU_A2) && guoFlashUseType.B.btToshiba)
#else
			if (FQ->ubALUIndex == ALU_A2)
#endif
			{
#pragma frequency_hint FREQUENT
				if (uwFPU == FPU_PTR_AG_C00_A5_C30) {
#pragma frequency_hint FREQUENT
#if Hynix
					uwFPU = (gubDaCmdEnable) ? (FPU_PTR_AG_C00_A5_C30) : (FPU_PTR_AG_CA2_C00_A5_C30);
#else
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C30;
#endif
					/*	if (ENABLE_READ_CHECK_FUNCTION) { //(RD)
							flaGetUnitReadCheckInfo(ubgFQLinkIndex, 1);
						}*/

					//D1 SLC or D3 SLC
				}
				else if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30) {
					uwFPU = FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30;
				}
				else if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_C30) {
					uwFPU = FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30;
				}
				else if (uwFPU == FPU_PTR_AG_C00_A5_C31) {
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C31;
					//D1 SLC or D3 SLC
					/*	if (ENABLE_READ_CHECK_FUNCTION) { //(RD)
							flaGetUnitReadCheckInfo(ubgFQLinkIndex, 1);
						}*/

				}
				else if (uwFPU == FPU_PTR_C3F) {
					uwFPU = FPU_PTR_CA2_C3F;
				}
				else if (uwFPU == FPU_PTR_C31) {
					uwFPU = FPU_PTR_CA2_C31;
				}
			}
#if TLC
			gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;
#endif
			gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
				gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
			}
			else {
				gulPara0x1C.BitMap.ubCEValue  = ubCEIndex;
			}
			gulPara0x1C.BitMap.ubPolSeqSel  = ubPolSeqSel;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
			FQ->btUsingAutoPol = 1;
			FQ->ulAutoPolStartTime = gulOperationTime;
#endif
#if FORCE_AUTOPOL_TIMEOUT
			if ((gubDebugAutoPolFPUSeqInUse == 0) && (gubPreformat == 0) && (VT->gulFTLState.B.btNeedRUTReplace == 0)) {
				gulRandomValue = getIntRandValue(FORCE_AUTOPOL_TIMEOUT_DENOMINATOR);
				if (gulRandomValue < FORCE_AUTOPOL_TIMEOUT_NUMERATOR) {
					gulPara0x1C.BitMap.ubPolSeqSel = WAIT_AUTOPOL_TIMEOUT;
					gubDebugAutoPolFPUSeqInUse = 1;
					gubDebugAutoPolFQIndex = ubgFQLinkIndex;
					gubDebugAutoPolMTDepth = ubDepth;
#if UART_AUTOPOL_TIMEOUT_HANDLING
					UartString("\nMPRPol Timeout");
#endif
				}
			}
#endif
			gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
			gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
			gMTQ->Depth[ubDepth].Para0x10.ulAll = gulPara0x10.ulAll;
			gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
#if FORCE_AUTOPOL_TIMEOUT
			// restore PolSeqSel
			gulPara0x1C.BitMap.ubPolSeqSel = ubPolSeqSel;
#endif

#pragma flush
			if (ENABLE_DEBUG_RW) {
				while (1) {
					if (ubCEIndex < 8) {
						ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
					}
					else {
						ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
					}
					if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
						M_AssertError(1);
					}
					if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
						break;
					}
				}
			}
			FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | ((ubMode & USE_REUSE_MT) ? 0 : REUSEMT_STOP) | QUEUE_INDEX(ubCEIndex));

		}
		else if (ubPhase == BYTE_TRIG_DMA_DONE) {
			M_AssertError((FQ->ubFQPhase != BYTE_CMD_ADDR_DONE) && (FQ->ubFQPhase != BYTE_WAIT_DMA) &&  (FQ->ubFQPhase != BYTE_WAIT_MTQFREE));
			M_AssertError(FQ->btNeedStopDMA);

			ubDepth = FQ->ubDepth;

			gulPara0x00.BitMap.btStaNoStop  = 0;
			gulPara0x00.BitMap.btQueryCheck  = FQ->FQPara.B.btQueryCheck;
			gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex; // 僅在flaSetFlashSourceAddrCE_CH重新設定FQ->ubALUIndex
			gulPara0x04.BitMap.btUltraDMADisable = 0;
			gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;

#if TLC
			if (TLC) {
				if (FQ->FQPara.B.btConvBypass) {
					gulPara0x04.BitMap.btConvLogicPageEn = 0;
				}
				else {
#if TSB_BICS4_SUPPORT || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
					gulPara0x04.BitMap.uwConvLogicPage = (M_RefSeedPage(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#else
					gulPara0x04.BitMap.uwConvLogicPage = (M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#endif
					gulPara0x04.BitMap.btConvLogicPageEn = 1;
				}
			}
#endif
			if ((ubMode & USE_REUSE_MT)) {
#pragma frequency_hint FREQUENT
				if (ENABLE_iFSA) {
					guliFSA0_2 = uliFSA;
					gMTQ->Depth[ubDepth].uliFSA0_2 = guliFSA0_2;
				}
				else {
					gulPara0x20.BitMap.ubFSASel2 = FQ->ubFSAIndex;
				}

				gulPara0x00.BitMap.btSetInternalBusy = 1;	// 因為下面會再次assign Para0x00, 所以需維持其cmd phase的autopol, 不然會下完cmd沒有autopol就DMA, 造成LCA cmp/signoffmask, unzip fail...

				gulPara0x20.BitMap.btSetInternalBusy2 = 0;
				gulPara0x20.BitMap.btbtIntVctEn2 = 1;
				gulPara0x20.BitMap.uw2ndFPUPtr = uwFPU;
				gMTQ->Depth[ubDepth].Para0x20.ulAll = gulPara0x20.ulAll;

				if (FQ->btNeedSetCEWhenTriggerDMA) {
#if(UFS)
					if (1)
#else
					if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
					{
						gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
						gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
					}
					else {
						gulPara0x1C.BitMap.ubCEValue  = ubCEIndex;
					}
					gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
				}
			}
			else {
				if (ENABLE_iFSA) {
					guliFSA[0] = uliFSA;
					gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = guliFSA[0];
				}
				else {
					gulPara0x10.BitMap.ubFSASel = FQ->ubFSAIndex;
					gMTQ->Depth[ubDepth].Para0x10.ulAll = gulPara0x10.ulAll;
				}
				gulPara0x00.BitMap.btSetInternalBusy = 0;
				gulPara0x04.BitMap.btIntVctEn = 1;
#if(UFS)
				if (1)
#else
				if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
				{
					gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
					gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
				}
				else {
					gulPara0x1C.BitMap.ubCEValue  = ubCEIndex;	// single MT的DMA都需要在此設定CE
				}
				gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
				gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
			}

#if ENABLE_RUNTIME_SEEDINIT
			// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
			gMTQ->Depth[ubDepth].ulSeedInit = M_RandomSeed((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)), ((guoFlashUseType.B.btHynix3DV6) ? 512 : 384));
#elif TSB_BICS4_SUPPORT
			gMTQ->Depth[ubDepth].ulSeedInit = flaGenRandSeed(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#else
			if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 512) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x0;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1024) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0xffffff;
			}
#if TLC_BICS2
#else /* TLC_BICS2 */
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1536) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x555555;
			}
#if B0KB
#else
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2048) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0xAAAAAA;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2560) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0xCCCCCC;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3072) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x333333;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3584) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x999999;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 4096) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x666666;
			}
#endif
#endif /* TLC_BICS2 */
#endif
			// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#endif /* ENABLE_RUNTIME_SEEDINIT */
			gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
			gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;

			/* 由於FQn+1 的cmd 綁 FQn的DMA, 下完FQn+1 其會有殘留值在gulPara0x1C, 如果直接assign gulPara0x1C.ulAll給FQn的DMA MT
			    雖然是要trigger reusetMT2的CE, 卻會連帶偷改到FQn的1st FPU (ex: FQn為 non-A2, FQn+1為A2, 結果FQn的1st FPU被改成A2)
			*/

			/*
			if (gFQI.ubXferPhase[ubCEIndex] != BYTE_CER_0x3F_CMD_DONE) {
				gulPara0x1C.BitMap.ubCEValue  = ubCEIndex;
				gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
			}
			*/


			FQ->FQPara.B.btCanRemove = 1;

			if (gubGenReadFailDebug && FQ->btGenFail) {
				if ((Failed_Test_UNC || Failed_Test_E3D) && (FQ->uwFJobInfo & BIT_FJOBI_BUFFERMODE) && (FQ->FQPara.B.btUserData)) {
					if (Failed_Test_UNC) {
						volatile UWORD uwfailGen;
						if (ENABLE_KT_DEBUG_MOREFAIL)
							uwfailGen = getIntRandValue(0x100);	//rand()&0xff
						else
							uwfailGen = getIntRandValue(0x4000);	//rand()&0x3fff

						if (uwfailGen == 0) {
							//UartString("\r\nUnc!");
							//UartLLongHex(guoRCount);
							gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
						}
					}

					if (Failed_Test_E3D && (!FQ->btPreRead)) { //H_E3D_EN在Write方向會讓Flh舉E3D error, 如果非host獨走,可能影響到下一筆Cmd
						volatile UWORD uwfailGen;
						if (ENABLE_KT_DEBUG_MOREFAIL)
							uwfailGen = getIntRandValue(0x100);	// rand() & 0xff;
						else
							uwfailGen = getIntRandValue(0x4000);


						if (uwfailGen == 0) {
							HW[HW_MZ_CTRL] |= H_E3D_EN;
						}
					}
				}

#if (ENABLE_GEN_FAIL_WHEN_SCAN_ACTIVE_GR || ENABLE_GEN_FAIL_WHEN_COPY_UNIT)
				if (ENABLE_DEBUG_HW_FORCEREADFAIL && gubBunchRSCorrectFlag && FQ->btBunchRSParity && ((guwRUTLogNum < (RUT_LOG_NUM / 2)) || (VT->gulFTLState.B.btNeedRUTReplace)))
#else
#if (TLC && ENABLE_DEBUG_HW_FORCEREADFAIL)
				/*
				//----------------------------------------------
				// [CC_Merge_Info] : Just for TLC D1 D3 Read Fail Verify
				//----------------------------------------------
				*/
				UBYTE ubGenForceReadFail = 0;
				if (ENABLE_DEBUG_HW_FORCEREADFAIL && (( FQ->uwFJobInfo &  BIT_FJOBI_NON_DMA ) == 0)) {
					/* Avoid to gen read fail while doing CopyBack */
					switch (ENABLE_DEBUG_HW_FORCEREADFAIL_DATA_REGION)	{	//0: Default, 1: D1 Only, 2: D3 Only
						case 1: /* D1 Only */
							ubGenForceReadFail = ((FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit));
							break;
						case 2: /* D3 Only without A2 Fast Page (Table) */
							ubGenForceReadFail = (((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex == Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link == Default_D1_Unit) && ((FQ->FQPara.B.btRWTableUnit) == 0));
							break;
						case 3:/* D3 + Table */
							ubGenForceReadFail = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex == Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link == Default_D1_Unit);
							break;
						case 4: /* D1 + D3 without Table*/
							ubGenForceReadFail = ((FQ->FQPara.B.btRWTableUnit) == 0);
							break;
						case 0: /* Default */
						default:
							ubGenForceReadFail = 1;
							//[CC_TEMP] : Test without table
							//ubGenForceReadFail = ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE)==0);
							break;
					}
				}

				if (ENABLE_DEBUG_HW_FORCEREADFAIL && ubGenForceReadFail && (gubPreformat == 0) && (FQ->btCheckEmpty == 0) && ((guwRUTLogNum < (RUT_LOG_NUM / 2)) || (VT->gulFTLState.B.btNeedRUTReplace)))
#else
				if (ENABLE_DEBUG_HW_FORCEREADFAIL && (gubPreformat == 0) && (FQ->btCheckEmpty == 0) && ((guwRUTLogNum < (RUT_LOG_NUM / 2)) || (VT->gulFTLState.B.btNeedRUTReplace)))
#endif
#endif
				{
					/* start if ENABLE_DEBUG_HW_FORCEREADFAIL */
#if GEN_FAIL_PER_MILLE>0
					if (getIntRandValue(1000) >= (1000 - GEN_FAIL_PER_MILLE)) {
#else
					if (getIntRandValue(100) >= 92) {
#endif
						if (getIntRandValue(100) >= ENABLE_DEBUG_HW_FORCEREADFAIL_MT_PERCENTAGE) {
							UBYTE ubLoop = 0;
							UBYTE ubLoopCount = 0;
							ubLoopCount = (getIntRandValue(4) + 1);
							while (ubLoop < ubLoopCount) {
								gFREG[FCTLL_UNC_CFG].L |= FORCE_BCH_UNCORRECTABLE_FRAME(1 << (getIntRandValue(4)));
								ubLoop++;
							}
						}
						else {
							//UartString("\n\nUNC: ");
							//Uart_Tx_DataHex(ubgFQLinkIndex);
							//UartString("\n");
							gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
						}
					}
				} /* end if ENABLE_DEBUG_HW_FORCEREADFAIL */

				if (ENABLE_BURNER_FORCEGENFAIL && (FQ->btCheckEmpty == 0)) {
					if (getIntRandValue(2)) {
						gFREG[FCTLL_UNC_CFG].L |= getIntRandValue(0x11);	//rand() & 0x10
					}
					else {
						gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
					}
				}
			}
#pragma flush
			if (ENABLE_DEBUG_RW) {
				while (1) {
					if (ubCEIndex < 8) {
						ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
					}
					else {
						ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
					}
					if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
						M_AssertError(1);
					}
					if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
						break;
					}
				}
			}
			if (ENABLE_CY_DEBUG) {
				L4KTable16B * pL4KTablePtr;
				UBYTE ubL4KCount;
				for (ubL4KCount = 0; ubL4KCount < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					M_AssertError(pL4KTablePtr->Para0x04.BitMap.ubL4K_SPRV);
				}
			}
			FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | REUSEMT_STOP | QUEUE_INDEX(ubCEIndex));
			//FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) |REUSEMT_AUTO_TRIG |QUEUE_INDEX(ubCEIndex)); //Reuse MT auto trig

#if TLC
			VT->guoTotalNandReadSectorCnt += (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum * SectorsPer4K);
#else
			VT->guoTotalNandReadSectorCnt += gubSectorsPerPlane;
#endif

		}
	}
	else {
		ubDepth = FQ->ubDepth;
		if (ubPhase == BYTE_CMD_ADDR_DONE) {
#if MicronFlashOnly
			if ((FQ->ubALUIndex == ALU_A2) && guoFlashUseType.B.btToshiba)
#else
			if (FQ->ubALUIndex == ALU_A2)
#endif
			{
				if (uwFPU == FPU_PTR_AG_C00_A5_C30) {
#if Hynix
					uwFPU = (gubDaCmdEnable) ? (FPU_PTR_AG_C00_A5_C30) : (FPU_PTR_AG_CA2_C00_A5_C30);
#else
					uwFPU = FPU_PTR_AG_CA2_C00_A5_C30;
#endif
				}
				else {
					M_AssertError(1); // normal read不會有其他FPU
				}
			}
			if (ENABLE_READ_CHECK_FUNCTION) {
				//				flaGetUnitReadCheckInfo(ubgFQLinkIndex, 1);
			}
			gulPara0x00.BitMap.btSetInternalBusy = 1;
			gulPara0x00.BitMap.btStaNoStop  = 0;
			gulPara0x00.BitMap.btQueryCheck  = 0;

			if (ENABLE_iFSA) {
				if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
					FQ->ubFSAIndex = 0xFF;
				}
				guliFSA[0] = uliFSA;
				gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = guliFSA[0];
			}
			else {
				gulPara0x10.BitMap.ubFSASel = FQ->ubFSAIndex;
				gMTQ->Depth[ubDepth].Para0x10.ulAll = gulPara0x10.ulAll;
			}
			gulPara0x04.BitMap.btIntVctEn = 1;
			gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex;
			gulPara0x04.BitMap.btUltraDMADisable = 0;
			gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;	// reuse MT且always只用00 30時, 可以在這的cmd phase先順便設定ConvBypass, 實際上到DMA phase才會看ConvBypass
			gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
				gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
			}
			else {
				gulPara0x1C.BitMap.ubCEValue = ubCEIndex;
			}
			gulPara0x1C.BitMap.ubPolSeqSel  = ubPolSeqSel;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
			FQ->btUsingAutoPol = 1;
			FQ->ulAutoPolStartTime = gulOperationTime;
#endif
#if FORCE_AUTOPOL_TIMEOUT
			if ((gubDebugAutoPolFPUSeqInUse == 0) && (gubPreformat == 0) && (VT->gulFTLState.B.btNeedRUTReplace == 0)) {
				gulRandomValue = getIntRandValue(FORCE_AUTOPOL_TIMEOUT_DENOMINATOR);
				if (gulRandomValue < FORCE_AUTOPOL_TIMEOUT_NUMERATOR) {
					gulPara0x1C.BitMap.ubPolSeqSel = WAIT_AUTOPOL_TIMEOUT;
					gubDebugAutoPolFPUSeqInUse = 1;
					gubDebugAutoPolFQIndex = ubgFQLinkIndex;
					gubDebugAutoPolMTDepth = ubDepth;
#if UART_AUTOPOL_TIMEOUT_HANDLING
					UartString("\nSPRPol Timeout");
#endif
				}
			}
#endif

			gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
			gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
			gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
#if FORCE_AUTOPOL_TIMEOUT
			// restore PolSeqSel
			gulPara0x1C.BitMap.ubPolSeqSel = ubPolSeqSel;
#endif

#pragma flush
			if (ENABLE_DEBUG_RW) {
				while (1) {
					if (ubCEIndex < 8) {
						ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
					}
					else {
						ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
					}
					if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
						M_AssertError(1);
					}
					if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
						break;
					}
				}
			}
			FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | QUEUE_INDEX(ubCEIndex));

		}
		else if (ubPhase == BYTE_TRIG_DMA_DONE) {
			gulPara0x00.BitMap.btQueryCheck  = FQ->FQPara.B.btQueryCheck;
			gulPara0x00.BitMap.btSetInternalBusy = 1;
			if (ENABLE_iFSA) {
				guliFSA0_2 = uliFSA;
				gMTQ->Depth[ubDepth].uliFSA0_2 = guliFSA0_2;
			}
			else {
				gulPara0x20.BitMap.ubFSASel2 = FQ->ubFSAIndex;
			}
			gulPara0x20.BitMap.uw2ndFPUPtr = uwFPU;
			gulPara0x20.BitMap.btbtIntVctEn2 = 1;
			gulPara0x20.BitMap.btSetInternalBusy2 = 0;
#if TLC
			if (TLC) {
				gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex;
				gulPara0x04.BitMap.btUltraDMADisable = 0;
				//gulPara0x04.BitMap.btIntVctEn = 1;
				if (FQ->FQPara.B.btConvBypass) {
					gulPara0x04.BitMap.btConvLogicPageEn = 0;
				}
				else {
#if TSB_BICS4_SUPPORT || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
					gulPara0x04.BitMap.uwConvLogicPage = (M_RefSeedPage(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#else
					gulPara0x04.BitMap.uwConvLogicPage = (M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#endif
					gulPara0x04.BitMap.btConvLogicPageEn = 1;
				}
				// ConvBypass的設定移到cmd phase先偷做, 減少這邊多設一次IRAM的動作
				gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;
				gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
			}
#else
			// ConvBypass的設定移到cmd phase先偷做, 減少這邊多設一次IRAM的動作
			//gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;
			//gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
#endif

#if ENABLE_RUNTIME_SEEDINIT
			// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
			gMTQ->Depth[ubDepth].ulSeedInit = M_RandomSeed((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)), ((guoFlashUseType.B.btHynix3DV6) ? 512 : 384));
#elif TSB_BICS4_SUPPORT
			gMTQ->Depth[ubDepth].ulSeedInit = flaGenRandSeed(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#else
			if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 512) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x0;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1024) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0xffffff;
			}
#if TLC_BICS2
#else /* TLC_BICS2 */
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1536) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x555555;
			}
#if B0KB
#else
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2048) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0xAAAAAA;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2560) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0xCCCCCC;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3072) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x333333;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3584) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x999999;
			}
			else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 4096) {
				gMTQ->Depth[ubDepth].ulSeedInit = 0x666666;
			}
#endif
#endif /* TLC_BICS2 */
#endif

			// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#endif /* ENABLE_RUNTIME_SEEDINIT */

			gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
			gMTQ->Depth[ubDepth].Para0x20.ulAll = gulPara0x20.ulAll;

			FQ->FQPara.B.btCanRemove = 1;

			if ((Failed_Test_UNC || Failed_Test_E3D) && (FQ->uwFJobInfo & BIT_FJOBI_BUFFERMODE) && (FQ->FQPara.B.btUserData)) {
				if (Failed_Test_UNC) {
					volatile UWORD uwfailGen;
					if (ENABLE_KT_DEBUG_MOREFAIL)
						uwfailGen = getIntRandValue(0x100);	//rand() & 0xff
					else
						uwfailGen = getIntRandValue(0x4000);	//rand() & 0x3fff

					if (uwfailGen == 0) {
						//UartString("\r\nUnc!");
						//UartLLongHex(guoRCount);
						gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
					}
				}

				if (Failed_Test_E3D && (!FQ->btPreRead)) {
					volatile UWORD uwfailGen;
					if (ENABLE_KT_DEBUG_MOREFAIL)
						uwfailGen = getIntRandValue(0x100);	//rand() & 0xff
					else
						uwfailGen = getIntRandValue(0x4000);	//rand() & 0x3fff

					if (uwfailGen == 0) {
						HW[HW_MZ_CTRL] |= H_E3D_EN;
					}
				}
			}

#if (ENABLE_GEN_FAIL_WHEN_SCAN_ACTIVE_GR || ENABLE_GEN_FAIL_WHEN_COPY_UNIT)
			if (ENABLE_DEBUG_HW_FORCEREADFAIL && gubBunchRSCorrectFlag && FQ->btBunchRSParity && (gubPreformat == 0))
#else
#if (TLC && ENABLE_DEBUG_HW_FORCEREADFAIL)
			/*
			//---------------------------------------------
			// [CC_Merge_Info] : Just for TLC D1 D3 Read Fail Verify
			//---------------------------------------------
			*/
			UBYTE ubGenForceReadFail = 0;
			if (ENABLE_DEBUG_HW_FORCEREADFAIL && ( FQ->uwFJobInfo &  BIT_FJOBI_NON_DMA ) == 0) {
				/* Avoid to gen read fail while doing CopyBack */
				switch (ENABLE_DEBUG_HW_FORCEREADFAIL_DATA_REGION)	{	//0: Default, 1: D1 Only, 2: D3 Only
					case 1: /* D1 Only */
						ubGenForceReadFail = ((FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit));
						break;
					case 2: /* D3 Only without A2 Fast Page (Table) */
						ubGenForceReadFail = (((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex == Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link == Default_D1_Unit) && ((FQ->FQPara.B.btRWTableUnit) == 0));
						break;
					case 3:/* D3 + Table */
						ubGenForceReadFail = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex == Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link == Default_D1_Unit);
						break;
					case 4: /* D1 + D3 without Table*/
						ubGenForceReadFail = ((FQ->FQPara.B.btRWTableUnit) == 0);
						break;
					case 0: /* Default */
					default:
						ubGenForceReadFail = 1;
						//[CC_TEMP] : Test without table
						//ubGenForceReadFail = ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE)==0);
						break;
				}
			}

			if (ENABLE_DEBUG_HW_FORCEREADFAIL && ubGenForceReadFail && (gubPreformat == 0) && (FQ->btCheckEmpty == 0))
#else
			if (ENABLE_DEBUG_HW_FORCEREADFAIL && (gubPreformat == 0) && (FQ->btCheckEmpty == 0))
#endif
#endif
			{
				/* start if ENABLE_DEBUG_HW_FORCEREADFAIL */
#if GEN_FAIL_PER_MILLE>0
				if (getIntRandValue(1000) >= (1000 - GEN_FAIL_PER_MILLE)) {
#else
				if (getIntRandValue(100) >= 92) {
#endif
					if (getIntRandValue(100) >= ENABLE_DEBUG_HW_FORCEREADFAIL_MT_PERCENTAGE) {
						UBYTE ubLoop = 0;
						UBYTE ubLoopCount = 0;
						ubLoopCount = (getIntRandValue(4) + 1);
						while (ubLoop < ubLoopCount) {
							gFREG[FCTLL_UNC_CFG].L |= FORCE_BCH_UNCORRECTABLE_FRAME(1 << (getIntRandValue(4)));
							ubLoop++;
						}
					}
					else {
						//UartString("\n\nUNC: ");
						//Uart_Tx_DataHex(ubgFQLinkIndex);
						//UartString("\n");
						gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
					}
				}
			} /* end if ENABLE_DEBUG_HW_FORCEREADFAIL */

			if (ENABLE_BURNER_FORCEGENFAIL && (FQ->btCheckEmpty == 0)) {
				if (getIntRandValue(2)) {
					gFREG[FCTLL_UNC_CFG].L |= getIntRandValue(0x11);	//(rand() & 0x10)
				}
				else {
					gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
				}
			}
#pragma flush
			if (ENABLE_DEBUG_RW) {
				while (1) {
					if (ubCEIndex < 8) {
						ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
					}
					else {
						ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
					}
					if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
						M_AssertError(1);
					}
					if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
						break;
					}
				}
			}
			FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | REUSEMT_STOP | QUEUE_INDEX(ubCEIndex));
			//FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) |REUSEMT_AUTO_TRIG |QUEUE_INDEX(ubCEIndex)); //Reuse MT auto trig

#if TLC
			VT->guoTotalNandReadSectorCnt += (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum * SectorsPer4K);
#else
			VT->guoTotalNandReadSectorCnt += gubSectorsPerPlane;
#endif
		}
	}
	FQ->ubFQPhase = ubPhase;
	if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
		FQ->ubMTDepthCnt++;
		gubMTDepthPushNumber[ubCEIndex]++;
	}

	return 0;
}
void flaGetUnitReadCheckInfo(UBYTE ubgFQLinkIndex, UBYTE ubMaxPlaneOffset, UBYTE ubA2Mode) {
#if ((!BURNER) && ENABLE_READ_CHECK_FUNCTION)
	if (gubBusyNow) {
		flaGetUnitReadCheckInfo_Rule(ubgFQLinkIndex, ubMaxPlaneOffset, ubA2Mode);
	}
#endif//((!BURNER) && ENABLE_READ_CHECK_FUNCTION)
	return;
}
#if TLC
#if ((!BURNER) && ENABLE_READ_CHECK_FUNCTION &&0)
void flaGetUnitReadCheckInfo_DynamicD1SLC(UWORD uwFUnit, U32 ulFEntry, U32 ulCurrentPlaneIndex) {

	if (guw4kEntrysPerPage <= (ulFEntry % gulFastPage4kEntrysPerUnit)) {//所指向SLC的位置大於Page0,可往前讀

		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry - guw4kEntrysPerPage/*跳到上個WL*/) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog );
	}
	else { //所指向SLC的位置等於Page0,只能往後讀 ,但是要判斷後面能不能讀取
		if (((ulFEntry + guw4kEntrysPerPage) >> gub4kEntrysPerPlaneLog) >= ulCurrentPlaneIndex) {
			guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry + guw4kEntrysPerPage/*跳到下個WL*/) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
		}
		else {
			guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry + 0/*讀自己*/) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
		}
	}

	return;
}
void flaGetUnitReadCheckInfo_DynamicD3SLC(UWORD uwFUnit, U32 ulFEntry, U32 ulCurrentPlaneIndex) {
	if (guw4kEntrysPerPage <= (ulFEntry % gulFastPage4kEntrysPerUnit)) {//所指向SLC的位置大於Page0,可往前讀

		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry - guw4kEntrysPerPage/*跳到上個WL*/) % gulFastPage4kEntrysPerUnit)/* - (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog );
	}
	else { //所指向SLC的位置等於Page0,只能往後讀 ,但是要判斷後面能不能讀取
		if (((ulFEntry + guw4kEntrysPerPage) >> gub4kEntrysPerPlaneLog) >= ulCurrentPlaneIndex) {
			guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry + guw4kEntrysPerPage/*跳到下個WL*/) % gulFastPage4kEntrysPerUnit)/* - (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
		}
		else {
			guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry + 0/*讀自己*/) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
		}
	}

	return;
}
void flaGetUnitReadCheckInfo_StaticD1SLC(UWORD uwFUnit, U32 ulFEntry) {
	if (guw4kEntrysPerPage <= (ulFEntry % gulFastPage4kEntrysPerUnit)) {//所指向SLC的位置大於Page0,可往前讀

		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry - guw4kEntrysPerPage/*跳到上個WL*/) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog );
	}
	else { //所指向SLC的位置等於Page0,只能往後讀
		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry + guw4kEntrysPerPage/*跳到下個WL*/) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
	}

	return;
}
void flaGetUnitReadCheckInfo_StaticD3SLC(UWORD uwFUnit, U32 ulFEntry) {
	if (guw4kEntrysPerPage <= (ulFEntry % gulFastPage4kEntrysPerUnit)) {//所指向SLC的位置大於Page0,可往前讀

		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry - guw4kEntrysPerPage/*跳到上個WL*/) % gulFastPage4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog );
	}
	else { //所指向SLC的位置等於Page0,只能往後讀
		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry + guw4kEntrysPerPage/*跳到下個WL*/) % gulFastPage4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
	}

	return;
}

void flaGetUnitReadCheckInfo_StaticD3TLC(UWORD uwFUnit, U32 ulFEntry) {

	UBYTE ubD3LMU = 0;
	ubD3LMU = ( ( (ulFEntry >> gub4kEntrysPerPageLog) % 3 ) + 1 );
	if ((guw4kEntrysPerPage * ((ubD3LMU - 1/*跳到Low*/) + 3/*跳到上個WL*/)) <= ulFEntry) {//Previous WL L Page
		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry - (guw4kEntrysPerPage * ((ubD3LMU - 1/*跳到Low*/) + 3/*跳到上個WL*/))) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
	}
	else {//Next WL L Page
		guwUnitReadCheckInfo[uwFUnit].B.uwPlaneIndex = ((((ulFEntry + (guw4kEntrysPerPage * ( 3/*跳到下個WL*/ - (ubD3LMU - 1/*跳到Low*/)))) % gul4kEntrysPerUnit) /*- (ulFEntry & gub4kEntrysPerPlaneMask)alignment 4kEntrysPerPlane*/) >> gub4kEntrysPerPlaneLog);
	}

	return;
}
void flaGetUnitReadCheckInfo_TLCRule(UBYTE ubgFQLinkIndex, UBYTE ubMaxPlaneOffset) {
	UBYTE ubPhyPlaneOffset = 0;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	U32 ulFEntry = 0;
	UBYTE ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
	if ((gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) && ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) && (!VT->gulFTLState.B.btDoingFlushD1) && (FQ->uwFUnit < 4096)) {
		for (ubPhyPlaneOffset = 0; ubPhyPlaneOffset < ubMaxPlaneOffset; ubPhyPlaneOffset++) {
			//ulFEntry = (FQ->ulFEntry + (ubPhyPlaneOffset * gub4kEntrysPerPlane));
			ulFEntry = (((FQ->ulFEntry / gub4kEntrysPerPlane) * gub4kEntrysPerPlane) + (U32)(ubPhyPlaneOffset * gub4kEntrysPerPlane)) % gul4kEntrysPerUnit; //(H)
			guwUnitReadCnt[FQ->uwFUnit]++;
#if Read_Chk_ByHostRead
			if ( (ubD1Unit != Default_D1_Unit)  || (gulVC[FQ->uwFUnit].B.btFastPage) ) {
				VT->guoD1UnitTotalReadCnt++;
			}
			else {
				VT->guoD3UnitTotalReadCnt++;
			}
#endif
			if (	guwUnitReadCnt[FQ->uwFUnit] >= ReadDisturb_Base) {
				guwUnitReadCnt[FQ->uwFUnit] = 0;
				if (gulVC[FQ->uwFUnit].B.ulReadCount < gubUnitForceReadMoveThreshold) {
					gulVC[FQ->uwFUnit].B.ulReadCount++;
				}
			}
#if Read_Chk_ByHostRead
			if ((gubUnitReadCheckThreshold != 0xFF) &&  ( (VT->guoD3UnitTotalReadCnt / gulHostReadTH) || (VT->guoD1UnitTotalReadCnt / gulHostReadTH) )) //Read disturb switch
#else
			if ((gubUnitReadCheckThreshold != 0xFF) && (gulVC[FQ->uwFUnit].B.ulReadCount / gubUnitReadCheckThreshold))
#endif
			{
				//有設定 且 超過Threshold

#if Read_Chk_ByHostRead // gubUnitForceReadMoveThreshold looks as gubReadCheckPageCnt
				if ( (((VT->guoD3UnitTotalReadCnt % gulHostReadTH) == 0) && (VT->guoD3UnitTotalReadCnt > 0)) ||
				        (((VT->guoD1UnitTotalReadCnt % gulHostReadTH) == 0) && (VT->guoD1UnitTotalReadCnt > 0))  ) {
					if (guwUnitReadCheckInfo[FQ->uwFUnit].B.uwPlaneIndex == 0xFFFF) {
						guwReadCheckCnt++;
					}
					else {

					}
				}
				else {
					continue;
				}
#else
				if (((gulVC[FQ->uwFUnit].B.ulReadCount % gubUnitReadCheckThreshold) == 0)// 只看Match Threshold的
				        || (gulVC[FQ->uwFUnit].B.ulReadCount >= gubUnitForceReadMoveThreshold)) {
					//已經達到要Force Read Move的階段
					if (guwUnitReadCheckInfo[FQ->uwFUnit].B.uwPlaneIndex == 0xFFFF) {
						guwReadCheckCnt++;
					}
					else {

					}
				}
				else {
					continue;
				}
#endif
				if (ubD1Unit != Default_D1_Unit) {
					//D1 SLC
					if (FQ->uwFUnit == VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget) {
						if ((ulFEntry / gulFastPage4kEntrysPerUnit) < (VT->gulGRTargetPTR / guwFastPagePlanesPerUnit)) { //只有Activ GR會沒寫滿所以要檢查
							flaGetUnitReadCheckInfo_StaticD1SLC(FQ->uwFUnit, ulFEntry);//所指向的SLC為寫滿的SLC
						}
						else {
							flaGetUnitReadCheckInfo_DynamicD1SLC(FQ->uwFUnit, ulFEntry, VT->gulGRTargetPTR);//所指向的SLC為還沒寫滿的SLC
						}
					}
					else {
						//D3 TLC 一定寫滿
						flaGetUnitReadCheckInfo_StaticD1SLC(FQ->uwFUnit, ulFEntry);	//只有Activ GR會沒寫滿, 可以往後面讀
					}
				}
				else { //D3 TLC or D3 SLC
					if (gulVC[FQ->uwFUnit].B.btFastPage) { //D3 SLC ,只有1個SLC組成, 可能是Active GR or Active Table
						if ((FQ->uwFUnit == VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget) || (FQ->uwFUnit == VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget)) {
							//所指向的SLC為還沒寫滿的SLC
							flaGetUnitReadCheckInfo_DynamicD3SLC(FQ->uwFUnit, ulFEntry, ((FQ->uwFUnit == VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget) ? VT->gulGRTargetPTR : VT->guwTableTargetPTR));
						}
						else { //不是Active GR or Active Table, 保證可以往前或往後讀
							flaGetUnitReadCheckInfo_StaticD3SLC(FQ->uwFUnit, ulFEntry);//所指向的SLC為寫滿的SLC
						}
					}
					else { //D3 TLC 一定寫滿
						flaGetUnitReadCheckInfo_StaticD3TLC(FQ->uwFUnit, ulFEntry);
					}
				}
				if (ENABLE_READ_CHECK_FUNCTION_UART) {
					UartString("\nI");
					UartWordHex(FQ->uwFUnit);
					UartString(" f");
					Uart_Tx_DataHex(gulVC[FQ->uwFUnit].B.btFastPage);
					UartString(" P");
					UartWordHex(guwUnitReadCheckInfo[FQ->uwFUnit].B.uwPlaneIndex);
				}
			}
		}
	}



	return;
}
#endif
UWORD flaFourPlaneD3WriteChangFPU(UBYTE ubDMA_Mode, UBYTE ubD3WriteMode, UBYTE ubPlane ) {
#if (Hynix3DV6 || YMTC_JGS)
	return guwFourPlaneD3WriteFPUArray[ubDMA_Mode][ubPlane][flaCheckLogValue(((ubD3WriteMode & D3_WRITE_LMU_MASK) >> D3_WRITE_LMU_OFFSET))];
#else
	flaLEDBlink(1);
	return 0;
#endif
}

UWORD flaTwoPlaneD3WriteChangFPU(UBYTE ubDMA_Mode, UBYTE ubD3WriteMode, UBYTE ubPlane ) {
#if (Hynix3DV6 || YMTC_JGS)
	return guwTwoPlaneD3WriteFPUArray[ubDMA_Mode][ubPlane][flaCheckLogValue(((ubD3WriteMode & D3_WRITE_LMU_MASK) >> D3_WRITE_LMU_OFFSET))];
#else
	return guwTwoPlaneD3WriteFPUArray[ubDMA_Mode][flaCheckLogValue(((ubD3WriteMode & D3_WRITE_FFF_MASK) >> D3_WRITE_FFF_OFFSET))][ubPlane][flaCheckLogValue(((ubD3WriteMode & D3_WRITE_LMU_MASK) >> D3_WRITE_LMU_OFFSET))];
#endif
}
UWORD flaOnePlaneD3WriteChangFPU(UBYTE ubD3WriteMode) {

#if (Hynix3DV6 || YMTC_JGS)
	return guwOnePlaneD3WriteFPUArray[flaCheckLogValue(((ubD3WriteMode & D3_WRITE_LMU_MASK) >> D3_WRITE_LMU_OFFSET))];
#else
	return guwOnePlaneD3WriteFPUArray[flaCheckLogValue(((ubD3WriteMode & D3_WRITE_FFF_MASK) >> D3_WRITE_FFF_OFFSET))][flaCheckLogValue(((ubD3WriteMode & D3_WRITE_LMU_MASK) >> D3_WRITE_LMU_OFFSET))];
#endif
}
#endif
#if ((!BURNER) && ENABLE_READ_CHECK_FUNCTION)
void flaGetUnitReadCheckInfo_Rule(UBYTE ubgFQLinkIndex, UBYTE ubMaxPlaneOffset, UBYTE ubA2Mode) {

	UBYTE ubPhyPlaneOffset = 0;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	U32 ulFEntry = 0;
	if ((gubUnitReadCheckThreshold != 0xFF) && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) && ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) && (FQ->uwFUnit < 4096)) {
		for (ubPhyPlaneOffset = 0; ubPhyPlaneOffset < ubMaxPlaneOffset; ubPhyPlaneOffset++) {
			ulFEntry = (FQ->ulFEntry + (ubPhyPlaneOffset * gub4kEntrysPerPlane));

			if (flaCheckFQSeq(ubgFQLinkIndex, ulFEntry) == 1) { //no Seq
				guwUnitReadCnt[FQ->uwFUnit]++;
				UWORD uwTempForceReadMoveThreshold_1kBase = flaGetForceReadThreshold(ubA2Mode, FQ->uwFUnit);
				UBYTE ubTempReadCount_4kBase = gulVC[FQ->uwFUnit].B.ulReadCount; // 現在是4K Base
				UWORD uwTempReadCount_1kBase = (UWORD) ubTempReadCount_4kBase * 4;
				if (ENABLE_FORCE_TABLE_READ_CNT && FQ->FQPara.B.btRWTableUnit) {
					uwTempForceReadMoveThreshold_1kBase = 10;
				}
				if (uwTempReadCount_1kBase < uwTempForceReadMoveThreshold_1kBase) { // 還在ForceReadMove range裡面
					if (guwUnitRRCnt[FQ->uwFUnit] > 0) { // 如果已經開始檢查, 又read 到同一個unit, 就會繼續檢查 //原本這段是插在flaNeedCheck 會return 1 就會舉 PlaneIndex , 但我不想要flaNeedCheck進入太多次
						if (guwUnitReadCheckInfo[FQ->uwFUnit].B.uwPlaneIndex == 0xFFFF) {
							guwUnitReadCheckInfo[FQ->uwFUnit].B.uwPlaneIndex = 0x01;
							guwReadCheckCnt++;
						}
					}
					if ((guwUnitReadCnt[FQ->uwFUnit] % ReadDisturb_Base) == 0) { // 每 增加 ReadDisturb_Base 次 增加gulVC中的read count 1
						guwUnitReadCnt[FQ->uwFUnit] = 0;
						ubTempReadCount_4kBase += 1;  // 現在是4K Base
						uwTempReadCount_1kBase += 4;
						gulVC[FQ->uwFUnit].B.ulReadCount = ubTempReadCount_4kBase;
						if (gulVC[FQ->uwFUnit].B.ulReadCount > gulMaxReadCount) {
							gulMaxReadCount = gulVC[FQ->uwFUnit].B.ulReadCount;
						}
						if (uwTempReadCount_1kBase >= uwTempForceReadMoveThreshold_1kBase) { //確認此Unit是否已經在ForceRead的Queue裡面
							if (ftlCheckForceReadFIFO(FQ->uwFUnit)) {
								ftlPushForceReadInfo(FQ->uwFUnit);
								continue;
							}
						}
						if (flaNeedReadChk(FQ->uwFUnit, ubgFQLinkIndex, ubA2Mode)) { // 達到 Read check的標準
							if (guwUnitReadCheckInfo[FQ->uwFUnit].B.uwPlaneIndex == 0xFFFF) {	// 充當flag 的功能	 ( 其實用一個byte就可以搞定了 浪費空間
								guwUnitReadCheckInfo[FQ->uwFUnit].B.uwPlaneIndex = 0x01;
								guwReadCheckCnt++;
							}
						}
					}
				}
			}
		}
	}
	return;
}
UBYTE flaNeedReadChk(UWORD uwFUnit, UBYTE ubgFQLinkIndex, UBYTE ubA2Mode) {
	UBYTE ubLimit, ubNeed;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	ubLimit = ubNeed = 0;

	U32 ulTempRC = (gulVC[uwFUnit].B.ulReadCount * ReadDisturb_Base) + (guwUnitReadCnt[uwFUnit] % ReadDisturb_Base);
#if TLC
	U32 ulTempEC = (gubD1LinkTable[uwFUnit].B.ubD1Link == Default_D1_Unit) ? gulEC_D3[uwFUnit].B.ulEraseCount : gulEC_D1[gubD1LinkTable[uwFUnit].B.ubD1Link].B.ulEraseCount;
#else
	U32 ulTempEC = gulEC_D3[uwFUnit].B.ulEraseCount;
#endif

	if (RDCHECKF * gubPlanesPerPage > 10000) {
		ubLimit = 1;
	}
	if (!(gulVC[uwFUnit].B.ulReadCount)) {
		return 0;
	}
	if (ENABLE_FORCE_TABLE_READ_CNT && FQ->FQPara.B.btRWTableUnit) {
		if ((ubLimit) || ((ulTempRC % (4000 - RDCHECKF * gubPlanesPerPage)) == 0)) { //4k case
			ubNeed = 1;
		}
	}
	if (ubA2Mode) { // A2Mode

		if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshiba1Znm && (!TLC)) { // SAN MLC
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(15 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 25000) {
				if ((ulTempRC % ((U32)(20 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 20000) {
				if ((ulTempRC % ((U32)(30 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 15000) {
				if ((ulTempRC % ((U32)(50 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 10000) {
				if ((ulTempRC % ((U32)(75 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 5000) {
				if ((ulTempRC % ((U32)(200 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(250 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshibaBiCs3) { //San Bics3
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(100 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 20000) {
				if ((ulTempRC % ((U32)(300 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 10000) {
				if ((ulTempRC % ((U32)(400 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(500 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshibaBiCs4) { //San Bics4
			if ((ubLimit) || ((ulTempRC % ((U32)(200 * ReadCheck_Base))) == 0) ) {
				ubNeed = 1;
			}
		}
		else if (guoFlashUseType.B.btToshiba1Znm && !TLC) { //15 nm MLC
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(15 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 25000) {
				if ((ulTempRC % ((U32)(25 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 15000) {
				if ((ulTempRC % ((U32)(45 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(100 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshiba1Znm && TLC) { //15nm TLC
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(15 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 25000) {
				if ((ulTempRC % ((U32)(20 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 20000) {
				if ((ulTempRC % ((U32)(25 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 15000) {
				if ((ulTempRC % ((U32)(40 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(70 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs2) { //Bics2
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(75 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 25000) {
				if ((ulTempRC % ((U32)(150 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 20000) {
				if ((ulTempRC % ((U32)(300 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(500 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs3) { //Bics3
			if (ulTempEC > 90000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(60 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 70000) {
				if ((ulTempRC % ((U32)(75 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(100 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btIntelMicronL06B) { //L06B
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(250 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 25000) {
				if ((ulTempRC % ((U32)(300 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 20000) {
				if ((ulTempRC % ((U32)(400 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(500 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if ((guoFlashUseType.B.btToshibaBiCs4) && (!guoFlashUseType.B.btSandisk) && ((gubDensitysize == FLH_Size_512Gb) || (gRRTMode.B.btRevision))) { // Bics4
			if ((ubLimit) || ((ulTempRC % ((U32)(1000 * ReadCheck_Base))) == 0) ) {
				ubNeed = 1;
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs4) { //Bics4
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(150 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 20000) {
				if ((ulTempRC % ((U32)(300 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 10000) {
				if ((ulTempRC % ((U32)(400 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(500 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btHynix3DV6) {
			// NAND Team 只做實驗到 EOL 55k 內 RC 1.5kk沒事 , 不特別卡EC
			if ((ubLimit) || ((ulTempRC % ((U32)(1500 * ReadCheck_Base))) == 0) ) {
				ubNeed = 1;
			}
		}
		else if (guoFlashUseType.B.btYMTCJGS) {
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(400 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 10000) {
				if ((ulTempRC % ((U32)(500 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(1000 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else { //default
			if (ulTempEC > 30000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(15 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 25000) {
				if ((ulTempRC % ((U32)(25 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 15000) {
				if ((ulTempRC % ((U32)(45 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(100 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
	}
	else { // non  A2 Mode

		if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshiba1Znm && (!TLC)) { // SAN MLC
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(15 * ReadCheck_Base / 2 ))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2500) {
				if ((ulTempRC % ((U32)(10 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(15 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1500) {
				if ((ulTempRC % ((U32)(20 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(35 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 500) {
				if ((ulTempRC % ((U32)(65 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(75 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshibaBiCs3) { //San Bics3
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(5 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2500) {
				if ((ulTempRC % ((U32)(10 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(15 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(20 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(45 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs3) { //Bics3
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH5 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2500) {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH4 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH3 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH2 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH1 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if ((guoFlashUseType.B.btToshibaBiCs4) && (!guoFlashUseType.B.btSandisk) && ((gubDensitysize == FLH_Size_512Gb) || (gRRTMode.B.btRevision))) { // Bics4
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(300 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(400 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(500 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(1000 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs4) { //Bics4
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(20 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2500) {
				if ((ulTempRC % ((U32)(30 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(40 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(50 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(75 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshiba1Znm && !TLC) { // 15 nm MLC
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)( 15 * ReadCheck_Base / 2))) == 0) ) {
					ubNeed = 1;
				}

			}
			else if (ulTempEC > 2500) {
				if ((ulTempRC % ((U32)(10 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(15 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1500) {
				if ((ulTempRC % ((U32)(20 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(35 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 500) {
				if ((ulTempRC % ((U32)(65 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(75 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshiba1Znm && TLC) { //15nm TLC
			if (ulTempEC > 1000) {
				if ((ubLimit) || ((ulTempRC % ((U32)( 3 * ReadCheck_Base / 2))) == 0) ) {
					ubNeed = 1;
				}

			}
			else if (ulTempEC > 500) {
				if ((ulTempRC % ((U32)(5 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(25 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs2) { // Bics2
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)( 5 * ReadCheck_Base / 2))) == 0) ) {
					ubNeed = 1;
				}

			}
			else if (ulTempEC > 1500) {
				if ((ulTempRC % ((U32)(5 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(15 * ReadCheck_Base / 2))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 500) {
				if ((ulTempRC % ((U32)(25 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(35 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btIntelMicronL06B) { //L06B
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(25 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1500) {
				if ((ulTempRC % ((U32)(50 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(75 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btHynix3DV6) {
			if (ulTempEC > 3000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(30 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(50 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(100 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(300 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
		}
		else if (guoFlashUseType.B.btYMTCJGS) {
			if (ulTempEC > 1000) {
				if ((ubLimit) || ((ulTempRC % ((U32)(300 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 500) {
				if ((ulTempRC % ((U32)(350 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(450 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}

		}
		else { //default
			if (ulTempEC > 2500) {
				if ((ubLimit) || ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH5 * ReadCheck_Base))) == 0) ) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 2000) {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH4 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1500) {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH3 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else if (ulTempEC > 1000) {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH2 * ReadCheck_Base))) == 0) {
					ubNeed = 1;
				}
			}
			else {
				if ((ulTempRC % ((U32)(ReadDisturb_CheckECC_TH1 * ReadCheck_Base))) == 0) { //現在用mod去判斷
					ubNeed = 1;
				}
			}
		}
	}

	return ubNeed;

}
UBYTE flaCheckFQSeq(UBYTE ubgFQLinkIndex, U32 ulCurrentFEntry) {

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];

	UBYTE ubLastBurstBank = (gubBurstsPerBank - 1);
	UWORD uwPreviousUnit = (gFQI.ulPreviousEntry[FQ->ubCEIndex] / gul4kEntrysPerUnit);
	U32 ulPreviousEntry = (gFQI.ulPreviousEntry[FQ->ubCEIndex] % gul4kEntrysPerUnit);
	UBYTE ubResult = 1;//no Seq

	if (uwPreviousUnit == FQ->uwFUnit) { //uwPreviousUnit 等於 CurrentUnit
		//只要是Previous 4K 後的都當做 Seq
		U32 ulPreviousLast4KEntry = (ulPreviousEntry | gub4kEntrysPerPlaneMask);
		if (ulPreviousLast4KEntry < ulCurrentFEntry) {
			ubResult = 0;//Seq
		}
		/*
		if (ulPreviousEntry < ulCurrentFEntry) { //ulPreviousEntry小於 CurrentEntry
			U32 ul4kEntrysDiff = (ulCurrentFEntry - ulPreviousEntry);
			UBYTE ub4kEntrysDiffForContinuousPlane;	// for pA接pB, pB接pC, pC接pD (同page不同plane)
			UWORD uw4kEntrysDiffForContinuousPage; // only for pD (page N) 接pA (page N+1)
			UBYTE ubBurstBank;
			UBYTE ubPreviousBurstBank;

			if (ENABLE_PLANE_CH_CE) {
				ubBurstBank = (ulCurrentFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
				ubPreviousBurstBank = (ulPreviousEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
			}
			else {
				ubBurstBank = (ulCurrentFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
				ubPreviousBurstBank = (ulPreviousEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
			}
			if (ENABLE_PLANE_CH_CE) {
				ub4kEntrysDiffForContinuousPlane = gub4kEntrysPerPlane;
				uw4kEntrysDiffForContinuousPage = guw4kEntrysPerPage - gub4kEntrysPerPlane * ubPreviousBurstBank;
			}
			else {
				ub4kEntrysDiffForContinuousPlane = gub4kEntrysPerBurst;
				uw4kEntrysDiffForContinuousPage = guw4kEntrysPerPage - gub4kEntrysPerBurst * ubPreviousBurstBank;
			}
			if (0
			        || (ul4kEntrysDiff == ub4kEntrysDiffForContinuousPlane)
			        || ((ubPreviousBurstBank == ubLastBurstBank) && (ul4kEntrysDiff == uw4kEntrysDiffForContinuousPage)) ) {
				ubResult =  0;//Seq
			}
		}
		*/
	}

	if (ENABLE_SEQ_READ_CHECK_KAO_UART && (FQ->ubCEIndex == 1) && (ubResult == 1)) {
		UartString("\nNo seq:");
		Uart_Tx_DataHex(FQ->ubCEIndex);
		if (FQ->FQPara.B.btRWTableUnit == 1) {
			UartString(" Table");
		}
		UartString("\n");
		UartWordHex(uwPreviousUnit);
		UartString(" ");
		UartLongHex(ulPreviousEntry);
		UartString("\n");
		UartWordHex(FQ->uwFUnit);
		UartString(" ");
		UartLongHex(ulCurrentFEntry);
	}

	if (0) {
		U32 ulCurrentPlaneIndex = 0xFFFFFFFF;
		UBYTE ubCurrentBurstBank = 0xFF;
		U32 ulCurrentPage = 0xFFFFFFFF;
		U32 ulNextSeqPlaneIndex = 0xFFFFFFFF;
		UBYTE ubNextSeqPlaneOffset = 0xFF;

		while (1) {
			ulCurrentPlaneIndex = (ulCurrentFEntry >> gub4kEntrysPerPlaneLog);
			ubCurrentBurstBank = (ulCurrentPlaneIndex & gubBurstsPerBankMask);
			ulCurrentPage = (ulCurrentPlaneIndex / gubPlanesPerPage);
			ulNextSeqPlaneIndex = 0xFFFFFFFF;
			ubNextSeqPlaneOffset = 0xFF;

			if (ubCurrentBurstBank == ubLastBurstBank) {

				if (((gulVC[FQ->uwFUnit].B.btFastPage) && (ulCurrentPage != (guwFastPagePagesPerUnit - 1)))
				        || (ulCurrentPage != (guwPagesPerUnit - 1)) ) { //不是最後一個Page
					ubNextSeqPlaneOffset = gubPlanesPerPageMask;
				}
				else {
					break;
				}
			}
			else {
				ubNextSeqPlaneOffset = 1;
			}
			ulNextSeqPlaneIndex = (ulCurrentPlaneIndex + ubNextSeqPlaneOffset);
			if (FQ->FQPara.B.btRWTableUnit == 0) { //避掉Data RS

				Generate_RS_MAP_Index_by_Plane(ulNextSeqPlaneIndex);

				if ((gulVC[FQ->uwFUnit].B.btFastPage &&
				        (((RS.ulLastDataPlaneIndexFastPage[RS.ubGroupIndex] + 1) == ulNextSeqPlaneIndex)
				         || ((RS.ulLastDataPlaneIndexFastPage[RS.ubGroupIndex] + 2) == ulNextSeqPlaneIndex)))
				        || ((gulVC[FQ->uwFUnit].B.btFastPage == 0) &&
				            (((RS.ulLastDataPlaneIndex[RS.ubGroupIndex] + 1) == ulNextSeqPlaneIndex)
				             || ((RS.ulLastDataPlaneIndex[RS.ubGroupIndex] + 2) == ulNextSeqPlaneIndex)))
				        || (RS.uwPlaneOffset >= (guwParityPlanesNum - 3))) {

					//Physical Seq 是RS
				}
				else {
					break;
				}
			}
			else if (FQ->FQPara.B.btRWTableUnit == 1) { //避掉Table RS
				if ((ulNextSeqPlaneIndex % guwTableParityPlanesNum) == guwTableParityPlanesNumMax) {
					//Physical Seq 是RS
				}
				else {
					break;
				}
			}
			else {
				break;
			}

			ulCurrentFEntry += (ubNextSeqPlaneOffset << gub4kEntrysPerPlaneLog);//Physical Seq 是RS
		}


	}
	gFQI.ulPreviousEntry[FQ->ubCEIndex] = ((FQ->uwFUnit * gul4kEntrysPerUnit) + ulCurrentFEntry);

	return ubResult;

}
#endif

UBYTE flaSramToFlash_CH(UBYTE ubPhase, UBYTE ubgFQLinkIndex, UWORD uwFPU, UBYTE ubAutoPoll, UBYTE ubPolSeqSel) {
#if(!RDT)
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubDepth = FQ->ubRelativeDepth;
	UBYTE ubBurstBank, ubPhyPlane;
	U32 ulTrigReg;

	UBYTE ubChannel;
	UBYTE ubRemainQCnt;
	UBYTE ubRemainQCnt2;

	gulPara0x04.BitMap.btRSOneParityEn = 0;
	if (ENABLE_DEBUG_RW) {
		ubRemainQCnt = 0;
		ubRemainQCnt2 = 0;
		ubChannel = ubCEIndex & gubPlanesPerBurstMask;
		gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
	}

	M_AssertError(FQ->ubFQPhase);
	M_AssertError(FQ->ubFJob != BYTE_FJOB_WRITE);
	M_AssertError(FQ->btNeedStopDMA);
	if (ENABLE_DEBUG_UART_RS) {
		UartString("\ntag:  ");
		Uart_Tx_DataHex(FQ->ubRSFrameIndex);
		UartString("  PTR:  ");
		UartWordHex(FQ->ulFEntry / gub4kEntrysPerPlane);
	}
	M_VirtualToPhysicalCE(ubCEIndex);

	if (FQ->uwFJobInfo & BIT_FJOBI_PARITY) {
		if (ENABLE_DEBUG_UART_RS) {
			UartString("  PageNum:  ");
			Uart_Tx_DataHex(FQ->ubRSPageNum);
		}
		ulTrigReg = (MT_INDEX(ubDepth)) | REUSEMT_STOP | (QUEUE_INDEX(ubCEIndex)) | (RS_TAG_MT(FQ->ubRSFrameIndex)) | (RS_OTFENC_EN);
		gulPara0x04.BitMap.btRSEncodeMode = FQ->btRSOneParityEn;
		gulPara0x04.BitMap.ubRSPageNum = FQ->ubRSPageNum ;
		gulPara0x04.BitMap.btRSProg2ndParity = FQ->btRSProg2ndParity;
		gulPara0x04.BitMap.btRSLastPage = FQ->btRSLastPage;
	}
	else if ( (ENABLE_DEBUG_NOT_ENCODE_PARITY == 0) && (FQ->btParity)) {
		ulTrigReg = (MT_INDEX(ubDepth)) | REUSEMT_STOP | (QUEUE_INDEX(ubCEIndex)) | (RS_TAG_MT(FQ->ubRSFrameIndex)) | (RS_PROG_PARITY);
		gulPara0x04.BitMap.btRSEncodeMode = FQ->btRSOneParityEn;
		gulPara0x04.BitMap.ubRSPageNum = FQ->ubRSPageNum ;
		gulPara0x04.BitMap.btRSProg2ndParity = FQ->btRSProg2ndParity;
		gulPara0x04.BitMap.btRSLastPage = FQ->btRSLastPage;
	}
	else {
		if ((ENABLE_DEBUG_RW) && (FQ->btParity)) {
			if (ENABLE_DEBUG_UART_RS) {
				UartString("  Parity");
			}
			guwCHDebug[FQ->ubRSFrameIndex] = FQ->ulFEntry;
		}
		ulTrigReg = (MT_INDEX(ubDepth)) | REUSEMT_STOP | (QUEUE_INDEX(ubCEIndex));
	}
	// 之後回收統一用ubDepth, ex: 2 plane cache write, 80 Addr DMA              / 11 81 Addr DMA 15 / CheckWrite
	//                                                                      MT1 Depth(=Relateive)   MT2 Relative            MT2 Depth
	FQ->ubDepth = FQ->ubRelativeDepth;

	M_AssertError(FQ->FQPara.B.btCanRemove);
	FQ->FQPara.B.btCanRemove = 0;	//  可以等到NextWriteFQ, 再設為1

	if (ENABLE_iFSA) {
		U32 ulFEntry = FQ->ulFEntry;
		UWORD uwFBlock;
		UBYTE ubD1Unit = 0;

#if TLC
		if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
			ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
			if ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
				ulFEntry = FQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
			}
		}
#endif

		if (ENABLE_PLANE_CH_CE) {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
		}
		else {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
		}

		if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
			uwFBlock = FQ->uwFUnit;
			ubPhyPlane = ubBurstBank;
		}
		else {
			if ((TLC == 0) || (FQ->btD3 || (ubD1Unit == Default_D1_Unit))) {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
			}
			else {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
			}
			if (gubRUTMixPlaneEnable == 0) {
				ubPhyPlane = ubBurstBank;
			}
			else {
				ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
				uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
			}
		}

		if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
			if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
				uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
			}
		}

		M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				FQ->ubALUIndex = ALU_A2;
				goto Mark_NormalSectorAddress;
			}
			else {
				FQ->ubALUIndex = ALU_NORMAL;
#if TLC
				if (TLC) {
#if (B0KB || YMTC_JGS)
					guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				}
				else {
#else
				{
#endif
					guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
				}
			}
		}
		else {
			FQ->ubALUIndex = ALU_NORMAL;
#if TLC
			if (TLC) {
#if (B0KB || YMTC_JGS)
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
			}
			else {
#else
			{
#endif
			Mark_NormalSectorAddress:
#if (B0KB || YMTC_JGS)
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
			}
		}
		guliFSA[0] |= (uwFBlock & 0xFFFF) << 16;
		M_AssertCriticalError((((uwFBlock >= gulTotalPhyUnitNum) && ((gubDieNumber == 1) && (gubExpandCE == 0))) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog))), VT_ASSERT_SRAMTOFLASH_CH_0x44);	// SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum
		if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
			FQ->ubFSAIndex = 0xFF;
		}
		gulPara0x04.BitMap.btiFSAEn = 1;

		gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = guliFSA[0];

#if ENABLE_RUNTIME_SEEDINIT
		// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
		gMTQ->Depth[ubDepth].ulSeedInit = M_RandomSeed((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)), ((guoFlashUseType.B.btHynix3DV6) ? 512 : 384));
#elif TSB_BICS4_SUPPORT
		gMTQ->Depth[ubDepth].ulSeedInit = flaGenRandSeed(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#else
		if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 512) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0x0;
		}
		else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1024) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0xffffff;
		}
#if TLC_BICS2
#else /* TLC_BICS2 */
		else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1536) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0x555555;
		}
#if B0KB
#else
		else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2048) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0xAAAAAA;
		}
		else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2560) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0xCCCCCC;
		}
		else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3072) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0x333333;
		}
		else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3584) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0x999999;
		}
		else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 4096) {
			gMTQ->Depth[ubDepth].ulSeedInit = 0x666666;
		}
#endif
#endif /* TLC_BICS2 */
#endif

		// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#endif /* ENABLE_RUNTIME_SEEDINIT */
	}
	else {
		gulPara0x10.BitMap.ubFSASel = FQ->ubFSAIndex;
		gMTQ->Depth[ubDepth].Para0x10.ulAll = gulPara0x10.ulAll;
	}

	// 上面算完iFSA後才有FQ->ubALUIndex
#if MicronFlashOnly
	if ( (FQ->ubALUIndex == ALU_A2) && guoFlashUseType.B.btToshiba)
#else
	if (FQ->ubALUIndex == ALU_A2)
#endif
	{
		if (uwFPU == FPU_PTR_AG_C80_A5_DW_C10) {
#if Hynix
			uwFPU = (gubDaCmdEnable) ? (FPU_PTR_AG_C80_A5_DW_C10) : (FPU_PTR_AG_CA2_C80_A5_DW_C10);
#else
			uwFPU = FPU_PTR_AG_CA2_C80_A5_DW_C10;
#endif
		}
		else if (uwFPU == FPU_PTR_AG_C80_A5_DW_C15) {
#if Hynix
			uwFPU = (gubDaCmdEnable) ? (FPU_PTR_AG_C80_A5_DW_C15) : (FPU_PTR_AG_CA2_C80_A5_DW_C15);
#else
			uwFPU = FPU_PTR_AG_CA2_C80_A5_DW_C15;
#endif
		}
		else if (uwFPU == FPU_PTR_AG_C80_A5_DW) {
#if Hynix
			uwFPU =  (gubDaCmdEnable) ? (FPU_PTR_AG_C80_A5_DW) : (FPU_PTR_AG_CA2_C80_A5_DW);
#else
			uwFPU = FPU_PTR_AG_CA2_C80_A5_DW;
#endif
		}
	}

#if ( TLC && (!B0KB) && (!Hynix3DV6) && (!Hynix3DV5) && (!YMTC_JGS))

	if ( (uwFPU == FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15) ) {
		uwFPU = RAM_SEQ_PTR16(FPU_AG_C11_C70_POL_MK40_C80_A5_DW_C15);
	}
	else if ( (uwFPU == FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW) ) {
		uwFPU = RAM_SEQ_PTR16(FPU_AG_C11_C70_POL_MK40_C80_A5_DW);
	}

#endif
	gulPara0x00.BitMap.btSetInternalBusy = ubAutoPoll;
	gulPara0x00.BitMap.btStaNoStop  = 0;
#if FixShallowErase
	if (gubSkipProgramFail) {
		//gulPara0x00.BitMap.btStaNoStop  = 1;
	}
#endif
	gulPara0x00.BitMap.btQueryCheck = 0;
	gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex;
	gulPara0x04.BitMap.btIntVctEn = 1;
	gulPara0x04.BitMap.btUltraDMADisable = 1;
	gulPara0x04.BitMap.btConvBypass = 0;	// read可能有ConvBypass = 1 or 0, 但program always使用Conv
#if TLC
	if (TLC) {
#if TSB_BICS4_SUPPORT || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
		gulPara0x04.BitMap.uwConvLogicPage = (M_RefSeedPage(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
		// using lower page Conversion Seed
		if (FQ->btTLCProgramCodeBlock) {
			gulPara0x04.BitMap.uwConvLogicPage /= 3;
			gulPara0x04.BitMap.uwConvLogicPage *= 3;
		}
#endif

#else
		gulPara0x04.BitMap.uwConvLogicPage = (M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#endif
		gulPara0x04.BitMap.btConvLogicPageEn = 1;
	}
#endif
	gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
		gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
	}
	else {
		gulPara0x1C.BitMap.ubCEValue = ubCEIndex;
	}
	if (ubAutoPoll) {
		gulPara0x1C.BitMap.ubPolSeqSel  = ubPolSeqSel;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
		FQ->btUsingAutoPol = 1;
		FQ->ulAutoPolStartTime = gulOperationTime;
#endif
#if FORCE_AUTOPOL_TIMEOUT
		if ((gubDebugAutoPolFPUSeqInUse == 0) && (gubPreformat == 0) && (VT->gulFTLState.B.btNeedRUTReplace == 0)) {
			gulRandomValue = getIntRandValue(FORCE_AUTOPOL_TIMEOUT_DENOMINATOR);
			if (gulRandomValue < FORCE_AUTOPOL_TIMEOUT_NUMERATOR) {//(uwFPU == FPU_PTR_AG_C80_A5_DW_C10) ? FORCE_AUTOPOL_TIMEOUT_NUMERATOR : 0) {
				gulPara0x1C.BitMap.ubPolSeqSel = WAIT_AUTOPOL_TIMEOUT;
				gubDebugAutoPolFPUSeqInUse = 1;
				gubDebugAutoPolFQIndex = ubgFQLinkIndex;
				gubDebugAutoPolMTDepth = ubDepth;
				if (uwFPU == FPU_PTR_AG_C80_A5_DW_C10) {
					gulPara0x1C.BitMap.uwFPUPtr = FPU_PTR_AG_C80_A5_DW_CFC;	// to simulate data loss
#if UART_AUTOPOL_TIMEOUT_HANDLING
					UartString("\nLoss");
#endif
				}
#if UART_AUTOPOL_TIMEOUT_HANDLING
				UartString("\nPPol Timeout");
#endif
			}
		}
#endif
	}

	gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
	gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
	gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
#if FORCE_AUTOPOL_TIMEOUT
	if (ubAutoPoll) {
		// restore PolSeqSel
		gulPara0x1C.BitMap.ubPolSeqSel = ubPolSeqSel;
	}
#endif

	if (ENABLE_DEBUG_FIP_NO_HANG_BY_GEN_COMPRESSION_ERROR) {
		if (FQ->FQPara.B.btUserData && (getIntRandValue(0x100) == 0) ) {	//(rand() & 0xFF)
			UartString("\nGoE: ");
			Uart_Tx_DataHex(ubgFQLinkIndex);
			volatile L4KTable16B * pL4KTable = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + (gMTQ->Depth[FQ->ubRelativeDepth].Para0x24.BitMap.ubL4KNum - 1)) * Spare4kSize));
			if (pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE & BIT7) {
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0x87;
			}
			else {
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0x07;
			}
		}
	}

#pragma flush
	if (ENABLE_DEBUG_RW) {
		while (1) {
			if (ubCEIndex < 8) {
				ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
			}
			else {
				ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
			}
			if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
				M_AssertError(1);
			}
			if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
				break;
			}
		}
	}

	FCONL[FCONL_MT_CFG] = ulTrigReg;

	FQ->ubFQPhase = ubPhase;
	if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
		FQ->ubMTDepthCnt++;
		gubMTDepthPushNumber[ubCEIndex]++;
	}


#if TLC
	if ( (TLC_BICS2 || Hynix3DV5 || Hynix3DV6 || YMTC_JGS) || (!(FQ->ubD3WriteMode & (D3_WRITE_FOGGY | D3_WRITE_FINE)))) { //不計算 Write D3 的 FOGGY, FINE
		UBYTE ubTureTable = 0;
		ubTureTable |= (FQ->btD3) ? (BIT0 << 2) : 0;
		ubTureTable |= (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? (BIT0 << 1) : 0;
		//ubTureTable |= (gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit) ? (BIT0 << 0) : 0;

		if (FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) {
			ubTureTable |= (FQ->ubFSAIndex != Default_D1_Unit) ? (BIT0 << 0) : 0;
		}
		else {
			ubTureTable |= (gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit) ? (BIT0 << 0) : 0;
		}

		//True Table
		//btD3-FastPage-D1Link
		switch (ubTureTable) {
			case TURE_TABE_B001://D1 Data
				VT->guoTotalNandWritePlaneCnt_D1Data++;
				break;
			case TURE_TABE_B101://D3 TLC Data
				VT->guoTotalNandWritePlaneCnt_D3TLCData++;
				break;
			case TURE_TABE_B110://D3 SLC Data & D3 SLC Table
				if (FQ->FQPara.B.btRWTableUnit) {
					//D3 SLC Table
					VT->guoTotalNandWritePlaneCnt_D3SLCTable++;
				}
				else {
					//D3 SLC Data
					VT->guoTotalNandWritePlaneCnt_D3SLCData++;
				}
				break;
#if BICS2_GC_TO_D3
			case TURE_TABE_B100://D3 GC (TLC & Without D1 Link Unit)
				VT->guoTotalNandWritePlaneCnt_D3TLCData++;
				break;
#endif
			default:
				break;
		}
	}


#else
	if (FQ->FQPara.B.btRWTableUnit) {
		VT->guoTotalNandWriteTable++;
	}
	else {
		VT->guoTotalNandWriteData++;
	}
	VT->guoTotalNandWriteSectorCnt += gubSectorsPerPlane;
#endif

#endif
	return 0;
}

#if TLC
#if (!BURNER)
const __attribute__((section(".ICODE.rodata"))) UWORD garEraseFPU[3][3] =   /* [plane][mode] */
#else
const UWORD garEraseFPU[3][3] =   /* [plane][mode] */
#endif
{
#if Hynix3DV6

	{
		FPU_PTR_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_CD0,
		RAM_SEQ_PTR16(FPU_C78_A3_C60_A3_CD3)
	},

	{
		FPU_PTR_AG_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0,
		RAM_SEQ_PTR16(FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD3)
	},

	{
		RAM_SEQ_PTR16(FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD0),
		RAM_SEQ_PTR16(FPU_AG_C78_A3_CA2_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD0),
		RAM_SEQ_PTR16(FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD3)
	}
#elif YMTC
	{
		FPU_PTR_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_CD0,
		FPU_PTR_CC6_AG_C60_A3_CD0
	},

	{
		FPU_PTR_AG_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0
	},

	{
		FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0
	}


#else
	{
		FPU_PTR_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_CD0,
		FPU_PTR_CC6_AG_C60_A3_CD0
	},

	{
		FPU_PTR_AG_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0
	},

	{
		FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0,
		FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0
	}
#endif
};
#endif
#if TLC
UBYTE flaEraseTarget_CH(UBYTE ubPhase, UBYTE ubgFQLinkIndex, UBYTE ubPlaneMode)
#else
UBYTE flaEraseTarget_CH(UBYTE ubPhase, UBYTE ubgFQLinkIndex, UWORD uwFPU)
#endif
{
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubDepth = FQ->ubRelativeDepth;
	U32 ulFEntry = FQ->ulFEntry;
	UBYTE ubBurstBank, ubPhyPlane;
	UWORD uwFBlock;
	M_AssertError(FQ->btNeedStopDMA);
	U32 uliFSA;
#if TLC
	UWORD uwFPU;
#endif

	UBYTE ubChannel;
	UBYTE ubRemainQCnt;
	UBYTE ubRemainQCnt2;

	if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
#if MicronFlashOnly
		if (!gubMicronModeSet) {
			M_AssertError((FQ->ubFQPhase == 0) && FQ->ubMTDepthCnt);
		}
		else {
			M_AssertError((FQ->ubFQPhase == 0) && (FQ->ubMTDepthCnt != 1));
		}
#else
		M_AssertError((FQ->ubFQPhase == 0) && FQ->ubMTDepthCnt);
#endif
	}

	if (ENABLE_DEBUG_RW) {
		ubRemainQCnt = 0;
		ubRemainQCnt2 = 0;
		ubChannel = ubCEIndex & gubPlanesPerBurstMask;
		gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
	}

	M_VirtualToPhysicalCE(ubCEIndex);

	if (TLC && (FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0)) {
		ulFEntry = ulFEntry % gulFastPage4kEntrysPerUnit;
	}

	if (ENABLE_iFSA) {
		if (ENABLE_PLANE_CH_CE) {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
		}
		else {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
		}
		if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
			uwFBlock = FQ->uwFUnit;
			ubPhyPlane = ubBurstBank;
		}
		else {
			if ((TLC == 0) || FQ->btD3) {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
			}
			else {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex, ((guwFWTotalUnitNum + (FQ->uwFUnit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
			}
			if (gubRUTMixPlaneEnable == 0) {
				ubPhyPlane = ubBurstBank;
			}
			else {
				ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
				uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
			}

		}

		if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
			if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
				uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
			}
		}
		M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				FQ->ubALUIndex = ALU_A2;
				goto Mark_NormalSectorAddress;
			}
			else {
				FQ->ubALUIndex = ALU_NORMAL;
#if (B0KB || YMTC_JGS)
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog))  >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
			}
		}
		else {
			FQ->ubALUIndex = ALU_NORMAL;
		Mark_NormalSectorAddress:
#if (B0KB || YMTC_JGS)
			uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
			uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
		}

		uliFSA |= (uwFBlock & 0xFFFF) << 16;

		if ((((uwFBlock >= gulTotalPhyUnitNum) && ((gubDieNumber == 1) && (gubExpandCE == 0))) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog)))) {
			VT->gulDebugInfo[0] = uwFBlock;
			VT->gulDebugInfo[1] = gulTotalPhyUnitNum;
			VT->gulDebugInfo[2] = ulFEntry;
			VT->gulDebugInfo[3] = (gulPlanesPerUnit << gub4kEntrysPerPlaneLog);
			VT->gulDebugInfo[4] = ubgFQLinkIndex;
			M_AssertCriticalError(1, VT_ASSERT_ERASETARGET_CH_0x45);    // SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum
		}
		gulPara0x04.BitMap.btiFSAEn = 1;
	}

	if (ENABLE_iFSA) {

#if TLC
		UBYTE ubiFSACnt;

		guliFSA[0] = uliFSA;

		ubiFSACnt = ubPlaneMode;
#else
		UBYTE ubiFSACnt = 1;

		guliFSA[0] = uliFSA;

		if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0) {
			ubiFSACnt = 4;
		}
		else if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_CD0) {
			ubiFSACnt = 2;
		}
#endif
		for (ubBurstBank = 1; ubBurstBank < ubiFSACnt; ubBurstBank++) {
			if (ENABLE_PLANE_CH_CE) {
				ulFEntry +=  gub4kEntrysPerPlane;
			}
			else {
				ulFEntry +=  gub4kEntrysPerBurst;
			}

			if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
				uwFBlock = FQ->uwFUnit;
				ubPhyPlane = ubBurstBank;
			}
			else {
				if ((TLC == 0) || FQ->btD3) {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
				}
				else {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (FQ->uwFUnit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
				}
				if (gubRUTMixPlaneEnable == 0) {
					ubPhyPlane = ubBurstBank;
				}
				else {
					ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
					uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
				}
			}


			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
				if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
					uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
				}
			}
			M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

			if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0))) {
				if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
					FQ->ubALUIndex = ALU_A2;
#if (B0KB || YMTC_JGS)
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				}
				else {
					FQ->ubALUIndex = ALU_NORMAL;
#if (B0KB || YMTC_JGS)
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				}
			}
			else {
				FQ->ubALUIndex = ALU_NORMAL;
#if (B0KB || YMTC_JGS)
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
			}

			uliFSA |= (uwFBlock & 0xFFFF) << 16;

			M_AssertError(((uwFBlock >= gulTotalPhyUnitNum) && (!ENABLE_MULTI_DIE)) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog)));	// SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum

			guliFSA[ubPhyPlane] = uliFSA;

		}

		gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = guliFSA[0];
		gMTQ->Depth[ubDepth].uliFSA1 = guliFSA[1];
		gMTQ->Depth[ubDepth].uliFSA2 = guliFSA[2];
		gMTQ->Depth[ubDepth].uliFSA3 = guliFSA[3];

		if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
			FQ->ubFSAIndex = 0xFF;
		}

		gulPara0x04.BitMap.btiFSAEn = 1;

	}
	else {
		// FSA ring回FSA 0的處理
		gulPara0x10.BitMap.ubFSA2Cnt = 0;
#if TLC
		if (4 == ubPlaneMode) {
#else
		if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0) {
#endif
			if (FQ->ubFSAIndex > 124) {
				gulPara0x10.BitMap.ubFSA2Cnt = 4 - (128 - FQ->ubFSAIndex);
			}
		}
#if TLC
		else if (2 == ubPlaneMode) {
#else
		else if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_CD0) {
#endif
			if (FQ->ubFSAIndex == 127) {
				gulPara0x10.BitMap.ubFSA2Cnt = 3;
			}
		}
		gulPara0x10.BitMap.ubFSASel = FQ->ubFSAIndex;
		gMTQ->Depth[ubDepth].Para0x10.ulAll = gulPara0x10.ulAll;
	}

#if TLC
	{
#if B0KB
		if (ubPlaneMode == 4) {
			uwFPU = FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0;
		}
		else if (ubPlaneMode == 2) {
			uwFPU = FPU_PTR_AG_C60_A3_AG_C60_A3_CD0;
		}
		else {
			uwFPU = FPU_PTR_AG_C60_A3_CD0;
		}
#else
		UBYTE ubMode = (FQ->ubALUIndex == ALU_A2) ? 1 : 0;
		if (TLC && (FQ->uwFJobInfo & BIT_FJOBI_TLC_TREATMENT)) {
			ubMode = 2;
		}
#if Hynix
		if (((ubPlaneMode >> 1) == 0) && (ubMode == 1) && gubDaCmdEraseEnable) {
			ubMode = 0;
		}
#endif
		uwFPU = garEraseFPU[ubPlaneMode >> 1][ubMode];
#endif
	}
#else
	// 上面算完iFSA後才有FQ->ubALUIndex
#if MicronFlashOnly
	if ((FQ->ubALUIndex == ALU_A2) && (guoFlashUseType.B.btToshiba))
#else
	if (FQ->ubALUIndex == ALU_A2)
#endif
	{
		if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0) {
			uwFPU = FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0;
		}
		else if (uwFPU == FPU_PTR_AG_C60_A3_AG_C60_A3_CD0) {
			uwFPU = FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0;
		}
		else if (uwFPU == FPU_PTR_AG_C60_A3_CD0) {
			uwFPU = FPU_PTR_AG_CA2_C60_A3_CD0;
		}
	}
#endif

	gulPara0x00.BitMap.btStaNoStop  = 0;
	gulPara0x00.BitMap.btSetInternalBusy = 1;
	gulPara0x00.BitMap.btQueryCheck = 0;
	gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex;
	gulPara0x04.BitMap.btIntVctEn = 0;
	gulPara0x04.BitMap.btConvBypass = 0;	// read可能有ConvBypass = 1 or 0, erase不看此bit, 但保險起見還是設定為0
	gulPara0x04.BitMap.btUltraDMADisable = 1;
	gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
		gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
	}
	else {
		gulPara0x1C.BitMap.ubCEValue = ubCEIndex;
	}
	gulPara0x1C.BitMap.ubPolSeqSel = WAIT_TRUE_READY;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
	FQ->btUsingAutoPol = 1;
	FQ->ulAutoPolStartTime = gulOperationTime;
#endif
#if FORCE_AUTOPOL_TIMEOUT
	if ((gubDebugAutoPolFPUSeqInUse == 0) && (gubPreformat == 0) && (VT->gulFTLState.B.btNeedRUTReplace == 0)) {
		gulRandomValue = getIntRandValue(FORCE_AUTOPOL_TIMEOUT_DENOMINATOR);
		if (gulRandomValue < FORCE_AUTOPOL_TIMEOUT_NUMERATOR) {
			gulPara0x1C.BitMap.ubPolSeqSel = WAIT_AUTOPOL_TIMEOUT;
			gubDebugAutoPolFPUSeqInUse = 1;
			gubDebugAutoPolFQIndex = ubgFQLinkIndex;
			gubDebugAutoPolMTDepth = ubDepth;
#if UART_AUTOPOL_TIMEOUT_HANDLING
			UartString("\nEPol Timeout");
#endif
		}
	}
#endif

	gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
	gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
	gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
#if FORCE_AUTOPOL_TIMEOUT
	// restore PolSeqSel
	gulPara0x1C.BitMap.ubPolSeqSel = WAIT_TRUE_READY;
#endif

#pragma flush
	if (ENABLE_DEBUG_RW) {
		while (1) {
			if (ubCEIndex < 8) {
				ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
			}
			else {
				ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
			}
			if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
				M_AssertError(1);
			}
			if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
				break;
			}
		}
	}
	FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | REUSEMT_STOP | QUEUE_INDEX(ubCEIndex));

	FQ->ubFQPhase = ubPhase;
	if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
		FQ->ubMTDepthCnt++;
		gubMTDepthPushNumber[ubCEIndex]++;
	}

	return 0;
}

#if 0
UBYTE flaFlashToSramRandCache_CH(UBYTE ubFQLinkIndexForCmd, UBYTE ubFQLinkIndexForDMA) {
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckNonBurnerModeToDisableFunctionReturnValid(0);
	FlashQueue_t *FQ = &gFQI.gFQLink[ubFQLinkIndexForCmd];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubDepth;
	UBYTE ubMode;
	U32 ulFEntry = FQ->ulFEntry;
	UBYTE ubBurstBank;
	UBYTE ubDoingPhase;
	UWORD uwFBlock;
	UWORD uwFPU;
	U32 uliFSA;

	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
	}
	ubDoingPhase = 0;

	M_VirtualToPhysicalCE(ubCEIndex);

	M_AssertError(FQ->ubFJob != BYTE_FJOB_READ);

Mark_CMD_Phase:
	FQ = &gFQI.gFQLink[ubFQLinkIndexForCmd];

	if (FQ->ubFQPhase == 0) {
		ubMode = USE_REUSE_MT;

		uwFPU = FPU_PTR_AG_C00_A5_C31;

		ubDepth = FQ->ubDepth;

		if (ENABLE_iFSA) {
			UBYTE ubD1Unit = Default_D1_Unit;
#if TLC
			if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
				ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
				if ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
					ulFEntry = FQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
				}
			}
#endif

			if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
				uwFBlock = FQ->uwFUnit;
			}
			else {
				if ((TLC == 0) || FQ->btD3 || (ubD1Unit == Default_D1_Unit)) {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
				}
				else {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
				}
			}

			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
				if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
					uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
				}
			}

			M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

			if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
				if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
					FQ->ubALUIndex = ALU_A2;
#if B0KB
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				}
				else {
					FQ->ubALUIndex = ALU_NORMAL;
#if B0KB
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog)) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				}
			}
			else {
				FQ->ubALUIndex = ALU_NORMAL;
#if TLC
				if (TLC) {
#if B0KB
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((ulFEntry >> gub4kEntrysPerPageLog) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask)); // 3:LMU
#endif
				}
				else {
#else
				{
#endif
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
				}
			}

			uliFSA |= (uwFBlock & 0xFFFF) << 16;

			M_AssertCriticalError((((uwFBlock >= gulTotalPhyUnitNum) && ((gubDieNumber == 1) && (gubExpandCE == 0))) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog))), VT_ASSERT_FLASHTOSRAMRANDCACHE_CH_0x46);	// SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum

			gulPara0x04.BitMap.btiFSAEn = 1;

			if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
				FQ->ubFSAIndex = 0xFF;
			}

			guliFSA[0] = uliFSA;

			gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = guliFSA[0];
		}
		else {
			gulPara0x10.BitMap.ubFSASel = FQ->ubFSAIndex;
			gMTQ->Depth[ubDepth].Para0x10.ulAll = gulPara0x10.ulAll;
		}

		M_AssertError(FQ->ubFQPhase != 0);

		gulPara0x00.BitMap.btQueryCheck  = 0;
		gulPara0x00.BitMap.btSetInternalBusy = 1;
		gulPara0x00.BitMap.btStaNoStop  = 0;
		gulPara0x04.BitMap.btIntVctEn = 1;
		gulPara0x04.BitMap.btUltraDMADisable = 0;
		gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex;

		if (FQ->ubALUIndex == ALU_A2) {
			uwFPU = FPU_PTR_CA2_C31;
		}

		gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
			gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
		}
		else {
			gulPara0x1C.BitMap.ubCEValue  = ubCEIndex;
		}

		gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
		gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
		gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;

#pragma flush
		FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | ((ubMode & USE_REUSE_MT) ? 0 : REUSEMT_STOP) | QUEUE_INDEX(ubCEIndex));

		FQ->ubFQPhase = BYTE_CMD_ADDR_DONE;

		ubDoingPhase |= BIT0;
	}

Mark_DMA_Phase:
	FQ = &gFQI.gFQLink[ubFQLinkIndexForDMA];

	M_AssertError((FQ->ubFQPhase != BYTE_CMD_ADDR_DONE) && (FQ->ubFQPhase != BYTE_WAIT_DMA));

	if (FQ->btNeedStopDMA == 0) {
		ubDepth = FQ->ubDepth;

		uwFPU = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;

		if (ENABLE_iFSA) {
			UBYTE ubD1Unit = Default_D1_Unit;
#if TLC
			if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
				ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
				if ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
					ulFEntry = FQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
				}
			}
#endif

			if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
				uwFBlock = FQ->uwFUnit;
			}
			else {
				if ((TLC == 0) || FQ->btD3 || (ubD1Unit == Default_D1_Unit)) {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
				}
				else {
					uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
				}
			}

			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
				if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
					uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
				}
			}

			M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

			if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
				if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
					FQ->ubALUIndex = ALU_A2;
#if B0KB
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				}
				else {
					FQ->ubALUIndex = ALU_NORMAL;
#if B0KB
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog)) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage((U32)(ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				}
			}
			else {
				FQ->ubALUIndex = ALU_NORMAL;
#if TLC
				if (TLC) {
#if B0KB
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((ulFEntry >> gub4kEntrysPerPageLog) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask)); // 3:LMU
#endif
				}
				else {
#else
				{
#endif
					uliFSA = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
				}
			}

			uliFSA |= (uwFBlock & 0xFFFF) << 16;

			M_AssertError(((uwFBlock >= gulTotalPhyUnitNum) && (!ENABLE_MULTI_DIE)) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog)));	// SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum

			gulPara0x04.BitMap.btiFSAEn = 1;

			if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
				FQ->ubFSAIndex = 0xFF;
			}

			guliFSA0_2 = uliFSA;

			gMTQ->Depth[ubDepth].uliFSA0_2 = guliFSA0_2;
		}
		else {
			gulPara0x20.BitMap.ubFSASel2 = FQ->ubFSAIndex;
		}

		gulPara0x00.BitMap.btStaNoStop  = 0;
		gulPara0x00.BitMap.btQueryCheck  = FQ->FQPara.B.btQueryCheck;
		gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex; // 僅在flaSetFlashSourceAddrCE_CH重新設定FQ->ubALUIndex
		gulPara0x04.BitMap.btUltraDMADisable = 0;
		gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;

		gulPara0x00.BitMap.btSetInternalBusy = 1;	// 因為下面會再次assign Para0x00, 所以需維持其cmd phase的autopol, 不然會下完cmd沒有autopol就DMA, 造成LCA cmp/signoffmask, unzip fail...

		gulPara0x20.BitMap.btSetInternalBusy2 = 0;
		gulPara0x20.BitMap.btbtIntVctEn2 = 1;
		gulPara0x20.BitMap.uw2ndFPUPtr = uwFPU;
		gMTQ->Depth[ubDepth].Para0x20.ulAll = gulPara0x20.ulAll;

		if (FQ->btNeedSetCEWhenTriggerDMA) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
				gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
			}
			else {
				gulPara0x1C.BitMap.ubCEValue  = ubCEIndex;
			}
			gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
		}

		gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
		gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;

		FQ->FQPara.B.btCanRemove = 1;
#pragma flush
		FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | REUSEMT_STOP | QUEUE_INDEX(ubCEIndex));
		//FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) |REUSEMT_AUTO_TRIG |QUEUE_INDEX(ubCEIndex)); //Reuse MT auto trig

		FQ->ubFQPhase = BYTE_TRIG_DMA_DONE;

		ubDoingPhase |= BIT1;
	}

	if (ENABLE_CY_DEBUG) {
		M_AssertError(ubDoingPhase == 0); //進來一定至少做一種phase
	}
	return 0;
}
#endif

#if ENABLE_RESET_RETRY_LEVEL
void flaSwitchDefaultRetryLevel(UBYTE ubgFQLinkIndex , UBYTE ubCEIndex) {
	UWORD uwFPU;

#if TLC_BICS2

	uwFPU = (guoFlashUseType.B.btSandisk) ? RAM_SEQ_PTR16(FPU_CEF_A12_D00_D00_D00_D00_C70_POL_MK20_CEF_A13_D00_D00_D00_D00_C70_POL_MK20) : RAM_SEQ_PTR16(FPU_CEF_A89_D00_D00_D00_D00_C70_POL_MK20_CEF_A8A_D00_D00_D00_D00_C70_POL_MK20);
#else //MLC

	uwFPU = RAM_SEQ_PTR16(FPU_C5C_CC5_C55_A04_D00_C55_A05_D00_C55_A06_D00_C55_A07_D00_C55_A0D_D00);
#endif
	gulKeepRetryLevelCE_BITMAP &= ~(BIT0 << ubCEIndex);
	FlashQueue_t * FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	FQ->ubBicsRetryLevelDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
	flaSendCmd_CH(ubgFQLinkIndex, FQ->ubCEIndex, uwFPU, 0, 0, 0xFF);
	FQ->ubMTDepthCnt ++;
}
#endif

#if MicronFlashOnly
void flaSwitchMicronSLCMode(UBYTE  ubgFQLinkIndex , UBYTE ubCEIndex) {

	gubMicronModeSet = 0;
	if ( ENABLE_MICRON_SLC && (guoFlashUseType.B.btIntelMicron && guoFlashUseType.B.btA2Cmd)) {
		//Check SLC or MLC mode
		UWORD uwSLCAccess = 0;
		UWORD uwFPUPointer;
		FlashQueue_t *FQTemp = &gFQI.gFQLink[ubgFQLinkIndex];
#if B0KB
		UBYTE ubD1UnitTemp = (FQTemp->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQTemp->ubFSAIndex : gubD1LinkTable[FQTemp->uwFUnit].B.ubD1Link;
		if ( (((FQTemp->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) && (FQTemp->btD3 == 0)
		        && (ubD1UnitTemp != Default_D1_Unit)) || (FQTemp->uwFJobInfo & BIT_FJOBI_FASTPAGE)) {
			uwSLCAccess = BIT_FJOBI_FASTPAGE;
		}
#else
		uwSLCAccess = FQTemp->uwFJobInfo;
#endif
		// Micron flash mode change.
		gubMicronModeSet = 1;
		FQTemp->ubInExitSLCDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
		if ((uwSLCAccess & BIT_FJOBI_FASTPAGE) == 0) {
			//switch to MLC
			gubMicronMode[ubCEIndex] = MICRON_MLC;
			uwFPUPointer = FPU_PTR_CDF;
		}
		else {
			//switch to SLC
			gubMicronMode[ubCEIndex] = MICRON_SLC;
			uwFPUPointer = FPU_PTR_CDA;
		}
		flaSendCmd_CH(ubgFQLinkIndex, ubCEIndex, uwFPUPointer, 0, 0, 0xFF);
		FQTemp->ubMTDepthCnt ++;
	}
}
#endif
U32 flaFastPage(U32 ulFSector) {
	U32 ulFastPageSector;
	M_FastPage(ulFastPageSector, ulFSector);
	return ulFastPageSector;
}

UBYTE flaFlashToSram_0030(UBYTE ubPhase, UBYTE ubgFQLinkIndex, UWORD uwFPU, UBYTE ubMode) {
	/*
		因為burner爆code加上burner mode下都是直接下00 30 DMA只會用到flaFlashToSram_CH(), 不會用到這個func, 所以burner mdoe直接拿掉這func
	*/
	M_CheckBurnerModeToDisableFunctionReturnValid(0);

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubDepth = FQ->ubDepth;
	U32 ulFEntry = FQ->ulFEntry;
	UBYTE ubBurstBank, ubPhyPlane;
	UWORD uwFBlock;
	U32 uliFSA;
#if TLC
	UWORD uwFPU2;
#if B0KB

	if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator) {
		uwFPU2 = FPU_PTR_AG_C00_DR;
	}
	else {
		uwFPU2 = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
	}
	//uwFPU2 = FPU_PTR_AG_C00_DR;

#else
#if Hynix
	if (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC) {
		uwFPU2 = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
	}
#elif YMTC
	uwFPU2 = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
#else
	if (!gulDQSREMismatchMap) {
		uwFPU2 = (TLC) ? (RAM_SEQ_PTR16(FPU_AG_C05_A5_CE0_DR)) : (FPU_PTR_AG_C00_DR);
	}
	else {
		uwFPU2 = RAM_SEQ_PTR16(FPU_AG_DLY_C05_A5_CE0_DR);
	}

#endif
#endif
#else
	UWORD uwFPU2;
#if MicronFlashOnly
	if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator) {
#else
	if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
#endif
		if (!gulDQSREMismatchMap) {
			uwFPU2 = ((HV) ? FPU_PTR_AG_C00_A5_C05_A2_CE0_DR : FPU_PTR_AG_C00_DR);// To avoid read out fail
		}
		else {
			uwFPU2 = RAM_SEQ_PTR16(FPU_AG_DLY_C00_A5_C05_A2_CE0_DR);
		}
	}
	else {
		uwFPU2 = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
	}
#endif

	UBYTE ubChannel;
	UBYTE ubRemainQCnt;
	UBYTE ubRemainQCnt2;

	if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
#if MicronFlashOnly
		if (!gubMicronModeSet) {
			M_AssertError((FQ->ubFQPhase == 0) && FQ->ubMTDepthCnt);
		}
		else {
			M_AssertError((FQ->ubFQPhase == 0) && (FQ->ubMTDepthCnt != 1));
		}
#else
		M_AssertError((FQ->ubFQPhase == 0) && FQ->ubMTDepthCnt);
#endif
	}
	M_AssertError(FQ->ubFJob != BYTE_FJOB_READ);

	if (ENABLE_DEBUG_RW) {
		ubRemainQCnt = 0;
		ubRemainQCnt2 = 0;
		ubChannel = ubCEIndex & gubPlanesPerBurstMask;
		gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
	}
	M_VirtualToPhysicalCE(ubCEIndex);

	if (ENABLE_iFSA) {
		UWORD ubD1Unit = Default_D1_Unit;
#if TLC
		if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {
			ubD1Unit = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link);
			if ((FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit)) {
				ulFEntry = FQ->ulFEntry  % gulFastPage4kEntrysPerUnit;
			}
		}
#endif
		if (ENABLE_PLANE_CH_CE) {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
		}
		else {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
		}

		if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
			uwFBlock = FQ->uwFUnit;
			ubPhyPlane = ubBurstBank;
		}
		else {
			if ((TLC == 0) || FQ->btD3 || (ubD1Unit == Default_D1_Unit)) {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
			}
			else {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
			}
			if (gubRUTMixPlaneEnable == 0) {
				ubPhyPlane = ubBurstBank;
			}
			else {
				ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
				uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
			}
		}

		if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
			if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
				uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
			}
		}

		M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);
		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				FQ->ubALUIndex = ALU_A2;
#if Hynix
				uwFPU = (gubDaCmdEnable) ? (FPU_PTR_AG_C00_A5_C30) : (FPU_PTR_AG_CA2_C00_A5_C30);
#else
#if MicronFlashOnly
#else
				uwFPU = FPU_PTR_AG_CA2_C00_A5_C30;
#endif
#endif
				//D1 SLC or D3 SLC

				goto Mark_NormalSectorAddress;
			}
			else {
				FQ->ubALUIndex = ALU_NORMAL;
				U32 ulFastPageSector;
				M_FastPage(ulFastPageSector, (((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)));
#if (B0KB || YMTC_JGS)
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((ulFastPageSector >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((ulFastPageSector >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
#endif
				//MLC
			}
		}
		else {
			FQ->ubALUIndex = ALU_NORMAL;
#if TLC

#if B0KB || YMTC

#else
			UBYTE ubD3LMU;
			ubD3LMU = ( ( (M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) % 3 ) + 1 );
			if ( uwFPU == FPU_PTR_AG_C00_A5_C30 ) {

#if (TLC_BICS2 & (!BURNER))
				if (guoFlashUseType.B.btSandisk) {
					if ( ubD3LMU == 1) {
						uwFPU  = RAM_SEQ_PTR16(FPU_C5D_C01_AG_C00_A5_C30);
					}
					else if ( ubD3LMU == 2) {
						uwFPU  = RAM_SEQ_PTR16(FPU_C5D_C02_AG_C00_A5_C30);
					}
					else {
						uwFPU  = RAM_SEQ_PTR16(FPU_C5D_C03_AG_C00_A5_C30);
					}
				}
				else
#endif
				{
					if ( ubD3LMU == 1) {
						uwFPU = RAM_SEQ_PTR16(FPU_C01_AG_C00_A5_C30);
					}
					else if ( ubD3LMU == 2) {
						uwFPU = RAM_SEQ_PTR16(FPU_C02_AG_C00_A5_C30);
					}
					else {
						uwFPU = RAM_SEQ_PTR16(FPU_C03_AG_C00_A5_C30);
					}
				}
			}
#endif


#else
#if ENABLE_RESET_RETRY_LEVEL
			if (((BIT0 << ubCEIndex) & gulKeepRetryLevelCE_BITMAP) && (FQ->ubALUIndex != ALU_A2) && (!guoFlashUseType.B.btSandisk)) {

				if ( uwFPU == FPU_PTR_AG_C00_A5_C30 ) {
					uwFPU = (gub26CMDflag) ? RAM_SEQ_PTR16(FPU_PTR_AG_C26_C5D_C00_A5_C30) : RAM_SEQ_PTR16(FPU_PTR_AG_C5D_C00_A5_C30);
				}
			}
#endif
#endif
			//D3 TLC, MLC

		Mark_NormalSectorAddress:
#if TLC
			if (TLC) {
#if (B0KB || YMTC_JGS)
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> 2) + (ulFEntry & gub4kEntrysPerPlaneMask));
#else
				if (FQ->ubALUIndex == ALU_NORMAL) { // D3 Read
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) / 3) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask)); // 3:LMU
				}
				else {
					uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) + (ulFEntry & gub4kEntrysPerPlaneMask));
				}
#endif
			}
			else {
#else
			{
#endif
				uliFSA = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			}
		}

		uliFSA |= (uwFBlock & 0xFFFF) << 16;

		if ((((uwFBlock >= gulTotalPhyUnitNum) && ((gubDieNumber == 1) && (gubExpandCE == 0))) || (ulFEntry >= (gulPlanesPerUnit << gub4kEntrysPerPlaneLog)))) {
			VT->gulDebugInfo[0] = uwFBlock;
			VT->gulDebugInfo[1] = gulTotalPhyUnitNum;
			VT->gulDebugInfo[2] = ulFEntry;
			VT->gulDebugInfo[3] = (gulPlanesPerUnit << gub4kEntrysPerPlaneLog);
			VT->gulDebugInfo[4] = ubgFQLinkIndex;
			M_AssertCriticalError(1, VT_ASSERT_FLASHTOSRAM_0030_0x47);  // SwapRUT用的TargetUnit可能 >= gulTotalPhyUnitNum, 所以不卡FQ->uwFUnit >= gulTotalPhyUnitNum
		}

		gulPara0x04.BitMap.btiFSAEn = 1;

		guliFSA[0] = uliFSA;
		gMTQ->Depth[ubDepth].Para0x2C.uliFSA0 = guliFSA[0];

		if ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) == 0) {
			FQ->ubFSAIndex = 0xFF;
		}
	}

	//if (ENABLE_CACHEREAD && (gul4KRandomCacheRead == 0)) {
	if (ENABLE_READ_CHECK_FUNCTION) {
		flaGetUnitReadCheckInfo(ubgFQLinkIndex, 1 , FQ->ubALUIndex);
	}
	M_AssertError(FQ->ubFQPhase != 0);

	gulPara0x00.BitMap.btQueryCheck  = 0;
	gulPara0x00.BitMap.btSetInternalBusy = 1;
	gulPara0x00.BitMap.btStaNoStop  = 0;
	gulPara0x04.BitMap.btIntVctEn = 1;
	gulPara0x04.BitMap.btUltraDMADisable = 0;
	gulPara0x04.BitMap.ubALUSel = FQ->ubALUIndex;
	gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;	// reuse MT且always只用00 30時, 可以在這的cmd phase先順便設定ConvBypass, 實際上到DMA phase才會看ConvBypass
#if TLC
	if (TLC) {
		if (FQ->FQPara.B.btConvBypass) {
			gulPara0x04.BitMap.btConvLogicPageEn = 0;
		}
		else {
#if TSB_BICS4_SUPPORT || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
			gulPara0x04.BitMap.uwConvLogicPage = (M_RefSeedPage(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#else
			gulPara0x04.BitMap.uwConvLogicPage = (M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#endif
			gulPara0x04.BitMap.btConvLogicPageEn = 1;
		}
	}
#endif
	gulPara0x10.BitMap.ubFSASel = FQ->ubFSAIndex;

	gulPara0x1C.BitMap.uwFPUPtr = uwFPU;
#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		gulPara0x1C.BitMap.ubCEValue = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubCEIndex & gubPlanesPerBurstMask][ubCEIndex / gubPlanesPerBurst]);
		gMTQ->Depth[ubDepth].Para0x28.BitMap.btCESelMode = 0;   //change mode to normal mode
	}
	else {
		gulPara0x1C.BitMap.ubCEValue  = ubCEIndex;
	}
	gulPara0x1C.BitMap.ubPolSeqSel = WAIT_TRUE_READY;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
	FQ->btUsingAutoPol = 1;
	FQ->ulAutoPolStartTime = gulOperationTime;
#endif
#if FORCE_AUTOPOL_TIMEOUT
	if ((gubDebugAutoPolFPUSeqInUse == 0) && (gubPreformat == 0) && (VT->gulFTLState.B.btNeedRUTReplace == 0)) {
		gulRandomValue = getIntRandValue(FORCE_AUTOPOL_TIMEOUT_DENOMINATOR);
		if (gulRandomValue < FORCE_AUTOPOL_TIMEOUT_NUMERATOR) {
			gulPara0x1C.BitMap.ubPolSeqSel = WAIT_AUTOPOL_TIMEOUT;
			gubDebugAutoPolFPUSeqInUse = 1;
			gubDebugAutoPolFQIndex = ubgFQLinkIndex;
			gulPara0x1C.BitMap.uwFPUPtr = FPU_PTR_AG_C00_A5_CFC;	// to simulate data loss
			gubDebugAutoPolMTDepth = ubDepth;
#if UART_AUTOPOL_TIMEOUT_HANDLING
			UartString("\nR0030Pol Timeout");
#endif
		}
	}
#endif

#if ENABLE_RUNTIME_SEEDINIT
	// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
	gMTQ->Depth[ubDepth].ulSeedInit = M_RandomSeed((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)), ((guoFlashUseType.B.btHynix3DV6) ? 512 : 384));
#elif TSB_BICS4_SUPPORT
	gMTQ->Depth[ubDepth].ulSeedInit = flaGenRandSeed(M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#else
	if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 512) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0x0;
	}
	else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1024) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0xffffff;
	}
#if TLC_BICS2
#else /* TLC_BICS2 */
	else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1536) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0x555555;
	}
#if B0KB
#else
	else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2048) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0xAAAAAA;
	}
	else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2560) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0xCCCCCC;
	}
	else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3072) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0x333333;
	}
	else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3584) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0x999999;
	}
	else if ((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 4096) {
		gMTQ->Depth[ubDepth].ulSeedInit = 0x666666;
	}
#endif
#endif /* TLC_BICS2 */
#endif

	// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#endif /* ENABLE_RUNTIME_SEEDINIT */

	gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;
	gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;
	gMTQ->Depth[ubDepth].Para0x10.ulAll = gulPara0x10.ulAll;
	gMTQ->Depth[ubDepth].Para0x1C.ulAll = gulPara0x1C.ulAll;
#if FORCE_AUTOPOL_TIMEOUT
	// restore PolSeqSel
	gulPara0x1C.BitMap.ubPolSeqSel = WAIT_TRUE_READY;
#endif

#pragma flush
	if (ENABLE_DEBUG_RW) {
		while (1) {
			if (ubCEIndex < 8) {
				ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
			}
			else {
				ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
			}
			if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
				M_AssertError(1);
			}
			if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
				break;
			}
		}
	}
	FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | ((ubMode & USE_REUSE_MT) ? 0 : REUSEMT_STOP) | QUEUE_INDEX(ubCEIndex));


	M_AssertError(FQ->btNeedStopDMA);

	gulPara0x00.BitMap.btQueryCheck  = FQ->FQPara.B.btQueryCheck; //此00 30 special function可考慮直接在上面cmd phase就設定btQueryCheck, 其他func則一定要到DMA phase才設定


	guliFSA0_2 = uliFSA;
	gMTQ->Depth[ubDepth].uliFSA0_2 = guliFSA0_2;

	gulPara0x20.BitMap.btSetInternalBusy2 = 0;
	gulPara0x20.BitMap.btbtIntVctEn2 = 1;
	gulPara0x20.BitMap.uw2ndFPUPtr = uwFPU2;
	gMTQ->Depth[ubDepth].Para0x20.ulAll = gulPara0x20.ulAll;
	gMTQ->Depth[ubDepth].Para0x00.ulAll = gulPara0x00.ulAll;

	// ConvBypass的設定移到cmd phase先偷做, 減少這邊多設一次IRAM的動作
	//gulPara0x04.BitMap.btConvBypass = FQ->FQPara.B.btConvBypass;
	//gMTQ->Depth[ubDepth].Para0x04.ulAll = gulPara0x04.ulAll;

	FQ->FQPara.B.btCanRemove = 1;

	if (gubGenReadFailDebug && FQ->btGenFail) {
		if ((Failed_Test_UNC || Failed_Test_E3D) && (FQ->uwFJobInfo & BIT_FJOBI_BUFFERMODE) && (FQ->FQPara.B.btUserData)) {
			if (Failed_Test_UNC) {
				volatile UWORD uwfailGen;
				if (ENABLE_KT_DEBUG_MOREFAIL)
					uwfailGen = getIntRandValue(0x100);	//rand() & 0xff
				else
					uwfailGen = getIntRandValue(0x4000);	//rand() & 0x3fff

				if (uwfailGen == 0) {
					//UartString("\r\nUnc!");
					//UartLLongHex(guoRCount);
					gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
				}
			}

			if (Failed_Test_E3D && (!FQ->btPreRead)) { //H_E3D_EN在Write方向會讓Flh舉E3D error, 如果非host獨走,可能影響到下一筆Cmd
				volatile UWORD uwfailGen;
				if (ENABLE_KT_DEBUG_MOREFAIL)
					uwfailGen = getIntRandValue(0x100);	//rand() & 0xff
				else
					uwfailGen = getIntRandValue(0x4000);	//rand() & 0x3fff

				if (uwfailGen == 0) {
					HW[HW_MZ_CTRL] |= H_E3D_EN;
				}
			}
		}

#if (TLC && ENABLE_DEBUG_HW_FORCEREADFAIL)
		/*
		//-------------------------------------
		// [CC_Merge_Info] : Just for TLC D1 D3 Read Fail Verify
		//-------------------------------------
		*/
		UBYTE ubGenForceReadFail = 0;
		if (ENABLE_DEBUG_HW_FORCEREADFAIL && (( FQ->uwFJobInfo &  BIT_FJOBI_NON_DMA ) == 0)) {
			/* Avoid to gen read fail while doing CopyBack */
			switch (ENABLE_DEBUG_HW_FORCEREADFAIL_DATA_REGION)	{	//0: Default, 1: D1 Only, 2: D3 Only
				case 1: /* D1 Only */
					ubGenForceReadFail = ((FQ->btD3 == 0) && ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex != Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link != Default_D1_Unit));
					break;
				case 2: /* D3 Only without A2 Fast Page (Table) */
					ubGenForceReadFail = (((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex == Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link == Default_D1_Unit) && ((FQ->FQPara.B.btRWTableUnit) == 0));
					break;
				case 3:/* D3 + Table */
					ubGenForceReadFail = ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex == Default_D1_Unit : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link == Default_D1_Unit);
					break;
				case 4: /* D1 + D3 without Table*/
					ubGenForceReadFail = ((FQ->FQPara.B.btRWTableUnit) == 0);
					break;
				case 0: /* Default */
				default:
					ubGenForceReadFail = 1;
					//[CC_TEMP] : Test without table
					//ubGenForceReadFail = ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE)==0);
					break;
			}
		}

		if (ENABLE_DEBUG_HW_FORCEREADFAIL && ubGenForceReadFail && (gubPreformat == 0) && (FQ->btCheckEmpty == 0) && ((guwRUTLogNum < (RUT_LOG_NUM / 2)) || (VT->gulFTLState.B.btNeedRUTReplace)))
#else
#if (ENABLE_GEN_FAIL_WHEN_SCAN_ACTIVE_GR || ENABLE_GEN_FAIL_WHEN_COPY_UNIT)
		if (ENABLE_DEBUG_HW_FORCEREADFAIL && (gubPreformat == 0) && gubBunchRSCorrectFlag && FQ->btBunchRSParity && ((guwRUTLogNum < (RUT_LOG_NUM / 2)) || (VT->gulFTLState.B.btNeedRUTReplace)))
#else
		if (ENABLE_DEBUG_HW_FORCEREADFAIL && (gubPreformat == 0) && (FQ->btCheckEmpty == 0) && ((guwRUTLogNum < (RUT_LOG_NUM / 2)) || (VT->gulFTLState.B.btNeedRUTReplace)))
#endif
#endif
		{
			/* start if ENABLE_DEBUG_HW_FORCEREADFAIL */
#if GEN_FAIL_PER_MILLE>0
			if (getIntRandValue(1000) >= (1000 - GEN_FAIL_PER_MILLE)) {
#else
			if (getIntRandValue(100) >= 95) {
#endif
				if (getIntRandValue(100) >= ENABLE_DEBUG_HW_FORCEREADFAIL_MT_PERCENTAGE) {
					UBYTE ubLoop = 0;
					UBYTE ubLoopCount = 0;
					ubLoopCount = (getIntRandValue(4) + 1);
					while (ubLoop < ubLoopCount) {
						gFREG[FCTLL_UNC_CFG].L |= FORCE_BCH_UNCORRECTABLE_FRAME(1 << (getIntRandValue(4)));
						ubLoop++;
					}
				}
				else {
					//UartString("\n\nUNC: ");
					//Uart_Tx_DataHex(ubgFQLinkIndex);
					//UartString("\n");
					gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
				}
			}
		} /* end if ENABLE_DEBUG_HW_FORCEREADFAIL */

		if (ENABLE_BURNER_FORCEGENFAIL && (FQ->btCheckEmpty == 0)) {
			if (getIntRandValue(2)) {
				gFREG[FCTLL_UNC_CFG].L |= getIntRandValue(0x11);	//(rand() & 0x10)
			}
			else {
				gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
			}
		}

		if (ENABLE_DEBUG_HW_FORCEREADFAIL && FQ->FQPara.B.btRWTableUnit) {
			if (ENABLE_DEBUG_HW_FORCEREADFAIL_DATA_REGION == 3) {
				gubTemp = 0;
				if (guwRUTLogNum < (RUT_LOG_NUM - 100)) {
					gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 1;	// 這邊造fail對IRAM有read-modify-write, 但fail handle test不care performance
				}
			}
		}
	}

#pragma flush
	if (ENABLE_DEBUG_RW) {
		while (1) {
			if (ubCEIndex < 8) {
				ubRemainQCnt = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_0].L, ubCEIndex / gubPlanesPerBurst);
			}
			else {
				ubRemainQCnt2 = GET_QUEUE_NON_EXT_NUM(gFREG[FCTLL_MTQ_INF_1].L, (ubCEIndex / gubPlanesPerBurst) & 3);
			}
			if ((ubRemainQCnt >= MT_QUEUE_DEPTH) || (ubRemainQCnt2 >= MT_QUEUE_DEPTH)) {
				M_AssertError(1);
			}
			if ((ubRemainQCnt < MT_QUEUE_DEPTH) && (ubRemainQCnt2 < MT_QUEUE_DEPTH)) {
				break;
			}
		}
	}
	if (ENABLE_CY_DEBUG) {
		L4KTable16B * pL4KTablePtr;
		UBYTE ubL4KCount;
		for (ubL4KCount = 0; ubL4KCount < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			M_AssertError(pL4KTablePtr->Para0x04.BitMap.ubL4K_SPRV);
		}
	}
	FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) | REUSEMT_STOP | QUEUE_INDEX(ubCEIndex));
	//FCONL[FCONL_MT_CFG] = (MT_INDEX(ubDepth) |REUSEMT_AUTO_TRIG |QUEUE_INDEX(ubCEIndex)); //Reuse MT auto trig


	FQ->ubFQPhase = BYTE_TRIG_DMA_DONE;
	if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
		FQ->ubMTDepthCnt += 2;
		gubMTDepthPushNumber[ubCEIndex] += 2;
	}
#if TLC
	VT->guoTotalNandReadSectorCnt += (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum * SectorsPer4K);
#else
	VT->guoTotalNandReadSectorCnt += gubSectorsPerPlane;
#endif

	return 0;
}

#if TLC_NES_TEST_SB_FLOW
void flaNES_SB_TESTFlow( UBYTE ubgFQLinkIndex ) {
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	if ((guwNESFQIndex == FQ->ubL4kIndex)) {
		UBYTE ubChannel = (FQ->ubCEIndex & 1);
		U32 ulTemp_FCONL_FCE_ENB;

		// force empty to avoid program parity clear interrupt vector enable bit bug
		gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);

		// wait the channel idle for read retry
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		// wait the channel true idle when force empty
		gFREG[FCTLL_DBG_INF].L = (0x0C);
		while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));
		gFREG[FCTLL_INT_CFG].L &= (~ABORT_DONE_INTR_EN); // follow FLH RegSpec 4.19 MTQ Manual Abort
		gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix the future of copy data bug & remain 2 FQ (1 program parity + 1 read FQ) bug & the future mix of one and two(already finish first) parity bug

		ulTemp_FCONL_FCE_ENB = FCONL[FCONL_FCE_ENB];
		FCONL[FCONL_FCE_ENB] = 0x00000000;
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (FQ->ubCEIndex / gubPlanesPerBurst);
		UBYTE ubj, ubDepth_t;
		M_GetMTDepth(ubDepth_t);
		gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
		gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth_t);
		for (ubj = 0 ; ubj < FQ->ubL4kNum ; ubj++) {
			NESGetIBFData(ubj, ubDepth_t);
			NESSendC9Data(ubj, 1, guwNESDataLen);
		}
		NESSendC9Data_CMD10();
		M_AddMTDepth(ubDepth_t);
		FCONL[FCONL_FCE_ENB] = ulTemp_FCONL_FCE_ENB;

		gFREG[FCTLL_INT_CFG].L |= ABORT_DONE_INTR_EN;
		gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
		gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
	}

}
#endif

#if TLC
UBYTE flaCheckD3CopyBackMTDone(UBYTE ubgFQLinkIndex) {
	M_CheckBurnerModeToDisableFunctionReturnValid(1);
	M_CheckRDTModeToDisableFunctionReturnValid(1);
	/*
	保證R/W都是Multi Plane 的FQ收掉
	保證R/W都是Multi Plane 進行
	不能在FQ Doing裡面看到有機會做One Plane R/W
	*/
	if (TLC) {
		UBYTE ubPlaneIndex;
		UBYTE ubgFQLinkIndex_NextPlane;
		FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
		U32 ulFEntry = FQ->ulFEntry;
		UBYTE ubBurstBank;
		UBYTE ubMaxPlane = ((guoFlashUseType.B.btFourPlane) ? 4 : (guoFlashUseType.B.btTwoPlane) ? 2 : 1);
		UBYTE ubCEIndex = FQ->ubCEIndex;

		M_VirtualToPhysicalCE(ubCEIndex);
		UBYTE ubFQIndex = gFQI.ubFQFirst[ubCEIndex];

		if (ENABLE_PLANE_CH_CE) {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
		}
		else {
			ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
		}

		if ((ubBurstBank != PLANEA) /*不是PLANE A*/
		        || (!FQ->ubD3WriteMode) /*不是D3 WRITE*/
		        || (((FQ->uwFJobInfo & BIT_FJOBI_NON_DMA) == 0x00)  && (FQ->ubFJob != BYTE_FJOB_WRITE ))
		        || ((FQ->ubD3WriteMode) && (FQ->btIsMixPlaneUnit))) {
			return 1;/*PASS Check*/
		}

		/*
		當前這筆為D3 PLANE A
		直接檢查下一筆PLANE 做完沒
		*/
		FlashQueue_t *FQ_Next;
		for (ubPlaneIndex = PLANEB; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex++) {
			ubgFQLinkIndex_NextPlane = gFQI.ubFQIndex[ubCEIndex][((ubFQIndex + ubPlaneIndex) & FQ_NUM_MASK)];
			FQ_Next = &gFQI.gFQLink[ubgFQLinkIndex_NextPlane];

			// debug cnt
			if (FQ_Next->FQPara.B.btCanRemove && (gubINTTable[FQ_Next->ubDepth].B.btAll_Done == 0) && (gubINTTable[FQ_Next->ubRemainderDepth].B.btSTA) && (FQ_Next->ubRemainderDepth != 0xFF)) {
#if ENABLE_PROGRAM_FAIL_CHECK_UART
				UartString("\nProFail!");
#endif
				VT->gulCopyBackProgramFail_DebugCount++;
			}

			if (!(((gubINTTable[FQ_Next->ubDepth].B.btAll_Done && (gubINTTable[FQ_Next->ubDepth].B.btAbort_Done == 0) && (FQ_Next->ubDepth != 0xFF))
			        || (gubINTTable[FQ_Next->ubRemainderDepth].B.btSTA && (FQ_Next->ubRemainderDepth != 0xFF)) || (gubINTTable[FQ_Next->ubRelativeDepth].B.btMT_STOP)) /*Check Fail*/
			        && FQ_Next->FQPara.B.btCanRemove)) { // 這裡可以直接用ubRelativeDepth btDMA_Done作完即可，但需要verify時間而先用此方式
				break;/*還沒做完*/
			}
		}
		if (ubPlaneIndex != ubMaxPlane) {
			return 0;/*還沒全部做完*/
		}
		return 1;/*全部做完*/
	}
	else {
		return 1;
	}
}
#endif


#if TSB_BICS4_SUPPORT
U32 flaGenRandSeed(UWORD uwPage) {

	U32 ulSeed = 0;
#if TSB_BICS4_SUPPORT
	if (guoFlashUseType.B.btToshibaBiCs4) {
		if (uwPage < 384) {
			ulSeed = 0x0;
		}
		else if (uwPage < 768) {
			ulSeed = 0xFFFFFF;
		}
		else if (uwPage < 1152) {
			ulSeed = 0x555555;
		}
	}
	else {	//For BICS2, BICS3
		if (uwPage < 512) {
			ulSeed = 0x0;
		}
		else if (uwPage < 1024) {
			ulSeed = 0xFFFFFF;
		}
	}
#else
	if (uwPage < 512) {
		ulSeed = 0x0;
	}
	else if (uwPage < 1024) {
		ulSeed = 0xFFFFFF;
	}
#if TLC_BICS2
#else /* TLC_BICS2 */
	else if (uwPage < 1536) {
		ulSeed = 0x555555;
	}
#if B0KB
#else
	else if (uwPage < 2048) {
		ulSeed = 0xAAAAAA;
	}
	else if (uwPage < 2560) {
		ulSeed = 0xCCCCCC;
	}
	else if (uwPage < 3072) {
		ulSeed = 0x333333;
	}
	else if (uwPage < 3584) {
		ulSeed = 0x999999;
	}
	else if (uwPage < 4096) {
		ulSeed = 0x666666;
	}
#endif
#endif /* TLC_BICS2 */
#endif //BICS4

	return ulSeed;
}
#endif

#if ENABLE_READ_CHECK_FUNCTION && (!BURNER)
UWORD flaGetForceReadThreshold(UBYTE ubA2Mode , UWORD uwUnit) {
	UWORD uwTempThreshold = 0xFFFF;
#if TLC
	U32 ulTempEC = (gubD1LinkTable[uwUnit].B.ubD1Link == Default_D1_Unit) ? gulEC_D3[uwUnit].B.ulEraseCount : gulEC_D1[gubD1LinkTable[uwUnit].B.ubD1Link].B.ulEraseCount;
#else
	U32 ulTempEC = gulEC_D3[uwUnit].B.ulEraseCount;
#endif
	if (ubA2Mode) { //A2Mode
		if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshiba1Znm && (!TLC)) { // SAN
			if (ulTempEC > 30000) {
				uwTempThreshold = 30;
			}
			else if (ulTempEC > 25000) {
				uwTempThreshold = 40;
			}
			else if (ulTempEC > 20000) {
				uwTempThreshold = 60;
			}
			else if (ulTempEC > 15000) {
				uwTempThreshold = 100;
			}
			else if (ulTempEC > 10000) {
				uwTempThreshold = 150;
			}
			else if (ulTempEC > 5000) {
				uwTempThreshold = 400;
			}
			else {
				uwTempThreshold = 500;
			}
		}
		else if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshibaBiCs3) { //San Bics3
			if (ulTempEC > 30000) {
				uwTempThreshold = 200;
			}
			else if (ulTempEC > 20000) {
				uwTempThreshold = 600;
			}
			else if (ulTempEC > 10000) {
				uwTempThreshold = 800;
			}
			else {
				uwTempThreshold = 1000;
			}
		}
		else if (guoFlashUseType.B.btToshiba1Znm && TLC) { // 15nm TLC
			if (ulTempEC > 30000) {
				uwTempThreshold = 30;
			}
			else if (ulTempEC > 25000) {
				uwTempThreshold = 40;
			}
			else if (ulTempEC > 20000) {
				uwTempThreshold = 50;
			}
			else if (ulTempEC > 15000) {
				uwTempThreshold = 80;
			}
			else {
				uwTempThreshold = 140;
			}
		}
		else if (guoFlashUseType.B.btToshiba1Znm && !TLC) { // 15 nm MLC
			if (ulTempEC > 30000) {
				uwTempThreshold = 30;
			}
			else if (ulTempEC > 25000) {
				uwTempThreshold = 50;
			}
			else if (ulTempEC > 15000) {
				uwTempThreshold = 90;
			}
			else {
				uwTempThreshold = 200;
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs2) { // Bics2
			if (ulTempEC > 30000) {
				uwTempThreshold = 150;
			}
			else if (ulTempEC > 25000) {
				uwTempThreshold = 300;
			}
			else if (ulTempEC > 20000) {
				uwTempThreshold = 600;
			}
			else {
				uwTempThreshold = 1000;
			}
		}
		else if (guoFlashUseType.B.btIntelMicronL06B) { //L06B
			if (ulTempEC > 30000) {
				uwTempThreshold = 500;
			}
			else if (ulTempEC > 25000) {
				uwTempThreshold = 600;
			}
			else if (ulTempEC > 20000) {
				uwTempThreshold = 800;
			}
			else {
				uwTempThreshold = 1000;
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs3) { // bics3
			if (ulTempEC > 90000) {
				uwTempThreshold = 120;
			}
			else if (ulTempEC > 70000) {
				uwTempThreshold = 150;
			}
			else {
				uwTempThreshold = 200;
			}
		}
		else if ((guoFlashUseType.B.btToshibaBiCs4) && (!guoFlashUseType.B.btSandisk) && ((gubDensitysize == FLH_Size_512Gb) || (gRRTMode.B.btRevision))) {
			uwTempThreshold = 0xFFFF;
		}
		else if (guoFlashUseType.B.btToshibaBiCs4) { //bics4
			if (ulTempEC > 30000) {
				uwTempThreshold = 300;
			}
			else if (ulTempEC > 20000) {
				uwTempThreshold = 600;
			}
			else if (ulTempEC > 10000) {
				uwTempThreshold = 800;
			}
			else {
				uwTempThreshold = 1000;
			}
		}
		else if (guoFlashUseType.B.btHynix3DV6) { // Hynix V6
			uwTempThreshold = 2000;  // NAND TEAM 做到 EOL 55k 所以不特別卡 EC
		}
		else if (guoFlashUseType.B.btYMTCJGS) {

			if (ulTempEC > 30000) {
				uwTempThreshold = 800;
			}
			else if (ulTempEC > 10000) {
				uwTempThreshold = 1000;
			}
			else {
				uwTempThreshold = 2000;
			}

		}
		else { //default
			if (ulTempEC > 30000) {
				uwTempThreshold = 30;
			}
			else if (ulTempEC > 25000) {
				uwTempThreshold = 50;
			}
			else if (ulTempEC > 15000) {
				uwTempThreshold = 90;
			}
			else {
				uwTempThreshold = 200;
			}
		}
	}
	else { // Non A2 Mode
		if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshiba1Znm && (!TLC) ) { // SAN
			if (ulTempEC > 3000) {
				uwTempThreshold = 15;
			}
			else if (ulTempEC > 2500) {
				uwTempThreshold = 20;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = 30;
			}
			else if (ulTempEC > 1500) {
				uwTempThreshold = 40;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = 70;
			}
			else if (ulTempEC > 500) {
				uwTempThreshold = 130;
			}
			else {
				uwTempThreshold = 150;
			}
		}
		else if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshibaBiCs3) { //San Bics3
			if (ulTempEC > 3000) {
				uwTempThreshold = 10;
			}
			else if (ulTempEC > 2500) {
				uwTempThreshold = 20;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = 30;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = 40;
			}
			else {
				uwTempThreshold = 90;
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs3) {	//Bics3
			if (ulTempEC > 3000) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH5;
			}
			else if (ulTempEC > 2500) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH4;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH3;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH2;
			}
			else {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH1;
			}
		}

		else if (guoFlashUseType.B.btToshiba1Znm && !TLC ) { //15 nm MLC
			if (ulTempEC > 3000) {
				uwTempThreshold = 15;
			}
			else if (ulTempEC > 2500) {
				uwTempThreshold = 20;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = 30;
			}
			else if (ulTempEC > 1500) {
				uwTempThreshold = 40;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = 70;
			}
			else if (ulTempEC > 500) {
				uwTempThreshold = 130;
			}
			else {
				uwTempThreshold = 150;
			}
		}
		else if (guoFlashUseType.B.btToshiba1Znm && TLC) { //15nm TLC
			if (ulTempEC > 1000) {
				uwTempThreshold = 3;
			}
			else if (ulTempEC > 500) {
				uwTempThreshold = 10;
			}
			else {
				uwTempThreshold = 50;
			}
		}
		else if (guoFlashUseType.B.btIntelMicronL06B) { //L06B
			if (ulTempEC > 3000) {
				uwTempThreshold = 50;
			}
			else if (ulTempEC > 1500) {
				uwTempThreshold = 100;
			}
			else {
				uwTempThreshold = 150;
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs2) { // Bics2
			if (ulTempEC > 3000) {
				uwTempThreshold = 5;
			}
			else if (ulTempEC > 1500) {
				uwTempThreshold = 10;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = 15;
			}
			else if (ulTempEC > 500) {
				uwTempThreshold = 50;
			}
			else {
				uwTempThreshold = 70;
			}
		}
		else if ((guoFlashUseType.B.btToshibaBiCs4) && (!guoFlashUseType.B.btSandisk) && ((gubDensitysize == FLH_Size_512Gb) || (gRRTMode.B.btRevision))) {
			if (ulTempEC > 3000) {
				uwTempThreshold = 600;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = 800;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = 1000;
			}
			else {
				uwTempThreshold = 2000;
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs4) { // Bics4
			if (ulTempEC > 3000) {
				uwTempThreshold = 40;
			}
			else if (ulTempEC > 2500) {
				uwTempThreshold = 60;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = 80;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = 100;
			}
			else {
				uwTempThreshold = 150;
			}
		}
		else if (guoFlashUseType.B.btHynix3DV6) { // Hynix V6
			if (ulTempEC > 3000) {
				uwTempThreshold = 60;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = 100;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = 200;
			}
			else {
				uwTempThreshold = 600;
			}
		}
		else if (guoFlashUseType.B.btYMTCJGS) {

			if (ulTempEC > 1000) {
				uwTempThreshold = 600;
			}
			else if (ulTempEC > 500) {
				uwTempThreshold = 700;
			}
			else {
				uwTempThreshold = 900;
			}

		}
		else { //default
			if (ulTempEC > 3000) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH5;
			}
			else if (ulTempEC > 2500) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH5;
			}
			else if (ulTempEC > 2000) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH4;
			}
			else if (ulTempEC > 1500) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH3;
			}
			else if (ulTempEC > 1000) {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH2;
			}
			else {
				uwTempThreshold = ReadDisturb_ForceReadMove_TH1;
			}
		}
	}

	//Retain 1k buffer to do partial ForceReadMove.
	if (uwTempThreshold > ReadDisturb_ForceReadMove_Buffer)
		uwTempThreshold -= ReadDisturb_ForceReadMove_Buffer;

	return uwTempThreshold;
}
#endif
