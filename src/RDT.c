
#include "global/_global.h"
#include "Setup.h"
#include "Vardef.h"
#include "FTL.h"
#include "RDT.h"
#include "RDTVarDef.h"
#if !BURNER
//volatile UBYTE gubTaskCurrrentStatus;
//volatile U32 gulSDRLoopTimes = 20;
//UBYTE gubBasiCMDLoopNumber = 1;
//volatile UBYTE gubFlaTestType = 0;
//volatile UBYTE gubFlashModeSet = 0;
//volatile UBYTE gubFlashClockSet = 0;
//volatile UBYTE gubInfoPageFound = 0;
//volatile U32 guwECCthreshold = 200;
//volatile U32 gulFlaWaitTime = 25000;
//volatile UBYTE gubWriteErrorLog = 0;
//volatile UBYTE gubWriteSDRLog = 0;
#endif
#if ENABLE_USE_DBT_CHECK_RETEST==1
UWORD const END_LOOP_BAD_BLK_THRESHOLD = 1024;
#elif ENABLE_USE_DBT_CHECK_RETEST==0
UWORD const END_LOOP_BAD_BLK_THRESHOLD = 1024;
#endif

/////////////////////
extern UWORD SerialTable[10];
/////////////////////
void RTT_ISR(void)
{
	/*insert the code*/
	volatile unsigned long RTT_interrupt_event = 0;
	RTT_interrupt_event = ((dINTERRUPT(INT_STS1)) & (dINTERRUPT(INT_EN1)));
	if (gubEraseLightTwinkle) {
		if ((gulOperationTime % 1000) == 0) {
			if ( (*((volatile unsigned long*)0x04000118)&BIT2) == 0) {
				*((volatile unsigned long*)0x04000118) |= BIT2;
				*((volatile unsigned long*)0x04000118) &= CLR_BIT1;
				//gublighton=1;
			}
			else {
				*((volatile unsigned long*)0x04000118) |= BIT1;
				*((volatile unsigned long*)0x04000118) &= CLR_BIT2;

				//gublighton=0;

			}
		}
	}
	if (RTT_interrupt_event & RTT1_INT_1) { //rtt1 interrupt status
		dINTERRUPT(INT_STS1) |= RTT1_INT_1;
		gulOperationTime ++;
#if ENABLE_PFAIL
		gulPfail_time++;
		/*
		UartLongHex(gulPfail_time);
		if (gulPfail_time == 10000) {
		    UartString("\n");
		}
		else {
		    UartString(" ");
		}
		*/
		// unlock buffer windows after false signal
		if ((PFAIL_STATE_LOCKED == gubPfail_state) && (gulPfail_time >= 250)) {
			gubPfail_state = PFAIL_STATE_PRE_RELEASE;
			gBQI.ubWrite_TQ_lock &= (~POWER_FAIL_TQ_LOCK);
			gubAbort_CMD = 0;
#if ENABLE_ULTRAMLC_PFAIL_SIM
			//UartString("\nUL:");
			//UartLongHex(gulLastWriteCmdsCount);
#if (DEBUG_ULTRAMLC_POWER_FAIL_BG_COPY && DEBUG_ULTRAMLC_POWER_FAIL)
			_DEBUG_TRACE((0xE0 << 24) | (gulLastWriteCmdsCount & 0x0FFFFFFF));
#endif
#endif
		}

#if ENABLE_ULTRAMLC_PFAIL_SIM

		/*
		*
		* pfail +-------------------------------------- 10s ------------------------------------+
		*      Lock                                                                            Lock
		*       +------100ms------+
		*                        unlock
		*/
		// false signal simulation, will be replaced by gpio interrupt
		if ((gulPfail_time >= 10000) /*&& (gubTimeToDoBG==0)*/) { //  every 10 seconds
			gulPfail_time = 0;

			if (gulWriteCmdsCount > gulLastWriteCmdsCount) {
				gulLastWriteCmdsCount = gulWriteCmdsCount;
				gubPfail_state = PFAIL_STATE_PRE_LOCK;
				gBQI.ubWrite_TQ_lock |= POWER_FAIL_TQ_LOCK;
				//UartString("\nL:");
				//UartLongHex(gulLastWriteCmdsCount);
#if (DEBUG_ULTRAMLC_POWER_FAIL_BG_COPY && DEBUG_ULTRAMLC_POWER_FAIL)
				_DEBUG_TRACE((0xA0 << 24) | (gulLastWriteCmdsCount & 0x0FFFFFFF));
#endif
#if ENABLE_ULTRAMLC_PFAIL_MEASURE
				SYS1B[SYS1B_RTT2_CTRL] |= SYS_CNT_CLEAR;
#endif
			}
		}
#endif
#endif
	}
	else if (RTT_interrupt_event & RTT1_INT_2) { //rtt2 interrupt status

		dINTERRUPT(INT_STS1) |= RTT1_INT_2;
		//Other invalid events
	}
	else { //RTT0
		dINTERRUPT(INT_STS1) |= RTT0_INT;
		while ((dINTERRUPT(INT_STS1)&RTT0_INT) != 0x0) {} ///Wait while W1C done
		//	idle(100); //取代上面這行因為上面這行有時會卡住
	}
}
