/*flash.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __FLASH_H__
#define __FLASH_H__

#include "includes.h"


extern void HT_Flash_ByteWrite(uint8_t* pWriteByte, uint32_t Address, uint32_t Num);
extern void HT_Flash_HalfWordWrite(uint16_t* pWriteHalfWord, uint32_t Address, uint32_t Num);
extern void HT_Flash_WordWrite(uint32_t* pWriteWord, uint32_t Address, uint32_t Num);
extern void HT_Flash_ByteRead(uint8_t* pReadByte, uint32_t Address, uint32_t Num);
extern void HT_Flash_HalfWordRead(uint16_t* pReadHalfWord, uint32_t Address, uint32_t Num);
extern void HT_Flash_WordRead(uint32_t* pReadWord, uint32_t Address, uint32_t Num);
extern void HT_Flash_PageErase(uint32_t EraseAddress);
extern void HT_Flash_ChipErase(void);

extern int FlashReadGunInfo(void *pGunInfo, uint16_t size);
extern int FlashReadSysInfo(void *pInfo, uint16_t size);
extern int FlashReadSysInfo1(void *pInfo, uint16_t size);
extern int FlashWriteSysInfo(void *pSysInfo, uint16_t size, uint8_t readBack);
extern int FlashWriteSysInfo1(void *pSysInfo, uint16_t size, uint8_t readBack);
extern int FlashWriteGunInfo(void *pGunInfo, uint16_t size, uint8_t readBack);
extern void FlashWriteAppBackup(uint32_t app_backup_record_addr, uint8_t* buffer, uint16_t len);
extern void FlashTestTask(void);
extern void WriteUpdateInfo(uint32_t fsize, uint32_t checkSum);
extern void FlashEraseAppBackup(uint32_t addr, uint32_t size);


#endif // __FLASH_H__


