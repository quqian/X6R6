/*rtc.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __RTC_H__
#define __RTC_H__

#include "includes.h"


#define	C_Toff		0x0000									//温度偏置寄存器
#define	C_MCON01	0x2000									//控制系数01
#define	C_MCON23	0x0588									//控制系数23
#define	C_MCON45	0x4488									//控制系数45



void SetRtcCount(uint32_t tick);
uint32_t GetRtcCount(void);
void GetRtcTime(void* pRTCTime);
int RtcInit(void);
char* GetCurrentTime(void);
uint32_t GetTimeStamp(void);


extern __IO uint32_t gSysSecond;

#endif //__RTC_H__


