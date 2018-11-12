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
* File         : ht6xxx_uart&7816.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_UART7816_H__
#define __HT6XXX_UART7816_H__

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
* @brief  UARTͨѶ�߼�����
*/
typedef enum
{ 
    UartLogicPositive = UART_UARTCON_UNEG_POSITIVE,     /*!< UARTͨѶѡ�����߼�             */
    UartLogicNegetive = UART_UARTCON_UNEG_NEGETIVE,     /*!< UARTͨѶѡ���߼�             */
#if defined  HT6x2x
    UartLogicAuto = UART_UARTCON_UNEG_AUTO,             /*!< UARTͨѶѡ������߼��Զ��ж�   */
#endif   
}UARTLogic_TypeDef;                                     /*!< end of group UARTLogic_TypeDef */   

/* 
* @brief  UARTֹͣλ����
*/
typedef enum
{ 
    OneStopBits = UART_UARTCON_STOPSEL_1Bit,            /*!< 1λֹͣλ                         */
    TwoStopBits = UART_UARTCON_STOPSEL_2Bit,            /*!< 2λֹͣλ                         */
}UARTStopBits_TypeDef;                                  /*!< end of group UARTStopBits_TypeDef */

/* 
* @brief  UART���ݳ���λ����
*/
typedef enum
{ 
    WordLength_7Bits = UART_UARTCON_LENSEL_7Bit,        /*!< 7λ����λ                          */
    WordLength_8Bits = UART_UARTCON_LENSEL_8Bit,        /*!< 8λ����λ                          */
}UARTWordLength_TypeDef;                                /*!< end of group UARTWordLength_TypeDef*/

/* 
* @brief  UART��żУ��λ����
*/
typedef enum
{ 
    UartParity_Disable = 0xff,                          /*!< û����żУ��λ                     */
    UartParity_Always1 = UART_UARTCON_PARITYSEL_1,      /*!< ��żУ��λ�㶨Ϊ1                  */
    UartParity_Always0 = UART_UARTCON_PARITYSEL_0,      /*!< ��żУ��λ�㶨Ϊ0                  */
    UartParity_ODD = UART_UARTCON_PARITYSEL_ODD,        /*!< ��У��                             */
    UartParity_EVEN = UART_UARTCON_PARITYSEL_EVEN,      /*!< żУ��                             */
}UARTParity_TypeDef;                                    /*!< end of group UARTParity_TypeDef    */


/* 
* @brief  UARTģ���ʼ���ṹ����
*/
typedef struct
{
    UARTLogic_TypeDef  UART_Logic;                      /*!< �����߼�����                       */                                                                          
    UARTStopBits_TypeDef  UART_StopBits;                /*!< ����ֹͣλ����                     */                                                                         
    UARTWordLength_TypeDef  UART_WordLength;            /*!< ����ͨѶ���ݳ�������               */ 
    UARTParity_TypeDef  UART_Parity;                    /*!< ������żУ��λ����                 */
    uint32_t  UART_BaudRate;                            /*!< ���ڲ�����, ����1200, 2400         */
    FunctionalState  ReceiveEN;                         /*!< ���ڽ���ʹ�ܿ���                   */ 
    FunctionalState  SendEN;                            /*!< ���ڷ���ʹ�ܿ���                   */ 
}UART_InitTypeDef;                                      /*!< end of group UART_InitTypeDef      */

/* 
* @brief  �������������Զ���
*/
typedef enum
{ 
    IRPolarityPositive = UART_IRCON_IRLVL_POSITIVE,     /*!< ��������������Ϊ��               */
    IRPolarityNegetive = UART_IRCON_IRLVL_NEGETIVE,     /*!< ��������������Ϊ��               */
}IRPolarity_TypeDef;                                    /*!< end of group IRPolarity_TypeDef    */

/* 
* @brief  �������������Զ���
*/
typedef enum
{ 
    IRDuty50 = UART_IRDUTY_IRDUTY_50,                   /*!< ������Ʋ���ռ�ձ�50%              */
    IRDuty25 = UART_IRDUTY_IRDUTY_25,                   /*!< ������Ʋ���ռ�ձ�25%              */
    IRDuty12P5 = UART_IRDUTY_IRDUTY_12P5,               /*!< ������Ʋ���ռ�ձ�12.5%            */
    IRDuty6P25 = UART_IRDUTY_IRDUTY_6P25,               /*!< ������Ʋ���ռ�ձ�6.25%            */
}IRDuty_TypeDef;                                        /*!< end of group IRDuty_TypeDef        */

/* 
* @brief  ����ģ���ʼ���ṹ����
*/
typedef struct
{
    UART_InitTypeDef  UARTModule;                        /*!< UARTͨѶ��ʼ��                     */                                                                          
    IRPolarity_TypeDef  IRPolarity;                      /*!< ������������������               */                                                                         
    IRDuty_TypeDef  IRDuty;                              /*!< ������Ʋ���ռ�ձ�����             */ 
}IR_InitTypeDef;                                         /*!< end of group IR_InitTypeDef        */

/* 
* @brief  ISO7816��żУ��λ����
*/
typedef enum
{ 
    ISO7816Parity_ODD = UART_ISO7816CON_7816PARITY_ODD,  /*!< ��У��                             */
    ISO7816Parity_EVEN = UART_ISO7816CON_7816PARITY_EVEN,/*!< żУ��                             */
}ISO7816Parity_TypeDef;                                  /*!< end of group ISO7816Parity_TypeDef */

/* 
* @brief  ISO7816�Զ��ط����մ�������
*/
typedef enum
{ 
    Repeat_Zero = UART_ISO7816CON_REPTR_0,               /*!< �Զ��ط����մ�����Ϊ0              */
    Repeat_One = UART_ISO7816CON_REPTR_1,                /*!< �Զ��ط����մ�����Ϊ1              */
    Repeat_Two = UART_ISO7816CON_REPTR_2,                /*!< �Զ��ط����մ�����Ϊ2              */
    Repeat_Three = UART_ISO7816CON_REPTR_3,              /*!< �Զ��ط����մ�����Ϊ3              */
}ISO7816Repeat_TypeDef;                                  /*!< end of group ISO7816Repeat_TypeDef */

/* 
* @brief  ISO7816��Ӧλ���ȶ���
*/
typedef enum
{ 
    ACKLen_1Bit = UART_ISO7816CON_ACKLEN_1Bit,            /*!< ACK��Ӧ����1Bit                   */
    ACKLen_1P5Bit = UART_ISO7816CON_ACKLEN_1P5Bit,        /*!< ACK��Ӧ����1.5Bit                 */
    ACKLen_2Bit = UART_ISO7816CON_ACKLEN_2Bit,            /*!< ACK��Ӧ����2Bit                   */
}ISO7816ACKLen_TypeDef;                                   /*!< end of group ISO7816ACKLen_TypeDef*/    

/* 
* @brief  ISO7816ģ���ʼ���ṹ����
*/
typedef struct
{
    ISO7816Parity_TypeDef  ISO7816_Parity;                /*!< ISO7816��ż�趨                   */                                                                          
    ISO7816Repeat_TypeDef  ISO7816_RepeatTime;            /*!< ISO7816�ط����մ����趨           */
    FunctionalState  AutoRxEN;                            /*!< �Զ����չ����趨                  */ 
    FunctionalState  AutoTxEN;                            /*!< �Զ��ط������趨                  */ 
    ISO7816ACKLen_TypeDef  ISO7816_AckLen;                /*!< ISO7816��Ӧ�����趨               */ 
    uint32_t  ISO7816_BaudRate;                           /*!< ISO7816�������趨������9600       */ 
}ISO7816_InitTypeDef;                                     /*!< end of group ISO7816_InitTypeDef  */


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
void HT_UART_Init(HT_UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void HT_IR_Init(HT_UART_TypeDef* UARTx, IR_InitTypeDef* IR_InitStruct);
void HT_ISO7816_Init(HT_ISO7816_TypeDef* ISO7816x, ISO7816_InitTypeDef* ISO7816_InitStruct);
void HT_UART_ITConfig(HT_UART_TypeDef* UARTx, uint8_t ITEn, FunctionalState NewState);
ITStatus HT_UART_ITFlagStatusGet(HT_UART_TypeDef* UARTx, uint8_t ITFlag);
void HT_UART_ClearITPendingBit(HT_UART_TypeDef* UARTx, uint8_t ITFlag);
void HT_ISO7816_ITConfig(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITEn, FunctionalState NewState);
ITStatus HT_ISO7816_ITFlagStatusGet(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag);
void HT_ISO7816_ClearITPendingBit(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag);
void HT_UART_SendData(HT_UART_TypeDef* UARTx, uint16_t Data);
uint8_t HT_UART_ReceiveData(HT_UART_TypeDef* UARTx);
void HT_ISO7816_SendData(HT_ISO7816_TypeDef* ISO7816x, uint8_t Data);
uint8_t HT_ISO7816_ReceiveData(HT_ISO7816_TypeDef* ISO7816x);
void HT_UART_RXFilterConfig(uint32_t RXFLTEn,FunctionalState NewState);






#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_UART7816_H__ */ 
