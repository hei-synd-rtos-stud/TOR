//////////////////////////////////////////////////////////////////////////////////
/// \file time_sender.c
/// \brief Time sender thread
/// \author Pascal Sartoretti (sap at hevs dot ch)
/// \version 1.0 - original
/// \date  2018-02
//////////////////////////////////////////////////////////////////////////////////
#include "stm32f7xx_hal.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "rtx_os.h"
extern void    *osRtxMemoryAlloc(void *mem, uint32_t size, uint32_t type);
extern uint32_t osRtxMemoryFree (void *mem, void *block);

//////////////////////////////////////////////////////////////////////////////////
// THREAD TIME SENDER
//////////////////////////////////////////////////////////////////////////////////
void TimeSender(void *argument)
{
	struct queueMsg_t queueMsg;				// queue message
	char * stringPtr;									// string to send pointer
	time_t    seconds;								// current time in seconds
	struct tm timeStr;								// current time in structure
	struct tm * ptrTm;								// pointer to it
	int32_t eventFlag;								// event flag for check
	osStatus_t retCode;								// return code error
	//------------------------------------------------------------------------------
	// initial time and date set
	//------------------------------------------------------------------------------
	timeStr.tm_hour = 12;
	timeStr.tm_min = 00;
	timeStr.tm_sec = 00;
	timeStr.tm_year = 2025 - 1900;		// years since 1900
	timeStr.tm_mon = 5;								// month (0-11) 6-> june
	timeStr.tm_mday = 1;
	seconds = mktime(&timeStr);

	//------------------------------------------------------------------------------
	for (;;)															// loop until doomsday
	{
		osDelay(1000);											// wake-up each second
		seconds++;													// increment seconds
		//----------------------------------------------------------------------------
		// EVENT FLAG WAIT for checked box without to clear it and no wait
		//----------------------------------------------------------------------------
		eventFlag = osEventFlagsWait(
			eventFlag_id,
			BROADCAST_TIME_EVT,
			osFlagsWaitAny | osFlagsNoClear,
			0);

		if((eventFlag & BROADCAST_TIME_EVT) == BROADCAST_TIME_EVT)	// want to send time ?
		{
			ptrTm = localtime(&seconds);

			stringPtr = osMemoryPoolAlloc(memPool,osWaitForever);
			queueMsg.type = DATA_IND;											// prepare message
			queueMsg.anyPtr = stringPtr;
			queueMsg.sapi = TIME_SAPI;
			queueMsg.addr = BROADCAST_ADDRESS;						// of type broadcast
			sprintf((char*)stringPtr," %02d:%02d:%02d  ",
				ptrTm->tm_hour,ptrTm->tm_min,ptrTm->tm_sec);
			//------------------------------------------------------------------------
			// QUEUE SEND
			//------------------------------------------------------------------------
			retCode = osMessageQueuePut(
				queue_macS_id,
				&queueMsg,
				osPriorityNormal,
				osWaitForever);
			CheckRetCode(retCode,__LINE__,__FILE__,CONTINUE);
		}
	}
}
