#ifndef __USART_H__
#define __USART_H__

#include "include.h"





void Init_Uart0(void);
void Init_Uart1(void);
void Init_Uart2(void);
void Init_Uart3(void);
void Init_Uart4(void);
void Init_Uart5(void);
void Init_Uart6(void);
void Init_Uart7(void);
void Init_Uart8(void);
			

void UartSend(HT_UART_TypeDef* USARTx, char *data, uint16_t len);
void UsartInit(void);

#endif //__USART_H__



