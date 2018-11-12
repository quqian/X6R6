/*CpuCfg.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __CPUCFG_H__
#define __CPUCFG_H__

#include "includes.h"

#define EnWr_WPREG()		HT_CMU->WPREG = 0xA55A			//则关闭写保护功能，用户可以写操作被保护的寄存器
#define DisWr_WPREG()		HT_CMU->WPREG = 0x0000			//则开启写保护功能，用户禁止写操作被保护的寄存器

#define Debug_En_WDT()		HT_CMU->CLKCTRL1|=0x4000		//调试模式下使能看门狗功能
#define Debug_Dis_WDT()		HT_CMU->CLKCTRL1&=~0x4000		//调试模式下关闭看门狗功能


#define	Feed_WDT64mS()		HT_WDT->WDTCLR = 0xAA00		    //FeedWDT per 64ms
#define	Feed_WDT()			HT_WDT->WDTCLR = 0xAA7F		    //FeedWDT per 8s: 64ms*(1+SET[0..7])
//#define	Feed_WDT()			HT_WDT->WDTCLR = 0xAA3F		//FeedWDT per 4s: 64ms*(1+SET[0..7])
#define	Feed_WDT16S()		HT_WDT->WDTCLR = 0xAAFF		    //FeedWDT per 16s



//flash区域划分: boot:6k app0:83k app1:83k app2:81k 系统信息1:1k 系统信息2:1k 升级信息:1k
#define AppFlashAddr	    (uint32_t)0x02000               //App运行入口地址(83k=81920=0x14000)
#define AppUpBkpAddr	    (uint32_t)0x16800               //app缓存数据存储基地址(83k)
#define TERMINAL_APP_ADDR   (uint32_t)0x2B000               //终端设备app存储基地址(81k=0x13800)
#define SysCfgInfoAddr	    (uint32_t)0x3F400               //系统配置信息存储基地址  1k
#define SysCfg1InfoAddr	    (uint32_t)0x3F800               //系统配置信息1S存储基地址  1k
#define SysUpInfoAddr	    (uint32_t)0x3FC00               //升级请求消息(1K)



#define FLASH_PAGE_SIZE		1024
#define APP_FW_SIZE     	(AppUpBkpAddr - AppFlashAddr)


typedef struct{
	uint16_t updateFlag;
	uint16_t checkSum;
	uint32_t fsize;
}SYS_UPDATE_INFO_t;



void delay_ms(uint16_t ms);
void delay_us(uint32_t us);
int BootSetReqInfo(void);
int BootClrReqInfo(void);

extern SYS_UPDATE_INFO_t updateInfo;

#endif //__CPUCFG_H__



