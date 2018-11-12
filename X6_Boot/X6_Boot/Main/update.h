/*update.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "include.h"


#define FLASH_PAGE_SIZE		1024					//页大小

#define BootFlashAdde		(uint32_t)0x00000		//Boot运行入口地址
#define AppFlashAddr		(uint32_t)0x02000		//App运行入口地址(100K)
#define AppUpBkpAddr		(uint32_t)0x1B000		//app缓存数据存储基地址(100K)
#define SysConfigBaseAddr	(uint32_t)0x34000		//其他配置信息(48K)


#define EE_SAVE_ENERGY		(SysConfigBaseAddr)				//电量	(1K)
#define SysInfoFlashAddr	(SysConfigBaseAddr + 0x400)		//系统配置文件(1K)
#define GunInfoFlashAddr	(SysConfigBaseAddr + 0x800)		//枪头信息(1K)


#define OrderFlashAddr		(SysConfigBaseAddr + 0xC00)		//历史订单(45K)



//页大小
#define FLASH_PAGE_SIZE		1024

#pragma pack(1)

typedef struct{
	uint16_t updateFlag;
    uint32_t size;
    uint16_t checkSum;
}SYS_UPDATE_INFO_t;

#pragma pack()

//升级消息头偏移量
#define UPDATE_HEAD_POS		(sizeof(SYS_UPDATE_INFO_t))

int UpdateFromAppBkp(uint32_t fsize, uint16_t checksum);
int FlashClrSysUpInfo(void);
int FlashWriteSysUpInfo(void);
int FlashReadSysUpInfo(void);




#endif // __UPDATE_H__










