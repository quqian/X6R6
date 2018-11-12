/**
  ******************************************************************************
  * @file    main.c 
  * @author  liutao
  * @version V1.0
  * @date    17-October-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2017 </center></h2>
  ******************************************************************************
  */  
  
#include "include.h"
#include "usart.h"
#include "systick.h"
#include "flash.h"
#include "update.h"


typedef void (*FunVoidType)(void);
extern SYS_UPDATE_INFO_t updateInfo;


//******************************************************************
//! \brief  	SystemClockInit
//! \param  	
//! \retval 	
//! \note   	初始化系统时钟
//******************************************************************
void SystemClockInit(void)
{	
	Feed_WDT();
	EnWr_WPREG();
	HT_CMU->CLKCTRL0 &= ~0x0380;//关闭HRC_DET,PLL_DET,LF_DET
	DisWr_WPREG();
	Feed_WDT();
	
	SwitchTo_Fpll();
	Close_Hrc();
}

//******************************************************************
//! \brief  	main
//! \param  	
//! \retval 	
//! \note   	主函数		
//******************************************************************
int main(void)
{
	//unsigned int  t_msp;
	void (*pFunction)(void);
	uint32_t address;
 
	//SystemClockInit();
	
	HT_Flash_ByteRead((uint8_t *)&updateInfo, AppUpBkpAddr, sizeof(updateInfo));
	if(updateInfo.updateFlag == 0xAA55)
	{
		UpdateFromAppBkp(updateInfo.size,updateInfo.checkSum);
	}
	
	//直接跳转App
	Feed_WDT();
//	t_msp=__get_MSP();
//	HT_Flash_WordWrite(&t_msp, AppFlashAddr, 1);
	HT_Flash_WordRead(&address, AppFlashAddr+4,1);			
	pFunction = (void (*)(void))address;	
	__set_MSP(*(uint32_t*) AppFlashAddr);
	pFunction();//Jump t
	
	while(1);
	
}


















