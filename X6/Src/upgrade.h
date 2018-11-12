#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#include "includes.h"
#include "flash_usr.h"


#pragma pack(1)

typedef struct{
	uint16_t updateFlag;
    uint32_t size;
    uint16_t checkSum;
}SYS_UPDATE_INFO_t;


typedef struct{
	uint32_t size;			
	uint16_t checkSum;      //ȫ�����ݵ�У���
	uint16_t package_num;   //�ܵ����ݰ�����
	uint8_t fw_version;
	uint16_t current_package; 	//��ǰ���չ̼����ݰ�
	uint8_t lastIndex;
	char name[10];
}FW_INFO_STR;

typedef struct{
	uint32_t size;			
	uint16_t checkSum;      //ȫ�����ݵ�У���
	uint8_t name[10];
}FW_INFOR_STR;

typedef struct{
    uint32_t AddressOffset;     //������ַƫ��
	uint8_t fw_version;         //�̼��汾
	uint16_t current_package; 	//��ǰ���չ̼����ݰ�
}UPGRADE_RECORD_STR;

#pragma pack()

//������Ϣͷƫ����
#define UPDATE_HEAD_POS		(sizeof(SYS_UPDATE_INFO_t))
extern UPGRADE_RECORD_STR UpgradeRecord;
extern FW_INFO_STR fw_info;

extern void Upgrade_Init(uint32_t fw_size,uint16_t checkSum,uint32_t package_num,uint8_t fw_version);
uint8_t Upgrade_Write(uint8_t *pData,uint16_t len);
int GetUpgradeCurrentPackage(void);
/*****************************************************************************
** Function name:       COM_Upgrade_Write
** Descriptions:        
** input parameters:    pMsg: �ڲ�Э������ͷָ��
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern uint8_t COM_Upgrade_Write(uint8_t *pData,uint16_t len);
extern void UpgradeFlashErasePage(void);


#endif
