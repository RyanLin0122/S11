
#ifdef _HOST_DECL_

typedef void (*pFuncPtr) (void);

#if (RDT == 0)

const pFuncPtr  SataCmdTable[256] =
{
    NoOpCmd,                  /* 0x00: no op command                 */
    NotImplement,             /* 0x01: not implemented               */
    NotImplement,             /* 0x02: not implemented               */
    NotImplement,             /* 0x03: not implemented               */
    NotImplement,             /* 0x04: not implemented               */
    NotImplement,             /* 0x05: not implemented               */

    Trim,                 /* 0x06: data set management               */
 
    NotImplement,             /* 0x07: not implemented               */
    NotImplement,             /* 0x08: ATAPI Soft Reset              */
    NotImplement,             /* 0x09: not implemented               */
    NotImplement,             /* 0x0a: not implemented               */
    NotImplement,             /* 0x0b: not implemented               */
    NotImplement,             /* 0x0c: not implemented               */
    NotImplement,             /* 0x0d: not implemented               */
    NotImplement,             /* 0x0e: not implemented               */
    NotImplement,             /* 0x0f: not implemented               */

    ReCalibrate,              /* 0x10: ReCalibrate                   */
    ReCalibrate,              /* 0x11: ReCalibrate                   */
    ReCalibrate,              /* 0x12: ReCalibrate                   */
    ReCalibrate,              /* 0x13: ReCalibrate                   */
    ReCalibrate,              /* 0x14: ReCalibrate                   */
    ReCalibrate,              /* 0x15: ReCalibrate                   */
    ReCalibrate,              /* 0x16: ReCalibrate                   */
    ReCalibrate,              /* 0x17: ReCalibrate                   */
    ReCalibrate,              /* 0x18: ReCalibrate                   */
    ReCalibrate,              /* 0x19: ReCalibrate                   */
    ReCalibrate,              /* 0x1a: ReCalibrate                   */
    ReCalibrate,              /* 0x1b: ReCalibrate                   */
    ReCalibrate,              /* 0x1c: ReCalibrate                   */
    ReCalibrate,              /* 0x1d: ReCalibrate                   */
    ReCalibrate,              /* 0x1e: ReCalibrate                   */
    ReCalibrate,              /* 0x1f: ReCalibrate                   */

    ReadSectors,              /* 0x20: Read Sectors w/ Retry        */
    AP_Entry,           /* 0x21: Read Sectors w/o Retry        */
    NotImplement,             /* 0x22: Read Long w/ Retry            */
    NotImplement,             /* 0x23: Read Long w/o Retry           */
    
    ReadSectors,              /* 0x24: Read Sector EXT               */
    ReadSectors,              /* 0x25: Read DMA EXT                  */
 

    NotImplement,             /* 0x26: not implemented               */
    
    ReadNativeMax,            /* 0x27: read native max address ext               */
    NotImplement,             /* 0x28: not implemented               */
    ReadMultiple,             /* 0x29: Read Multiple EXT             */


    NotImplement,             /* 0x2a: not implemented               */
    NotImplement,             /* 0x2b: not implemented               */
    NotImplement,             /* 0x2c: not implemented               */
    NotImplement,             /* 0x2d: not implemented               */
    NotImplement,             /* 0x2e: not implemented               */
    
    Read_LOG_EXT,             /* 0x2f: Read Log EXT                  */
 

    WriteSectors,             /* 0x30: Write Sectors w/ Retry        */
    AP_Entry,          /* 0x31: Write Sectors w/o Retry       */
    NotImplement,             /* 0x32: Write Long w/ Retry           */
    NotImplement,             /* 0x33: Write Long w/o Retry          */

    WriteSectors,             /* 0x34: Write Sector EXT              */
    WriteSectors,             /* 0x35: Write DMA EXT                 */
  
    NotImplement,             /* 0x36: not implemented               */

    SetMax,                   /* 0x37: Set MAX               */  //S5
    WriteSectors,          /* 0x38: Write Sectors w/o Erase - CF  */

    WriteMultiple,            /* 0x39: Write Multiple EXT            */

    NotImplement,             /* 0x3a: not implemented               */

    NotImplement,             /* 0x3b: not implemented               */

    NotImplement,             /* 0x3c: Write Verify                  */  // by Dragonslee  20080410

    WriteSectorFUA,           /* 0x3d: Write DMA FUA EXT             */

    NotImplement,             /* 0x3e: not implemented               */

    Write_LOG_EXT,/* 0x3f: Write Log EXT                  */
 

    ReadVerify,               /* 0x40: Read Verify w/ Retry          */
    ReadVerify,               /* 0x41: Read Verify w/o Retry         */

    ReadVerify,               /* 0x42: Read Verify EXT               */


    NotImplement,             /* 0x43: not implemented               */
    Zero,             /* 0x44: zero ext               */

    WriteUncorrectable,       /* 0x45: not implemented               */

    NotImplement,             /* 0x46: not implemented               */

    Read_LOG_EXT,             /* 0x47: Read Log EXT DMA                 */
          
    NotImplement,             /* 0x48: not implemented               */
    NotImplement,             /* 0x49: not implemented               */
    NotImplement,             /* 0x4a: not implemented               */
    NotImplement,             /* 0x4b: not implemented               */
    NotImplement,             /* 0x4c: not implemented               */
    NotImplement,             /* 0x4d: not implemented               */
    NotImplement,             /* 0x4e: not implemented               */
    NotImplement,             /* 0x4f: not implemented               */

    NotImplement,              /* 0x50: Format Track                  */  // by Dragonslee  20080410
    NotImplement,             /* 0x51: configure stream               */
    NotImplement,             /* 0x52: not implemented               */
    NotImplement,             /* 0x53: not implemented               */
    NotImplement,             /* 0x54: not implemented               */
    NotImplement,             /* 0x55: not implemented               */
    NotImplement,             /* 0x56: not implemented               */

    Write_LOG_EXT,             /* 0x57: Write Log EXT DMA                 */
  
    NotImplement,             /* 0x58: not implemented               */
    NotImplement,             /* 0x59: not implemented               */
    NotImplement,             /* 0x5a: not implemented               */
    NotImplement,             /* 0x5b: not implemented               */
    NotImplement,             /* 0x5c: not implemented               */
    NotImplement,             /* 0x5d: not implemented               */
    NotImplement,             /* 0x5e: not implemented               */
    NotImplement,             /* 0x5f: not implemented               */

    ReadSectors,              /* 0x60: Read_FPDMA_Queued             */
    WriteSectors,             /* 0x61: Write_FPDMA_Queued            */

    NotImplement,             /* 0x62: not implemented               */
    NotImplement,             /* 0x63: NCQ non-data               */
    NotImplement,             /* 0x64: Send FPDMA Queued               */
    NotImplement,             /* 0x65: Receive FPDMA Queued               */
    NotImplement,             /* 0x66: not implemented               */
    NotImplement,             /* 0x67: not implemented               */
    NotImplement,             /* 0x68: not implemented               */
    NotImplement,             /* 0x69: not implemented               */
    NotImplement,             /* 0x6a: not implemented               */
    NotImplement,             /* 0x6b: not implemented               */
    NotImplement,             /* 0x6c: not implemented               */
    NotImplement,             /* 0x6d: not implemented               */
    NotImplement,             /* 0x6e: not implemented               */
    NotImplement,             /* 0x6f: not implemented               */
    Seek,                     /* 0x70: Seek Command                  */
    Seek,                     /* 0x71: Seek Command                  */
    Seek,                     /* 0x72: Seek Command                  */
    Seek,                     /* 0x73: Seek Command                  */
    Seek,                     /* 0x74: Seek Command                  */
    Seek,                     /* 0x75: Seek Command                  */
    Seek,                     /* 0x76: Seek Command                  */
    NotImplement,     /* 0x77: not implemented  */
    AccessibleMaxAddress,     /* 0x78: Accessible max address configration                  */
    Seek,                     /* 0x79: Seek Command                  */
    Seek,                     /* 0x7a: Seek Command                  */
    Seek,                     /* 0x7b: Seek Command                  */
    Seek,                     /* 0x7c: Seek Command                  */
    Seek,                     /* 0x7d: Seek Command                  */
    Seek,                     /* 0x7e: Seek Command                  */
    Seek,                     /* 0x7f: Seek Command                  */
    NotImplement,             /* 0x80: not implemented               */
    NotImplement,             /* 0x81: not implemented               */
    NotImplement,             /* 0x82: not implemented               */
    NotImplement,             /* 0x83: not implemented               */
    NotImplement,             /* 0x84: not implemented               */
    NotImplement,             /* 0x85: not implemented               */
    NotImplement,             /* 0x86: not implemented               */
    NotImplement,              /* 0x87: reserved for CFA        */
    NotImplement,             /* 0x88: not implemented               */ // by Dragonslee  20080410
    NotImplement,             /* 0x89: not implemented               */
    NotImplement,             /* 0x8a: Preformat => change to            */
    NotImplement,             /* 0x8b: not implemented               */
    NotImplement,             /* 0x8c: not implemented               */
    NotImplement,             /* 0x8d: Recover                       */
    NotImplement,             /* 0x8e: TestRamAll                    */
    NotImplement,             /* 0x8f: WriteSpare                    */

    Diagnostics,              /* 0x90: Execute Drive Diagnostics     */
    InitDrvPara,              /* 0x91: Initialize Device Parameters  */

    Download_MicroCode,       /* 0x92: Download MicroCode            */
    Download_MicroCode,       /* 0x93: Download MicroCode DMA     */

    NotImplement,                  /* 0x94: not implemented            */
    NotImplement,                     /* 0x95: not implemented                */
    NotImplement,                  /* 0x96: not implemented                     */
    NotImplement,                     /* 0x97: not implemented                          */
    NotImplement,               /* 0x98: not implemented              */
    NotImplement,                    /* 0x99: not implemented                         */
    NotImplement,             /* 0x9a: not implemented               */
    NotImplement,             /* 0x9b: not implemented               */
    NotImplement,             /* 0x9c: not implemented               */
    NotImplement,             /* 0x9d: not implemented               */
    NotImplement,             /* 0x9e: not implemented               */
    NotImplement,             /* 0x9f: not implemented               */

    NotImplement,             /* 0xa0: ATAPI Packet                  */
    NotImplement,             /* 0xa1: ATAPI Identify Packet Device  */
    NotImplement,             /* 0xa2: ATAPI Service                 */
    NotImplement,             /* 0xa3: not implemented               */
    NotImplement,             /* 0xa4: not implemented               */
    NotImplement,             /* 0xa5: not implemented               */
    NotImplement,             /* 0xa6: not implemented               */
    NotImplement,             /* 0xa7: not implemented               */
    NotImplement,             /* 0xa8: not implemented               */
    NotImplement,             /* 0xa9: not implemented               */
    NotImplement,             /* 0xaa: not implemented               */
    NotImplement,             /* 0xab: not implemented               */
    NotImplement,             /* 0xac: not implemented               */
    NotImplement,             /* 0xad: not implemented               */
    NotImplement,             /* 0xae: not implemented               */
    NotImplement,             /* 0xaf: not implemented               */

    SMART,                    /* 0xb0: SMART		             */

    DeviceConfig,             /* 0xb1: DCO                           */

    NotImplement,             /* 0xb2: not implemented               */
    NotImplement,             /* 0xb3: not implemented               */
    Sanitize,             /* 0xb4: Sanitize              */
    NotImplement,             /* 0xb5: not implemented               */
    NotImplement,             /* 0xb6: not implemented               */
    NotImplement,             /* 0xb7: not implemented               */
    NotImplement,             /* 0xb8: not implemented               */
    NotImplement,             /* 0xb9: not implemented               */
    NotImplement,             /* 0xba: not implemented               */
    NotImplement,             /* 0xbb: not implemented               */
    NotImplement,             /* 0xbc: not implemented               */
    NotImplement,             /* 0xbd: not implemented               */
    NotImplement,             /* 0xbe: not implemented               */
    NotImplement,             /* 0xbf: not implemented               */

    //EraseSectors,             /* 0xc0: Erase Sector(s) - CF          */ // by Dragonslee  20080410
    NotImplement,             /* 0xc0: Erase Sector(s) - CF          */
    NotImplement,             /* 0xc1: not implemented               */
    NotImplement,             /* 0xc2: not implemented               */
    NotImplement,             /* 0xc3: not implemented               */
    ReadMultiple,             /* 0xc4: Read Multiple    [10.21]      */
    WriteMultiple,            /* 0xc5: Write Multiple   [10.21]      */
    SetMultiple,              /* 0xc6: Set Multiple     [10.21]      */
    NotImplement,             /* 0xc7: Read DMA Queued (ATA-4)       */
    
#ifdef _BURNER_MODE_    
    SDR_RW_DMA,       
#else    
    ReadSectors,              /* 0xc8: Read DMA w/ Retry             */
#endif    

    ReadSectors,              /* 0xc9: Read DMA w/o Retry            */
    
#ifdef _BURNER_MODE_    
    SDR_RW_DMA,
#else
    WriteSectors,             /* 0xca: Write DMA w/ Retry            */
#endif    

    WriteSectors,             /* 0xcb: Write DMA w/o Retry           */
    NotImplement,             /* 0xcc: Write DMA Queued (ATA-4)      */
    NotImplement,            /* 0xcd: reserved for CFA */  // by Dragonslee  20080410

    WriteSectorFUA,           /* 0xce: Write Multiple FUA EXT        */
 
    NotImplement,             /* 0xcf: not implemented               */

    NotImplement,             /* 0xd0: not implemented               */
    NotImplement,             /* 0xd1: not implemented               */
    NotImplement,             /* 0xd2: not implemented               */
    NotImplement,             /* 0xd3: not implemented               */
    NotImplement,             /* 0xd4: not implemented               */
    NotImplement,             /* 0xd5: not implemented               */
    NotImplement,             /* 0xd6: not implemented               */
    NotImplement,             /* 0xd7: not implemented               */
    NotImplement,             /* 0xd8: not implemented               */
    NotImplement,             /* 0xd9: not implemented               */
    NotImplement,             /* 0xda: Get Media Status (ATA-4)      */
    NotImplement,             /* 0xdb: not implemented               */
    NotImplement,             /* 0xdc: not implemented               */
    NotImplement,             /* 0xdd: not implemented               */
    NotImplement,             /* 0xde: Media Lock                    */
    NotImplement,             /* 0xdf: Media Unlock                  */

    Standby,                  /* 0xe0: Standby Immediate             */
    Idle,                     /* 0xe1: Idle Immediate                */
    Standby,                  /* 0xe2: Standby                       */
    Idle,                     /* 0xe3: Idle                          */
    ReadBuffer,               /* 0xe4: Read Buffer                   */
    ChkPwrMode,               /* 0xe5: Check Power Mode              */
    Sleep,                    /* 0xe6: Sleep                         */
    FlushCacheCmd,               /* 0xe7: Flush Cache (ATA-4)           */
    WriteBuffer,              /* 0xe8: Write Buffer                  */
    ReadBuffer,             /* 0xe9: READ BUFFER DMA               */

    FlushCacheCmd,               /* 0xea: Flush Cache EXT               */

    WriteBuffer,             /* 0xeb: WRITE BUFFER DMA               */
    AtaIdentifyDrive,         /* 0xec: Identify Device               */
    NotImplement,             /* 0xed: Media Eject                   */
    NotImplement,             /* 0xee: Identify Device DMA           */
    SetFeature,               /* 0xef: Set Features                  */
    NotImplement,             /* 0xf0: not implemented               */
    Security,                 /* 0xf1: Security Set Password         */
    Security,                 /* 0xf2: Security Unlock               */
    Security,                 /* 0xf3: Security Erase Prepare        */
    Security,                 /* 0xf4: Security Erase Unit           */
    Security,                 /* 0xf5: Security Freeze Lock          */
    Security,                 /* 0xf6: Security Disable Password     */


    NotImplement,             /* 0xf7: Security Freeze               */

    ReadNativeMax,            /* 0xf8: Read Native Max Address       */ //S5
    SetMax,                  /* 0xf9: Set MAX               */  //S5
    NotImplement,             /* 0xfa: not implemented               */
    NotImplement,             /* 0xfb: Read Native MAX Address(ATA-4)*/
    NotImplement,	      /* 0xfc: CF Preformat or SM MS Card Pretest*/
    NotImplement,             /* 0xfd: Mark specific Logic address bad */
    NotImplement,             /* 0xfe: Self Test Program             */
    NotImplement,             /* 0xff: not implemented               */
};

#endif


#endif

