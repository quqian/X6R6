/*flash.c
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "flash.h"

#define  __HT60XX_FLASH_C



/*
*********************************************************************************************************
*                                           ���غ�/�ṹ��
*********************************************************************************************************
*/

#define M8(adr)     (*((uint8_t * ) (adr)))
#define M16(adr)    (*((uint16_t *) (adr)))
#define M32(adr)    (*((uint32_t *) (adr)))

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};


/*
*********************************************************************************************************
*                                         FLASH BYTE WRITE
*
* ����˵��: Flash�ֽ�д
*
* ��ڲ���: pWriteByte    ָ��д���ݵ��׵�ַ
*
*           Address       ���ݽ�Ҫд��Flash�ĵ�ַ
*
*           Num           д���ݳ��ȣ����ֽ�Ϊ��λ
*
* ���ز���: ��
*
* ����˵��: �û�Ӧ��֤����ִ�й����мĴ���д����״̬�Լ�Flash����״̬����
*********************************************************************************************************
*/
void HT_Flash_ByteWrite(uint8_t* pWriteByte, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */

    MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash����              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flashд                */

    for(i=0; i<Num; i++)
    {
        M8(Address+i) = pWriteByte[i];                                     /*!< ִ��Flashд            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< �ȴ�д���             */
        Feed_WDT();
    }
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash����              */

    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
    MASTER_INT_EN();
}

/*
*********************************************************************************************************
*                                         FLASH BYTE READ
*
* ����˵��: Flash�ֽڶ�
*
* ��ڲ���: pReadByte     ָ��洢���������ݵ��׵�ַ
*
*           Address       ��Flash���׵�ַ
*
*           Num           �����ݳ��ȣ����ֽ�Ϊ��λ
*
* ���ز���: ��
*
* ����˵��: �û���ע��������ݲ�Ҫ��������ķ�Χ���Է����
*********************************************************************************************************
*/
void HT_Flash_ByteRead(uint8_t* pReadByte, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */

    for(i=0; i<Num; i++)
    {
        pReadByte[i] = M8(Address+i);                                      /*!< ִ��Flash��            */
    }

}

#if 1
/*
*********************************************************************************************************
*                                         FLASH HALF WORD WRITE
*
* ����˵��: Flash����д
*
* ��ڲ���: pWriteHalfWord    ָ��д���ݵ��׵�ַ
*
*           Address           ���ݽ�Ҫд��Flash�ĵ�ַ
*
*           Num               д���ݳ��ȣ��԰���Ϊ��λ
*
* ���ز���: ��
*
* ����˵��: 1)�û�Ӧ��֤����ִ�й����мĴ���д����״̬�Լ�Flash����״̬����
*           2)�û�Ӧ��֤���ݸ������ĵ�ַΪ���ٰ��ֶ���
*********************************************************************************************************
*/
void HT_Flash_HalfWordWrite(uint16_t* pWriteHalfWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */

    MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash����              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flashд                */

    Address &= 0xFFFFFFFE;                                                 /*!< ��֤���ֶ���           */
    for(i=0; i<Num; i++)
    {
        M16(Address+i*2) = pWriteHalfWord[i];                              /*!< ִ��Flashд            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< �ȴ�д���             */
        Feed_WDT();
    }
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash����              */

    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
    MASTER_INT_EN();
}

/*
*********************************************************************************************************
*                                         FLASH HALF WORD READ
*
* ����˵��: Flash���ֶ�
*
* ��ڲ���: pReadHalfWord ָ��洢���������ݵ��׵�ַ
*
*           Address       ��Flash���׵�ַ
*
*           Num           �����ݳ��ȣ��԰���Ϊ��λ
*
* ���ز���: ��
*
* ����˵��: 1)�û�Ӧ��֤����ִ�й����мĴ���д����״̬�Լ�Flash����״̬����
*           2)�û�Ӧ��֤���ݸ������ĵ�ַΪ���ٰ��ֶ���
*********************************************************************************************************
*/
void HT_Flash_HalfWordRead(uint16_t* pReadHalfWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */
    Address &= 0xFFFFFFFE;                                                 /*!< ��֤���ֶ���           */
    for(i=0; i<Num; i++)
    {
        pReadHalfWord[i] = M16(Address+i*2);                               /*!< ִ��Flash��            */
    }

}

/*
*********************************************************************************************************
*                                         FLASH WORD WRITE
*
* ����˵��: Flash��д
*
* ��ڲ���: pWriteWord    ָ��д���ݵ��׵�ַ
*
*           Address       ���ݽ�Ҫд��Flash�ĵ�ַ
*
*           Num           д���ݳ��ȣ�����Ϊ��λ
*
* ���ز���: ��
*
* ����˵��: 1)�û�Ӧ��֤����ִ�й����мĴ���д����״̬�Լ�Flash����״̬����
*           2)�û�Ӧ��֤���ݸ������ĵ�ַΪ�����ֶ���
*********************************************************************************************************
*/
void HT_Flash_WordWrite(uint32_t* pWriteWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */

    MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash����              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flashд                */

    Address &= 0xFFFFFFFC;                                                 /*!< ��֤�ֶ���             */
    for(i=0; i<Num; i++)
    {
        M32(Address+i*4) = pWriteWord[i];                                  /*!< ִ��Flashд            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< �ȴ�д���             */
        Feed_WDT();
    }
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash����              */

    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
    MASTER_INT_EN();
}


/*
*********************************************************************************************************
*                                         FLASH WORD READ
*
* ����˵��: Flash�ֶ�
*
* ��ڲ���: pReadWord     ָ��洢���������ݵ��׵�ַ
*
*           Address       ��Flash���׵�ַ
*
*           Num           �����ݳ��ȣ�����Ϊ��λ
*
* ���ز���: ��
*
* ����˵��: 1)�û�Ӧ��֤����ִ�й����мĴ���д����״̬�Լ�Flash����״̬����
*           2)�û�Ӧ��֤���ݸ������ĵ�ַΪ�����ֶ���
*********************************************************************************************************
*/
void HT_Flash_WordRead(uint32_t* pReadWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */
    Address &= 0xFFFFFFFC;                                                 /*!< ��֤�ֶ���             */
    for(i=0; i<Num; i++)
    {
        pReadWord[i] = M32(Address+i*4);                                   /*!< ִ��Flash��            */
    }

}
#endif

/*
*********************************************************************************************************
*                                         FLASH PAGE ERASE
*
* ����˵��: Flashҳ����
*
* ��ڲ���: EraseAddress    ҳ������ַ
*
* ���ز���: ��
*
* ����˵��: �û�Ӧ��֤����ִ�й����мĴ���д����״̬�Լ�Flash����״̬���䣬1K bytes/page
*********************************************************************************************************
*/
void HT_Flash_PageErase(uint32_t EraseAddress)
{
    /*  assert_param  */

    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */

	MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash����              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_PAGEERASE;                         /*!< Flashҳ����            */

    EraseAddress &= 0xFFFFFFFC;                                            /*!< ��֤�ֶ���             */
    M32(EraseAddress) = 0xFF;                                              /*!< ִ��Flashҳ����        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< �ȴ�д���             */
    Feed_WDT();

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash����              */

    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
	MASTER_INT_EN();
}

#if 0
/*
*********************************************************************************************************
*                                         FLASH CHIP ERASE
*
* ����˵��: Flashȫ����
*
* ��ڲ���: ��
*
* ���ز���: ��
*
* ����˵��: �û�Ӧ��֤����ִ�й����мĴ���д����״̬�Լ�Flash����״̬����
*********************************************************************************************************
*/
void HT_Flash_ChipErase(void)
{
    /*  assert_param  */

    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */

	MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash����              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_CHIPERASE;                         /*!< Flashҳ����            */

    M32(0x1000) = 0xFF;                                                    /*!< ִ��Flashȫ����        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< �ȴ�д���             */
    Feed_WDT();

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash��                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash����              */

    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
	MASTER_INT_EN();
}
#endif

void FlashWriteAppBackup(uint32_t app_backup_record_addr, uint8_t* buffer, uint16_t len)
{
   // HT_Flash_WordWrite((void*)buffer, app_backup_record_addr, len/4);
	HT_Flash_ByteWrite((void*)buffer, app_backup_record_addr, len);
}


void FlashEraseAppBackup(uint32_t addr, uint32_t size)
{
    int i;

    CL_LOG("erase flash,addr=%d,len=%d.\n",addr,size);
    for (i=0; i<(size/1024); i++)
	{
        HT_Flash_PageErase(addr+i*1024);
    }
}


void WriteUpdateInfo(uint32_t fsize, uint32_t checkSum)
{
    updateInfo.updateFlag = 0xaa55;
    updateInfo.fsize = fsize;
    updateInfo.checkSum = checkSum;
    BootSetReqInfo();
}

//readBack: д���Ƿ�ض� 0���ض�  1�ض�
//����: 0д�ɹ�  ����:�ض�ʧ��
int FlashWriteSysInfo1(void *pSysInfo, uint16_t size, uint8_t readBack)
{
    uint32_t i = 0;
	uint32_t k = 0;
    uint8_t  data = 0;
	static uint8_t timesFlag = 0;
	
FlashPageErase1: 
	if(3 < timesFlag++)
	{
		return CL_FAIL;
	}
	HT_Flash_PageErase(SysCfg1InfoAddr);
	HT_Flash_ByteWrite((void*)pSysInfo, SysCfg1InfoAddr, size);

    if (readBack)
	{
        for (i = 0; i < size; i++)
		{
			for (k = 0; k < 5; k++)
			{
	            HT_Flash_ByteRead(&data, SysCfg1InfoAddr + i, 1);
	            if (data == *((uint8_t*)pSysInfo + i))
				{
	                break;
	            }
				else
				{
					CL_LOG("дflash����[%d]!\n", k);
					if(4 == k)
					{
						goto FlashPageErase1;
					}
				}
			}
        }
    }
	timesFlag = 0;
    //CL_LOG("write falsh ok.\n");
    return CL_OK;
}

//readBack: д���Ƿ�ض� 0���ض�  1�ض�
//����: 0д�ɹ�  ����:�ض�ʧ��
int FlashWriteSysInfo(void *pSysInfo, uint16_t size, uint8_t readBack)
#if 1
{
    uint32_t i = 0;
	uint32_t k = 0;
    uint8_t  data = 0;
	static uint8_t timesFlag = 0;
	
FlashPageErase: 
	if(3 < timesFlag++)
	{
		return CL_FAIL;
	}
	HT_Flash_PageErase(SysCfgInfoAddr);
	HT_Flash_ByteWrite((void*)pSysInfo, SysCfgInfoAddr, size);

    if (readBack)
	{
        for (i = 0; i < size; i++)
		{
			for (k = 0; k < 5; k++)
			{
	            HT_Flash_ByteRead(&data, SysCfgInfoAddr+i, 1);
	            if (data == *((uint8_t*)pSysInfo + i))
				{
	                break;
	            }
				else
				{
					CL_LOG("дflash����[%d]!\n", k);
					if(4 == k)
					{
						goto FlashPageErase;
					}
				}
			}
        }
    }
	timesFlag = 0;
    CL_LOG("write falsh ok.\n");
    return CL_OK;
}
#else
{
    int i;
    uint8_t  data;

    HT_Flash_PageErase(SysCfgInfoAddr);
    //HT_Flash_PageErase(SysCfgInfoAddr+1024);
	HT_Flash_ByteWrite((void*)pSysInfo, SysCfgInfoAddr, size);

    if (readBack) {
        for (i=0; i<size; i++) {
            HT_Flash_ByteRead(&data, SysCfgInfoAddr+i, 1);
            if (data != *((uint8_t*)pSysInfo + i)) {
                CL_LOG("write falsh fail.\n");
                return CL_FAIL;
            }
        }
    }
    CL_LOG("write falsh ok.\n");
    return CL_OK;
}
#endif

#if 0
//readBack: д���Ƿ�ض� 0���ض�  1�ض�
//����: 0д�ɹ�  ����:�ض�ʧ��
int FlashWriteGunInfo(void *pGunInfo, uint16_t size, uint8_t readBack)
{
	int i;
    uint8_t  data;

    for (i=0; i<4; i++)
	{
	    HT_Flash_PageErase(GunInfoAddr+i*1024);
    }
	HT_Flash_ByteWrite((void*)pGunInfo, GunInfoAddr, size);

    if (readBack)
	{
        for (i = 0; i < size; i++)
		{
            HT_Flash_ByteRead(&data, GunInfoAddr+i, 1);
            if (data != *((uint8_t*)pGunInfo + i))
			{
                CL_LOG("write falsh fail.\n");
                return CL_FAIL;
            }
        }
    }
    CL_LOG("write falsh ok.\n");
    return CL_OK;
}

int FlashReadGunInfo(void *pGunInfo, uint16_t size)
{
    HT_Flash_ByteRead((void*)pGunInfo, GunInfoAddr, size);
    return CL_OK;
}
#endif

int FlashReadSysInfo(void *pInfo, uint16_t size)
{
	HT_Flash_ByteRead((void*)pInfo, SysCfgInfoAddr, size);
    return CL_OK;
}

int FlashReadSysInfo1(void *pInfo, uint16_t size)
{
	HT_Flash_ByteRead((void*)pInfo, SysCfg1InfoAddr, size);
	
    return CL_OK;
}




