/*
*********************************************************************************************************
*                                 			   HT60XX
*                            			      HT60XX³ÌÐò
*
*                       			Copyright 2013, Hi-Trend Tech, Corp.
*                             			 All Rights Reserved
*
*
* Project      : HT60xx
* File         : ht60xx_it.c
* By           : SocTeam
* Version      : V0.4
* Description  :
*********************************************************************************************************
*/

#define  __HT60XX_IT_C

#include "ht60xx_it.h"
#include "flash.h"
#include "update.h"


//void Reset_Handler()
//{
//    void (*pFunction)(void);

//    uint32_t address;

//    HT_Flash_WordRead(&address, AppFlashAddr+4*1,1);

//	pFunction = (void (*)(void))address;
//	pFunction();//Jump t
//}



void NMI_Handler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*2,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void HardFault_Handler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*3,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void SVC_Handler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*11,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void PendSV_Handler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*14,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void SysTick_Handler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*15,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void PMU_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*16,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void AES_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*17,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void EXTI0_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*18,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void EXTI1_IRQHandler()
{
     void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*19,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void EXTI2_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*20,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void EXTI3_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*21,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void EXTI4_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*22,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void EXTI5_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*23,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void EXTI6_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*24,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void UART0_IRQHandler()
{

    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*25,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}


extern __IO uint8_t UART_SendState[7];
void UART1_IRQHandler()
{
    void (*pFunction)(void);
    uint32_t address;

	uint8_t ch= ch;

    HT_Flash_WordRead(&address, AppFlashAddr+4*26,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

//	//send interrupt
//	if((HT_UART1->UARTSTA & 0x01)!=0){
//		HT_UART1->UARTSTA &= 0x06;
//
//		UART_SendState[1] = 0;
//	}
}

void UART2_IRQHandler()
{
     void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*27,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void UART3_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*28,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}

void UART4_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*29,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void UART5_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*30,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t
}


void TIMER_0_IRQHandler()
{

     void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*31,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void TIMER_1_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*32,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t


}

void TIMER_2_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*33,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t


}

void TIMER_3_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*34,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void TBS_IRQHandler()
{

    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*35,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t



}

void RTC_IRQHandler()
{
     void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*36,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t


}

void I2C_IRQHandler()
{
     void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*37,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void SPI0_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*38,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t


}

void SPI1_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*39,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void SelfTestF_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+40,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void TIMER_4_IRQHandler()
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*41,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void TIMER_5_IRQHandler(void)
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*42,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void UART6_IRQHandler(void)
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*43,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}

void EXTI7_IRQHandler(void)
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*44,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}


void EXTI8_IRQHandler(void)
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*45,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}


void EXTI9_IRQHandler(void)
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*46,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}


void DMA_IRQHandler(void)
{
    void (*pFunction)(void);

    uint32_t address;

    HT_Flash_WordRead(&address, AppFlashAddr+4*47,1);

	pFunction = (void (*)(void))address;
	pFunction();//Jump t

}


