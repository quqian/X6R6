
#include "includes.h"
#include "simuart.h"
#include "usart.h"




SIM_UART_STR gSimUartCtrl[SIM_UART_CNT];




void ShowSimUartErr(SIM_USART_STATI_STR *pStati)
{
    CL_LOG("recvIrqOk     =%d.\n",pStati->recvIrqOk);
    CL_LOG("sendTimeOut   =%d.\n",pStati->sendTimeOut);
    CL_LOG("byteCheckErr  =%d.\n",pStati->byteCheckErr);
    CL_LOG("recvOk        =%d.\n",pStati->recvOk);
    printf("\n");
}


uint8_t check_sum(unsigned char revbyte)//返回偶校验值
{
    int count = 0;
    int temp = 0;

	for (int i=0; i<8; i++) {
		temp = (revbyte>>i)&(0x01);
		if (temp == 1)
		count++;
	}

	if ((count%2)==1)
		return 1;
	else
	    return 0;
}


int OpenTimer(uint8_t timer, uint16_t us)
{
    if (0 == timer) {
    	HT_TMR0->TMRCON = (0x3<<1) | (0<<0); //向上计数   //MODE周期计数
    	HT_TMR0->TMRDIV = 44 - 1;  //1us
    	HT_TMR0->TMRPRD = us;		//重装值  208us
    	HT_TMR0->TMRIE = 1;
	    return CL_OK;
    }else if (1 == timer) {
    	HT_TMR1->TMRCON = (0x3<<1) | (0<<0); //向上计数   //MODE周期计数
    	HT_TMR1->TMRDIV = 44 - 1;  //1us
    	HT_TMR1->TMRPRD = us;		//重装值52us
    	HT_TMR1->TMRIE = 1;
    	return CL_OK;
    }
    return CL_FAIL;
}


void TIM_Cmd(int timer, int en)
{
	if(timer == 0) {
		if(en)
			setbit(HT_TMR0->TMRCON,0);
		else
			clrbit(HT_TMR0->TMRCON,0);
	}else{
		if(en)
			setbit(HT_TMR1->TMRCON,0);
		else
			clrbit(HT_TMR1->TMRCON,0);
	}
}


void IO_USART_TX_PutVal(unsigned char val, uint8_t uartIndex)
{
    if (0 == uartIndex) {
    	if (val){
            GPIO_SetBits(HT_GPIOH, GPIO_Pin_4);
    	}else{
            GPIO_ResetBits(HT_GPIOH, GPIO_Pin_4);
    	}
    }else if (1 == uartIndex) {
        if (val){
            GPIO_SetBits(HT_GPIOA, GPIO_Pin_8);
    	}else{
            GPIO_ResetBits(HT_GPIOA, GPIO_Pin_8);
    	}
    }
}


void SimUartSendByte(uint8_t sendStat, uint8_t sendData, uint8_t uartIndex)
{
	if (sendStat == COM_START_BIT) {//发送起始位
		IO_USART_TX_PutVal(0, uartIndex);
	}else if(sendStat == COM_CHECKSUM_BIT) {
		IO_USART_TX_PutVal(check_sum(sendData), uartIndex);
	}else if(sendStat == COM_STOP_BIT) {//stop  high
		IO_USART_TX_PutVal(1, uartIndex);
	}else if(sendStat == 11) {//stop  delay
	}else{
		IO_USART_TX_PutVal(sendData & (1<<(sendStat-1)), uartIndex);
	}
}


void SimuUartRecvByte(uint8_t uartIndex, uint8_t buffIndex)
{
    SIM_UART_STR *pSimUart = &gSimUartCtrl[uartIndex];

    if (4 <= ++pSimUart->recvCnt) {
        pSimUart->recvCnt = 0;
        pSimUart->recvStat++;
    }

	if (pSimUart->recvStat == COM_STOP_BIT) {
		TIM_Cmd(pSimUart->timerIndex, DISABLE);
		FIFO_S_Put(&gUartPortAddr[buffIndex].rxBuffCtrl, pSimUart->recvData);
        pSimUart->statis.recvOk++;
	}else if (pSimUart->recvStat == COM_CHECKSUM_BIT) { //如果当前是校验位 则检查数据正确性
		if (GET_USART_RX() != check_sum(pSimUart->recvData)) {//校验位正确 进入下一步接收停止位
            pSimUart->statis.byteCheckErr++;
        }
	}else if (pSimUart->recvStat) {
        if (GET_USART_RX()) {
    		pSimUart->recvData |= (1 << (pSimUart->recvStat - 1));
    	}
    }
}

#if 1
void TIMER_0_IRQHandler(void)
{
    SIM_UART_STR *pSimUart = &gSimUartCtrl[0];
    UART_INFO_STR *pUart = &gUartPortAddr[7];

    if (2 == pSimUart->recvSend) {
    	if (HT_TMR0->TMRIF != 1) {
    		return;
    	}
        HT_TMR0->TMRIF = 0;

        SimUartSendByte(pSimUart->gSendStat, pSimUart->gSendData, 0);
        pSimUart->gSendStat++;
        if (12 == pSimUart->gSendStat) {
            if (++pUart->sendCnt < pUart->allCnt) {
                pSimUart->gSendData = pUart->pSendData[pUart->sendCnt];
                pSimUart->gSendStat = 0;
            }else{
                TIM_Cmd(0, DISABLE);
            }
        }
    }else if (1 == pSimUart->recvSend) {
        if (HT_TMR0->TMRIF == 1) {
            HT_TMR0->TMRIF = 0;
            SimuUartRecvByte(0, 7);
        }
    }
}
#endif

void TIMER_1_IRQHandler(void)
{
    SIM_UART_STR *pSimUart = &gSimUartCtrl[1];
    UART_INFO_STR *pUart = &gUartPortAddr[8];

    if (2 == pSimUart->recvSend) {
    	if (HT_TMR1->TMRIF != 1) {
    		return;
    	}
        HT_TMR1->TMRIF = 0;

        SimUartSendByte(pSimUart->gSendStat, pSimUart->gSendData, 1);
        pSimUart->gSendStat++;
        if (12 == pSimUart->gSendStat) {
            if (++pUart->sendCnt < pUart->allCnt) {
                pSimUart->gSendData = pUart->pSendData[pUart->sendCnt];
                pSimUart->gSendStat = 0;
            }else{
                TIM_Cmd(1, DISABLE);
            }
        }
    }else if (1 == pSimUart->recvSend) {
        if (HT_TMR1->TMRIF == 1) {
            HT_TMR1->TMRIF = 0;
            SimuUartRecvByte(1, 8);
        }
    }
}


int SimuUartSendData(UART_INFO_STR*pUart, const uint8_t *pData, uint16_t len)
{
    int waitCnt = 4*len;
    int cnt = 0;
    SIM_UART_STR *pSimUart = &gSimUartCtrl[pUart->Data.simUart.deviceIndex];

    pUart->allCnt = len;
    pUart->sendCnt = 0;
    pUart->pSendData = (void*)pData;
    pSimUart->gSendData = pData[0];
    pSimUart->gSendStat = 0;
    pSimUart->recvSend = 2;
    OpenTimer(pSimUart->timerIndex, pSimUart->baudRate);
	TIM_Cmd(pSimUart->timerIndex, ENABLE);
    while (pUart->sendCnt < pUart->allCnt) {
        OS_DELAY_MS(1);
        Feed_WDT();
        if (waitCnt < ++cnt) {
            pSimUart->statis.sendTimeOut++;
            TIM_Cmd(pSimUart->timerIndex, DISABLE);
            return CL_FAIL;
        }
    }
    return CL_OK;
}


void EXTI9_IRQHandler(void)
{
    SIM_UART_STR *pSimUart = &gSimUartCtrl[0];

	HT_INT->EXTIF2 = 0x0000;
	if (GET_USART_RX() == 0) {
		if (pSimUart->recvStat == COM_STOP_BIT) {
            pSimUart->statis.recvIrqOk++;
            pSimUart->recvCnt = 0;
			pSimUart->recvStat = COM_START_BIT;
            pSimUart->recvData = 0;
            pSimUart->recvSend = 1;
            OpenTimer(0, 52);
			TIM_Cmd(pSimUart->timerIndex, ENABLE);
		}
	}
}


void SimuartGpioInit(void)
{
	GPIO_Cfg(HT_GPIOH, GPIO_Pin_4, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);//pH4 tx
	GPIO_SetBits(HT_GPIOH, GPIO_Pin_4);
	GPIO_Cfg(HT_GPIOH, GPIO_Pin_3, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);//PH3 rx

	GPIO_Cfg(HT_GPIOA, GPIO_Pin_8, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
	GPIO_SetBits(HT_GPIOA, GPIO_Pin_8);
	GPIO_Cfg(HT_GPIOA, GPIO_Pin_7, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);

	EnWr_WPREG();
	HT_INT->EXTIF2 = 0x0000;
	NVIC_ClearPendingIRQ(EXTI9_IRQn);
	NVIC_SetPriority(EXTI9_IRQn, 2);
	HT_INT->EXTIE2 |= 0x004;		//INT6下降沿中断使能
	NVIC_EnableIRQ(EXTI9_IRQn);
    DisWr_WPREG();
}


int SimuartInit(void)
{
    int i;

    memset(gSimUartCtrl, 0, sizeof(gSimUartCtrl));
    for (i=0; i<SIM_UART_CNT; i++) {
        gSimUartCtrl[i].recvStat = COM_STOP_BIT;
    }
    gSimUartCtrl[0].timerIndex = 0;
    gSimUartCtrl[0].baudRate = 208; //波特率4800
    gSimUartCtrl[1].timerIndex = 1;
    gSimUartCtrl[1].baudRate = 52;  //波特率19200

    EnWr_WPREG();
    setbit(HT_CMU->CLKCTRL1 ,0);//打开定时器0
    setbit(HT_CMU->CLKCTRL1 ,1);//打开定时器1
    NVIC_EnableIRQ(TIMER_0_IRQn);
    NVIC_EnableIRQ(TIMER_1_IRQn);
    DisWr_WPREG();

	SimuartGpioInit();
	return CL_OK;
}


