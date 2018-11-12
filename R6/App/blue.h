
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
    BT_REQ_BREAK = 10,                          //����Ͽ�����
    BT_CMD_SET_CHARGER_SN = 0x0f,
    BT_CMD_SET_DEVICE_ID,
    BT_CMD_SET_GW_ADDR,                             //0x11
    BT_CMD_SET_SERVER_ADDR,                         //0x12
    BT_CMD_SET_SERVER_PORT,                         //0x13
    BT_CMD_SET_TERMINAL_INFO = 0x14,
    BT_CMD_REMO_CTRL,
};


/*
1��ϵͳ��������
2������ǹͷ
3���ر�ǹͷ
4������/�˳� ������״̬
5���趨����������
6���趨����������ֵ
7���趨����ʱ����ֵ
8���趨��ǹʱ����ֵ
9����ǹ�Ƿ�ֹͣ��綩��
10������ˢ���幤��ģʽ
11���������
12�����ô�ӡ����
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
    char     name[4];                               //0. �豸�ͺ�
    uint8_t  chargerSn[5];                          //1. ׮���
    uint8_t  fwVer;                                 //2. �̼��汾��
    uint8_t  portCnt;                               //3. ������Ŀ
    uint8_t  startNo;                               //4. ������ʼ���
}BLUE_SHAKE_ACK_STR;

typedef struct {
    uint8_t GateWay24gAddr[5];					    //����2.4mac��ַ
}BLUE_2_4G_ADDR_STR;

typedef struct {
    uint8_t Result;
}BLUE_2_4G_ADDR_ACK_STR;

#pragma pack()


typedef struct {
    uint8_t  sendMux;
    uint8_t  resetMode;                             //0:������ģ�飬1:��������ģ��  2:�������
    uint8_t  destIndex;                             //2.4G���͵�Ŀ������
    uint32_t lastRecv24gTime;                       //���һ�α��Ľ���״̬
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
//����2.4Gģ��X6����
extern int SetX6Count(uint8_t Count);

#endif


