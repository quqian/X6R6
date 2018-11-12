/*
*********************************************************************************************************
*                                              HT6XXXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_aes&rand.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT6x2x, HT501x and HT502x 
*********************************************************************************************************
*/

#define  __HT6XXX_AES_RAND_C

#include "ht6xxx_aes&rand.h"

#if defined HT6x2x  ||  defined  HT501x  ||  defined  HT502x             /* This File Only support HT6x2x, HT501x and HT502x */

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
*                                      AES Encrypt MODULE
*
* ����˵��: AES����ģ��
*
* ��ڲ���: AESKeyMode			ö�ٱ�����ѡ�����ģʽ
*														KEY_Mode_128bits
*														KEY_Mode_192bits
*														KEY_Mode_256bits
*						pKeyAddr  			uint32_t *�ͱ�������ָ�����洢AES key : pKeyAddr[0]Ϊ��λ
*                       
*						pIntDataAddr  	uint32_t *�ͱ�������ָ�����洢���������ݣ����ģ�: pIntDataAddr[0]Ϊ��λ
*   
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_AES_Encrypt(AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr, uint32_t *pIntDataAddr)
{
			if(AESKeyMode == KEY_Mode_128bits)
			{
					HT_AES->AESCON = 0x00; 										/*!< key-128bits,����ģʽ							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< ��Կд����Կ�Ĵ���								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_192bits)
			{
					HT_AES->AESCON = 0x02; 										/*!< key-192bits,����ģʽ							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< ��Կд����Կ�Ĵ���								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_256bits)
			{
					HT_AES->AESCON = 0x04; 										/*!< key-256bits,����ģʽ							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< ��Կд����Կ�Ĵ���								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr++;
					HT_AES->AESKEY6 = *pKeyAddr++;
					HT_AES->AESKEY7 = *pKeyAddr;
			}
			
			HT_AES->AESINLL = *pIntDataAddr++;					/*!< ���������ݣ����ģ�д��Ĵ���				*/
			HT_AES->AESINML = *pIntDataAddr++;
			HT_AES->AESINHM =	*pIntDataAddr++;
			HT_AES->AESINHH = *pIntDataAddr;
			
			HT_AES->AESSTR = 0x8329;										/*!< ����AES����												*/
}

/*
*********************************************************************************************************
*                                      AES Decrypt MODULE
*
* ����˵��: AES����ģ��
*
* ��ڲ���: AESKeyMode			ö�ٱ�����ѡ�����ģʽ
*														KEY_Mode_128bits
*														KEY_Mode_192bits
*														KEY_Mode_256bits
*						pKeyAddr  			uint32_t *�ͱ�������ָ�����洢AES key : pKeyAddr[0]Ϊ��λ
*                       
*						pIntDataAddr  	uint32_t *�ͱ�������ָ�����洢���������ݣ����ģ�: pIntDataAddr[0]Ϊ��λ
*   
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_AES_Decrypt(AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr, uint32_t *pIntDataAddr)
{
			if(AESKeyMode == KEY_Mode_128bits)
			{
					HT_AES->AESCON = 0x01; 										/*!< key-128bits,����ģʽ							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< ��Կд����Կ�Ĵ���								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_192bits)
			{
					HT_AES->AESCON = 0x03; 										/*!< key-192bits,����ģʽ							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< ��Կд����Կ�Ĵ���								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_256bits)
			{
					HT_AES->AESCON = 0x05; 										/*!< key-256bits,����ģʽ							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< ��Կд����Կ�Ĵ���								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr++;
					HT_AES->AESKEY6 = *pKeyAddr++;
					HT_AES->AESKEY7 = *pKeyAddr;
			}
			
			HT_AES->AESINLL = *pIntDataAddr++;					/*!< ���������ݣ����ģ�д��Ĵ���				*/
			HT_AES->AESINML = *pIntDataAddr++;
			HT_AES->AESINHM =	*pIntDataAddr++;
			HT_AES->AESINHH = *pIntDataAddr;
			
			HT_AES->AESSTR = 0x8329;										/*!< ����AES����												*/
}

/*
*********************************************************************************************************
*                                      GHASH �˷����� MODULE
*
* ����˵��: GHASH �˷�����ģ��
*
* ��ڲ���: 
*						pIntDataAddr1		uint32_t *�ͱ�������ָ�����洢����1���� : pIntDataAddr1[0]Ϊ��λ
*                       
*						pIntDataAddr2  	uint32_t *�ͱ�������ָ�����洢����2���� : pIntDataAddr2[0]Ϊ��λ
*   
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_GHASH_Multiply(uint32_t *pIntDataAddr1, uint32_t *pIntDataAddr2)
{
		HT_GHASH->INPUT1LL = *pIntDataAddr1++;				/*!< д�����1													*/
		HT_GHASH->INPUT1ML = *pIntDataAddr1++;
		HT_GHASH->INPUT1HM = *pIntDataAddr1++;
		HT_GHASH->INPUT1HH = *pIntDataAddr1;
		
		HT_GHASH->INPUT2LL = *pIntDataAddr2++;				/*!< д�����2													*/
		HT_GHASH->INPUT2ML = *pIntDataAddr2++;
		HT_GHASH->INPUT2HM = *pIntDataAddr2++;
		HT_GHASH->INPUT2HH = *pIntDataAddr2;
	
		HT_GHASH->GHASHSTR = 0x0501;									/*!< ����GHASH�˷�											*/
		
}
/*
*********************************************************************************************************
*                                      AES/GHASH�˷�������ݶ�ȡ MODULE
*
* ����˵��: AES/GHASH�˷�������ݶ�ȡ
*
* ��ڲ���: 
                     
*						pOutDataAddr  	uint32_t *�ͱ�������ָ�����洢���������ݣ����ģ�: pIntDataAddr[0]Ϊ��λ
*   
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/

void HT_AES_Read(uint32_t *pOutDataAddr)
{
		*pOutDataAddr++ = HT_AES->AESOUTLL;
		*pOutDataAddr++ = HT_AES->AESOUTML;
		*pOutDataAddr++ = HT_AES->AESOUTHM;
		*pOutDataAddr 	= HT_AES->AESOUTHH;
}

void HT_GHASH_Multiply_Read(uint32_t *pOutDataAddr)
{
		*pOutDataAddr++ = HT_GHASH->OUTPUTLL;
		*pOutDataAddr++ = HT_GHASH->OUTPUTML;
		*pOutDataAddr++ = HT_GHASH->OUTPUTHM;
		*pOutDataAddr   = HT_GHASH->OUTPUTHH;
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE AES/GHASH/RAND INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�AES/GHASH/RAND�ж�
*
* ��ڲ���: ITEn       AES/GHASH/RAND�ж�����λ������Ϊ���²����������
*                        @arg AES_IE
*                        @arg GHASH_IE
*                        @arg RAND_IE
*
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_AES_GHASH_RAND_ITConfig(AES_ITEnTypeDef ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_GHASH->AESGHASHIE |= (uint32_t)ITEn;            /*!< ʹ��AES/GHASH/RAND�ж�           */
    }
    else
    {
        HT_GHASH->AESGHASHIE &= ~(uint32_t)ITEn;           /*!< �ر�AES/GHASH/RAND�ж�           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED AES/GHASH/RAND INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦAES/GHASH/RAND�жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����AES/GHASH/RAND�жϣ�����Ϊ���²���:
*                        @arg AES_IF
*                        @arg GHASH_IF
*                        @arg RAND_IF
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_AES_GHASH_RAND_ITFlagStatusGet(AES_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */
    
    if (HT_GHASH->AESGHASHIF & ITFlag)
    {       
        return SET;                        /*!< AES/GHASH/RAND Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< AES/GHASH/RAND Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR AES/GHASH/RAND INTERRUPT FLAG
*
* ����˵��: ���AES/GHASH/RAND�жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��RTC�жϱ�־������Ϊ���²����������:
*                        @arg AES_IF
*                        @arg GHASH_IF
*                        @arg RAND_IF
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_AES_GHASH_RAND_ClearITPendingBit(AES_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */
    
      
    HT_GHASH->AESGHASHIF  &= ~((uint32_t)ITFlag);                  /*!< Clear AES/GHASH/RAND Interrupt Flag       */
    
}

/*
*********************************************************************************************************
*                            GET AES/GHASH/RAND BUSY STATUS
*
* ����˵��: ��ȡ��ӦAES/GHASH/RAND��BUSY״̬
*
* ��ڲ���: ��
*
* ���ز���: ITStatus    = SET��  busyæµ
*                       = RESET��busy����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_AES_Busy_StatusGet(void)
{
    /*  assert_param  */
    return (ITStatus)HT_AES->AESFLG;                        		/*!< ����AES busy status  				*/
}

ITStatus HT_GHASH_Busy_StatusGet(void)
{
    /*  assert_param  */
    return (ITStatus)HT_GHASH->GHASHFLG;                        /*!< ����GHASHCH�˷� busy status  */
}

/*
*********************************************************************************************************
*                          128bits  xor function
*
* ����˵��: 128bits �����
*
* ��ڲ���: 
*						pIntDataAddr1		uint32_t *�ͱ�������ָ�����洢���ǰ����1 : pIntDataAddr1[0]Ϊ��λ
*                       
*						pIntDataAddr2  	uint32_t *�ͱ�������ָ�����洢���ǰ����2:  pIntDataAddr2[0]Ϊ��λ
*
* ���ز���: 
*						pOutDataAddr  	uint32_t *�ͱ�������ָ�����洢�������� : pOutDataAddr[0]Ϊ��λ
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_AES_Xor128bits(uint32_t *pIntDataAddr1, uint32_t *pIntDataAddr2, uint32_t *pOutDataAddr)
{
    /*  assert_param  */
		uint8_t i;
	
		for(i=0;i<4;i++)
		{
			*pOutDataAddr++ = (*pIntDataAddr1++) ^ (*pIntDataAddr2++);		/*!< ������ 				*/
		}
           		
}



#endif                                        /* This File Only support HT6x2x and HT501x */

