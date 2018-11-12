
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
006.7   1. 修改R6给X6升级的逻辑错误

006.6   1. 修改X6升级版本号与X6本身版本号相同而一直给X6发送升级版本的bug
		2. 修改R6给X6升级的逻辑错误

006.5   1. 修改R6不给X6升级的bug
		2. 计费模板增加起步金计费
		3. R6采用轮询的方式，每间隔5秒向其绑定的X6逐个发送心跳数据包

006.4   1. 修改R6不给X6升级的bug

006.3   1. 修改R6给X6升级由同时升级多个改为升级完一个再升级下一个

006.2   修改R6使用4g模块升级X6的bin文件后不能给X6升级的bug

006.1   修改R6升级死机的问题

006.0   修改卡片余额一千，语音播报3百多的问题
005.9   增加DFI工具绑定完成后复位蓝牙模块
005.8   Recv24gData没有节点不重启模块 BindRelation增加绑定关系回应答 OpenNet修改异常复位问题
005.7   Recv24gData重启检测时间调整到5分钟 RecvX6StopChargingNotice修改应答报文的序号 完善设置自动接听模式功能
005.6   心跳应答增加固件版本字段
005.5   系统时间戳修复 兼容3610模块 SetListenMode设置自动接听
005.4   RecvX6AuthReqProc/RecvX6StartChargingNotice报文序号超时自动更新
005.3   SendHeartBeatFun修复枪头个数错误问题 RecvX6StartChargingNotice先发2.4g应答再发充电启动通知
005.2   增加开启/停止充电请求重发机制 Blue24GNetProc修改不能带多台x6问题 ReqStartCharging修改请求开启充电应答报文序号错误问题
005     鉴权命令分拆为2条加上重传机制 完善2g/4g模块固件升级功能 X6AuthResult重发间隔设置为6秒 SendProtoPkt升级的时候不发送网络报文 flash分区调整app80k，终端78k
004.2   增加打印节点状态 根据485组网方式修改协议和通信机制
004     2.4g修改为点对点发送 增加测试功能宏 修改sn/id报文接收处理目的mac错误问题
003     增加支持3630模块
002     只支持蓝牙2.4G组网
*/


#if 0
	//#define NET_SERVER_IP                   "172.16.0.228"     //新开发环境
	#define NET_SERVER_IP                   "47.97.238.64"     //新开发环境
	#define NET_SERVER_PORT                 10010
#elif 1
	#define NET_SERVER_IP                   "47.97.224.240"     //正式环境
	#define NET_SERVER_PORT                 10010
#elif 0
	#define NET_SERVER_IP                   "47.97.236.187"		//预发布环境
	#define NET_SERVER_PORT                 10010
#else
	#define NET_SERVER_IP                   "test.access.chargerlink.com"	//测试环境
	#define NET_SERVER_PORT                 10010
#endif
#define FW_VERSION                      8
#define SUB_FW_VERSION                  9




#define USE_ME3610_4G_NET_MODE			(0)
#define USE_ME3612_4G_NET_MODE			(0)


#define MAGIC_NUM_BASE                  0x123456AB
#define STATION_MACHINE_TYPE            "R6"
#define CHARGER_TYPE                    3  //0:无效；1:X9；2:X10；3:R6；4:X6P；5:X6L；6:R8
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
#define CHARGER_SN_LEN			        8   //bcd的长度
#define EVEN_DISCRI_LEN                 32
#define MAC_LEN                         5

#define setbit(x,y) x|=(1<<y)           //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y)          //将X的第Y位清0
#define revbit(x,y) x^=(1<<y)           //将X的第Y位反转


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


