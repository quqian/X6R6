/*cmu.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __CMU_H__
#define __CMU_H__

#include "includes.h"





void SwitchTo_Flf(void);
void SwitchTo_Fhrc(void);
void SwitchTo_Fpll(void);
void Close_Hrc(void);
void Close_Pll(void);

void HT_CMU_ClkCtrl0Config(uint32_t ClkCtrl0Module, uint8_t NewState);
void HT_CMU_ClkCtrl1Config(uint32_t ClkCtrl1Module, FunctionalState NewState);












#endif //__CMU_H__






