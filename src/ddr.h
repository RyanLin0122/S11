#ifndef __DDR_H__
#define __DDR_H__



#define SIMPLE		0
#define FULL		1

#define RTT1_GetValue()      * ( (volatile unsigned long*) 0x04002620)
//#if BURNER
#if(!RDT)
#define bRTT(index)           * ( (volatile unsigned char*) 0x04002600+(  index              ))
#endif
//#define RTT1_Stop()			(bRTT(RTT1_CTRL) = (bRTT(RTT1_CTRL) & (~SYS_RTTEN)) | SYS_CNT_CLEAR);
//#define RTT1_Start()			(bRTT(RTT1_CTRL) |= SYS_RTTEN);

//=============================
// DDR Phy cfg
//=============================
#define SDR        0x03
#define DDR3_400   0x00
#define DDR3_533   0x01
#define MRAM       0x02

#define SIZE_2MB   0x00
#define SIZE_8MB   0x01
#define SIZE_16MB  0x02
#define SIZE_32MB  0x03
#define SIZE_128MB 0x04
#define SIZE_256MB 0x05
#define SIZE_512MB 0x06

#define GPIO_LH           		0x04

////////////////////// ECC and E3D Control Rgister
#define wMISCL(index)         * ( (volatile unsigned short*) 0x04000000+(  index>>1           ))

#define dCLK(index)           * ( (volatile unsigned long*) 0x04000400+(  index>>2           ))

//DDR Controller Register
//#define bDDR(index)           * ( (volatile unsigned char*) 0x04005000+(  index              ))
//#define wDDR(index)           * ( (volatile unsigned short*) 0x04005000+(  index>>1           ))
//#define dDDR(index)           * ( (volatile unsigned long*) 0x04005000+(  index>>2           ))
//#define qDDR(index)           * ( (volatile unsigned long long*) 0x04005000+(  index>>2           ))

#define bMISCH(index)         * ( (volatile unsigned char*) 0x04003000+(  index              ))
#define wMISCH(index)         * ( (volatile unsigned short*) 0x04003000+(  index>>1           ))
#if(!RDT)
#define dMISCH(index)         * ( (volatile unsigned long*) 0x04003000+(  index>>2           ))
#endif

//#define bSYS0(index)           * ( (volatile unsigned char*) 0x04000000+(  index              ))


#define ECC_EN			0x30

#define ZONE1_ADR		0x00
#define ZONE1_LEN		0x04
#define ZONE2_ADR		0x08
#define ZONE2_LEN		0x0C
#define ZONE3_ADR		0x10
#define ZONE3_LEN		0x14

#define CR_DDRP_CKEN        BIT23
#define DRAM_ADR			0x28
#define DRAM_LEN			0x2C
#if(!RDT)
#define RET_TIMEOUT   0xff
#define TO_DDR        8500
#define CR_E3D_ENABLE		(BIT7)
#define CR_E3D_DISABLE		(~BIT7)
#endif
#define Boot_Enable_ECC		BIT11
#define CLK_EN_CTRL                  0x14
#define ECC_EN			0x30


#define RET_OK            0
#define RET_ERR           1


#define BIST_INCR 0
#define BIST_Len_0_3_15_63      1
//#define RW_PERIOD_ADJUST        _bit24
#define BIST_Data_Sequential 0

#define RTT1_CTRL                 (0x00)
//	    #define SYS_RTTAURST           _bit6        //RTT auto reset               0:auto reset,   1:stop   	(0)
   // 	#define SYS_RTTEN              _bit7        //enable RTT timer             0:Disable,      1:Enable 	(0)
	//	#define SYS_CNT_CLEAR          _bit4        //counter clear
      //  #define SYS_RTT_SCALE          _bit0        //scale =0: 33.33ns ; 1=100ns; 2=1us; 3=1ms

//#endif


extern void idle (unsigned long len);
extern unsigned char ScanDelayLine_DDR3(void);
extern unsigned char CheckData_DDR3(unsigned long* uladdress, unsigned long ulZ1size);
extern unsigned char CheckData_SDR(unsigned long* uladdress, unsigned long ulZ1size);
extern unsigned char ScanDelayLine_SDR(void);
extern void Auto_Refresh(unsigned char ddr_type);
extern unsigned char Scan_bank(void);
//extern unsigned char entry_selfrefresh (void);
//extern unsigned char exit_selfrefresh (void);
extern unsigned char Scan_size(unsigned char ddr_type);
extern unsigned char ddr_controller_init(unsigned char ddr_type, unsigned char ddr_size, unsigned char ddr_bank, unsigned char ubSetAgain);
extern unsigned char ddr_phy_init(unsigned char ddr_type);
extern unsigned char MRS_Setting(unsigned char ddr_type, unsigned char ddr_size);
extern void Set_DDR3_DQS_G(unsigned char Index_Gate_DQS0, unsigned char Index_Gate_DQS1);
extern void Set_DDR3_DQS_Fine(unsigned short DQS0_fine, unsigned short DQS1_fine);
extern unsigned char DRAM_init(void);
extern unsigned char exit_retention(unsigned char ddr_type, unsigned char ddr_size, unsigned char ddr_bank);
extern void entry_retention(void);

//extern unsigned char CheckData(unsigned long* uladdress,unsigned long ulZ1size);
#endif

