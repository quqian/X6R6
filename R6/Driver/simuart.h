
#ifndef __SIMUART_H__
#define __SIMUART_H__

#include "includes.h"

#define SIM_UART_CNT                2

#define SIM_UART_EMU                0
#define SIM_UART_DEBUG              1


#define GET_USART_RX()		        GPIO_ReadInputDataBit(HT_GPIOH, GPIO_Pin_3)


enum{
	COM_START_BIT=0,
	COM_D0_BIT,
	COM_D1_BIT,
	COM_D2_BIT,
	COM_D3_BIT,
	COM_D4_BIT,
	COM_D5_BIT,
	COM_D6_BIT,
	COM_D7_BIT,
	COM_CHECKSUM_BIT,//9
	COM_STOP_BIT,
};


typedef struct {
    uint16_t recvIrqOk;
    uint16_t sendTimeOut;
    uint16_t byteCheckErr;
    uint16_t recvOk;
}SIM_USART_STATI_STR;


typedef struct {
    __IO uint8_t recvSend;                  //0�޲��� 1����  2����
    __IO uint8_t recvStat;
    __IO uint8_t recvData;
    __IO uint8_t recvCnt;
    __IO uint8_t gSendStat;
    __IO uint8_t gSendData;
    uint8_t  timerIndex;
    uint8_t  baudRate;                      //0:2400 1:4800 2:9600 3:19200 4:38400
    SIM_USART_STATI_STR statis;
}SIM_UART_STR;




void simutask(void);
void ShowSimUartErr(SIM_USART_STATI_STR *pStati);
int SimuUartSendData(UART_INFO_STR*pUart, const uint8_t *pData, uint16_t len);
int SimuartInit(void);

extern SIM_UART_STR gSimUartCtrl[SIM_UART_CNT];

#endif

