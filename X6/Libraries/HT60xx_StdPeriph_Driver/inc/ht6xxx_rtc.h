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
* File         : ht6xxx_rtc.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_RTC_H__
#define __HT6XXX_RTC_H__

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
* @brief  RTC Timer����
*/
typedef enum
{ 
    RTCTMR1 = 0x20,                /*!< RTC��ʱ��1                        */
    RTCTMR2 = 0x40,                /*!< RTC��ʱ��2                        */
}RTCTMR_SelectTypeDef;             /*!< end of group RTCTMR_SelectTypeDef */   

/* 
* @brief  RTC Timer��ʼ������
*/
typedef struct
{
    FunctionalState NewState;      /*!< ��ʱ��״̬���ã�ʹ��or�ر�        */
    uint16_t  Count;               /*!< ��ʱ����ʱ��������                */ 
}RTCTMR_InitTypeDef;               /*!< end of group RTCTMR_InitTypeDef   */

/* 
* @brief  Tout�������
*/
typedef enum
{ 
    ToutLOW    = RTC_RTCCON_TOUT_LOW, 
    ToutHigh   = RTC_RTCCON_TOUT_HIGH,  
    ToutLF     = RTC_RTCCON_TOUT_LF,    
    Tout1Hz    = RTC_RTCCON_TOUT_1HZ,   
    Tout2Hz    = RTC_RTCCON_TOUT_2HZ,
    Tout4Hz    = RTC_RTCCON_TOUT_4HZ,
    Tout8Hz    = RTC_RTCCON_TOUT_8HZ,
#if !defined  HT502x
    Tout16Hz   = RTC_RTCCON_TOUT_16HZ,
    Tout32Hz   = RTC_RTCCON_TOUT_32HZ,
    Tout64Hz   = RTC_RTCCON_TOUT_64HZ,
    Tout128Hz  = RTC_RTCCON_TOUT_128HZ,
#endif
}RTCTout_TypeDef;                  /*!< end of group RTCTout_TypeDef  */  

/* 
* @brief  �������ö���
*/
typedef union
{ 
    struct
    {
        uint8_t minute;            /*!< ���ӼĴ����з�������          */
        uint8_t hour;              /*!< ���ӼĴ�����Сʱ����          */
    }Muster;                                                              
    uint16_t HourAndMinute;                                               
}RTCAlarm_TypeDef;                 /*!< end of group RTCAlarm_TypeDef */  

/* 
* @brief  RTCģ���ʼ���ṹ����
*/
typedef struct
{
    RTCTMR_InitTypeDef RTC_TMR1;   /*!< RTC��ʱ��1����                */                                                                          
    RTCTMR_InitTypeDef RTC_TMR2;   /*!< RTC��ʱ��2����                */                                                                         
    RTCAlarm_TypeDef RTC_Alarm;    /*!< RTC��������                   */                                                                             
    RTCTout_TypeDef RTC_TOUT;      /*!< RTC Tout�������              */                                                                        
}RTC_InitTypeDef;                  /*!< end of group RTC_InitTypeDef  */


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
void HT_RTC_Init(RTC_InitTypeDef* RTC_InitStruct);
void HT_RTC_Read(void* pRTCTime);
void HT_RTC_Write(void* pRTCTime);
void HT_RTC_TMRConfig(RTCTMR_SelectTypeDef RTCTMRx, RTCTMR_InitTypeDef* RTCTMR_InitStruct);
void HT_RTC_ToutSet(RTCTout_TypeDef RTCToutSet);
void HT_RTC_AlarmSet(RTCAlarm_TypeDef RTCAlarmSet);
void HT_RTC_ITConfig(uint8_t ITEn, FunctionalState NewState);
ITStatus HT_RTC_ITFlagStatusGet(uint8_t ITFlag);
void HT_RTC_ClearITPendingBit(uint8_t ITFlag);
Bool HT_RTC_LoadInfoData(void);






#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_RTC_H__ */ 
