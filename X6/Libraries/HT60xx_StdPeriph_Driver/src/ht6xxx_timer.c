/*
*********************************************************************************************************
*                                              HT6XXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_timer.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_TIMER_C

#include "ht6xxx_timer.h"

/*
*********************************************************************************************************
*                                           ���غ�/�ṹ��
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                             ���ر���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         INITIAL TIMER MODULE
*
* ����˵��: ��ʼ��TIMERģ��
*
* ��ڲ���: TMRx               ֻ����HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3��һ��
*
*           TMR_InitStruct     TIMER��ʼ���ṹ��ָ�룬��Ҫ����4������: 
*                              1) TimerMode    : ��ʱ������ģʽ
*                              2) TimerPeriod  : ��ʱ����ʱ��������
*                              3) TimerCompare : ��ʱ���ȽϼĴ�������  
*                              4) TimerPreDiv  : ��ʱ��Ԥ��Ƶ����                      
*
* ���ز���: ��                                      
* 
* ����˵��: �û�������TIMER�Ĵ���ǰӦ��ʹ��TIMERģ�飬����μ�HT_CMU_ClkCtrl1Config()����
*********************************************************************************************************
*/
void HT_TMR_Init(HT_TMR_TypeDef* TMRx, TMR_InitTypeDef* TMR_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg = 0x01;                           /*!< ��ʱ��ʹ��                  */
    
    TMRx->TMRDIV = TMR_InitStruct->TimerPreDiv;        /*!< ���ö�ʱ��Ԥ��Ƶ��          */

    TMRx->TMRPRD = TMR_InitStruct->TimerPeriod;        /*!< ���ö�ʱ�����ڼĴ���        */

    TMRx->TMRCMP = TMR_InitStruct->TimerCompare;       /*!< ���ö�ʱ���ȽϼĴ���        */  
    
    tempreg |= TMR_InitStruct->TimerMode;

    TMRx->TMRCON = tempreg;                            /*!< ���ö�ʱ������ģʽ          */

} 

/*
*********************************************************************************************************
*                                         INITIAL TIMEREXT MODULE
*
* ����˵��: ��ʼ��TIMERģ��
*
* ��ڲ���: TMRx               ֻ����HT_TMR4/HT_TMR5��һ��
*
*           TMR_InitStruct     TIMER��ʼ���ṹ��ָ�룬��Ҫ����4������: 
*                              1) TimerSource  : ��ʱ��ʱ��Դѡ��
*                              2) TimerMode    : ��ʱ������ģʽ
*                              3) TimerPeriod  : ��ʱ����ʱ��������
*                              4) TimerCompare : ��ʱ���ȽϼĴ�������  
*                              5) TimerPreDiv  : ��ʱ��Ԥ��Ƶ����                      
*
* ���ز���: ��                                      
* 
* ����˵��: �û�������TIMER�Ĵ���ǰӦ��ʹ��TIMERģ�飬����μ�HT_CMU_ClkCtrl1Config()����
*********************************************************************************************************
*/
#if defined HT6x2x
void HT_TMRExt_Init(HT_TMR_TypeDef* TMRx, TMRExt_InitTypeDef* TMR_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg = 0x01;                           /*!< ��ʱ��ʹ��                  */
    
    TMRx->TMRDIV = TMR_InitStruct->TimerPreDiv;        /*!< ���ö�ʱ��Ԥ��Ƶ��          */

    TMRx->TMRPRD = TMR_InitStruct->TimerPeriod;        /*!< ���ö�ʱ�����ڼĴ���        */
    
    tempreg |= (TMR_InitStruct->TimerMode|TMR_InitStruct->TimerSource);
    

    TMRx->TMRCON = tempreg;                            /*!< ���ö�ʱ������ģʽ          */

}
#endif

/*
*********************************************************************************************************
*                                         GET TIMER CAPTURE VALUE
*
* ����˵��: ��ȡ����Ĵ�������ֵ
*
* ��ڲ���: TMRx               ֻ����HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3��һ��
*                     
* ���ز���: ���ض�ʱ������ֵ                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
uint16_t HT_TMR_CaptureGet(HT_TMR_TypeDef* TMRx)
{
    /*  assert_param  */
    

    return  ((uint16_t)TMRx->TMRCAP);                /*!< ���ز���Ĵ�������ֵ        */
} 

/*
*********************************************************************************************************
*                                         GET TIMER COUNT VALUE
*
* ����˵��: ��ȡ��ʱ����ǰ��ʱֵ
*
* ��ڲ���: TMRx               ֻ����HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5��һ��
*                     
* ���ز���: ���ض�ʱ����ʱֵ                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
uint16_t HT_TMR_CountGet(HT_TMR_TypeDef* TMRx)
{
    /*  assert_param  */
    

    return  ((uint16_t)TMRx->TMRCNT);                /*!< ���ض�ʱ����ʱֵ         */
} 

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE TIMER INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�TIMER�ж�
*
* ��ڲ���: TMRx       ֻ����HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5��һ��
*
*           ITEn       TIMER�ж�����λ������Ϊ���²����������
*                        @arg TMR_TMRIE_PRDIE
*                        @arg TMR_TMRIE_CAPIE
*                        @arg TMR_TMRIE_CMPIE
*                        @arg TMR_TMRIE_ACIE    (only for HT6x2x)
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_TMR_ITConfig(HT_TMR_TypeDef* TMRx, uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        TMRx->TMRIE |= (uint32_t)ITEn;               /*!< ʹ��TIMER�ж�           */
    }
    else
    {
        TMRx->TMRIE &= ~(uint32_t)ITEn;              /*!< �ر�TIMER�ж�           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED TIMER INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦTIMER�жϱ�־״̬
*
* ��ڲ���: TMRx       ֻ����HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5��һ��
*
*           ITFlag     ��Ҫ����ĳ��TIMER�жϣ�����Ϊ���²���:
*                        @arg TMR_TMRIF_PRDIF
*                        @arg TMR_TMRIF_CAPIF
*                        @arg TMR_TMRIF_CMPIF
*                        @arg TMR_TMRIF_ACIF    (only for HT6x2x)
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_TMR_ITFlagStatusGet(HT_TMR_TypeDef* TMRx, uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (TMRx->TMRIF & ITFlag)
    {       
        return SET;                        /*!< TIMER Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< TIMER Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR TIMER INTERRUPT FLAG
*
* ����˵��: ���TIMER�жϱ�־
*
* ��ڲ���: TMRx       ֻ����HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5��һ��
*
*           ITFlag     ��Ҫ�����ĳ��TIMER�жϱ�־������Ϊ���²����������:
*                        @arg TMR_TMRIF_PRDIF
*                        @arg TMR_TMRIF_CAPIF
*                        @arg TMR_TMRIF_CMPIF
*                        @arg TMR_TMRIF_ACIF    (only for HT6x2x, HT502x)
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_TMR_ClearITPendingBit(HT_TMR_TypeDef* TMRx, uint8_t ITFlag)
{
    /*  assert_param  */
#if  defined  HT6x1x  ||  defined  HT501x                /*!< HT6x1x, HT501x                  */
    ITFlag |= 0x08;
#endif                                                   /*!< HT6x2x, HT502x                  */   
    TMRx->TMRIF  &= ~((uint32_t)ITFlag);                 /*!< Clear TIMER Interrupt Flag      */
    
}

