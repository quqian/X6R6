/*
*********************************************************************************************************
*                                              HT6XXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_gpio.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_GPIO_C

#include "ht6xxx_gpio.h"

/*
*********************************************************************************************************
*                                           ���غ�/�ṹ��
*********************************************************************************************************
*/

#define  GPIO_Mode_AFx            0x10                      /*!< GPIO���ù��ܱ�ʹ��             */

/*
*********************************************************************************************************
*                                             ���ر���
*********************************************************************************************************
*/

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};
/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/


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


/*
*********************************************************************************************************
*                         INITIAL GPIO TO SPECIFIC ALTERNATE FUNCTION
*
* ����˵��: ����GPIO����һ�γ�ʼ��ͬһ��GPIO�ж��Pin��ÿ��Pin GPIO_Mode���Բ�һ��
*
* ��ڲ���: GPIOx              ֻ����HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE�е�һ��
*
*           GPIO_InitAFStruct  GPIO�ܽų�ʼ���ṹ��ָ�룬��Ҫ����4������: 
*                              1) GPIO_Pin         : ����ʼ���ܽ�:  GPIO_Pin_0 ~ GPIO_Pin_15
*                              2) GPIO_AFMode      : GPIOģʽ�����ù��ܣ�����Ϊ����ĳһ�鲻ͬPin�ĸ��ù������
*                                                    ע������ֻ�г���GPIOA��GPIOE
*                                                    GPIOB/GPIOC/GPIOD ���ù��ܶ���μ�ht60xx_gpio.h
*                                                    PA0_SEG28           PE0_TMR0  
*                                                    PA1_SEG29           PE1_TX4   
*                                                    PA2_SEG30           PE1_TOUT2 
*                                                    PA3_SEG31           PE2_RX4   
*                                                    PA4_SEG32           PE2_ESAMIO
*                                                    PA5_INT0            PE3_CLKOUT
*                                                    PA6_INT1            PE4_RX3   
*                                                    PA7_INT2            PE4_CARDIO
*                                                    PA7_SEG33           PE5_TX3   
*                                                    PA8_INT3            PE6_TMR1  
*                                                    PA8_SEG34           PE7_LVDIN 
*                                                    PA9_INT4            PE8_POWIN 
*                                                    PA10_INT5           PE9_TMR2  
*                                                    PA11_INT6           PE10_TMR3 
*                                                    PA12_SEG35          PE11_TX2  
*                                                    PA12_ADCIN0         PE12_RX2  
*                                                    PA13_SEG36          PE13_SCL  
*                                                    PA13_ADCIN1         PE14_SDA    
*
*                              3) GPIO_InputStruct : ��������ʱ������������ 
*                              4) GPIO_OutputStruct: �������ʱ��©��������
*
* ���ز���: ��                                      
* 
* ����˵��: 1)���б���ʼ���ĹܽŶ�������ʼ��Ϊ���ù���Pin
*           2)�û�Ӧ��֤GPIO_Pin��GPIO_AFMode���Ӧ����GPIOx=HT_GPIOAΪ�������GPIO_Pin����GPIO_Pin_0����
              GPIO_AFMode��ҲӦ��Ӧ����PA0_SEG28�Ŷ�
*           3)����ģ�⹦�ܱ���ADCIN/Segment/Com�����ܽ�����Ϊ��Щ����ʱ���������������Լ������©������
*             ����
*           4)�û�Ӧ��֤�ں���ִ�й����мĴ���д����״̬��Ҫ���ı�
*********************************************************************************************************
*/
void HT_GPIO_AFInit(HT_GPIO_TypeDef* GPIOx, GPIO_InitAFTypeDef* GPIO_InitAFStruct)
{
    /*  assert_param  */
    
    uint32_t AFState;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01]; /*!< ���浱ǰд����״̬        */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                              /*!< �ر�д��������            */
    
    GPIOx->IOCFG |= GPIO_InitAFStruct->GPIO_Pin;                        /*!< ����Ϊ���ùܽ�            */
    
    AFState = GPIOx->AFCFG & (~(uint32_t)GPIO_InitAFStruct->GPIO_Pin);  /*!< ����δ���ùܽŸ���״̬    */
    
    GPIOx->AFCFG = (uint32_t)GPIO_InitAFStruct->GPIO_AFMode | AFState;  /*!< ������Ӧ�ܽŸ��ù���      */
    
    if(GPIO_InitAFStruct->GPIO_InputStruct == GPIO_Input_Up)            /*!< �ж��Ƿ�ʹ��������������  */
    {
        GPIOx->PTUP &=  ~((uint32_t)GPIO_InitAFStruct->GPIO_Pin);       /*!< ����������������          */
    }
    else
    {
        GPIOx->PTUP |=  (uint32_t)GPIO_InitAFStruct->GPIO_Pin;          /*!< �ر�������������          */
    }
    
    if(GPIO_InitAFStruct->GPIO_OutputStruct == GPIO_Output_OD)          /*!< �ж��Ƿ�ʹ�ܿ�©�������  */
    {
        GPIOx->PTOD &=  ~((uint32_t)GPIO_InitAFStruct->GPIO_Pin);       /*!< ���ÿ�©���              */
    }
    else
    {
        GPIOx->PTOD |=  (uint32_t)GPIO_InitAFStruct->GPIO_Pin;          /*!< �����������              */
    } 

    HT_CMU->WPREG = writeProtect;                                       /*!< �ָ�֮ǰд��������        */    
} 

/*
*********************************************************************************************************
*                                          SET GPIO
*
* ����˵��: ʹIO����ܽ�����ߵ�ƽ���Ը��ù���Pin�Լ�IO������Ч
*
* ��ڲ���: GPIOx       ֻ����HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE�е�һ��
*
*           GPIO_Pin    GPIO�ܽţ�GPIO_Pin_0 ~ GPIO_Pin_15
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_GPIO_BitsSet(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    GPIOx->PTSET = (uint32_t)GPIO_Pin;                      /*!< ʹ��ӦIO�����         */  
} 

/*
*********************************************************************************************************
*                                          RESET GPIO
*
* ����˵��: ʹIO����ܽ�����͵�ƽ���Ը��ù���Pin�Լ�IO������Ч
*
* ��ڲ���: GPIOx       ֻ����HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE�е�һ��
*
*           GPIO_Pin    GPIO�ܽţ�GPIO_Pin_0 ~ GPIO_Pin_15
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_GPIO_BitsReset(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    GPIOx->PTCLR = (uint32_t)GPIO_Pin;                      /*!< ʹ��ӦIO�����         */  
} 

/*
*********************************************************************************************************
*                                         TOGGLE GPIO
*
* ����˵��: ʹIO����ܽ������ƽ��ת���Ը��ù���Pin�Լ�IO������Ч
*
* ��ڲ���: GPIOx       ֻ����HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE�е�һ��
*
*           GPIO_Pin    GPIO�ܽţ�GPIO_Pin_0 ~ GPIO_Pin_15
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_GPIO_BitsToggle(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    GPIOx->PTTOG = (uint32_t)GPIO_Pin;                      /*!< ��ת��ӦIO�ڵ�ƽ       */  
} 

/*
*********************************************************************************************************
*                                      READ GPIO BITS
*
* ����˵��: ���ض��˿ڵ�Pin��״̬
*
* ��ڲ���: GPIOx       ֻ����HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE�е�һ��
*
*           GPIO_Pin    GPIO�ܽţ�GPIO_Pin_0 ~ GPIO_Pin_15
*                       ��GPIO_Pin = GPIO_Pin_All ʱ��Ϊ�������˿�IO����Pinֵ
*
* ���ز���: �˿�����Pin��״̬                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
uint16_t HT_GPIO_BitsRead(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    return (uint16_t)(GPIOx->PTDAT & GPIO_Pin);             /*!< ����ӦIO�ڵ�ƽ         */  
} 

/*
*********************************************************************************************************
*                                 SET GPIO ON HIGH CURRENT MODE 
*
* ����˵��: ���ò���GPIO�������ģʽ
*
* ��ڲ���: GPIO_Pin    �����ģʽ����Bit������Ϊ���²����������:
*                         @arg  GPIO_HDPORT_PC0
*                         @arg  GPIO_HDPORT_PA6
*                         @arg  GPIO_HDPORT_PA7
*                         @arg  GPIO_HDPORT_PA8
*
*           NewState    = ENABLE�� ʹ�ܴ����ģʽ
*                       = DISABLE����ͨ����ģʽ
*
* ���ز���: ��                                     
* 
* ����˵��: ֻ��PC0/PA6/PA7/PA8��Ч
*********************************************************************************************************
*/
void HT_GPIO_HighCurrentSet(uint16_t GPIO_Pin, FunctionalState NewState)
{
    /*  assert_param  */
        
    if (NewState != DISABLE)
    {       
        HT_GPIOHDPORT |= (uint32_t)(GPIO_Pin & GPIO_HDPORT);    /*!< ʹ��GPIO�����ģʽ     */ 
    }
    else
    {
        HT_GPIOHDPORT &= ~(uint32_t)(GPIO_Pin & GPIO_HDPORT);  /*!< �ر�GPIO�����ģʽ     */ 
    }   
    
} 


