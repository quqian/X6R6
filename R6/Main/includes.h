
#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "systick.h"
#include "FreeRTOS.h"
#include "ht6xxx.h"
#include "CpuCfg.h"
#include "cmu.h"
#include "task.h"
#include "queue.h"



/*
006.7   1. �޸�R6��X6�������߼�����

006.6   1. �޸�X6�����汾����X6����汾����ͬ��һֱ��X6���������汾��bug
		2. �޸�R6��X6�������߼�����

006.5   1. �޸�R6����X6������bug
		2. �Ʒ�ģ�������𲽽�Ʒ�
		3. R6������ѯ�ķ�ʽ��ÿ���5������󶨵�X6��������������ݰ�

006.4   1. �޸�R6����X6������bug

006.3   1. �޸�R6��X6������ͬʱ���������Ϊ������һ����������һ��

006.2   �޸�R6ʹ��4gģ������X6��bin�ļ����ܸ�X6������bug

006.1   �޸�R6��������������

006.0   �޸Ŀ�Ƭ���һǧ����������3�ٶ������
005.9   ����DFI���߰���ɺ�λ����ģ��
005.8   Recv24gDataû�нڵ㲻����ģ�� BindRelation���Ӱ󶨹�ϵ��Ӧ�� OpenNet�޸��쳣��λ����
005.7   Recv24gData�������ʱ�������5���� RecvX6StopChargingNotice�޸�Ӧ���ĵ���� ���������Զ�����ģʽ����
005.6   ����Ӧ�����ӹ̼��汾�ֶ�
005.5   ϵͳʱ����޸� ����3610ģ�� SetListenMode�����Զ�����
005.4   RecvX6AuthReqProc/RecvX6StartChargingNotice������ų�ʱ�Զ�����
005.3   SendHeartBeatFun�޸�ǹͷ������������ RecvX6StartChargingNotice�ȷ�2.4gӦ���ٷ��������֪ͨ
005.2   ���ӿ���/ֹͣ��������ط����� Blue24GNetProc�޸Ĳ��ܴ���̨x6���� ReqStartCharging�޸����������Ӧ������Ŵ�������
005     ��Ȩ����ֲ�Ϊ2�������ش����� ����2g/4gģ��̼��������� X6AuthResult�ط��������Ϊ6�� SendProtoPkt������ʱ�򲻷������籨�� flash��������app80k���ն�78k
004.2   ���Ӵ�ӡ�ڵ�״̬ ����485������ʽ�޸�Э���ͨ�Ż���
004     2.4g�޸�Ϊ��Ե㷢�� ���Ӳ��Թ��ܺ� �޸�sn/id���Ľ��մ���Ŀ��mac��������
003     ����֧��3630ģ��
002     ֻ֧������2.4G����
*/


#if 0
	//#define NET_SERVER_IP                   "172.16.0.228"     //�¿�������
	#define NET_SERVER_IP                   "47.97.238.64"     //�¿�������
	#define NET_SERVER_PORT                 10010
#elif 1
	#define NET_SERVER_IP                   "47.97.224.240"     //��ʽ����
	#define NET_SERVER_PORT                 10010
#elif 0
	#define NET_SERVER_IP                   "47.97.236.187"		//Ԥ��������
	#define NET_SERVER_PORT                 10010
#else
	#define NET_SERVER_IP                   "test.access.chargerlink.com"	//���Ի���
	#define NET_SERVER_PORT                 10010
#endif
#define FW_VERSION                      8
#define SUB_FW_VERSION                  9




#define USE_ME3610_4G_NET_MODE			(0)
#define USE_ME3612_4G_NET_MODE			(0)


#define MAGIC_NUM_BASE                  0x123456AB
#define STATION_MACHINE_TYPE            "R6"
#define CHARGER_TYPE                    3  //0:��Ч��1:X9��2:X10��3:R6��4:X6P��5:X6L��6:R8
#define CHARGER_NAME                    "R6"
#define REGISTER_CODE					"572059137094"
#define ORDER_SECTION_LEN               10
#define OUT_NET_PKT_LEN                 256
#define MAX_TERMINAL_NUM                12
#define COST_TEMP_CNT                   6
#define IN_QUEUE_NET_PKT_LEN                 100

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int  uint32_t;
typedef unsigned long long uint64_t;      /* Unsigned 64 bit quantity */

#define TX_FAIL_MAX_CNT                 4
#define BUFFER_SIZE                     520
#define ICCID_LEN                       20

#define THIS_LINE                       __LINE__
#define CL_LINE                         THIS_LINE
#define CL_FUNC                         __func__
#define CL_OK                           0
#define CL_FAIL                         (-1)
#define CL_TRUE                         1
#define CL_FALSE                        0

#define	NOP()				            __NOP()
#define osGetTimes()                    (xTaskGetTickCount()*portTICK_PERIOD_MS)
#define OS_DELAY_MS                     vTaskDelay
#define GUN_NUM_MAX				        16
#define CHARGER_SN_LEN			        8   //bcd�ĳ���
#define EVEN_DISCRI_LEN                 32
#define MAC_LEN                         5

#define setbit(x,y) x|=(1<<y)           //��X�ĵ�Yλ��1
#define clrbit(x,y) x&=~(1<<y)          //��X�ĵ�Yλ��0
#define revbit(x,y) x^=(1<<y)           //��X�ĵ�Yλ��ת


// socket rx state-machine
#define _SOCKET_RX_STATE_INIT           0
#define _SOCKET_RX_STATE_WAIT           1
#define _SOCKET_RX_STATE_RECV_DATA      2
#define _SOCKET_RX_STATE_FINISH         3
#define _SOCKET_RX_STATE_CLOSE          4

enum{
    CHARGER_TYPE_X9 = 1,                //1.
    CHARGER_TYPE_X10,                   //2.
    CHARGER_TYPE_R6,                    //3.
    CHARGER_TYPE_X6P,                   //4.
    CHARGER_TYPE_X6L,                   //5.
    CHARGER_TYPE_R8,                    //6.
    CHARGER_TYPE_R9,                    //7.
    CHARGER_TYPE_X6,                    //8.
};

enum{
    NET_STATE_SOCKET0_OPEN=0,
    NET_STATE_READY,
    NET_STATE_FAILURE,
    NET_NULL,
};

enum{
    LOCAL_NET = 1,
	OUT_485_NET,
	BLUE_NET,
};


#pragma pack(1)
typedef struct{
    uint32_t size;
    uint16_t checkSum;
    uint8_t  name[10];
}FW_INFO_STR;
#pragma pack()


#include "gpio.h"
#include "FIFO.h"
#include "semphr.h"
#include "FIFO.h"
#include "server.h"
#include "flash.h"
#include "sim800c.h"
#include "usart.h"
#include "rtc.h"
#include "ntcad.h"
#include "spi.h"
#include "spi_w25qx.h"



char* GetCurrentTime(void);
#define CL_LOG(fmt,args...) do {    \
    if (system_info.printfSwitch) {    \
        printf("R6 %s %s:(%d) "fmt,GetCurrentTime(), __func__, __LINE__, ##args); \
    }   \
}while(0)

#endif //__INCLUDE_H__


