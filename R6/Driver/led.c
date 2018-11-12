/*gpio.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "includes.h"
#include "led.h"



int LedInit(void)
{
    GPIO_Cfg(HT_GPIOD, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
    return CL_OK;
}


