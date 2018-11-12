/*cmu.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __BLUE_TOOTH_H__
#define __BLUE_TOOTH_H__

#include "includes.h"


#define BLUE_DEBUG                              1



#define BLUE_ENABLE()	  	                    GPIO_SetBits(HT_GPIOD,GPIO_Pin_9)
#define BLUE_DISENABLE()	  	                GPIO_ResetBits(HT_GPIOD,GPIO_Pin_9)

#define BLUE_CD_HIGHT()	  	                    GPIO_SetBits(HT_GPIOD,GPIO_Pin_11)
#define BLUE_CD_LOW()	  	                    GPIO_ResetBits(HT_GPIOD,GPIO_Pin_11)

#define GET_BLUE_IRQ()		       	            GPIO_ReadInputDataBit(HT_GPIOD, GPIO_Pin_10)

enum{
    NODE_BLUE = 0,
    NODE_24G
};

enum{
    USER_CMD_ADDR = 0,
	USER_CMD_GWID,
};

enum{
    BLUE_RESET = 0,
    BLUE_MODE_SET_ECHO,
    BLUE_MODE_TEST,
    BLUE_MODE_SET_DEVT,
    BLUE_MODE_GET_NAME, 
    BLUE_MODE_SET_NAME = 5,
    BLUE_MODE_SET_GWID_ADDR,
    BLUE_MODE_PAIR,
    BLUE_MODE_CONS,
    BLUE_MODE_VERS,
    BLUE_MODE_GET_GWID_ADDR = 10,
    BLUE_MODE_SET_X6_COUNT,
    BLUE_MODE_BAUD,
    BLUE_MODE_ECHO,
    BLUE_MODE_DEVT,
    BLUE_STATE_NUM
};

enum {
    BLUE_AB,
    BLUE_CD,
    BLUE_TARGET,
    BLUE_ADDR,
    BLUE_LEN,
    BLUE_RX_DATA,
    BLUE_CHK,
    BLUE_END,
};


#pragma pack(1)

typedef struct {
    uint8_t  ab;                                //0.
    uint8_t  cd;                                //1.
    uint8_t  target;                            //2.
    uint8_t  addr[6];                           //3.
    uint16_t len;                               //4.
}BLUE_HEAD_STR;

typedef struct {
    BLUE_HEAD_STR head;
    uint8_t  data[OUT_NET_PKT_LEN];             // x6报文头 + x6静荷
}BLUE_PKT_STR;

typedef struct {
    uint8_t  ab;                                //0.
    uint8_t  cd;                                //1.
    uint8_t  target;                            //2.
    uint16_t len;                               //4.
}BLUETOOTH_HEAD_RECEIVE_STR;

typedef struct {
    BLUETOOTH_HEAD_RECEIVE_STR head;
    uint8_t  data[OUT_NET_PKT_LEN];             // x6报文头 + x6静荷
}BLUETOOTH_PKT_RECEIVE_STR;

#pragma pack()


typedef struct{
    char *cmd;
    char *res;
    int wait;
    int nwait;
    int (*process)(char ok, uint8_t retry);
}BlUE_INITTAB;



int BuleCheckReset(char ok, uint8_t retry);
int BuleReconnect(void);
int BuleModeInfo(char ok, uint8_t retry);
int BlueSendCmd(char *cmd, char *ack, uint16_t waittime);
int BuleModePair(char ok, uint8_t retry);
int BuleModeCons(char ok, uint8_t retry);
int BuleModeName(char ok, uint8_t retry);
int BuleModeVers(char ok, uint8_t retry);
int BuleModeAddr(char ok, uint8_t retry);
int BuleModeBaud(char ok, uint8_t retry);
int BuleModeEcho(char ok, uint8_t retry);
int BuleModeDevt(char ok, uint8_t retry);
int BuleSetDevt(char ok, uint8_t retry);
int BuleModeTest(char ok, uint8_t retry);
int SendBluePkt(uint8_t nodeType, BLUE_PKT_STR *pFrame, uint16_t len);
int BuleSetName(char ok, uint8_t retry);
int BuleModeRstf(char ok, uint8_t retry);
//获取2.4G gwmac地址
extern int GetGateWay24GMacAddr(uint8_t cmd);
//设置2.4G gwmac地址
extern int SetGateWay24GMacAddr(char *mac, uint8_t cmd);
//获取蓝牙名称
extern int GetBlueName(void);
extern int isArraryEmpty(uint8_t *array,int len);
extern int BuleSetGWMac(char ok, uint8_t retry);
extern char *BCDToString(char *dest, unsigned char *BCD, int bytes);
extern int BuleSetX6Count(char ok, uint8_t retry);


extern uint8_t gBlueInitStep;
extern uint8_t const gZeroArray[8];


#endif


