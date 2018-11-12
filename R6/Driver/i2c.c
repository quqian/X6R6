/*i2c.c
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "i2c.h"

//定义用于和I2C中断传递信息的全局变量
volatile uint8_t   I2C_sla;   //I2C器件从地址
volatile uint16_t  I2C_suba;   //2C器件内部子地址
volatile uint8_t   I2C_suba_num;  //I2C子地址字节数
volatile uint8_t   *I2C_buf;      //数据缓冲区指针
volatile uint16_t  I2C_num;  	 //要读取/写入的数据个数
volatile uint8_t   I2C_end;   //I2C总线结束标志：结束总线是置1
volatile uint8_t   I2C_suba_en;  //子地址控制
uint8_t gI2cMux=0;


//******************************************************************
//! \brief  	I2C_Init
//! \param
//! \retval
//! \note   	I2C模块初始化
//******************************************************************
void I2C_Init(void)
{
	//PC13 PC14 as SCL SDA
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_13|GPIO_Pin_14, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	GPIO_Cfg(HT_GPIOE, GPIO_Pin_0, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
	GPIO_ResetBits(HT_GPIOE, GPIO_Pin_0);

	HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_I2CEN, ENABLE);

	NVIC_DisableIRQ(I2C_IRQn);		//禁止I2C中断
	HT_I2C->I2CCON=0x0247;

	NVIC_ClearPendingIRQ(I2C_IRQn);	//清除挂起状态
	NVIC_SetPriority(I2C_IRQn, 4);	//设置优先级
	NVIC_EnableIRQ(I2C_IRQn); 		//使能I2C中断

	//PA11 as CTP_INT(INT6)
	GPIO_Cfg(HT_GPIOA, GPIO_Pin_11, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_INT->EXTIF = 0x0000;
	NVIC_ClearPendingIRQ(EXTI6_IRQn);
	NVIC_SetPriority(EXTI6_IRQn, 3);
	HT_INT->EXTIE |= 0x0040;		//INT6下降沿中断使能
	NVIC_EnableIRQ(EXTI6_IRQn);
}

//******************************************************************
//! \brief  	EXTI6_IRQHandler
//! \param
//! \retval
//! \note   	外部中断处理函数
//******************************************************************
uint8_t keyHappenEvent=0;
void EXTI6_IRQHandler(void)
{
	keyHappenEvent = 1;
	//delay_ms(100);
	HT_INT->EXTIF = 0x0000;		//清中断标志
}

//******************************************************************
//! \brief  	GetKeyHappenEvent
//! \param
//! \retval 	keyHappenEvent
//! \note   	获取按键中断发生事件
//******************************************************************
uint8_t GetKeyHappenEvent(void)
{
	return keyHappenEvent;
}

//******************************************************************
//! \brief  	ClrKeyHappenEvent
//! \param
//! \retval 	keyHappenEvent
//! \note   	清除按键中断发生事件
//******************************************************************
void ClrKeyHappenEvent(void)
{
	keyHappenEvent = 0;
	return;
}
//******************************************************************
//! \brief  	I2C_Start
//! \param
//! \retval
//! \note   	start信号
//! \note
//******************************************************************
void I2C_Start(void)
{
 	HT_I2C->I2CCON |= 0x0020;
}
//******************************************************************
//! \brief  	I2C_Stop
//! \param
//! \retval
//! \note   	stop信号
//! \note
//******************************************************************
void I2C_Stop(void)
{
	HT_I2C->I2CCON |= 0x0010;
}
//******************************************************************
//! \brief  	I2C_ClrSi
//! \param
//! \retval
//! \note   	清除中断标志
//! \note
//******************************************************************
void I2C_ClrSi(void)
{
	HT_I2C->I2CCON &= 0xffd7;

}
//******************************************************************
//! \brief  	I2C_IRQHandler
//! \param
//! \retval
//! \note   	I2C中断函数
//! \note
//******************************************************************
void I2C_IRQHandler(void)
{
	//根据状态码进行相应的处理
    switch (HT_I2C->I2CSTA&0x000000F8){
		case 0x08: //已发送起始条件(主发送和主接收都有)
		{
			//读写从机地址已初始化,直接发送
			HT_I2C->I2CDAT = I2C_sla;
			//清中断标志
			I2C_ClrSi();
		}
		break;

		case 0x10: //已发送重复起始条件 (主发送和主接收都有)
		{
			//装入SLA+W或者SLA+R
			HT_I2C->I2CDAT = I2C_sla; //重起总线后，重发从地址
			//清零SI,STA
			I2C_ClrSi();
		}
		break;

		case 0x18://SLA+W已被发送,ACK已被接收
		case 0x28://已发送I2DAT中的数据，已接收ACK
		{
			if (I2C_suba_en == 0){
				if (I2C_num > 0){
					HT_I2C->I2CDAT = *I2C_buf++;
					I2C_ClrSi();     // 清零SI,STA
					I2C_num--;
				}else{ //没有数据发送了
					//停止总线
					I2C_Stop();    		//STO
					I2C_ClrSi();     	//清零SI,STA
					I2C_end = 1;      	//总线已经停止
				}
			}

			if(I2C_suba_en == 1) {// 若是指定子地址读，则重新启动总线
				if (I2C_suba_num == 2){
					HT_I2C->I2CDAT = ((I2C_suba >> 8) & 0xff);
					I2C_ClrSi();     //清零SI,STA
					I2C_suba_num--;
					break;
				}

				if(I2C_suba_num == 1){
					HT_I2C->I2CDAT = (I2C_suba & 0xff);
					I2C_ClrSi();     //清零SI,STA
					I2C_suba_num--;
					break;
				}

				if (I2C_suba_num == 0){
					I2C_Start();
					HT_I2C->I2CCON &= 0xfff7;
					I2C_sla += 1;
					I2C_suba_en = 0;        //已发送子地址
					break;
				}
			}

			if (I2C_suba_en == 2){// 指定子地址写,子地址尚未指定,则发送子地址
				if (I2C_suba_num > 0){
					if (I2C_suba_num == 2){
						HT_I2C->I2CDAT = ((I2C_suba >> 8) & 0xff);
						I2C_ClrSi();
						I2C_suba_num--;
						break;
					}

					if (I2C_suba_num == 1){
						HT_I2C->I2CDAT    = (I2C_suba & 0xff);
						I2C_ClrSi();
						I2C_suba_num--;
						I2C_suba_en  = 0;			 //已发送子地址
						break;
					}
				}
			}
		}
		break;

		case 0x40: //已发送SLA+R,已接收ACK
		{
			//如果是最后一个字节
			if (I2C_num <= 1) {
				HT_I2C->I2CCON &= 0xfffb;    //下次发送非应答信号
			}else{
				HT_I2C->I2CCON |= 0x04;      //下次发送应答信号
			}
			I2C_ClrSi();       //清零SI,STA
		}
		break;

		case 0x20: //已发送SLA+W,已接收非应答
		case 0x30: //已发送I2DAT中的数据，已接收非应答
		case 0x38: //在SLA+R/W或数据字节中丢失仲裁
		case 0x48: //已发送SLA+R,已接收非应答
		{
			I2C_Stop();             //结束总线
			I2C_ClrSi();
			I2C_end = 0xFF;
		}
	    break;

		case 0x50: //已接收数据字节，已返回ACK
		{
			*I2C_buf++ = HT_I2C->I2CDAT;//-10;
			I2C_num--;

			if (I2C_num == 0){//接收最后一个字节
				HT_I2C->I2CCON &= 0xffd3;      //STA,SI,AA = 0  I2CCON
			}else{
				HT_I2C->I2CCON &= 0xffff;      //AA=1
				I2C_ClrSi();
			}
		}
		break;

		case 0x58: //已接收数据字节，已返回非应答
		{
			*I2C_buf++ = HT_I2C->I2CDAT;//-10;          //读取最后一字节数据
			I2C_Stop();             //结束总线
			I2C_ClrSi();
			I2C_end = 1;
		}
		break;

		default:
			break;
	}
    //中断处理结束
}

//******************************************************************
//! \brief  	I2C_ReadNByte
//! \param  	slaveAddr   		器件从地址
//! 			subAddrType 		子地址结构 1－单字节地址 3－8+X结构 2－双字节地址
//!				sub_addr  			器件内部物理地址
//!				*rbuf    			数据存储的指针
//!				len   				将要读取的数据的个数
//! \retval		TRUE  操作成功		FALSE  操作失败
//! \note   	I2C读取数据
//******************************************************************
uint8_t I2C_ReadNByte(uint8_t slaveAddr, uint8_t subAddrType, uint32_t subAddr, uint8_t *rbuf, uint32_t len)
{
    int i;
	MuxSempTake(&gI2cMux);

	// 判断读取长度的合法性
	if(0 == len) return FALSE;

	//参数设置
	if (0 == subAddrType) {
		I2C_sla      = slaveAddr + 1; 				//写入读器件的从地址，R=1
		I2C_suba     = 0;        					//器件子地址为0
		I2C_suba_num = 0;        					//器件子地址为0字节
	} else if (1 == subAddrType) {
		I2C_sla      = slaveAddr; 					//写入写器件的从地址，W=0
		I2C_suba     = subAddr;        				//器件子地址
		I2C_suba_num = 1;        					//器件子地址为1字节
	} else if (2 == subAddrType) {
		I2C_sla      = slaveAddr; 					//写入写器件的从地址，W=0
		I2C_suba     = subAddr;        				//器件子地址
		I2C_suba_num = 2;        					//器件子地址为2字节
	} else if (3 == subAddrType) { 					//子地址结构为8+X
		I2C_sla   = slaveAddr + ((subAddr >> 7 )& 0x0e) + 1; 	//读器件的从地址，R=1
		I2C_suba  = subAddr & 0x0ff;      						//器件子地址
		I2C_suba_num = 1;        								//器件子地址为8+x
	}

	I2C_buf     = rbuf;       //数据接收缓冲区指针
	I2C_num     = len;        //要读取的个数
	I2C_suba_en = 1;          //有子地址读
	I2C_end     = 0;

	//清除STA,SI,AA标志位
	HT_I2C->I2CCON &= 0xffd3;
	//置位STA,启动I2C总线
	HT_I2C->I2CCON |= 0x60; //I2CEN

	/* 等待I2C操作完成 */
    i = 0;
	while (I2C_end == 0) {
        vTaskDelay(1);
        if (2000 < ++i) {
            break;
        }
    }

	MuxSempGive(&gI2cMux);
	if (I2C_end == 1) {
		return (TRUE);
	}else{
		return (FALSE);
	}
}

//******************************************************************
//! \brief  	I2C_WriteNByte
//! \param  	slaveAddr   			器件从地址
//! 			subAddrType 		子地址结构 1－单字节地址 3－8+X结构 2－双字节地址
//!				subAddr  			器件内部物理地址
//!				*wBuf   			将要写入的数据的指针
//!				len   			将要写入的数据的个数
//! \retval		TRUE  操作成功		FALSE  操作失败
//! \note
//******************************************************************
uint8_t I2C_WriteNByte(uint8_t slaveAddr, uint8_t subAddrType, uint32_t subAddr, uint8_t *wBuf, uint32_t len)
{
    int i;

	MuxSempTake(&gI2cMux);
	//设置参数
	if (subAddrType == 1){ //子地址为单字节
		I2C_sla      = slaveAddr;     	// 读器件的从地址
		I2C_suba     = subAddr;       	//器件子地址
		I2C_suba_num = 1;       		//器件子地址为1字节
	}

	if (subAddrType == 2){ //子地址为2字节
		I2C_sla      = slaveAddr;        	//读器件的从地址
		I2C_suba      = subAddr;       	//器件子地址
		I2C_suba_num = 2;        			//器件子地址为2字节
	}

	if (subAddrType == 3){ // 子地址结构为8+X
		I2C_sla   = slaveAddr + ((subAddr >> 7 )& 0x0e);  		//读器件的从地址
		I2C_suba  = subAddr & 0x0ff;      							//器件子地址
		I2C_suba_num = 1;        									//器件子地址为8+X
	}

	I2C_buf     = wBuf;          		//数据
	I2C_num     = len;          		//数据个数
	I2C_suba_en = 2;          			//有子地址，写操作
	I2C_end     = 0;

	//清除STA,SI,AA标志位
	HT_I2C->I2CCON &= 0xffd3;
	//置位STA,启动I2C总线
	HT_I2C->I2CCON |= 0x60;

	//等待I2C操作完成
    i = 0;
	while (I2C_end == 0) {
        vTaskDelay(1);
        if (2000 < ++i) {
            CL_LOG("wait i2c fail.\n");
            break;
        }
    }

	MuxSempGive(&gI2cMux);
	if (I2C_end == 1){
		return (TRUE);
	}else{
		return (FALSE);
	}
}


