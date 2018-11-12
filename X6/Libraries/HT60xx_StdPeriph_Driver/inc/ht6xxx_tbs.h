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
* File         : ht6xxx_tbs.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_TBS_H__
#define __HT6XXX_TBS_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
/*
*********************************************************************************************************
*                                           ȫ�ֺ�/�ṹ��
*********************************************************************************************************
*/

typedef enum
{
    TMPPRD  = TBS_TBSPRD_TMPPRD,         /*!< �¶�                                */
    VBATPRD = TBS_TBSPRD_VBATPRD,        /*!< ��ص�ѹ                            */
    ADC0PRD = TBS_TBSPRD_ADC0PRD,        /*!< ADC0                                */
    ADC1PRD = TBS_TBSPRD_ADC1PRD,        /*!< ADC1                                */
    VCCPRD  = TBS_TBSPRD_VCCPRD,         /*!< ��Դ��ѹ                            */
#if  defined  HT501x
    ADC2PRD = TBS_TBSPRD_ADC2PRD,        /*!< ADC2                                */
    VREFPRD = TBS_TBSPRD_VREFPRD,        /*!< Vref                                */
#endif    
}TBS_PeriodTypeDef;                      /*!< end of group TBS_PeriodTypeDef      */

typedef enum
{
    TBS_TMP  = 0,                        /*!< �¶�                                */
    TBS_VBAT ,                           /*!< ��ص�ѹ                            */
    TBS_ADC0 ,                           /*!< ADC0                                */
    TBS_ADC1 ,                           /*!< ADC1                                */
    TBS_VCC  ,                           /*!< ��Դ��ѹ                            */
#if  defined  HT501x
    TBS_ADC2 ,                           /*!< ADC2                                */
#endif    
}TBS_SubModuleTypeDef;                   /*!< end of group TBS_SubModuleTypeDef   */

typedef enum
{
    CMP_VBAT =0,                         /*!< ��ص�ѹ                            */
#if defined HT501x  ||  HT6x2x  ||  HT502x
    CMP_ADC0 ,                           /*!< ADC0                                */
#endif
#if  defined  HT501x  ||  HT502x
    CMP_ADC1 ,                           /*!< ADC1                                */
#endif    
}TBS_CMPRegTypeDef;                      /*!< end of group TBS_CMPRegTypeDef      */

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

void HT_TBSConfig(uint32_t TBS_SubModule, FunctionalState NewState);
void HT_TBS_PeriodSet(TBS_PeriodTypeDef PRDModule, uint32_t Period);
void HT_TBS_CMPThresholdSet(TBS_CMPRegTypeDef CmpReg, int16_t Threshold);
int16_t HT_TBS_ValueRead(TBS_SubModuleTypeDef SubModule);  
void HT_TBS_ITConfig(uint32_t ITEn, FunctionalState NewState);
ITStatus HT_TBS_ITFlagStatusGet(uint32_t ITFlag);
void HT_TBS_ClearITPendingBit(uint32_t ITFlag);





#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_TBS_H__ */ 
