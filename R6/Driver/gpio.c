/*gpio.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "includes.h"



static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

/*
*********************************************************************************************************
*                                         INITIAL THE GPIO
*
* 函数说明: 配置GPIO，可一次初始化同一组GPIO中多个Pin，但其GPIO_Mode必须一致
*
* 入口参数: GPIOx              只能是HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE中的一个
*
*           GPIO_InitStruct    GPIO管脚初始化结构体指针，主要包含4个参数:
*                              1) GPIO_Pin         : 被初始化管脚:  GPIO_Pin_0 ~ GPIO_Pin_15
*                              2) GPIO_Mode        : GPIO模式，为以下四种模式之一：
*                                                    GPIO_Mode_IOIN   IO输入
*                                                    GPIO_Mode_IOOUT  IO输出
*                                                    GPIO_Mode_AF1    复用功能1
*                                                    GPIO_Mode_AF2    复用功能2
*                              3) GPIO_InputStruct : 数字输入时上拉功能配置
*                              4) GPIO_OutputStruct: 数字输出时开漏功能配置
*
* 返回参数: 无
*
* 特殊说明: 1)部分模拟功能比如ADCIN/Segment/Com，当管脚配置为这些功能时，配置输入上拉以及输出开漏功能无
*             意义
*           2)用户应保证在函数执行过程中寄存器写保护状态不要被改变
*********************************************************************************************************
*/
void HT_GPIO_Init(HT_GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    /*  assert_param  */

    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01]; /*!< 保存当前写保护状态        */

    HT_CMU->WPREG = CMU_WPREG_UnProtected;                              /*!< 关闭写保护功能            */

    if(GPIO_InitStruct->GPIO_Mode & GPIO_Mode_AFx)                      /*!< 判断GPIO是否复用为功能Pin */
    {
        GPIOx->IOCFG |= GPIO_InitStruct->GPIO_Pin;                      /*!< 配置GPIO为复用功能Pin     */

        if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF1)                 /*!< 判断是否为第一复用功能    */
        {
            GPIOx->AFCFG &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);     /*!< 配置为第一复用功能        */
        }
        else
        {
            GPIOx->AFCFG |= (uint32_t)GPIO_InitStruct->GPIO_Pin;        /*!< 配置为第二复用功能        */
        }
    }
    else
    {
        GPIOx->IOCFG &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< 配置GPIO为IO              */

        if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IOIN)                /*!< 判断是否为GPIO输入        */
        {
            GPIOx->PTDIR &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);     /*!< 配置为IO输入              */
        }
        else
        {
            GPIOx->PTDIR |= (uint32_t)GPIO_InitStruct->GPIO_Pin;        /*!< 配置为IO输出              */
        }
    }


    if(GPIO_InitStruct->GPIO_InputStruct == GPIO_Input_Up)              /*!< 判断是否使能数字输入上拉  */
    {
        GPIOx->PTUP &=  ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< 配置输入上拉功能          */
    }
    else
    {
        GPIOx->PTUP |=  (uint32_t)GPIO_InitStruct->GPIO_Pin;            /*!< 关闭输入上拉功能          */
    }

    if(GPIO_InitStruct->GPIO_OutputStruct == GPIO_Output_OD)            /*!< 判断是否使能开漏输出功能  */
    {
        GPIOx->PTOD &=  ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< 配置开漏输出              */
    }
    else
    {
        GPIOx->PTOD |=  (uint32_t)GPIO_InitStruct->GPIO_Pin;            /*!< 配置推挽输出              */
    }

    HT_CMU->WPREG = writeProtect;                                       /*!< 恢复之前写保护设置        */
}


//******************************************************************
//! \brief  	GPIO_ReadInputDataBit
//! \param  	none
//! \retval
//! \note   	GPIO初始化
//! \note
//******************************************************************
void GPIO_Cfg(HT_GPIO_TypeDef* GPIOx, uint32_t Gpio_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOInput_TypeDef GPIO_Input, GPIOOutput_TypeDef GPIO_Output)
{
	GPIO_InitTypeDef  GPIO_Init;

	GPIO_Init.GPIO_Pin = Gpio_Pin;
	GPIO_Init.GPIO_Mode=GPIO_Mode;
	GPIO_Init.GPIO_InputStruct=GPIO_Input;
	GPIO_Init.GPIO_OutputStruct=GPIO_Output;

	HT_GPIO_Init(GPIOx,&GPIO_Init);
}

//******************************************************************
//! \brief  	GPIO_ReadInputDataBit
//! \param  	none
//! \retval
//! \note   	读取GPIO状态
//! \note
//******************************************************************
uint8_t GPIO_ReadInputDataBit(HT_GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;
	if((GPIOx->PTDAT & GPIO_Pin) != 0){
		bitstatus = 1;
	}else{
		bitstatus = 0;
	}

	return bitstatus;
}

//******************************************************************
//! \brief  	GPIO_SetBits
//! \param  	none
//! \retval
//! \note   	设置GPIO为高电平
//! \note
//******************************************************************
void GPIO_SetBits(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->PTSET |= GPIO_Pin;

}
//******************************************************************
//! \brief  	GPIO_ResetBits
//! \param  	none
//! \retval
//! \note   	设置GPIO为低电平
//! \note
//******************************************************************
void GPIO_ResetBits(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->PTCLR |= GPIO_Pin;
}

//******************************************************************
//! \brief  	GPIO_Toggle
//! \param  	none
//! \retval
//! \note   	GPIO翻转
//! \note
//******************************************************************
void GPIO_Toggle(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->PTTOG |= GPIO_Pin;
}

