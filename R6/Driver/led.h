/*gpio.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __LED_H__
#define __LED_H__

#include "includes.h"


#define LED_BLUE_LIGHT_OFF()	  	            GPIO_SetBits(HT_GPIOD,GPIO_Pin_3)
#define LED_BLUE_LIGHT_ON()	  	            	GPIO_ResetBits(HT_GPIOD,GPIO_Pin_3)

#define LED_GREEN_LIGHT_OFF()	  	            GPIO_SetBits(HT_GPIOD,GPIO_Pin_4)
#define LED_GREEN_LIGHT_ON()	  	        	GPIO_ResetBits(HT_GPIOD,GPIO_Pin_4)

#define LED_RED_LIGHT_OFF()	  	            	GPIO_SetBits(HT_GPIOD,GPIO_Pin_5)
#define LED_RED_LIGHT_ON()	  	                GPIO_ResetBits(HT_GPIOD,GPIO_Pin_5)


int LedInit(void);

#endif


