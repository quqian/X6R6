/*CpuCfg.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "include.h"


//******************************************************************
//! \brief  	Delay_mSec( software delay ms function )
//! \param  	delay time
//! \retval 	none
//! \note   	
//! \note  
//******************************************************************
void delay_ms(uint16_t ms)
{
	uint32_t	i;
	while (ms--)
	{
		for (i=0; i<2250; i++) 
		{
			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	}
}
























