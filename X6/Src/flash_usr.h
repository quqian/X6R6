#ifndef __FLASH_USR_H__
#define __FLASH_USR_H__

#include "includes.h"



#define FLASH_PAGE_SIZE						1024					//ҳ��С

#define BootFlashAdde						(uint32_t)0x00000		                //Boot������ڵ�ַ(8K)
#define AppFlashAddr						(uint32_t)(BootFlashAdde + 0x02000)		//App������ڵ�ַ(100K)
#define AppUpBkpAddr						(uint32_t)(AppFlashAddr + 0x19000)		//app�������ݴ洢����ַ(100K  ͷ+�̼�)
#define SysConfigBaseAddr					(uint32_t)(AppUpBkpAddr + 0x19000)		//����������Ϣ(48K)

#define SysInfoFlashAddr					(SysConfigBaseAddr)					//ϵͳ�����ļ�(1K)
#define SysInfoFlashAddr1					(SysInfoFlashAddr + 0x400)			//ϵͳ����1�ļ�(1K)
#define GunInfoFlashAddr					(SysInfoFlashAddr1 + 0x400)    		//ǹͷ��Ϣ(1K)
#define GunInfoFlashAddr3					(GunInfoFlashAddr + 0x400)    		//ǹͷ��Ϣ(1K)
#define GunInfoFlashAddr1					(GunInfoFlashAddr3 + 0x400)    		//ǹͷ��Ϣ1(1K)
#define GunInfoFlashAddr2					(GunInfoFlashAddr1 + 0x400)    		//ǹͷ��Ϣ2(1K)
#define EnergyFlashAddr	    				(GunInfoFlashAddr2 + 0x400)			//����(1K)
#define OrderFlashAddr						(EnergyFlashAddr + 0x400)			//��ʷ����(30K)
#define CostTemplateFlashAddr				(OrderFlashAddr + 0x7800)			//�Ʒ�ģ��(1K)
#define UpgradeFlashAddr					(CostTemplateFlashAddr + 0x400)   	//������Ϣ��(1K)
#define OtherFlashAddr						(UpgradeFlashAddr + 0x400)			//����(9K)


typedef enum{
	FIRMWARE_PART = 0,
	SYSCONF_PART,
	SYSCONF_PART1,
	GUNINFO_PART,
	GUNINFO_PART3,
	GUNINFO_PART1,
	GUNINFO_PART2,
	ENERGY_PART,
	ORDER_PART,
	COST_TEMPLATE_PART,
	UPGRADE_PART,
	OTHER_PART,
	PART_NUM,
}PARTITION;


typedef struct {
    uint32_t s_base;  		//start addr
    uint32_t s_count; 		//section count
	uint32_t pos_w;			//дƫ����
	uint32_t pos_r;			//��ƫ����
}FLASH_PART;

extern FLASH_PART flash_partition[PART_NUM];

void Flash_Init(void);
void Flash_Erase(PARTITION n);
void Flash_EraseALL(PARTITION n);
int Flash_Read(PARTITION n,void* buff, int len);
int Flash_Write(PARTITION n,void* buff, int len);
void Flash_ErasePage(PARTITION n,uint16_t page);
void Flash_SetPos_r(PARTITION n,uint32_t pos);
void Flash_SetPos_w(PARTITION n,uint32_t pos);
extern int FlashWriteData(PARTITION n,void* buff, int len);
extern void FlashWriteGunInfoData(void);
extern void FlashWriteSystemInfoData(void);
extern void FlashWriteDataToSystemInfo1(void);
extern void FlashWriteGunInfo_1_Data(void);
extern void FlashWriteGunInfo_2_Data(void);
extern void FlashWriteGunInfo_3_Data(void);
extern void FlashWriteCostTemplateData(void);

/*****************************************************************************
** Function name:       FlashWritePage
** Descriptions:        
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void FlashWritePage(PARTITION n,void* buff, int len);
/*****************************************************************************
** Function name:       WritePageDataToFlash
** Descriptions:        page��д���ڼ�ҳ
                        buffer��Ҫд�������׵�ַ
** input parameters:    None
** output parameters:   None
** Returned value:	    None
** Author:              quqian
*****************************************************************************/
extern void WritePageDataToFlash(PARTITION n, uint32_t page, uint8_t *buffer);

#endif
