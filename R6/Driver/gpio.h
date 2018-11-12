/*gpio.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __GPIO_H__
#define __GPIO_H__

#include "includes.h"

/*
* @brief  GPIO�ܽŶ���
*/
#define GPIO_Pin_0                 ((uint16_t)0x0001)     /*!< Pin 0 selected    */
#define GPIO_Pin_1                 ((uint16_t)0x0002)     /*!< Pin 1 selected    */
#define GPIO_Pin_2                 ((uint16_t)0x0004)     /*!< Pin 2 selected    */
#define GPIO_Pin_3                 ((uint16_t)0x0008)     /*!< Pin 3 selected    */
#define GPIO_Pin_4                 ((uint16_t)0x0010)     /*!< Pin 4 selected    */
#define GPIO_Pin_5                 ((uint16_t)0x0020)     /*!< Pin 5 selected    */
#define GPIO_Pin_6                 ((uint16_t)0x0040)     /*!< Pin 6 selected    */
#define GPIO_Pin_7                 ((uint16_t)0x0080)     /*!< Pin 7 selected    */
#define GPIO_Pin_8                 ((uint16_t)0x0100)     /*!< Pin 8 selected    */
#define GPIO_Pin_9                 ((uint16_t)0x0200)     /*!< Pin 9 selected    */
#define GPIO_Pin_10                ((uint16_t)0x0400)     /*!< Pin 10 selected   */
#define GPIO_Pin_11                ((uint16_t)0x0800)     /*!< Pin 11 selected   */
#define GPIO_Pin_12                ((uint16_t)0x1000)     /*!< Pin 12 selected   */
#define GPIO_Pin_13                ((uint16_t)0x2000)     /*!< Pin 13 selected   */
#define GPIO_Pin_14                ((uint16_t)0x4000)     /*!< Pin 14 selected   */
#define GPIO_Pin_15                ((uint16_t)0x8000)     /*!< Pin 15 selected   */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)     /*!< All pins selected */

#define  GPIO_Mode_AFx            0x10                      /*!< GPIO���ù��ܱ�ʹ��             */

//typedef enum{
//	PIN_INPUT = 0,
//	PIN_OUTPUT = 1,
//}PIN_GPIO_DIR;

//typedef enum{
//	PIN_FUNC_GPIO = 0,
//	PIN_FUNC_AFIO = 1,
//}PIN_FUNC_CONFIG;

//typedef enum{
//	AFIO_FUNC1 = 0,
//	AFIO_FUNC2 = 1,
//}AFIO_FUNC;


/*
* @brief  GPIO�ܽ�ģʽ����
*/
typedef enum
{
    GPIO_Mode_IOIN  = 0x00,       /*!< GPIO����                      */
    GPIO_Mode_IOOUT = 0x01,       /*!< GPIO���                      */
    GPIO_Mode_AF1 = 0x10,         /*!< ���ù���1                     */
    GPIO_Mode_AF2 = 0x11          /*!< ���ù���2                     */
}GPIOMode_TypeDef;                /*!<end of group GPIOMode_TypeDef  */

/*
* @brief  GPIO����������������
*/
typedef enum
{
    GPIO_Input_Up   = 0x00,      /*!< GPIO������������ʹ��           */
    GPIO_Input_Floating = 0x01,  /*!< GPIO�������������ر�           */
}GPIOInput_TypeDef;              /*!< end of group GPIOInput_TypeDef */

/*
* @brief  GPIO���������©����
*/
typedef enum
{
    GPIO_Output_OD = 0x00,       /*!< GPIO���ֿ�©���               */
    GPIO_Output_PP = 0x01,       /*!< GPIO�����������               */
}GPIOOutput_TypeDef;


/*
* @brief  GPIO��ʼ���ṹ����
*/
typedef struct
{
    uint16_t GPIO_Pin;                    /*!< GPIO�ܽ�                     */

    GPIOMode_TypeDef GPIO_Mode;           /*!< GPIOģʽ                     */

    GPIOInput_TypeDef GPIO_InputStruct;   /*!< GPIO��������������������     */

    GPIOOutput_TypeDef GPIO_OutputStruct; /*!< GPIO���������©��������     */

}GPIO_InitTypeDef;                        /*!< end of group GPIO_InitTypeDef*/




void GPIO_Cfg(HT_GPIO_TypeDef* GPIOx, uint32_t Gpio_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOInput_TypeDef GPIO_Input, GPIOOutput_TypeDef GPIO_Output);

uint8_t GPIO_ReadInputDataBit(HT_GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void GPIO_SetBits(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Toggle(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);



#endif //__GPIO_H__




