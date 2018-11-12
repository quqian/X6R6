#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "includes.h"
#include "FIFO.h"
#include "24g_proto.h"


#define COST_TEMP_CNT                   6


enum {
    CARD_AUTH = 0,
    CARD_CHECK_MONEY,
	CARD_MONTH,
};


enum {
    EVENT_PROMPT = 1,
    EVENT_ALARM,
    EVENT_FAULT,
};

typedef struct {
    uint8_t gSc8042bMux;
    uint8_t gBuff1024Byte;
    uint8_t gOrderReportMux;
    uint8_t gUpgradeReportMux;
    uint8_t gBlueSendMux;
    uint8_t preserve[1];    //����
}PROTO_MUX_TYPE;

typedef struct {
    uint32_t isReadCardFlag:1;
    uint32_t ReadCardTimeLenth:1;
    uint32_t isNeedSendUpgradeFlag:1;      //
    uint32_t isBT_UpgradeFlag:1;      //
    uint32_t isUART_UpgradeFlag:1;      //
    uint32_t isBTSet24gAddrFlag:2;      //
    uint32_t fuseState:2;
}RF2_4G_CARD_FLAG_TYPE;

#define  POWER_CHECK_CNT                        60

typedef struct {
    uint32_t chargingTime;
    uint32_t beginTime;                         //��ʱ��ʼʱ��
    //uint16_t timeLimit;                         //ʱ������ ��
    uint16_t currentPower;                      //ʵʱ��繦�� 0.1w
    //uint8_t  isTesting;                         //0:û�в���״̬��1:���Դ򿪣�2:Ӧ����򿪣�3:��̨Զ�̿��ƴ�
    //uint8_t  isPullOutGun;                      //�Ƿ����ư�ǹ 0���ǣ�1�ǲ������¼���δ�յ�Ӧ�� 0xff:�����¼����յ�Ӧ��,���߷��ͳ�ʱû��Ӧ��
    uint8_t  checkPowerCnt;                     //��⹦�ʴ���
    //uint8_t  inCnt;
    uint8_t  isFull;
    uint8_t  powerIndex;
    uint16_t power[POWER_CHECK_CNT];
    //uint8_t  startChargerTimers;                //��ʼ���֪ͨ���ʹ���
    //uint8_t  resetEmuChipFlag;
    uint8_t  stopFlag;                          //�����ʱ�رռ̵�����־
    uint32_t stopTime;                          //�رռ̵���ʱ��
    uint8_t isRebootMCU;                  
	uint8_t isTimesFlag;
}GUN_CHARGING_STR;

typedef struct
{
    uint8_t  isOverTimeFlag;
    uint8_t  heartReceiveFlag;
	uint16_t chargingTotalPower;                //����ܹ��� w
	
	uint8_t  netStatus;
    uint32_t  money;                       //ˢ����ɨ��ѡ��ĳ����  Ԫ(��λ��0.1Ԫ)
	uint8_t  current_usr_gun_id;          //1~12
    uint8_t  mode;                        //���ģʽ 0�����ܳ��� 1������� 2����ʱ�� 3��������
    
	uint8_t  current_usr_card_id[16];       //ˢ����ȡ�Ŀ���
	uint32_t user_card_balance;           //�û������ ��ˢ����ȨӦ��õ� �ֻ����� ��
	
	RF2_4G_CARD_FLAG_TYPE  RF24GCardFlag;
	PROTO_MUX_TYPE gProtoMuxType;
    uint32_t gUpgradeWaitTimerTick;
    uint8_t  reserve[3];
	uint8_t isBlueToothInit;
	uint32_t gStartChargeTimeTick[GUN_NUM_MAX];
	uint32_t gCardChargeTimeTick[GUN_NUM_MAX];
	uint32_t gStartChargeNoticeTick[GUN_NUM_MAX];
	uint32_t gStopChargeNoticeTick[GUN_NUM_MAX];
	uint32_t gCardAuthAckNoticeTick[GUN_NUM_MAX];
	uint32_t gStartChargingNoticeTick[GUN_NUM_MAX];
	//uint32_t gScanCodeChargerNoticeTick[GUN_NUM_MAX];
	GUN_CHARGING_STR gunCharging[GUN_NUM_MAX];
	uint32_t ReceiveR6Ticks;
	uint8_t lastFuseholderStatus;				//��һ�α���˿״̬ 0������ 1-����
	uint8_t sendFuseholderStatus;				//���ʹ���
	uint8_t blue_connectstate;					//��������״̬  0 :��ʾ�����ѶϿ�����              1:��ʾ����������
	uint8_t gun_id[GUN_NUM_MAX];
	uint8_t TestCardFlag;
	uint8_t ReadCardFlag;
	uint8_t UpperMonitorTestFlag;
	uint32_t UpperMonitorTicks;
	uint8_t ReceiveR624gDataFlag;
	uint8_t X6OldTestFlag;
	char  blue_name[12];						//��������
}CHG_INFO_STR;

#pragma pack(1)

typedef struct {
    uint16_t startPower;                        //��ʼ���� ��λw
    uint16_t endPower;                          //�������� ��λw
    uint16_t price;                             //ָ��ʱ�������ã��ֱ��ʷ�
    uint16_t duration;                          //�Ʒ�ʱ��,����  ����=100,ʱ��=120��ʾ1Ԫ��2Сʱ
}segment_str;

typedef struct {
    uint8_t  segmentCnt;                        //���ʶ���Ŀ��1~4
    segment_str segmet[COST_TEMP_CNT];
}multiPower_t;

typedef struct {
    uint16_t  price;                            //ָ��ʱ�������ã��ֱ���:��
    uint16_t  duration;                         //�Ʒ�ʱ��,����  ����=100,ʱ��=120��ʾ1Ԫ��2Сʱ
}unify_t;

// cost template   �Ʒ�ģ������
typedef struct {
    uint8_t  gunId;
	uint32_t template_id;                       //�Ʒ�ģ��id
    uint8_t  mode;                              //1�������ʶμƷ� 2��ͳһ�շ�
    union {
        multiPower_t powerInfo;
        unify_t unifyInfo;
    }Data;/////
}COST_TEMPLATE_REQ_STR;



typedef struct{
    uint8_t AuthSerialNum;
    uint8_t StartChargeSN;
    uint8_t StopChargingAckSN;
    uint8_t StartChargingAckSN;
    uint8_t StartChargingNoticeSN;
    uint8_t UpgradeReqSN;
    uint8_t FWDownLoadSN;
    uint8_t FWUpgradeAckSN;
    uint8_t CostTemplateAckSN;
    uint8_t RemoteCtrlAckSN;
    uint8_t HeartBatAckSN;
    uint8_t CardAuthResultSN;
    uint8_t EventNoticeSN;
    uint8_t ReqCostTemplateSN;
    uint8_t StopChargingNoticeSN;
    uint8_t OrderReportNoticeSN;
    START_OR_STOP_CHARGING_BTRF2_4G_TYPE isSendChargeInform[GUN_NUM_MAX];   //�Ƿ��Ѿ����ͳ��֪ͨ 0: �Ѿ����� 1: û�з���
    uint8_t AuthFlag[GUN_NUM_MAX];
	uint8_t ScanCodeChargerFlag[GUN_NUM_MAX];
    uint32_t lastSendAuthTime;
	START_CHARGING_STR StartChargingNotice;
}PACK_SERIAL_NUM_TYPE;

//������֪ͨ
typedef struct {
	uint8_t  gun_id;                            //1.
	uint8_t  user_type;                         //2.
    uint8_t  stop_reason;                       //3.����ԭ��
    uint8_t  reasonDetail;                      //4. ����ԭ��ϸ��
    uint32_t errStatus;                         //5. ���������������ֹ����쳣״̬
    uint8_t  fwVer;                             //4.�̼��汾
	uint8_t  user_account[20];                  //5.�û��˺�
	uint8_t  order[ORDER_SECTION_LEN];          //6.������
	uint32_t startTime;                         //
	uint32_t stop_time;                         //7.�������ʱ��
	uint32_t startElec;
	uint32_t stop_power;                        //8.����������
	uint32_t charger_cost;                      //9.��������  ��
	uint32_t template_id;                       //10.����id
	uint16_t chargingPower;                     //11.�����ʶμƷѵĹ��ʣ���λ��w
}STOP_CHARGING_NOTICE_REQ_STR;

typedef struct {
    uint32_t wIndex;
	uint32_t rIndex;
}ORDER_STR;

typedef struct{
    uint8_t  station_id[8];                     // bcd, SN=10, 10 byte  0001123456789
}sys_info1;//1k

typedef struct{
	uint32_t magic_number;
	uint8_t  pcb_sn[8];
}gun_info_str3;	//1k

typedef struct{
    uint32_t fw_version_preserve;                        	// �汾��
	uint8_t  baseGunSn;							//ǹͷ��ʼ���				
    uint8_t  printSwitch;                       //��ӡ���� 0�ر�  1��
    uint8_t card_state;                         //������״̬  0 :��ʾ��ʼ��ok 1: ��ʾ��ʼ������
	char  reserve_blue_name[12];				//����		//��������
	char  btVersion[20];					    //�����汾
    IS_START_CHARGING_BTRF2_4G_TYPE isStartCharge[GUN_NUM_MAX];   //�Ƿ���Ҫ�������
    EVENT_NOTICE_STR_BTRF2_4G NoticeEvent[GUN_NUM_MAX][6];   //0��������ͨ�Ź��ϣ� 1������˿�۶ϣ� 2�����س��ޣ� 3����������Ԥ���� 4���������� 5.��ǹ
	uint16_t chargingFullTime;                  //��������ʱ����ֵ ��ʱ
}system_info_t;	//1k

typedef struct{
	COST_TEMPLATE_REQ_STR cost_template;
	uint8_t  chargerMethod;						//�Ʒѷ�ʽ 1���̶��շ�  2��ʵʱ�շ�  3���𲽽��շ�
	uint16_t chargerStartingGold;				//�𲽽� add
}cost_template_str;	//1k

typedef struct{
	ORDER_STR orderInfo;                            //���涩����Ϣ
}gun_info_str1;	//1k

typedef struct{
	uint8_t _24gMacAddr[MAC_LEN];				//2.4����mac��ַ
	uint8_t Local24gMacAddr[MAC_LEN];			//����2.4mac��ַ
}gun_info_str2;	//1k

typedef struct{
	uint8_t  gun_id;
    uint8_t  gun_state;                 //1�ȴ���ǹ 2�����
    uint8_t  is_load_on;                //0:û��������磬��0:�������״̬ 1:����й���δ���  2:����С��1.5w 3:1.5w<����<40w 4:40w<����
	uint8_t  user_type;
	uint8_t  order[ORDER_SECTION_LEN];
    uint16_t charger_time_plan;         //Ԥ�Ƴ��ʱ�� ����  ����3����
    uint8_t  mode;                      //�Ʒ�ģʽ 1�ֹ���  2���ֹ���
    multiPower_t powerInfo;             //�ֹ��ʶ���Ϣ
    uint8_t  getPowerFlag;              //��⹦�ʶδ���
	uint32_t start_time;
	//uint32_t stop_time;
    uint32_t startElec;                   //���� 
    uint32_t startChargerElec;                   //��ʼ���ʱ�ĵ��� 
    uint16_t money;                     //�Ʒ��ۼ����ѽ�� ��
    uint8_t  startMode;                 //����ģʽ 1ˢ��  2ɨ��
	uint16_t current_usr_money;		    //�����  ��
    uint8_t  stopReason;
    uint8_t  reasonDetail;              //ֹͣ���ԭ��ϸ��
    uint8_t  chargingMode;              //���ģʽ 0�����ܳ��� 1������� 2����ʱ�� 3��������
	uint16_t chargingPower;             //���ڹ��ʶμƷѵĹ��� 0.1w
    uint8_t  powerCheckcnt;             //���ʶ�ȡ����
    uint8_t  powerSemenIndex;           //���ʶ�ָʾ
    uint16_t chargingElec;              //0.01kwh �ۼƳ�����
    time_t beginTime;
    uint16_t realChargingTime;          //ʵ�ʳ��ʱ�䣬��������ʱ�� ����
    //uint8_t  isSync;                    //��������ͬ��״̬ 0-����Ҫͬ�� 1-��һ������ 2--�������� 3--�����ϵ�
	uint8_t  chargerMethod;			    //�Ʒѷ�ʽ 1���̶��շ�  2��ʵʱ�շ�  3���𲽽��շ�
	uint8_t  maxPower;					//������������� 0.1w ����10w��10w
	uint16_t startGoldTime;				//�𲽽�ʱ�� ����
	uint16_t chargerStartingGold;       //�𲽽�� �� @2018-5-25 add

}gun_info_t;	//2k


#pragma pack()

typedef struct {

	osSemaphoreId rxSem;
	FIFO_S_t rxBtBuff;
    FIFO_S_t rx24GBuff;
	uint8_t  msgIndex; 
	
	uint32_t sendTimers ;
	uint32_t recvTimers ;
	
	uint8_t flag ;
}CKB_CTRL_STR;



extern int GetCpuTemp(void);


extern int CheckSum(uint8_t *pData, uint16_t len);
extern void PrintfData(void *pfunc, uint8_t *pdata, int len);
extern int StringToBCD(unsigned char *BCD, const char *str) ;
extern char *BCDToString(char *dest, unsigned char *BCD, int bytes) ;
extern int isArraryEmpty(uint8_t *array,int len);

extern system_info_t system_info;
extern sys_info1 gSystemInfo1;
extern gun_info_str1 gGunInfo1;
extern gun_info_str2 gGunInfo2;
extern gun_info_str3 gGunInfo3;
extern cost_template_str gCostTemplateInfo;
extern PACK_SERIAL_NUM_TYPE SerialNumInfo;
extern uint32_t ErrorCode;
extern gun_info_t gun_info[GUN_NUM_MAX];
extern CHG_INFO_STR gChgInfo;

extern CKB_CTRL_STR gCKBCtrl;



#endif
