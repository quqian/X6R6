/*lcd.h
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C" {
#endif 

#include "include.h"

/* 
* @brief  LCD偏压驱动选择位定义
*/
typedef enum
{ 
    LCDBias_OneThird = 0x80,               /*!< 1/3 bias 偏压驱动              */
    LCDBias_OneFourth = 0x00,              /*!< 1/4 bias 偏压驱动              */
}LCDBias_TypeDef;                          /*!< end of group LCDBias_TypeDef   */  

/* 
* @brief  LCDCom定义
*/
typedef enum
{ 
    LCD_4COM = 0x00,                       /*!< 4Com                           */
    LCD_6COM = 0x08,                       /*!< 6Com                           */
    LCD_8COM = 0x10,                       /*!< 8Com                           */
}LCDCom_TypeDef;                           /*!< end of group LCDCom_TypeDef    */ 

/* 
* @brief  LCD扫描频率选择位定义
*/
typedef enum
{ 
    LCDCLK_Div1 = 0x04,                    /*!< LCD扫描频率最高                */
    LCDCLK_Div2 = 0x00,                    /*!< LCD扫描频率/2                  */
    LCDCLK_Div4 = 0x01,                    /*!< LCD扫描频率/4                  */
    LCDCLK_Div8 = 0x02,                    /*!< LCD扫描频率/8                  */
    LCDCLK_Div16 = 0x03,                   /*!< LCD扫描频率/16                 */
}LCDCLK_TypeDef;                           /*!< end of group LCDCLK_TypeDef    */

/* 
* @brief  LCD充电模式定义
*/
typedef enum
{ 
    LCDCharge_HighCurrent = 0x00,          /*!< LCD大电流充电模式                   */
    LCDCharge_LittleCurrent = 0x10,        /*!< LCD小电流充电模式                   */
    LCDChargeQuick_PeriodDiv8 = 0x30,      /*!< 快速充电模式，充电时间1/8个Flcd周期 */
    LCDChargeQuick_PeriodDiv16 = 0x70,     /*!< 快速充电模式，充电时间1/16个Flcd周期*/
    LCDChargeQuick_PeriodDiv32 = 0xb0,     /*!< 快速充电模式，充电时间1/32个Flcd周期*/
    LCDChargeQuick_PeriodDiv64 = 0xf0,     /*!< 快速充电模式，充电时间1/64个Flcd周期*/
}LCDChargeMode_TypeDef;                    /*!< end of group LCDChargeMode_TypeDef  */  

/* 
* @brief  LCD模块初始化结构定义
*/
typedef struct
{
    LCDBias_TypeDef LCD_Bias;              /*!< LCD偏压驱动定义                     */
    LCDCom_TypeDef  LCD_Com;               /*!< LCDCom Number定义                   */
    LCDCLK_TypeDef  LCD_CLK;               /*!< LCD扫描频率定义                     */
    LCDChargeMode_TypeDef LCD_ChargeMode;  /*!< LCD充电模式定义                     */
    uint16_t LCD_Vrsel;                    /*!< LCD_Vrsel[3..0]: 对比度设置         */                                                
}LCD_InitTypeDef;                          /*!< end of group LCD_InitTypeDef        */


//1/4 bias 对比度
typedef enum  
{
	VRsel_OneFourth_P99= 0x0000,     /*!< 99.1%  */
	VRsel_OneFourth_P93= 0x0001,     /*!< 94.0%  */
	VRsel_OneFourth_P88= 0x0010,     /*!< 88.8%  */
	VRsel_OneFourth_P83= 0x0011,     /*!< 84.2%  */
	VRsel_OneFourth_P78= 0x0100,     /*!< 80.0%  */
	VRsel_OneFourth_P74= 0x0101,     /*!< 76.2%  */
	VRsel_OneFourth_P71= 0x0110,     /*!< 72.8%  */
	VRsel_OneFourth_P68= 0x0111,     /*!< 69.6%  */
	VRsel_OneFourth_P65= 0x1000,     /*!< 66.6%  */
	VRsel_OneFourth_P64= 0x1001,     /*!< 64.0%  */
	VRsel_OneFourth_P60= 0x1010,     /*!< 60.0%  */
	VRsel_OneFourth_P59= 0x1011,     /*!< 59.2%  */
	VRsel_OneFourth_P57= 0x1100,     /*!< 57.2%  */
	VRsel_OneFourth_P55= 0x1101,     /*!< 55.2%  */
	VRsel_OneFourth_P53= 0x1110,     /*!< 53.4%  */
	VRsel_OneFourth_P51= 0x1111,     /*!< 51.6%  */
	
}LCD_VRsel_OneFourth_TypeDef;


//1/3 bias 对比度
typedef enum  
{
	VRsel_OneThree_P97= 0x0000,     /*!< 97.4%  */
	VRsel_OneThree_P92= 0x0001,     /*!< 92.4%  */
	VRsel_OneThree_P85= 0x0010,     /*!< 85.8%  */
	VRsel_OneThree_P80= 0x0011,     /*!< 80.0%  */
	VRsel_OneThree_P75= 0x0100,     /*!< 75.0%  */
	VRsel_OneThree_P70= 0x0101,     /*!< 70.6%  */
	VRsel_OneThree_P66= 0x0110,     /*!< 66.6%  */
	VRsel_OneThree_P63= 0x0111,     /*!< 63.2%  */
	VRsel_OneThree_P60= 0x1000,     /*!< 60.0%  */
	VRsel_OneThree_P57= 0x1001,     /*!< 57.2%  */
	VRsel_OneThree_P54= 0x1010,     /*!< 54.6%  */
	VRsel_OneThree_P52= 0x1011,     /*!< 52.2%  */
	VRsel_OneThree_P50= 0x1100,     /*!< 50.0%  */
	VRsel_OneThree_P48= 0x1101,     /*!< 48.0%  */
	VRsel_OneThree_P46= 0x1110,     /*!< 46.2%  */
	VRsel_OneThree_P44= 0x1111,     /*!< 44.4%  */
	
}LCD_VRsel_OneThree_TypeDef;


typedef enum{
	LCD_CLEAR = 0,
	LCD_DISPLAY = 1,
}LCD_Display_TypeDef;

typedef enum{
	SEG0_NOSIGNAL 			= 0x01,	//无信号
	SEG0_SIGNAL   			= 0x02,	//信号
	SEG0_CARD_CHARGING 		= 0x04,//充电中
	SEG0_CARD_BALANCE 		= 0x08,//卡片余额
}SEG0_DISPLAY_t;

typedef enum{
	SEG1_CHARGE_METHOD 	   	= 0x01,	//请刷卡或扫码
	SEG1_CARD_FLAG   		= 0x02,	//卡标志
	SEG1_CARD_NOMONEY 		= 0x04,	//欠费
	SEG1_CARD_INVALID		= 0x08,	//无效
}SEG1_DISPLAY_t;

typedef enum{
	SEG2_1F					= 0x01,	//1F
	SEG2_1G					= 0x02,	//1G
	SEG2_1E 				= 0x04,	//1E
	SEG2_LARGER	  			= 0x08, //大于
}SEG2_DISPLAY_t;

typedef enum{
	SEG3_1A 				= 0x01,//1A
	SEG3_1B   				= 0x02,//1B
	SEG3_1C 				= 0x04,//1C
	SEG3_1D  				= 0x08,//1D
}SEG3_DISPLAY_t;

typedef enum{
	SEG4_2F					 = 0x01,//2F
	SEG4_2G   				 = 0x02,//2G
	SEG4_2E					 = 0x04,//2E
	SEG4_POINT1				 = 0x08,//小数点1
}SEG4_DISPLAY_t;

typedef enum{
	SEG5_2A 				= 0x01,//2A
	SEG5_2B   				= 0x02,//2B
	SEG5_2C 				= 0x04,//2C
	SEG5_2D					= 0x08,//2D
}SEG5_DISPLAY_t;

typedef enum{
	SEG6_3F					= 0x01,//3F
	SEG6_3G 			    = 0x02,//3G
	SEG6_3E					= 0x04,//3E
	SEG6_POINT2			    = 0x08,//小数点2
}SEG6_DISPLAY_t;

typedef enum{
	SEG7_3A					= 0x01,//3A
	SEG7_3B				    = 0x02,//3B
	SEG7_3C				    = 0x04,//3C
	SEG7_3D				    = 0x08,//3D
}SEG7_DISPLAY_t;

typedef enum{
	SEG8_4F 				= 0x01,//4F
	SEG8_4G   				= 0x02,//4G
	SEG8_4E 				= 0x04,//4E
	SEG8_POINT3  			= 0x08,//小数点3
}SEG8_DISPLAY_t;

typedef enum{
	SEG9_SOCKET_SUCCESS  	= 0x01,//成功
	SEG9_SOCKET_OCUUPIED  	= 0x02,//占用
	SEG9_SOCKET_FAULT 		= 0x04,//故障
}SEG9_DISPLAY_t;

typedef enum{
	SEG10_SOCKET_FAILURE 	= 0x01,//失败
	SEG10_SOCKET_FLAG   	= 0x02,//插座标志
	SEG10_SOCKET_INVALID 	= 0x04,//插座无效
}SEG10_DISPLAY_t;

typedef enum{
	SEG11_5A 				= 0x01,//5A
	SEG11_5B   				= 0x02,//5B
	SEG11_5C 				= 0x04,//5C
	SEG11_5D  				= 0x08,//5D
}SEG11_DISPLAY_t;

typedef enum{
	SEG12_5F				= 0x01,//5F
	SEG12_5G				= 0x02,//5G
	SEG12_5E				= 0x04,//5E
	SEG12_POINT4			= 0x08,//小数点4
}SEG12_DISPLAY_t;

typedef enum{
	SEG13_4A				= 0x01,//4A
	SEG13_4B				= 0x02,//4B
	SEG13_4C				= 0x04,//4C
	SEG13_4D				= 0x08,//4D
}SEG13_DISPLAY_t;

typedef enum{
	SEG14_11D				= 0x01,//11D
	SEG14_11C				= 0x02,//11C
	SEG14_11B				= 0x04,//11B
	SEG14_11A				= 0x08,//11A
}SEG14_DISPLAY_t;

typedef enum{
	SEG15_11E				= 0x01,//11E
	SEG15_11G				= 0x02,//11G
	SEG15_11F				= 0x04,//11F
	SEG15_SOCKET_NUM  		= 0x08,//插座编号
}SEG15_DISPLAY_t;

typedef enum{
	SEG16_10D				= 0x01,//10D
	SEG16_10C				= 0x02,//10C
	SEG16_10B				= 0x04,//10B
	SEG16_10A				= 0x08,//10A
}SEG16_DISPLAY_t;

typedef enum{
	SEG17_10E				= 0x01,//10E
	SEG17_10G				= 0x02,//10G
	SEG17_10F				= 0x04,//10F
	SEG17_COL1				= 0x08,//冒号1
}SEG17_DISPLAY_t;

typedef enum{
	SEG18_9D				= 0x01,//9D
	SEG18_9C				= 0x02,//9C
	SEG18_9B				= 0x04,//9B
	SEG18_9A				= 0x08,//9A
}SEG18_DISPLAY_t;

typedef enum{
	SEG19_9E				= 0x01,//9E
	SEG19_9G				= 0x02,//9G
	SEG19_9F				= 0x04,//9F
	SEG19_CHARING_MONEY		= 0x08,//充电金额
}SEG19_DISPLAY_t;

typedef enum{
	SEG20_8D				= 0x01,//8D
	SEG20_8C				= 0x02,//8C
	SEG20_8B				= 0x04,//8B
	SEG20_8A				= 0x08,//8A
}SEG20_DISPLAY_t;

typedef enum{
	SEG21_8E				= 0x01,//8E
	SEG21_8G				= 0x02,//8G
	SEG21_8F				= 0x04,//8F
	SEG21_COL0				= 0x08,//冒号0
}SEG21_DISPLAY_t;

typedef enum{
	SEG22_7D				= 0x01,//7D
	SEG22_7C   				= 0x02,//7C
	SEG22_7B 				= 0x04,//7B
	SEG22_7A  				= 0x08,//7A
}SEG22_DISPLAY_t;

typedef enum{
	SEG23_7E 				= 0x01,//7E
	SEG23_7G   				= 0x02,//7G
	SEG23_7F 				= 0x04,//7F
	SEG23_CHARGING_TIME  	= 0x08,//已充时长
}SEG23_DISPLAY_t;

typedef enum{
	SEG24_6D 				= 0x01,//6D
	SEG24_6C  				= 0x02,//6C
	SEG24_6B 				= 0x04,//6B
	SEG24_6A  				= 0x08,//6A
}SEG24_DISPLAY_t;

typedef enum{
	SEG25_6E 				= 0x01,//6E
	SEG25_6G   				= 0x02,//6G
	SEG25_6F 				= 0x04,//6F
	SEG25_PRE_CHARGE_TIME	= 0x08,//预充时长
}SEG25_DISPLAY_t;


static void HT_LCD_Init(LCD_InitTypeDef* LCD_InitStruct);
static void Lcd_All_SegCom_IO_Config(void);
static void HT_LCD_Write(uint8_t SegNum, uint8_t SegValue);
static uint8_t HT_LCD_Read(uint8_t SegNum);


void Lcd_Init(void);
void Lcd_all_on(void);
void Lcd_all_off(void);
void LcdTurnOnLed(void);
void LcdTurnOffLed(void);


void Lcd_Display_Singnal(LCD_Display_TypeDef displayFlg);
void Lcd_Display_NoSingnal(LCD_Display_TypeDef displayFlg);
void Lcd_Display_ChgMethod(LCD_Display_TypeDef displayFlg);
void Lcd_Display_Success(LCD_Display_TypeDef displayFlg);
void Lcd_Display_Failure(LCD_Display_TypeDef displayFlg);
void Lcd_Display_CardFlg(LCD_Display_TypeDef displayFlg);
void Lcd_Display_CardChargeing(LCD_Display_TypeDef displayFlg);
void Lcd_Display_CardNoMoney(LCD_Display_TypeDef displayFlg);
void Lcd_Display_CardInvalid(LCD_Display_TypeDef displayFlg);
void Lcd_Display_SocketFlg(LCD_Display_TypeDef displayFlg);
void Lcd_Display_SocketOccupied(LCD_Display_TypeDef displayFlg);
void Lcd_Display_SocketFault(LCD_Display_TypeDef displayFlg);
void Lcd_Display_SocketInvalid(LCD_Display_TypeDef displayFlg);
void Lcd_Display_CardBalance(LCD_Display_TypeDef displayFlg);
void Lcd_Display_Larger(LCD_Display_TypeDef displayFlg);
void Lcd_Display_SocketNum(LCD_Display_TypeDef displayFlg);
void Lcd_Display_ChargeingMoney(LCD_Display_TypeDef displayFlg);
void Lcd_Display_ChargeingTime(LCD_Display_TypeDef displayFlg);
void Lcd_Display_PreChargerTime(LCD_Display_TypeDef displayFlg);

#endif //__LCD_H__













