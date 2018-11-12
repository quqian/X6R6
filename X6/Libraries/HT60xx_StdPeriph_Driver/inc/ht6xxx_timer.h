/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_timer.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_TIMER_H__
#define __HT6XXX_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
/*
*********************************************************************************************************
*                                           ȫ�ֺ�/�ṹ��
*********************************************************************************************************
*/

/* 
* @brief  Timer����ģʽ����
*/
typedef enum
{ 
    TIMPWMCountUpLevelHigh = 0x02,               /*!< PWM���ϼ�����ʽ����ʼ��ƽ��    */
    TIMPWMCountDownLevelHigh = 0x12,             /*!< PWM���¼�����ʽ����ʼ��ƽ��    */
    TIMPWMCenterAlignedLevelHigh = 0x22,         /*!< PWM������������ʽ����ʼ��ƽ��*/
    TIMPWMCountUpLevelLow = 0x42,                /*!< PWM���ϼ�����ʽ����ʼ��ƽ��    */
    TIMPWMCountDownLevelLow	= 0x52,              /*!< PWM���¼�����ʽ����ʼ��ƽ��    */
    TIMPWMCenterAlignedLevelLow = 0x62,          /*!< PWM������������ʽ����ʼ��ƽ��*/
    TIMCaptureRiseEdge = 0x04,                   /*!< �����ز���ʽ                 */
    TIMCaptureFallEdge = 0x0c,                   /*!< �����ز���ʽ                 */
#if defined HT6x2x
    TIMEventRecordRiseEdge = 0x00,               /*!< �������¼���¼                 */
    TIMEventRecordFallEdge = 0x08,               /*!< �������¼���¼                 */    
#endif
    TIMCycleTiming = 0x06,                       /*!< ��ʱ�����ڶ�ʱ                 */
}TMRMode_TypeDef;                                /*!< end of group TMRMode_TypeDef   */   

/* 
* @brief  TimerExt����ģʽ����     only for HT6x2x  Timer4 & Timer5  
*/
#if defined HT6x2x
typedef enum
{ 
    TIMExtPWMCountUpLevelHigh = 0x02,            /*!< PWM���ϼ�����ʽ����ʼ��ƽ��    */
    TIMExtPWMCountDownLevelHigh = 0x12,          /*!< PWM���¼�����ʽ����ʼ��ƽ��    */
    TIMExtPWMCenterAlignedLevelHigh = 0x22,      /*!< PWM������������ʽ����ʼ��ƽ��*/
    TIMExtPWMCountUpLevelLow = 0x42,             /*!< PWM���ϼ�����ʽ����ʼ��ƽ��    */
    TIMExtPWMCountDownLevelLow	= 0x52,          /*!< PWM���¼�����ʽ����ʼ��ƽ��    */
    TIMExtPWMCenterAlignedLevelLow = 0x62,       /*!< PWM������������ʽ����ʼ��ƽ��*/
    TIMExtCycleTiming = 0x06,                    /*!< ��ʱ�����ڶ�ʱ                 */
}TMRExtMode_TypeDef;                             /*!< end of group TMRExtMode_TypeDef*/ 

typedef enum
{ 
    TIMExtClk_LRC = TMR_TMRCON_CLKSEL_LRC,       /*!< Timer Clkʱ��ԴΪLRC                */
    TIMExtClk_LF  = TMR_TMRCON_CLKSEL_LF,        /*!< Timer Clkʱ��ԴΪLF                 */
    TIMExtClk_HRC = TMR_TMRCON_CLKSEL_HRC,       /*!< Timer Clkʱ��ԴΪHRC                */
    TIMExtClk_PLL = TMR_TMRCON_CLKSEL_PLL,       /*!< Timer Clkʱ��ԴΪPLL                */    
    TIMExtClk_MEMS= TMR_TMRCON_CLKSEL_MEMS,      /*!< Timer Clkʱ��ԴΪMEMS               */     
}TMRExtClkSource_TypeDef;                        /*!< end of group TMRExtClkSource_TypeDef*/ 
#endif

/* 
* @brief  Timerģ���ʼ���ṹ����
*/
typedef struct
{
    TMRMode_TypeDef TimerMode;                   /*!< ��ʱ������ģʽ                 */
    uint16_t TimerPreDiv;                        /*!< ��ʱ��ʱ��Ԥ��Ƶ����           */ 
    uint16_t TimerPeriod;                        /*!< ��ʱ����ʱ����                 */
    uint16_t TimerCompare;                       /*!< ��ʱ���ȽϼĴ���               */                      
}TMR_InitTypeDef;                                /*!< end of group TMR_InitTypeDef   */

/* 
* @brief  TimerExtģ���ʼ���ṹ����
*/
#if defined HT6x2x
typedef struct
{
    TMRExtClkSource_TypeDef TimerSource;         /*!< ��ʱ��ʱ��Դѡ��               */
    TMRExtMode_TypeDef TimerMode;                /*!< ��ʱ������ģʽ                 */
    uint16_t TimerPreDiv;                        /*!< ��ʱ��ʱ��Ԥ��Ƶ����           */ 
    uint16_t TimerPeriod;                        /*!< ��ʱ����ʱ����                 */     
}TMRExt_InitTypeDef;                             /*!< end of group TMRExt_InitTypeDef*/
#endif

/*
*********************************************************************************************************
*                                             ȫ�ֱ���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           ȫ�ֺ�������
*********************************************************************************************************
*/

void HT_TMR_Init(HT_TMR_TypeDef* TMRx, TMR_InitTypeDef* TMR_InitStruct);
#if defined HT6x2x
void HT_TMRExt_Init(HT_TMR_TypeDef* TMRx, TMRExt_InitTypeDef* TMR_InitStruct); /*only for HT6x2x Timer4&Timer5 */
#endif
uint16_t HT_TMR_CaptureGet(HT_TMR_TypeDef* TMRx);
uint16_t HT_TMR_CountGet(HT_TMR_TypeDef* TMRx);
void HT_TMR_ITConfig(HT_TMR_TypeDef* TMRx, uint8_t ITEn, FunctionalState NewState);
ITStatus HT_TMR_ITFlagStatusGet(HT_TMR_TypeDef* TMRx, uint8_t ITFlag);
void HT_TMR_ClearITPendingBit(HT_TMR_TypeDef* TMRx, uint8_t ITFlag);




#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_TIMER_H__ */ 
