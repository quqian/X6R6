/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_lcd.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_LCD_H__
#define __HT6XXX_LCD_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
/*
*********************************************************************************************************
*                                           ȫ�ֺ�/�ṹ��
*********************************************************************************************************
*/

/* 
* @brief  LCDƫѹ����ѡ��λ����
*/
typedef enum
{ 
    LCDBias_OneThird = 0x80,               /*!< 1/3 bias ƫѹ����              */
    LCDBias_OneFourth = 0x00,              /*!< 1/4 bias ƫѹ����              */
}LCDBias_TypeDef;                          /*!< end of group LCDBias_TypeDef   */  

/* 
* @brief  LCDCom����
*/
typedef enum
{ 
    LCD_4COM = 0x00,                       /*!< 4Com                           */
    LCD_6COM = 0x08,                       /*!< 6Com                           */
    LCD_8COM = 0x10,                       /*!< 8Com                           */
}LCDCom_TypeDef;                           /*!< end of group LCDCom_TypeDef    */ 

/* 
* @brief  LCDɨ��Ƶ��ѡ��λ����
*/
typedef enum
{ 
    LCDCLK_Div1 = 0x04,                    /*!< LCDɨ��Ƶ�����                */
    LCDCLK_Div2 = 0x00,                    /*!< LCDɨ��Ƶ��/2                  */
    LCDCLK_Div4 = 0x01,                    /*!< LCDɨ��Ƶ��/4                  */
    LCDCLK_Div8 = 0x02,                    /*!< LCDɨ��Ƶ��/8                  */
    LCDCLK_Div16 = 0x03,                   /*!< LCDɨ��Ƶ��/16                 */
}LCDCLK_TypeDef;                           /*!< end of group LCDCLK_TypeDef    */

/* 
* @brief  LCD���ģʽ����
*/
typedef enum
{ 
    LCDCharge_HighCurrent = 0x00,          /*!< LCD��������ģʽ                   */
    LCDCharge_LittleCurrent = 0x10,        /*!< LCDС�������ģʽ                   */
    LCDChargeQuick_PeriodDiv8 = 0x30,      /*!< ���ٳ��ģʽ�����ʱ��1/8��Flcd���� */
    LCDChargeQuick_PeriodDiv16 = 0x70,     /*!< ���ٳ��ģʽ�����ʱ��1/16��Flcd����*/
    LCDChargeQuick_PeriodDiv32 = 0xb0,     /*!< ���ٳ��ģʽ�����ʱ��1/32��Flcd����*/
    LCDChargeQuick_PeriodDiv64 = 0xf0,     /*!< ���ٳ��ģʽ�����ʱ��1/64��Flcd����*/
}LCDChargeMode_TypeDef;                    /*!< end of group LCDChargeMode_TypeDef  */  

/* 
* @brief  LCDģ���ʼ���ṹ����
*/
typedef struct
{
    LCDBias_TypeDef LCD_Bias;              /*!< LCDƫѹ��������                     */
    LCDCom_TypeDef  LCD_Com;               /*!< LCDCom Number����                   */
    LCDCLK_TypeDef  LCD_CLK;               /*!< LCDɨ��Ƶ�ʶ���                     */
    LCDChargeMode_TypeDef LCD_ChargeMode;  /*!< LCD���ģʽ����                     */
    uint16_t LCD_Vrsel;                    /*!< LCD_Vrsel[3..0]: �Աȶ�����         */                                                
}LCD_InitTypeDef;                          /*!< end of group LCD_InitTypeDef        */


/*
*********************************************************************************************************
*                                             ȫ�ֱ���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           ȫ�ֺ�������
*********************************************************************************************************
*/

void HT_LCD_Init(LCD_InitTypeDef* LCD_InitStruct);
void HT_LCD_Write(uint8_t SegNum, uint8_t SegValue);
uint8_t HT_LCD_Read(uint8_t SegNum);





#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_LCD_H__ */ 
