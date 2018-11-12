/*CpuCfg.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __CPUCFG_H__
#define __CPUCFG_H__

#include "include.h"

#define EnWr_WPREG()		HT_CMU->WPREG = 0xA55A			//则关闭写保护功能，用户可以写操作被保护的寄存器
#define DisWr_WPREG()		HT_CMU->WPREG = 0x0000			//则开启写保护功能，用户禁止写操作被保护的寄存器

#define Debug_En_WDT()		HT_CMU->CLKCTRL1|=0x4000		//调试模式下使能看门狗功能
#define Debug_Dis_WDT()		HT_CMU->CLKCTRL1&=~0x4000		//调试模式下关闭看门狗功能


#define	Feed_WDT64mS()		HT_WDT->WDTCLR = 0xAA00		//FeedWDT per 64ms
#define	Feed_WDT()			HT_WDT->WDTCLR = 0xAA3F		//FeedWDT per 4s: 64ms*(1+SET[0..7])
#define	Feed_WDT16S()		HT_WDT->WDTCLR = 0xAAFF		//FeedWDT per 16s	




void delay_ms(uint16_t ms);


#endif //__CPUCFG_H__

