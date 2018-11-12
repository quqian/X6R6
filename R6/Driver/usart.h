
#ifndef __USART_H__
#define __USART_H__

#include "includes.h"

#define  PHY_UART                       1   //物理串口
#define  SIMU_UART                      2   //模拟串口

#define  SYS_UART_CNT                   9

#define  GPRS_UART_PORT                 3
#define  UART_485_PORT                  1
#define  DEBUG_UART                     2
#define  BLUE_TOOTH                     0

#define  UART0_RX_SIZE                  OUT_NET_PKT_LEN
#define  UART1_RX_SIZE                  OUT_NET_PKT_LEN
#define  UART2_RX_SIZE                  128
#define  UART3_RX_SIZE                  520
#define  UART4_RX_SIZE                  64
#define  UART5_RX_SIZE                  64
#define  UART6_RX_SIZE                  64
#define  UART7_RX_SIZE                  64
#define  UART8_RX_SIZE                  4

typedef struct{
    uint8_t  deviceIndex;               //对应模拟串口数组索引
    uint8_t  buffIndex;
    uint8_t  rsv;
    uint8_t  rsv1;
}SIM_UART_INFO_STR;

typedef struct{
    uint8_t  index;                     //串口系统索引 0~8
    uint8_t  type;                      //串口类型，1硬件串口  2模拟串口
    __IO uint16_t allCnt;
    __IO uint16_t sendCnt;
    __IO uint8_t  *pSendData;
    FIFO_S_t rxBuffCtrl;                //接收缓存控制信息
    uint16_t sendTimeOut;
    union {
        HT_UART_TypeDef * sysAddr;
        SIM_UART_INFO_STR simUart;
    }Data;
}UART_INFO_STR;


extern void UsartInit(void);
int UsartSend(int portIndex, const uint8_t *pData, uint16_t len);
int UsartGetOneData(int portIndex, uint8_t *pData);
int GetUsartRecvCnt(int portIndex);
int GetUsartRecvData(int portIndex, uint8_t *pBuff, uint16_t len);
void UsartFlush(int portIndex);
void Init_Uart1(UART_INFO_STR *pUartInfo);

extern UART_INFO_STR gUartPortAddr[SYS_UART_CNT];
extern __IO uint8_t UART0_RxBuff[UART0_RX_SIZE];
#endif //__USART_H__



