/*gpio.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __GPIO_H__
#define __GPIO_H__

#include "includes.h"

/*
* @brief  GPIO管脚定义
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

#define  GPIO_Mode_AFx            0x10                      /*!< GPIO复用功能被使用             */

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
* @brief  GPIO管脚模式定义
*/
typedef enum
{
    GPIO_Mode_IOIN  = 0x00,       /*!< GPIO输入                      */
    GPIO_Mode_IOOUT = 0x01,       /*!< GPIO输出                      */
    GPIO_Mode_AF1 = 0x10,         /*!< 复用功能1                     */
    GPIO_Mode_AF2 = 0x11          /*!< 复用功能2                     */
}GPIOMode_TypeDef;                /*!<end of group GPIOMode_TypeDef  */

/*
* @brief  GPIO数字输入上拉定义
*/
typedef enum
{
    GPIO_Input_Up   = 0x00,      /*!< GPIO数字输入上拉使能           */
    GPIO_Input_Floating = 0x01,  /*!< GPIO数字输入上拉关闭           */
}GPIOInput_TypeDef;              /*!< end of group GPIOInput_TypeDef */

/*
* @brief  GPIO数字输出开漏定义
*/
typedef enum
{
    GPIO_Output_OD = 0x00,       /*!< GPIO数字开漏输出               */
    GPIO_Output_PP = 0x01,       /*!< GPIO数字推挽输出               */
}GPIOOutput_TypeDef;


/*
* @brief  GPIO初始化结构定义
*/
typedef struct
{
    uint16_t GPIO_Pin;                    /*!< GPIO管脚                     */

    GPIOMode_TypeDef GPIO_Mode;           /*!< GPIO模式                     */

    GPIOInput_TypeDef GPIO_InputStruct;   /*!< GPIO数字输入上拉功能配置     */

    GPIOOutput_TypeDef GPIO_OutputStruct; /*!< GPIO数字输出开漏功能配置     */

}GPIO_InitTypeDef;                        /*!< end of group GPIO_InitTypeDef*/




void GPIO_Cfg(HT_GPIO_TypeDef* GPIOx, uint32_t Gpio_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOInput_TypeDef GPIO_Input, GPIOOutput_TypeDef GPIO_Output);

uint8_t GPIO_ReadInputDataBit(HT_GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void GPIO_SetBits(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Toggle(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);



#endif //__GPIO_H__




