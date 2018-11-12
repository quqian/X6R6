/*CpuCfg.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __SYSTICK_H__
#define __SYSTICK_H__


#include "ht6xxx.h"



void SysTick_Init(void);
uint32_t  OSTimeGet (void);
void OSTimeDly(uint32_t tick);

#endif
