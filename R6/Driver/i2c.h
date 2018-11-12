/*I2C.h
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __I2C_H__
#define __I2C_H__

#include "includes.h"




void I2C_Init(void);
uint8_t GetKeyHappenEvent(void);
void ClrKeyHappenEvent(void);
uint8_t I2C_ReadNByte(uint8_t slaveAddr, uint8_t subAddrType, uint32_t subAddr, uint8_t *rbuf, uint32_t len);
uint8_t I2C_WriteNByte(uint8_t slaveAddr, uint8_t subAddrType, uint32_t subAddr, uint8_t *wBuf, uint32_t len);

#endif //__I2C_H__











