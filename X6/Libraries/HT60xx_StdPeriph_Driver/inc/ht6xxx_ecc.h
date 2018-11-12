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
* File         : ht6xxx_ecc.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT502x 
**********************************************************************************************************
*/

#ifndef __HT6XXX_ECC_H__
#define __HT6XXX_ECC_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
#if defined  HT502x             /* This File Only support HT502x */

    
/*
*********************************************************************************************************
*                                           ȫ�ֺ�/�ṹ��
*********************************************************************************************************
*/
	
/* 
* @brief  ����ģ����ģʽ����
*/
typedef enum
{ 
    ModAdd_Mode = ECC_ECCCON_OP_SEL_MA,       /*!< ģ������                     */
    ModSub_Mode = ECC_ECCCON_OP_SEL_MS,       /*!< ģ������                     */
    ModMul_Mode = ECC_ECCCON_OP_SEL_MM,       /*!< ģ������                  		*/
		ModDiv_Mode = ECC_ECCCON_OP_SEL_MD,				/*!< ģ������                  		*/
		ModInv_Mode = ECC_ECCCON_OP_SEL_MI,				/*!< ģ������                  		*/
}ECC_ModMode_TypeDef;                										/*!<end of group ECC_ModMode_TypeDef  */	

/* 
* @brief  ECC������ģʽ����
*/
typedef enum
{ 
    EccAdd_Mode = ECC_ECCCON_OP_SEL_ECA,       /*!< ECC�������                   */
    EccDou_Mode = ECC_ECCCON_OP_SEL_ECD,       /*!< ECC��������                   */
    EccMul_Mode = ECC_ECCCON_OP_SEL_ECSM,      /*!< ECC�������                  	*/
}ECC_PointMode_TypeDef;                										/*!<end of group ECC_PointMode_TypeDef  */	

/* 
* @brief  ECC�жϱ�־����
*/
typedef enum
{ 
    ECC_IF = ECC_ECCSTA_ECCFLG,       							/*!< ECC��������жϱ�־            	*/
    ECC_FLG_BUSY = ECC_ECCSTA_BUSY,       					/*!< ECC����busy��־                  */
		ECC_FLG_ECDSA_V = ECC_ECCSTA_ECDSA_V,       		/*!< ECDSA��֤��־		                */
		ECC_FLG_ECDSA_S = ECC_ECCSTA_ECDSA_S,						/*!< ECDSAǩ�����Ա�־     		        */
		ECC_FLG= ECC_ECCSTA_PKV,												/*!< ECDSA��Կ��֤��־   	            */
}ECC_ITFlagTypeDef;

/* 
* @brief  Modular operation���� ��������ṹ����
*/
typedef struct
{
			ECC_ModMode_TypeDef	ModMode;				//ģ����ģʽ
			uint32_t *pnAddr;										//����ģn
			uint32_t *pPxAddr;									//��1������PX
			uint32_t *pPyAddr;									//��2������PY
}ECC_ModOperate_InputTypedef;							/*!< end of group ECC_ModOperate_InputTypedef*/

/* 
* @brief  ECC PointOperate Initial�����ṹ����
*/
typedef struct
{
			uint32_t *pECCpAddr;								//����Fp�Ľ�p
			uint32_t *pECCaAddr;								//ECC���߲���a
}ECC_PointOperate_InitTypedef;						/*!< end of group ECC_PointOperate_InitTypedef*/
/* 
* @brief  ECC Point operation���� ��������ṹ����
*/
typedef struct
{
			ECC_PointMode_TypeDef	PointMode;		//ECC������ģʽ
			uint32_t *pECCPxAddr;								//ECC���ߵ�1��x����
			uint32_t *pECCPyAddr;								//ECC���ߵ�1��y����
			uint32_t *pECCSxAddr;								//ECC���ߵ�2��x����
			uint32_t *pECCSyAddr;								//ECC���ߵ�2��y����
			uint32_t *pKscalAddr;								//ECC��˱���k
}ECC_PointOperate_InputTypedef;						/*!< end of group ECC_PointOperate_InputTypedef*/

/* 
* @brief  ECC Point operation���� ��������ṹ����
*/
typedef struct
{
			uint32_t *pRxAddr;									//ECC��������x����
			uint32_t *pRyAddr;									//ECC��������y����
}ECC_PointOperate_OutputTypedef;				/*!< end of group ECC_ModOperate_InputTypedef*/

/* 
* @brief  ECDSA Initial�����ṹ����
*/
typedef struct
{
			uint32_t *pECCpAddr;								//����Fp�Ľ�p
			uint32_t *pECCaAddr;								//ECC���߲���a
			uint32_t *pECCGxAddr;								//ECC���߻���x����
			uint32_t *pECCGyAddr;								//ECC���߻���y����
			uint32_t *pECCnAddr;								//ECC���߻���Ľ�n
}ECC_ECDSA_InitTypedef;									/*!< end of group ECC_ECDSA_InitTypedef*/

/* 
* @brief  ECDSA Signature operation���� ��������ṹ����
*/
typedef struct
{
			uint32_t *pdAddr;										//˽Կd
			uint32_t *pkAddr;										//���������k
			uint32_t *pMAddr;										//HASH�����ϢժҪM
}ECC_ECDSA_Signature_InputTypedef;						/*!< end of group ECC_ECDSA_Signature_InputTypedef*/

/* 
* @brief  ECDSA Verification operation���� ��������ṹ����
*/
typedef struct
{
			uint32_t *pECCDxAddr;								//ECC���߹�Կx����
			uint32_t *pECCDyAddr;								//ECC���߹�Կy����
			uint32_t *pMAddr;										//HASH�����ϢժҪM
			uint32_t *prAddr;										//ǩ�����r����
			uint32_t *psAddr;										//ǩ�����s����
}ECC_ECDSA_Verification_InputTypedef;						/*!< end of group ECC_ECDSA_Verification_InputTypedef*/

/* 
* @brief  ECC Public Key Validation��Կ��֤ ���� ��������ṹ����
*/
typedef struct
{
			uint32_t *pECCpAddr;								//����Fp�Ľ�p
			uint32_t *pECCaAddr;								//ECC���߲���a
			uint32_t *pECCbAddr;								//ECC���߲���b
			uint32_t *pECCPxAddr;								//ECC���ߵ�x����
			uint32_t *pECCPyAddr;								//ECC���ߵ�y����
}ECC_PKV_InputTypedef;						/*!< end of group ECC_PKV_InputTypedef*/



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
void HT_ECC_PointOperate_Init(ECC_PointOperate_InitTypedef* ECC_PointOperate_InitStruct);
void HT_ECC_ECDSA_Init(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct);
void HT_ECC_ECDSA_S(ECC_ECDSA_Signature_InputTypedef* ECC_ECDSA_S_InputStruct);
void HT_ECC_ECDSA_V(ECC_ECDSA_Verification_InputTypedef* ECC_ECDSA_V_InputStruct);
void HT_ECC_PointOperate(ECC_PointOperate_InputTypedef* ECC_PointOperate_InputStruct);
void HT_ECC_PublickeyValidate(ECC_PKV_InputTypedef* ECC_PKV_InputStruct);
void HT_ECC_ModOperate(ECC_ModOperate_InputTypedef* ECC_ModOperate_InputStruct);
void HT_ECC_PointOperate_Read(ECC_PointOperate_OutputTypedef* ECC_PointOperate_OutputStruct);
void HT_ECC_ModOperate_Read(uint32_t *pModOperateOutAddr);
void HT_ECC_ITConfig(FunctionalState NewState);
ITStatus HT_ECC_ITFlagStatusGet(ECC_ITFlagTypeDef ITFlag);
void HT_ECC_ClearITPendingBit(ECC_ITFlagTypeDef ITFlag);
ITStatus HT_ECC_Busy_StatusGet(void);



#endif                                        /* This File Only support HT6x2x ��HT501xand HT502x */

#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_ECC_H__ */ 
