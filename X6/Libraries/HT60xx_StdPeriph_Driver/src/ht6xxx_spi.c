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
* File         : ht6xxx_spi.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_SPI_C

#include "ht6xxx_spi.h"

/*
*********************************************************************************************************
*                                           ���غ�/�ṹ��
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                             ���ر���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                 INITIAL SPI MODULE
*
* ����˵��: ��ʼ��SPIģ��
*
* ��ڲ���:  SPIx               ֻ����HT_SPI0/HT_SPI1��һ��
*            SPI_InitStruct     SPIģ���ʼ���ṹ��ָ�룬��Ҫ����5������: 
*                               1) SPI_CSInCtrol : �Ƿ��SPI_CS���� 
*                               2) SPI_Baudrate  : SPIʱ������
*                               3) SPI_CPHA      : ʱ����λ����
*                               4) SPI_CPOL      : ʱ�Ӽ�������        
*                               5) SPI_Mode      : SPI����ģʽ����    
*
* ���ز���: ��                                      
* 
* ����˵��: �û�������SPI�Ĵ���ǰӦ��ʹ��SPIģ�飬����μ�HT_CMU_ClkCtrl0Config()����
*********************************************************************************************************
*/
void HT_SPI_Init(HT_SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg = 0x81;
    
    if(SPI_InitStruct->SPI_CSInCtrol != DISABLE)
    {
        tempreg &= ~SPI_SPICON_SSDIS;                               /*!< ��SPI_CS����          */
    }
    
    tempreg |= SPI_InitStruct->SPI_Baudrate;                        /*!< ����ʱ��Ƶ��            */
    tempreg |= SPI_InitStruct->SPI_CPHA;                            /*!< ������λ                */
    tempreg |= SPI_InitStruct->SPI_CPOL;                            /*!< ����ʱ�Ӽ���            */
    tempreg |= SPI_InitStruct->SPI_Mode;                            /*!< ��������ģʽ            */
    
    SPIx->SPICON = tempreg;
  
} 

/*
*********************************************************************************************************
*                            GET SPECIFIED SPI INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦSPI�жϱ�־״̬
*
* ��ڲ���: SPIx       ֻ����HT_SPI0/HT_SPI1��һ��
*           ITFlag     ��Ҫ����ĳ��SPI�жϣ�����Ϊ���²���:
*                        @arg SPI_SPISTA_MODF
*                        @arg SPI_SPISTA_SSERR   ע���˱�־λ�����жϱ�־λ
*                        @arg SPI_SPISTA_WCOL    ע���˱�־λ�����жϱ�־λ
*                        @arg SPI_SPISTA_SPIF
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_SPI_ITFlagStatusGet(HT_SPI_TypeDef* SPIx, uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (SPIx->SPISTA & ITFlag)
    {       
        return SET;                              /*!< Interrupt Flag is set   */
    }
    else
    {
        return RESET;                            /*!< Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR SPI INTERRUPT FLAG
*
* ����˵��: ���SPI�жϱ�־
*
* ��ڲ���:     SPIx       ֻ����HT_SPI0/HT_SPI1��һ��
*               ITFlag     ��Ҫ�����ĳ��SPI�жϱ�־������Ϊ���²����������:
*                        @arg SPI_SPISTA_MODF
*                        @arg SPI_SPISTA_SSERR   ע���˱�־λ�����жϱ�־λ
*                        @arg SPI_SPISTA_WCOL    ע���˱�־λ�����жϱ�־λ
*                        @arg SPI_SPISTA_SPIF
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_SPI_ClearITPendingBit(HT_SPI_TypeDef* SPIx, uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    SPIx->SPISTA  &= ~((uint32_t)ITFlag);               /*!< Clear SPI Interrupt Flag      */
    
}

/*
*********************************************************************************************************
*                              WRITE AND READ SPIDAT REGISTER
*
* ����˵��: ��SPI���ݼĴ���д���ݣ�ͬʱ������
*
* ��ڲ���: SPIx       ֻ����HT_SPI0/HT_SPI1��һ��
*           halfword   ��Ҫд������  
*
* ���ز���: SPIDAT  : SPIDAT�Ĵ����ж���������
* 
* ����˵��: ��
*********************************************************************************************************
*/
uint8_t HT_SPI_SendByte(HT_SPI_TypeDef* SPIx, uint16_t halfword)
{
    while(SET==HT_SPI_ITFlagStatusGet(SPIx, SPI_SPISTA_SPIF));
    SPIx->SPIDAT = halfword;
    
    while(RESET==HT_SPI_ITFlagStatusGet(SPIx, SPI_SPISTA_SPIF));
    return (uint8_t)SPIx->SPIDAT;
}

/*
*********************************************************************************************************
*                                 SET SPI_CS HIGH
*
* ����˵��: �趨SPICSΪ��
*
* ��ڲ���: SPIx       ֻ����HT_SPI0/HT_SPI1��һ��    
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_SPI_CSHigh(HT_SPI_TypeDef* SPIx)
{
    /*  assert_param  */
    
    SPIx->SPISSN = 0x03;                       /*!< SPI CS����           */
}

/*
*********************************************************************************************************
*                                 SET SPI_CS LOW
*
* ����˵��: �趨SPICSΪ��
*
* ��ڲ���: SPIx       ֻ����HT_SPI0/HT_SPI1��һ��   
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_SPI_CSLow(HT_SPI_TypeDef* SPIx)
{
    /*  assert_param  */
    
    SPIx->SPISSN = 0x02;                       /*!< SPI CS����           */
}

