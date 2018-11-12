/*usart.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "include.h"
#include "flash.h"


__IO uint8_t UART_SendState[7];


FIFO_S_t FIFO_usart0;
FIFO_S_t FIFO_usart1;
FIFO_S_t FIFO_usart2;
FIFO_S_t FIFO_usart3;
FIFO_S_t FIFO_usart4;
FIFO_S_t FIFO_usart5;
FIFO_S_t FIFO_usart6;
FIFO_S_t FIFO_usart7;
FIFO_S_t FIFO_usart8;



/***********************************************************************/
//******************************************************************
//! \brief  	Set_Uart_Bandrate
//! \param  	none
//! \retval 	
//! \note   	设置串口波特率
//! \note  
//******************************************************************
static void Set_Uart_Bandrate(HT_UART_TypeDef* usart, uint32_t bps)
{
	//bps = Fsys/2*(SREL+1)  
	//初值SREL=Fsys/2*bps-1, Fsys=44MHz
	usart->SREL=44040192/(2*bps)-1;		
}

//******************************************************************
//! \brief  	Init_Uart0
//! \param  	none
//! \retval 	
//! \note   	串口0初始化
//! \note  
//******************************************************************
void Init_Uart0(void)
{
	//PC3 PC2 as TX0 RX0
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);
	
	NVIC_DisableIRQ(UART0_IRQn);				//禁止串口中断
	HT_UART0->MODESEL=0;						//uart function
	HT_UART0->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART0->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART0->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART0, 115200);

	NVIC_ClearPendingIRQ(UART0_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART0_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART0_IRQn);					//使能串口中断
}
//******************************************************************
//! \brief  	Init_Uart1
//! \param  	none
//! \retval 	
//! \note   	串口1初始化
//! \note  
//******************************************************************
void Init_Uart1(void)
{
	//PC0 PC1 as TX1 RX1
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_0|GPIO_Pin_1, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART1EN, ENABLE);
	
	NVIC_DisableIRQ(UART1_IRQn);				//禁止串口中断
	HT_UART1->MODESEL=0;						//uart function
	HT_UART1->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART1->UARTCON = 0x003F;					//正逻辑,8位数据，奇校验，2停止位,接收发送使能
	HT_UART1->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART1, 115200);

	NVIC_ClearPendingIRQ(UART1_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART1_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART1_IRQn);					//使能串口中断
}
//******************************************************************
//! \brief  	Init_Uart2
//! \param  	none
//! \retval 	
//! \note   	串口2初始化
//! \note  
//******************************************************************
void Init_Uart2(void)
{
	//PC11 PC12 as TX2 RX2	
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_11|GPIO_Pin_12, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART2EN, ENABLE);
	
	NVIC_DisableIRQ(UART2_IRQn);				//禁止串口中断
	HT_UART2->MODESEL=0;						//uart function
	HT_UART2->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART2->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART2->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART2, 2400);

	NVIC_ClearPendingIRQ(UART2_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART2_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART2_IRQn);					//使能串口中断
}
//******************************************************************
//! \brief  	Init_Uart3
//! \param  	none
//! \retval 	
//! \note   	串口3初始化
//! \note  
//******************************************************************
void Init_Uart3(void)
{
	//PE5 PE4 as TX3 RX3
	GPIO_Cfg(HT_GPIOE, GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);
	
	NVIC_DisableIRQ(UART3_IRQn);				//禁止串口中断
	HT_UART3->MODESEL=0;						//uart function
	HT_UART3->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART3->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART3->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART3, 2400);

	NVIC_ClearPendingIRQ(UART3_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART3_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART3_IRQn);					//使能串口中断
}
//******************************************************************
//! \brief  	Init_Uart4
//! \param  	none
//! \retval 	
//! \note   	串口4初始化
//! \note  
//******************************************************************
void Init_Uart4(void)
{
	//PE1 PE2 as TX4 RX4
	GPIO_Cfg(HT_GPIOE, GPIO_Pin_1|GPIO_Pin_2, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART4_7816_0EN, ENABLE);
	
	NVIC_DisableIRQ(UART4_IRQn);				//禁止串口中断
	HT_UART4->MODESEL=0;						//uart function
	HT_UART4->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART4->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART4->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART4, 2400);

	NVIC_ClearPendingIRQ(UART4_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART4_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART4_IRQn);					//使能串口中断
}
//******************************************************************
//! \brief  	Init_Uart5
//! \param  	none
//! \retval 	
//! \note   	串口5初始化
//! \note  
//******************************************************************
void Init_Uart5(void)
{
	//PC5 PC4 as TX5 RX5
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_AF2, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART5EN, ENABLE);
	
	NVIC_DisableIRQ(UART5_IRQn);				//禁止串口中断
	HT_UART5->MODESEL=0;						//uart function
	HT_UART5->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART5->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART5->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART5, 2400);

	NVIC_ClearPendingIRQ(UART5_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART5_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART5_IRQn);					//使能串口中断
}
//******************************************************************
//! \brief  	Init_Uart6
//! \param  	none
//! \retval 	
//! \note   	串口6初始化
//! \note  
//******************************************************************
void Init_Uart6(void)
{
	//PG12 PG11 as TX6 RX6	
	GPIO_Cfg(HT_GPIOG, GPIO_Pin_11|GPIO_Pin_12, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART6EN, ENABLE);

	NVIC_DisableIRQ(UART6_IRQn);				//禁止串口中断
	HT_UART6->MODESEL=0;						//uart function
	HT_UART6->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART6->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART6->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART6, 2400);

	NVIC_ClearPendingIRQ(UART6_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART6_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART6_IRQn);					//使能串口中断
}
//******************************************************************
//! \brief  	Init_Uart7
//! \param  	none
//! \retval 	
//! \note   	串口7初始化
//! \note  
//******************************************************************
void Init_Uart7(void)
{

}
//******************************************************************
//! \brief  	Init_Uart8
//! \param  	none
//! \retval 	
//! \note   	串口8初始化
//! \note  
//******************************************************************
void Init_Uart8(void)
{

}
//******************************************************************
//! \brief  	USART_SendData
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
static void USART_SendData(HT_UART_TypeDef* USARTx, uint8_t Data)
{
	uint8_t index ;
	
	if(HT_UART0 == USARTx){
		index = 0;
	}else if(HT_UART1 == USARTx){
		index = 1;
	}else if(HT_UART2 == USARTx){
		index = 2;
	}else if(HT_UART3 == USARTx){
		index = 3;
	}else if(HT_UART4 == USARTx){
		index = 4;
	}else if(HT_UART5 == USARTx){
		index = 5;
	}else if(HT_UART6 == USARTx){
		index = 6;
	}

	//等待发送完成
	while(UART_SendState[index] != 0){
	}
	
	UART_SendState[index] = 1;
	USARTx->SBUF = Data;
}
//******************************************************************
//! \brief  	UartSend
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
void UartSend(HT_UART_TypeDef* USARTx, char *data, uint16_t len)
{
	int i = 0;
	for(i=0;i<len;i++){
		USART_SendData(USARTx,data[i]);
	}
}
//******************************************************************
//! \brief  	UART0_IRQHandler
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
//void UART0_IRQHandler(void)
//{
//	uint8_t ch= ch;
//	
//	//receive interrupt 
//	if((HT_UART0->UARTSTA & 0x02)!=0){
//		
//		ch = HT_UART0->SBUF;
//		
//		FIFO_S_Put(&FIFO_usart0, ch);
//		//clear recevie interrupt flag
//		HT_UART0->UARTSTA &= 0x05;
//	}

//	//send interrupt
//	if((HT_UART0->UARTSTA & 0x01)!=0){
//		UART_SendState[0] = 0;
//		//clear send interrupt flag 
//		HT_UART0->UARTSTA &= 0x06;
//	}
//}
//******************************************************************
//! \brief  	UART1_IRQHandler
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
//void UART1_IRQHandler(void)
//{
//	uint8_t ch= ch;
//	
//	//receive interrupt 
//	if((HT_UART1->UARTSTA & 0x02)!=0){
//		ch = HT_UART1->SBUF;
//		xQueueSendToBackFromISR(uart1Queue, &ch, 0);
//		HT_UART1->UARTSTA &= 0x05;
//	}

//	//send interrupt
//	if((HT_UART1->UARTSTA & 0x01)!=0){
//		HT_UART1->UARTSTA &= 0x06;
//  
//		UART_SendState[1] = 0;
//	}
//}
//******************************************************************
//! \brief  	UART2_IRQHandler
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
//void UART2_IRQHandler(void)
//{
//	uint8_t ch = ch;
//	
//	//receive interrupt 
//	if((HT_UART2->UARTSTA & 0x02)!=0){
//		ch = HT_UART2->SBUF;
//		
//		HT_UART2->UARTSTA &= 0x05;
//	}

//	//send interrupt
//	if((HT_UART2->UARTSTA & 0x01)!=0){
//		HT_UART2->UARTSTA &= 0x06;
//		UART_SendState[2] = 0;
//	}
//}
//******************************************************************
//! \brief  	UART3_IRQHandler
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
//void UART3_IRQHandler(void)
//{
//	uint8_t ch= ch;

//	//receive interrupt 
//	if((HT_UART3->UARTSTA & 0x02)!=0){
//		ch = HT_UART3->SBUF;
//		
//		HT_UART3->UARTSTA &= 0x05;
//	}

//	//send interrupt
//	if((HT_UART3->UARTSTA & 0x01)!=0){
//		HT_UART3->UARTSTA &= 0x06;
//		UART_SendState[3] = 0;
//	}
//}
//******************************************************************
//! \brief  	UART4_IRQHandler
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
//void UART4_IRQHandler(void)
//{
//	uint8_t ch= ch;
//	
//	//receive interrupt 
//	if((HT_UART4->UARTSTA & 0x02)!=0){
//		ch = HT_UART4->SBUF;
//		
//		HT_UART4->UARTSTA &= 0x05;
//	}

//	//send interrupt
//	if((HT_UART4->UARTSTA & 0x01)!=0){
//		HT_UART4->UARTSTA &= 0x06;
//		UART_SendState[4] = 0;
//	}
//}
//******************************************************************
//! \brief  	UART5_IRQHandler
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
//void UART5_IRQHandler(void)
//{
//	uint8_t ch= ch;
//	
//	//receive interrupt 
//	if((HT_UART5->UARTSTA & 0x02)!=0){
//		ch = HT_UART5->SBUF;
//		
//		HT_UART5->UARTSTA &= 0x05;
//	}

//	//send interrupt
//	if((HT_UART5->UARTSTA & 0x01)!=0){
//		HT_UART5->UARTSTA &= 0x06;
//		UART_SendState[5] = 0;
//	}
//}
//******************************************************************
//! \brief  	UART6_IRQHandler
//! \param  	
//! \retval 	
//! \note   	
//******************************************************************
//void UART6_IRQHandler(void)
//{
//	uint8_t ch= ch;
//	
//	//receive interrupt 
//	if((HT_UART6->UARTSTA & 0x02)!=0){
//		ch = HT_UART6->SBUF;
//		
//		HT_UART6->UARTSTA &= 0x05;
//	}

//	//send interrupt
//	if((HT_UART6->UARTSTA & 0x01)!=0){
//		HT_UART6->UARTSTA &= 0x06;
//		UART_SendState[6] = 0;
//	}
//}

//******************************************************************
//! \brief  	UsartInit
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
void UsartInit(void)
{
	Init_Uart0();//2g通信
	Init_Uart1();//debug
	Init_Uart2();
	Init_Uart3();
	Init_Uart4();
	Init_Uart5();
	Init_Uart6();
	Init_Uart7();
	Init_Uart8();
	
	int i = 0;
	for(i=0;i<6;i++){
		UART_SendState[i] = 0;
	}
}





