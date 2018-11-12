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
* File         : ht6xxx_pmu.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_PMU_C

#include "ht6xxx_pmu.h"

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

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         INITIAL PMU MODULE
*
* ����˵��: ��ʼ��PMUģ��
*
* ��ڲ���: PMU_InitStruct     PMU��ʼ���ṹ��ָ�룬��Ҫ����6������: 
*                              1) LvdinState     : Lvdin״̬����
*                              2) BorState       : Bor״̬����
*                              3) VccLvlSet      : Vcc�����ֵ�趨
*                              4) BorLvlSet      : Bor�����ֵ�趨
*                              5) VccBorDetTime  : Vcc�Լ�Bor���ʱ������
*                              6) VccBorDetPeriod: Vcc�Լ�Bor�����������
*
* ���ز���: ��                                      
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
void HT_PMU_Init(PMU_InitTypeDef* PMU_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg = 0;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
    
    tempreg = HT_PMU->PMUCON & 0x8010;
    
    tempreg |= PMU_InitStruct->Lvdin0State;                                /*!< ����LVDIN0����         */
#if  defined  HT501x ||  defined  HT6x2x  ||  defined  HT502x
    tempreg |= (PMU_InitStruct->Lvdin1State<<1);                           /*!< LVDIN1״̬����         */
#endif 
#if  defined  HT6x2x 	
		tempreg |= PMU_InitStruct->POWState;                                	 /*!< ����POW����         */
#endif
    tempreg |= PMU_InitStruct->BorState;                                   /*!< ����BOR����            */
    
    HT_PMU->PMUCON = tempreg;
   
    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
    
    HT_PMU->VDETCFG &= ~(PMU_VDETCFG_VCCLVL|PMU_VDETCFG_BORLVL);   
    HT_PMU->VDETCFG |= (PMU_InitStruct->VccLvlSet | PMU_InitStruct->BorLvlSet);
                                                                           /*!< VccBor�����ֵ����     */
    HT_PMU->VDETPCFG &= ~(PMU_VDETPCFG_VDETPRD|PMU_VDETPCFG_VDETTIME);                                                                       
    HT_PMU->VDETPCFG |= (PMU_InitStruct->VccBorDetTime | PMU_InitStruct->VccBorDetPeriod);
#if defined  HT6x2x                                                                           /*!< VccBor�����ֵ����     */  
		HT_PMU->PDTFLT = PMU_InitStruct->POWFilter;                            /*!< POW�����˲�ֵ����     */ 
#endif
} 

/*
*********************************************************************************************************
*                            Enable/Disable THE BIG POWER LDO ON HOLD MODE
*
* ����˵��: ����Holdģʽ�´󹦺�LDO
*
* ��ڲ���: NewState    = ENABLE�� Holdģʽ��ʹ�ܴ󹦺�LDO
*                       = DISABLE��Holdģʽ�¹رմ󹦺�LDO
*
* ���ز���: ��                                      
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
void HT_PMU_HoldLDOConfig(FunctionalState NewState)
{
    /*  assert_param  */
    
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
    
    if (NewState != DISABLE)
    {       
        HT_PMU->PMUCON |= PMU_PMUCON_HoldLDO;                              /*!< Holdģʽ��ʹ�ܴ󹦺�LDO*/ 
    }
    else
    {
        HT_PMU->PMUCON &= ~PMU_PMUCON_HoldLDO;                             /*!< Holdģʽ�¹رմ󹦺�LDO*/  
    }       
   
    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */ 
} 

/*
*********************************************************************************************************
*                                    DISCHARGE  CONFIGURE
*
* ����˵��: ��ضۻ�����
*
* ��ڲ���: ChargeMode  = DisCharge0mA
*                       = DisCharge1mA
*                       = DisCharge2mA
*
* ���ز���: ��                                      
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined HT501x  ||  HT502x
void HT_PMU_DisChargeConfig(DISCharge_TypeDef ChargeMode)
{
    /*  assert_param  */
    uint32_t tempreg;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
    
    tempreg  =  HT_PMU->PMUCON & 0x7FFF;
    tempreg |=  ChargeMode & 0x8000; 
    HT_PMU->PMUCON = tempreg;                                              /*!< ����DisChargeʹ��      */       
   
    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */ 
#if defined   HT6x2x  
    tempreg  =  HT_PMU->VDETCFG & 0x3FFF;
    tempreg |=  (ChargeMode>>4)&0xC000; 
#elif defined  HT501x  || HT502x
    tempreg  =  HT_PMU->VDETCFG & 0x7FFF;
    tempreg |=  (ChargeMode>>4)&0x8000;
#endif
    HT_PMU->VDETCFG = tempreg;                                             /*!< ����DisCharge����      */     
}
#endif

/*
*********************************************************************************************************
*                               GET SPECIFIED PMU STATE FLAG STATUS
*
* ����˵��: ��ȡ��ӦPMU���״̬��־
*
* ��ڲ���: STAFlag    ��Ҫ����ĳ��PMU�жϣ�����Ϊ���²���:
*                        @arg PMU_PMUSTA_VCCFLG
*                        @arg PMU_PMUSTA_BORFLG
*                        @arg PMU_PMUSTA_LVD0FLG
*                        @arg PMU_PMUSTA_LVD1FLG   (for HT6x2x, HT501x, HT502x)
*
* ���ز���: ITStatus    = SET��  ���ֵ�����趨��ֵ
*                       = RESET�����ֵ�����趨��ֵ
* 
* ����˵��: ��
*********************************************************************************************************
*/
FlagStatus HT_PMUState_FlagStatusGet(uint8_t STAFlag)
{
    /*  assert_param  */
    
    if (HT_PMU->PMUSTA & STAFlag)
    {       
        return SET;                        /*!< ���ֵ�����趨��ֵ         */
    }
    else
    {
        return RESET;                      /*!< ���ֵ�����趨��ֵ         */
    } 
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE PMU INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�PMU�ж�
*
* ��ڲ���: ITEn       PMU�ж�����λ������Ϊ���²����������
*                        @arg PMU_PMUIE_VCCIE
*                        @arg PMU_PMUIE_BORIE
*                        @arg PMU_PMUIE_LVD0IE
*                        @arg PMU_PMUIE_LVD1IE    (for HT6x2x, HT501x)
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_PMU_ITConfig(uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_PMU->PMUIE |= (uint32_t)(ITEn & PMU_PMUIE);            /*!< ʹ��PMU�ж�           */
    }
    else
    {
        HT_PMU->PMUIE &= ~(uint32_t)ITEn;                         /*!< �ر�PMU�ж�           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED PMU INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦPMU�жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����ĳ��PMU�жϣ�����Ϊ���²���:
*                        @arg PMU_PMUIF_VCCIF
*                        @arg PMU_PMUIF_BORIF
*                        @arg PMU_PMUIF_LVD0IF
*                        @arg PMU_PMUIF_LVD1IF   (for HT6x2x, HT501x)
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_PMU_ITFlagStatusGet(uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (HT_PMU->PMUIF & ITFlag)
    {       
        return SET;                        /*!< PMU Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< PMU Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                CLEAR PMU INTERRUPT FLAG
*
* ����˵��: ���PMU�жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��PMU�жϱ�־������Ϊ���²����������:
*                        @arg PMU_PMUIF_VCCIF
*                        @arg PMU_PMUIF_BORIF
*                        @arg PMU_PMUIF_LVD0IF
*                        @arg PMU_PMUIF_LVD1IF    (for HT6x2x, HT501x)
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_PMU_ClearITPendingBit(uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    HT_PMU->PMUIF &= ~((uint32_t)ITFlag);                  /*!< Clear PMU Interrupt Flag    */
    
}

