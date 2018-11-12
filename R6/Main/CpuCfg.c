/*CpuCfg.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "includes.h"
#include "CpuCfg.h"
#include "flash.h"
#include "server.h"


SYS_UPDATE_INFO_t updateInfo;

//******************************************************************
//! \brief  	delay_ms
//! \param  	ms
//! \retval
//! \note
//******************************************************************
void delay_ms(uint16_t ms)
{
	uint32_t	i;
    volatile int cnt;

	while (ms--) {
		for (i=0; i<650; i++) {
			cnt++;
			cnt++;
			cnt++;
			cnt++;
		}
	}
}

//******************************************************************
//! \brief  	delay_us
//! \param
//! \retval
//! \note
//******************************************************************
void delay_us(uint32_t count)
{
	__IO uint32_t i = 0;
	__IO uint32_t j = 0;

	for(i=0; i<count; i++) {
		//todo
		for(j=0; j<5; j++);
	}
}


//******************************************************************
//! \brief  	BootSetReqInfo
//! \param
//! \retval
//! \note		写入升级请求消息
//******************************************************************
int BootSetReqInfo(void)
{
	HT_Flash_PageErase(SysUpInfoAddr);
	HT_Flash_ByteWrite((uint8_t*)&updateInfo, SysUpInfoAddr, sizeof(updateInfo));
	return 0;
}


//******************************************************************
//! \brief  	BootClrReqInfo
//! \param
//! \retval
//! \note		清除升级请求消息
//******************************************************************
int BootClrReqInfo(void)
{
	HT_Flash_PageErase(SysUpInfoAddr);
	return 0;
}


void HardFault_Handler(void)
{
    printf("Hard Fault\n");

  /* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
		//printf("Hard Fault\n");
		//delay_ms(200);
	}
}


