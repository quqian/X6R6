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
	
	//计算备份App程序checksum
	for(i=0; i<fsize; i++)
	{
		HT_Flash_ByteRead(&rbyte, AppUpBkpAddr+i, 1);
		ck += rbyte;
	}
	//校验与升级信息校验不相等返回失败
	if(ck != checksum)
	{
		return -1;
	}
	
	//擦除App存储页面并写入数据
	for( page=0; page<lenth; page++)
	{
		//擦除一页
		HT_Flash_PageErase(AppFlashAddr + page * FLASH_PAGE_SIZE);

		//读取App备份域一页的数据到buffer
		memcpy(buffer, (uint8_t *)(AppUpBkpAddr + page * FLASH_PAGE_SIZE), FLASH_PAGE_SIZE);
		
		p = buffer;
		
		//写入一页的数据
		for(i=0; i<FLASH_PAGE_SIZE; i++)
		{
			//每次写入4个byte的数据
			HT_Flash_ByteWrite(p, AppFlashAddr + page * FLASH_PAGE_SIZE + i, 1);
			p+=1;
		}
	}
	
	ck = 0;
	
	//写入数据完毕,计算App更新程序后的校验
	for(i=0;i<fsize;i++)
	{
        ck = *(unsigned char*)(AppFlashAddr+i) + ck;
    }
	
	//如果校验不等于升级程序的校验,返回失败
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























