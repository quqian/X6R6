#include "upgrade.h"
#include "flash_usr.h"
#include "sc8042b.h"
#include "system_info.h"
#include "charger.h"

FW_INFO_STR fw_info;
UPGRADE_RECORD_STR UpgradeRecord;

void UpgradeStart(void)
{
	PlayVoice(VOIC_START_UPGRADE);

    return;
}

#if 1
void UpgradeFlashErasePage(void)
{
	uint32_t mPage = 0;
    uint32_t currentPageLenth = 0;
	uint32_t i = 0;
    
    mPage = (UpgradeRecord.AddressOffset / 1024);       //����д�˶���ҳ
    currentPageLenth = UpgradeRecord.AddressOffset % 1024;       //
    CL_LOG("д�˵�ҳ��Ϊ mPage =%d, ����һҳ�ĳ��� currentPageLenth = %d ! \n", mPage, currentPageLenth);
    if(mPage >= flash_partition[FIRMWARE_PART].s_count)
    {
        CL_LOG("flash����flash����flash����flash����");
        fw_info.current_package = 1;
        UpgradeRecord.current_package = 1;
    }
    Flash_SetPos_r(FIRMWARE_PART, mPage * FLASH_PAGE_SIZE);    //��ҳ�׵�ַΪƫ��, ��ȡû��д�����һҳ��Ȼ���ٲ�����ҳ����������д��ȥ

    MuxSempTake(&gChgInfo.gProtoMuxType.gBuff1024Byte);
	{
	    Flash_Read(FIRMWARE_PART, gFlashCacheBuff, currentPageLenth);  //����Ҫ����ҳ
		//ReadFlashInfoData(FIRMWARE_PART, (uint8_t*)gFlashCacheBuff, (uint8_t*)gFlashCacheBuff , currentPageLenth);  	//����Ҫ����ҳ

	    while(1)
	  	{
			for(i = 0; i < (flash_partition[FIRMWARE_PART].s_count - mPage); i++)
			{	//�����̼�������
				//HT_Flash_PageErase(FIRMWARE_PART + (mPage + i)*FLASH_PAGE_SIZE);
				Flash_ErasePage(FIRMWARE_PART, (mPage + i));
			}
			Flash_SetPos_w(FIRMWARE_PART, mPage * FLASH_PAGE_SIZE);    //��������ҳ, ҲҪ���׵�ַΪƫ����д��ȥ
			if(0 == Flash_Write(FIRMWARE_PART,&gFlashCacheBuff[0], currentPageLenth))	//��Ҫд��ҳ, ���滹û������
			{
				break;
			}
			else
			{
				Feed_WDT();
				CL_LOG("flash[%d]д����! \n", FIRMWARE_PART);
				//osDelay(100);
			}
		}
	}
    MuxSempGive(&gChgInfo.gProtoMuxType.gBuff1024Byte);
}
#endif

void Upgrade_Init(uint32_t fw_size,uint16_t checkSum,uint32_t package_num,uint8_t fw_version)
{
	memset(&fw_info,0,sizeof(fw_info));
    
	fw_info.size = fw_size;
	fw_info.checkSum = checkSum;
	fw_info.fw_version = fw_version;
    fw_info.package_num = package_num;
    CL_LOG("fw_size=%d checkSum=%X package_num=%d fw_version=%d.\n",fw_info.size,fw_info.checkSum,fw_info.package_num,fw_info.fw_version);
    //CL_LOG("GetUpgradeReqestFlag()=%d.\n",GetUpgradeReqestFlag());
    //CL_LOG("GetUARTUpgradeFlag()=%d.\n",GetUARTUpgradeFlag());
    //CL_LOG("GetBlueToothUpgradeFlag()=%d.\n",GetBlueToothUpgradeFlag());

    if((1 == GetUARTUpgradeFlag()) || (1 == GetBlueToothUpgradeFlag()))
    {
        fw_info.current_package = 1;
        //�����̼�������
        Flash_EraseALL(FIRMWARE_PART);
        Flash_SetPos_w(FIRMWARE_PART,UPDATE_HEAD_POS);//���ƫ��  �����洢ͷ��Ϣ
        UpgradeStart();
        CL_LOG("���ڻ���������\n");
    }
    else //�����2.4g����
    {
        CL_LOG("2.4g����\n");
        //if(1 == GetUpgradeReqestFlag())
        {
            //CL_LOG("֮ǰ�汾 UpgradeRecord.fw_version = %d, fw_version = %d\n", UpgradeRecord.fw_version, fw_version);
            if((UpgradeRecord.fw_version != fw_version))
            {
                fw_info.current_package = 1;
                UpgradeRecord.current_package = 1;
                UpgradeRecord.fw_version = fw_version;  //�ϴ�û��������Ͷϵ��� , ����ֱ�����һ���汾, ��Ҫ��ͷ��ʼд����, ���ұ��浱ǰ�����汾��
                UpgradeRecord.AddressOffset = UPDATE_HEAD_POS;
                //�����̼�������
                Flash_EraseALL(FIRMWARE_PART);
                Flash_SetPos_w(FIRMWARE_PART,UpgradeRecord.AddressOffset);//���ƫ��  �����洢ͷ��Ϣ
                UpgradeStart();
                CL_LOG("2.4g��ͷ��ʼ����! ��ǰ��  = %d, current_package = %d\n", fw_info.current_package, UpgradeRecord.current_package);
            }
            else if((UpgradeRecord.fw_version == fw_version) && 
                ((0xff != UpgradeRecord.fw_version) && (0xffffffff != UpgradeRecord.AddressOffset)))
            {
                fw_info.current_package = UpgradeRecord.current_package;    //
                #if 1
				UpgradeFlashErasePage();
				#else
                {
                    uint32_t i;
					uint32_t mPage = 0;
    				uint32_t currentPageLenth = 0;
                    mPage = (UpgradeRecord.AddressOffset / 1024);       //����д�˶���ҳ
                    currentPageLenth = UpgradeRecord.AddressOffset % 1024;       //
                    CL_LOG("д�˵�ҳ��Ϊ mPage =%d, ����һҳ�ĳ��� currentPageLenth = %d ! \n", mPage, currentPageLenth);
                    if(mPage >= flash_partition[FIRMWARE_PART].s_count)
                    {
                        CL_LOG("flash����flash����flash����flash����");
                        fw_info.current_package = 1;
                        UpgradeRecord.current_package = 1;
                    }
                    Flash_SetPos_r(FIRMWARE_PART, mPage * FLASH_PAGE_SIZE);    //��ҳ�׵�ַΪƫ��, ��ȡû��д�����һҳ��Ȼ���ٲ�����ҳ����������д��ȥ

                    MuxSempTake(&gChgInfo.gProtoMuxType.gBuff1024Byte);
                    Flash_Read(FIRMWARE_PART, gFlashCacheBuff, currentPageLenth);  //����Ҫ����ҳ
                    //PrintfData("��������һҳ���� Upgrade_Init", gFlashCacheBuff, currentPageLenth);
                    while(1)
                  	{
						for(i = 0; i < (flash_partition[FIRMWARE_PART].s_count - mPage); i++)
						{	//�����̼�������
							//HT_Flash_PageErase(FIRMWARE_PART + (mPage + i)*FLASH_PAGE_SIZE);
							Flash_ErasePage(FIRMWARE_PART, (mPage + i));
						}
						Flash_SetPos_w(FIRMWARE_PART, mPage * FLASH_PAGE_SIZE);    //��������ҳ, ҲҪ���׵�ַΪƫ����д��ȥ
						//MuxSempTake(&gChgInfo.gProtoMuxType.gBuff1024Byte);
						if(0 == Flash_Write(FIRMWARE_PART,&gFlashCacheBuff[0], currentPageLenth))	//��Ҫд��ҳ, ���滹û������
						{
							break;
						}
						else
						{
							Feed_WDT();
							CL_LOG("flash[%d]д����! \n", FIRMWARE_PART);
							//osDelay(100);
						}
					}
                    MuxSempGive(&gChgInfo.gProtoMuxType.gBuff1024Byte);
                }
				#endif
                Flash_SetPos_w(FIRMWARE_PART,UpgradeRecord.AddressOffset);//���ƫ��  �����洢ͷ��Ϣ
                //UpgradeStart();
                CL_LOG("2.4g���м俪ʼ����! \n");
            }
            else
            {
                CL_LOG("2.4g��������! \n");
                UpgradeRecord.current_package = 1;
                UpgradeRecord.AddressOffset = UPDATE_HEAD_POS;
            }
            CL_LOG("��ǰ����� current_package = %d, UpgradeRecord.current_package = %d! \n", fw_info.current_package, UpgradeRecord.current_package);
			
			FlashWriteData(UPGRADE_PART, (void*)&UpgradeRecord, sizeof(UpgradeRecord));
        }
    }
}


uint8_t Upgrade_FlashCheck(void)
{
	uint16_t checksum = 0;
	uint8_t  rbyte = 0;
	uint32_t i = 0;
	
	Flash_SetPos_r(FIRMWARE_PART,UPDATE_HEAD_POS);
	for(i = 0; i < fw_info.size; i++)
	{	
		Flash_Read(FIRMWARE_PART,&rbyte,1);
		//ReadFlashInfoData(FIRMWARE_PART, (uint8_t*)&rbyte, (uint8_t*)&ReadByte , 1);
        //printf("%x, %x.\n", i, rbyte);
		checksum += rbyte;
	}

    CL_LOG("checksum=%X.\n",checksum);
	if(checksum != fw_info.checkSum)
    {
        CL_LOG("fw_info.checkSum = %X.\n", fw_info.checkSum);
        return 1;
    }
	
	return 0;
}

/*****************************************************************************
** Function name:       Upgrade_Write
** Descriptions:        
** input parameters:    pMsg: �ڲ�Э������ͷָ��
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
uint8_t Upgrade_Write(uint8_t *pData,uint16_t len)
{    
    //PrintfData("������������",pData,len);
    
	if(1 == Flash_Write(FIRMWARE_PART,pData,len))
	{
		return 8;
	}
    //CL_LOG("upgrade current package=%d.\n",fw_info.current_package);
    //printf("%d\n",fw_info.current_package);
    if((fw_info.current_package == fw_info.package_num) && (0 < fw_info.package_num))
    {
        CL_LOG("���һ�����Ѿ�д��, ��ʼУ��.\n");
        //���ݰ�������� У�����ݺ�
        if(Upgrade_FlashCheck() != 0)
		{
            CL_LOG("upgrade checksum error.\n");
            return 0;
        }
        //д����Ϣͷ
        SYS_UPDATE_INFO_t info; 
        info.checkSum = fw_info.checkSum;
        info.size = fw_info.size;
        info.updateFlag = 0xAA55;
        Flash_SetPos_w(FIRMWARE_PART,0);
		if(1 == Flash_Write(FIRMWARE_PART, &info, sizeof(SYS_UPDATE_INFO_t)))
		{
			#if 0
			while(1)
			{
				MuxSempTake(&gChgInfo.gProtoMuxType.gBuff1024Byte);
				{
					Flash_SetPos_r(FIRMWARE_PART, 0);    //��ҳ�׵�ַΪƫ��, ��ȡû��д�����һҳ��Ȼ���ٲ�����ҳ����������д��ȥ
	                //Flash_Read(FIRMWARE_PART, &gFlashCacheBuff[0], 1024);  //����Ҫ����ҳ
					ReadFlashInfoData(FIRMWARE_PART, (uint8_t*)gFlashCacheBuff, (uint8_t*)gFlashCacheBuff, 1024);  	//����Ҫ����ҳ
	                SYS_UPDATE_INFO_t *pFlash = (SYS_UPDATE_INFO_t*)&gFlashCacheBuff[0];
					pFlash->checkSum = fw_info.checkSum;
				    pFlash->size = fw_info.size;
				    pFlash->updateFlag = 0xAA55;
					Flash_ErasePage(FIRMWARE_PART, 0);
					Flash_SetPos_w(FIRMWARE_PART,0);
					if(0 == Flash_Write(FIRMWARE_PART, &gFlashCacheBuff[0], 1024))
					{
						MuxSempGive(&gChgInfo.gProtoMuxType.gBuff1024Byte);
						break;
					}
					else
					{
						CL_LOG("flash1д���ݳ���[%d]! \n", FIRMWARE_PART);
					}
				}
				MuxSempGive(&gChgInfo.gProtoMuxType.gBuff1024Byte);
				osDelay(100);
			}
			#else
			return 0;
			#endif
		}
		else
		{
			CL_LOG("upgrade success,reset effect.\n");
			CL_LOG("upgrade success. size=%d checksum=%X\n",fw_info.size,fw_info.checkSum);
				
			return 2;
		}
    }
    else
	{
        fw_info.current_package++;
    }
    
    return 1;
}

/*****************************************************************************
** Function name:       COM_Upgrade_Write
** Descriptions:        
** input parameters:    pMsg: �ڲ�Э������ͷָ��
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
uint8_t COM_Upgrade_Write(uint8_t *data,uint16_t datalen)
{
	uint8_t index = data[0];
	uint8_t *pData = data+1;
	uint16_t len = datalen - 1;
	
	//CL_LOG("index %d. \n", index);
	//CL_LOG("upgradeInfo.current_package = %d, len = %d. \n", fw_info.current_package, len);
	if((fw_info.current_package == 1) && (len >= 32) /*&& (1 == GetUARTUpgradeFlag())*/)
	{
		fw_info.lastIndex = index;
		if(pData[0] == 0xAA && pData[1] == 0x55)
		{
			FW_INFOR_STR *pFwInfo = (void*)(pData+16);
			if(0 == memcmp(pFwInfo->name,"X6",2))
			{
				fw_info.size = pFwInfo->size;
				fw_info.checkSum = pFwInfo->checkSum;
				if(1 == Flash_Write(FIRMWARE_PART,pData+32,len-32))
				{
					CL_LOG("flash2 д���ݴ�. \n");
					return 0;
				}
				fw_info.current_package++;
				CL_LOG("fw head info:size=%d  checkSum=%X \n",fw_info.size,fw_info.checkSum);
				return 1;
			}
			else
			{
				CL_LOG("fw name error. \n");
				return 0;
			}
		}
		else
		{
			CL_LOG("fw head error. \n");
			return 0;
		}
	}

	if((fw_info.lastIndex == index) /*&& (1 == GetUARTUpgradeFlag())*/)
	{
		CL_LOG("current pacakge is repeat transmate..\n");
		return 1;
	}
	fw_info.lastIndex = index;
	#if 1
	return Upgrade_Write(pData, len);
	#else
	Flash_Write(FIRMWARE_PART, pData, len);
	if((fw_info.current_package == fw_info.package_num)  && (0 < fw_info.package_num))
    {
		if(Upgrade_FlashCheck() != 0)
		{
			CL_LOG("upgrade checksum error.\n");
			return 0;
		}
		//д����Ϣͷ
		SYS_UPDATE_INFO_t info; 
		info.checkSum = fw_info.checkSum;
		info.size = fw_info.size;
		info.updateFlag = 0xAA55;
		Flash_SetPos_w(FIRMWARE_PART,0);
		Flash_Write(FIRMWARE_PART,&info,sizeof(SYS_UPDATE_INFO_t));
		#if 0
		Flash_SetPos_r(FIRMWARE_PART,UPDATE_HEAD_POS);
		for(uint32_t i = 0;i<fw_info.size;i++)
		{
		    Flash_Read(FIRMWARE_PART,&rbyte,1);
		    printf("%02X ", rbyte);
		    if(0 == (i+1)%16)
		    {
		        printf("\n");
		    }
		}
		#endif
		CL_LOG("upgrade success,reset effect.\n");
		CL_LOG("upgrade success. size=%d checksum=%X\n",fw_info.size,fw_info.checkSum);
		return 2;
	}
	else
	{
		fw_info.current_package++;
	}
	#endif
}

//��ȡ��ǰ�̼�����Ƭ�� ��1��ʼ
int GetUpgradeCurrentPackage(void)
{
	return fw_info.current_package;
}	



