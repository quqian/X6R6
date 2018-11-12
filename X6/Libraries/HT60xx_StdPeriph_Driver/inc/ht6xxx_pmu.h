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
* File         : ht6xxx_pmu.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_PMU_H__
#define __HT6XXX_PMU_H__

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
* @brief  LVDIN״̬����
*/
typedef enum
{ 
    LVDINEnable = 0x04,                   /*!< LVDINʹ��                        */
    LVDINDisable = 0x00,                  /*!< LVDIN�ر�                        */
}LVDINState_TypeDef;                      /*!< end of group LVDIN_StateTypeDef  */   

/* 
* @brief  BOR״̬����
*/
typedef enum
{ 
    BORDisable = 0x00,                    /*!< BOR�ر�                          */
    BORReset = 0x03,                      /*!< BORʹ�ܣ�������λ                */
    BORInterrupt = 0x01,                  /*!< BORʹ�ܣ������ж�                */
}BORState_TypeDef;                        /*!< end of group BOR_StateTypeDef    */

/* 
* @brief  VCC��ֵ����
*/
typedef enum
{ 
    VccLVL_2V4 = PMU_VDETCFG_VCCLVL_2V4,  /*!< Vcc�����ֵ = 2.4V               */
    VccLVL_2V6 = PMU_VDETCFG_VCCLVL_2V6,  /*!< Vcc�����ֵ = 2.6V               */
    VccLVL_2V8 = PMU_VDETCFG_VCCLVL_2V8,  /*!< Vcc�����ֵ = 2.8V               */
    VccLVL_3V0 = PMU_VDETCFG_VCCLVL_3V0,  /*!< Vcc�����ֵ = 3.0V               */
#if  defined  HT6x1x ||  defined  HT6x2x    
    VccLVL_3V2 = PMU_VDETCFG_VCCLVL_3V2,  /*!< Vcc�����ֵ = 3.2V               */
    VccLVL_3V4 = PMU_VDETCFG_VCCLVL_3V4,  /*!< Vcc�����ֵ = 3.4V               */
    VccLVL_3V6 = PMU_VDETCFG_VCCLVL_3V6,  /*!< Vcc�����ֵ = 3.6V               */
    VccLVL_3V8 = PMU_VDETCFG_VCCLVL_3V8,  /*!< Vcc�����ֵ = 3.8V               */
    VccLVL_4V0 = PMU_VDETCFG_VCCLVL_4V0,  /*!< Vcc�����ֵ = 4.0V               */
    VccLVL_4V2 = PMU_VDETCFG_VCCLVL_4V2,  /*!< Vcc�����ֵ = 4.2V               */    
    VccLVL_4V4 = PMU_VDETCFG_VCCLVL_4V4,  /*!< Vcc�����ֵ = 4.4V               */
    VccLVL_4V6 = PMU_VDETCFG_VCCLVL_4V6,  /*!< Vcc�����ֵ = 4.6V               */
    VccLVL_4V8 = PMU_VDETCFG_VCCLVL_4V8,  /*!< Vcc�����ֵ = 4.8V               */ 
    VccLVL_5V0 = PMU_VDETCFG_VCCLVL_5V0,  /*!< Vcc�����ֵ = 5.0V               */ 
#endif
}VCCLVL_TypeDef;                          /*!< end of group VCCLVL_TypeDef      */

/* 
* @brief  BOR��ֵ����
*/
typedef enum
{
#if !defined HT6x2x	
    BORLVL_2V2 = PMU_VDETCFG_BORLVL_2V2,  /*!< BOR�����ֵ = 2.2V               */
    BORLVL_2V4 = PMU_VDETCFG_BORLVL_2V4,  /*!< BOR�����ֵ = 2.4V               */
    BORLVL_2V6 = PMU_VDETCFG_BORLVL_2V6,  /*!< BOR�����ֵ = 2.6V               */
    BORLVL_2V8 = PMU_VDETCFG_BORLVL_2V8,  /*!< BOR�����ֵ = 2.8V               */ 
#else
    BORLVL_2V2 = PMU_VDETCFG_BORLVL_2V0,  /*!< BOR�����ֵ = 2.0V               */
    BORLVL_2V4 = PMU_VDETCFG_BORLVL_2V2,  /*!< BOR�����ֵ = 2.2V               */
    BORLVL_2V6 = PMU_VDETCFG_BORLVL_2V6,  /*!< BOR�����ֵ = 2.6V               */
    BORLVL_2V8 = PMU_VDETCFG_BORLVL_2V8,  /*!< BOR�����ֵ = 2.8V               */	
#endif
}BOTLVL_TypeDef;                          /*!< end of group BOTLVL_TypeDef      */

/* 
* @brief  VCC_DET,BOR_DET��ʱ���ʱ�䶨��
*/
typedef enum
{ 
    DETTIM_300uS = PMU_VDETPCFG_VDETTIME_300uS,  /*!< ��ʱ���ʱ�� = 300us        */
    DETTIM_360uS = PMU_VDETPCFG_VDETTIME_360uS,  /*!< ��ʱ���ʱ�� = 360us        */
    DETTIM_480uS = PMU_VDETPCFG_VDETTIME_480uS,  /*!< ��ʱ���ʱ�� = 480us        */
#if defined HT6x2x
    DETTIM_1120uS = PMU_VDETPCFG_VDETTIME_1120uS,  /*!< ��ʱ���ʱ�� = 1120us        */
#else
    DETTIM_560uS = PMU_VDETPCFG_VDETTIME_560uS,   /*!< ��ʱ���ʱ�� = 560us        */
#endif	
}DETTIM_TypeDef;                                 /*!< end of group DETTIM_TypeDef */

/* 
* @brief  VCC_DET,BOR_DET��ʱ������ڶ���
*/
typedef enum
{ 
    DETPRD_16P5mS = PMU_VDETPCFG_VDETPRD_16P5mS, /*!< ��ʱ������� = 16.5ms       */
    DETPRD_33mS   = PMU_VDETPCFG_VDETPRD_33mS,   /*!< ��ʱ������� = 33ms         */
    DETPRD_67mS   = PMU_VDETPCFG_VDETPRD_67mS,   /*!< ��ʱ������� = 67ms         */
    DETPRD_134mS  = PMU_VDETPCFG_VDETPRD_134mS,  /*!< ��ʱ������� = 134ms        */ 
    DETPRD_268mS  = PMU_VDETPCFG_VDETPRD_268mS,  /*!< ��ʱ������� = 268ms        */
    DETPRD_536mS  = PMU_VDETPCFG_VDETPRD_536mS,  /*!< ��ʱ������� = 536ms        */
    DETPRD_1072mS = PMU_VDETPCFG_VDETPRD_1072mS, /*!< ��ʱ������� = 1072ms       */
    DETPRD_2144mS = PMU_VDETPCFG_VDETPRD_2144mS, /*!< ��ʱ������� = 2144ms       */
}DETPRD_TypeDef;                                 /*!< end of group DETPRD_TypeDef */

/* 
* @brief  DISChargeģʽ����
*/
#if  defined  HT6x2x  ||  defined HT501x  ||  HT502x
typedef enum
{ 
    DisCharge0mA =0,                            /*!< �رյ�ضۻ�����              */
    DisCharge1mA =0x8000,                       /*!< ��ضۻ�ʹ�ܣ�����1mA         */
    DisCharge2mA =0x88000,                      /*!< ��ضۻ�ʹ�ܣ�����2mA         */
	  DisCharge50uA =0x48000,                     /*!< ��ضۻ�ʹ�ܣ�����50uA        */
	  DisCharge100uA =0xC8000,                    /*!< ��ضۻ�ʹ�ܣ�����100uA       */
}DISCharge_TypeDef;                             /*!< end of group DISCharge_TypeDef*/
#endif

/* 
* @brief  POW״̬����
*/
#if  defined HT6x2x
typedef enum
{ 
    POWEnable = 0x20,                   /*!< POW_DETʹ��                        */
    POWDisable = 0x00,                  /*!< POW_DET�ر�                        */
}POWState_TypeDef;                      /*!< end of group POWState_TypeDef			 */   
#endif

/* 
* @brief  PMUģ���ʼ���ṹ����
*/
typedef struct
{
    LVDINState_TypeDef Lvdin0State;              /*!< LVDIN0״̬����              */
#if  defined  HT501x ||  defined  HT6x2x  ||  defined  HT502x
    LVDINState_TypeDef Lvdin1State;              /*!< LVDIN1״̬����              */
#endif  
#if  defined  HT6x2x
	POWState_TypeDef POWState;				 	 /*!< POW״̬����   	           */		
	uint32_t	POWFilter;						 /*!< POW�����˲�����            */	
#endif	
    BORState_TypeDef BorState;                   /*!< BOR״̬����                 */
    VCCLVL_TypeDef VccLvlSet;                    /*!< Vcc�����ֵ����             */
    BOTLVL_TypeDef BorLvlSet;                    /*!< BOR�����ֵ����             */
    DETTIM_TypeDef VccBorDetTime;                /*!< Vcc��Bor���ʱ������        */
    DETPRD_TypeDef VccBorDetPeriod;              /*!< Vcc��Bor�����������        */                                                         
}PMU_InitTypeDef;                                /*!< end of group PMU_InitTypeDef*/


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
void HT_PMU_Init(PMU_InitTypeDef* PMU_InitStruct);
void HT_PMU_HoldLDOConfig(FunctionalState NewState);
FlagStatus HT_PMUState_FlagStatusGet(uint8_t STAFlag);
void HT_PMU_ITConfig(uint8_t ITEn, FunctionalState NewState);
ITStatus HT_PMU_ITFlagStatusGet(uint8_t ITFlag);
void HT_PMU_ClearITPendingBit(uint8_t ITFlag);
#if  defined  HT501x ||  defined  HT6x2x 
void HT_PMU_DisChargeConfig(DISCharge_TypeDef ChargeMode);
#endif



#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_PMU_H__ */ 
