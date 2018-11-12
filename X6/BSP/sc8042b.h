#ifndef __SC8042B_H__
#define __SC8042B_H__

#include "includes.h"

#define AU_POWER_EN()   HT_GPIO_BitsSet(HT_GPIOA,GPIO_Pin_5)
#define AU_POWER_DIS()  HT_GPIO_BitsReset(HT_GPIOA,GPIO_Pin_5)

#define AU_DATA_HIHG()  HT_GPIO_BitsReset(HT_GPIOA,GPIO_Pin_8)
#define AU_DATA_LOW()   HT_GPIO_BitsSet(HT_GPIOA,GPIO_Pin_8)

#define AU_RST_EN()     HT_GPIO_BitsReset(HT_GPIOD,GPIO_Pin_2)
#define AU_RST_DIS()    HT_GPIO_BitsSet(HT_GPIOD,GPIO_Pin_2)

#define READ_AU_BUSY()  HT_GPIO_BitsRead(HT_GPIOD,GPIO_Pin_3)


#define Sc8042bSpeech   PlayVoice


typedef enum{
	VOIC_NULL1=1,							//��
	VOIC_WELCOME=2,							//��ӭʹ��
	VOIC_NULL2=3,							//��
	VOIC_SHARE_CHARGE=4,					//������
	VOIC_START_UPGRADE=5,                   //��ʼ����
	VOIC_READING_CARD=6,               		//���ڶ���
	VOIC_DEVICE_EXCEPTION=7,				//�豸����
	VOIC_CARD_BANLANCE=8,					//��Ƭ���
	VOIC_CARD_RECHARGER=9,					//������,���ֵ
	VOIC_CARD_INVALID=10,					//��Ƭ��Ч
	VOIC_CARD_CHARGING=11,					//��İ������ڳ��
	VOIC_INPUT_SOCKET_NUM=12,				//������������,��ȷ�ϼ�
	VOIC_INPUT_CHARGRE_MONEY=13,			//����������,��ȷ�ϼ�.���践����һ��,�밴���ؼ�
	VOIC_SOCKET_OCCUPID=14,					//������ռ��,��ѡ����������
	VOIC_SOCKET_ERROR=15,					//��������,��ѡ����������
	VOIC_SOCKET_NUM_INVALID=16,				//���������Ч,����������
	VOIC_NIN_YI_XUANZE=17,					//����ѡ��
	VOIC_HAO_CHAZUO=18,						//�Ų���
	VOIC_CHARGER_MONEY=19,					//�����
	VOIC_YUAN=20,							//Ԫ
	VOIC_VERIFIED_PLUG=21,					//��ȷ�ϳ������ͳ��˲������Ѳ��
	VOIC_CARD_BANLANCE_INSUFFICIENT=22,		//����,����������
	VOIC_START_CHARGING=23,					//��ʼ���
	VOIC_STOP_CHARGER_TIP=24,				//����������,�ɰε���ͷ
	VOIC_PLUG_IN_PLUG=25,					//�뽫��ͷ����
	VOIC_CARD=26,                          	//�֣�ˢ������
	VOIC_KEY=27,                           	//�⣨���Ҽ���������
	VOIC_BLUETOOTH_ONLINE=28,				//����������
	VOIC_BLUETOOTH_OFFLINE=29,				//�����ѶϿ�
	VOIC_ERROR=30,							//����
	VOIC_0=31,                             	//0 (����0)
    VOIC_1=32,                             	//1.
    VOIC_2=33,                             	//2.
    VOIC_3=34,                             	//3.
    VOIC_4=35,                             	//4.
    VOIC_5=36,                             	//5.
    VOIC_6=37,                             	//6.
    VOIC_7=38,                            	//7.
    VOIC_8=39,                             	//8.
    VOIC_9=40,                             	//9
	VOIC_TEST_TIP=41,						//1��׮��,2��ʶ����,3�̵�������
	VOIC_INPUT_SOCKET_TEST=42,				//������Ű�ȷ������
	VOIC_HUNDRED=43,						//��
	VOIC_TEN=44,							//ʮ
	VOIC_YUAN1=45,							//Ԫ
	VOIC_POINT=46,							//��
	VOIC_DEVICE_REBOOT=47,					//�豸����
	VOIC_SUCCESS=48,						//�ɹ�
	VOIC_A=49,								//a
	VOIC_B=50,								//b
	VOIC_C=51,								//c
	VOIC_D=52,								//d
	VOIC_E=53,								//e
	VOIC_F=54,								//f
	VOIC_TEM=55,							//�¶�
	VOIC_DEGREE=56,							//��
	VOIC_THOUSAND=57,						//ǧ
	VOIC_START_CHARGER_FAIL=58,				//�������ʧ��
	VOIC_POWER_TOO_LARGE=59,                //�豸���ʹ����޷����
	VOICE_NUM,
}SC8042B_VOICE_TYPE;



enum
{    
    ERROR_CODE_VOLATAGE_HIGH    = 21,       //�豸������ѹ
    ERROR_CODE_VOLATAGE_LOW     = 22,       //�豸Ƿѹ
    ERROR_CODE_CURRENT_HIGH     = 23,       //�豸����
    ERROR_CODE_EMU_CONNECT      = 24,       //����оƬͨѶ����
    ERROR_CODE_TEMP_HIGH        = 25,       //�豸�¶ȹ���
    ERROR_CODE_EEPROM           = 26,       //EEPROM����
    ERROR_CODE_FLASH            = 27,       //Flash��д����
    ERROR_CODE_GROUND           = 28,       //�豸�ӵش�����
    
    ERROR_CODE_CARD_HEART       = 40,       //��ʱ��δ�յ��������������
    
    ERROR_CODE_2G_SEND_LENGTH   = 50,       //�豸GPRS�������ݳ��ȵ�ʱ�����
    ERROR_CODE_2G_SEND_FAILED   = 51,       //GPRS��������ʧ��
    ERROR_CODE_OTA_FILE_SIZE    = 52,       //OTA�����ļ���С����
    ERROR_CODE_OTA_FILE_ERROR   = 53,       //OTA������ȡ�ļ����󣬻��������쳣
    ERROR_CODE_OTA_GET_FAILED   = 54,       //OTA����Get����ʧ��
    ERROR_CODE_OTA_CHECK_FAILED = 55,       //FTP�������������ݺ󣬼���У���ʧ��
    ERROR_CODE_OTA_FILE_SIZE_ERROR = 56,    //FTP�������������ݺ�������ݴ�Сʧ��
    ERROR_CODE_OTA_GET_FWCNT    = 57,       //FTP����Get���ݵĵ�һ֡16���ֽ�����ʧ��,��ȡ�̼�����ʧ��
    ERROR_CODE_OTA_GET_FWHEAD   = 58,       //FTP����Get���ݵĵ�һ֡16���ֽ���������ͷ����,�̼�ͷ����
    ERROR_CODE_OTA_GET_FW_NAME  = 59,       //FTP����Get���ݵĵڶ�֡16�ֽ������жϹ̼�����ʧ��
    ERROR_CODE_OTA_FAILED       = 60,       //FTP����ʧ��
    ERROR_CODE_OTA_FLASH_CHECKSUM = 61,     //FTP����,д��flash�Ͷ����ļ���ʹ���
    ERROR_CODE_OTA_FLASH        = 62,        //FTPд��̼���flash����
    
    ERROR_CODE_REGISTER         = 100,      //���յ��������ظ���ע����Ϣ��ע��ʧ��
    ERROR_CODE_LOGIN            = 101,      //���յ��������ظ���Ϣ����¼ʧ��
    ERROR_CODE_START_GUN_NUM    = 102,      //��⵽�����е�ǹͷ�Ų��ڵ�ǰ�豸��ǹͷ��Χ��
    ERROR_CODE_START_GUN_CGARGING= 103,     //��⵱ǰǹ���ڳ��״̬
    ERROR_CODE_START_NO_COST    = 104,      //��⵱ǰ�豸û�мƷ�ģ��
    ERROR_CODE_STOP_GUN_NUM     = 106,      //���շ�������Զ�̹ر�ָ��ʱ��������ǹͷ�Ų��ڵ�ǰ�豸��ǹͷ��Χ��
    ERROR_CODE_STOP_ACK_GUN_NUM = 107,      //������������ָ֪ͨ����շ�������ACK�������е�ǹ�Ķ˿ڲ���1����12֮��
    ERROR_CODE_MESSAGE_UNKNONE  = 108,      //���յ�����������Ϣ��������Ϣ����δ֪
    ERROR_CODE_MESSAGE_IDOCDE   = 109,      //���յ�����������Ϣ���豸ע��ʱ�������ظ���Ϣ�е�׮�Ų����豸��׮�Ż����豸��ʶ�������
    ERROR_CODE_MESSAGE_TYPE     = 110,      //���յ�����������Ϣ����Ϣ�����Ͳ����豸����Ϣ
    ERROR_CODE_HEART_BEAT       = 111,      //��ʱ��δ�յ��������������ظ�
    ERROR_CODE_HEART_BEAT_GUN   = 112,      //��װ�����������ݣ����ǹͷ״̬����
    ERROR_CODE_SOCKET           = 120,      //��ѯ��ǰsocket������״̬��socket�����쳣
	ERROR_CODE_REALY			= 121,		//�̵�������
};

void SC8042B_Init(void);
void OptFailNotice(int code);
void OptSuccessNotice(int code);
void TempNotice(uint16_t temp);
void SpeechChargeing(uint8_t id);
int PlayVoice(unsigned char cnt);
void SpeechCardBalance(uint32_t balance);


#endif


