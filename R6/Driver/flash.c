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
*                                           本地宏/结构体
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
* 函数说明: Flash字节写
*
* 入口参数: pWriteByte    指向被写数据的首地址
*
*           Address       数据将要写到Flash的地址
*
*           Num           写数据长度，以字节为单位
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*********************************************************************************************************
*/
void HT_Flash_ByteWrite(uint8_t* pWriteByte, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flash写                */

    for(i=0; i<Num; i++)
    {
        M8(Address+i) = pWriteByte[i];                                     /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
        Feed_WDT();
    }
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
    MASTER_INT_EN();
}

/*
*********************************************************************************************************
*                                         FLASH BYTE READ
*
* 函数说明: Flash字节读
*
* 入口参数: pReadByte     指向存储读到的数据的首地址
*
*           Address       读Flash的首地址
*
*           Num           读数据长度，以字节为单位
*
* 返回参数: 无
*
* 特殊说明: 用户需注意读的数据不要超出保存的范围，以防溢出
*********************************************************************************************************
*/
void HT_Flash_ByteRead(uint8_t* pReadByte, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */

    for(i=0; i<Num; i++)
    {
        pReadByte[i] = M8(Address+i);                                      /*!< 执行Flash读            */
    }

}

#if 1
/*
*********************************************************************************************************
*                                         FLASH HALF WORD WRITE
*
* 函数说明: Flash半字写
*
* 入口参数: pWriteHalfWord    指向被写数据的首地址
*
*           Address           数据将要写到Flash的地址
*
*           Num               写数据长度，以半字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少半字对齐
*********************************************************************************************************
*/
void HT_Flash_HalfWordWrite(uint16_t* pWriteHalfWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flash写                */

    Address &= 0xFFFFFFFE;                                                 /*!< 保证半字对齐           */
    for(i=0; i<Num; i++)
    {
        M16(Address+i*2) = pWriteHalfWord[i];                              /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
        Feed_WDT();
    }
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
    MASTER_INT_EN();
}

/*
*********************************************************************************************************
*                                         FLASH HALF WORD READ
*
* 函数说明: Flash半字读
*
* 入口参数: pReadHalfWord 指向存储读到的数据的首地址
*
*           Address       读Flash的首地址
*
*           Num           读数据长度，以半字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少半字对齐
*********************************************************************************************************
*/
void HT_Flash_HalfWordRead(uint16_t* pReadHalfWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    Address &= 0xFFFFFFFE;                                                 /*!< 保证半字对齐           */
    for(i=0; i<Num; i++)
    {
        pReadHalfWord[i] = M16(Address+i*2);                               /*!< 执行Flash读            */
    }

}

/*
*********************************************************************************************************
*                                         FLASH WORD WRITE
*
* 函数说明: Flash字写
*
* 入口参数: pWriteWord    指向被写数据的首地址
*
*           Address       数据将要写到Flash的地址
*
*           Num           写数据长度，以字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少字对齐
*********************************************************************************************************
*/
void HT_Flash_WordWrite(uint32_t* pWriteWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flash写                */

    Address &= 0xFFFFFFFC;                                                 /*!< 保证字对齐             */
    for(i=0; i<Num; i++)
    {
        M32(Address+i*4) = pWriteWord[i];                                  /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
        Feed_WDT();
    }
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
    MASTER_INT_EN();
}


/*
*********************************************************************************************************
*                                         FLASH WORD READ
*
* 函数说明: Flash字读
*
* 入口参数: pReadWord     指向存储读到的数据的首地址
*
*           Address       读Flash的首地址
*
*           Num           读数据长度，以字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少字对齐
*********************************************************************************************************
*/
void HT_Flash_WordRead(uint32_t* pReadWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    Address &= 0xFFFFFFFC;                                                 /*!< 保证字对齐             */
    for(i=0; i<Num; i++)
    {
        pReadWord[i] = M32(Address+i*4);                                   /*!< 执行Flash读            */
    }

}
#endif

/*
*********************************************************************************************************
*                                         FLASH PAGE ERASE
*
* 函数说明: Flash页擦除
*
* 入口参数: EraseAddress    页擦除地址
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变，1K bytes/page
*********************************************************************************************************
*/
void HT_Flash_PageErase(uint32_t EraseAddress)
{
    /*  assert_param  */

    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

	MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_PAGEERASE;                         /*!< Flash页擦除            */

    EraseAddress &= 0xFFFFFFFC;                                            /*!< 保证字对齐             */
    M32(EraseAddress) = 0xFF;                                              /*!< 执行Flash页擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */
    Feed_WDT();

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
	MASTER_INT_EN();
}

#if 0
/*
*********************************************************************************************************
*                                         FLASH CHIP ERASE
*
* 函数说明: Flash全擦除
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*********************************************************************************************************
*/
void HT_Flash_ChipErase(void)
{
    /*  assert_param  */

    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

	MASTER_INT_DIS();
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_CHIPERASE;                         /*!< Flash页擦除            */

    M32(0x1000) = 0xFF;                                                    /*!< 执行Flash全擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */
    Feed_WDT();

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
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

//readBack: 写完是否回读 0不回读  1回读
//返回: 0写成功  其他:回读失败
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
					CL_LOG("写flash出错[%d]!\n", k);
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

//readBack: 写完是否回读 0不回读  1回读
//返回: 0写成功  其他:回读失败
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
					CL_LOG("写flash出错[%d]!\n", k);
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
//readBack: 写完是否回读 0不回读  1回读
//返回: 0写成功  其他:回读失败
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




