/*
**********************************************************************************************************
*                                              HT60XX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : ht60xx_it.h
* By           : SocTeam
* Version      : V0.4
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT60xx_IT_H__
#define __HT60xx_IT_H__

#ifdef __cplusplus
extern "C" {
#endif 
	
#include "include.h"	
    
/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             全局变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           全局函数申明
*********************************************************************************************************
*/

void PMU_IRQHandler(void);
void DES_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI5_IRQHandler(void);
void EXTI6_IRQHandler(void);
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void UART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void TIMER_0_IRQHandler(void);
void TIMER_1_IRQHandler(void);
void TIMER_2_IRQHandler(void);
void TIMER_3_IRQHandler(void);
void TBS_IRQHandler(void);
void RTC_IRQHandler(void);
void I2C_IRQHandler(void);
void SPI0_IRQHandler(void);
void SPI1_IRQHandler(void);
void SelfTestF_IRQHandler(void);
void TIMER_4_IRQHandler(void);
void TIMER_5_IRQHandler(void);
void UART6_IRQHandler(void);
void EXTI7_IRQHandler(void);
void EXTI8_IRQHandler(void);
void EXTI9_IRQHandler(void);
void DMA_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __HT60xx_IT_H__ */ 






