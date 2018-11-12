/*proto.h
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __X6_PROC_H__
#define __X6_PROC_H__

#include "includes.h"


#define X6_PKT_MAX_LEN                          256

#define PIECE_PKT_LEN_24G_SEND_TO_X6            50      //不要轻易修改
#define RF24G_BUFF_PKT_LEN_SEND_TO_X6           220



enum {
    X6_FIND_AA,
    X6_FIND_55,
    X6_FIND_SRC,
    X6_FIND_DEST,
    X6_FIND_LEN,
    X6_FIND_VER,
    X6_FIND_SERNUM,
    X6_FIND_CMD,
    X6_RX_DATA,
    X6_FIND_CHK,
    X6_FIND_END,
};


enum {
    CMD_AUTH_REQ                		= 1,            //1. 鉴权请求
	CMD_AUTH_RESULT             		= 2,            //2. 鉴权结果返回
	CMD_REMOTE_SET_POWER_ON     		= 0x03,         //3. 请求开始充电
	CMD_REMOTE_SET_POWER_OFF    		= 0x04,         //4. 请求停止充电
	CMD_REPORT_POWER_ON         		= 0x05,         //5. 充电开始通知
	CMD_REPORT_POWER_OFF        		= 0x06,         //6. 充电结束通知
	CMD_HEART_BEAT              		= 0x07,         //7
	CMD_COST_DOWN               		= 0x08,         //8 计费模版下载
	CMD_COST_REQ                		= 0x09,         //9 请求计费模板
	
	CMD_DFU_DOWN_FW_INFO        		= 0x0A,         //10 固件升级
    CMD_FW_DOWN_LOAD_24G	    		= 0x0B,	        //11 固件下发
	CMD_FW_UPGRADE_REQ_24G	    		= 0x0C,	        //12 请求固件升级
	
    CMD_REMOTE_CTRL             		= 0x0d,         //13 远程控制
    CMD_SYSTEM_LOG_24G		    		= 0x0E,	        //14 系统日志
    CMD_EVENT_NOTICE            		= 0x0f,         //15 事件通知
    CMD_ENUM_BIND_REQ           		= 0x10,         //16 设备枚举
    CMD_SCAN_CODE_START_CHARGE     		= 0x20,         //扫码开启充电表示接受到X6的开启充电应答
};


#pragma pack(1)

typedef struct {
    uint8_t  ee;                                //0.
    uint8_t  len;                               //1.
    uint8_t  sn;                                //2.
    uint8_t  cmd;                               //3.
}X6_PKT_HEAD_STR;

typedef struct {
    X6_PKT_HEAD_STR head;
    uint8_t  data[X6_PKT_MAX_LEN-20];
}X6_PKT_STR;


typedef struct {
    uint8_t  gunId;                             //0. 枪头号
    uint8_t  optType;                           //1. 0：刷卡鉴权，开始充电；1：仅查询余额；2：手机用户反向鉴权
    uint8_t  userId[8];                         //2. Bcd	用户账号（或者卡号）
    uint8_t  passwd[3];                         //3. Bcd	用户账号认证密码
    uint8_t  mode;                              //4. 0：智能充满；1：按金额，具体参数见 充电参数；2：按时长；3：按电量
    uint16_t para;                              //5. 智能充满，为0；按金额，分辨率1分；按时长，分辨率1分钟；按电量，分辨率0.01kwh
}X6_AUTH_REQ_STR;

typedef struct {
    uint8_t  gunId;
    uint8_t  result;                            //0：接收成功；1：接收失败
}X6_AUTH_ACK_STR;

typedef struct {
	uint8_t  gunId;
    uint8_t  result;                            //0：接收成功；1：接收失败
}ACK_X6_RECEIVED_SCAN_CODE_STR;

typedef struct {
    uint8_t  gunId;                             //0. 枪头号
    uint8_t  result;                            //1. 0:鉴权成功；1：密码错误；2：余额不足或者次数已用完；3：正在使用，不能同时使用；4：账户无效；5：其它原因
    uint8_t  cardType;                          //2. 1：月卡
    uint32_t lefeMoney;                         //3. 账户余额，分辨率1分
    uint8_t  orderId[ORDER_SECTION_LEN];        //4. 后台生成的充电订单号
}X6_AUTH_RESULT_REQ_STR;

typedef struct {
    uint8_t  gunId;
    uint8_t  result;                            //0：接收成功；1：接收失败
}X6_AUTH_RESULT_ACK_STR;


typedef struct {
    uint8_t  gunId;
    uint8_t  mode;
    uint16_t para;
    uint8_t  orderId[ORDER_SECTION_LEN];
}REQ_START_CHARGING_STR;


typedef struct {
    uint8_t  gunId;
    uint8_t  reason;
}REQ_STOP_CHARGING_STR;


typedef struct {
    uint8_t  gunId;                             //0.
    uint8_t  status;                            //1.充电口状态：0 空闲；1 占用已打开，充电中；2 故障
    uint8_t  faultCode;                         // 1：状态异常； 2： 计量芯片通信故障
    uint8_t  voltage;                           //
    uint8_t  current;                           //
    uint16_t power;                             //
    uint16_t elec;                              //
    uint16_t money;                             //
    uint16_t rsv;                               //
}X6_GUN_STAUS_STR;

typedef struct {
    uint8_t  gunId;                             //0. 枪头号
    uint8_t  status;                            //1. 充电口状态：0 空闲；1 占用已打开，充电中；2 故障
    uint8_t  faultCode;                         //2. b0:10分钟没有收到
    uint16_t rsv;                               //3.
}X6_GUN_STAUS_IDEL_STR;

typedef struct {
    uint8_t  temp;                              //0. 温度
    uint8_t  fwVer;                             //1. 固件版本
    uint8_t  gunCnt;                            //2. 枪头数目
    X6_GUN_STAUS_STR gunStatus[2];
}X6_HB_ACK_STR;

typedef struct {
    uint32_t sysTime;                           //1.
    uint32_t id;                                //2. 计费模版ID
    uint8_t  x6Ver;                             //3. 固件版本
    uint8_t  gwAddr[MAC_LEN];                   //4. 网关地址
    uint8_t  startGunIndex;                     //5. 枚举设备起始枪头号
}X6_HB_REQ_STR;


typedef struct {
    uint16_t startPower;                        //起始功率 单位w
    uint16_t endPower;                          //结束功率 单位w
    uint16_t price;                             //指定时长充电费用，分辨率分
    uint16_t duration;                          //计费时长,分钟  费率=100,时长=120表示1元充2小时
}X6_segment_str;

typedef struct {
    uint8_t  segmentCnt;                        //功率段数目，1~4
    X6_segment_str segmet[COST_TEMP_CNT];
}X6_multiPower_t;

typedef struct {
    uint16_t  price;                            //指定时长充电费用，分辨率:分
    uint16_t  duration;                         //计费时长,分钟  费率=100,时长=120表示1元充2小时
}X6_unify_t;

// cost template   计费模版下载
typedef struct {
    uint8_t  gunId;                             //0.
	uint32_t template_id;                       //1.
    uint8_t  mode;                              //2：按功率段计费 2按统一收费
    uint8_t  type;                              //3. 1：固定计费；2：实时计费
    uint16_t chargerStartingGold;				//起步金 add
    union {
        X6_multiPower_t powerInfo;
        X6_unify_t unifyInfo;
    }Data;
}X6_COST_TEMPLATE_REQ_STR;


typedef struct {
    uint8_t  gunId;                             //0.
    uint8_t  orderId[ORDER_SECTION_LEN];        //1.
    uint32_t startTime;                         //2.
    uint8_t  result;                            //3.
}X6_START_NOTICE_STR;


typedef struct {
    uint8_t  gunId;
    uint8_t  result;                            //0：接收成功；1：接收失败
}X6_START_NOTICE_ACK_STR;


typedef struct {
    uint8_t  gunId;
    uint8_t  result;
    uint8_t  reason;
}X6_START_ACK_STR;


typedef struct {
    uint8_t  gunId;
    uint8_t  stopReason;
    uint8_t  reasonDetal;
    uint32_t startTime;
    uint16_t chargingTime;
    uint16_t elec;
    uint16_t money;
    uint16_t power;
    uint8_t  orderId[ORDER_SECTION_LEN];
}X6_STOP_NOTICE_STR;

typedef struct {
    uint8_t  gunId;
    uint8_t  result;                            //0：接收成功；1：接收失败
}X6_STOP_NOTICE_ACK_STR;

typedef struct {
    uint8_t  gunId;
    uint8_t  result;
}X6_STOP_ACK_STR;


typedef struct {
    uint8_t  gunId;
    uint8_t  code;
    uint8_t  para1;
    uint32_t para2;
    uint8_t  status;
    uint8_t  level;
}X6_EVENT_NOTICE_REQ_STR;

typedef struct {
    uint8_t  gun_id;                            //0
	uint8_t  ReqFwVer;                            //请求固件版本号
	uint32_t  AddrOffset;                            //固件地址偏移 , 从此处开始接着升级
}X6_REQ_UPGRADE_STR_BTRF2_4G_TYPE;

typedef struct {
    uint32_t  ServerTime;         //时间
}R6X6_BIND_REQ_STR_24G_TYPE;

typedef struct {
    uint8_t  deviceID[MAC_LEN];      	//设备号
	uint8_t  baseGunSn;        	//起始枪头
}R6X6_BIND_ACK_STR_24G_TYPE;

typedef struct {
	uint8_t  result;                            // 0： 可以升级； 1： 不能升级
}ACK_X6_UPGRADE_STR_BTRF2_4G_TYPE;

// 固件升级请求
typedef struct{
	uint32_t     file_size;                 //文件大小      
    uint16_t     package_num;               //固件分片总数   
	uint32_t     checkSum;                  //校验码
    uint8_t      fwverson;                  //固件版本号
}X6_FW_UPGRADE_NOTI_REQ_STR_24G;

typedef struct {
	uint8_t UpgeadeState;                             // 0：可以升级； 1：不能升级
}X6_FW_UPGRADE_ACK_STR_24G;

//固件下发
typedef struct{
    uint16_t package_num;               //固件分片序号
	uint8_t  checkSum;           //固件分片校验码
	uint8_t len;               //固件分片长度
	uint8_t  data[PIECE_PKT_LEN_24G_SEND_TO_X6];         //固件分片静荷
}X6_DOWN_FW_REQ_STR_24G;

typedef struct{
	uint8_t result;             //0： 接收成功； 1：接收失败 2：停止升级
	uint16_t package_num;       //固件分片序号 X6发送给R6说我要接收哪一个分片包
	uint8_t gunId;              //枪头号
}X6_DOWN_FW_ACK_STR_24G;

#pragma pack()


void x6RecvTask(void);
int SendX6Pkt(uint8_t nodeIndex, uint16_t sn, X6_PKT_STR *pPkt, uint16_t len);
void SyncCostTemplate(void);
void RecvX6Data(uint8_t *pBuff, uint16_t len);
int SendX6HeartBeatReqPkt(X6_PKT_STR *pPkt, uint8_t gunId);
int GetTerminalIdByGunId(uint8_t gunIndex);
int GetGunIdByTerminalId(uint8_t nodeId);
/*****************************************************************************
** Function name:       SendX6FwUpgradeDownload2_4gNotice
** Descriptions:        //给X6下发要升级的分片包
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
extern void SendX6FwUpgradeDownload2_4gNotice(uint8_t gunId);

/*****************************************************************************
** Function name:       X6Req2_4gUpgrade
** Descriptions:        //X6请求固件升级
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
extern void X6Req2_4gUpgrade(X6_PKT_STR *pMsg);
/*****************************************************************************
** Function name:       CheckUpgradeWaitTimer
** Descriptions:        //升级过程中，大于50s没有接收到数据说明 X6 已经断电或离线了
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void CheckUpgradeWaitTimer(void);




#endif


