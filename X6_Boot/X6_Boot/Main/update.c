/*update.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "update.h"
#include "flash.h"

unsigned char buffer[FLASH_PAGE_SIZE];
SYS_UPDATE_INFO_t updateInfo;

uint8_t Upgrade_FlashCheck(void)
{
	uint16_t checksum = 0;
	uint8_t  rbyte = 0;
	for(int i = 0;i<updateInfo.size;i++){
		HT_Flash_ByteRead(&rbyte,AppUpBkpAddr+UPDATE_HEAD_POS+i,1);
		checksum += rbyte;
	}
	
	if(checksum != updateInfo.checkSum)  return 1;
	
	return 0;
}


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
	uint16_t ck = 0;
	uint16_t page,lenth;
	uint8_t bytes;
	
	lenth = fsize/FLASH_PAGE_SIZE + 1;
		
	//���㱸��App����checksum
	if(Upgrade_FlashCheck()!=0){
		return -1;
	}
	
	//����App�洢ҳ�沢д������
	for( page=0; page<lenth; page++)
	{
		//����һҳ
		HT_Flash_PageErase(AppFlashAddr + page * FLASH_PAGE_SIZE);

		//��ȡApp������һҳ�����ݵ�buffer
		memcpy(buffer, (uint8_t *)(AppUpBkpAddr + UPDATE_HEAD_POS + page * FLASH_PAGE_SIZE), FLASH_PAGE_SIZE);
		
		p = buffer;
		
		//д��һҳ������
		for(i=0; i<FLASH_PAGE_SIZE; i++)
		{
			//ÿ��д��1��byte������
			while(1)
			{
				HT_Flash_ByteWrite(p, AppFlashAddr + page * FLASH_PAGE_SIZE + i, 1);
				HT_Flash_ByteRead(&bytes,AppFlashAddr + page * FLASH_PAGE_SIZE + i,1);
				if(bytes == *p)
				{
					break;
				}
			}
			p+=1;
		}
	}
	
	ck = 0;
	
	//д���������,����App���³�����У��
	for(i=0;i<fsize;i++){
        ck= *(unsigned char*)(AppFlashAddr+i) + ck;
    }
	
	//���У�鲻�������������У��,����ʧ��
    if(ck == checksum){
		//printf("checksum ok, update success!\r\n");
		HT_Flash_PageErase(AppUpBkpAddr);
		return 0;
	}else{
		//printf("checksum fail, update failed!\r\n");
		return -1;
	}  

}























