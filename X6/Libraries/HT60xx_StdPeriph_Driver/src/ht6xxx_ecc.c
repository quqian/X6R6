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

#define  __HT6XXX_ECC_C

#include "ht6xxx_ecc.h"

#if defined  HT502x             /* This File Only support HT502x */

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
*                              ECDSA ECC������ʼ�� 
*
* ����˵��: ECDSA ECC������ʼ�� 
*
* ��ڲ���: ECC_ECDSA_InitStruct    ECDSA ECC������ʼ�� �����ṹ��ָ�룬��Ҫ����5������: 
*														1) pECCpAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�p : pECCpAddr[0]Ϊ��λ
*														2) pECCaAddr				uint32_t *�ͱ�������ָ�����洢�����ϵ����߲���A : pECCaAddr[0]Ϊ��λ
*														3) pECCnAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�n : pECCnAddr[0]Ϊ��λ
*														4) pECCGxAddr				uint32_t *�ͱ�������ָ�����洢����x���� : pECCGxAddr[0]Ϊ��λ
*														5) pECCGyAddr  			uint32_t *�ͱ�������ָ�����洢����y���� : pECCGyAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_ECDSA_Init(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct)
{
		uint8_t	i;

		for(i=0;i<8;i++)
		{
				HT_ECC->PREG = *ECC_ECDSA_InitStruct->pECCpAddr++;			//����ECC_p
				HT_ECC->AREG = *ECC_ECDSA_InitStruct->pECCaAddr++;			//����ECC_a
				HT_ECC->PXREG = *ECC_ECDSA_InitStruct->pECCGxAddr++;		//����ECC_Gx
				HT_ECC->PYREG = *ECC_ECDSA_InitStruct->pECCGyAddr++;		//����ECC_Gy
				HT_ECC->SXREG = *ECC_ECDSA_InitStruct->pECCnAddr++;			//����ECC_n
		}
}

/*
*********************************************************************************************************
*                              ECC PointOperate ������ʼ�� 
*
* ����˵��: ECC PointOperate������ʼ�� 
*
* ��ڲ���: ECC_PointOperate_InitStruct    PointOperate ECC������ʼ�� �����ṹ��ָ�룬��Ҫ����2������: 
*														1) pECCpAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�p : pECCpAddr[0]Ϊ��λ
*														2) pECCaAddr				uint32_t *�ͱ�������ָ�����洢�����ϵ����߲���a : pECCaAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_PointOperate_Init(ECC_PointOperate_InitTypedef* ECC_PointOperate_InitStruct)
{
		uint8_t	i;

		for(i=0;i<8;i++)
		{
				HT_ECC->PREG = *ECC_PointOperate_InitStruct->pECCpAddr++;			//����ECC_p
				HT_ECC->AREG = *ECC_PointOperate_InitStruct->pECCaAddr++;			//����ECC_a
		}
}

/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Signature operation
*
* ����˵��: ��Բ��������ǩ���㷨��ECDSA-S
*
* ��ڲ���: ECC_ECDSA_S_InputStruct    ECDSA-S������������ṹ��ָ�룬��Ҫ����3������: 
*								           	1) pdAddr						uint32_t *�ͱ�������ָ�����洢˽Կd : pdAddrΪ��λ            
*														2) pkAddr  					uint32_t *�ͱ�������ָ�����洢���������k: pkAddr[0]Ϊ��λ
*									   				3) pMAddr						uint32_t *�ͱ�������ָ�����洢HASH�����ϢժҪm: pMAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_ECDSA_S(ECC_ECDSA_Signature_InputTypedef* ECC_ECDSA_S_InputStruct)
{
		uint8_t	i;
		uint32_t tempreg = 0;

		tempreg = HT_ECC->ECCCON;
		tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ECCCON_OP_SEL_ECDSA_S);						
																											/*!< ��������ģʽΪ��ECDSA-S				*/
		for(i=0;i<8;i++)
		{
				HT_ECC->KEYREG = *ECC_ECDSA_S_InputStruct->pdAddr++;			//���� d
				HT_ECC->SYREG = *ECC_ECDSA_S_InputStruct->pkAddr++;				//���� k
				HT_ECC->MREG = *ECC_ECDSA_S_InputStruct->pMAddr++;				//���� M
		}
		HT_ECC->ECCSTA = 0x0;															/*!< �����־												*/			
		HT_ECC->ECCCON = tempreg;					
		HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;							/*!< ��������												*/
}

/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Verification operation
*
* ����˵��: ��Բ��������ǩ���㷨��ECDSA-V
*
* ��ڲ���: ECC_ECDSA_V_InputStruct    ECDSA-V������������ṹ��ָ�룬��Ҫ����5������: 
*								           	1) pECCDxAddr				uint32_t *�ͱ�������ָ�����洢��Կx���� : pECCDxAddr[0]Ϊ��λ            
*														2) pECCDyAddr  			uint32_t *�ͱ�������ָ�����洢��Կy���� : pECCDyAddr[0]Ϊ��λ
*									   				3) pMAddr						uint32_t *�ͱ�������ָ�����洢HASH�����ϢժҪm: pMAddr[0]Ϊ��λ
*														4) prAddr;					uint32_t *�ͱ�������ָ�����洢ǩ�����r����:prAddr[0]Ϊ��λ
*														5) psAddr;					uint32_t *�ͱ�������ָ�����洢ǩ�����s����:psAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_ECDSA_V(ECC_ECDSA_Verification_InputTypedef* ECC_ECDSA_V_InputStruct)
{
		uint8_t	i;
		uint32_t tempreg = 0;
	
		tempreg = HT_ECC->ECCCON;
		tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ECCCON_OP_SEL_ECDSA_V);						
																											/*!< ��������ģʽΪ��ECDSA-v				*/
		for(i=0;i<8;i++)
		{
				HT_ECC->KEYREG = *ECC_ECDSA_V_InputStruct->pECCDxAddr++;	//����ECC_Dx
				HT_ECC->SYREG = *ECC_ECDSA_V_InputStruct->pECCDyAddr++;		//����ECC_Dy
				HT_ECC->MREG = *ECC_ECDSA_V_InputStruct->pMAddr++;				//���� M
				HT_ECC->RXREG = *ECC_ECDSA_V_InputStruct->prAddr++;				//���� r
				HT_ECC->RYREG = *ECC_ECDSA_V_InputStruct->psAddr++;				//���� s
		}
		HT_ECC->ECCSTA = 0x0;															/*!< �����־												*/			
		HT_ECC->ECCCON = tempreg;					
		HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;							/*!< ��������												*/
}
/*
*********************************************************************************************************
*                              ECC Point operation����
*
* ����˵��: ��Բ���ߵ�����
*
* ��ڲ���: ECC_PointOperate_InputStruct    ��Բ���ߵ�������������ṹ��ָ�룬��Ҫ����10������: 
*														1) PointMode				ECC_PointMode_TypeDef������ECC������ģʽ
*																EccAdd_Mode ��P(X1,Y1) + S(X2,Y2) = R(X3,Y3)
*																EccDou_Mode ��2 *P(X1,Y1) = R(X3,Y3)
*																EccMul_Mode ��k*P(X1,Y1) = R(X3,Y3)
*														2) pECCPxAddr				uint32_t *�ͱ�����ECC���ߵ�1��x����: pECCPxAddr[0]Ϊ��λ
*														3) pECCPyAddr  			uint32_t *�ͱ�����ECC���ߵ�1��y����: pECCPyAddr[0]Ϊ��λ
*								           	4) pECCSxAddr				uint32_t *�ͱ�����ECC���ߵ�2��x����: pECCSxAddr[0]Ϊ��λ            
*														5) pECCSyAddr  			uint32_t *�ͱ�����ECC���ߵ�2��y���� : pECCSyAddr[0]Ϊ��λ
*									   				6) pKscalAddr				uint32_t *�ͱ�����ECC��˱���k: pKscalAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_PointOperate(ECC_PointOperate_InputTypedef* ECC_PointOperate_InputStruct)
{
		uint8_t	i;
		uint32_t tempreg = 0;
	
		tempreg = HT_ECC->ECCCON;
		tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_PointOperate_InputStruct->PointMode);						
																											/*!< ��������ģʽΪ��ECC ���				*/
		switch(ECC_PointOperate_InputStruct->PointMode)
		{
				case EccAdd_Mode:
					for(i=0;i<8;i++)
					{
							HT_ECC->PXREG = *ECC_PointOperate_InputStruct->pECCPxAddr++;		//����ECC_Px
							HT_ECC->PYREG = *ECC_PointOperate_InputStruct->pECCPyAddr++;		//����ECC_Py
							HT_ECC->SXREG = *ECC_PointOperate_InputStruct->pECCSxAddr++;		//����ECC_Sx
							HT_ECC->SYREG = *ECC_PointOperate_InputStruct->pECCSyAddr++;		//����ECC_Sy
					}	
					break;
				case EccDou_Mode:
					for(i=0;i<8;i++)
					{
							HT_ECC->PXREG = *ECC_PointOperate_InputStruct->pECCPxAddr++;		//����ECC_Px
							HT_ECC->PYREG = *ECC_PointOperate_InputStruct->pECCPyAddr++;		//����ECC_Py
					}
					break;
				case EccMul_Mode:
					for(i=0;i<8;i++)
					{
							HT_ECC->PXREG = *ECC_PointOperate_InputStruct->pECCPxAddr++;		//����ECC_Px
							HT_ECC->PYREG = *ECC_PointOperate_InputStruct->pECCPyAddr++;		//����ECC_Py
							HT_ECC->KEYREG = *ECC_PointOperate_InputStruct->pKscalAddr++;		//���� k
					}	
					break;
				default:
					break;
		}
		HT_ECC->ECCSTA = 0x0;															/*!< �����־												*/			
		HT_ECC->ECCCON = tempreg;					
		HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;							/*!< ��������												*/
}

/*
*********************************************************************************************************
*                              ECC Public Key Validation operation
*
* ����˵��: ��Բ���߹�Կ��֤��PKV
*
* ��ڲ���: ECC_PKV_InputStruct    PKV������������ṹ��ָ�룬��Ҫ����10������: 
*														1) pECCpAddr				uint32_t *�ͱ�������ָ�����洢����Ľ�p : pECCpAddr[0]Ϊ��λ
*														2) pECCaAddr				uint32_t *�ͱ�������ָ�����洢���߲���a : pECCaAddr[0]Ϊ��λ
*														3) pECCbAddr				uint32_t *�ͱ�������ָ�����洢���߲���b : pECCbAddr[0]Ϊ��λ
*														4) pECCPxAddr				uint32_t *�ͱ�������ָ�����洢����x���� : pECCPxAddr[0]Ϊ��λ
*														5) pECCPyAddr  			uint32_t *�ͱ�������ָ�����洢����y���� : pECCPyAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_PublickeyValidate(ECC_PKV_InputTypedef* ECC_PKV_InputStruct)
{
		uint8_t	i;
		uint32_t tempreg = 0;
	
		tempreg = HT_ECC->ECCCON;
		tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ECCCON_OP_SEL_PKV);						
																											/*!< ��������ģʽΪ��PKV				*/
		for(i=0;i<8;i++)
		{
				HT_ECC->PREG = *ECC_PKV_InputStruct->pECCpAddr++;			//����ECC_p
				HT_ECC->AREG = *ECC_PKV_InputStruct->pECCaAddr++;			//����ECC_a
				HT_ECC->PXREG = *ECC_PKV_InputStruct->pECCPxAddr++;		//����ECC_Px
				HT_ECC->PYREG = *ECC_PKV_InputStruct->pECCPyAddr++;		//����ECC_Py
				HT_ECC->SYREG = *ECC_PKV_InputStruct->pECCbAddr++;		//����ECC_b
		}
		HT_ECC->ECCSTA = 0x0;															/*!< �����־										*/			
		HT_ECC->ECCCON = tempreg;					
		HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;							/*!< ��������										*/
}


/*
*********************************************************************************************************
*                              Modular operation����
*
* ����˵��: ����ģ����
*
* ��ڲ���: ECC_ModOperate_InputStruct    ����ģ������������ṹ��ָ�룬��Ҫ����10������: 
*														1) ModMode				ECC_ModMode_TypeDef������ģ����ģʽ
*																ModAdd_Mode ��PX + PY ( mod n ) = RX
*																ModSub_Mode ��PX - PY ( mod n ) = RX
*																ModMul_Mode ��PX * PY ( mod n ) = RX
*																ModDiv_Mode ��PY / PX ( mod n ) = RX
*																ModInv_Mode ��PX-1 ( mod n ) = RX
*														2) pnAddr				uint32_t *�ͱ���������ģn : pnAddr[0]Ϊ��λ
*														3) pPxAddr				uint32_t *�ͱ�������1������PX: pPxAddr[0]Ϊ��λ
*														4) pPyAddr				uint32_t *�ͱ�������2������PX: pPyAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_ModOperate(ECC_ModOperate_InputTypedef* ECC_ModOperate_InputStruct)
{
		uint8_t	i;
		uint32_t tempreg = 0;
	
		tempreg = HT_ECC->ECCCON;
		tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ModOperate_InputStruct->ModMode);						
																											/*!< ��������ģʽΪ��ECC ���				*/
		switch(ECC_ModOperate_InputStruct->ModMode)
		{
				case ModAdd_Mode:
				case ModSub_Mode:
				case ModMul_Mode:
				case ModDiv_Mode:	
					for(i=0;i<8;i++)
					{
							HT_ECC->PREG = *ECC_ModOperate_InputStruct->pnAddr++;			//����n
							HT_ECC->PXREG = *ECC_ModOperate_InputStruct->pPxAddr++;		//����Px
							HT_ECC->PYREG = *ECC_ModOperate_InputStruct->pPyAddr++;		//����Py
					}	
					break;
				case ModInv_Mode:
					for(i=0;i<8;i++)
					{
							HT_ECC->PREG = *ECC_ModOperate_InputStruct->pnAddr++;			//����n
							HT_ECC->PXREG = *ECC_ModOperate_InputStruct->pPxAddr++;		//����Px
					}
					break;
				default:
					break;
		}
		HT_ECC->ECCSTA = 0x0;															/*!< �����־												*/			
		HT_ECC->ECCCON = tempreg;					
		HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;							/*!< ��������												*/
}



/*
*********************************************************************************************************
*                                      ECC���������ݶ�ȡ MODULE
*
* ����˵��: ECC���������ݶ�ȡ 
*
* ��ڲ���: ECC_PointOperate_OutputStruct   ECC���������ݽṹ��ָ�룬��Ҫ����10������: 
*										1) pRxAddr				uint32_t *�ͱ�����ECC��������x����/ECDSAǩ�����r: pRxAddr[0]Ϊ��λ
*										2) pRyAddr				uint32_t *�ͱ�����ECC��������y����/ECDSAǩ�����s: pRyAddr[0]Ϊ��λ
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/
void HT_ECC_PointOperate_Read(ECC_PointOperate_OutputTypedef* ECC_PointOperate_OutputStruct)
{
		uint8_t i;
	
		ECC_PointOperate_OutputStruct->pRxAddr += 7;
		ECC_PointOperate_OutputStruct->pRyAddr += 7;
		for(i=0;i<8;i++)
		{
				*ECC_PointOperate_OutputStruct->pRxAddr-- = HT_ECC->RXREG;
				*ECC_PointOperate_OutputStruct->pRyAddr-- = HT_ECC->RYREG;
		}
}

/*
*********************************************************************************************************
*                                      ����ģ���������ݶ�ȡ MODULE
*
* ����˵��: ����ģ���������ݶ�ȡ 
*
* ��ڲ���: pModOperateOutAddr   uint32_t *�ͱ���,����ģ����������ָ��
*   
*
* ���ز���: ��                                      
* 
* ����˵��: 
*********************************************************************************************************
*/

void HT_ECC_ModOperate_Read(uint32_t *pModOperateOutAddr)
{
		uint8_t i;
	
		pModOperateOutAddr += 7;
		for(i=0;i<8;i++)
		{
				*pModOperateOutAddr-- = HT_ECC->RXREG;
		}
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE ECC INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�ECC�ж�
*
* ��ڲ���: 
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_ECC_ITConfig(FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_ECC->ECCCON |= (uint32_t)ECC_ECCCON_ECCIE;            /*!< ʹ��ECC�ж�           */
    }
    else
    {
        HT_ECC->ECCCON &= ~(uint32_t)ECC_ECCCON_ECCIE;           /*!< �ر�ECC�ж�           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED ECC INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦECC�жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����ECC�жϱ�־������Ϊ���²���:
*                        @arg ECC_IF
*                        @arg ECC_FLG_BUSY
*                        @arg ECC_FLG_ECDSA_V
*                        @arg ECC_FLG_ECDSA_S
*                        @arg ECC_FLG_PKV
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_ECC_ITFlagStatusGet(ECC_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */
    
    if (HT_ECC->ECCSTA & ITFlag)
    {       
        return SET;                        /*!< ECC Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< ECC Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR ECC INTERRUPT FLAG
*
* ����˵��: ���ECC�жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��ECC�жϱ�־������Ϊ���²����������:
*                        @arg ECC_IF
*                        @arg ECC_FLG_BUSY
*                        @arg ECC_FLG_ECDSA_V
*                        @arg ECC_FLG_ECDSA_S
*                        @arg ECC_FLG_PKV
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_ECC_ClearITPendingBit(ECC_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */
    
      
    HT_ECC->ECCSTA  &= ~((uint32_t)ITFlag);                  /*!< Clear ECC Interrupt Flag       */
    
}

/*
*********************************************************************************************************
*                            GET ECC BUSY STATUS
*
* ����˵��: ��ȡ��ӦECC��BUSY״̬
*
* ��ڲ���: ��
*
* ���ز���: ITStatus    = SET��  busyæµ
*                       = RESET��busy����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_ECC_Busy_StatusGet(void)
{
    /*  assert_param  */
    return (ITStatus)(HT_ECC->ECCSTA & ECC_FLG_BUSY);               /*!< ����ECC busy status  				*/
}


#endif                                        /* This File Only support HT6x2x and HT501x */

