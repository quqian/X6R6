/*proto.h
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __X6_PROC_H__
#define __X6_PROC_H__

#include "includes.h"


#define X6_PKT_MAX_LEN                          256

#define PIECE_PKT_LEN_24G_SEND_TO_X6            50      //��Ҫ�����޸�
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
    CMD_AUTH_REQ                		= 1,            //1. ��Ȩ����
	CMD_AUTH_RESULT             		= 2,            //2. ��Ȩ�������
	CMD_REMOTE_SET_POWER_ON     		= 0x03,         //3. ����ʼ���
	CMD_REMOTE_SET_POWER_OFF    		= 0x04,         //4. ����ֹͣ���
	CMD_REPORT_POWER_ON         		= 0x05,         //5. ��翪ʼ֪ͨ
	CMD_REPORT_POWER_OFF        		= 0x06,         //6. ������֪ͨ
	CMD_HEART_BEAT              		= 0x07,         //7
	CMD_COST_DOWN               		= 0x08,         //8 �Ʒ�ģ������
	CMD_COST_REQ                		= 0x09,         //9 ����Ʒ�ģ��
	
	CMD_DFU_DOWN_FW_INFO        		= 0x0A,         //10 �̼�����
    CMD_FW_DOWN_LOAD_24G	    		= 0x0B,	        //11 �̼��·�
	CMD_FW_UPGRADE_REQ_24G	    		= 0x0C,	        //12 ����̼�����
	
    CMD_REMOTE_CTRL             		= 0x0d,         //13 Զ�̿���
    CMD_SYSTEM_LOG_24G		    		= 0x0E,	        //14 ϵͳ��־
    CMD_EVENT_NOTICE            		= 0x0f,         //15 �¼�֪ͨ
    CMD_ENUM_BIND_REQ           		= 0x10,         //16 �豸ö��
    CMD_SCAN_CODE_START_CHARGE     		= 0x20,         //ɨ�뿪������ʾ���ܵ�X6�Ŀ������Ӧ��
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
    uint8_t  gunId;                             //0. ǹͷ��
    uint8_t  optType;                           //1. 0��ˢ����Ȩ����ʼ��磻1������ѯ��2���ֻ��û������Ȩ
    uint8_t  userId[8];                         //2. Bcd	�û��˺ţ����߿��ţ�
    uint8_t  passwd[3];                         //3. Bcd	�û��˺���֤����
    uint8_t  mode;                              //4. 0�����ܳ�����1��������������� ��������2����ʱ����3��������
    uint16_t para;                              //5. ���ܳ�����Ϊ0�������ֱ���1�֣���ʱ�����ֱ���1���ӣ����������ֱ���0.01kwh
}X6_AUTH_REQ_STR;

typedef struct {
    uint8_t  gunId;
    uint8_t  result;                            //0�����ճɹ���1������ʧ��
}X6_AUTH_ACK_STR;

typedef struct {
	uint8_t  gunId;
    uint8_t  result;                            //0�����ճɹ���1������ʧ��
}ACK_X6_RECEIVED_SCAN_CODE_STR;

typedef struct {
    uint8_t  gunId;                             //0. ǹͷ��
    uint8_t  result;                            //1. 0:��Ȩ�ɹ���1���������2��������ߴ��������ꣻ3������ʹ�ã�����ͬʱʹ�ã�4���˻���Ч��5������ԭ��
    uint8_t  cardType;                          //2. 1���¿�
    uint32_t lefeMoney;                         //3. �˻����ֱ���1��
    uint8_t  orderId[ORDER_SECTION_LEN];        //4. ��̨���ɵĳ�綩����
}X6_AUTH_RESULT_REQ_STR;

typedef struct {
    uint8_t  gunId;
    uint8_t  result;                            //0�����ճɹ���1������ʧ��
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
    uint8_t  status;                            //1.����״̬��0 ���У�1 ռ���Ѵ򿪣�����У�2 ����
    uint8_t  faultCode;                         // 1��״̬�쳣�� 2�� ����оƬͨ�Ź���
    uint8_t  voltage;                           //
    uint8_t  current;                           //
    uint16_t power;                             //
    uint16_t elec;                              //
    uint16_t money;                             //
    uint16_t rsv;                               //
}X6_GUN_STAUS_STR;

typedef struct {
    uint8_t  gunId;                             //0. ǹͷ��
    uint8_t  status;                            //1. ����״̬��0 ���У�1 ռ���Ѵ򿪣�����У�2 ����
    uint8_t  faultCode;                         //2. b0:10����û���յ�
    uint16_t rsv;                               //3.
}X6_GUN_STAUS_IDEL_STR;

typedef struct {
    uint8_t  temp;                              //0. �¶�
    uint8_t  fwVer;                             //1. �̼��汾
    uint8_t  gunCnt;                            //2. ǹͷ��Ŀ
    X6_GUN_STAUS_STR gunStatus[2];
}X6_HB_ACK_STR;

typedef struct {
    uint32_t sysTime;                           //1.
    uint32_t id;                                //2. �Ʒ�ģ��ID
    uint8_t  x6Ver;                             //3. �̼��汾
    uint8_t  gwAddr[MAC_LEN];                   //4. ���ص�ַ
    uint8_t  startGunIndex;                     //5. ö���豸��ʼǹͷ��
}X6_HB_REQ_STR;


typedef struct {
    uint16_t startPower;                        //��ʼ���� ��λw
    uint16_t endPower;                          //�������� ��λw
    uint16_t price;                             //ָ��ʱ�������ã��ֱ��ʷ�
    uint16_t duration;                          //�Ʒ�ʱ��,����  ����=100,ʱ��=120��ʾ1Ԫ��2Сʱ
}X6_segment_str;

typedef struct {
    uint8_t  segmentCnt;                        //���ʶ���Ŀ��1~4
    X6_segment_str segmet[COST_TEMP_CNT];
}X6_multiPower_t;

typedef struct {
    uint16_t  price;                            //ָ��ʱ�������ã��ֱ���:��
    uint16_t  duration;                         //�Ʒ�ʱ��,����  ����=100,ʱ��=120��ʾ1Ԫ��2Сʱ
}X6_unify_t;

// cost template   �Ʒ�ģ������
typedef struct {
    uint8_t  gunId;                             //0.
	uint32_t template_id;                       //1.
    uint8_t  mode;                              //2�������ʶμƷ� 2��ͳһ�շ�
    uint8_t  type;                              //3. 1���̶��Ʒѣ�2��ʵʱ�Ʒ�
    uint16_t chargerStartingGold;				//�𲽽� add
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
    uint8_t  result;                            //0�����ճɹ���1������ʧ��
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
    uint8_t  result;                            //0�����ճɹ���1������ʧ��
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
	uint8_t  ReqFwVer;                            //����̼��汾��
	uint32_t  AddrOffset;                            //�̼���ַƫ�� , �Ӵ˴���ʼ��������
}X6_REQ_UPGRADE_STR_BTRF2_4G_TYPE;

typedef struct {
    uint32_t  ServerTime;         //ʱ��
}R6X6_BIND_REQ_STR_24G_TYPE;

typedef struct {
    uint8_t  deviceID[MAC_LEN];      	//�豸��
	uint8_t  baseGunSn;        	//��ʼǹͷ
}R6X6_BIND_ACK_STR_24G_TYPE;

typedef struct {
	uint8_t  result;                            // 0�� ���������� 1�� ��������
}ACK_X6_UPGRADE_STR_BTRF2_4G_TYPE;

// �̼���������
typedef struct{
	uint32_t     file_size;                 //�ļ���С      
    uint16_t     package_num;               //�̼���Ƭ����   
	uint32_t     checkSum;                  //У����
    uint8_t      fwverson;                  //�̼��汾��
}X6_FW_UPGRADE_NOTI_REQ_STR_24G;

typedef struct {
	uint8_t UpgeadeState;                             // 0������������ 1����������
}X6_FW_UPGRADE_ACK_STR_24G;

//�̼��·�
typedef struct{
    uint16_t package_num;               //�̼���Ƭ���
	uint8_t  checkSum;           //�̼���ƬУ����
	uint8_t len;               //�̼���Ƭ����
	uint8_t  data[PIECE_PKT_LEN_24G_SEND_TO_X6];         //�̼���Ƭ����
}X6_DOWN_FW_REQ_STR_24G;

typedef struct{
	uint8_t result;             //0�� ���ճɹ��� 1������ʧ�� 2��ֹͣ����
	uint16_t package_num;       //�̼���Ƭ��� X6���͸�R6˵��Ҫ������һ����Ƭ��
	uint8_t gunId;              //ǹͷ��
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
** Descriptions:        //��X6�·�Ҫ�����ķ�Ƭ��
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
extern void SendX6FwUpgradeDownload2_4gNotice(uint8_t gunId);

/*****************************************************************************
** Function name:       X6Req2_4gUpgrade
** Descriptions:        //X6����̼�����
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
extern void X6Req2_4gUpgrade(X6_PKT_STR *pMsg);
/*****************************************************************************
** Function name:       CheckUpgradeWaitTimer
** Descriptions:        //���������У�����50sû�н��յ�����˵�� X6 �Ѿ��ϵ��������
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void CheckUpgradeWaitTimer(void);




#endif


