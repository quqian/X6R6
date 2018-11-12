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
* ����˵��: ����GPIO����һ�γ�ʼ��ͬһ��GPIO�ж��Pin������GPIO_Mode����һ��
*
* ��ڲ���: GPIOx              ֻ����HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE�е�һ��
*
*           GPIO_InitStruct    GPIO�ܽų�ʼ���ṹ��ָ�룬��Ҫ����4������:
*                              1) GPIO_Pin         : ����ʼ���ܽ�:  GPIO_Pin_0 ~ GPIO_Pin_15
*                              2) GPIO_Mode        : GPIOģʽ��Ϊ��������ģʽ֮һ��
*                                                    GPIO_Mode_IOIN   IO����
*                                                    GPIO_Mode_IOOUT  IO���
*                                                    GPIO_Mode_AF1    ���ù���1
*                                                    GPIO_Mode_AF2    ���ù���2
*                              3) GPIO_InputStruct : ��������ʱ������������
*                              4) GPIO_OutputStruct: �������ʱ��©��������
*
* ���ز���: ��
*
* ����˵��: 1)����ģ�⹦�ܱ���ADCIN/Segment/Com�����ܽ�����Ϊ��Щ����ʱ���������������Լ������©������
*             ����
*           2)�û�Ӧ��֤�ں���ִ�й����мĴ���д����״̬��Ҫ���ı�
*********************************************************************************************************
*/
void HT_GPIO_Init(HT_GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    /*  assert_param  */

    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01]; /*!< ���浱ǰд����״̬        */

    HT_CMU->WPREG = CMU_WPREG_UnProtected;                              /*!< �ر�д��������            */

    if(GPIO_InitStruct->GPIO_Mode & GPIO_Mode_AFx)                      /*!< �ж�GPIO�Ƿ���Ϊ����Pin */
    {
        GPIOx->IOCFG |= GPIO_InitStruct->GPIO_Pin;                      /*!< ����GPIOΪ���ù���Pin     */

        if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF1)                 /*!< �ж��Ƿ�Ϊ��һ���ù���    */
        {
            GPIOx->AFCFG &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);     /*!< ����Ϊ��һ���ù���        */
        }
        else
        {
            GPIOx->AFCFG |= (uint32_t)GPIO_InitStruct->GPIO_Pin;        /*!< ����Ϊ�ڶ����ù���        */
        }
    }
    else
    {
        GPIOx->IOCFG &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< ����GPIOΪIO              */

        if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IOIN)                /*!< �ж��Ƿ�ΪGPIO����        */
        {
            GPIOx->PTDIR &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);     /*!< ����ΪIO����              */
        }
        else
        {
            GPIOx->PTDIR |= (uint32_t)GPIO_InitStruct->GPIO_Pin;        /*!< ����ΪIO���              */
        }
    }


    if(GPIO_InitStruct->GPIO_InputStruct == GPIO_Input_Up)              /*!< �ж��Ƿ�ʹ��������������  */
    {
        GPIOx->PTUP &=  ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< ����������������          */
    }
    else
    {
        GPIOx->PTUP |=  (uint32_t)GPIO_InitStruct->GPIO_Pin;            /*!< �ر�������������          */
    }

    if(GPIO_InitStruct->GPIO_OutputStruct == GPIO_Output_OD)            /*!< �ж��Ƿ�ʹ�ܿ�©�������  */
    {
        GPIOx->PTOD &=  ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< ���ÿ�©���              */
    }
    else
    {
        GPIOx->PTOD |=  (uint32_t)GPIO_InitStruct->GPIO_Pin;            /*!< �����������              */
    }

    HT_CMU->WPREG = writeProtect;                                       /*!< �ָ�֮ǰд��������        */
}


//******************************************************************
//! \brief  	GPIO_ReadInputDataBit
//! \param  	none
//! \retval
//! \note   	GPIO��ʼ��
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
//! \note   	��ȡGPIO״̬
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
//! \note   	����GPIOΪ�ߵ�ƽ
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
//! \note   	����GPIOΪ�͵�ƽ
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
//! \note   	GPIO��ת
//! \note
//******************************************************************
void GPIO_Toggle(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->PTTOG |= GPIO_Pin;
}

