#ifndef _RDTVarDef_h_
#define _RDTVarDef_h_

#if (RDT)
typedef union{
	ULLONG ullAll;
	struct {
		ULLONG  ubSDRTestResult:    8;     //0x00 8 Bit
		ULLONG  ubCycleNumber:      8;     //     8 bit
		ULLONG  ubStep:             8;     //     8 Bit
		ULLONG  ubTemperature:     8;     //     8 Bit
		ULLONG  ulTimeStamp:        32;	   //	  32 Bit
	}logMapS;
}SDRlog;




///////////////////////////////
typedef union{
	ULONG ulAll;
	struct {
		ULONG  ubFailType:    8;     //0x00 8 Bit
		ULONG  ubFlashCE:      8;     //     8 bit
		ULONG  ubFlashChannel:   8;     //     8 Bit
		ULONG  ubTotalLoopNumber:     8;     //     8 Bit
	}logMapS;
}FlashErrorlogOffset0x00;

typedef union{
	ULONG ulAll;
	struct {
		ULONG  uwPhysicalBlockAddress:    16;     //0x00 16 Bit
		ULONG  uwPhysicalPageAddress:      16;     //    16 bit
	}logMapS;
}FlashErrorlogOffset0x04;

typedef union{
	ULONG ulAll;
	struct {
		ULONG  ubLoopNumber:              8;     //0x00 8 Bit
		ULONG  ubCycleNumber:             8;     //     8 bit
		ULONG  ubStepNumber:              8;     //0x00 8 Bit
		ULONG  ubCorrectableBytesHighBit:        8;     //     8 bit
	}logMapS;
}FlashErrorlogOffset0x08;

typedef union{
	ULONG ulAll;
	struct {
		ULONG  ubCorrectableBytesLowBit:              8;     //0x00 8 Bit
		ULONG  ulTimeStampMS:                 24;     //     8 bit
	}logMapS;
}FlashErrorlogOffset0x0c;

typedef union{
	ULONG ulAll;
	struct {
		ULONG  reserve0:              8;     //0x00 8 Bit
		ULONG  reserve1:                 24;     //     8 bit
	}logMapS;
}FlashErrorlogOffset0x10;

typedef union{
	ULONG ulAll;
	struct {
		ULONG  reserve2:              8;     //0x00 8 Bit
		ULONG  reserve3:                 24;     //     8 bit
	}logMapS;
}FlashErrorlogOffset0x14;

typedef union{
	ULONG ulAll;
	struct {
		ULONG  reserve4:              8;     //0x00 8 Bit
		ULONG  reserve5:                 24;     //     8 bit
	}logMapS;
}FlashErrorlogOffset0x18;

typedef union{
	ULONG ulAll;
	struct {
		ULONG  reserve6:    8;     //0x00 8 Bit
		ULONG  reserve7:      8;     //     8 bit
		ULONG  A2_info:   8;     //     8 Bit
		ULONG  ubplane_info:     8;     //     8 Bit
	}logMapS;
}FlashErrorlogOffset0x1c;

typedef struct{
	FlashErrorlogOffset0x00  Offset0x00;
	FlashErrorlogOffset0x04  Offset0x04;
	FlashErrorlogOffset0x08  Offset0x08;
	FlashErrorlogOffset0x0c  Offset0x0c;
	FlashErrorlogOffset0x10  Offset0x10;
	FlashErrorlogOffset0x14  Offset0x14;
	FlashErrorlogOffset0x18  Offset0x18;
	FlashErrorlogOffset0x1c  Offset0x1c;
}FlashErrorlog;

////////////////////////////
typedef union{
	ULLONG ullAll;
	struct {
		ULLONG  ubLoopNumber:    8;     //0x00 8 Bit
		ULLONG  ubReserved:      8;     //     8 bit
		ULLONG  ubRDTStep:             8;     //     8 Bit
		ULLONG  ubTemperature:     8;     //     8 Bit
		ULLONG  ulTimeStamp:        32;	   //	  32 Bit
	}logMapS;
}TempAndTimeStamp;

////////////////////////
typedef union{
	ULLONG ulAll;
	struct {
		ULONG  uwFlashCE:                        16;     //0x00 8 Bit
		ULONG  uwMaxErrorBitPerBlock:            16;     //     8 bit
		ULONG  uwPhysicalBlockAddress:           16;     //0x00 8 Bit
		ULONG  uwReserved:                       16;
	}logMapS;
}ECCErrorBits;

///////////////////



#endif
#endif
