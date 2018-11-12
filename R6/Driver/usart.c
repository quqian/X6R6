/*usart.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "includes.h"
#include "usart.h"
#include "FIFO.h"
#include "simuart.h"


__IO uint8_t UART0_RxBuff[UART0_RX_SIZE] = {0};
__IO uint8_t UART1_RxBuff[UART1_RX_SIZE] = {0};
__IO uint8_t UART2_RxBuff[UART2_RX_SIZE] = {0};
__IO uint8_t UART3_RxBuff[UART3_RX_SIZE] = {0};
__IO uint8_t UART4_RxBuff[UART4_RX_SIZE] = {0};
__IO uint8_t UART5_RxBuff[UART5_RX_SIZE] = {0};
__IO uint8_t UART6_RxBuff[UART6_RX_SIZE] = {0};
__IO uint8_t UART7_RxBuff[UART7_RX_SIZE] = {0};
__IO uint8_t UART8_RxBuff[UART8_RX_SIZE] = {0};

UART_INFO_STR gUartPortAddr[SYS_UART_CNT] = {0};




//******************************************************************
//! \brief  	Set_Uart_Bandrate
//! \param  	none
//! \retval
//! \note   	设置串口波特率
//! \note
//******************************************************************
void Set_Uart_Bandrate(HT_UART_TypeDef* usart, uint32_t bps)
{
	//bps = Fsys/2*(SREL+1)
	//初值SREL=Fsys/2*bps-1, Fsys=44MHz
	usart->SREL=44040192/2/(2*bps)-1;
}


//******************************************************************
//! \brief  	USART_SendData
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
int UartSendData(UART_INFO_STR*pUart, const uint8_t *pData, uint16_t len)
{
    int cnt = 0;
    int waitCnt = 3*len;

    pUart->allCnt = len;
    pUart->sendCnt = 0;
    pUart->pSendData = (void*)pData;
    pUart->Data.sysAddr->SBUF = pData[0];
    while (pUart->sendCnt < pUart->allCnt) 
	{
        OS_DELAY_MS(1);
        if (waitCnt < ++cnt) 
		{
            pUart->sendTimeOut++;
            break;
        }
    }
    return CL_OK;
}


//******************************************************************
//! \brief  	UartSend
//! \param  	none
//! \retval
//! \note
//******************************************************************
int UsartSend(int portIndex, const uint8_t *pData, uint16_t len)
{
    int ret = CL_FAIL;
    UART_INFO_STR *pUart = &gUartPortAddr[portIndex];

    if (PHY_UART == pUart->type) {
        ret = UartSendData(pUart, pData, len);
    }else if (SIMU_UART == pUart->type) {
        //ret = SimuUartSendData(pUart, pData, len);
    }
    return ret;
}


//返回值为0，读取成功，其他:读取失败
//读取一个字符后，该字符从缓存区清空
int UsartGetOneData(int portIndex, uint8_t *pData)
{
    UART_INFO_STR *pUart = &gUartPortAddr[portIndex];

    return FIFO_S_Get(&pUart->rxBuffCtrl, pData);
}


int GetUsartRecvCnt(int portIndex)
{
    return FIFO_S_CountUsed(&gUartPortAddr[portIndex].rxBuffCtrl);
}


void UsartFlush(int portIndex)
{
    FIFO_S_Flush(&gUartPortAddr[portIndex].rxBuffCtrl);
}


int GetUsartRecvData(int portIndex, uint8_t *pBuff, uint16_t len)
{
    uint8_t  data;
    int i;
    UART_INFO_STR *pUart = &gUartPortAddr[portIndex];

    for (i=0; i<len; i++) {
        if (CL_OK == FIFO_S_Get(&pUart->rxBuffCtrl, &data)) {
            pBuff[i] = data;
        }else{
            return CL_FAIL;
        }
    }
    return CL_OK;
}


//******************************************************************
//! \brief  	Init_Uart0
//! \param  	none
//! \retval
//! \note   	串口0初始化
//! \note
//******************************************************************
void Init_Uart0(UART_INFO_STR *pUartInfo)
{
	//PC3 PC2 as TX0 RX0
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);
	NVIC_DisableIRQ(UART0_IRQn);				//禁止串口中断
	HT_UART0->MODESEL=0;						//uart function
	HT_UART0->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART0->UARTCON = 0x000f;					//正逻辑,8位数据，无校验，2停止位,接收发送使能
	HT_UART0->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART0, 115200);
	NVIC_ClearPendingIRQ(UART0_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART0_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART0_IRQn);					//使能串口中断
	pUartInfo->index = 0;
    pUartInfo->type = PHY_UART;
	FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART0_RxBuff, sizeof(UART0_RxBuff));
    pUartInfo->Data.sysAddr = HT_UART0;
}


//******************************************************************
//! \brief  	Init_Uart1
//! \param  	none
//! \retval
//! \note   	串口1初始化
//! \note
//******************************************************************
void Init_Uart1(UART_INFO_STR *pUartInfo)
{
	//PC0 PC1 as TX1 RX1
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_0|GPIO_Pin_1, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART1EN, ENABLE);
	NVIC_DisableIRQ(UART1_IRQn);				//禁止串口中断
	HT_UART1->MODESEL=0;						//uart function
	HT_UART1->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART1->UARTCON = 0x000f;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	Set_Uart_Bandrate(HT_UART1, 19200);
	HT_UART1->IRCON   = 0x0000;					//非调制
	NVIC_ClearPendingIRQ(UART1_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART1_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART1_IRQn);					//使能串口中断
	pUartInfo->index = 1;
    pUartInfo->type = PHY_UART;
	FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART1_RxBuff, sizeof(UART1_RxBuff));
    pUartInfo->Data.sysAddr = HT_UART1;
}


//******************************************************************
//! \brief  	Init_Uart2
//! \param  	none
//! \retval
//! \note   	串口2初始化
//! \note
//******************************************************************
void Init_Uart2(UART_INFO_STR *pUartInfo)
{
	//PC11 PC12 as TX2 RX2
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_11|GPIO_Pin_12, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART2EN, ENABLE);
	NVIC_DisableIRQ(UART2_IRQn);				//禁止串口中断
	HT_UART2->MODESEL=0;						//uart function
	HT_UART2->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART2->UARTCON = 0x000F;					//正逻辑,8位数据，无校验，2停止位,接收发送使能
	HT_UART2->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART2, 19200);
	NVIC_ClearPendingIRQ(UART2_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART2_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART2_IRQn);					//使能串口中断
	pUartInfo->index = 2;
    pUartInfo->type = PHY_UART;
	FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART2_RxBuff, sizeof(UART2_RxBuff));
    pUartInfo->Data.sysAddr = HT_UART2;
}


//******************************************************************
//! \brief  	Init_Uart3
//! \param  	none
//! \retval
//! \note   	串口3初始化
//! \note
//******************************************************************
void Init_Uart3(UART_INFO_STR *pUartInfo)
{
	//PE5 PE4 as TX3 RX3
	GPIO_Cfg(HT_GPIOE, GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);
	NVIC_DisableIRQ(UART3_IRQn);				//禁止串口中断
	HT_UART3->MODESEL=0;						//uart function
	HT_UART3->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART3->UARTCON = 0x000F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART3->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART3, 115200);
	NVIC_ClearPendingIRQ(UART3_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART3_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART3_IRQn);					//使能串口中断
	pUartInfo->index = 3;
    pUartInfo->type = PHY_UART;
	FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART3_RxBuff, sizeof(UART3_RxBuff));
    pUartInfo->Data.sysAddr = HT_UART3;
}


//******************************************************************
//! \brief  	Init_Uart4
//! \param  	none
//! \retval
//! \note   	串口4初始化
//! \note
//******************************************************************
void Init_Uart4(UART_INFO_STR *pUartInfo)
{
	//PE1 PE2 as TX4 RX4
	GPIO_Cfg(HT_GPIOE, GPIO_Pin_1|GPIO_Pin_2, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART4_7816_0EN, ENABLE);
	NVIC_DisableIRQ(UART4_IRQn);				//禁止串口中断
	HT_UART4->MODESEL=0;						//uart function
	HT_UART4->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART4->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART4->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART4, 4800);
	NVIC_ClearPendingIRQ(UART4_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART4_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART4_IRQn);					//使能串口中断
	pUartInfo->index = 4;
    pUartInfo->type = PHY_UART;
	FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART4_RxBuff, sizeof(UART4_RxBuff));
    pUartInfo->Data.sysAddr = HT_UART4;
}


//******************************************************************
//! \brief  	Init_Uart5
//! \param  	none
//! \retval
//! \note   	串口5初始化
//! \note
//******************************************************************
void Init_Uart5(UART_INFO_STR *pUartInfo)
{
	//PC5 PC4 as TX5 RX5
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_AF2, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART5EN, ENABLE);
	NVIC_DisableIRQ(UART5_IRQn);				//禁止串口中断
	HT_UART5->MODESEL=0;						//uart function
	HT_UART5->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART5->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART5->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART5, 4800);
	NVIC_ClearPendingIRQ(UART5_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART5_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART5_IRQn);					//使能串口中断
	pUartInfo->index = 5;
    pUartInfo->type = PHY_UART;
	FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART5_RxBuff, sizeof(UART5_RxBuff));
    pUartInfo->Data.sysAddr = HT_UART5;
}


//******************************************************************
//! \brief  	Init_Uart6
//! \param  	none
//! \retval
//! \note   	串口6初始化
//! \note
//******************************************************************
void Init_Uart6(UART_INFO_STR *pUartInfo)
{
	//PG12 PG11 as TX6 RX6
	GPIO_Cfg(HT_GPIOG, GPIO_Pin_11|GPIO_Pin_12, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART6EN, ENABLE);
	NVIC_DisableIRQ(UART6_IRQn);				//禁止串口中断
	HT_UART6->MODESEL=0;						//uart function
	HT_UART6->UARTSTA = 0x0000;					//清除中断标志位
	HT_UART6->UARTCON = 0x005F;					//正逻辑,8位数据，偶校验，2停止位,接收发送使能
	HT_UART6->IRCON   = 0x0000;					//非调制
	Set_Uart_Bandrate(HT_UART6, 4800);
	NVIC_ClearPendingIRQ(UART6_IRQn);			//清除挂起状态
	NVIC_SetPriority(UART6_IRQn, 3);			//设置优先级
	NVIC_EnableIRQ(UART6_IRQn);					//使能串口中断
	pUartInfo->index = 6;
    pUartInfo->type = PHY_UART;
	FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART6_RxBuff, sizeof(UART6_RxBuff));
    pUartInfo->Data.sysAddr = HT_UART6;
}


//******************************************************************
//! \brief  	Init_Uart7
//! \param  	none
//! \retval
//! \note   	串口7初始化
//! \note
//******************************************************************
void Init_Uart7(UART_INFO_STR *pUartInfo)
{
    pUartInfo->index = 7;
    pUartInfo->type = SIMU_UART;
    FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART7_RxBuff, sizeof(UART7_RxBuff));
    pUartInfo->Data.simUart.deviceIndex = 0;
}


//******************************************************************
//! \brief  	Init_Uart8
//! \param  	none
//! \retval
//! \note   	串口7初始化
//! \note
//******************************************************************
void Init_Uart8(UART_INFO_STR *pUartInfo)
{
    pUartInfo->index = 8;
    pUartInfo->type = SIMU_UART;
    FIFO_S_Init(&pUartInfo->rxBuffCtrl, (void*)UART8_RxBuff, sizeof(UART8_RxBuff));
    pUartInfo->Data.simUart.deviceIndex = 1;
}


//******************************************************************
//! \brief  	UART0_IRQHandler
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void UART0_IRQHandler(void)
{
    UART_INFO_STR *pUart = &gUartPortAddr[0];

	//receive interrupt
	if ((pUart->Data.sysAddr->UARTSTA & 0x02)!=0) {
		FIFO_S_Put(&pUart->rxBuffCtrl, pUart->Data.sysAddr->SBUF);
		pUart->Data.sysAddr->UARTSTA &= 0x05;
	}

    //send interrupt
	if ((pUart->Data.sysAddr->UARTSTA & 0x01) != 0) {
		pUart->Data.sysAddr->UARTSTA &= 0x06;
        if (++pUart->sendCnt < pUart->allCnt) {
            pUart->Data.sysAddr->SBUF = pUart->pSendData[pUart->sendCnt];
        }
	}
}

//******************************************************************
//! \brief  	UART1_IRQHandler
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void UART1_IRQHandler(void)
{
    UART_INFO_STR *pUart = &gUartPortAddr[1];
	//receive interrupt
	if ((pUart->Data.sysAddr->UARTSTA & 0x02)!=0) {
        FIFO_S_Put(&pUart->rxBuffCtrl, pUart->Data.sysAddr->SBUF);
		pUart->Data.sysAddr->UARTSTA &= 0x05;
	}

    //send interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x01) != 0) {
		pUart->Data.sysAddr->UARTSTA &= 0x06;
        if (++pUart->sendCnt < pUart->allCnt) {
            pUart->Data.sysAddr->SBUF = pUart->pSendData[pUart->sendCnt];
        }
	}
}

//******************************************************************
//! \brief  	UART2_IRQHandler
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void UART2_IRQHandler(void)
{
    UART_INFO_STR *pUart = &gUartPortAddr[2];

	//receive interrupt
	if ((pUart->Data.sysAddr->UARTSTA & 0x02)!=0) {
        FIFO_S_Put(&pUart->rxBuffCtrl, pUart->Data.sysAddr->SBUF);
		pUart->Data.sysAddr->UARTSTA &= 0x05;
	}
    //send interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x01) != 0) {
		pUart->Data.sysAddr->UARTSTA &= 0x06;
        if (++pUart->sendCnt < pUart->allCnt) {
            pUart->Data.sysAddr->SBUF = pUart->pSendData[pUart->sendCnt];
        }
	}
}

//******************************************************************
//! \brief  	UART3_IRQHandler
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void UART3_IRQHandler(void)
{
    UART_INFO_STR *pUart = &gUartPortAddr[3];
    uint8_t  data;

	//receive interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x02)!=0){
        data = pUart->Data.sysAddr->SBUF;
        FIFO_S_Put(&pUart->rxBuffCtrl, data);
        if (system_info.is_socket_0_ok) {
            FIFO_S_Put(&gSocketPktRxCtrl, data);
        }
		pUart->Data.sysAddr->UARTSTA &= 0x05;
	}
    //send interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x01) != 0) {
		pUart->Data.sysAddr->UARTSTA &= 0x06;
        if (++pUart->sendCnt < pUart->allCnt) {
            pUart->Data.sysAddr->SBUF = pUart->pSendData[pUart->sendCnt];
        }
	}
}

//******************************************************************
//! \brief  	UART4_IRQHandler
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void UART4_IRQHandler(void)
{
    UART_INFO_STR *pUart = &gUartPortAddr[4];

	//receive interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x02)!=0) {
        FIFO_S_Put(&pUart->rxBuffCtrl, pUart->Data.sysAddr->SBUF);
		pUart->Data.sysAddr->UARTSTA &= 0x05;
	}
    //send interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x01) != 0) {
		pUart->Data.sysAddr->UARTSTA &= 0x06;
        if (++pUart->sendCnt < pUart->allCnt) {
            pUart->Data.sysAddr->SBUF = pUart->pSendData[pUart->sendCnt];
        }
	}
}

//******************************************************************
//! \brief  	UART5_IRQHandler
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void UART5_IRQHandler(void)
{
    UART_INFO_STR *pUart = &gUartPortAddr[5];

	//receive interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x02)!=0) {
        FIFO_S_Put(&pUart->rxBuffCtrl, pUart->Data.sysAddr->SBUF);
		pUart->Data.sysAddr->UARTSTA &= 0x05;
	}
    //send interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x01) != 0) {
		pUart->Data.sysAddr->UARTSTA &= 0x06;
        if (++pUart->sendCnt < pUart->allCnt) {
            pUart->Data.sysAddr->SBUF = pUart->pSendData[pUart->sendCnt];
        }
	}
}

//******************************************************************
//! \brief  	UART6_IRQHandler
//! \param
//! \retval
//! \note
//******************************************************************
void UART6_IRQHandler(void)
{
    UART_INFO_STR *pUart = &gUartPortAddr[6];

	//receive interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x02)!=0) {
        FIFO_S_Put(&pUart->rxBuffCtrl, pUart->Data.sysAddr->SBUF);
		pUart->Data.sysAddr->UARTSTA &= 0x05;
	}
    //send interrupt
	if((pUart->Data.sysAddr->UARTSTA & 0x01) != 0) {
		pUart->Data.sysAddr->UARTSTA &= 0x06;
        if (++pUart->sendCnt < pUart->allCnt) {
            pUart->Data.sysAddr->SBUF = pUart->pSendData[pUart->sendCnt];
        }
	}
}

//******************************************************************
//! \brief  	usart_init
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void UsartInit(void)
{
    memset(gUartPortAddr, 0, sizeof(gUartPortAddr));
	Init_Uart0(&gUartPortAddr[0]);//2
	Init_Uart1(&gUartPortAddr[1]);
	Init_Uart2(&gUartPortAddr[2]);
	Init_Uart3(&gUartPortAddr[3]);
	//Init_Uart4(&gUartPortAddr[4]);
	//Init_Uart5(&gUartPortAddr[5]);
	//Init_Uart6(&gUartPortAddr[6]);
	//Init_Uart7(&gUartPortAddr[7]);
	//Init_Uart8(&gUartPortAddr[8]);
}


