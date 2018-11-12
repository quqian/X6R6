/*spi_w25qx.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "spi_w25qx.h"


uint8_t  gTradeBuffer[W25X_FLASH_SECTOR_SIZE];


//spi初始化
static void SpiInit(void)
{
	SPI_InitTypeDef SPI_Init;
	EnWr_WPREG();
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_7, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
	HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_SPI0EN, ENABLE);
	SPI_Init.SPI_Baudrate = SPI_BaudRatePrescaler_8;
	SPI_Init.SPI_CPHA = SPI_CPHA_2Edge; //沿前半周期数据发送
	SPI_Init.SPI_CPOL = SPI_CPOL_high;  //低电平空闲
	SPI_Init.SPI_CSInCtrol = DISABLE;
	SPI_Init.SPI_Mode = SPI_Master;
	HT_SPI_Init(HT_SPI0,&SPI_Init);
	DisWr_WPREG();
}

//spi读写一个字节
uint8_t SpiReadWriteByte(HT_SPI_TypeDef* SPIx, uint16_t halfword)
{
	while(SET==HT_SPI_ITFlagStatusGet(SPIx, SPI_SPISTA_SPIF));
    SPIx->SPIDAT = halfword;

	while(RESET==HT_SPI_ITFlagStatusGet(SPIx, SPI_SPISTA_SPIF));
    return (uint8_t)SPIx->SPIDAT;
}

//读flash型号:W25Q80, W25Q16, and W25Q32
uint16_t W25Qx_ReadID(void)
{
    uint16_t Temp = 0;
    ENABLE_CS();
    SpiReadWriteByte(HT_SPI0, W25X_ManufactDeviceID);
    SpiReadWriteByte(HT_SPI0, 0x00);
    SpiReadWriteByte(HT_SPI0, 0x00);
    SpiReadWriteByte(HT_SPI0, 0x00);
    Temp |= SpiReadWriteByte(HT_SPI0, 0xFF)<<8;
    Temp |= SpiReadWriteByte(HT_SPI0, 0xFF);
    DISABLE_CS();
    return Temp;
}

//打开写使能
void W25Qx_Write_Enable(void)
{
	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_WriteEnable);
	DISABLE_CS();
}

//关闭写使能
void W25Qx_Write_Disable(void)
{
	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_WriteDisable);
	DISABLE_CS();
}

//读状态寄存器1
uint8_t W25Qx_Read_StatusReg1(void)
{
	uint8_t status = 0;
	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_ReadStatusReg_05);
	status = SpiReadWriteByte(HT_SPI0, 0xFF);
	DISABLE_CS();
	return status;
}

//读状态寄存器2
uint8_t W25Qx_Read_StatusReg2(void)
{
	uint8_t status = 0;
	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_ReadStatusReg_35);
	status = SpiReadWriteByte(HT_SPI0, 0xFF);
	DISABLE_CS();
	return status;
}

//写状态寄存器
void W25QX_Write_StatusReg(uint8_t reg1, uint8_t reg2)
{
    ENABLE_CS();
    SpiReadWriteByte(HT_SPI0, W25X_WriteStatusReg);
    SpiReadWriteByte(HT_SPI0, reg1);
    SpiReadWriteByte(HT_SPI0, reg2);
    DISABLE_CS();
}

//等待读写结束
void W25Qx_Wait_Busy(void)
{
	while((W25Qx_Read_StatusReg1() && 0x01) == 0x01);
}

uint32_t W25Qx_Wait_Busy_Noblock(void)
{
	return (W25Qx_Read_StatusReg1() && 0x01);
}


//擦除一个扇区(4k)
void W25Qx_Erase_Sector(uint32_t nSector)
{
    uint32_t  Addr = W25X_FLASH_SECTOR_SIZE * nSector;

	W25Qx_Write_Enable();
	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_ERASE_4K);
	SpiReadWriteByte(HT_SPI0, (uint8_t)((Addr & 0xFFFFFF) >> 16));
	SpiReadWriteByte(HT_SPI0, (uint8_t)((Addr & 0xFFFF) >> 8));
	SpiReadWriteByte(HT_SPI0, (uint8_t)(Addr & 0xFF));
	DISABLE_CS();

    // wait 2s
    for(int i = 0; i < 40; i++){
        OS_DELAY_MS (50);
        if(!W25Qx_Wait_Busy_Noblock()){
            break;
        }
    }
}

//擦除一个块(64K)
void W25Qx_Erase_Block(uint32_t nSector)
{
    uint32_t  Addr = W25X_FLASH_SECTOR_SIZE * nSector;

	W25Qx_Write_Enable();
	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_ERASE_64K);
	SpiReadWriteByte(HT_SPI0, (uint8_t)((Addr & 0xFFFFFF) >> 16));
	SpiReadWriteByte(HT_SPI0, (uint8_t)((Addr & 0xFFFF) >> 8));
	SpiReadWriteByte(HT_SPI0, (uint8_t)Addr & 0xFF);
	DISABLE_CS();

    // wait 4s
    for (int i = 0; i < 80; i++) {
        OS_DELAY_MS (50);
        if (!W25Qx_Wait_Busy_Noblock()) {
            break;
        }
    }
}

//擦除整个芯片
void W25Qx_Erase_Chip(void)
{
	W25Qx_Write_Enable();

	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_ERASE_FULL);
	DISABLE_CS();

    // wait 8s
    for (int i = 0; i < 80; i++) {
        OS_DELAY_MS (100);
        if (!W25Qx_Wait_Busy_Noblock()) {
            break;
        }
    }
}

/*****************************************************************
写入一个字节
Addr：目标地址 0~~W25Qx_MAX_ADDR(0x0FFFFF)
Txdata：发送的字节
返回值：true or false
*****************************************************************/
uint8_t W25Qx_Write_Byte(uint32_t Addr, uint8_t Txdata)
{
    uint32_t secpos;

    if((Addr+1) > W25Qx_MAX_ADDR){
        return CL_FALSE;
    }
    secpos = Addr / W25X_FLASH_SECTOR_SIZE;
    W25Qx_Erase_Sector(secpos);//擦除要写入的扇区
    W25Qx_Write_Enable();
    ENABLE_CS();
    SpiReadWriteByte(HT_SPI0, W25X_PageProgram);
    SpiReadWriteByte(HT_SPI0, (uint8_t)(Addr >> 16));
    SpiReadWriteByte(HT_SPI0, (uint8_t)(Addr >> 8));
    SpiReadWriteByte(HT_SPI0, (uint8_t) Addr);
    SpiReadWriteByte(HT_SPI0, Txdata);
    DISABLE_CS();
    W25Qx_Wait_Busy();
    return CL_TRUE;
}

/**************************************************************************
页编程 0-~~256个字节
Addr：目标地址 0~~W25Qx_MAX_ADDR(0x0FFFFF)
Txdata：发送的指针
NByte ： 要写入的长度
返回值：true or false
**************************************************************************/
uint8_t W25Qx_WritePage(uint32_t nDest, uint8_t* pBuffer, uint16_t nBytes)
{
    W25Qx_Write_Enable();
    ENABLE_CS();
    SpiReadWriteByte(HT_SPI0, W25X_PageProgram);
    SpiReadWriteByte(HT_SPI0, (uint8_t)(nDest >> 16));
    SpiReadWriteByte(HT_SPI0, (uint8_t)(nDest >> 8));
    SpiReadWriteByte(HT_SPI0, (uint8_t) nDest);
    for (uint16_t i=0; i<nBytes; i++) {
        SpiReadWriteByte(HT_SPI0, pBuffer[i]);
    }
    DISABLE_CS();
    W25Qx_Wait_Busy();

    return CL_TRUE;
}


/*******************************************************************
读取一个扇区的 数据
nSector：扇区号  0~~W25Qx_SECTOR_COUNT(256)
RxData：接收缓存区
返回值：
*******************************************************************/
void W25Qx_Read_Sector(uint32_t nSector, uint8_t* RxData)
{
	nSector *= W25X_FLASH_PAGE_SIZE;

	ENABLE_CS();
	SpiReadWriteByte(HT_SPI0, W25X_ReadData);
	SpiReadWriteByte(HT_SPI0, (uint16_t)(nSector >> 16));
	SpiReadWriteByte(HT_SPI0, (uint16_t)(nSector >> 8));
	SpiReadWriteByte(HT_SPI0, (uint16_t) nSector);
	for (uint16_t i=0; i<W25X_FLASH_SECTOR_SIZE; i++) {
        RxData[i] = SpiReadWriteByte(HT_SPI0, 0xFF);
	}
	DISABLE_CS();
	W25Qx_Wait_Busy();
}


/***************************************************************************
从某个地址读取nBytes数据
Addr：目标地址
RxData：接收缓存区
nBytes: 要读取的字节数
返回值：
***************************************************************************/
uint8_t W25Qx_Read_Bytes(uint32_t Addr, uint8_t* RxData, uint16_t nBytes)
{
    if (((Addr + nBytes) > W25Qx_MAX_ADDR)||(nBytes == 0)){
        return (CL_FALSE);
    }
    ENABLE_CS();
    SpiReadWriteByte(HT_SPI0, W25X_ReadData);
    SpiReadWriteByte(HT_SPI0, (uint8_t)(Addr >> 16));
    SpiReadWriteByte(HT_SPI0, (uint8_t)(Addr >> 8));
    SpiReadWriteByte(HT_SPI0, (uint8_t)Addr);
    for(uint16_t i=0; i<nBytes; i++){
        RxData[i] = SpiReadWriteByte(HT_SPI0, 0xFF);
    }
    DISABLE_CS();
    W25Qx_Wait_Busy();
    return CL_TRUE;
}

// W25Q80 flash 初始化
uint8_t W25Q80_Init(void)
{
	uint32_t timesFlag = 0;
	
	while(1)
	{
		SpiInit();

	    SpiReadWriteByte(HT_SPI0, W25X_MODE_RESET);
	    if(W25Qx_ReadID() != SPI_FLASH_TYPE)
		{
	        CL_LOG("W25Q80_Init not ready!\n");
			vTaskDelay(1000);
	        if(20 < timesFlag++)
			{
				return CL_FALSE;
			}
	    } 
		else 
		{
	        CL_LOG("W25Q80_Init OK!\n");
	        return CL_TRUE;
	    }
	}
}


//测试
uint8_t W25Q80_Test(void)
{
	uint8_t wBuf[300] = {0};
	uint8_t rBuf[300] = {0};
	uint32_t wAddr = 0;

	for (int i=0; i<256; i++) {
		wBuf[i] = i;
	}
//	memset(wBuf, 0x55, 100);
//	memset((void*)&wBuf[100], 0xaa, 156);

//    CL_LOG("chip erase start\r\n");
//    W25Qx_Erase_Chip();
//    CL_LOG("chip erase finish\r\n");
	for (int sector=0; sector<FLASH_SECTOR_COUNT; sector++) {
		W25Qx_Erase_Sector(sector);
	}

	for (int i=0; i<4096; i++) {
		wAddr = i*256;
		W25Qx_WritePage(wAddr, wBuf, 256);
		memset(rBuf, 0, sizeof(rBuf));
		W25Qx_Read_Bytes(wAddr, rBuf, 256);
		CL_LOG("w25Q80 Addr = %d, wcnt = %d.\n", wAddr, i);
		PrintfData("W25Q80_Test", (void*)rBuf, 256);
		OS_DELAY_MS(100);
	}

    return 0;
}


int FlashWriteOrder(void *pTradeRecord, uint16_t size)
{
    ORDER_STR *pOrder = &system_info.orderInfo;
	uint8_t  currentTradeRecordPage;
	uint8_t  tradeRecordBlockPosInPage;
	uint32_t currentPageAddr;
	uint32_t tradeRecordBlockAddr;
	uint16_t saveTradeRecordSize;
	int ret;
	uint16_t orderCnt;

	if (pOrder->wIndex >= pOrder->rIndex)
	{
        orderCnt = pOrder->wIndex - pOrder->rIndex + 1;
    }
	else
	{
        orderCnt = pOrder->wIndex + RECORD_MAX_BLOCK_NUM - pOrder->rIndex + 1;
    }

    if (RECORD_MAX_BLOCK_NUM > orderCnt)
	{
        //当前订单记录的页
		currentTradeRecordPage = pOrder->wIndex / RECORD_BLOCK_NUM_PER_PAGE;
		//当前页记录块的位置
		tradeRecordBlockPosInPage = pOrder->wIndex % RECORD_BLOCK_NUM_PER_PAGE;
		//当前页已记录订单的数据大小
		saveTradeRecordSize = tradeRecordBlockPosInPage * RECORD_BLOCK_SIZE_PER_PAGE;
		//当前页的基地址
		currentPageAddr = currentTradeRecordPage * SPI_FLASH_PAGE_SIZE;
		//订单记录的块地址
		tradeRecordBlockAddr =  currentPageAddr + saveTradeRecordSize;

		CL_LOG("wIndex=%d,saveTradeRecordSize=%d,currentPageAddr=%d,tradeRecordBlockAddr=%d.\n", pOrder->wIndex,saveTradeRecordSize,currentPageAddr,tradeRecordBlockAddr);
		//读取当前页数据后,填充要写入的订单数据到buffer
		W25Qx_Read_Bytes(currentPageAddr, gTradeBuffer, SPI_FLASH_PAGE_SIZE);
		memcpy((void*)&gTradeBuffer[saveTradeRecordSize], (void*)pTradeRecord, size);
		//擦除当前页
		W25Qx_Erase_Sector(currentTradeRecordPage);
		//写入buffer
		//PrintfData("FlashWriteTradeRecord", &tradeBuffer[saveTradeRecordSize], size);
		for (ret = 0; ret<(SPI_FLASH_PAGE_SIZE/W25X_FLASH_PAGE_SIZE); ret++) {
		    W25Qx_WritePage(currentPageAddr, &gTradeBuffer[ret * W25X_FLASH_PAGE_SIZE], W25X_FLASH_PAGE_SIZE);
            currentPageAddr += W25X_FLASH_PAGE_SIZE;
		}

		pOrder->wIndex++;
		pOrder->orderCnt++;
		if (RECORD_MAX_BLOCK_NUM <= pOrder->wIndex)
		{
            pOrder->wIndex = 0;
        }
		CL_LOG("save a new order,wIndex=%d,rIndex=%d,orderCnt=%d.\n",pOrder->wIndex,pOrder->rIndex,pOrder->orderCnt);
		FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
		ret = CL_OK;
    } 
	else
	{
        CL_LOG("no enough space,error,orderCnt=%d.\n",orderCnt);
		ret = CL_FAIL;
	}
	
	return ret;
}


void OrderReport(void)
{
    ORDER_STR *pOrder = &system_info.orderInfo;
    uint32_t rAddr = 0;
	STOP_CHARGING_NOTICE_REQ_STR report_power_off_req;

	if ((RECORD_MAX_BLOCK_NUM < pOrder->rIndex) || (RECORD_MAX_BLOCK_NUM < pOrder->wIndex))
	{
		CL_LOG("wIndex=%d != rIndex=%d, 错误!\n", pOrder->wIndex, pOrder->rIndex);
        pOrder->rIndex = 0;
		pOrder->wIndex = 0;
		FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
    }
	if (pOrder->wIndex != pOrder->rIndex) 
	{//缓冲区非空
		memset((void*)&report_power_off_req, 0, sizeof(report_power_off_req));
		CL_LOG("wIndex=%d != rIndex=%d, send order.\n", pOrder->wIndex, pOrder->rIndex);
		rAddr = pOrder->rIndex * RECORD_BLOCK_SIZE_PER_PAGE;
		if (pOrder->sendCnt < 2)
		{
            W25Qx_Read_Bytes(rAddr, (void*)&report_power_off_req, sizeof(report_power_off_req));   //先判断订单数据
            pOrder->gunId = report_power_off_req.gun_id;
			PrintfData("OrderReport发送结束订单给后台", (void*)&report_power_off_req, sizeof(STOP_CHARGING_NOTICE_REQ_STR));
			SendStopChargingNotice(&report_power_off_req);
            pOrder->sendCnt++;
		}
		else 
		{
			if (RECORD_MAX_BLOCK_NUM <= ++pOrder->rIndex) 
			{
				pOrder->rIndex = 0;
			}
			if (pOrder->orderCnt) 
			{
				pOrder->orderCnt--;
			}
			pOrder->sendCnt = 0;
            FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
			CL_LOG("retry send trade record cnt > 8,drop, rIndex=%d, orderCnt=%d.\n",pOrder->rIndex, pOrder->orderCnt);
		}
	}
}


