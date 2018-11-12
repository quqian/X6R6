/*update.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "include.h"


//flash���򻮷�: boot:6k app0:83k app1:83k app2:81k ϵͳ��Ϣ1:1k ϵͳ��Ϣ2:1k ������Ϣ:1k
#define AppFlashAddr	    (uint32_t)0x02000               //App������ڵ�ַ(83k=81920=0x14000)
#define AppUpBkpAddr	    (uint32_t)0x16800               //app�������ݴ洢����ַ(83k)
#define TERMINAL_APP_ADDR   (uint32_t)0x2B000               //�ն��豸app�洢����ַ(81k=0x13800)
#define SysCfgInfoAddr	    (uint32_t)0x3F400               //ϵͳ������Ϣ�洢����ַ  1k
#define SysCfg1InfoAddr	    (uint32_t)0x3F800               //ϵͳ������Ϣ1S�洢����ַ  1k
#define SysUpInfoAddr	    (uint32_t)0x3FC00               //����������Ϣ(1K)


#define FLASH_PAGE_SIZE		1024
//#define APP_FW_SIZE     	(AppUpBkpAddr - AppFlashAddr)


typedef struct{
	uint16_t updateFlag;
	uint16_t checkSum;
	uint32_t fsize;
}SYS_UPDATE_INFO_t;




int UpdateFromAppBkp(uint32_t fsize, uint16_t checksum);
int FlashClrSysUpInfo(void);
int FlashWriteSysUpInfo(void);
int FlashReadSysUpInfo(void);




#endif // __UPDATE_H__










