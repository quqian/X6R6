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

#define RECORD_MAX_BLOCK_NUM                        512     //最多记录订单数量
#define SPI_FLASH_PAGE_SIZE					        4096    //flash页大小
#define	RECORD_BLOCK_SIZE_PER_PAGE		            256     //一条订单最大缓存
#define	RECORD_BLOCK_NUM_PER_PAGE		            16//(SPI_FLASH_PAGE_SIZE/RECORD_BLOCK_SIZE_PER_PAGE)    //flash每页可记录的订单数量
//#define	RECORD_FLASH_SIZE				            (RECORD_MAX_BLOCK_NUM*RECORD_BLOCK_SIZE_PER_PAGE)	//记录订单的flash空间


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
    TEST_CMD_KEY = 1,			//按键测试
	TEST_CMD_CARD = 2,			//刷卡测试
	TEST_CMD_LCD = 3,			//LCD测试
	TEST_CMD_2G = 4,			//2G模块测试
	TEST_CMD_VOICE = 5,			//喇叭测试
	TEST_CMD_RELAY = 6,			//继电器
	TEST_CMD_PCB_CODE = 7,			//PCB编码
    DEBUG_CMD_SN = 8,
    TEST_CMD_BLE = 0x09, //BLE测试
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
    DEBUG_CMD_SET_RTC,                          //0x15 设置RTC时钟
    DEBUG_CMD_RED_RTC,                          //0x16 读取RTC时钟
    DEBUG_CMD_FLASH,                            //0x17 Flash读写测试
	DEBUG_CMD_SET_LISTEN_MODE,                  	//0x18
    DEBUG_CMD_QUE_LISTEN_MODE,                  	//0x19
    DEBUG_CMD_LED,                              //0x1a Led指示灯测试
    DEBUG_CMD_485,                              //0x1b 485接口测试
    DEBUG_CMD_NET_CONNECT,                      //0x1c 后台对接测试
    DEBUG_CMD_4G_TO_2G,                         //0x1d 设置4G兼容2G
    DEBUG_CMD_GET_CHARGER_SN,                   //0x1e 获取桩编号
    DEBUG_CMD_GET_DEVICE_ID,                    //0x1f 获取识别码
    START_AGE_TEST,                             //0x20 启动老化测试
    GET_AGE_TEST_RESULT,                        //0x21 获取老化测试结果
    DEBUG_CMD_24G_COMMUNICATION = 0xF3,                    //2.4G通信测试 
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
    uint8_t  heartBeatStatus;                       //心跳状态  0:未知；1:心跳不正常；2:心跳正常
    uint16_t sendCnt;
    uint16_t recvCnt;
    uint16_t reqStartChargingSn;
    uint16_t reqStopChargingSn;
}TERMINAL_STATUS_STR;


typedef struct {
    uint8_t  nodeIndex;     //从节点0开始，一个X6就是一个节点
    uint8_t  sendTimes;
}LOOP_SEND_STR;

typedef struct {
    uint32_t  Upgrade2_4Gflag:2;         //2.4G升级标志 1:正在升级 2：不能升级 
    uint32_t  isSendFwUpgradeOk:2;       //发送固件升级0x0A是否成功
    uint32_t  X6R6BindRelation:2;       //
    uint32_t  ReceiveX6BindMsg:2;       //收到X6发送过来的绑定信息
    //uint32_t  FwUpgradeFlag:2;			//如果为 1 表示正在FTPGET X6升级包
}BIT_FLAG_STR;

typedef struct {
    uint8_t  netStatus;                             //网络操作状态: b0:是否发生过心跳接收失败 b1:是否发送过发送报文失败 b2:是否带有蓝牙 b3:是否带有2.4G
    uint8_t  netLedStatus;                          //收到包后置1，闪灯后清零
    uint8_t  blueLedStatus;                         //收到包后置1，闪灯后清零
    uint8_t  sendHbStartGunIndex;                   //发送的起始枪头id标志

    uint8_t  authAckRecvFlag;                       //刷卡鉴权应答发送的确认标志
    uint8_t  reqX6StartCharging;                    //请求x6启动充电标识
    uint8_t  reqX6StopCharging;                     //请求x6停止充电标识
    uint8_t  authReqSn;                             //正在进行鉴权的sn号

    uint8_t  startChargingNoticeSn;                 //开启充电通知序号
    uint8_t  stopChargingNoticeSn;                  //x6 停止充电通知序号
    uint8_t  sendPKtFlag;                           //0:可以发；1:正在升级
    LOOP_SEND_STR loopSend;
    TERMINAL_STATUS_STR terminalStatus[MAX_TERMINAL_NUM];
    BIT_FLAG_STR flagBit;
    uint16_t current_package[MAX_TERMINAL_NUM];       //固件分片序号 X6发送给R6说我要接收哪一个分片包
    uint8_t  gun_id;                //2.4G升级的枪头号
    uint32_t gUpgradeWaitTimerTick;
    uint8_t reserve;
	uint8_t isBlueToothInit;
	uint8_t X6R6BindSn;
	uint8_t UpgradeFlag;	//0可以升级 , 1不能升级
	uint8_t ReceiveR624gDataFlag;
	uint8_t ProductToolTestFlag;
	uint32_t gCardAuthNoticeTick[MAX_TERMINAL_NUM];
	uint32_t gPowerOffNoticeTick[MAX_TERMINAL_NUM];
}CHG_INFO_STR;


#pragma pack(1)

typedef struct {
    uint8_t  aa;                                    //0.
    uint8_t  five;                                  //1.
    uint8_t  src[5];                                //2. 桩号   Bcd
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
    uint32_t  sum;                                   //0:固件无效 1:固件有效
}X6_FW_STR;


typedef struct{
    X6_GUN_STAUS_STR  gunStatus;
	uint8_t  order[ORDER_SECTION_LEN];
}gun_info_t;


typedef struct {
    uint16_t wIndex;
	uint16_t rIndex;
    uint16_t orderCnt;
    uint8_t  sendCnt;                               //不为0，则有正在发送的订单
    uint8_t  gunId;                                 //不为0，则有正在发送的订单
}ORDER_STR;

typedef struct {
    uint8_t UpgradeFinish[MAX_TERMINAL_NUM];	//为1表示升级完成
	uint16_t x6package_num[MAX_TERMINAL_NUM];
}IS_UPGRADE_OK_STR;

typedef struct {
    uint8_t  station_id[8];                         // bcd, SN=9, 10 byte  0000123456789
    uint8_t  gwAddr[MAC_LEN];                       //网关地址
}system_info_str1;

typedef struct {
    uint32_t magic_number;
	char     idCode[8];                             // bcd 识别码		modified by liutao, 2017/11/27
	uint8_t     preserve[32];								//保留
	COST_TEMPLATE_REQ_STR cost_template;
    uint32_t rsv[8];                               	//保留
    uint8_t  iccid[22];
    char     server_ip[32];                         // ASCII "202.104.136.212"
    uint16_t server_port;
    uint8_t  rsv1[8];                         		// 保留
    uint32_t mqtt_sn;
    volatile uint8_t    is_socket_0_ok;
    uint8_t  tcp_tx_error_times;
    uint8_t  isRecvStartUpAck;
    uint8_t  netType;                               //0:未检测  2:485  3:蓝牙2.4G
    uint8_t  localNetType;                          //1:2g； 2:4g； 3:nbiot-me3612
    volatile uint8_t  printfSwitch;                          //打印开关
    uint8_t  rsv2[5];                       			//保留
    X6_FW_STR  x6FwInfo;
	uint16_t x6PktSn;
    uint16_t x6AuthResultPktSn;
    uint8_t x6SetPowerOnPktSn;
    uint8_t x6SetPowerOffPktSn;
    uint8_t x6UpgradePktSn;
    uint8_t  chargerCnt;                            //x6数目
    CHARGER_STR chargerInfo[MAX_TERMINAL_NUM];
    ORDER_STR orderInfo;                            //缓存订单信息
    uint16_t x6_package_num;               //固件分片总数 
    uint8_t  pcbCode[8];                        //pcb编码
    IS_UPGRADE_OK_STR isUpgradeOk;
	uint8_t  disturbingStartTime;				//语音禁播开启时间
	uint8_t  disturbingStopTime;				//语音禁播结束时间
	uint8_t  chargerMethod;						//计费方式 1、固定收费  2、实时收费  3、起步金收费
	uint16_t chargerStartingGold;				//起步金 add
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
    uint8_t  netType;                           //1:移动;2:电信;3:联通
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
extern uint8_t  gCurrentModuleInitType;        //用于初始化的时候，当前正在进行通讯模块初始化的类型
extern uint8_t gGetModuleType;                 //用于通讯模块返回的设备类型

#endif //__SERVER_H__


