/*usart.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "includes.h"

static __IO uint32_t TimingDelay=0;


//******************************************************************
//! \brief  	SysTick_Init
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void SysTick_Init(void)
{
	if(SysTick_Config (44040192/1000)){
		while (1);
	}
}










