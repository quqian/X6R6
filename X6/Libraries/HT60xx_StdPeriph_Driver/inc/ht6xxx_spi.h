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
* File         : ht6xxx_spi.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_SPI_H__
#define __HT6XXX_SPI_H__

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
* @brief  SPIʱ����λ����
*/
typedef enum
{ 
    SPI_CPHA_1Edge = 0x00,                      /*!< ��һ����̬��֮ǰ���������ݱ�����  */
    SPI_CPHA_2Edge = 0x08,                      /*!< ��̬��ͨ�������ݱ�����            */
}SPICPHA_TypeDef;                               /*!< end of group SPICPHA_TypeDef      */   

/* 
* @brief  SPIʱ�Ӽ��Զ���
*/
typedef enum
{ 
    SPI_CPOL_Low = 0x00,                        /*!< ����״̬SCK����Ϊ�͵�ƽ           */
    SPI_CPOL_high = 0x04,                       /*!< ����״̬SCK����Ϊ�ߵ�ƽ           */
}SPICPOL_TypeDef;                               /*!< end of group SPICPOL_TypeDef      */ 

/* 
* @brief  SPIʱ��Ƶ�ʶ���
*/
typedef enum
{ 
    SPI_BaudRatePrescaler_2   = 0x00,           /*!< Fspi = Fsys/2                     */
    SPI_BaudRatePrescaler_4   = 0x10,           /*!< Fspi = Fsys/4                     */
    SPI_BaudRatePrescaler_8   = 0x20,           /*!< Fspi = Fsys/8                     */
    SPI_BaudRatePrescaler_16  = 0x30,           /*!< Fspi = Fsys/16                    */
    SPI_BaudRatePrescaler_32  = 0x40,           /*!< Fspi = Fsys/32                    */
    SPI_BaudRatePrescaler_64  = 0x50,           /*!< Fspi = Fsys/64                    */
    SPI_BaudRatePrescaler_128 = 0x60,           /*!< Fspi = Fsys/128                   */
    SPI_CLK_None              = 0x70,           /*!< ��������ʱ��                      */
}SPIBaudrate_TypeDef;                           /*!< end of group SPIBaudrate_TypeDef  */ 

/* 
* @brief  SPIMode����
*/
typedef enum
{ 
    SPI_Master = 0x02,                          /*!< SPI����ģʽ                       */
    SPI_Slave  = 0x00,                          /*!< SPI�ӻ�ģʽ                       */
}SPIMode_TypeDef;                               /*!< end of group SPIMode_TypeDef      */ 


/* 
* @brief  SPIģ���ʼ���ṹ����
*/
typedef struct
{
    FunctionalState SPI_CSInCtrol;              /*!< ���ӻ�ʱ���Ƿ��SPI_CS�������  */
    SPIBaudrate_TypeDef SPI_Baudrate;           /*!< SPIʱ������                       */
    SPICPHA_TypeDef SPI_CPHA;                   /*!< ʱ����λ����                      */
    SPICPOL_TypeDef SPI_CPOL;                   /*!< ʱ�Ӽ�������                      */
    SPIMode_TypeDef SPI_Mode;                   /*!< SPI_Mode����                      */

}SPI_InitTypeDef;                               /*!< end of group SPI_InitTypeDef      */


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

void HT_SPI_Init(HT_SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
ITStatus HT_SPI_ITFlagStatusGet(HT_SPI_TypeDef* SPIx, uint8_t ITFlag);
void HT_SPI_ClearITPendingBit(HT_SPI_TypeDef* SPIx, uint8_t ITFlag);
uint8_t HT_SPI_SendByte(HT_SPI_TypeDef* SPIx, uint16_t halfword);
void HT_SPI_CSHigh(HT_SPI_TypeDef* SPIx);
void HT_SPI_CSLow(HT_SPI_TypeDef* SPIx);




#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_SPI_H__ */ 
