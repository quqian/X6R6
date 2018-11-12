/*spi_w25qx.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#ifndef __SPI_W25QX_H__
#define __SPI_W25QX_H__

#include "includes.h"

/** @defgroup flash type define
 */
#define FLASH_W25Q80                    0xEF13
#define FLASH_W25Q16                    0xEF14
#define FLASH_W25Q32                    0xEF15
#define FLASH_W25Q64                    0xEF16

//flash默认型号
#define SPI_FLASH_TYPE                  FLASH_W25Q80

#if (SPI_FLASH_TYPE == FLASH_W25Q80)//1M

#define W25X_FLASH_PAGE_SIZE	        256         //页大小
#define W25X_FLASH_SECTOR_SIZE		    4096        //扇区大小
#define FLASH_SECTOR_COUNT		        256         //扇区个数
#define W25Qx_MAX_ADDR                  0x0FFFFF    //最大地址
#define FLASH_PAGES_PER_SECTOR          (W25X_FLASH_SECTOR_SIZE/W25X_FLASH_PAGE_SIZE)//一个扇区的page个数

#elif ((SPI_FLASH_TYPE == FLASH_W25Q16))//2M

#elif ((SPI_FLASH_TYPE == FLASH_W25Q32))//4M

#elif ((SPI_FLASH_TYPE == FLASH_W25Q64))//8M

#endif

/****************************************
	W25Qx命令
****************************************/
#define W25X_MODE_RESET                 0xFF
#define W25X_WriteEnable			    0x06  /**   打开写使能        */
#define W25X_WriteDisable			    0x04  /**   关闭写使能        */
#define W25X_ReadStatusReg_05		    0x05  /**   读状态寄存器      */
#define W25X_ReadStatusReg_35           0x35  /**   读状态寄存器      */
#define W25X_WriteStatusReg			    0x01  /**   写状态寄存器      */
#define W25X_ReadData				    0x03  /**   读数据            */
#define W25X_FastReadData			    0x0B  /**   fast read         */
#define W25X_FastReadDual			    0x3B  /**Fast Read Dual Output*/
#define W25X_PageProgram			    0x02  /**   页编程            */
#define W25X_QuadProgram			    0x32  /**                     */
#define W25X_ERASE_64K				    0xD8  /**    擦除64K          */
#define W25X_ERASE_32K                  0x52  /**    擦除32K          */
#define W25X_ERASE_4K		    	    0x20  /**    擦除4K           */
#define W25X_ERASE_FULL				    0x60  /**    全部擦除         */
#define W25X_SetPowerDown			    0xB9  /**   power down        */
#define W25X_SetReleasePowerDown	    0xAB  /**  Release Power down or HPM / Device ID*/
#define W25X_ManufactDeviceID		    0x90  /**   读设备制造商ID   */
#define W25X_JedecDeviceID			    0x9F  /**   JEDEC ID */

/**  enable or disable CS pin*/
#define ENABLE_CS()                     GPIO_ResetBits(HT_GPIOC, GPIO_Pin_7)
#define DISABLE_CS()                    GPIO_SetBits(HT_GPIOC, GPIO_Pin_7)

uint8_t W25Q80_Init(void);
uint16_t W25Qx_ReadID(void);
void W25Qx_Write_Enable(void);
void W25Qx_Write_Disable(void);
uint8_t W25Qx_Read_StatusReg1(void);
uint8_t W25Qx_Read_StatusReg2(void);
void W25QX_Write_StatusReg(uint8_t reg1,uint8_t reg2);
void W25Qx_Wait_Busy(void);
void W25Qx_Erase_Sector(uint32_t nSector);
void W25Qx_Erase_Block(uint32_t nSector);
void W25Qx_Erase_Chip(void);
uint8_t W25Qx_Write_Byte(uint32_t Addr, uint8_t Txdata);
uint8_t W25Qx_WritePage(uint32_t nDest,uint8_t* pBuffer, uint16_t nBytes);
void W25Qx_Read_Sector(uint32_t nSector, uint8_t* RxData);
uint8_t W25Qx_Read_Bytes(uint32_t Addr, uint8_t* RxData, uint16_t nBytes);
uint8_t W25Q80_Test(void);
uint32_t W25Qx_Wait_Busy_Noblock(void);
int FlashWriteOrder(void *pTradeRecord, uint16_t size);
void OrderReport(void);

#endif // __SPI_W25QX_H__


