#ifndef __RTC_H__
#define __RTC_H__

#include "includes.h"
#include "time.h"

void RTC_Init(void);
void SetRtcCount(time_t timestamp);
char* GetCurrentTime(void);
time_t GetTimeStamp(void);
/*****************************************************************************
** Function name:       GetRtcCount
** Descriptions:        //���ʱ��
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern inline uint32_t GetRtcCount(void);
/*****************************************************************************
** Function name:       RtcInitTime
** Descriptions:        //��ʼ��
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern int RtcInitTime(void);

#endif

