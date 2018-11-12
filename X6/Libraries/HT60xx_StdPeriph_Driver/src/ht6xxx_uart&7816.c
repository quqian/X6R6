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
* File         : ht6xxx_uart&7816.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_UART7816_C

#include "ht6xxx_uart&7816.h"
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
*                                         INITIAL UART MODULE
*
* ����˵��: ��ʼ��UARTģ��
*
* ��ڲ���: UARTx               ֻ����HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5��һ��
*
*           UART_InitStruct     UART��ʼ���ṹ��ָ�룬��Ҫ����7������: 
*                               1) UART_Logic      : UARTͨѶ�߼��趨
*                               2) UART_StopBits   : UARTͨѶֹͣλ�趨
*                               3) UART_WordLength : UARTͨѶ����λ�����趨
*                               4) UART_Parity     : UARTͨѶ��żУ���趨 
*                               5) UART_BaudRate   : UARTͨѶ������
*                               6) ReceiveEN       : UART����ʹ�ܿ���
*                               7) SendEN          : UART����ʹ�ܿ���
*
* ���ز���: ��                                      
* 
* ����˵��: 1) HT_UART3/HT_UART4��ISO7816���ܸ��ã��������ж�HT_UART3/HT_UART4��ʼ����������ʹ��
*              HT_ISO7816_0/HT_ISO7816_1���ܣ����Ӧ��ϵΪ��
*              HT_UART3 <----->HT_ISO7816_1             
*              HT_UART4 <----->HT_ISO7816_0
*           2) �����漰�������ʼ��㣬�����û��ڵ��ô˺���ʱӦ�����ú�ϵͳʱ�ӡ���������Ѿ����úò����ʣ�
*              �ٸı�ϵͳʱ�ӣ����������ý�ʧЧ
*           3) �û�������UART�Ĵ���ǰӦ��ʹ��UARTģ�飬����μ�HT_CMU_ClkCtrl1Config()����
*********************************************************************************************************
*/
void HT_UART_Init(HT_UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg,sysclk;
    
    UARTx->MODESEL = UART_MODESEL_UART;                                   /*!< ѡ��ΪUART����         */
                                                                          /*!< ֻ��Uart3/4��Ч        */

    tempreg = UARTx->UARTCON & (UART_UARTCON_TXIE | UART_UARTCON_RXIE);   /*!< ����TXIE��RXIE����     */
    
    if(UART_InitStruct->UART_Parity != UartParity_Disable )
    {
        tempreg |= UART_UARTCON_PARITYEN;
        tempreg |= UART_InitStruct->UART_Parity;                          /*!< ������żУ��λ         */
    }
    tempreg |= UART_InitStruct->UART_Logic;                               /*!< ����UARTͨѶ�߼�       */
    tempreg |= UART_InitStruct->UART_StopBits;                            /*!< ����ֹͣλ             */
    tempreg |= UART_InitStruct->UART_WordLength;                          /*!< ����ͨѶ���ݳ���       */
    
    if(UART_InitStruct->ReceiveEN != DISABLE)
    {
        tempreg |= UART_UARTCON_RXEN;                                     /*!< ���ý���ʹ��           */
    }
    if(UART_InitStruct->SendEN != DISABLE)
    {
        tempreg |= UART_UARTCON_TXEN;                                     /*!< ���÷���ʹ��           */
    }
    
    UARTx->UARTCON = tempreg;                                             /*!< ���üĴ���             */   
        
    sysclk = HT_CMU_SysClkGet()>>1;                                       /*!< ��ȡ��ǰϵͳʱ��Ƶ��/2 */
    UARTx->SREL = sysclk/UART_InitStruct->UART_BaudRate-1;                /*!< ���㲨����             */
    
    UARTx->IRCON &= ~UART_IRCON_IRTX;                                     /*!< �رպ�������������   */
} 

/*
*********************************************************************************************************
*                                         INITIAL IR MODULE
*
* ����˵��: ��ʼ������ģ��
*
* ��ڲ���: UARTx               ֻ����HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5��һ��
*
*           IR_InitStruct       IRģ���ʼ���ṹ��ָ�룬��Ҫ����3������: 
*                               1) UART_Module   : UARTͨѶģ���ʼ������ΪUART_InitTypeDef���ͽṹ��
*                               2) IR_Polarity   : ������������������ 
*                               3) IR_Duty       : ������Ʋ���ռ�ձ�����
*
* ���ز���: ��                                      
* 
* ����˵��: 1) HT_UART3/HT_UART4��ISO7816���ܸ��ã��������ж�HT_UART3/HT_UART4��ʼ����������ʹ��
*              HT_ISO7816_0/HT_ISO7816_1���ܣ����Ӧ��ϵΪ��
*              HT_UART3 <----->HT_ISO7816_1             
*              HT_UART4 <----->HT_ISO7816_0
*           2) �����漰�������ʼ��㣬�����û��ڵ��ô˺���ʱӦ�����ú�ϵͳʱ�ӡ���������Ѿ����úò����ʣ�
*              �ٸı�ϵͳʱ�ӣ����������ý�ʧЧ
*           3) �û�������UART�Ĵ���ǰӦ��ʹ��UARTģ�飬����μ�HT_CMU_ClkCtrl1Config()����
*********************************************************************************************************
*/
void HT_IR_Init(HT_UART_TypeDef* UARTx, IR_InitTypeDef* IR_InitStruct)
{
    /*  assert_param  */
    
    HT_UART_Init(UARTx, &IR_InitStruct->UARTModule);                    /*!< ��ʼ��Uartģ��           */
    
    UARTx->IRCON = (IR_InitStruct->IRPolarity | UART_IRCON_IRTX);       /*!< ʹ�ܺ����Լ����Ƽ������� */
    
    UARTx->IRDUTY = IR_InitStruct->IRDuty;                              /*!< ����ռ�ձ�               */
} 

/*
*********************************************************************************************************
*                                      INITIAL ISO7816 MODULE
*
* ����˵��: ��ʼ��ISO7816ģ��
*
* ��ڲ���: ISO7816x            ֻ����HT_ISO7816_0/HT_ISO7816_1��һ��
*
*           ISO7816_InitStruct  ISO7816��ʼ���ṹ��ָ�룬��Ҫ����6������: 
*                               1) ISO7816_Parity     : ISO7816��żУ��λѡ��
*                               2) ISO7816_RepeatTime : ISO7816�ط����մ����趨
*                               3) AutoRxEN           : ISO7816�Զ�����ʹ�ܿ���
*                               4) AutoTxEN           : ISO7816�Զ�����ʹ�ܿ���
*                               5) ISO7816_AckLen     : ISO7816��Ӧλ�����趨
*                               6) ISO7816_BaudRate   : ISO7816�������趨
*
* ���ز���: ��                                      
* 
* ����˵��: 1) HT_UART3/HT_UART4��ISO7816���ܸ��ã��������ж�HT_ISO7816_0/HT_ISO7816_1��ʼ����������ʹ��
*              HT_UART3/HT_UART4���ܣ����Ӧ��ϵΪ��
*              HT_UART3 <----->HT_ISO7816_1             
*              HT_UART4 <----->HT_ISO7816_0 
*           2) �����漰�������ʼ��㣬�����û��ڵ��ô˺���ʱӦ�����ú�ϵͳʱ�ӡ���������Ѿ����úò����ʣ�
*              �ٸı�ϵͳʱ�ӣ����������ý�ʧЧ
*           3) �û�������ISO7816�Ĵ���ǰӦ��ʹ��ISO7816ģ�飬����μ�HT_CMU_ClkCtrl1Config()����
*********************************************************************************************************
*/
void HT_ISO7816_Init(HT_ISO7816_TypeDef* ISO7816x, ISO7816_InitTypeDef* ISO7816_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg, sysclk;
    
    ISO7816x->MODESEL = UART_MODESEL_7816;                                   /*!< ѡ��ΪISO7816����      */
    
    tempreg = ISO7816x->ISO7816CON & UART_ISO7816CON_IEALL;                  /*!< �����ж�ʹ�ܲ���       */
    
    if(ISO7816_InitStruct->AutoRxEN != DISABLE)
    {
        tempreg |= UART_ISO7816CON_AUTORXEN;                                 /*!< ʹ���Զ����չ���       */ 
    }
    if(ISO7816_InitStruct->AutoTxEN != DISABLE)
    {
        tempreg |= UART_ISO7816CON_AUTOTXEN;                                 /*!< ʹ���Զ��ط�����       */ 
    }    
    
    tempreg |= ISO7816_InitStruct->ISO7816_Parity;                           /*!< ������ż��             */
    tempreg |= ISO7816_InitStruct->ISO7816_RepeatTime;                       /*!< �����ط����մ���       */
    tempreg |= ISO7816_InitStruct->ISO7816_AckLen;                           /*!< ������Ӧλ����         */
    
    ISO7816x->ISO7816CON = tempreg;                                          /*!< ���üĴ���             */ 
        
    sysclk = HT_CMU_SysClkGet()>>1;                                          /*!< ��ȡ��ǰϵͳʱ��Ƶ��/2 */
    ISO7816x->SREL = sysclk/ISO7816_InitStruct->ISO7816_BaudRate-1;          /*!< ���㲨����             */
    
    ISO7816x->IRCON &= ~UART_IRCON_IRTX;                                     /*!< �رպ�������������   */
} 

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE UART INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�UART�ж�
*
* ��ڲ���: UARTx      ֻ����HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5��һ��
*
            ITEn       UART�ж�����λ������Ϊ���²����������
*                        @arg UART_UARTCON_TXIE
*                        @arg UART_UARTCON_RXIE
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_UART_ITConfig(HT_UART_TypeDef* UARTx, uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    uint32_t it;
    
    it = (uint32_t)ITEn & (UART_UARTCON_RXIE | UART_UARTCON_TXIE);
    
    if (NewState != DISABLE)
    {       
        UARTx->UARTCON |= it;                                             /*!< ʹ��UART�ж�           */
    }
    else
    {
        UARTx->UARTCON &= ~it;                                            /*!< �ر�UART�ж�           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED UART INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦUART�жϱ�־
*
* ��ڲ���: UARTx      ֻ����HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5��һ��
*
*           ITFlag     ��Ҫ�����ĳ��UART�жϣ�����Ϊ���²���:
*                        @arg UART_UARTSTA_TXIF
*                        @arg UART_UARTSTA_RXIF
*                        @arg UART_UARTSTA_PARITY    ע���˱�־λ�����жϱ�־λ
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_UART_ITFlagStatusGet(HT_UART_TypeDef* UARTx, uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (UARTx->UARTSTA & ITFlag)
    {       
        return SET;                                            /*!< UART Interrupt Flag is set   */
    }
    else
    {
        return RESET;                                          /*!< UART Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                CLEAR UART INTERRUPT FLAG
*
* ����˵��: ���UART�жϱ�־
*
* ��ڲ���: UARTx      ֻ����HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5��һ��
*
*           ITFlag     ��Ҫ�����ĳ��UART�жϱ�־������Ϊ���²����������:
*                        @arg UART_UARTSTA_TXIF
*                        @arg UART_UARTSTA_RXIF
*                        @arg UART_UARTSTA_PARITY    ע���˱�־λ�����жϱ�־λ
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_UART_ClearITPendingBit(HT_UART_TypeDef* UARTx, uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    UARTx->UARTSTA &= ~((uint32_t)ITFlag);                /*!< Clear UART Interrupt Flag    */
    
}


/*
*********************************************************************************************************
*                              ENABLE OR DISABLE ISO7816 INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�ISO7816�ж�
*
* ��ڲ���: ISO7816x  ֻ����HT_ISO7816_0/HT_ISO7816_1��һ��
*
*           ITEn       ISO7816�ж�����λ������Ϊ���²����������
*                        @arg UART_ISO7816CON_TXIE
*                        @arg UART_ISO7816CON_RXIE
*                        @arg UART_ISO7816CON_PRDIE
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_ISO7816_ITConfig(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    uint32_t it;
    
    it = (uint32_t)ITEn & UART_ISO7816CON_IEALL;
    
    if (NewState != DISABLE)
    {       
        ISO7816x->ISO7816CON |= it;                                   /*!< ʹ��ISO7816�ж�         */
    }
    else
    {
        ISO7816x->ISO7816CON &= ~it;                                  /*!< �ر�ISO7816�ж�         */
    } 
}

/*
*********************************************************************************************************
*                         GET SPECIFIED ISO7816 INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦISO7816�жϱ�־״̬
*
* ��ڲ���: ISO7816x  ֻ����HT_ISO7816_0/HT_ISO7816_1��һ��
*
*           ITFlag     ��Ҫ����ĳ��ISO7816�жϣ�����Ϊ���²���:
*                        @arg UART_ISO7816STA_TXIF
*                        @arg UART_ISO7816STA_RXIF
*                        @arg UART_ISO7816STA_PRDIF
*                        @arg UART_ISO7816STA_TXERRSTAT    ע���˱�־λ�����жϱ�־λ
*                        @arg UART_ISO7816STA_RXERRSTAT    ע���˱�־λ�����жϱ�־λ
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_ISO7816_ITFlagStatusGet(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (ISO7816x->ISO7816STA & ITFlag)
    {       
        return SET;                                        /*!< ISO7816 Interrupt Flag is set   */
    }
    else
    {
        return RESET;                                      /*!< ISO7816 Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                CLEAR ISO7816 INTERRUPT FLAG
*
* ����˵��: ���ISO7816�жϱ�־
*
* ��ڲ���: ISO7816x  ֻ����HT_ISO7816_0/HT_ISO7816_1��һ��
*
*           ITFlag     ��Ҫ�����ĳ��ISO7816�жϱ�־������Ϊ���²����������:
*                        @arg UART_ISO7816STA_TXIF
*                        @arg UART_ISO7816STA_RXIF
*                        @arg UART_ISO7816STA_PRDIF
*                        @arg UART_ISO7816STA_TXERRSTAT    ע���˱�־λ�����жϱ�־λ
*                        @arg UART_ISO7816STA_RXERRSTAT    ע���˱�־λ�����жϱ�־λ
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_ISO7816_ClearITPendingBit(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    ISO7816x->ISO7816STA &= ~((uint32_t)ITFlag);             /*!< Clear ISO7816 Interrupt Flag    */
    
}

/*
*********************************************************************************************************
*                                        WRITE DATA TO UART BUFFER
*
* ����˵��: ��Uart buffer��д����
*
* ��ڲ���: UARTx      ֻ����HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5��һ��
*
*           Data       ��Uart Buffer��Ҫд������
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_UART_SendData(HT_UART_TypeDef* UARTx, uint16_t Data)
{
    /*  assert_param  */
    
    
    UARTx->SBUF = Data;
}

/*
*********************************************************************************************************
*                                         READ UART BUFFER
*
* ����˵��: ��Uart������
*
* ��ڲ���: UARTx      ֻ����HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5��һ��
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
uint8_t HT_UART_ReceiveData(HT_UART_TypeDef* UARTx)
{
    /*  assert_param  */
        
    return UARTx->SBUF;
}

/*
*********************************************************************************************************
*                                        WRITE DATA TO ISO7816 BUFFER
*
* ����˵��: ��ISO7816 buffer��д����
*
* ��ڲ���: ISO7816x   ֻ����HT_ISO7816_0/HT_ISO7816_1��һ��
*
*           Data       ��ISO7816 Buffer��Ҫд������
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_ISO7816_SendData(HT_ISO7816_TypeDef* ISO7816x, uint8_t Data)
{
    /*  assert_param  */
    
    
    ISO7816x->SBUF = Data;
}

/*
*********************************************************************************************************
*                                         READ ISO7816 BUFFER
*
* ����˵��: ��ISO7816������
*
* ��ڲ���: ISO7816x   ֻ����HT_ISO7816_0/HT_ISO7816_1��һ��
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
uint8_t HT_ISO7816_ReceiveData(HT_ISO7816_TypeDef* ISO7816x)
{
    /*  assert_param  */
    
    
    return ISO7816x->SBUF;
}

/*
*********************************************************************************************************
*                                   ENABLE OR DISABLE RX DIGITAL FILTER
*
* ����˵��: ʹ�ܻ�ر��ⲿ�жϵ������˲�����
*
* ��ڲ���: RXFLTEn     �ж����������˲�����λ������Ϊ���²����������
*                        @arg INT_PINFLT_RXFLT_RX0
*                        @arg INT_PINFLT_RXFLT_RX1
*                        @arg INT_PINFLT_RXFLT_RX2
*                        @arg INT_PINFLT_RXFLT_RX3
*                        @arg INT_PINFLT_RXFLT_RX4
*                        @arg INT_PINFLT_RXFLT_RX5
*                        @arg INT_PINFLT_RXFLT_RX6
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_UART_RXFilterConfig(uint32_t RXFLTEn,FunctionalState NewState)
{
    /*  assert_param  */
  
    RXFLTEn &= INT_PINFLT_RXFLT;  
    if (NewState != DISABLE)
    {       
        HT_INT->PINFLT |= RXFLTEn;            /*!< ʹ�������˲�         */
    }
    else
    {
        HT_INT->PINFLT &= ~RXFLTEn;           /*!< �ر������˲�         */
    }    
}
