#include "timer.h"
#include "ht6xxx.h"


void Timer0_Init(void)
{
	HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL1 |= 0x0001; //��ʱ��0ʹ��
	HT_TMR0->TMRCON |= 0x07;	//���ڶ�ʱ��������ʹ��
	HT_TMR0->TMRDIV = 19660-1;//Ԥ��Ƶ��
	HT_TMR0->TMRPRD = 1000;		//��װֵ  1000ms
	
	HT_TMR0->TMRIE = 1;
	NVIC_EnableIRQ(TIMER_0_IRQn);
}
