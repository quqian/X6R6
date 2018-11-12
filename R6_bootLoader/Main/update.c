/*update.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "update.h"
#include "flash.h"

unsigned char buffer[FLASH_PAGE_SIZE];
SYS_UPDATE_INFO_t updateInfo;

//******************************************************************
//! \brief  	UpdateFromAppBkp
//! \param  
//! \retval 	
//! \note   	
//******************************************************************
int UpdateFromAppBkp(uint32_t fsize, uint16_t checksum)
{
	unsigned char* p;
	uint32_t i = 0;
	uint8_t  rbyte = 0;
	uint16_t ck = 0;
	uint16_t page,lenth;
	
	lenth = fsize/FLASH_PAGE_SIZE + 1;
	
	FlashClrSysUpInfo();
	
	//���㱸��App����checksum
	for(i=0; i<fsize; i++)
	{
		HT_Flash_ByteRead(&rbyte, AppUpBkpAddr+i, 1);
		ck += rbyte;
	}
	//У����������ϢУ�鲻��ȷ���ʧ��
	if(ck != checksum)
	{
		return -1;
	}
	
	//����App�洢ҳ�沢д������
	for( page=0; page<lenth; page++)
	{
		//����һҳ
		HT_Flash_PageErase(AppFlashAddr + page * FLASH_PAGE_SIZE);

		//��ȡApp������һҳ�����ݵ�buffer
		memcpy(buffer, (uint8_t *)(AppUpBkpAddr + page * FLASH_PAGE_SIZE), FLASH_PAGE_SIZE);
		
		p = buffer;
		
		//д��һҳ������
		for(i=0; i<FLASH_PAGE_SIZE; i++)
		{
			//ÿ��д��4��byte������
			HT_Flash_ByteWrite(p, AppFlashAddr + page * FLASH_PAGE_SIZE + i, 1);
			p+=1;
		}
	}
	
	ck = 0;
	
	//д���������,����App���³�����У��
	for(i=0;i<fsize;i++)
	{
        ck = *(unsigned char*)(AppFlashAddr+i) + ck;
    }
	
	//���У�鲻�������������У��,����ʧ��
    if(ck == checksum)
	{
		//printf("checksum ok, update success!\r\n");
		return 0;
	}
	else
	{
		//printf("checksum fail, update failed!\r\n");
		return -1;
	}  

}

int FlashClrSysUpInfo(void)
{
	HT_Flash_PageErase(SysUpInfoAddr);
	
	return 0;
}


int FlashWriteSysUpInfo(void)
{
	HT_Flash_ByteWrite((uint8_t*)&updateInfo, SysUpInfoAddr, sizeof(updateInfo));
	
	return 0;
}

int FlashReadSysUpInfo(void)
{
	HT_Flash_ByteRead((uint8_t*)&updateInfo, SysUpInfoAddr, sizeof(updateInfo));
	
	return 0;
}























