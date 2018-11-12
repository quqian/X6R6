
#ifndef __BLUE_H__
#define __BLUE_H__

#include "includes.h"


#define STR_MAC_LENTH                               11

#define RECEIVE_MAX_PKT_24G                 	1


#define DETECT_SEND_PKT_CNT                         200

#define RETRY_24G_MAX_CNT                           3
#define RESENT_24G_MSG_CNT                          50//20
#define RETRY_24G_DELAY_TIME                        40//150


enum {
    BT_FIND_EE,
    BT_FIND_CMD,
    BT_FIND_LEN,
    BT_RX_DATA,
    BT_FIND_CHK,
    BT_FIND_END,
};


enum {
    FIND_24G_EE,
    FIND_24G_LEN,
    FIND_24G_VER,
    FIND_24G_CMD,
    FIND_24G_RX_DATA,
    FIND_24G_CHK,
};


enum {
    BT_SHAKE_REQ = 1,
    BT_OPPO_SITE_AUTH,
    BT_START_CHARGING,
    BT_STOP_CHARGING,
    BT_HEART_BEAT = 5,
    BT_COST_TEMPLATE_UPLOAD,
    BT_REQ_COST_TEMPLATE,
    BT_FW_UPGRADE_NOTICE,
    BT_FW_DOWN_LOAD,
    BT_REQ_BREAK = 10,                          //请求断开连接
    BT_CMD_SET_CHARGER_SN = 0x0f,
    BT_CMD_SET_DEVICE_ID,
    BT_CMD_SET_GW_ADDR,                             //0x11
    BT_CMD_SET_SERVER_ADDR,                         //0x12
    BT_CMD_SET_SERVER_PORT,                         //0x13
    BT_CMD_SET_TERMINAL_INFO = 0x14,
    BT_CMD_REMO_CTRL,
};


/*
1：系统立即重启
2：开启枪头
3：关闭枪头
4：进入/退出 充电服务状态
5：设定最大输出功率
6：设定充满功率阈值
7：设定充满时间阈值
8：设定拔枪时间阈值
9：拔枪是否停止充电订单
10：设置刷卡板工作模式
11：清除数据
12：设置打印开关
*/
//enum {
////    CTRL_SYS_REBOOT = 1,
////    CTRL_OPEN_GUN,                                  //0x02
////    CTRL_CLOSE_GUN,                                 //0x03
////    CTRL_SET_SERVER_STATUS,                         //0x04
////    CTRL_SET_OUT_POWER,                             //0x05
////    CTRL_SET_FULL_POWER,                            //0x06
////    CTRL_SET_FULL_TIME,                             //0x07
////    CTRL_SET_PULL_GUN_TIME,                         //0x08
////    CTRL_SET_PULL_GUN_STOP,                         //0x09
////    CTRL_SET_CARD_WORK,                             //0x0a
////    CTRL_CLEAR_DATA,                                //0x0b
////    CTRL_SET_PRINT_SWITCH,                          //0x0c
//};

#pragma pack(1)

typedef struct {
    uint8_t  start;
    uint8_t  cmd;
    uint8_t  len;
}BLUE_PROTO_HEAD_STR;

typedef struct {
    BLUE_PROTO_HEAD_STR head;
    uint8_t  data[64];
    uint8_t  checkSum;
}BLUE_PROTO_STR;


typedef struct {
    uint8_t  num;
    uint8_t  chargerSn[5];
}BLUE_TERMINAL_STR;


typedef struct {
    char     name[4];                               //0. 设备型号
    uint8_t  chargerSn[5];                          //1. 桩编号
    uint8_t  fwVer;                                 //2. 固件版本号
    uint8_t  portCnt;                               //3. 插座数目
    uint8_t  startNo;                               //4. 插座起始编号
}BLUE_SHAKE_ACK_STR;

typedef struct {
    uint8_t GateWay24gAddr[5];					    //本地2.4mac地址
}BLUE_2_4G_ADDR_STR;

typedef struct {
    uint8_t Result;
}BLUE_2_4G_ADDR_ACK_STR;

#pragma pack()


typedef struct {
    uint8_t  sendMux;
    uint8_t  resetMode;                             //0:不重启模块，1:正在重启模块  2:重启完成
    uint8_t  destIndex;                             //2.4G发送的目的索引
    uint32_t lastRecv24gTime;                       //最后一次报文接收状态
    uint32_t lastSendTIme;
    FIFO_S_t rxBtBuff;
    FIFO_S_t rx24GBuff;
    uint8_t  blueName[12];
}BLUE_CTRL_STR;


int BlueInit(void);
void BlueNetProc(void);
void ProcNodeStatus(void);
void PktRecvTask(void);
void BlueTask(void);


extern BLUE_CTRL_STR gBlueCtrl;

extern int SendX6ReqPkt(SYS_PKT_STR *pBuff, uint8_t SendFlag);
//告诉2.4G模块X6个数
extern int SetX6Count(uint8_t Count);

#endif


