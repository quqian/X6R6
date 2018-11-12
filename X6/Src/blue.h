
#ifndef __BLUE_H__
#define __BLUE_H__
#include "stdint.h"
#include "includes.h"

#define OUT_NET_PKT_LEN             128

enum {
    BT_SHAKE_REQ = 1,               	//��������
    BT_OPPO_SITE_AUTH,              	//�û������Ȩ
    BT_START_CHARGING,              	//�������
    BT_STOP_CHARGING,               	//�������
    BT_HEART_BEAT = 5,                  //ң�ż�����
    BT_COST_TEMPLATE_UPLOAD,        	//�Ʒ�ģ���ϴ�
    BT_REQ_COST_TEMPLATE,           	//����Ʒ�ģ��
    BT_FW_UPGRADE_NOTICE,           	//�̼�������ʼ֪ͨ//�ֻ���������->X6 , X6Ӧ��->�ֻ�
    BT_FW_DOWN_LOAD,                	//�̼��·�
    BT_REQ_BREAK = 0x0A,                //����Ͽ���������
    BT_DEV_REGISTER	= 0x0B,				//�豸ע��
	BT_DEV_LOGIN	= 0x0C,				//�豸��½
	BT_HISTORY_ORDER_UPLOAD=0x0D,		//��ʷ�����ϴ�
	BT_COST_TEMPLATE_DOWNLOAD=0x0E,		//�Ʒ�ģ���·�
    BT_CMD_SET_CHARGER_SN = 0x0F,   	//���ó��׮���
    BT_CMD_SET_DEVICE_ID = 0x10,           	//���ó��׮ʶ����
    BT_CMD_SET_GW_ADDR = 0x11,             	//���� 2.4G ���ص�ַ
    BT_CMD_SET_SERVER_ADDR = 0x12,         	//���÷�����IP
    BT_CMD_SET_SERVER_PORT = 0x13,         	//���÷������˿�
    BT_CMD_SET_TERMINAL_INFO = 0x14,    //�����ն˱����Ϣ
    BT_CMD_REMO_CTRL,               	//Զ�̿���
};


#pragma pack(1)

typedef struct {
    uint8_t  ab;                                //0.
    uint8_t  cd;                                //1.
    uint8_t  target;                            //2.
    uint16_t len;                               //4.
}BLUE_HEAD_RECEIVE_STR;

typedef struct {
    BLUE_HEAD_RECEIVE_STR head;
    uint8_t  data[OUT_NET_PKT_LEN];             // x6����ͷ + x6����
}BLUE_TOOTH_RECEIVE_PKT_STR;

typedef struct {
    uint8_t  ab;                                //0.
    uint8_t  cd;                                //1.
    uint8_t  target;                            //2.
    uint8_t  addr[6];  
    uint16_t len;                               //4.
}BLUE_HEAD_SEND_STR;

typedef struct {
    BLUE_HEAD_SEND_STR head;
    uint8_t  data[OUT_NET_PKT_LEN];             // x6����ͷ + x6����
}BLUE_TOOTH_SEND_PKT_STR;


typedef struct {
    uint8_t  start_ee;
    uint8_t  cmd;
    uint8_t  len;
}BLUE_PROTO_HEAD_STR;

typedef struct {
    BLUE_PROTO_HEAD_STR head;
    uint8_t  data[64];
    //uint8_t  checkSum;
}BLUE_PROTO_STR;

/***********�ֻ���������**********/
typedef struct {
    uint32_t time;
	char phonesSn[12];
}BLUE_SHAKE_REQ_STR;

typedef struct {
    char     name[4];                               //0. �豸�ͺ�
    uint8_t  chargerSn[5];                          //1. ׮���
    uint8_t  fwVer;                                 //2. �̼��汾��
    uint8_t  portCnt;                               //3. ������Ŀ
    uint8_t  startNo;                               //4. ������ʼ���
}BLUE_SHAKE_ACK_STR;

typedef struct {
    uint8_t Local24gAddr[5];					    //����2.4mac��ַ
}BLUE_2_4G_ADDR_STR;

typedef struct {
    uint8_t Result;
}BLUE_2_4G_ADDR_ACK_STR;

typedef struct {
    uint8_t DeviceID[8];					    //�豸��
}BLUE_DEV_ID_STR;

typedef struct {
    uint8_t Result;
}BLUE_DEV_ID_ACK_STR;

typedef struct {
    uint8_t  port;                                  //0. ǹ�� ��1��ʼ
    uint8_t  order[ORDER_SECTION_LEN];              //1. bcd ������
    uint8_t  mode;                                  //2. 0�����ܳ�����1��������������� ��������2����ʱ����3��������
    uint16_t chargingPara;                                  //3. ���ܳ�����Ϊ0�������ֱ���1�֣���ʱ�����ֱ���1���ӣ����������ֱ���0.01kwh
	uint8_t orderSource;							//4. ������Դ
	uint8_t subsidyType;							//5. ��������
	uint16_t subsidyPararm;							//6. ��������
}BLUE_START_CHARGING_STR;

typedef struct {
    uint8_t  port;                                  //0. ǹ�� ��1��ʼ
    uint8_t  result;                                //1. 0:�����ɹ���1: ����ʧ��
    uint8_t  reason;                                //2. 1���˿ڹ��ϣ�2��û�мƷ�ģ�棻3���Ѿ��ڳ���У�4���豸û��У׼��5����������
	uint32_t startTime;								//3. ����ʱ��
	uint16_t startElec;								//4. ��������
}BLUE_START_CHARGING_ACK_STR;

/**********����Ʒ�ģ��********/ 
typedef struct{
	uint8_t gun_id;
	uint32_t template_id;
}BLUE_COST_TEMPLATE_REQ;

/**********�Ʒ�ģ���·�ACK*********/ 
typedef struct {
	uint8_t  result;
}BLUE_COST_TEMPLATE_ACK_STR;

typedef struct {
    uint8_t  gunId;
	uint32_t template_id;
    uint8_t  mode;                              //1�������ʶμƷ� 2��ͳһ�շ�
	uint8_t Data[256];
}COST_TEMPLATE_HEAD_STR;

typedef struct {
    uint8_t  gunId;
	uint8_t  result;
}COST_TEMPLATE_ACK_STR;

/**********�̼�����********/ 
typedef struct{
	uint32_t     fw_size;
	uint32_t     package_num;                       
	uint16_t     checkSum;
    uint8_t      fw_version;
}BLUE_DOWN_FW_REQ_STR;
typedef struct {
	uint8_t result;                             //0: �����ɹ�  1: ����ʧ�� 2: У��ʧ�� 3: д��ʧ��
}BLUE_DOWN_FW_ACK_STR;

/**********�̼��·�********/ 
typedef struct {
	uint8_t data[64];
}BLUE_FW_DOWNLOAD_REQ_STR;
typedef struct {
	uint8_t result;
	uint8_t index;
}BLUE_FW_DOWNLOAD_ACK_STR;

/**********����Ͽ�����********/ 
typedef struct {
	uint32_t     timestamp;	
}BLUE_DISCONNECT_DEV_REQ_STR;
typedef struct {
	uint8_t     status;	
}BLUE_DISCONNECT_DEV_ACK_STR;

/**********ң�ż�����*********/ 
typedef struct {
    uint8_t  port;                                  //0. ǹ�� ��1��ʼ 
    uint8_t  status;                                //1. ����״̬��0 ���У�1 ռ�ã�2 ���ϣ�3 ����
    uint8_t  errCode;                               //2. 1~255  ������״̬Ϊ2����ʱ��Ч��1��״̬�쳣��2������оƬͨ�Ź���
}BLUE_GUN_HEART_BEAT_STR;

typedef struct {
    uint8_t  simSignal;                             //0. Sim���ź�
    uint8_t  temp;                                  //1. �����¶� �� -50��ƫ��  -50~200
    uint8_t  portCnt;                               //2. ���α����������ĳ��ӿ���Ŀ
    BLUE_GUN_HEART_BEAT_STR gunStatus[GUN_NUM_MAX];
}HEART_BEAT_STR;

typedef struct {
    uint32_t time;                                  //0. ϵͳʱ��
    uint8_t  result;                                //1. 0���ճɹ���1����ʧ��
}BLUE_HEART_BEAT_ACK_STR;

/***********����������*********/
typedef struct {
    uint8_t  port;                                  //0. ǹ�� ��1��ʼ
    uint8_t  order[ORDER_SECTION_LEN];              //1. bcd ������
}BLUE_STOP_CHARGING_STR;

typedef struct {
    uint8_t  port;                                  //0. ǹ�� ��1��ʼ
    uint8_t  result;                                //1. 0:�ɹ���1: ʧ��
	uint32_t stopTime;								//2. ����ʱ��
	uint16_t stopElec;								//3. ��������
	uint8_t stopReason;								//4. ����ԭ��
	uint32_t start_time;							//��ʼ���ʱ��
	uint16_t startElec;              			//0.01kwh ��ʼ������
	uint16_t money;                     			//�ѳ���� ��
	uint16_t chargerPower;             				//���ڹ��ʶμƷѵĹ��� w
}BLUE_STOP_CHARGING_ACK_STR;

#pragma pack()



//�ֻ��������ݽ��մ���
extern void RecvBtData(void);
extern uint16_t GetHistoryOrderNum(void);
extern void BlueSendHeartBeat(void);
#endif


