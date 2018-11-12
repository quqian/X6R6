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
* File         : ht6xxx_cmu.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_CMU_H__
#define __HT6XXX_CMU_H__

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
* @brief  ϵͳʱ��Դѡ����
*/
typedef enum
{ 
    SysLRC = CMU_SYSCLKCFG_CLKSEL_LRC,            /*!< Fsys = �ڲ���ƵRCʱ��            */
    SysLF  = CMU_SYSCLKCFG_CLKSEL_LF,             /*!< Fsys = �ⲿ����ʱ�� or mems/16   */
    SysHRCDiv1 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x00), /*!< Fsys = �ڲ���ƵRCʱ��/1          */
    SysHRCDiv2 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x10), /*!< Fsys = �ڲ���ƵRCʱ��/2          */
    SysHRCDiv4 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x20), /*!< Fsys = �ڲ���ƵRCʱ��/4          */
    SysHRCDiv8 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x30), /*!< Fsys = �ڲ���ƵRCʱ��/8          */
    SysPLL = CMU_SYSCLKCFG_CLKSEL_PLL,            /*!< Fsys = �ڲ�PLLʱ��               */
#if  defined  HT6x1x ||  defined  HT6x2x
    SysMEMS= CMU_SYSCLKCFG_CLKSEL_MEMS,           /*!< Fsys = �ڲ�memsʱ��              */
#endif
#if defined HT501x || defined  HT6x2x  ||  defined  HT502x
    SysPLLX2= (CMU_SYSCLKCFG_CLKSEL_PLL|0x08),    /*!< Fsys = �ڲ�PLLʱ�� X 2           */
#endif
}SYSCLK_SelectTypeDef;                             /*!< end of group SYSCLK_SelectTypeDef*/  

/* 
* @brief  Clkoutʱ��Դѡ����
*/
typedef enum
{ 
    ClkoutFLRC = CMU_CLKOUTSEL_LRC,     /*!< �ڲ���ƵRCʱ�� ---> Clkout       */
    ClkoutFLF  = CMU_CLKOUTSEL_LF,      /*!< osc or mems/16 ---> Clkout       */
    ClkoutFHRC = CMU_CLKOUTSEL_HRC,     /*!< �ⲿ��ƵRCʱ�� ---> Clkout       */
    ClkoutFSys = CMU_CLKOUTSEL_SYS,     /*!< �ڲ�PLLʱ��    ---> Clkout       */
#if  defined  HT6x1x ||  defined  HT6x2x    
    ClkoutFMEMS= CMU_CLKOUTSEL_MEMS,    /*!< �ڲ�memsʱ��   ---> Clkout       */
#endif
}Clkout_SelectTypeDef;                  /*!< end of group Clkout_SelectTypeDef*/ 

/* 
* @brief  CPUʱ�ӷ�Ƶ����
*/
typedef enum
{ 
    CPUDiv1   = CMU_CPUCLKDIV_1,        /*!< Fcpu=Fsys                        */
    CPUDiv2   = CMU_CPUCLKDIV_2,        /*!< Fcpu=Fsys/2                      */
    CPUDiv4   = CMU_CPUCLKDIV_4,        /*!< Fcpu=Fsys/4                      */
    CPUDiv8   = CMU_CPUCLKDIV_8,        /*!< Fcpu=Fsys/8                      */
    CPUDiv16  = CMU_CPUCLKDIV_16,       /*!< Fcpu=Fsys/16                     */
    CPUDiv32  = CMU_CPUCLKDIV_32,       /*!< Fcpu=Fsys/32                     */
    CPUDiv64  = CMU_CPUCLKDIV_64,       /*!< Fcpu=Fsys/64                     */
    CPUDiv128 = CMU_CPUCLKDIV_128,      /*!< Fcpu=Fsys/128                    */    
}CPU_DivTypeDef;                        /*!< end of group CPU_DivTypeDef      */ 

/* 
* @brief  ϵͳʱ������ģ���ʼ���ṹ����
*/
typedef struct
{
   SYSCLK_SelectTypeDef  SysClkSel;    /*!< ϵͳʱ��ѡ��                      */
   CPU_DivTypeDef        CPUDiv;       /*!< CPUʱ�ӷ�Ƶ                       */
}CMU_InitTypeDef;                      /*!< end of group CMU_InitTypeDef      */

/* 
* @brief  ϵͳʱ������ģ���ʼ���ṹ����
*/
typedef struct
{
   Clkout_SelectTypeDef  ClkoutSel;    /*!< Clkoutʱ��Դѡ��                  */
   uint8_t  ClkoutDiv;                 /*!< Clkout���ʱ�ӷ�Ƶ����            
                                            ClkoutDiv ���÷�ΧΪ��0~15
                                            ��Ƶϵ�� = 2*(ClkoutDiv[3..0]+1)  */
}Clkout_InitTypeDef;                   /*!< end of group Clkout_InitTypeDef   */


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
void HT_CMU_Init(CMU_InitTypeDef* CMU_InitStruct);
uint32_t HT_CMU_SysClkGet(void);
uint32_t HT_CMU_CPUClkGet(void);
void HT_CMU_ClkoutSet(Clkout_InitTypeDef* Clkout_InitStruct);
void HT_CMU_ClkCtrl0Config(uint32_t ClkCtrl0Module, FunctionalState NewState);
void HT_CMU_ClkCtrl1Config(uint32_t ClkCtrl1Module, FunctionalState NewState);
Bool HT_CMU_JTAGStatusGet(void);
FlagStatus HT_CMU_StopFlagGet(uint8_t STAFlag);
void	HT_CMU_FlashDly_Set(FunctionalState NewState);
void	HT_CMU_Prefetch_Set(FunctionalState NewState);
#ifdef __cplusplus
}
#endif

#endif /* __HT6xxx_CMU_H__ */ 
