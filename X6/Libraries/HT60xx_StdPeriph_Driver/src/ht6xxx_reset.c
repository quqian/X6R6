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
* File         : ht6xxx_reset.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_RESET_C

#include "ht6xxx_reset.h"
#include "ht6xxx_cmu.h"

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
*                               GET SPECIFIED WAKE FLAG STATUS
*
* ����˵��: ��ȡ��Ӧ����״̬��־
*
* ��ڲ���: STAFlag    ��Ҫ����ĳ�����ѱ�־������Ϊ���²���:
*                       @arg PMU_WAKEIF_PMUWKIF      
*                       @arg PMU_WAKEIF_INT0WKIF     
*                       @arg PMU_WAKEIF_INT1WKIF     
*                       @arg PMU_WAKEIF_INT2WKIF     
*                       @arg PMU_WAKEIF_INT3WKIF     
*                       @arg PMU_WAKEIF_INT4WKIF     
*                       @arg PMU_WAKEIF_INT5WKIF     
*                       @arg PMU_WAKEIF_INT6WKIF     
*                       @arg PMU_WAKEIF_RX0WKIF      
*                       @arg PMU_WAKEIF_RX1WKIF      
*                       @arg PMU_WAKEIF_RX2WKIF      
*                       @arg PMU_WAKEIF_RX3WKIF      
*                       @arg PMU_WAKEIF_RX4WKIF      
*                       @arg PMU_WAKEIF_RX5WKIF      
*                       @arg PMU_WAKEIF_TBSWKIF      
*                       @arg PMU_WAKEIF_RTCWKIF      
*                       @arg PMU_WAKEIF_WDTWKIF
*                       @arg PMU_WAKEIF_AREGWKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR0WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR1WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR2WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR3WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR4WKIF    (Only for HT6x2x)
*                       @arg PMU_WAKEIF_TMR5WKIF    (Only for HT6x2x)
*                       @arg PMU_WAKEIF_IICWKIF     (Only for HT502x)
*                       @arg PMU_WAKEIF_SPIWKIF     (Only for HT502x)
*                       @arg PMU_WAKEIF_EMUWKIF     (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_DMAWKIF     (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_KEYWKIF     (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_RX6WKIF     (Only for HT6x2x)
*                       @arg PMU_WAKEIF_INT7WKIF    (Only for HT6x2x, HT502x)
*                       @arg PMU_WAKEIF_INT8WKIF    (Only for HT6x2x, HT502x)
*                       @arg PMU_WAKEIF_INT9WKIF    (Only for HT6x2x, HT502x)
*                       @arg PMU_WAKEIF_NMIWKIF     (Only for HT502x)
*
* ���ز���: ITStatus    = SET��  ���ѱ�־����
*                       = RESET�����ѱ�־δ����
* 
* ����˵��: Hold��Sleep���Ѷ��ᵼ����Ӧ���ѱ�־��λ���û�������һ��������Hold���ѻ���Sleep���ѣ����Բ鿴
*           RSTSTA�Ĵ�����PMU_RSTSTA_SleepFlag��PMU_RSTSTA_HoldFlag��־λ������
*********************************************************************************************************
*/
FlagStatus HT_WakeFlagStatusGet(uint32_t STAFlag)
{
    /*  assert_param  */
    
    if (HT_PMU->WAKEIF & STAFlag)
    {       
        return SET;                        /*!< ������Ӧ���ѱ�־          */
    }
    else
    {
        return RESET;                      /*!< δ������Ӧ���ѱ�־        */
    } 
}

/*
*********************************************************************************************************
*                               CLEAR SPECIFIED WAKE FLAG STATUS
*
* ����˵��: �����Ӧ���ѱ�־
*
* ��ڲ���: STAFlag    ��Ҫ�����ĳ�����ѱ�־������Ϊ���²���:
*                       @arg PMU_WAKEIF           ����������б�־
*                       @arg PMU_WAKEIF_PMUWKIF      
*                       @arg PMU_WAKEIF_INT0WKIF     
*                       @arg PMU_WAKEIF_INT1WKIF     
*                       @arg PMU_WAKEIF_INT2WKIF     
*                       @arg PMU_WAKEIF_INT3WKIF     
*                       @arg PMU_WAKEIF_INT4WKIF     
*                       @arg PMU_WAKEIF_INT5WKIF     
*                       @arg PMU_WAKEIF_INT6WKIF     
*                       @arg PMU_WAKEIF_RX0WKIF      
*                       @arg PMU_WAKEIF_RX1WKIF      
*                       @arg PMU_WAKEIF_RX2WKIF      
*                       @arg PMU_WAKEIF_RX3WKIF      
*                       @arg PMU_WAKEIF_RX4WKIF      
*                       @arg PMU_WAKEIF_RX5WKIF      
*                       @arg PMU_WAKEIF_TBSWKIF      
*                       @arg PMU_WAKEIF_RTCWKIF      
*                       @arg PMU_WAKEIF_WDTWKIF
*                       @arg PMU_WAKEIF_AREGWKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR0WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR1WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR2WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR3WKIF    (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR4WKIF    (Only for HT6x2x)
*                       @arg PMU_WAKEIF_TMR5WKIF    (Only for HT6x2x)
*                       @arg PMU_WAKEIF_IICWKIF     (Only for HT502x)
*                       @arg PMU_WAKEIF_SPIWKIF     (Only for HT502x)
*                       @arg PMU_WAKEIF_EMUWKIF     (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_DMAWKIF     (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_KEYWKIF     (Only for HT501x, HT502x)
*                       @arg PMU_WAKEIF_RX6WKIF     (Only for HT6x2x)
*                       @arg PMU_WAKEIF_INT7WKIF    (Only for HT6x2x, HT502x)
*                       @arg PMU_WAKEIF_INT8WKIF    (Only for HT6x2x, HT502x)
*                       @arg PMU_WAKEIF_INT9WKIF    (Only for HT6x2x, HT502x)
*                       @arg PMU_WAKEIF_NMIWKIF     (Only for HT502x)
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_WakeFlagClear(uint32_t STAFlag)
{
    /*  assert_param  */
    
    HT_PMU->WAKEIF &= ~STAFlag;                 /*!< �����Ӧ���ѱ�־          */
}

/*
*********************************************************************************************************
*                               GET SPECIFIED RESET FLAG STATUS
*
* ����˵��: ��ȡ��Ӧϵͳ��λ��־
*
* ��ڲ���: STAFlag    ��Ҫ����ĳ����λ��־������Ϊ���²���:
*                       @arg PMU_RSTSTA_PORRST                                   
*                       @arg PMU_RSTSTA_LBORRST                                  
*                       @arg PMU_RSTSTA_WDTRST                                   
*                       @arg PMU_RSTSTA_WakeupRST                                
*                       @arg PMU_RSTSTA_ExtRST                                   
*                       @arg PMU_RSTSTA_SoftRST                                  
*                       @arg PMU_RSTSTA_DebugRST                                 
*                       @arg PMU_RSTSTA_BORRST                                   
*                       @arg PMU_RSTSTA_SleepFlag                                
*                       @arg PMU_RSTSTA_HoldFlag   

     
*
* ���ز���: ITStatus    = SET��  ��λ��־����
*                       = RESET  ��λ��־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
FlagStatus HT_ResetFlagStatusGet(uint32_t STAFlag)
{
    /*  assert_param  */
    
    if (HT_PMU->RSTSTA & STAFlag)
    {       
        return SET;                        /*!< ������Ӧ���ѱ�־          */
    }
    else
    {
        return RESET;                      /*!< δ������Ӧ���ѱ�־        */
    } 
} 

/*
*********************************************************************************************************
*                               CLEAR SPECIFIED RESET FLAG STATUS
*
* ����˵��: �����Ӧ��λ��־
*
* ��ڲ���: STAFlag    ��Ҫ�����ĳ����λ��־������Ϊ���²���:
*                       @arg PMU_RSTSTA           ����������б�־
*                       @arg PMU_RSTSTA_PORRST                                   
*                       @arg PMU_RSTSTA_LBORRST                                  
*                       @arg PMU_RSTSTA_WDTRST                                   
*                       @arg PMU_RSTSTA_WakeupRST                                
*                       @arg PMU_RSTSTA_ExtRST                                   
*                       @arg PMU_RSTSTA_SoftRST                                  
*                       @arg PMU_RSTSTA_DebugRST                                 
*                       @arg PMU_RSTSTA_BORRST                                   
*                       @arg PMU_RSTSTA_SleepFlag                                
*                       @arg PMU_RSTSTA_HoldFlag        
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_ResetFlagClear(uint32_t STAFlag)
{
    /*  assert_param  */
    
    HT_PMU->RSTSTA &= ~STAFlag;                     /*!< �����Ӧ��λ��־          */
}

/*
*********************************************************************************************************
*                                     ENTER HOLD MODE 
*
* ����˵��: ���üĴ�����ʹоƬ����Hold�͹���ģʽ
*
* ��ڲ���: ��     
*                                               
* ���ز���: ��                                 
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_EnterHold(void)
{
    SCB->SCR = 0x0000;    
    __WFI();
}

/*
*********************************************************************************************************
*                                     ENTER SLEEP MODE 
*
* ����˵��: ���üĴ�����ʹоƬ����Sleep�͹���ģʽ
*
* ��ڲ���: ��     
*                                               
* ���ز���: ��                                 
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_EnterSleep(void)
{
    SCB->SCR = 0x0004;
    __WFI();    
}

/*
*********************************************************************************************************
*                                         FREE DOG 
*
* ����˵��: ι��
*
* ��ڲ���: ��     
*                                               
* ���ز���: ��                                 
* 
* ����˵��: ����������WatchDog���ʱ��ΪԼ4s���û����Ը���ʵ������ı䣬WatchDog���ʱ����㹫ʽ���£�
*           64ms * (1+WDTCLR[7:0])
*********************************************************************************************************
*/
void HT_FreeDog(void)
{
#if defined HT6x1x   
		HT_WDT->WDTCFG = 0x0;
#endif
    HT_WDT->WDTCLR = 0xAA40;                                        /*!< ���Ź�ι��                   */
}

/*
*********************************************************************************************************
*                                    WATCHDOG CONFIGURE ON SLEEP/HOLD MODE 
*
* ����˵��: Sleep/Holdģʽ��WatchDog����
*
* ��ڲ���: NewState   = ENABLE�� Sleep/Holdģʽ��WatchDogʹ��
*                      = DISABLE��Sleep/Holdģʽ��WatchDog�ر� 
*                                               
* ���ز���: ��                                 
* 
* ����˵��: ע�⣺�˺����漰��ClkCtrl0�Ĵ������ã��û�Ӧ��֤������˺������������Ҫ����ClkCtrl0�Ĵ�����
            Ӧ��֤�����޸�CMU_CLKCTRL0_WDTEN����λ
*********************************************************************************************************
*/
void HT_SHModeDogConfigure(FunctionalState NewState)
{

    if (NewState != DISABLE)
    {       
//        HT_RTC->CTRLBYFLASH  = 0x00;                               /*!< Hold/Sleepģʽ��ʹ��LRC      */
#if  defined  HT6x1x                                                /*!< HT6x1x               */        
        HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_WDTEN, ENABLE);           /*!< Hold/Sleepģʽ��ʹ��WatchDog */
#endif
    }
    else
    {
//        HT_RTC->CTRLBYFLASH  = 0x02;                                /*!< Hold/Sleepģʽ�¹ر�LRC      */
#if  defined  HT6x1x                                                /*!< HT6x1x               */         
        HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_WDTEN, DISABLE);         /*!< Hold/Sleepģʽ�¹ر�WatchDog */  
#endif        
    } 
}


