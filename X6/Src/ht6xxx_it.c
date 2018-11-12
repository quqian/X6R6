/*
*********************************************************************************************************
*                                 			   HT60XX
*                            			      HT60XX����
*
*                       			Copyright 2013, Hi-Trend Tech, Corp.
*                             			 All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : ht60xx_it.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT60XX_IT_C

#include "ht6xxx_lib.h"
#include "ht6xxx_it.h"
#include "emu_info.h"
#include "uart.h"
#include "system_info.h"

/*
*********************************************************************************************************
*                                            ���غ�/�ṹ��
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            ���ر���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        HARDFAULT INTERRUPT HANDLER #16-13
*
*********************************************************************************************************
*/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {

    }
}


/*
*********************************************************************************************************
*                                        PMU INTERRUPT HANDLER #16+0
*
*********************************************************************************************************
*/

void PMU_IRQHandler()
{	
    if(SET == HT_PMU_ITFlagStatusGet(PMU_PMUIF_LVD1IF))           
    {
        HT_PMU_ClearITPendingBit(PMU_PMUIF_LVD1IF);
		
		if(RESET == HT_PMUState_FlagStatusGet(PMU_PMUSTA_LVD1FLG)){//LVDIN1 С��1.2V
	
		}
    }
	if(SET == HT_PMU_ITFlagStatusGet(PMU_PMUIF_VCCIF)){
		HT_PMU_ClearITPendingBit(PMU_PMUIF_VCCIF);
		if(RESET == HT_PMUState_FlagStatusGet(PMU_PMUSTA_VCCFLG)){//VCC С��2.4V
	
		}
	}
}

/*
*********************************************************************************************************
*                                  3DES/AES INTERRUPT HANDLER  #16+1
*
*********************************************************************************************************
*/
#if defined HT6x1x
void TDES_IRQHandler()                      /*!< 3DES               */
{
    
}
#else
void AES_IRQHandler()                       /*!< AES                */
{
    
}
#endif

/*
*********************************************************************************************************
*                                   EXTI0 INTERRUPT HANDLER  #16+2
*
*********************************************************************************************************
*/

void EXTI0_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT0))         /*!< INT0�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT0);             /*!< ����жϱ�־             */
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT0))         /*!< INT0�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT0);             /*!< ����жϱ�־             */
    }      
}

/*
*********************************************************************************************************
*                                   EXTI1 INTERRUPT HANDLER  #16+3
*
*********************************************************************************************************
*/

void EXTI1_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT1))         /*!< INT1�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT1);             /*!< ����жϱ�־             */        
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT1))         /*!< INT1�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT1);             /*!< ����жϱ�־             */        
    }      
}

/*
*********************************************************************************************************
*                                   EXTI2 INTERRUPT HANDLER  #16+4
*
*********************************************************************************************************
*/

void EXTI2_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT2))         /*!< INT2�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT2);             /*!< ����жϱ�־             */         
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT2))         /*!< INT2�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT2);             /*!< ����жϱ�־             */       
    }      
}

/*
*********************************************************************************************************
*                                   EXTI3 INTERRUPT HANDLER  #16+5
*
*********************************************************************************************************
*/

void EXTI3_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT3))         /*!< INT3�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT3);             /*!< ����жϱ�־             */         
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT3))         /*!< INT3�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT3);             /*!< ����жϱ�־             */         
    }
}

/*
*********************************************************************************************************
*                                   EXTI4 INTERRUPT HANDLER  #16+6
*
*********************************************************************************************************
*/

void EXTI4_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT4))         /*!< INT4�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT4);             /*!< ����жϱ�־             */         
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT4))         /*!< INT4�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT4);             /*!< ����жϱ�־             */        
    }      
}

/*
*********************************************************************************************************
*                                   EXTI5 INTERRUPT HANDLER  #16+7
*
*********************************************************************************************************
*/

void EXTI5_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT5))         /*!< INT5�������ж�           */
    {

        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT5);             /*!< ����жϱ�־             */         
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT5))         /*!< INT5�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT5);             /*!< ����жϱ�־             */         
    }      
}

/*
*********************************************************************************************************
*                                   EXTI6 INTERRUPT HANDLER  #16+8
*
*********************************************************************************************************
*/

void EXTI6_IRQHandler()
{
    if(SET == HT_EXTIRise_ITFlagStatusGet(INT_EXTIF_RIF_INT6))         /*!< INT6�������ж�           */
    {
        
        HT_EXTIRise_ClearITPendingBit(INT_EXTIF_RIF_INT6);             /*!< ����жϱ�־             */        
    }

    if(SET == HT_EXTIFall_ITFlagStatusGet(INT_EXTIF_FIF_INT6))         /*!< INT6�½����ж�           */
    {

        HT_EXTIFall_ClearITPendingBit(INT_EXTIF_FIF_INT6);             /*!< ����жϱ�־             */         
    }      
}

/*
*********************************************************************************************************
*                                   UART0 INTERRUPT HANDLER  #16+9
*
*********************************************************************************************************
*/

//NB-LOT
void UART0_IRQHandler()
{
    static BaseType_t xHigherPriorityTaskWoken;
    uint8_t RX_Data = 0;
    
    if(SET == HT_UART_ITFlagStatusGet(HT_UART0, UART_UARTSTA_TXIF))         /*!< UART0�����ж�         */
    {
        HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_TXIF);             /*!< ����жϱ�־          */
		xSemaphoreGiveFromISR( gUart0Handle.xSemaphore, &xHigherPriorityTaskWoken );  
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); 
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART0, UART_UARTSTA_RXIF))         /*!< UART0�����ж�         */
    {
		RX_Data = HT_UART_ReceiveData(HT_UART0);
		FIFO_S_Put(&gUart0Handle.rxFIFO, RX_Data);
        HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_RXIF);             /*!< ����жϱ�־          */ 
    }      
}

/*
*********************************************************************************************************
*                                   UART1 INTERRUPT HANDLER  #16+10
*
*********************************************************************************************************
*/

void UART1_IRQHandler()
{
    if(SET == HT_UART_ITFlagStatusGet(HT_UART1, UART_UARTSTA_TXIF))         /*!< UART1�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART1, UART_UARTSTA_TXIF);             /*!< ����жϱ�־          */        
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART1, UART_UARTSTA_RXIF))         /*!< UART1�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART1, UART_UARTSTA_RXIF);             /*!< ����жϱ�־          */          
    }      
}

/*
*********************************************************************************************************
*                                   UART2 INTERRUPT HANDLER  #16+11
*
*********************************************************************************************************
*/

void UART2_IRQHandler()
{
    if(SET == HT_UART_ITFlagStatusGet(HT_UART2, UART_UARTSTA_TXIF))         /*!< UART2�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_TXIF);             /*!< ����жϱ�־          */         
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART2, UART_UARTSTA_RXIF))         /*!< UART2�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF);             /*!< ����жϱ�־          */ 
    }      
}

/*
*********************************************************************************************************
*                                   UART3 INTERRUPT HANDLER  #16+12
*
*********************************************************************************************************
*/

//��λ��ͨ��
void UART3_IRQHandler()
{ 
    static BaseType_t xHigherPriorityTaskWoken;  
    uint8_t RX_Data = 0;
    
    if(SET == HT_UART_ITFlagStatusGet(HT_UART3, UART_UARTSTA_TXIF))            /*!< UART3�����ж�         */
    {
		HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_TXIF);                /*!< ����жϱ�־          */   
		xSemaphoreGiveFromISR( gUart3Handle.xSemaphore, &xHigherPriorityTaskWoken );  
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );  
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART3, UART_UARTSTA_RXIF))            /*!< UART3�����ж�         */
    {
        HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_RXIF);                /*!< ����жϱ�־          */     
		RX_Data = HT_UART_ReceiveData(HT_UART3);
		FIFO_S_Put(&gUart3Handle.rxFIFO,RX_Data);
    }

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_1, UART_ISO7816STA_TXIF))  /*!< 7816�����ж�          */
    {
        HT_ISO7816_ClearITPendingBit(HT_ISO7816_1, UART_ISO7816STA_TXIF);      /*!< ����жϱ�־          */ 
    }  

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_1, UART_ISO7816STA_RXIF))  /*!< 7816�����ж�          */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_1, UART_ISO7816STA_RXIF);      /*!< ����жϱ�־          */         
    } 

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_1, UART_ISO7816STA_PRDIF)) /*!< 7816���������жϱ�־  */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_1, UART_ISO7816STA_PRDIF);     /*!< ����жϱ�־          */        
    }     
}

/*
*********************************************************************************************************
*                                   UART4 INTERRUPT HANDLER  #16+13
*
*********************************************************************************************************
*/


//����
void UART4_IRQHandler()
{
	static BaseType_t xHigherPriorityTaskWoken;  
    uint8_t RX_Data = 0;
    
    if(SET == HT_UART_ITFlagStatusGet(HT_UART4, UART_UARTSTA_TXIF))            /*!< UART4�����ж�         */
    {
        HT_UART_ClearITPendingBit(HT_UART4, UART_UARTSTA_TXIF);                /*!< ����жϱ�־          */   
		xSemaphoreGiveFromISR( gUart4Handle.xSemaphore, &xHigherPriorityTaskWoken );    //�ͷ��ź���
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );  	
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART4, UART_UARTSTA_RXIF))            /*!< UART4�����ж�         */
    {
        HT_UART_ClearITPendingBit(HT_UART4, UART_UARTSTA_RXIF);                /*!< ����жϱ�־          */     
		RX_Data = HT_UART_ReceiveData(HT_UART4);
		FIFO_S_Put(&gUart4Handle.rxFIFO,RX_Data);		
    }

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_0, UART_ISO7816STA_TXIF))  /*!< 7816�����ж�          */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_0, UART_ISO7816STA_TXIF);      /*!< ����жϱ�־          */         
    }  

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_0, UART_ISO7816STA_RXIF))  /*!< 7816�����ж�          */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_0, UART_ISO7816STA_RXIF);      /*!< ����жϱ�־          */         
    } 

    if(SET == HT_ISO7816_ITFlagStatusGet(HT_ISO7816_0, UART_ISO7816STA_PRDIF)) /*!< 7816���������жϱ�־  */
    {

        HT_ISO7816_ClearITPendingBit(HT_ISO7816_0, UART_ISO7816STA_PRDIF);     /*!< ����жϱ�־          */         
    }   
	
}

/*
*********************************************************************************************************
*                                   UART5 INTERRUPT HANDLER  #16+14
*
*********************************************************************************************************
*/

void UART5_IRQHandler()
{
    if(SET == HT_UART_ITFlagStatusGet(HT_UART5, UART_UARTSTA_TXIF))         /*!< UART5�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART5, UART_UARTSTA_TXIF);             /*!< ����жϱ�־          */        
    } 

    if(SET == HT_UART_ITFlagStatusGet(HT_UART5, UART_UARTSTA_RXIF))         /*!< UART5�����ж�         */
    {

        HT_UART_ClearITPendingBit(HT_UART5, UART_UARTSTA_RXIF);             /*!< ����жϱ�־          */         
    }      
}

/*
*********************************************************************************************************
*                                   TIMER_0 INTERRUPT HANDLER  #16+15
*
*********************************************************************************************************
*/

void TIMER_0_IRQHandler()
{
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR0, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
    {
        HT_TMR_ClearITPendingBit(HT_TMR0, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */
		CalcuElec();
        EMU_Proc();
        Feed_WDT();
    } 
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR0, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR0, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */        
    } 
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR0, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR0, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */         
    } 
    
}

/*
*********************************************************************************************************
*                                   TIMER_1 INTERRUPT HANDLER  #16+16
*
*********************************************************************************************************
*/

//void TIMER_1_IRQHandler()
//{

//    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR1, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
//    {

//        HT_TMR_ClearITPendingBit(HT_TMR1, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */        
//    }  
//    
//    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR1, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
//    {

//        HT_TMR_ClearITPendingBit(HT_TMR1, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */        
//    }  
//    
//    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR1, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
//    {

//        HT_TMR_ClearITPendingBit(HT_TMR1, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */        
//    } 
//    
//}

/*
*********************************************************************************************************
*                                   TIMER_2 INTERRUPT HANDLER  #16+17
*
*********************************************************************************************************
*/

void TIMER_2_IRQHandler()
{

    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR2, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR2, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */         
    }   
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR2, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR2, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */         
    }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR2, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR2, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */         
    } 
    
}

/*
*********************************************************************************************************
*                                   TIMER_3 INTERRUPT HANDLER  #16+18
*
*********************************************************************************************************
*/

void TIMER_3_IRQHandler()
{

    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR3, TMR_TMRIF_PRDIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR3, TMR_TMRIF_PRDIF);                    /*!< ����жϱ�־       */        
    }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR3, TMR_TMRIF_CAPIF))                /*!< �����ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR3, TMR_TMRIF_CAPIF);                    /*!< ����жϱ�־       */         
    }  
    
    if(SET == HT_TMR_ITFlagStatusGet(HT_TMR3, TMR_TMRIF_CMPIF))                /*!< �Ƚ��ж�           */
    {

        HT_TMR_ClearITPendingBit(HT_TMR3, TMR_TMRIF_CMPIF);                    /*!< ����жϱ�־       */        
    } 
    
}

/*
*********************************************************************************************************
*                                     TBS INTERRUPT HANDLER  #16+19
*
*********************************************************************************************************
*/

void TBS_IRQHandler()
{

    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_TMPIF))                 /*!< �¶Ȳ����ж�          */
    {
        
        HT_TBS_ClearITPendingBit(TBS_TBSIF_TMPIF);                     /*!< ����жϱ�־          */
    }  
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_VBATIF))                /*!< ��ز����ж�          */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_VBATIF);                    /*!< ����жϱ�־          */        
    }  
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_ADC0IF))                /*!< ADC0�����ж�          */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_ADC0IF);                    /*!< ����жϱ�־          */        
    }   
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_ADC1IF))                /*!< ADC1�����ж�          */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_ADC1IF);                    /*!< ����жϱ�־          */        
    }   
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_VBATCMPIF))             /*!< ��ص�ѹ�Ƚ��ж�      */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_VBATCMPIF);                 /*!< ����жϱ�־          */       
    }  
    
    if(SET == HT_TBS_ITFlagStatusGet(TBS_TBSIF_VCCIF))                 /*!< ��Դ�����ж�          */
    {

        HT_TBS_ClearITPendingBit(TBS_TBSIF_VCCIF);                     /*!< ����жϱ�־          */         
    }       
}

/*
*********************************************************************************************************
*                                    RTC INTERRUPT HANDLER  #16+20
*
*********************************************************************************************************
*/
extern __IO uint32_t gSysSecond;
#if 0
void RTC_IRQHandler(void)
{
    if (HT_RTC->RTCIF & 0x01)
    {
        HT_RTC->RTCIF &= (~0x01);
        gSysSecond++;
    }
}
#else
void RTC_IRQHandler()
{
    uint8_t Flag = HT_RTC->RTCIF;
    HT_RTC->RTCIF=0;                                               /*!< ����жϱ�־          */
    
    if(Flag & RTC_RTCIF_SECIF)                                     /*!< ���ж�                */
    {
		//gSysSecond++;
    }       
    
    if(Flag & RTC_RTCIF_MINIF)                                     /*!< ���ж�                */
    {
       
    }  
    
    if(Flag & RTC_RTCIF_HRIF)                                      /*!< Сʱ�ж�              */
    {
      
    }

    if(Flag & RTC_RTCIF_DAYIF)                                     /*!< ���ж�                */
    {
        
    }  

    if(Flag & RTC_RTCIF_MTHIF)                                     /*!< ���ж�                */
    {
       
    }  

    if(Flag & RTC_RTCIF_RTC1IF)                                    /*!< RTC��ʱ��1�ж�        */
    {
        
    }  

    if(Flag & RTC_RTCIF_RTC2IF)                                    /*!< RTC��ʱ��2�ж�        */
    {
       
    }  

    if(Flag & RTC_RTCIF_ALMIF)                                     /*!< �����ж�              */
    {
       
    }      
}
#endif
/*
*********************************************************************************************************
*                                    I2C INTERRUPT HANDLER  #16+21
*
*********************************************************************************************************
*/

void I2C_IRQHandler()
{
    
}

/*
*********************************************************************************************************
*                                    SPI0 INTERRUPT HANDLER  #16+22
*
*********************************************************************************************************
*/

void SPI0_IRQHandler()
{

    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI0,SPI_SPISTA_MODF))                 /*!< SPI0ģʽ���ϱ�־λ     */
    {

        HT_SPI_ClearITPendingBit(HT_SPI0, SPI_SPISTA_MODF);                    /*!< ����жϱ�־          */
    }    

    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI0, SPI_SPISTA_SPIF))                /*!< SPI0���ݴ�����ɱ�־λ*/
    {

        HT_SPI_ClearITPendingBit(HT_SPI0, SPI_SPISTA_SPIF);                    /*!< ����жϱ�־          */        
    }     

}

/*
*********************************************************************************************************
*                                  WDT/SPI1 INTERRUPT HANDLER  #16+23
*
*********************************************************************************************************
*/
#if defined  HT6x1x
void WDT_IRQHandler()
{
    
}
#elif defined HT6x2x
void SPI1_IRQHandler()
{
    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI1,SPI_SPISTA_MODF))                 /*!< SPI1ģʽ���ϱ�־λ     */
    {

        HT_SPI_ClearITPendingBit(HT_SPI1, SPI_SPISTA_MODF);                    /*!< ����жϱ�־          */
    }    

    if(SET == HT_SPI_ITFlagStatusGet(HT_SPI1, SPI_SPISTA_SPIF))                /*!< SPI1���ݴ�����ɱ�־λ*/
    {

        HT_SPI_ClearITPendingBit(HT_SPI1, SPI_SPISTA_SPIF);                    /*!< ����жϱ�־          */        
    }     
}
#endif

/*
*********************************************************************************************************
*                                 FREQ TEST INTERRUPT HANDLER  #16+24
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void SelfTestF_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                               TIMER_4/EMU INTERRUPT HANDLER  #16+25
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void TIMER_4_IRQHandler(void)
{
    
}
#elif defined HT501x
void EMU_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                               TIMER_5/DMA INTERRUPT HANDLER  #16+26
*
*********************************************************************************************************
*/

#if defined  HT6x2x
void TIMER_5_IRQHandler(void)
{
    
}
#elif defined HT501x
void DMA_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                               UART6/KEY INTERRUPT HANDLER  #16+27
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void UART6_IRQHandler(void)
{
    
}
#elif defined HT501x
void KEY_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                                 EXTI7 INTERRUPT HANDLER  #16+28
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void EXTI7_IRQHandler(void)
{
    
}
#endif
/*
*********************************************************************************************************
*                                 EXTI8 INTERRUPT HANDLER  #16+29
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void EXTI8_IRQHandler(void)
{
    
}
#endif

/*
*********************************************************************************************************
*                                 EXTI9 INTERRUPT HANDLER  #16+30
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void EXTI9_IRQHandler(void)
{
    
}
#endif
/*
*********************************************************************************************************
*                                 DMA INTERRUPT HANDLER  #16+31
*
*********************************************************************************************************
*/
#if defined  HT6x2x
void DMA_IRQHandler(void)
{
    HT_DMA_ClearITPendingBit(DMA_DMAIF_TCIF1);
    DMA_Cmd(HT_DMA_Channel1, DISABLE);  
    DMA_Cmd(HT_DMA_Channel0, DISABLE);  
}
#endif




