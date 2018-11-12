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
* File         : ht6xxx_tbs.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_TBS_C

#include "ht6xxx_tbs.h"

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
*                                     CONFIGUE TBS MODULE
*
* ����˵��: ����TBS��ģ��
*
* ��ڲ���: TBS_SubModule    TBS�ڸ�����ģ�飬����Ϊ���²���������� 
*                             @arg TBS_TBSCON_TMPEn      ���¶Ȳ���ģ��
*                             @arg TBS_TBSCON_VBATEn     ����ص�ѹ����ģ��
*                             @arg TBS_TBSCON_ADC0En     ��ADC0����ģ��
*                             @arg TBS_TBSCON_ADC1En     ��ADC1����ģ��
*                             @arg TBS_TBSCON_VCCEn      ����Դ��ѹ����ģ��
*                             @arg TBS_TBSCON_ADC2En     ��ADC2����ģ��      (only for HT501x)
*                             @arg TBS_TBSCON_VREFEn     ��Vref����ģ��      (only for HT501x)
*
*           NewState         = ENABLE�� ģ��ʹ��
*                            = DISABLE��ģ��ر�                      
*
* ���ز���: ��                                      
* 
* ����˵��: 1) ����RTC�����漰�¶ȴ�������������ر��¶ȴ�����ģ�顣
*           2) ������ʹ�ܵ�ص�ѹ�Ƚ�֮ǰ���ȵ��ú���HT_TBS_BatCmpThresholdSet() ���úõ�ص�ѹ�Ƚ���ֵ,
*           3) ��ص�ѹ�ȽϹ���������ȷִ�У���ʹ�ܵ�ص�ѹ����ģ��
*********************************************************************************************************
*/
void HT_TBSConfig(uint32_t TBS_SubModule, FunctionalState NewState)
{
    /*  assert_param  */

    uint32_t tempreg;

    tempreg = HT_TBS->TBSCON & TBS_TBSCON_xEn;  

    if(HT_CMU->CLKSTA & CMU_CLKSTA_LFSEL)
    {
        tempreg |= 0x6d60;                       /*!< MEMS TPS Ĭ������         */
    }  
    else
    {
#if defined HT501x
        tempreg |= 0x100;
#else
        tempreg |= 0x6540;                       /*!< OSC TPS Ĭ������         */
#endif
    }  
    
    TBS_SubModule &= TBS_TBSCON_xEn;
    if (NewState != DISABLE)
    {       
        tempreg |= (uint32_t)TBS_SubModule;      /*!< ʹ��TBSģ��                 */
    }
    else
    {
        tempreg &= ~(uint32_t)TBS_SubModule;     /*!< �ر�TBSģ��                 */
    } 

    HT_TBS->TBSCON = tempreg;                    /*!< д�Ĵ���                    */
  
} 

/*
*********************************************************************************************************
*                                 SET SubModule OPEN PERIOD
*
* ����˵��: ������ģ���Ƶ������
*
* ��ڲ���: PRDModule    : ��ģ�����ڿ��ƣ�����Ϊ���²���
*                          @arg TMPPRD      ���¶Ȳ�������
*                          @arg VBATPRD     ����ص�ѹ��������
*                          @arg ADC0PRD     ��ADC0��������
*                          @arg ADC1PRD     ��ADC1��������
*                          @arg VCCPRD      ��VCC��������
*                          @arg ADC2PRD     ��ADC2��������    (only for HT501x)
*                          @arg VREFPRD     ��VREF��������    (only for HT501x)
*                          @arg ADC3PRD     ��ADC3��������    (only for HT6x2x)
*                          @arg ADC4PRD     ��ADC4��������    (only for HT6x2x)
*                          @arg ADC5PRD     ��ADC5��������    (only for HT6x2x)
*
*           Period       : �����趨�����ݲ�ͬоƬ������Ϊ���²���
*                                   HT501x  HT502x                  HT6x1x  HT6x2x
*                             TMPPRD:                           TMPPRD:
*                        @arg TBS_TBSPRD_TMPPRD_1S              TBS_TBSPRD_TMPPRD_OSC0P5S_MEM8S    
*                        @arg TBS_TBSPRD_TMPPRD_8S              TBS_TBSPRD_TMPPRD_OSC1S_MEM4S      
*                        @arg TBS_TBSPRD_TMPPRD_32S             TBS_TBSPRD_TMPPRD_OSC2S_MEM2S      
*                        @arg TBS_TBSPRD_TMPPRD_125mS           TBS_TBSPRD_TMPPRD_OSC4S_MEM1S      
*                                                               TBS_TBSPRD_TMPPRD_OSC8S_MEM0P5S                                                                                                  
*                             VBATPRD:                          TBS_TBSPRD_TMPPRD_OSC16S_MEM125mS  
*                        @arg TBS_TBSPRD_VBATPRD_1S             TBS_TBSPRD_TMPPRD_OSC32S_MEM31mS   
*                        @arg TBS_TBSPRD_VBATPRD_4S             TBS_TBSPRD_TMPPRD_OSC64S_MEM8mS    
*                        @arg TBS_TBSPRD_VBATPRD_8S                                                
*                        @arg TBS_TBSPRD_VBATPRD_32S            VBATPRD:                
*                                                               TBS_TBSPRD_VBATPRD_1S              
*                             VCCPRD:                           TBS_TBSPRD_VBATPRD_2S              
*                        @arg TBS_TBSPRD_VCCPRD_1S              TBS_TBSPRD_VBATPRD_8S              
*                        @arg TBS_TBSPRD_VCCPRD_4S              TBS_TBSPRD_VBATPRD_16S             
*                        @arg TBS_TBSPRD_VCCPRD_8S                                                 
*                        @arg TBS_TBSPRD_VCCPRD_32S             ADC0PRD:                 
*                                                               TBS_TBSPRD_ADC0PRD_1S              
*                             ADC0PRD:                          TBS_TBSPRD_ADC0PRD_2S              
*                        @arg TBS_TBSPRD_ADC0PRD_1S             TBS_TBSPRD_ADC0PRD_8S              
*                        @arg TBS_TBSPRD_ADC0PRD_4S             TBS_TBSPRD_ADC0PRD_16S             
*                        @arg TBS_TBSPRD_ADC0PRD_8S                                                
*                        @arg TBS_TBSPRD_ADC0PRD_32S            ADC1PRD:                 
*                                                               TBS_TBSPRD_ADC1PRD_1S              
*                             ADC1PRD:                          TBS_TBSPRD_ADC1PRD_2S              
*                        @arg TBS_TBSPRD_ADC1PRD_1S             TBS_TBSPRD_ADC1PRD_8S              
*                        @arg TBS_TBSPRD_ADC1PRD_4S             TBS_TBSPRD_ADC1PRD_16S             
*                        @arg TBS_TBSPRD_ADC1PRD_8S                                                
*                        @arg TBS_TBSPRD_ADC1PRD_32S            VCCPRD:                  
*                                                               TBS_TBSPRD_VCCPRD_1S               
*                             ADC2PRD:                          TBS_TBSPRD_VCCPRD_2S               
*                        @arg TBS_TBSPRD_ADC2PRD_1S             TBS_TBSPRD_VCCPRD_8S               
*                        @arg TBS_TBSPRD_ADC2PRD_4S             TBS_TBSPRD_VCCPRD_16S              
*                        @arg TBS_TBSPRD_ADC2PRD_8S    
*                        @arg TBS_TBSPRD_ADC2PRD_32S            ADC3PRD:(only for HT6x2x)
*                                                               TBS_TBSPRD_ADC3PRD_1S
*                             VREFPRD:                          TBS_TBSPRD_ADC3PRD_2S
*                        @arg TBS_TBSPRD_VREFPRD_1S             TBS_TBSPRD_ADC3PRD_8S    
*                        @arg TBS_TBSPRD_VREFPRD_4S             TBS_TBSPRD_ADC3PRD_16S
*                        @arg TBS_TBSPRD_VREFPRD_8S    
*                        @arg TBS_TBSPRD_VREFPRD_32S            ADC4PRD:(only for HT6x2x)
*                                                               TBS_TBSPRD_ADC4PRD_1S
*                                                               TBS_TBSPRD_ADC4PRD_2S
*                                                               TBS_TBSPRD_ADC4PRD_8S
*                                                               TBS_TBSPRD_ADC4PRD_16S
*
*                                                               ADC5PRD:(only for HT6x2x)
*                                                               TBS_TBSPRD_ADC5PRD_1S
*                                                               TBS_TBSPRD_ADC5PRD_2S
*                                                               TBS_TBSPRD_ADC5PRD_8S
*                                                               TBS_TBSPRD_ADC5PRD_16S
*
* ���ز���: ��                                     
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_TBS_PeriodSet(TBS_PeriodTypeDef PRDModule, uint32_t Period)
{
    /*  assert_param  */

     uint32_t tempreg = (HT_TBS->TBSPRD & (~PRDModule));  
     tempreg |= Period;
     HT_TBS->TBSPRD = tempreg;                             /*!< ���ò�������           */     
}

/*
*********************************************************************************************************
*                                     SET COMPARE THRESHOLD
*
* ����˵��: ���ñȽ���ֵ
*
* ��ڲ���: CmpReg    �ȽϼĴ���������Ϊ���²���
*                     @arg CMP_VBAT     ����ص�ѹ�ȽϼĴ���  
*                     @arg CMP_ADC0     ��ADC0�ȽϼĴ���      (only for HT501x, HT502x, HT6x2x)
*                     @arg CMP_ADC1     ��ADC1�ȽϼĴ���      (only for HT501x, HT502x)
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_TBS_CMPThresholdSet(TBS_CMPRegTypeDef CmpReg, int16_t Threshold)
{
    /*  assert_param  */
    
    if(CmpReg == CMP_VBAT)
        HT_TBS->VBATCMP = Threshold;                /*!< ���õ�ص�ѹ�Ƚ���ֵ           */
#if defined HT501x  ||  HT6x2x  ||  HT502x
    else if(CmpReg == CMP_ADC0)
        HT_TBS->ADC0CMP = Threshold;                /*!< ����ADC0�Ƚ���ֵ               */
#endif
#if defined HT501x  ||  HT502x
    else if(CmpReg == CMP_ADC1)
        HT_TBS->ADC1CMP = Threshold;                /*!< ����ADC1�Ƚ���ֵ               */
#endif    
} 

/*
*********************************************************************************************************
*                                     GET MEASURED VALUES
*
* ����˵��: ��ȡ����ֵ
*
* ��ڲ���: SubModule    : ��ģ��ѡ��                       
*
* ���ز���: ��ѡ����ģ�����ֵ������ֵΪ16λ�з�����                                     
* 
* ����˵��: ��
*********************************************************************************************************
*/
int16_t HT_TBS_ValueRead(TBS_SubModuleTypeDef SubModule)
{
    /*  assert_param  */

    switch(SubModule)
    {
        case TBS_TMP:
            return  (int16_t)HT_TBS->TMPDAT;       /*!< �����¶Ȳ���ֵ           */
        case TBS_VBAT:
            return  (int16_t)HT_TBS->VBATDAT;      /*!< ���ص�ص�ѹ����ֵ       */
        case TBS_ADC0:
            return  (int16_t)HT_TBS->ADC0DAT;      /*!< ����ADC0����ֵ           */
        case TBS_ADC1:
            return  (int16_t)HT_TBS->ADC1DAT;      /*!< ����ADC1����ֵ           */
#if defined HT501x
        case TBS_ADC2:
            return  (int16_t)HT_TBS->ADC2DAT;      /*!< ����ADC2����ֵ           */
#endif
        default:
            return  (int16_t)HT_TBS->VCCDAT;       /*!< ���ص�Դ��ѹ����ֵ       */
    }
} 
/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE TBS INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�TBS�ж�
*
* ��ڲ���: ITEn       TBS�ж�����λ������Ϊ���²����������
*                        @arg TBS_TBSIE_TMPIE
*                        @arg TBS_TBSIE_VBATIE
*                        @arg TBS_TBSIE_ADC0IE
*                        @arg TBS_TBSIE_ADC1IE
*                        @arg TBS_TBSIE_VBATCMPIE
*                        @arg TBS_TBSIE_VCCIE
*                        @arg TBS_TBSIE_ADC2IE        (only for HT501x)
*                        @arg TBS_TBSIE_VREFIE        (only for HT501x)
*                        @arg TBS_TBSIE_ADC0CMPIE     (only for HT501x, HT6x2x, HT502x)
*                        @arg TBS_TBSIE_ADC1CMPIE     (only for HT501x, HT502x)
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_TBS_ITConfig(uint32_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_TBS->TBSIE |= (uint32_t)ITEn;            /*!< ʹ��TBS�ж�           */
    }
    else
    {
        HT_TBS->TBSIE &= ~(uint32_t)ITEn;           /*!< �ر�TBS�ж�           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED TBS INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦTBS�жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����ĳ��TBS�жϣ�����Ϊ���²���:
*                        @arg TBS_TBSIF_TMPIF
*                        @arg TBS_TBSIF_VBATIF
*                        @arg TBS_TBSIF_ADC0IF
*                        @arg TBS_TBSIF_ADC1IF
*                        @arg TBS_TBSIF_VBATCMPIF
*                        @arg TBS_TBSIF_VCCIF
*                        @arg TBS_TBSIF_ADC2IF        (only for HT501x)
*                        @arg TBS_TBSIF_VREFIF        (only for HT501x)
*                        @arg TBS_TBSIF_ADC0CMPIF     (only for HT501x, HT6x2x, HT502x)
*                        @arg TBS_TBSIF_ADC1CMPIF     (only for HT501x, HT502x)
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_TBS_ITFlagStatusGet(uint32_t ITFlag)
{
    /*  assert_param  */
    
    if (HT_TBS->TBSIF & ITFlag)
    {       
        return SET;                        /*!< TBS Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< TBS Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR TBS INTERRUPT FLAG
*
* ����˵��: ���TBS�жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��TBS�жϱ�־������Ϊ���²����������:
*                        @arg TBS_TBSIF_TMPIF
*                        @arg TBS_TBSIF_VBATIF
*                        @arg TBS_TBSIF_ADC0IF
*                        @arg TBS_TBSIF_ADC1IF
*                        @arg TBS_TBSIF_VBATCMPIF
*                        @arg TBS_TBSIF_VCCIF
*                        @arg TBS_TBSIF_ADC2IF        (only for HT501x)
*                        @arg TBS_TBSIF_VREFIF        (only for HT501x)
*                        @arg TBS_TBSIF_ADC0CMPIF     (only for HT501x, HT6x2x, HT502x)
*                        @arg TBS_TBSIF_ADC1CMPIF     (only for HT501x, HT502x)
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_TBS_ClearITPendingBit(uint32_t ITFlag)
{
    /*  assert_param  */
    
      
    HT_TBS->TBSIF  &= ~((uint32_t)ITFlag);                  /*!< Clear TBS Interrupt Flag       */
    
}

