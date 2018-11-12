/*lcd.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "lcd.h"

                                                                   
#define    MaxSegNum                 25



/**************************************************************************/
//******************************************************************
//! \brief  	Lcd_Init
//! \param  	none
//! \retval 	
//! \note   	
//******************************************************************
void Lcd_Init(void)
{
	LCD_InitTypeDef LCD_Init;
	
	//IO初始化
	Lcd_All_SegCom_IO_Config();
	//时钟使能
	HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_LCDEN, ENABLE);
	
	// 1/4bias, 4COM, Ffrm=64Hz
	// big current charge mode, 99.1%VLCD
	LCD_Init.LCD_Com=LCD_4COM;
	LCD_Init.LCD_Bias=LCDBias_OneFourth;	
	LCD_Init.LCD_ChargeMode=LCDCharge_HighCurrent;	
	LCD_Init.LCD_CLK=LCDCLK_Div2;     		
	LCD_Init.LCD_Vrsel=VRsel_OneFourth_P99;
	
	HT_LCD_Init(&LCD_Init);	
				
}

//******************************************************************
//! \brief  	Lcd_All_SegCom_IO_Config
//! \param  	none
//! \retval 	
//! \note   	
//! \note  
//******************************************************************
static void Lcd_All_SegCom_IO_Config(void)
{
	//COM0~COM3
	GPIO_Cfg(HT_GPIOD, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);  

	//SEG0~SEG15
	GPIO_Cfg(HT_GPIOB, GPIO_Pin_All, GPIO_Mode_AF1, GPIO_Input_Up,GPIO_Output_PP);  
	
	//SEG16~SEG23
	GPIO_Cfg(HT_GPIOD, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
			|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);  
	
	//SEG24~SEG25
	GPIO_Cfg(HT_GPIOD, GPIO_Pin_12|GPIO_Pin_13, GPIO_Mode_AF2, GPIO_Input_Up, GPIO_Output_PP);  

	//light
	GPIO_Cfg(HT_GPIOD, GPIO_Pin_14, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);  
}
//******************************************************************
//! \brief  	LcdTurnOnLed
//! \param  	
//! \retval 	
//! \note   	 
//******************************************************************
void LcdTurnOnLed(void)
{
	GPIO_SetBits(HT_GPIOD, GPIO_Pin_14);
}

//******************************************************************
//! \brief  	LcdTurnOffLed
//! \param  	
//! \retval 	
//! \note   	
//******************************************************************
void LcdTurnOffLed(void)
{
	GPIO_ResetBits(HT_GPIOD, GPIO_Pin_14);
}
//******************************************************************
//! \brief  	HT_LCD_Init
//! \param  	LCD_InitStruct
//!             1) LCD_Bias       : LCD偏压驱动
//!             2) LCD_Com        : LCDCom Number
//!             3) LCD_CLK        : LCD扫描频率 
//!             4) LCD_ChargeMode : LCD充电模式        
//!             5) LCD_Vrsel[3..0]: LCD对比度，数值越小，对比度越高  
//! \retval 	none
//! \note   	
//! \note  		用户在配置LCD寄存器前应先使能LCD模块，具体参见HT_CMU_ClkCtrl0Config()函数
//******************************************************************
static void HT_LCD_Init(LCD_InitTypeDef* LCD_InitStruct)
{
    /*  assert_param  */
    uint32_t tempreg;

    tempreg  = LCD_InitStruct->LCD_Bias;                   /*!< LCD偏压驱动              */
    tempreg |= LCD_InitStruct->LCD_Com;                    /*!< LCDCom Number            */
    tempreg |= LCD_InitStruct->LCD_CLK;                    /*!< LCD扫描频率              */

    HT_LCD->LCDCLK = tempreg;                              /*!< 配置LCDCLK寄存器         */

    tempreg  = (uint32_t)LCD_InitStruct->LCD_ChargeMode;   /*!< LCD充电模式设置          */
    tempreg |= (LCD_InitStruct->LCD_Vrsel & LCD_LCDCON_VRSEL);
                                                           /*!< LCD对比度设置            */
    HT_LCD->LCDCON = tempreg;                              /*!< 配置LCDCON寄存器         */ 
} 

//******************************************************************
//! \brief  	HT_LCD_Write
//! \param  	SegNum 			0-41之间
//! \param		SegValue		往Segment写的值     
//! \retval 	none
//! \note   	
//! \note  
//******************************************************************
//static void HT_LCD_Write(uint8_t SegNum, uint8_t SegValue)
//{
//    /*  assert_param  */
//    if(SegNum>MaxSegNum)
//        return;
//    HT_LCD->LCDBUF[SegNum] = SegValue;
//} 


//******************************************************************
//! \brief  	HT_LCD_Read
//! \param  	SegNum 			0-41之间
//! \retval 	none
//! \note   	
//! \note  
//******************************************************************
static uint8_t HT_LCD_Read(uint8_t SegNum)
{
    /*  assert_param  */
    return (uint8_t)HT_LCD->LCDBUF[SegNum];
} 

//******************************************************************
//! \brief  	Lcd_all_on
//! \param  	
//! \retval 	none
//! \note   	
//! \note  
//******************************************************************
void Lcd_all_on(void)
{
	uint8_t i=0;
	
	for(i=0; i<=MaxSegNum; i++){
		HT_LCD->LCDBUF[i] = 0xFF;
	}
}
//******************************************************************
//! \brief  	Lcd_all_off
//! \param  	
//! \retval 	none
//! \note   	
//! \note  
//******************************************************************
void Lcd_all_off(void)
{
	uint8_t i=0;
	
	for(i=0; i<=MaxSegNum; i++){
		HT_LCD->LCDBUF[i] = 0x00;
	}
}

//******************************************************************
//! \brief  	Lcd_Display_Singnal
//! \param  	
//! \retval 	none
//! \note   	显示信号
//! \note  
//******************************************************************
void Lcd_Display_Singnal(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(0);
	
	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG0_SIGNAL;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}else{
		lcdBuf &= ~SEG0_SIGNAL;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_NoSingnal
//! \param  	
//! \retval 	none
//! \note   	显示无信号
//! \note  
//******************************************************************
void Lcd_Display_NoSingnal(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(0);
	
	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG0_NOSIGNAL;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}else{
		lcdBuf &= ~SEG0_NOSIGNAL;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_ChgMethod
//! \param  	
//! \retval 	none
//! \note   	显示充电方式(请刷卡或扫码)
//! \note  
//******************************************************************
void Lcd_Display_ChgMethod(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(1);
	
	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG1_CHARGE_METHOD;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}else{
		lcdBuf &= ~SEG1_CHARGE_METHOD;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_Success
//! \param  	
//! \retval 	none
//! \note   	显示成功
//! \note  
//******************************************************************
void Lcd_Display_Success(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(9);
	
	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG9_SOCKET_SUCCESS;
		HT_LCD->LCDBUF[9] = lcdBuf;
	}else{
		lcdBuf &= ~SEG9_SOCKET_SUCCESS;
		HT_LCD->LCDBUF[9] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_Failure
//! \param  	
//! \retval 	none
//! \note   	显示失败
//! \note  
//******************************************************************
void Lcd_Display_Failure(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(10);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG10_SOCKET_FAILURE;
		HT_LCD->LCDBUF[10] = lcdBuf;
	}else{
		lcdBuf &= ~SEG10_SOCKET_FAILURE;
		HT_LCD->LCDBUF[10] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_CardFlg
//! \param  	
//! \retval 	none
//! \note   	显示卡标志
//! \note  
//******************************************************************
void Lcd_Display_CardFlg(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(1);
	
	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG1_CARD_FLAG;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}else{
		lcdBuf &= ~SEG1_CARD_FLAG;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_CardChargeing
//! \param  	
//! \retval 	none
//! \note   	显示充电中
//! \note  
//******************************************************************
void Lcd_Display_CardChargeing(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(0);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG0_CARD_CHARGING;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}else{
		lcdBuf &= ~SEG0_CARD_CHARGING;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_CardNoMoney
//! \param  	
//! \retval 	
//! \note   	显示欠费
//! \note  
//******************************************************************
void Lcd_Display_CardNoMoney(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(1);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG1_CARD_NOMONEY;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}else{
		lcdBuf &= ~SEG1_CARD_NOMONEY;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_CardNoMoney
//! \param  	
//! \retval 	
//! \note   	显示卡无效
//! \note  
//******************************************************************
void Lcd_Display_CardInvalid(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(1);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG1_CARD_INVALID;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}else{
		lcdBuf &= ~SEG1_CARD_INVALID;
		HT_LCD->LCDBUF[1] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_SocketFlg
//! \param  	
//! \retval 	
//! \note   	显示插座标志
//! \note  
//******************************************************************
void Lcd_Display_SocketFlg(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(10);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG10_SOCKET_FLAG;
		HT_LCD->LCDBUF[10] = lcdBuf;
	}else{
		lcdBuf &= ~SEG10_SOCKET_FLAG;
		HT_LCD->LCDBUF[10] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_SocketOccupied
//! \param  	
//! \retval 	
//! \note   	显示插座占用
//! \note  
//******************************************************************
void Lcd_Display_SocketOccupied(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(9);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG9_SOCKET_OCUUPIED;
		HT_LCD->LCDBUF[9] = lcdBuf;
	}else{
		lcdBuf &= ~SEG9_SOCKET_OCUUPIED;
		HT_LCD->LCDBUF[9] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_SocketFault
//! \param  	
//! \retval 	
//! \note   	显示插座故障
//! \note  
//******************************************************************
void Lcd_Display_SocketFault(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(9);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG9_SOCKET_FAULT;
		HT_LCD->LCDBUF[9] = lcdBuf;
	}else{
		lcdBuf &= ~SEG9_SOCKET_FAULT;
		HT_LCD->LCDBUF[9] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_SocketInvalid
//! \param  	
//! \retval 	
//! \note   	显示插座无效
//! \note  
//******************************************************************
void Lcd_Display_SocketInvalid(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(10);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG10_SOCKET_INVALID;
		HT_LCD->LCDBUF[10] = lcdBuf;
	}else{
		lcdBuf &= ~SEG10_SOCKET_INVALID;
		HT_LCD->LCDBUF[10] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_CardBalance
//! \param  	
//! \retval 	
//! \note   	显示卡余额
//! \note  
//******************************************************************
void Lcd_Display_CardBalance(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(0);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG0_CARD_BALANCE;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}else{
		lcdBuf &= ~SEG0_CARD_BALANCE;
		HT_LCD->LCDBUF[0] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_Larger
//! \param  	
//! \retval 	
//! \note   	显示大于
//! \note  
//******************************************************************
void Lcd_Display_Larger(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(2);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG2_LARGER;
		HT_LCD->LCDBUF[2] = lcdBuf;
	}else{
		lcdBuf &= ~SEG2_LARGER;
		HT_LCD->LCDBUF[2] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_SocketNum
//! \param  	
//! \retval 	
//! \note   	显示插座编号
//! \note  
//******************************************************************
void Lcd_Display_SocketNum(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(15);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG15_SOCKET_NUM;
		HT_LCD->LCDBUF[15] = lcdBuf;
	}else{
		lcdBuf &= ~SEG15_SOCKET_NUM;
		HT_LCD->LCDBUF[15] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_ChargeingMoney
//! \param  	
//! \retval 	
//! \note   	显示充电金额
//! \note  
//******************************************************************
void Lcd_Display_ChargeingMoney(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(19);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG19_CHARING_MONEY;
		HT_LCD->LCDBUF[19] = lcdBuf;
	}else{
		lcdBuf &= ~SEG19_CHARING_MONEY;
		HT_LCD->LCDBUF[19] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_ChargeingTime
//! \param  	
//! \retval 	
//! \note   	显示已充时长
//! \note  
//******************************************************************
void Lcd_Display_ChargeingTime(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(23);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG23_CHARGING_TIME;
		HT_LCD->LCDBUF[23] = lcdBuf;
	}else{
		lcdBuf &= ~SEG23_CHARGING_TIME;
		HT_LCD->LCDBUF[23] = lcdBuf;
	}
}
//******************************************************************
//! \brief  	Lcd_Display_PreChargerTime
//! \param  	
//! \retval 	
//! \note   	显示预充时长
//! \note  
//******************************************************************
void Lcd_Display_PreChargerTime(LCD_Display_TypeDef displayFlg)
{
	uint8_t lcdBuf = 0;
	lcdBuf = HT_LCD_Read(25);

	if(LCD_DISPLAY == displayFlg){
		lcdBuf |= SEG25_PRE_CHARGE_TIME;
		HT_LCD->LCDBUF[25] = lcdBuf;
	}else{
		lcdBuf &= ~SEG25_PRE_CHARGE_TIME;
		HT_LCD->LCDBUF[25] = lcdBuf;
	}
}

typedef enum{
	DATA1 = 0,
	DATA2,
	DATA3,
	DATA4,
	DATA5,
	DATA6,
	DATA7,
	DATA8,
	DATA9,
	DATA10,
	DATA11
}DATA_List_t;


void Lcd_Display_Zero()
{
	

}




























