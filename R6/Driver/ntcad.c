

#include "includes.h"
#include "ntcad.h"
#include "gpio.h"


typedef enum {
    PIN_0 = 0x0001,   /*!< Pin 0 selected    */
    PIN_1 = 0x0002,    /*!< Pin 1 selected    */
    PIN_2 = 0x0004,    /*!< Pin 2 selected    */
    PIN_3 = 0x0008,    /*!< Pin 3 selected    */
    PIN_4 = 0x0010,  /*!< Pin 4 selected    */
    PIN_5 = 0x0020, /*!< Pin 5 selected    */
    PIN_6 = 0x0040,  /*!< Pin 6 selected    */
    PIN_7 = 0x0080,    /*!< Pin 7 selected    */
    PIN_8 = 0x0100,    /*!< Pin 8 selected    */
    PIN_9 = 0x0200,    /*!< Pin 9 selected    */
    PIN_10 = 0x0400,    /*!< Pin 10 selected   */
    PIN_11 = 0x0800,   /*!< Pin 11 selected   */
    PIN_12 = 0x1000,  /*!< Pin 12 selected   */
    PIN_13 = 0x2000,  /*!< Pin 13 selected   */
    PIN_14 = 0x4000,    /*!< Pin 14 selected   */
    PIN_15 = 0x8000,    /*!< Pin 15 selected   */
    PIN_ALL = 0xFFFF   /*!< All pins selected */
} GPIONUM_TypeDef;




void TBS_IRQHandler()
{

}


#if 0
int  initAdc(void)//adcin3 G2 AF2
{
		EnWr_WPREG();

		HT_TBS->TBSCON = (0x01<<15) |(0x19<<10) | (3<<8) | (2<<5) | (1);//adc3en default  filter采样次数8  default
		DisWr_WPREG();
		return 0;
}
#endif

int InitTmp(void)//adcin3 G2 AF2
{
    EnWr_WPREG();
    GPIO_Cfg(HT_GPIOG, PIN_2,GPIO_Mode_AF2,GPIO_Input_Up,GPIO_Output_PP);
    HT_TBS->TBSCON = (0x01<<15) |(0x19<<10) | (3<<8) | (2<<5) | (1);//adc disable default  filter采样次数8  default
    HT_TBS->TBSIE =  0x00;
    HT_TBS->TBSPRD = 0;//采样周期
    DisWr_WPREG();
    return 0;
}


int GetCpuTemp(void)
{
	short temp;
	temp = 13 - (short)HT_TBS->TMPDAT * 0.0028;
	//printf("TMPDAT=%d,temp===%d.\n",(short)HT_TBS->TMPDAT,temp);
	return temp;
}


