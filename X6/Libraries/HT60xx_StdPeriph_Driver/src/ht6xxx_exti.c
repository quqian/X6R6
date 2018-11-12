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
* File         : ht6xxx_exti.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_EXTI_C

#include "ht6Xxx_exti.h"

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
*                                 ENABLE OR DISABLE EXTI RISE INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�EXTI�������ж�
*
* ��ڲ���: ITEn       RTC�ж�����λ������Ϊ���²����������
*                        @arg INT_EXTIE_RIE_INT0
*                        @arg INT_EXTIE_RIE_INT1
*                        @arg INT_EXTIE_RIE_INT2
*                        @arg INT_EXTIE_RIE_INT3
*                        @arg INT_EXTIE_RIE_INT4
*                        @arg INT_EXTIE_RIE_INT5
*                        @arg INT_EXTIE_RIE_INT6
*                        @arg INT_EXTIE_RIE
*                        @arg INT_EXTIE2_RIE_INT7   (only for HT6x2x and HT502x)         
*                        @arg INT_EXTIE2_RIE_INT8   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIE2_RIE_INT9   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIE2_RIE
*
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_EXTIRise_ITConfig(uint32_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
#if  defined  HT6x2x  ||  defined  HT502x     
    uint32_t tempreg = (ITEn&INT_EXTIE2_RIE)>>16;
#endif
    ITEn &= INT_EXTIE_RIE;
    if (NewState != DISABLE)                        /*!< ʹ���ж�           */
    {       
        HT_INT->EXTIE |= ITEn;            
#if  defined  HT6x2x  ||  defined  HT502x
        HT_INT->EXTIE2 |= tempreg;
#endif        
    }
    else                                            /*!< �ر��ж�           */
    {
        HT_INT->EXTIE &= ~ITEn;          
#if  defined  HT6x2x  ||  defined  HT502x
        HT_INT->EXTIE2 &= ~tempreg;
#endif           
    } 
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE EXTI FALL INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�EXTI�½����ж�
*
* ��ڲ���: ITEn       RTC�ж�����λ������Ϊ���²����������
*                        @arg INT_EXTIE_FIE_INT0
*                        @arg INT_EXTIE_FIE_INT1
*                        @arg INT_EXTIE_FIE_INT2
*                        @arg INT_EXTIE_FIE_INT3
*                        @arg INT_EXTIE_FIE_INT4
*                        @arg INT_EXTIE_FIE_INT5
*                        @arg INT_EXTIE_FIE_INT6
*                        @arg INT_EXTIE_FIE
*                        @arg INT_EXTIE2_FIE_INT7  (only for HT6x2x and HT502x)           
*                        @arg INT_EXTIE2_FIE_INT8  (only for HT6x2x and HT502x)
*                        @arg INT_EXTIE2_FIE_INT9  (only for HT6x2x and HT502x)
*                        @arg INT_EXTIE2_FIE
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_EXTIFall_ITConfig(uint32_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
#if  defined  HT6x2x  ||  defined  HT502x     
    uint32_t tempreg = (ITEn&INT_EXTIE2_FIE)>>16;
#endif    
    ITEn &= INT_EXTIE_FIE;
    if (NewState != DISABLE)
    {       
        HT_INT->EXTIE |= ITEn;            /*!< ʹ���ж�           */
#if  defined  HT6x2x  ||  defined  HT502x 
        HT_INT->EXTIE2 |= tempreg;
#endif          
    }
    else
    {
        HT_INT->EXTIE &= ~ITEn;           /*!< �ر��ж�           */
#if  defined  HT6x2x  ||  defined  HT502x 
        HT_INT->EXTIE2 &= ~tempreg;
#endif        
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED EXTI RISE INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦEXTI�������жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����ĳ��EXTI�������жϣ�����Ϊ���²���:
*                        @arg INT_EXTIF_RIF_INT0
*                        @arg INT_EXTIF_RIF_INT1
*                        @arg INT_EXTIF_RIF_INT2
*                        @arg INT_EXTIF_RIF_INT3
*                        @arg INT_EXTIF_RIF_INT4
*                        @arg INT_EXTIF_RIF_INT5
*                        @arg INT_EXTIF_RIF_INT6
*                        @arg INT_EXTIF2_RIF_INT7   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_RIF_INT8   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_RIF_INT9   (only for HT6x2x and HT502x)
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_EXTIRise_ITFlagStatusGet(uint32_t ITFlag)
{
    /*  assert_param  */
#if  defined  HT6x2x  ||  defined  HT502x     
    uint32_t tempreg = (ITFlag & INT_EXTIF2_RIF)>>16;
#endif       
    
    if (HT_INT->EXTIF & ITFlag)
    {       
        return SET;                        /*!< Interrupt Flag is set   */
    }
    else
    {
#if  defined  HT6x2x  ||  defined  HT502x     
        if (HT_INT->EXTIF2 & tempreg)
        {
            return SET;                    /*!< Interrupt Flag is set   */
        }
#endif                
        return RESET;                      /*!< Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                          GET SPECIFIED EXTI FALL INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦEXTI�½����жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����ĳ��EXTI�½����жϣ�����Ϊ���²���:
*                        @arg INT_EXTIF_FIF_INT0
*                        @arg INT_EXTIF_FIF_INT1
*                        @arg INT_EXTIF_FIF_INT2
*                        @arg INT_EXTIF_FIF_INT3
*                        @arg INT_EXTIF_FIF_INT4
*                        @arg INT_EXTIF_FIF_INT5
*                        @arg INT_EXTIF_FIF_INT6
*                        @arg INT_EXTIF2_FIF_INT7     (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_FIF_INT8     (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_FIF_INT9     (only for HT6x2x and HT502x)
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_EXTIFall_ITFlagStatusGet(uint32_t ITFlag)
{
    /*  assert_param  */
#if  defined  HT6x2x  ||  defined  HT502x     
    uint32_t tempreg = (ITFlag & INT_EXTIF2_FIF)>>16;
#endif     
    if (HT_INT->EXTIF & ITFlag)
    {       
        return SET;                        /*!< Interrupt Flag is set   */
    }
    else
    {
#if  defined  HT6x2x  ||  defined  HT502x     
        if (HT_INT->EXTIF2 & tempreg)
        {
            return SET;                    /*!< Interrupt Flag is set   */
        }
#endif       
        return RESET;                      /*!< Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR EXTI RISE INTERRUPT FLAG
*
* ����˵��: ���EXTI�������жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��EXTI�������жϱ�־������Ϊ���²����������:
*                        @arg INT_EXTIF_RIF_INT0
*                        @arg INT_EXTIF_RIF_INT1
*                        @arg INT_EXTIF_RIF_INT2
*                        @arg INT_EXTIF_RIF_INT3
*                        @arg INT_EXTIF_RIF_INT4
*                        @arg INT_EXTIF_RIF_INT5
*                        @arg INT_EXTIF_RIF_INT6
*                        @arg INT_EXTIF2_RIF_INT7   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_RIF_INT8   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_RIF_INT9   (only for HT6x2x and HT502x)
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_EXTIRise_ClearITPendingBit(uint32_t ITFlag)
{
    /*  assert_param  */
#if  defined  HT6x2x  ||  defined  HT502x     
    uint32_t tempreg = (ITFlag & INT_EXTIF2_RIF)>>16;
#endif        
    ITFlag &= INT_EXTIF_RIF;
    HT_INT->EXTIF  &= ~ITFlag;                  /*!< Clear EXTI Rise Edge Interrupt Flag */
#if  defined  HT6x2x  ||  defined  HT502x     
    HT_INT->EXTIF2  &= ~tempreg;  
#endif    
}

/*
*********************************************************************************************************
*                                   CLEAR EXTI FALL INTERRUPT FLAG
*
* ����˵��: ���EXTI�½����жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��EXTI�½����жϱ�־������Ϊ���²����������:
*                        @arg INT_EXTIF_FIF_INT0
*                        @arg INT_EXTIF_FIF_INT1
*                        @arg INT_EXTIF_FIF_INT2
*                        @arg INT_EXTIF_FIF_INT3
*                        @arg INT_EXTIF_FIF_INT4
*                        @arg INT_EXTIF_FIF_INT5
*                        @arg INT_EXTIF_FIF_INT6
*                        @arg INT_EXTIF2_FIF_INT7   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_FIF_INT8   (only for HT6x2x and HT502x)
*                        @arg INT_EXTIF2_FIF_INT9   (only for HT6x2x and HT502x)
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_EXTIFall_ClearITPendingBit(uint32_t ITFlag)
{
    /*  assert_param  */
#if  defined  HT6x2x  ||  defined  HT502x     
    uint32_t tempreg = (ITFlag & INT_EXTIF2_FIF)>>16;
#endif       
    ITFlag &= INT_EXTIF_FIF;  
    HT_INT->EXTIF  &= ~ITFlag;                /*!< Clear EXTI Fall Edge Interrupt Flag */
#if  defined  HT6x2x  ||  defined  HT502x     
    HT_INT->EXTIF2  &= ~tempreg;  
#endif     
}
/*
*********************************************************************************************************
*                                   ENABLE OR DISABLE EXTI DIGITAL FILTER
*
* ����˵��: ʹ�ܻ�ر��ⲿ�жϵ������˲�����
*
* ��ڲ���: ITFLTEn     �ж����������˲�����λ������Ϊ���²����������
*                        @arg INT_PINFLT_INTFLT_INT0
*                        @arg INT_PINFLT_INTFLT_INT1
*                        @arg INT_PINFLT_INTFLT_INT2
*                        @arg INT_PINFLT_INTFLT_INT3
*                        @arg INT_PINFLT_INTFLT_INT4
*                        @arg INT_PINFLT_INTFLT_INT5
*                        @arg INT_PINFLT_INTFLT_INT6
*                        @arg INT_PINFLT2_INTFLT_INT7  (only for HT6x2x and HT502x)           
*                        @arg INT_PINFLT2_INTFLT_INT8  (only for HT6x2x and HT502x)
*                        @arg INT_PINFLT2_INTFLT_INT9  (only for HT6x2x and HT502x)
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_EXTI_FilterConfig(uint32_t ITFLTEn,FunctionalState NewState)
{
    /*  assert_param  */
#if  defined  HT6x2x  ||  defined  HT502x     
    uint32_t tempreg = (ITFLTEn & INT_PINFLT2_INTFLT)>>16;
#endif       
    ITFLTEn &= INT_PINFLT_INTFLT;  
    if (NewState != DISABLE)
    {       
        HT_INT->PINFLT |= ITFLTEn;            /*!< ʹ�������˲�         */
#if  defined  HT6x2x  ||  defined  HT502x 
        HT_INT->PINFLT2 |= tempreg;
#endif          
    }
    else
    {
        HT_INT->PINFLT &= ~ITFLTEn;           /*!< �ر������˲�         */
#if  defined  HT6x2x  ||  defined  HT502x 
        HT_INT->PINFLT2 &= ~tempreg;
#endif        
    }    
}


