#ifndef __CHARGER_H__
#define __CHARGER_H__

#include "includes.h"
#include "gun.h"
#include "flash_usr.h"


#define  CHARGING_MAX_TIME              (12*60*60)

#define COST_POWER                      1  //�ֹ���
#define COST_UNIFY                      2   //ͳһ�Ʒ�

#define NO_LOAD_TIME                    (30) //����

#define CHARGING_POWER_LIMIT            2200  //��׮��������� w

#define PUT_OUT_GUN_POWER               15  //0.1w
#define PUT_OUT_GUN_TIME                300  //��

enum {
    FIRST_START_CHARGING = 1,
    NET_RECOVER,
    POWER_RECOVER,
};

enum{
    CHARGING_FIXED = 1,                         //�̶��Ʒ�
	CHARGING_ACTUAL,                            //ʵʱ�Ʒ�
	CHARGING_START,                             //�𲽽�
};

enum {
    START_FAIL_FAULT = 1,
    START_FAIL_NO_TEMPLATE,
    START_FAIL_CHARGING,
    START_FAIL_NO_ADJUST,
    START_FAIL_PARA_ERR,
};

enum {
    REASON_UNKNOW = 0,
    REASON_COMM_ERR = 1,                //1.��¼оƬͨ�Ź���
    REASON_RESET_TIME_ENOUGH,           //2.��λʱ��乻
    REASON_MONEY_ENOUGH,                //3.���乻
    REASON_TIME_ENOUGH,                 //4.ʱ��乻
    REASON_OVER_23H,                    //5.����23Сʱ
    REASON_BLUE,                        //6.��������
	REASON_TOTAL_POWER_LARGE,           //7.��׮���ʹ���
	REASON_AVERAGE_POWER_LOW,           //8.ƽ�����ʹ���
	RELAY_ERR,							//9.�̵�������
};

enum {
    STOP_NORMAL = 0,                    //0.���շ������ͣ���������磬��ʱ����磬�������ȣ�����ֹͣ���
    STOP_SWIPE_CARD,                    //1.�û�����ˢ��ֹͣ���
    STOP_INPUT_CODE,                    //2.�û���������У����ֹͣ���
    STOP_PHONE,                         //3.�û�Զ�̽���
    STOP_ADMINI,                        //4.����ԱԶ�̽���
    STOP_EMERG_STOP,                    //5.��ͣ���£�ֹͣ���
    STOP_PULL_OUT,                      //6.��⵽ǹͷ�Ͽ���ֹͣ���
    STOP_SYS_REBOOT,                    //7.ϵͳ����ֹͣ���
    STOP_NET_BREAK,                     //8.���׮���ߣ�ֹͣ���
    STOP_POWER_OFF,                     //9.���׮���磬ֹͣ���
    STOP_CHARGER_FAULT,                 //10.���׮���ϣ�ֹͣ���
    STOP_CAR_FAULT,                     //11.�綯�����ϣ�ֹͣ���
    STOP_OTHER,                         //12.����ԭ��ֹͣ��� δ֪ԭ��
    STOP_METER_FAULT,                   //13.������
    STOP_CARD_FAULT,                    //14.ˢ��������
    STOP_LCD_FAULT,                     //15.LCD����
    STOP_CHARGING_FULL,                 //16.soc ����
    STOP_BALANCE_NOT_ENOUG,             //17.����(�ľ�)
    STOP_OVER_LOAD,                     //18.����ֹͣ
    STOP_OVER_VOLTAGE,                  //19.��ѹֹͣ
    STOP_LOW_VOLTAGE,                   //20.Ƿѹֹͣ
    STOP_POWER_TOO_BIG,                 //21.��׮ʵʱ���ʹ���ֹͣ��磻
    STOP_BY_BLUETOOTH,					//22 �ֻ�����ֹͣ
    STOP_LINKGRAND_FAULT,				//23 �豸�ӵع���
    STOP_TEMPARATURE_HIGH,				//24 �豸�¶ȹ���
};

#define STOP_UNKNOW STOP_OTHER

enum {
	EVENT_CARD_FAULT = 1,               //1. ������ͨ�Ź���
	EVENT_FUSE_BREAK = 2,               // ����˿�۶�
	EVENT_OVER_LOAD = 3,                // ���س���
	EVENT_ENV_TEMP_HIGH = 4,            // ��������Ԥ��
	EVENT_CHIP_FAULT = 5,               //��������
    EVENT_PLUG_PULL_OUT = 6,            //6
    EVENT_SHUT_DOWN,                    //2.
	EVENT_POWER_DROP,                   //3.
	EVENT_NET_BREAK,                    //4.
	EVENT_NET_RECONNECT,                //5.
	EVENT_STOP_CHARGING,                //7
	EVENT_CHARGING_FULL,                //8
	EVENT_START_CHARGING,               //
    EVENT_MULTI_OPEN_FAIL,              //13
    EVENT_CHIP_TEMP_HIGH,               //15
    EVENT_ORDER_RESEND,                 //16
    EVENT_ADJUST_ERR,                   //17
};


extern int GetCostMode(void);
extern int GetChargingGunCnt(void);
extern int CheckStartPara(uint8_t gun_id);
extern void RestoreGunStatus(void);
/*****************************************************************************
** Function name:       BTRF2_4gStartCharging
** Descriptions:        //�������
** input parameters:    gunId: ���ڱ��豸��������ף�����ȡֵ1��2
                        pOrder: ������
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern uint8_t BTRF2_4gStartCharging(uint8_t gunId, uint32_t money, uint8_t *pOrder,uint8_t SerialNum);

/*****************************************************************************
** Function name:       Proto2_4gStopCharging
** Descriptions:        //ֹͣ���
** input parameters:    gunId: ���ڱ��豸��������ף�����ȡֵ1��2
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void Proto2_4gStopCharging(uint8_t gunId);
extern void ChargingCtrlProc(void);
extern void ChargingProc(void);

extern void Charger_Init(void);
/*****************************************************************************
** Function name:       ReadUpgradeInit
** Descriptions:        //������ʼ��
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void ReadUpgradeInit(void);
extern void FuseholderStatusProc(void);
extern void ReadFlashInfoData(PARTITION n, uint8_t *data_info, uint8_t *dataInfo, uint32_t len);

#endif
