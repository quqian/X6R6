/*server.h
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __SERVER_H__
#define __SERVER_H__

#include "includes.h"
#include "proto.h"
#include "x6proc.h"
#include "bluetooth.h"


#define QUEUE_BUFF_CNT                              4

#define PLUG_IDLE                                   0
#define PLUG_USED_NO_OPEN                           1
#define PLUG_USED_OPEN                              2
#define PLUG_FAULT                                  3

#define RECORD_MAX_BLOCK_NUM                        512     //����¼��������
#define SPI_FLASH_PAGE_SIZE					        4096    //flashҳ��С
#define	RECORD_BLOCK_SIZE_PER_PAGE		            256     //һ��������󻺴�
#define	RECORD_BLOCK_NUM_PER_PAGE		            16//(SPI_FLASH_PAGE_SIZE/RECORD_BLOCK_SIZE_PER_PAGE)    //flashÿҳ�ɼ�¼�Ķ�������
//#define	RECORD_FLASH_SIZE				            (RECORD_MAX_BLOCK_NUM*RECORD_BLOCK_SIZE_PER_PAGE)	//��¼������flash�ռ�


typedef enum{
	OFFLINE = 0,
	ONLINE,
}IS_ONLINE;


enum{
    NET_MODE_2G = 1,
    NET_MODE_4G_ME3630,
    NET_MODE_NBIOT_3612,
    NET_MODE_4G_ME3610,
    NET_MODE_ALL
};

typedef enum {
    FIND_AA,
    FIND_55,
    FIND_CHARGER_TYPE,
    FIND_CHAGER_SN,
    FIND_SRC,
    FIND_DEST,
    FIND_LEN,
    FIND_VER,
    FIND_SERNUM,
    FIND_CMD,
    RX_DATA,
    FIND_CHK,
    FIND_END,
} PROTO_MSG_STR;


enum{
    GUN_STATE_IDLE = 0,
    GUN_STATE_WAIT_PLUG_IN,
    GUN_STATE_ON,
    GUN_STATE_OFF,
    GUN_STATE_OFF_WAIT_ACK,
    GUN_STATE_OFF_NO_NET_WAIT_ACK,
    GUN_SATAE_INIT,
};

enum{
    TEST_CMD_KEY = 1,			//��������
	TEST_CMD_CARD = 2,			//ˢ������
	TEST_CMD_LCD = 3,			//LCD����
	TEST_CMD_2G = 4,			//2Gģ�����
	TEST_CMD_VOICE = 5,			//���Ȳ���
	TEST_CMD_RELAY = 6,			//�̵���
	TEST_CMD_PCB_CODE = 7,			//PCB����
    DEBUG_CMD_SN = 8,
    TEST_CMD_BLE = 0x09, //BLE����
    TEST_CMD_FW_VERSION = 0x0a,
    TEST_CMD_READ_PCB = 0x0b,
    DEBUG_CMD_DEV_TYPE  = 0x0d,                     //0x0d
    DEBUG_CMD_GW_ADDR = 0x0e,                              //0x0e
    DEBUG_CMD_TER_MINAL = 0x0f,                            //0x0f
    DEBUG_CMD_SERVER_ADDR = 0x10,                          //0x10
    DEBUG_CMD_SERVER_PORT = 0x11,                          //0x11
    DEBUG_CMD_PRINTF,                               //0x12
    DEBUG_CMD_REBOOT,                               //0x13
    DEBUG_CMD_DEVICE_ID,                            //0x14
    DEBUG_CMD_SET_RTC,                          //0x15 ����RTCʱ��
    DEBUG_CMD_RED_RTC,                          //0x16 ��ȡRTCʱ��
    DEBUG_CMD_FLASH,                            //0x17 Flash��д����
	DEBUG_CMD_SET_LISTEN_MODE,                  	//0x18
    DEBUG_CMD_QUE_LISTEN_MODE,                  	//0x19
    DEBUG_CMD_LED,                              //0x1a Ledָʾ�Ʋ���
    DEBUG_CMD_485,                              //0x1b 485�ӿڲ���
    DEBUG_CMD_NET_CONNECT,                      //0x1c ��̨�ԽӲ���
    DEBUG_CMD_4G_TO_2G,                         //0x1d ����4G����2G
    DEBUG_CMD_GET_CHARGER_SN,                   //0x1e ��ȡ׮���
    DEBUG_CMD_GET_DEVICE_ID,                    //0x1f ��ȡʶ����
    START_AGE_TEST,                             //0x20 �����ϻ�����
    GET_AGE_TEST_RESULT,                        //0x21 ��ȡ�ϻ����Խ��
    DEBUG_CMD_24G_COMMUNICATION = 0xF3,                    //2.4Gͨ�Ų��� 
    TEST_CMD_STATUS = 0x99,
};

typedef struct {
    char *cmd;
    char *res;
    int wait;
    int nwait;
    int (*process)(char ok, uint8_t retry);
}GPRS_INIT_TAB_STR;


typedef struct {
    uint8_t  heartBeatStatus;                       //����״̬  0:δ֪��1:������������2:��������
    uint16_t sendCnt;
    uint16_t recvCnt;
    uint16_t reqStartChargingSn;
    uint16_t reqStopChargingSn;
}TERMINAL_STATUS_STR;


typedef struct {
    uint8_t  nodeIndex;     //�ӽڵ�0��ʼ��һ��X6����һ���ڵ�
    uint8_t  sendTimes;
}LOOP_SEND_STR;

typedef struct {
    uint32_t  Upgrade2_4Gflag:2;         //2.4G������־ 1:�������� 2���������� 
    uint32_t  isSendFwUpgradeOk:2;       //���͹̼�����0x0A�Ƿ�ɹ�
    uint32_t  X6R6BindRelation:2;       //
    uint32_t  ReceiveX6BindMsg:2;       //�յ�X6���͹����İ���Ϣ
    //uint32_t  FwUpgradeFlag:2;			//���Ϊ 1 ��ʾ����FTPGET X6������
}BIT_FLAG_STR;

typedef struct {
    uint8_t  netStatus;                             //�������״̬: b0:�Ƿ�������������ʧ�� b1:�Ƿ��͹����ͱ���ʧ�� b2:�Ƿ�������� b3:�Ƿ����2.4G
    uint8_t  netLedStatus;                          //�յ�������1�����ƺ�����
    uint8_t  blueLedStatus;                         //�յ�������1�����ƺ�����
    uint8_t  sendHbStartGunIndex;                   //���͵���ʼǹͷid��־

    uint8_t  authAckRecvFlag;                       //ˢ����ȨӦ���͵�ȷ�ϱ�־
    uint8_t  reqX6StartCharging;                    //����x6��������ʶ
    uint8_t  reqX6StopCharging;                     //����x6ֹͣ����ʶ
    uint8_t  authReqSn;                             //���ڽ��м�Ȩ��sn��

    uint8_t  startChargingNoticeSn;                 //�������֪ͨ���
    uint8_t  stopChargingNoticeSn;                  //x6 ֹͣ���֪ͨ���
    uint8_t  sendPKtFlag;                           //0:���Է���1:��������
    LOOP_SEND_STR loopSend;
    TERMINAL_STATUS_STR terminalStatus[MAX_TERMINAL_NUM];
    BIT_FLAG_STR flagBit;
    uint16_t current_package[MAX_TERMINAL_NUM];       //�̼���Ƭ��� X6���͸�R6˵��Ҫ������һ����Ƭ��
    uint8_t  gun_id;                //2.4G������ǹͷ��
    uint32_t gUpgradeWaitTimerTick;
    uint8_t reserve;
	uint8_t isBlueToothInit;
	uint8_t X6R6BindSn;
	uint8_t UpgradeFlag;	//0�������� , 1��������
	uint8_t ReceiveR624gDataFlag;
	uint8_t ProductToolTestFlag;
	uint32_t gCardAuthNoticeTick[MAX_TERMINAL_NUM];
	uint32_t gPowerOffNoticeTick[MAX_TERMINAL_NUM];
}CHG_INFO_STR;


#pragma pack(1)

typedef struct {
    uint8_t  aa;                                    //0.
    uint8_t  five;                                  //1.
    uint8_t  src[5];                                //2. ׮��   Bcd
    uint8_t  dest[5];                               //3.
    uint16_t len;                                   //4.
    uint8_t  ver;                                   //5.
    uint8_t  sn;                                    //6.
    uint8_t  cmd;                                   //7.
}OUT_NET_HEAD_STR;

typedef struct {
    OUT_NET_HEAD_STR head;
    uint8_t  data[OUT_NET_PKT_LEN];
}OUT_PKT_STR;


typedef struct{
    uint32_t size;
    uint16_t checkSum;
    uint8_t  ver;
    uint32_t  sum;                                   //0:�̼���Ч 1:�̼���Ч
}X6_FW_STR;


typedef struct{
    X6_GUN_STAUS_STR  gunStatus;
	uint8_t  order[ORDER_SECTION_LEN];
}gun_info_t;


typedef struct {
    uint16_t wIndex;
	uint16_t rIndex;
    uint16_t orderCnt;
    uint8_t  sendCnt;                               //��Ϊ0���������ڷ��͵Ķ���
    uint8_t  gunId;                                 //��Ϊ0���������ڷ��͵Ķ���
}ORDER_STR;

typedef struct {
    uint8_t UpgradeFinish[MAX_TERMINAL_NUM];	//Ϊ1��ʾ�������
	uint16_t x6package_num[MAX_TERMINAL_NUM];
}IS_UPGRADE_OK_STR;

typedef struct {
    uint8_t  station_id[8];                         // bcd, SN=9, 10 byte  0000123456789
    uint8_t  gwAddr[MAC_LEN];                       //���ص�ַ
}system_info_str1;

typedef struct {
    uint32_t magic_number;
	char     idCode[8];                             // bcd ʶ����		modified by liutao, 2017/11/27
	uint8_t     preserve[32];								//����
	COST_TEMPLATE_REQ_STR cost_template;
    uint32_t rsv[8];                               	//����
    uint8_t  iccid[22];
    char     server_ip[32];                         // ASCII "202.104.136.212"
    uint16_t server_port;
    uint8_t  rsv1[8];                         		// ����
    uint32_t mqtt_sn;
    volatile uint8_t    is_socket_0_ok;
    uint8_t  tcp_tx_error_times;
    uint8_t  isRecvStartUpAck;
    uint8_t  netType;                               //0:δ���  2:485  3:����2.4G
    uint8_t  localNetType;                          //1:2g�� 2:4g�� 3:nbiot-me3612
    volatile uint8_t  printfSwitch;                          //��ӡ����
    uint8_t  rsv2[5];                       			//����
    X6_FW_STR  x6FwInfo;
	uint16_t x6PktSn;
    uint16_t x6AuthResultPktSn;
    uint8_t x6SetPowerOnPktSn;
    uint8_t x6SetPowerOffPktSn;
    uint8_t x6UpgradePktSn;
    uint8_t  chargerCnt;                            //x6��Ŀ
    CHARGER_STR chargerInfo[MAX_TERMINAL_NUM];
    ORDER_STR orderInfo;                            //���涩����Ϣ
    uint16_t x6_package_num;               //�̼���Ƭ���� 
    uint8_t  pcbCode[8];                        //pcb����
    IS_UPGRADE_OK_STR isUpgradeOk;
	uint8_t  disturbingStartTime;				//������������ʱ��
	uint8_t  disturbingStopTime;				//������������ʱ��
	uint8_t  chargerMethod;						//�Ʒѷ�ʽ 1���̶��շ�  2��ʵʱ�շ�  3���𲽽��շ�
	uint16_t chargerStartingGold;				//�𲽽� add
}system_info_t;


typedef struct{
    BLUE_HEAD_STR blueHead;
    X6_PKT_HEAD_STR x6Head;
    uint8_t  data[OUT_NET_PKT_LEN];
}SYS_PKT_STR;


typedef struct{
    uint8_t  gunId;
    uint8_t  cmd;
    uint8_t  data[IN_QUEUE_NET_PKT_LEN];
}MSG_STR;


typedef struct {
    uint8_t  modeType;                          //1:2g;2:4g
    uint8_t  netType;                           //1:�ƶ�;2:����;3:��ͨ
}LISTEN_STR;

typedef struct {
    uint8_t  ledSn;
    uint8_t  status;
}LED_REQ_STR;

typedef struct {
    uint8_t  queueCnt;
    uint8_t  writeIndex;
    uint8_t  readIndex;
    uint8_t  queueLen;
    uint8_t* queue[QUEUE_BUFF_CNT];
}QUEUE_STR;

#pragma pack()


void ServerTask(void);
void PrintfData(void *pfunc, uint8_t *pdata, int len);
int GetCostMode(void);
int StrToHex(uint8_t *pbDest, const char *pbSrc, int srcLen);
void ProcNetStatus(void);
void ChargingCtrlProc(void);
int MuxSempTake(uint8_t *pSemp);
void MuxSempGive(uint8_t *pSemp);
void DeviceBcd2str(char *des, unsigned char *src , int len);
int GetGPRSBuffer(uint8_t *buf, uint16_t len);
int restoi(char* buf, char* start, char end);
int UsartGetline(uint8_t port, char *pBuff, uint16_t len, uint16_t tmo);
int GprsSendCmdChkNoSpace(char * cmd, char * ack, int waitCnt, int waittime);
uint8_t * GprsCmdChkNoSpace(char * ack);
void GetCcidSn(char *pStr);
int SendSocketData(uint8_t socket, uint8_t *pData, uint16_t length);
int StartProc(void);
void HeartBeatHandle(void);
void DebugRecvProc(void);
void FlashLight(void);
void SetUpgradeInfo(DOWN_FW_REQ_STR *pfwInfo, uint8_t chargerType);
void PktInQueue(PKT_STR *pPkt, uint8_t gunId);
void sysTickRecover(void);
extern int SendDebugPkt(uint8_t nodeIndex, OUT_PKT_STR *pFrame, uint16_t len);


extern system_info_t system_info;
extern system_info_str1 system_info1;
extern uint8_t  gNetSignal;
extern uint32_t gSimStatus;
extern uint8_t  gprsBuffer[BUFFER_SIZE];
extern uint8_t istage;
extern FIFO_S_t gSocketPktRxCtrl;
extern __IO uint8_t gGprsRxBuff[OUT_NET_PKT_LEN];
extern uint16_t gGprsRxLen;
extern uint8_t gGprsSendMux;
extern CHG_INFO_STR gChgInfo;
extern uint8_t  gSendHearBeatCnt;
extern gun_info_t gun_info[MAX_TERMINAL_NUM<<1];
extern QUEUE_STR gQueueInfo;
extern const char *gpFwName[];
extern uint8_t  gCurrentModuleInitType;        //���ڳ�ʼ����ʱ�򣬵�ǰ���ڽ���ͨѶģ���ʼ��������
extern uint8_t gGetModuleType;                 //����ͨѶģ�鷵�ص��豸����

#endif //__SERVER_H__


