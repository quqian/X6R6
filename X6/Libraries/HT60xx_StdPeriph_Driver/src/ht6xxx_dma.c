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
* File         : ht6xxx_dma.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT6x2x, HT501x and HT502x
*********************************************************************************************************
*/

#define  __HT6XXX_DMA_C

#include "ht6xxx_dma.h"

#if defined HT6x2x  ||  defined  HT501x  ||  defined  HT502x           /* This File Only support HT6x2x, HT501x and HT502x */

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
*                                         INITIAL DMA MODULE
*
* ����˵��: ��ʼ��DMAģ��
*
* ��ڲ���: DMAy_Channelx      ֻ����HT_DMA_Channel0/HT_DMA_Channel1/HT_DMA_Channel2��һ��
*
*           DMA_InitStruct     DMA��ʼ���ṹ��ָ�룬��Ҫ����9������: 
*                              1) DMA_Request
*                              2) DMA_SourceAddr    
*                              3) DMA_DestinationAddr 
*                              4) DMA_BulkSize 
*                              5) DMA_SourceAddrInc  :   
*                              6) DMA_DestinationAddrInc : 
*                              7) DMA_MemoryDataSize  : 
*                              8) DMA_TransferMode : 
*                              9) DMA_CycleMode  : 
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/

void HT_DMA_Init(HT_DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg = 0;
    
    tempreg |= DMA_InitStruct->DMA_SourceAddrInc | DMA_InitStruct->DMA_DestinationAddrInc |
               DMA_InitStruct->DMA_MemoryDataSize | DMA_InitStruct->DMA_TransferMode | 
               DMA_InitStruct->DMA_CycleMode | DMA_InitStruct->DMA_Request;
    
    DMAy_Channelx->CHNCTL = tempreg;
    DMAy_Channelx->CHNSRC = DMA_InitStruct->DMA_SourceAddr;
    DMAy_Channelx->CHNTAR = DMA_InitStruct->DMA_DestinationAddr;
    DMAy_Channelx->CHNCNT = DMA_InitStruct->DMA_TransferNum;
    DMAy_Channelx->CHNBULKNUM = DMA_InitStruct->DMA_BulkSize;
    
}     

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE DMA INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�DMA�ж�
*
* ��ڲ���: ITEn       DMA�ж�����λ������Ϊ���²����������
*                        @arg DMA_DMAIE_TCIE0
*                        @arg DMA_DMAIE_TCIE1
*                        @arg DMA_DMAIE_TCIE2
*                        @arg DMA_DMAIE_HTIE0
*                        @arg DMA_DMAIE_HTIE1
*                        @arg DMA_DMAIE_HTIE2
*                        @arg DMA_DMAIE_TEIE0
*                        @arg DMA_DMAIE_TEIE1
*                        @arg DMA_DMAIE_TEIE2
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_DMA_ITConfig(uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_DMA->DMAIE |= (uint32_t)ITEn;            /*!< ʹ��DMA�ж�           */
    }
    else
    {
        HT_DMA->DMAIE &= ~(uint32_t)ITEn;           /*!< �ر�DMA�ж�           */
    } 
}


/*
*********************************************************************************************************
*                            GET SPECIFIED DMA INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦDMA�жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����ĳ��DMA�жϣ�����Ϊ���²���:
*                        @arg DMA_DMAIF_TCIF0
*                        @arg DMA_DMAIF_TCIF1
*                        @arg DMA_DMAIF_TCIF2
*                        @arg DMA_DMAIF_HTIF0
*                        @arg DMA_DMAIF_HTIF1
*                        @arg DMA_DMAIF_HTIF2
*                        @arg DMA_DMAIF_TEIF0
*                        @arg DMA_DMAIF_TEIF1
*                        @arg DMA_DMAIF_TEIF2
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_DMA_ITFlagStatusGet(uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (HT_DMA->DMAIF & ITFlag)
    {       
        return SET;                        /*!< DMA Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< DMA Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR DMA INTERRUPT FLAG
*
* ����˵��: ���DMA�жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��DMA�жϱ�־������Ϊ���²����������:
*                        @arg DMA_DMAIF_TCIF0
*                        @arg DMA_DMAIF_TCIF1
*                        @arg DMA_DMAIF_TCIF2
*                        @arg DMA_DMAIF_HTIF0
*                        @arg DMA_DMAIF_HTIF1
*                        @arg DMA_DMAIF_HTIF2
*                        @arg DMA_DMAIF_TEIF0
*                        @arg DMA_DMAIF_TEIF1
*                        @arg DMA_DMAIF_TEIF2
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_DMA_ClearITPendingBit(uint8_t ITFlag)
{
    /*  assert_param  */
    
    HT_DMA->DMAIF  &= ~((uint32_t)ITFlag);                  /*!< Clear DMA Interrupt Flag       */
    
}



/*
*********************************************************************************************************
*                                         Enable DMA MODULE
*
* ����˵��: DMA��ʼ������
*
* ��ڲ���: DMAy_Channelx      ֻ����HT_DMA_Channel0/HT_DMA_Channel1/HT_DMA_Channel2��һ��
*
*           NewState           ENABLE or DISABLE
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void DMA_Cmd(HT_DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected DMAy Channelx */
    DMAy_Channelx->CHNCTL |= DMA_Channel_Enable;
  }
  else
  {
    /* Disable the selected DMAy Channelx */
    DMAy_Channelx->CHNCTL &= DMA_Channel_Disable;
  }
}







#endif                                        /* This File Only support HT6x2x and HT501x */

