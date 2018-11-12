/*update.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "include.h"


#define FLASH_PAGE_SIZE		1024					//ҳ��С

#define BootFlashAdde		(uint32_t)0x00000		//Boot������ڵ�ַ
#define AppFlashAddr		(uint32_t)0x02000		//App������ڵ�ַ(100K)
#define AppUpBkpAddr		(uint32_t)0x1B000		//app�������ݴ洢����ַ(100K)
#define SysConfigBaseAddr	(uint32_t)0x34000		//����������Ϣ(48K)


#define EE_SAVE_ENERGY		(SysConfigBaseAddr)				//����	(1K)
#define SysInfoFlashAddr	(SysConfigBaseAddr + 0x400)		//ϵͳ�����ļ�(1K)
#define GunInfoFlashAddr	(SysConfigBaseAddr + 0x800)		//ǹͷ��Ϣ(1K)


#define OrderFlashAddr		(SysConfigBaseAddr + 0xC00)		//��ʷ����(45K)



//ҳ��С
#define FLASH_PAGE_SIZE		1024

#pragma pack(1)

typedef struct{
	uint16_t updateFlag;
    uint32_t size;
    uint16_t checkSum;
}SYS_UPDATE_INFO_t;

#pragma pack()

//������Ϣͷƫ����
#define UPDATE_HEAD_POS		(sizeof(SYS_UPDATE_INFO_t))

int UpdateFromAppBkp(uint32_t fsize, uint16_t checksum);
int FlashClrSysUpInfo(void);
int FlashWriteSysUpInfo(void);
int FlashReadSysUpInfo(void);




#endif // __UPDATE_H__










