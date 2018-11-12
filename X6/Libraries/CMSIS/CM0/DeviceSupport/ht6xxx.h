/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2014, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.2
* Description  : HT6x2x, HT6x1x, HT501x, HT502x are supported
**********************************************************************************************************
*/

#ifndef __HT6XXX_H__
#define __HT6XXX_H__

#ifdef __cplusplus
extern "C" {
#endif     
  
/*
**********************************************************************************************************
*                                             ȫ�ֺ�/�ṹ��
**********************************************************************************************************
*/
#if !defined (HT6x2x) && !defined (HT6x1x) && !defined (HT501x) && !defined (HT502x)
 #error "Please select one HT device used in your application"
#endif

#if defined (HT6x1x)
 #if defined (HT6x2x) || defined (HT501x) || defined (HT502x)
   #error "More than one HT device used in your application"
 #endif
#elif defined (HT6x2x)
 #if defined (HT501x) || defined (HT502x)
   #error "More than one HT device used in your application"
 #endif
#else
 #if defined (HT501x) &&  defined (HT502x)
   #error "More than one HT device used in your application"
 #endif
#endif

#define  CurrentVersion   Version1_7_20161226
    
/* 
* @brief  General Bool State  
*/ 
typedef enum {DISABLE = 0, ENABLE = !DISABLE}   FunctionalState;        /* ����ʹ��/��ʹ��            */
typedef enum {RESET   = 0, SET    = !RESET  }   FlagStatus, ITStatus;   /* ����״̬�����жϱ�־��1��0 */
typedef enum {FALSE   = 0, TRUE   = !FALSE  }   Bool;                   /* ���ں�������               */

    
/*
**********************************************************************************************************
*                                       Interrupt Number Definition
**********************************************************************************************************
*/  
typedef enum IRQn
{
/*****************************  Cortex-M0 Processor Exceptions Numbers  *********************************/
    NonMaskableInt_IRQn         = -14,      /*!< 2 Non Maskable Interrupt  CMU                    */
    HardFault_IRQn              = -13,      /*!< 3 Cortex-M0 Hard Fault Interrupt                 */
    SVCall_IRQn                 = -5,       /*!< 11 Cortex-M0 SV Call Interrupt                   */
    PendSV_IRQn                 = -2,       /*!< 14 Cortex-M0 Pend SV Interrupt                   */
    SysTick_IRQn                = -1,       /*!< 15 Cortex-M0 System Tick Interrupt               */

/*****************************  HT6xxx Specific Interrupt Numbers  **************************************/
    PMU_IRQn                    = 0,        /*!< PMU Interrupt                                    */
#if defined  HT6x1x
    TDES_IRQn                   = 1,        /*!< 3DES Interrupt                                   */
#else
    AES_IRQn                    = 1,        /*!< AES Interrupt                                    */
																						/*!<   HT6x2x: includ AES, RAND, GHASH                */
																						/*!<   HT501x: includ AES, GHASH                      */
																						/*!<   HT502x: includ AES, RAND, GHASH, ECC           */
#endif
    EXTI0_IRQn                  = 2,        /*!< External Interrupt 0 Interrupt                   */
    EXTI1_IRQn                  = 3,        /*!< External Interrupt 1 Interrupt                   */
    EXTI2_IRQn                  = 4,        /*!< External Interrupt 2 Interrupt                   */
    EXTI3_IRQn                  = 5,        /*!< External Interrupt 3 Interrupt                   */
    EXTI4_IRQn                  = 6,        /*!< External Interrupt 4 Interrupt                   */
    EXTI5_IRQn                  = 7,        /*!< External Interrupt 5 Interrupt                   */
    EXTI6_IRQn                  = 8,        /*!< External Interrupt 6 Interrupt                   */
    UART0_IRQn                  = 9,        /*!< UART0 Interrupt                                  */
    UART1_IRQn                  = 10,       /*!< UART1 Interrupt                                  */
    UART2_IRQn                  = 11,       /*!< UART2 Interrupt                                  */
    UART3_IRQn                  = 12,       /*!< UART3 Interrupt                                  */
    UART4_IRQn                  = 13,       /*!< UART4 Interrupt                                  */
    UART5_IRQn                  = 14,       /*!< UART5 Interrupt                                  */
    TIMER_0_IRQn                = 15,       /*!< Timer0 Interrupt                                 */
    TIMER_1_IRQn                = 16,       /*!< Timer1 Interrupt                                 */
    TIMER_2_IRQn                = 17,       /*!< Timer2 Interrupt                                 */
    TIMER_3_IRQn                = 18,       /*!< Timer3 Interrupt                                 */
    TBS_IRQn                    = 19,       /*!< TBS Interrupt                                    */
    RTC_IRQn                    = 20,       /*!< RTC Interrupt                                    */
    I2C_IRQn                    = 21,       /*!< I2C Converter Interrupt                          */
    SPI0_IRQn                   = 22,       /*!< SPI0 Interrupt                                   */
#if   defined HT6x1x
    WDT_IRQn                    = 23,       /*!< WDT Interrupt                                    */
#elif defined HT501x
    EMU_IRQn                    = 25,       /*!< EMU Interrupt                                    */
    DMA_IRQn                    = 26,       /*!< DMA Interrupt                                    */
    KEY_IRQn                    = 27,       /*!< KEY Scan Interrupt                               */
#elif defined HT502x
    EMU_IRQn                    = 25,       /*!< EMU Interrupt                                    */
    DMA_IRQn                    = 26,       /*!< DMA Interrupt                                    */
    KEY_IRQn                    = 27,       /*!< KEY Scan Interrupt                               */
    EXTI7_IRQn                  = 28,       /*!< External Interrupt 7 Interrupt                   */
    EXTI8_IRQn                  = 29,       /*!< External Interrupt 8 Interrupt                   */
    EXTI9_IRQn                  = 30,       /*!< External Interrupt 9 Interrupt                   */
#elif defined HT6x2x
    SPI1_IRQn                   = 23,       /*!< SPI1 Interrupt                                   */
    SelfTestF_IRQn              = 24,       /*!< SelfTestF Interrupt                              */
    TIMER_4_IRQn                = 25,       /*!< Timer4 Interrupt                                 */
    TIMER_5_IRQn                = 26,       /*!< Timer5 Interrupt                                 */
    UART6_IRQn                  = 27,       /*!< UART6 Interrupt                                  */
    EXTI7_IRQn                  = 28,       /*!< External Interrupt 7 Interrupt                   */
    EXTI8_IRQn                  = 29,       /*!< External Interrupt 8 Interrupt                   */
    EXTI9_IRQn                  = 30,       /*!< External Interrupt 9 Interrupt                   */
    DMA_IRQn                    = 31,	      /*!< DMA Interrupt                   	    	          */
#endif
} IRQn_Type;


/*
**********************************************************************************************************
*                                   Processor and Core Peripheral Section
**********************************************************************************************************
*/  

/********************  Configuration of the Cortex-M0 Processor and Core Peripherals  *******************/
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
                                            /* end of group HT60xx_CMSIS                          */

#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include <stdint.h>                         /* standard types definitions                         */


/*
**********************************************************************************************************
*                                 Device Specific Peripheral Registers structures 
**********************************************************************************************************
*/  

/*****************************************  Timer Control Block  *****************************************/
/* 
* @brief  ��ʱ���Ĵ�������
*/
typedef struct
{
    __IO uint32_t TMRCON;                   /*!< Offset: 0x000 ͨ�����ƼĴ���      (R/W) */
    __IO uint32_t TMRDIV;                   /*!< Offset: 0x004 ͨ��Ԥ��Ƶ�Ĵ���    (R/W) */
    __IO uint32_t TMRPRD;                   /*!< Offset: 0x008 ͨ�����ڼĴ���      (R/W) */
    __IO uint32_t TMRCAP;                   /*!< Offset: 0x00C ͨ���������ݼĴ���  (R/W) */
    __IO uint32_t TMRCNT;                   /*!< Offset: 0x010 ͨ���������Ĵ���    (R/W) */
    __IO uint32_t TMRCMP;                   /*!< Offset: 0x014 ͨ���Ƚ����Ĵ���    (R/W) */
    __IO uint32_t TMRIE;                    /*!< Offset: 0x018 ͨ���ж�ʹ�ܼĴ���  (R/W) */
    __IO uint32_t TMRIF;                    /*!< Offset: 0x01C ͨ���жϱ�־�Ĵ���  (R/W) */
} HT_TMR_TypeDef;                           /* end of group HT_TMR_TypeDef               */


/*****************************************  UART Control Block  ******************************************/
/* 
* @brief  UART�Ĵ�������
*/
typedef struct
{          
    __IO uint32_t MODESEL;                  /*!< Offset: 0x000 UART/7816����ѡ��Ĵ���    (R/W) */
    __IO uint32_t UARTCON;                  /*!< Offset: 0x004 ���ڹ������üĴ���         (R/W) */
    __IO uint32_t ISO7816CON;               /*!< Offset: 0x008 7816�������üĴ���         (R/W) */ 
    __IO uint32_t SREL;                     /*!< Offset: 0x00C ����/7816�����ʷ���������ֵ(R/W) */
    __IO uint32_t SBUF;                     /*!< Offset: 0x010 ����/7816���ݻ���Ĵ���    (R/W) */
    __IO uint32_t UARTSTA;                  /*!< Offset: 0x014 ����״̬�Ĵ���             (R/W) */
    __IO uint32_t ISO7816STA;               /*!< Offset: 0x018 7816״̬�Ĵ���             (R/W) */ 
    __I  uint32_t Reserved[5];  
    __IO uint32_t IRCON;                    /*!< Offset: 0x030 ������ƿ��ƼĴ���         (R/W) */ 
    __IO uint32_t IRDUTY;                   /*!< Offset: 0x034 ���������������Ĵ���     (R/W) */ 
#if defined  HT6x2x    
    __IO uint32_t LOGDETNUM;                /*!< Offset: 0x038 485�����߼��ж����ݳ�������(R/W) */ 
#endif   
} HT_UART_TypeDef, HT_ISO7816_TypeDef;      /* end of group HT_UART_TypeDef, HT_ISO7816_TypeDef */


/*****************************************  I2C Control Block  *******************************************/
/*
* @brief  I2C�Ĵ�������
*/
typedef struct
{
    __IO uint32_t I2CDAT;                   /*!< Offset: 0x000 I2C���ݼĴ��� (R/W) */
    __IO uint32_t I2CADR;                   /*!< Offset: 0x004 I2C��ַ�Ĵ��� (R/W) */
    __IO uint32_t I2CCON;                   /*!< Offset: 0x008 I2C���ƼĴ��� (R/W) */
    __IO uint32_t I2CSTA;                   /*!< Offset: 0x00C I2C״̬�Ĵ��� (R/W) */
} HT_I2C_TypeDef;                           /* end of group HT_I2C_TypeDef         */


/*****************************************  SPI Control Block  *******************************************/
/*
* @brief  SPI�Ĵ�������
*/
typedef struct
{
    __IO uint32_t SPICON;                   /*!< Offset: 0x000 SPI���ƼĴ���     (R/W) */
    __IO uint32_t SPISTA;                   /*!< Offset: 0x004 SPI״̬�Ĵ���     (R/W) */
    __IO uint32_t SPIDAT;                   /*!< Offset: 0x008 SPI���ݼĴ���     (R/W) */
    __IO uint32_t SPISSN;                   /*!< Offset: 0x00C SPI�ӻ�ѡ��Ĵ��� (R/W) */
} HT_SPI_TypeDef;                           /* end of group HT_SPI_TypeDef             */
        
        
/*****************************************  RTC Control Block  *******************************************/
/*
* @brief  RTC�Ĵ�������
*/
typedef struct
{
    __IO uint32_t RTCCON;                   /*!< Offset: 0x000 RTC���ƼĴ��� (R/W)                     */
    __IO uint32_t RTCIE;                    /*!< Offset: 0x004 RTC�ж�ʹ�ܼĴ��� (R/W)                 */
    __IO uint32_t RTCIF;                    /*!< Offset: 0x008 RTC�жϱ�־�Ĵ��� (R/W)                 */
    __IO uint32_t ALMR;                     /*!< Offset: 0x00C ���ӼĴ��� (R/W)                        */
    __IO uint32_t RTCTMR1;                  /*!< Offset: 0x010 RTC��ʱ��1�������� (R/W)                */
    __IO uint32_t RTCTMR2;                  /*!< Offset: 0x014 RTC��ʱ��2�������� (R/W)                */
    __IO uint32_t SECR;                     /*!< Offset: 0x018 ��Ĵ��� (R/W)                          */
    __IO uint32_t MINR;                     /*!< Offset: 0x01C ���ӼĴ��� (R/W)                        */
    __IO uint32_t HOURR;                    /*!< Offset: 0x020 Сʱ�Ĵ��� (R/W)                        */
    __IO uint32_t DAYR;                     /*!< Offset: 0x024 ��Ĵ��� (R/W)                          */
    __IO uint32_t MONTHR;                   /*!< Offset: 0x028 �¼Ĵ��� (R/W)                          */
    __IO uint32_t YEARR;                    /*!< Offset: 0x02C ��Ĵ��� (R/W)                          */
    __IO uint32_t WEEKR;                    /*!< Offset: 0x030 ���ڼĴ��� (R/W)                        */
    __IO uint32_t RTCCNTH;                  /*!< Offset: 0x034 Уʱ�����Ĵ�����16λ (R/W )             */
    __IO uint32_t RTCCNTL;                  /*!< Offset: 0x038 Уʱ�����Ĵ�����16λ (R/W )             */
    __IO uint32_t RTCRD;                    /*!< Offset: 0x03C RTC�����ƼĴ��� (R/W )                  */
    __IO uint32_t RTCWR;                    /*!< Offset: 0x040 RTCд���ƼĴ��� (R/W)                   */
    __I  uint32_t Reserved[3];  
    __IO uint32_t DFAH;                     /*!< Offset: 0x050 Memsʱ��Ƶ��ʱ�ĳ������7λ�� (R/W )  */
    __IO uint32_t DFAL;                     /*!< Offset: 0x054 Memsʱ��Ƶ��ʱ�ĳ������16λ��(R/W )  */
    __IO uint32_t DFBH;                     /*!< Offset: 0x058 Memsʱ��Ƶ��ʱ��һ�����7λ�� (R/W )  */
    __IO uint32_t DFBL;                     /*!< Offset: 0x05C Memsʱ��Ƶ��ʱ��һ�����16λ��(R/W )  */
    __IO uint32_t DFCH;                     /*!< Offset: 0x060 Memsʱ��Ƶ��ʱ�Ķ������7λ�� (R/W )  */
    __IO uint32_t DFCL;                     /*!< Offset: 0x064 Memsʱ��Ƶ��ʱ�Ķ������16λ��(R/W )  */
    __IO uint32_t DFDH;                     /*!< Offset: 0x068 Memsʱ��Ƶ��ʱ���������7λ�� (R/W )  */
    __IO uint32_t DFDL;                     /*!< Offset: 0x06C Memsʱ��Ƶ��ʱ���������16λ��(R/W )  */
    __IO uint32_t DFEH;                     /*!< Offset: 0x070 Memsʱ��Ƶ��ʱ���Ĵ����7λ�� (R/W )  */
    __IO uint32_t DFEL;                     /*!< Offset: 0x074 Memsʱ��Ƶ��ʱ���Ĵ����16λ��(R/W )  */ 
    __IO uint32_t Toff;                     /*!< Offset: 0x078 �¶�ƫ�üĴ��� (R/W )                   */ 
    __IO uint32_t MCON01;                   /*!< Offset: 0x07C Mems���ƼĴ���01 (R/W )                 */
    __IO uint32_t MCON23;                   /*!< Offset: 0x080 Mems���ƼĴ���23 (R/W )                 */
    __IO uint32_t MCON45;                   /*!< Offset: 0x084 Mems���ƼĴ���45 (R/W )                 */
    __IO uint32_t DFIH;                     /*!< Offset: 0x088 Ƶ�����Ĵ�������5λ�� (R/W )          */
    __IO uint32_t DFIL;                     /*!< Offset: 0x08C Ƶ�����Ĵ�������16λ�� (R/W )         */
    __IO uint32_t RTCRSTFLG;                /*!< Offset: 0x090 RTCģ�鸴λ��־�Ĵ��� (R/W )            */
    __IO uint32_t RTCRSTSET;                /*!< Offset: 0x094 RTCд��λ�Ĵ��� (R/W )                  */
#if defined  HT501x  ||  HT502x
    __I  uint32_t Reserved2[22];
#elif defined  HT6x2x    
    __I  uint32_t RTCCHKSUM;                /*!< Offset: 0x098 RTCд��λ�Ĵ��� (R/W )                  */    
    __I  uint32_t Reserved2[13];    
    __IO uint32_t HRa;                      /*!< Offset: 0x0D0 HRC1Hz���������� (R/W)                  */
    __IO uint32_t HRb;                      /*!< Offset: 0x0D4 HRC1Hz����һ���� (R/W)                  */  
    __IO uint32_t HRc;                      /*!< Offset: 0x0D8 HRC1Hz���������� (R/W)                  */ 
    __IO uint32_t HRd;                      /*!< Offset: 0x0DC HRC1Hz���������� (R/W)                  */                    
    __IO uint32_t HRe;                      /*!< Offset: 0x0E0 HRC1Hz�����Ĵ��� (R/W)                  */ 
    __IO uint32_t HDFi;                     /*!< Offset: 0x0E4 HRC1HzƵ�����Ĵ��� (R/W)              */  
    __I  uint32_t Reserved3[2]; 
#endif
#if defined  HT501x  || HT502x  || HT6x2x
    __IO uint32_t HFCFG;                    /*!< Offset: 0x0F0 ��Ƶģ�����üĴ��� ���û������� (R/W)   */ 
    __IO uint32_t HFNUM;                    /*!< Offset: 0x0F4 ���ò�Ƶ�������ڸ��� ������(R/W)        */
    __IO uint32_t HFCNTH;                   /*!< Offset: 0x0F8 ��Ƶ������16λ ������(R/W)              */
    __IO uint32_t HFCNTL;                   /*!< Offset: 0x0FC ��Ƶ������16λ ������(R/W)              */                 
    __IO uint32_t CTRLBYFLASH;              /*!< Offset: 0x100 �˼Ĵ������û������� (R/W )             */
	  __IO uint32_t LRCCOMAND;                /*!< Offset: 0x104 �˼Ĵ������û������� (R/W )             */
    __I  uint32_t Reserved4[62];                
    __IO uint32_t SECR2;                    /*!< Offset: 0x200 ��Ĵ��� (R/W)                          */
    __IO uint32_t MINR2;                    /*!< Offset: 0x204 ���ӼĴ��� (R/W)                        */
    __IO uint32_t HOURR2;                   /*!< Offset: 0x208 Сʱ�Ĵ��� (R/W)                        */
    __IO uint32_t DAYR2;                    /*!< Offset: 0x20C ��Ĵ��� (R/W)                          */
    __IO uint32_t MONTHR2;                  /*!< Offset: 0x210 �¼Ĵ��� (R/W)                          */
    __IO uint32_t YEARR2;                   /*!< Offset: 0x214 ��Ĵ��� (R/W)                          */
    __IO uint32_t WEEKR2;                   /*!< Offset: 0x218 ���ڼĴ��� (R/W)                        */   
    __IO uint32_t RTC2CAL;                  /*!< Offset: 0x21C LRC��ʱ�����Ĵ��� (R/W)                 */
		__IO uint32_t RTCRD2;                   /*!< Offset: 0x220 RTC�����ƼĴ��� (R/W )                  */ 
    __IO uint32_t RTCWR2;                   /*!< Offset: 0x224 RTCд���ƼĴ��� (R/W )                  */ 
#endif  
#if defined  HT501x  || HT502x
    __I  uint32_t FRE_LRC;                  /*!< Offset: 0x228 LRCƵ�ʲ���ֵ (R)                       */
#endif
} HT_RTC_TypeDef;                           /* end of group HT_RTC_TypeDef                             */


/*****************************************  LCD Control Block  *******************************************/
/*
* @brief  LCD�Ĵ�������
*/
typedef struct
{
    __IO uint32_t LCDCLK;                   /*!< Offset: 0x000 LCDʱ��Ƶ��ѡ��Ĵ��� (R/W)  */
    __IO uint32_t LCDCON;                   /*!< Offset: 0x004 LCD�������ƼĴ��� (R/W)      */
    __IO uint32_t Reserved;
	  __IO uint32_t LCDOUT;                   /*!< Offset: 0x00C LCD������ƼĴ��� (R/W)      */
#if defined  HT6x1x
    __IO uint32_t LCDBUF[37];               /*!< Offset: 0x010 LCD��ʾ���ݼĴ��� (R/W)      */
#elif defined  HT502x
    __IO uint32_t LCDBUF[53];               /*!< Offset: 0x010 LCD��ʾ���ݼĴ��� (R/W)      */
#else
    __IO uint32_t LCDBUF[43];               /*!< Offset: 0x010 LCD��ʾ���ݼĴ��� (R/W)      */
#endif
} HT_LCD_TypeDef;                           /* end of group HT_LCD_TypeDef                  */


/*****************************************  TBS Control Block  *******************************************/
/*
* @brief  TBS�Ĵ�������
*/
typedef struct
{
#if defined HT501x  || HT502x
    __IO uint32_t TBSCON;                   /*!< Offset: 0x000 TBS���üĴ��� (R/W)      */
    __IO uint32_t TBSPRD;                   /*!< Offset: 0x004 ��Ƶ�����üĴ��� (R/W) */    
    __IO uint32_t TBSIE;                    /*!< Offset: 0x008 TBS�ж�ʹ�ܼĴ��� (R/W)  */
    __IO uint32_t TBSIF;                    /*!< Offset: 0x00C TBS�жϱ�־�Ĵ��� (R/W)  */
    __I  uint32_t TMPDAT;                   /*!< Offset: 0x010 �¶Ȳ������ֵ (R/)      */
    __I  uint32_t VBATDAT;                  /*!< Offset: 0x014 ADCͨ��2�������ֵ (R/)  */
    __I  uint32_t VCCDAT;                   /*!< Offset: 0x018 ��Դ��ѹ�������ֵ(R/)   */
																						/*!<               HT502x��Ӧ�ֲ���VDDDAT   */
    __I  uint32_t ADC0DAT;                  /*!< Offset: 0x01C ADCͨ��0�������ֵ (R/)  */
    __I  uint32_t ADC1DAT;                  /*!< Offset: 0x020 ADCͨ��1�������ֵ (R/)  */
    __I  uint32_t ADC2DAT;                  /*!< Offset: 0x024 ADCͨ��2�������ֵ (R/)  */    
    __IO uint32_t VBATCMP;                  /*!< Offset: 0x028 VBAT�ȽϼĴ��� (R/W)     */
    __I  uint32_t Reserved[2];             
    __IO uint32_t ADC0CMP;                  /*!< Offset: 0x034 ADC0�ȽϼĴ��� (R/W)     */
    __IO uint32_t ADC1CMP;                  /*!< Offset: 0x038 ADC1�ȽϼĴ��� (R/W)     */    
#else
    __IO uint32_t TBSCON;                   /*!< Offset: 0x000 TBS���üĴ��� (R/W)      */
    __IO uint32_t TBSIE;                    /*!< Offset: 0x004 TBS�ж�ʹ�ܼĴ��� (R/W)  */
    __IO uint32_t TBSIF;                    /*!< Offset: 0x008 TBS�жϱ�־�Ĵ��� (R/W)  */
    __I  uint32_t TMPDAT;                   /*!< Offset: 0x00C �¶Ȳ������ֵ (R/)      */
    __I  uint32_t VBATDAT;                  /*!< Offset: 0x010 Vbat�������ֵ (R/)      */
    __I  uint32_t ADC0DAT;                  /*!< Offset: 0x014 ADCͨ��0�������ֵ (R/)  */
    __I  uint32_t ADC1DAT;                  /*!< Offset: 0x018 ADCͨ��1�������ֵ (R/)  */
    __IO uint32_t VBATCMP;                  /*!< Offset: 0x01C VBAT�ȽϼĴ��� (R/W)     */
    __IO uint32_t TBSPRD;                   /*!< Offset: 0x020 ��Ƶ�����üĴ��� (R/W) */
    __IO uint32_t TBSTEST;                  /*!< Offset: 0x024 TBS���ԼĴ��� (R/W)      */
    __I  uint32_t VCCDAT;                   /*!< Offset: 0x028 ��Դ��ѹ�������ֵ(R/)*/
#if defined  HT6x2x
    __IO uint32_t ADC0CMP;                  /*!< Offset: 0x02C ADC0�ȽϼĴ��� (R/)      */  
		__IO uint32_t ADC3DAT;                  /*!< Offset: 0x030 ADC3�������ֵ (R/)      */ 
		__IO uint32_t ADC4DAT;                  /*!< Offset: 0x034 ADC4�������ֵ (R/)      */ 
		__IO uint32_t ADC5DAT;                  /*!< Offset: 0x038 ADC5�������ֵ (R/)      */ 
		__IO uint32_t TBSPRD2;                  /*!< Offset: 0x03C ��Ƶ�����üĴ��� (R/W) */
#endif  
#endif    
} HT_TBS_TypeDef;                           /* end of group HT_TBS_TypeDef              */


/*****************************************  PMU Control Block  *******************************************/
/*
* @brief  PMU�Ĵ�������
*/
typedef struct
{
    __IO uint32_t PMUCON;                   /*!< Offset: 0x000 PMU���üĴ��� (R/W)              */
    __IO uint32_t VDETCFG;                  /*!< Offset: 0x004 ��Դ�����ֵ���üĴ��� (R/W)     */
    __IO uint32_t VDETPCFG;                 /*!< Offset: 0x008 ��Դ���ʱ���������üĴ��� (R/W) */
    __IO uint32_t PMUIE;                    /*!< Offset: 0x00C PMU�ж�ʹ�ܼĴ��� (R/W)          */
    __IO uint32_t PMUIF;                    /*!< Offset: 0x010 PMU�жϱ�־�Ĵ��� (R/W)          */
    __I  uint32_t PMUSTA;                   /*!< Offset: 0x014 PMU״ָ̬ʾ�Ĵ��� (R)          */
    __IO uint32_t WAKEIF;                   /*!< Offset: 0x018 ����Դ��־�Ĵ��� (R/W)           */
#if defined HT6x2x	
	__IO uint32_t PDTFLT;                   /*!< Offset: 0x01C POW�����˲��Ĵ��� (R/W)           */
    __I  uint32_t Reserved[4];
#else 
    __I  uint32_t Reserved[5];
#endif	
    __IO uint32_t RSTSTA;                   /*!< Offset: 0x030 ��λ��־�Ĵ��� (R/W)             */
} HT_PMU_TypeDef;                           /* end of group HT_PMU_TypeDef                      */


/*****************************************  CMU Control Block  *******************************************/
/*
* @brief  CMU�Ĵ�������
*/
typedef struct
{
    __IO uint32_t WPREG;                    /*!< Offset: 0x000 д�������ƼĴ��� (R/W)                    */
    __IO uint32_t SYSCLKCFG;                /*!< Offset: 0x004 ϵͳʱ�����üĴ��� (R/W)                  */
    __I  uint32_t JTAGSTA;                  /*!< Offset: 0x008 JTAG״ָ̬ʾ (R)                          */
    __IO uint32_t LRCADJ;                   /*!< Offset: 0x00C ��ƵRC���üĴ��� (R/W )                   */
    __IO uint32_t HRCADJ;                   /*!< Offset: 0x010 ��ƵRC���üĴ��� (R/W)                    */
    __IO uint32_t HRCDIV;                   /*!< Offset: 0x014 ��ƵRC��Ƶ�Ĵ��� (R/W)                    */
    __I  uint32_t CLKSTA;                   /*!< Offset: 0x018 ʱ��״̬�Ĵ��� (R)                        */
    __IO uint32_t SYSCLKDIV;                /*!< Offset: 0x01C ϵͳʱ�ӷ�Ƶ�Ĵ��� (R/W)                  */
    __I  uint32_t Reserved1[1];             
    __IO uint32_t CLKOUTSEL;                /*!< Offset: 0x024 CLKOUTʱ��ѡ��Ĵ��� (R/W)                */
    __IO uint32_t CLKOUTDIV;                /*!< Offset: 0x028 CLKOUTʱ�ӷ�Ƶ�Ĵ��� (R/W)                */
    __IO uint32_t CLKCTRL0;                 /*!< Offset: 0x02C �ڲ�ģ��ʹ�ܼĴ���0 (R/W)                 */
    __IO uint32_t CLKCTRL1;                 /*!< Offset: 0x030 �ڲ�ģ��ʹ�ܼĴ���1 (R/W)                 */
    __IO uint32_t FLASHCON;                 /*!< Offset: 0x034 Flash���ʿ��ƼĴ��� (R/W)                 */
    __O  uint32_t FLASHLOCK;                /*!< Offset: 0x038 Flash�����Ĵ��� (W)                       */
#if defined  HT6x2x     
    __IO uint32_t PREFETCH;                 /*!< Offset: 0x03C ָ��Ԥȡʹ�ܼĴ��� (R/W)                  */  
    __I  uint32_t Reserved2[4]; 
#elif defined HT501x  ||  defined HT502x
    __IO uint32_t FLASHDLY;                 /*!< Offset: 0x03C Flashȡָ��ȴ� (R/W)                     */  
    __I  uint32_t Reserved2[4];     
#else
    __I  uint32_t Reserved2[5]; 
#endif      
    __IO uint32_t INFOLOCK;                 /*!< Offset: 0x050 Info Flash�����Ĵ���(�����Ÿ��û�) (R/W)  */
    __IO uint32_t PORRSTSET;                /*!< Offset: 0x054 POR/LBOR��λ�Ĵ���(�����Ÿ��û�) (R/W)    */
#if defined  HT501x    
    __I  uint32_t Reserved4[2+10*4+14*64];             
    __I  uint32_t CHIPID;                   /*!< Offset: 0xF00 ChipID (R)                                */    
#elif defined  HT6x2x   
    __I  uint32_t Reserved3[2];
    __IO uint32_t RCCALICON;                /*!< Offset: 0x060 RCУ�����üĴ��� (R/W)                    */
    __IO uint32_t RCCALIIE;                 /*!< Offset: 0x064 RC��Ƶ�ж�ʹ�ܼĴ��� (R/W)                */
    __IO uint32_t RCCALIIF;                 /*!< Offset: 0x068 RC��Ƶ�жϱ�־�Ĵ��� (R/W)                */
    __IO uint32_t HRCVALUE;                 /*!< Offset: 0x06C HRC��Ƶֵ(24λ�޷�����) (R/W)             */     
    __IO uint32_t LRCVALUE;                 /*!< Offset: 0x070 LRC��Ƶֵ(17λ�޷�����) (R/W)             */ 
    __I  uint32_t Reserved4[2+10*4+14*64-7];             
    __I  uint32_t CHIPID;                   /*!< Offset: 0xF00 ChipID (R)                                */
#elif defined  HT502x   
    __I  uint32_t Reserved3[2];
    __IO uint32_t FS1LOCK;                  /*!< Offset: 0x060 FlashSector1 �����Ĵ��� (R/W)            */
    __IO uint32_t FS2LOCK;                  /*!< Offset: 0x064 FlashSector2 �����Ĵ��� (R/W)            */
    __IO uint32_t FS3LOCK;                  /*!< Offset: 0x068 FlashSector3 �����Ĵ��� (R/W)            */
    __IO uint32_t FS4LOCK;                  /*!< Offset: 0x06C FlashSector4 �����Ĵ��� (R/W)            */     
    __IO uint32_t FS5LOCK;                  /*!< Offset: 0x070 FlashSector5 �����Ĵ��� (R/W)            */ 
    __IO uint32_t FS6LOCK;                  /*!< Offset: 0x074 FlashSector6 �����Ĵ��� (R/W)            */
    __IO uint32_t FS7LOCK;                  /*!< Offset: 0x078 FlashSector7 �����Ĵ��� (R/W)            */
    __IO uint32_t FS8LOCK;                  /*!< Offset: 0x07C FlashSector8 �����Ĵ��� (R/W)            */
    __IO uint32_t FS9LOCK;                  /*!< Offset: 0x080 FlashSector9 �����Ĵ��� (R/W)            */     
    __IO uint32_t FSALOCK;                  /*!< Offset: 0x084 FlashSectorA �����Ĵ��� (R/W)            */ 
    __IO uint32_t FSBLOCK;                  /*!< Offset: 0x088 FlashSectorB �����Ĵ��� (R/W)            */
    __I  uint32_t Reserved4[1];
    __IO uint32_t FLASHCON2;                /*!< Offset: 0x090 Flash���ʿ��ƼĴ���2 (R/W)               */
    __I  uint32_t Reserved5[2+10*4+14*64-7-8];             
    __I  uint32_t CHIPID;                   /*!< Offset: 0xF00 ChipID (R)                                */
#endif
} HT_CMU_TypeDef;                           /* end of group HT_CMU_TypeDef                               */


/*****************************************  WDT Control Block  *******************************************/
/*
* @brief  WDT�Ĵ�������
*/
typedef struct
{   
#if defined HT6x1x	
    __IO uint32_t WDTCFG;                   /*!< Offset: 0x000 ���Ź����üĴ��� (R/W)           */ 
#else 
		__I  uint32_t Reserved;
#endif
    __IO uint32_t WDTCLR;                   /*!< Offset: 0x004 ���Ź�ι����ʱ�����üĴ��� (R/W) */
    __I  uint32_t WDTCNT;                   /*!< Offset: 0x008 ���Ź������Ĵ���(R)              */
} HT_WDT_TypeDef;                           /* end of group HT_WDT_TypeDef                      */


/*****************************************  GPIO Control Block  ******************************************/
/*
* @brief GPIO�Ĵ�������
*/
typedef struct
{
    __IO uint32_t IOCFG;                    /*!< Offset: 0x000 �˿ڹ������üĴ���1 (R/W)  */
    __IO uint32_t AFCFG;                    /*!< Offset: 0x004 �˿ڹ������üĴ���2 (R/W)  */
    __IO uint32_t PTDIR;                    /*!< Offset: 0x008 �˿ڷ������üĴ��� (R/W)   */
    __IO uint32_t PTUP;                     /*!< Offset: 0x00C �˿��������üĴ��� (R/W)   */
    __IO uint32_t PTDAT;                    /*!< Offset: 0x010 �˿����ݼĴ��� (R/W)       */
    __O  uint32_t PTSET;                    /*!< Offset: 0x014 �˿����üĴ��� ( /W)       */
    __O  uint32_t PTCLR;                    /*!< Offset: 0x018 �˿ڸ�λ�Ĵ��� ( /W)       */
    __O  uint32_t PTTOG;                    /*!< Offset: 0x01C �˿ڷ�ת�Ĵ��� ( /W)       */
    __IO uint32_t PTOD;                     /*!< Offset: 0x020 �˿�OD�������üĴ��� (R/W) */
} HT_GPIO_TypeDef;                          /* end of group HT_GPIO_TypeDef               */


/*****************************************  INT Control Block  *******************************************/
/*
* @brief INT�жϼĴ�������
*/
typedef struct
{
    __IO uint32_t EXTIE;                    /*!< Offset: 0x000 �ⲿ�ж�ʹ�ܼĴ��� (R/W) */
    __IO uint32_t EXTIF;                    /*!< Offset: 0x004 �ⲿ�жϱ�־�Ĵ��� (R/W) */
  	__IO uint32_t PINFLT;                    /*!< Offset: 0x008 ���������˲�ʹ�ܼĴ��� (R/W) */
#if defined  HT6x2x  || HT502x
    __IO uint32_t Reserved[1];                            
    __IO uint32_t EXTIE2;                   /*!< Offset: 0x010 �ⲿ�ж�ʹ�ܼĴ��� (R/W) */
    __IO uint32_t EXTIF2;                   /*!< Offset: 0x014 �ⲿ�жϱ�־�Ĵ��� (R/W) */  
	  __IO uint32_t PINFLT2;                  /*!< Offset: 0x0018 ���������˲�ʹ�ܼĴ��� (R/W) */
#endif    
} HT_INT_TypeDef;                           /* end of group HT_INT_TypeDef              */


/*****************************************  DMA Control Block  *******************************************/
/*
* @brief DMA�Ĵ�������
*/
#if defined HT501x  || defined HT502x || defined HT6x2x
typedef struct
{
    __IO uint32_t DMAIE;                    /*!< Offset: 0x000 �ж�ʹ�ܼĴ��� (R/W)       */
    __IO uint32_t DMAIF;                    /*!< Offset: 0x004 ͨ���жϱ�־ (R/W)         */
    __IO uint32_t CHNSTA;                   /*!< Offset: 0x008 ״̬�Ĵ��� (R/W)           */
} HT_DMA_TypeDef;                           /* end of group HT_DMA_TypeDef                */


typedef struct
{
    __IO uint32_t CHNCTL;                   /*!< Offset: 0x000 ͨ�����ƼĴ��� (R/W)       */
    __IO uint32_t CHNSRC;                   /*!< Offset: 0x004 ͨ��Դ��ַ�Ĵ��� (R/W)     */
    __IO uint32_t CHNTAR;                   /*!< Offset: 0x008 ͨ��Ŀ�ĵ�ַ�Ĵ��� (R/W)   */ 
    __IO uint32_t CHNCNT;                   /*!< Offset: 0x00C ͨ�������������üĴ���(R/W)*/
    __IO uint32_t CHNTCCNT;                 /*!< Offset: 0x010 ͨ������������ݸ��� (R/W) */
    __IO uint32_t CHNBULKNUM;               /*!< Offset: 0x014 ͨ���鴫���ڸ������� (R/W) */               
} HT_DMA_Channel_TypeDef;                   /* end of group HT_DMA_Channel_TypeDef        */
#endif


/*****************************************  CRC Control Block  *******************************************/
/*
* @brief CRC�Ĵ�������
*/
#if defined  HT6x2x
typedef struct
{
    __IO uint32_t CRCCON;                    /*!< Offset: 0x000 CRC���ƼĴ��� (R/W)        */
    __IO uint32_t CRCDAT;                    /*!< Offset: 0x004 CRC���ݼĴ��� (R/W)        */          
} HT_CRC_TypeDef;                            /* end of group HT_CRC_TypeDef              	 */
#endif


/*****************************************  AES Control Block  *******************************************/
/*
* @brief AES�Ĵ�������
*/
#if defined HT501x  || defined HT502x || defined HT6x2x
typedef struct
{
    __IO uint32_t AESCON;                    /*!< Offset: 0x000 AES���üĴ��� (R/W)                       */
    __O  uint32_t AESSTR;                    /*!< Offset: 0x004 AES��������Ĵ��� (W)                     */   
    __IO uint32_t AESFLG;                    /*!< Offset: 0x008 AES��־�Ĵ��� (R/W)                       */
    __IO uint32_t AESINLL;                   /*!< Offset: 0x00C AES���������/�������ݵ�λ�Ĵ��� (R/W)    */  
    __IO uint32_t AESINML;                   /*!< Offset: 0x010 AES���������/���������е�λ�Ĵ��� (R/W)  */  
    __IO uint32_t AESINHM;                   /*!< Offset: 0x014 AES���������/���������и�λ�Ĵ��� (R/W)  */
    __IO uint32_t AESINHH;                   /*!< Offset: 0x018 AES���������/�������ݸ�λ�Ĵ��� (R/W)    */ 
    __IO uint32_t AESOUTLL;                  /*!< Offset: 0x02C AES����Ѽ���/�������ݵ�λ�Ĵ��� (R/W)    */
    __IO uint32_t AESOUTML;                  /*!< Offset: 0x020 AES����Ѽ���/���������е�λ�Ĵ��� (R/W)  */ 
    __IO uint32_t AESOUTHM;                  /*!< Offset: 0x024 AES����Ѽ���/���������и�λ�Ĵ��� (R/W)  */ 
    __IO uint32_t AESOUTHH;                  /*!< Offset: 0x028 AES����Ѽ���/�������ݸ�λ�Ĵ��� (R/W)    */
    __IO uint32_t AESKEY0;                   /*!< Offset: 0x02C AES������Կ�Ĵ��� (R/W)                   */	
    __IO uint32_t AESKEY1;                   /*!< Offset: 0x030 AES������Կ�Ĵ��� (R/W)                   */
    __IO uint32_t AESKEY2;                   /*!< Offset: 0x034 AES������Կ�Ĵ��� (R/W)                   */
    __IO uint32_t AESKEY3;                   /*!< Offset: 0x038 AES������Կ�Ĵ��� (R/W)                   */
    __IO uint32_t AESKEY4;                   /*!< Offset: 0x03C AES������Կ�Ĵ��� (R/W)                   */
    __IO uint32_t AESKEY5;                   /*!< Offset: 0x040 AES������Կ�Ĵ��� (R/W)                   */
    __IO uint32_t AESKEY6;                   /*!< Offset: 0x044 AES������Կ�Ĵ��� (R/W)                   */
    __IO uint32_t AESKEY7;                   /*!< Offset: 0x048 AES������Կ�Ĵ��� (R/W)                   */
} HT_AES_TypeDef;                            /* end of group HT_AES_TypeDef                               */
#endif


#if defined  HT6x2x  || defined HT502x
typedef struct
{
    __IO uint32_t RANDSTR;                    /*!< Offset: 0x000 ���������������Ĵ��� (R/W)             */
    __IO uint32_t RANDDAT;                    /*!< Offset: 0x004 ����������ݼĴ��� (R/W)                 */   
} HT_RAND_TypeDef;                            /* end of group HT_RAND_TypeDef                             */
#endif


#if defined HT501x || defined HT6x2x  || defined HT502x
typedef struct
{
    __O  uint32_t GHASHSTR;                   /*!< Offset: 0x000 GHASH��������Ĵ��� (W)                   */
    __IO uint32_t GHASHFLG;                   /*!< Offset: 0x004 GHASH��־�Ĵ��� (R/W)                     */   
    __IO uint32_t INPUT1LL;                   /*!< Offset: 0x008 GHASH��������1��λ (R/W)                  */
    __IO uint32_t INPUT1ML;                   /*!< Offset: 0x00C GHASH��������1�е�λ (R/W)                */
    __IO uint32_t INPUT1HM;                   /*!< Offset: 0x010 GHASH��������1�и�λ (R/W)                */		
    __IO uint32_t INPUT1HH;                   /*!< Offset: 0x014 GHASH��������1��λ (R/W)                  */
    __IO uint32_t INPUT2LL;                   /*!< Offset: 0x018 GHASH��������2��λ (R/W)                  */
    __IO uint32_t INPUT2ML;                   /*!< Offset: 0x01C GHASH��������2�е�λ (R/W)                */
    __IO uint32_t INPUT2HM;                   /*!< Offset: 0x020 GHASH��������2�и�λ (R/W)                */
    __IO uint32_t INPUT2HH;                   /*!< Offset: 0x024 GHASH��������2��λ (R/W)                  */
    __IO uint32_t OUTPUTLL;                   /*!< Offset: 0x028 GHASH������ݵ�λ (R/W)                   */
    __IO uint32_t OUTPUTML;                   /*!< Offset: 0x02C GHASH��������е�λ (R/W)                 */
    __IO uint32_t OUTPUTHM;                   /*!< Offset: 0x030 GHASH��������и�λ (R/W)                 */
    __IO uint32_t OUTPUTHH;                   /*!< Offset: 0x034 GHASH������ݸ�λ (R/W)                   */
    __IO uint32_t AESGHASHIE;                 /*!< Offset: 0x038 AES/GHASH�ж�ʹ��λ (R/W)                 */
    __IO uint32_t AESGHASHIF;                 /*!< Offset: 0x03C AES/GHASH�жϱ�־λ (R/W)                 */
} HT_GHASH_TypeDef;                           /* end of group HT_GHASH_TypeDef                             */
#endif



/*****************************************  EMU Control Block  *******************************************/
/*
* @brief EMU EPR����
*/
#if defined  HT501x  ||  defined  HT502x
typedef struct
{										 
  __I uint32_t SPLI1;                       /*!< Offset: 0x000  ����ͨ��1ADC�������� (R) */
  __I uint32_t SPLI2;                       /*!< Offset: 0x004  ����ͨ��2ADC�������� (R) */
  __I uint32_t SPLU;                        /*!< Offset: 0x008  ��ѹͨ��ADC�������� (R)  */
  __I uint32_t SPLP;                        /*!< Offset: 0x00C  �й����ʲ������� (R)     */
  __I uint32_t SPLQ;                        /*!< Offset: 0x010  �޹����ʲ������� (R)     */
  __I uint32_t FASTRMSI1;                   /*!< Offset: 0x014  ���ٵ���ͨ��1����Чֵ (R)*/
  __I uint32_t FASTRMSI2;                   /*!< Offset: 0x018  ���ٵ���ͨ��2����Чֵ (R)*/
  __I uint32_t FASTRMSU;                    /*!< Offset: 0x01C  ���ٵ�ѹͨ������Чֵ (R) */
  __I uint32_t FREQU;                       /*!< Offset: 0x020  ��ѹƵ�� (R)             */
  __I uint32_t FASTPOWERP1;                 /*!< Offset: 0x024  ���ٵ�һ·�й����� (R)   */
  __I uint32_t FASTPOWERQ1;                 /*!< Offset: 0x028  ���ٵ�һ·�޹����� (R)   */
  __I uint32_t FASTPOWERP2;                 /*!< Offset: 0x02C  ���ٵڶ�·�й����� (R)   */
  __I uint32_t FASTPOWERQ2;                 /*!< Offset: 0x030  ���ٵڶ�·�޹����� (R)   */
  __I uint32_t FASTPOWERS1;                 /*!< Offset: 0x034  ���ٵ�һ·���ڹ��� (R)   */
  __I uint32_t FASTPOWERS2;                 /*!< Offset: 0x038  ���ٵڶ�·���ڹ��� (R)   */
  __I uint32_t RMSI1;                       /*!< Offset: 0x03C  ���ٵ���ͨ��1����Чֵ (R)*/
  __I uint32_t RMSI2;                       /*!< Offset: 0x040  ���ٵ���ͨ��2����Чֵ (R)*/
  __I uint32_t RMSU;                        /*!< Offset: 0x044  ���ٵ�ѹͨ������Чֵ (R) */
  __I uint32_t POWERP1;                     /*!< Offset: 0x048  ���ٵ�һ·�й����� (R)   */
  __I uint32_t POWERQ1;                     /*!< Offset: 0x04C  ���ٵ�һ·�޹����� (R)   */
  __I uint32_t POWERP2;                     /*!< Offset: 0x050  ���ٵڶ�·�й����� (R)   */
  __I uint32_t POWERQ2;                     /*!< Offset: 0x054  ���ٵڶ�·�޹����� (R)   */
  __I uint32_t POWERS1;                     /*!< Offset: 0x058  ���ٵ�һ·���ڹ��� (R)   */
  __I uint32_t POWERS2;                     /*!< Offset: 0x05C  ���ٵڶ�·���ڹ��� (R)   */ 
  __I uint32_t ENERGYP;                     /*!< Offset: 0x060  �й����� (R)             */
  __I uint32_t ENERGYQ;                     /*!< Offset: 0x064  �޹����� (R)             */
  __I uint32_t ENERGYS;                     /*!< Offset: 0x068  �������� (R)             */
  __I uint32_t ENERGYPC;                    /*!< Offset: 0x06C  ������0���й����� (R)    */
  __I uint32_t ENERGYQC;                    /*!< Offset: 0x070  ������0���޹����� (R)    */
  __I uint32_t ENERGYSC;                    /*!< Offset: 0x074  ������0���������� (R)    */
  __I uint32_t DC_UAVERAGE;                 /*!< Offset: 0x078  ֱ����ѹƽ��ֵ (R)       */
  __I uint32_t DC_I1AVERAGE;                /*!< Offset: 0x07C  ֱ������ͨ��1ƽ��ֵ (R)  */
  __I uint32_t DC_I2AVERAGE;                /*!< Offset: 0x080  ֱ������ͨ��2ƽ��ֵ (R)  */
  __I uint32_t CHECKSUM;                    /*!< Offset: 0x084  У��ͼĴ��� (R)         */
  __I uint32_t UPEAK;                       /*!< Offset: 0x088  ��ѹ���η�ֵ�Ĵ��� (R)   */
  __I uint32_t I1PEAK;                      /*!< Offset: 0x08C  I1�������η�ֵ�Ĵ��� (R) */ 
  __I uint32_t I2PEAK;                      /*!< Offset: 0x090  I2�������η�ֵ�Ĵ��� (R) */  
  __I uint32_t PFCNT_PHOTO;                 /*!< Offset: 0x094  PFCNT���� (R)            */
  __I uint32_t QFCNT_PHOTO;                 /*!< Offset: 0x098  QFCNT���� (R)            */ 
  __I uint32_t SFCNT_PHOTO;                 /*!< Offset: 0x09C  SFCNT���� (R)            */    
  __I uint32_t AUTOUGAIN;                   /*!< Offset: 0x0A0  Uͨ���¶��Զ�����ϵ�� (R) */
  __I uint32_t AUTOI1GAIN;                  /*!< Offset: 0x0A4  I1ͨ���¶��Զ�����ϵ��(R) */ 
  __I uint32_t AUTOI2GAIN;                  /*!< Offset: 0x0A8  I2ͨ���¶��Զ�����ϵ��(R) */   
  __I uint32_t R_Buffer;                    /*!< Offset: 0x0AC  buffer��ȡ���� (R)       */ 
  __I uint32_t Reserved[2]; 
  __I uint32_t UdetCNT;                     /*!< Offset: 0x0B8  SAG/Peak��������ʱ������Ĵ���(R) */ 
} HT_EMU_EPR_TypeDef;                       /* end of group HT_EMU_EPR_TypeDef           */
/*
* @brief EMU ECR����
*/
typedef struct
{										 
  __IO uint32_t EMUSR;                     /*!< Offset: 0x000  EMU״̬��־�Ĵ���                 (R/W)              */
  __IO uint32_t EMUIE;                     /*!< Offset: 0x004  EMU�ж�ʹ�ܼĴ���                 (R/W)              */
  __IO uint32_t EMUIF;                     /*!< Offset: 0x008  EMU�жϱ�־�Ĵ���                 (R/W)              */
  __IO uint32_t GP1;                       /*!< Offset: 0x00C  ͨ��1���й�����У��               (R/W)              */
  __IO uint32_t GQ1;                       /*!< Offset: 0x010  ͨ��1���޹�����У��               (R/W)              */
  __IO uint32_t GS1;                       /*!< Offset: 0x014  ͨ��1�����ڹ���У��               (R/W)              */
  __IO uint32_t GPHS1;                     /*!< Offset: 0x018  ͨ��1����λУ��                   (R/W)              */
  __IO uint32_t GP2;                       /*!< Offset: 0x01C  ͨ��2���й�����У��               (R/W)              */
  __IO uint32_t GQ2;                       /*!< Offset: 0x020  ͨ��2���޹�����У��               (R/W)              */
  __IO uint32_t GS2;                       /*!< Offset: 0x024  ͨ��2�����ڹ���У��               (R/W)              */
  __IO uint32_t GPHS2;                     /*!< Offset: 0x028  ͨ��2����λУ��                   (R/W)              */
  __IO uint32_t QPHSCAL;                   /*!< Offset: 0x02C  �޹���λ����                      (R/W)              */
  __IO uint32_t I2GAIN;                    /*!< Offset: 0x030  ����ͨ��2���油��                 (R/W)              */
  __IO uint32_t I1OFF;                     /*!< Offset: 0x034  ����ͨ��1��ƫ��У��               (R/W)              */
  __IO uint32_t I2OFF;                     /*!< Offset: 0x038  ����ͨ��2��ƫ��У��               (R/W)              */
  __IO uint32_t UOFF;                      /*!< Offset: 0x03C  ��ѹͨ����ƫ��У��                (R/W)              */
  __IO uint32_t PSTART;                    /*!< Offset: 0x040  �𶯹�������                      (R/W)              */
  __IO uint32_t QSTART;                    /*!< Offset: 0x044  �𶯹�������                      (R/W)              */
  __IO uint32_t SSTART;                    /*!< Offset: 0x048  �𶯹�������                      (R/W)              */
  __IO uint32_t HFCONST;                   /*!< Offset: 0x04C  �������Ƶ������                  (R/W)              */
  __IO uint32_t ADCCFG;                    /*!< Offset: 0x050  ADC���ƼĴ���                     (R/W)              */
  __IO uint32_t CHNLCR;                    /*!< Offset: 0x054  ͨ�����ƼĴ���                    (R/W)              */
  __IO uint32_t EMCON;                     /*!< Offset: 0x058  �����������ƼĴ���                (R/W)              */
  __IO uint32_t PFCNT;                     /*!< Offset: 0x05C  �����й��������                  (R/W)              */
  __IO uint32_t QFCNT;                     /*!< Offset: 0x060  �����޹��������                  (R/W)              */   
  __IO uint32_t SFCNT;                     /*!< Offset: 0x064  ���������������                  (R/W)              */
  __IO uint32_t ADCCON;                    /*!< Offset: 0x068  ADCͨ������ѡ��                   (R/W)              */
  __IO uint32_t IPTAMP;                    /*!< Offset: 0x06C  �Ե�����ֵ                      (R/W)              */
  __IO uint32_t ICHK;                      /*!< Offset: 0x070  �Ե���ֵ����                      (R/W)              */
  __IO uint32_t EMUCTRL;                   /*!< Offset: 0x074  EMU���ƼĴ���                     (R/W)              */
  __IO uint32_t P1OFFSET;                  /*!< Offset: 0x078  ͨ��1�й�����С�ź�ƫ��У��        (R/W)              */
  __IO uint32_t P2OFFSET;                  /*!< Offset: 0x07C  ͨ��2�й�����С�ź�ƫ��У��        (R/W)              */
  __IO uint32_t Q1OFFSET;                  /*!< Offset: 0x080  ͨ��1�޹�����С�ź�ƫ��У��        (R/W)              */
  __IO uint32_t Q2OFFSET;                  /*!< Offset: 0x084  ͨ��2�޹�����С�ź�ƫ��У��        (R/W)              */
  __IO uint32_t I1RMSOFFSET;               /*!< Offset: 0x088  ����ͨ��1��ЧֵС�ź�У���Ĵ���    (R/W)              */
  __IO uint32_t I2RMSOFFSET;               /*!< Offset: 0x08C  ����ͨ��2��ЧֵС�ź�У���Ĵ���    (R/W)              */
  __IO uint32_t URMSOFFSET;                /*!< Offset: 0x090  ��ѹͨ����ЧֵС�ź�У���Ĵ���     (R/W)              */
  __IO uint32_t ROSICTRL;                  /*!< Offset: 0x094  ������Ȧʹ�ܿ���λ                (R/W)              */
  __IO uint32_t ANACTRL;                   /*!< Offset: 0x098  ģ����ƼĴ���                    (R/W)              */
  __IO uint32_t UCONST;                    /*!< Offset: 0x09C  ʧѹ����²�������ĵ�ѹ��������Ե�         (R/W)     */
  __IO uint32_t LPIDELTIME;                /*!< Offset: 0x0A0  �͹��ļ���ģʽ�£���ʱģʽ����ʱ�����üĴ���  (R/W)    */
#if defined HT502x
  __IO uint32_t USAGLVL;                   /*!< Offset: 0x0A4  ��ѹ��������ֵ�Ĵ���                      (R/W)    */
  __IO uint32_t IPEAKCYC;                  /*!< Offset: 0x0A8  ����������ADC���ܲ������üĴ���            (R/W)    */
  __IO uint32_t UOVLVL;                    /*!< Offset: 0x0AC  ��ѹ��ֵ�����ֵ�Ĵ���                      (R/W)    */
	__IO uint32_t UdetCyc;                   /*!< Offset: 0x0B0  ��ѹPeak Sag�������                        (R/W)   */
#else
  __IO uint32_t USAGLVL;                   /*!< Offset: 0x0A4  ��ѹ��������ֵ�Ĵ���                      (R/W)    */
  __IO uint32_t USAGCYC;                   /*!< Offset: 0x0A8  ��ѹ������ADC���ܲ������üĴ���            (R/W)    */
  __IO uint32_t UOVLVL;                    /*!< Offset: 0x0AC  ��ѹ��ֵ�����ֵ�Ĵ���                      (R/W)    */
  __IO uint32_t OVCYC;                     /*!< Offset: 0x0B0  ��ֵ���ADC���ܲ������üĴ���                (R/W)   */
#endif
  __IO uint32_t IOVLVL;                    /*!< Offset: 0x0B4  ������ֵ��ֵ�Ĵ���                           (R/W)   */
  __IO uint32_t ZXILVL;                    /*!< Offset: 0x0B8  ����������ֵ�Ĵ���                           (R/W)   */
  __IO uint32_t PDATCPH;                   /*!< Offset: 0x0BC  �����й����ʼĴ�����16bit                    (R/W)   */
  __IO uint32_t PDATCPL;                   /*!< Offset: 0x0C0  �����й����ʼĴ�����16bit                    (R/W)   */
  __IO uint32_t QDATCPH;                   /*!< Offset: 0x0C4  �����޹����ʼĴ�����16bit                    (R/W)   */
  __IO uint32_t QDATCPL;                   /*!< Offset: 0x0C8  �����޹����ʼĴ�����16bit                    (R/W)   */
  __IO uint32_t SDATCPH;                   /*!< Offset: 0x0CC  �������ڹ��ʼĴ�����16bit                    (R/W)   */
  __IO uint32_t SDATCPL;                   /*!< Offset: 0x0D0  �������ʼĴ�����16bit                        (R/W)   */
  __IO uint32_t FILTERCTRL;                /*!< Offset: 0x0D4  ��ͨ��ͨ���˲���ϵ��ѡ�����                 (R/W)   */ 
  __IO uint32_t TUGAIN;                    /*!< Offset: 0x0D8  ��ѹͨ���ֶ��¶Ȳ���ϵ��                     (R/W)   */   
  __IO uint32_t TI1GAIN;                   /*!< Offset: 0x0DC  ����I1ͨ���ֶ��¶Ȳ���ϵ��                   (R/W)   */
  __IO uint32_t TI2GAIN;                   /*!< Offset: 0x0E0  ����I2ͨ���ֶ��¶Ȳ���ϵ��                   (R/W)   */
  __IO uint32_t UTCCOFFA;                  /*!< Offset: 0x0E4  Vrefgain�Ķ���ϵ��                           (R/W)   */   
  __IO uint32_t UTCCOFFB;                  /*!< Offset: 0x0E8  Vrefgain��һ��ϵ��                           (R/W)   */
  __IO uint32_t UTCCOFFC;                  /*!< Offset: 0x0EC  Vrefgain�ĳ�����                             (R/W)   */
  __IO uint32_t I1TCCOFFA;                 /*!< Offset: 0x0F0  Vrefgain�Ķ���ϵ��                           (R/W)   */   
  __IO uint32_t I1TCCOFFB;                 /*!< Offset: 0x0F4  Vrefgain��һ��ϵ��                           (R/W)   */
  __IO uint32_t I1TCCOFFC;                 /*!< Offset: 0x0F8  Vrefgain�ĳ�����                             (R/W)   */
  __IO uint32_t I2TCCOFFA;                 /*!< Offset: 0x0FC  Vrefgain�Ķ���ϵ��                           (R/W)   */   
  __IO uint32_t I2TCCOFFB;                 /*!< Offset: 0x100  Vrefgain��һ��ϵ��                           (R/W)   */
  __IO uint32_t I2TCCOFFC;                 /*!< Offset: 0x104  Vrefgain�ĳ�����                             (R/W)   */
  __IO uint32_t LOADDATACP;                /*!< Offset: 0x108  ���û���üĴ���д0x00BCʱ                    (R/W)   */    
#if defined HT502x
  __IO uint32_t BufferStart;               /*!< Offset: 0x10C  ��������������������У��ͣ�                   (R/W)   */
	__IO uint32_t BufferCoff;                /*!< Offset: 0x110  ���������ֶ�����ϵ����������У��ͣ�           (R/W)   */
#else
  __I  uint32_t Reserved[2];               /*!<                ��������DATACP load�����ۼ�Դ                        */                                 
#endif
  __IO uint32_t SRSTREG; 			   	   /*!< Offset: 0x114  �����λ�Ĵ���,д0x55���ڸ�λУ��Ĵ���  (R/W)       */    								  	
#if defined  HT502x
  __IO uint32_t PFCNTN;                    /*!< Offset: 0x118  ��������й��������              (R/W)              */
  __IO uint32_t QFCNTN;                    /*!< Offset: 0x11C  ��������޹��������              (R/W)              */   
#endif
} HT_EMU_ECR_TypeDef;                      /* end of group HT_EMU_ECR_TypeDef                                       */
#endif


/*****************************************  KEY Control Block  *******************************************/
#if defined  HT501x  ||  defined HT502x
typedef struct
{
    __IO uint32_t KEYSTA;                    /*!< Offset: 0x000 KEYSCAN״̬�Ĵ��� (R/W)       */
    __IO uint32_t KEYIF;                     /*!< Offset: 0x004 �����жϱ�־�Ĵ���(R/W)       */   
} HT_KEY_TypeDef;                            /* end of group HT_KEY_TypeDef                   */
#endif


/*****************************************  ECC Control Block  *******************************************/
#if defined  HT502x
typedef struct
{
    __IO uint32_t ECCCON;                    /*!< Offset: 0x000 ECCģ����ƼĴ���      (R/W)  */
    __IO uint32_t ECCSTA;                    /*!< Offset: 0x004 ECCģ��״̬�Ĵ���      (R/W)  */
	  __IO uint32_t PXREG;                     /*!< Offset: 0x008 ����X����Ĵ���        (R/W)  */
	  __IO uint32_t PYREG;                     /*!< Offset: 0x00C ����Y����Ĵ���        (R/W)  */
	  __IO uint32_t KEYREG;                    /*!< Offset: 0x010 ˽Կ�Ĵ���             (R/W)  */
	  __IO uint32_t AREG;                      /*!< Offset: 0x014 ��Բ���߲���a          (R/W)  */
	  __IO uint32_t PREG;                      /*!< Offset: 0x018 ������p                (R/W)  */
	  __IO uint32_t RXREG;                     /*!< Offset: 0x01C ECC���������X����     (R/W)  */
	  __IO uint32_t RYREG;                     /*!< Offset: 0x020 ECC���������Y����     (R/W)  */
	  __IO uint32_t SXREG;                     /*!< Offset: 0x024 ECC����ڶ�������X����  (R/W)  */
	  __IO uint32_t SYREG;                     /*!< Offset: 0x028 ECC����ڶ�������Y����  (R/W)  */
	  __IO uint32_t MREG;                      /*!< Offset: 0x02C Hash�㷨��ϢժҪ        (R/W)  */
} HT_ECC_TypeDef;                            /* end of group HT_ECC_TypeDef                   */
#endif


/*****************************************  Info Control Block  ******************************************/
/*
* @brief  RTCϵ����Info�ж���
*/
typedef union
{
    struct
    {
        __I  uint32_t iDFAH;
        __I  uint32_t iDFAL;
        __I  uint32_t iDFBH;
        __I  uint32_t iDFBL;
        __I  uint32_t iDFCH;
        __I  uint32_t iDFCL;
        __I  uint32_t iDFDH;
        __I  uint32_t iDFDL;
        __I  uint32_t iDFEH;
        __I  uint32_t iDFEL;
        __I  uint32_t iToff;
        __I  uint32_t iMCON01;
        __I  uint32_t iMCON23;
        __I  uint32_t iMCON45;
        __I  uint32_t iChkSum;               /* iChkSum = iDFAH + iDFAL +...+iMCON45   */
    }Muster;
    __I  uint32_t DataArry[15];
} HT_INFO_TypeDef;                           /* end of group HT_INFO_TypeDef           */


/*
**********************************************************************************************************
*                                        Peripheral memory map  
*                                           Unit of Byte
**********************************************************************************************************
*/  

/*  Base Address  */
#define HT_FLASH_BASE        (0x00000000UL)
#define HT_RAM_BASE          (0x20000000UL)
#define HT_APB0_BASE         (0x40000000UL)


#if defined HT6x1x
#define HT_FLASH_SIZE        (0x00020000UL)
#define HT_RAM_SIZE          (0x00002000UL)
#elif defined HT6x2x
#define HT_FLASH_SIZE        (0x00040000UL)
#define HT_RAM_SIZE          (0x00008000UL)
#elif defined HT501x
#define HT_FLASH_SIZE        (0x00020000UL)
#define HT_RAM_SIZE          (0x00002000UL)
#elif defined HT502x
#define HT_FLASH_SIZE        (0x00040000UL)
#define HT_RAM_SIZE          (0x00008000UL)
#endif
#define HT_APB0_SIZE         (0x00080000UL)


#define HT_INFO_BASE         (HT_FLASH_BASE + 0x00040000)
#if defined HT6x1x
#define HT_INFO_SIZE         (0x00000200UL)
#define HT_INFO_BANK         (0x00000100UL)
#elif defined HT6x2x
#define HT_INFO_SIZE         (0x00000400UL)
#define HT_INFO_BANK         (0x00000400UL)
#elif defined HT501x
#define HT_INFO_SIZE         (0x00000200UL)
#define HT_INFO_BANK         (0x00000100UL)
#elif defined HT502x
#define HT_INFO_SIZE         (0x00000400UL)
#define HT_INFO_BANK         (0x00000400UL)
#endif


/*  APB0 Peripherals Address  */
#define HT_TMR0_BASE         (HT_APB0_BASE + 0x01000)
#define HT_TMR1_BASE         (HT_APB0_BASE + 0x02000)
#define HT_TMR2_BASE         (HT_APB0_BASE + 0x03000)
#define HT_TMR3_BASE         (HT_APB0_BASE + 0x04000)
#if defined  HT6x2x
#define HT_TMR4_BASE         (HT_APB0_BASE + 0x19000)
#define HT_TMR5_BASE         (HT_APB0_BASE + 0x1A000)
#endif


#define HT_UART0_BASE        (HT_APB0_BASE + 0x05000)
#define HT_UART1_BASE        (HT_APB0_BASE + 0x06000)
#define HT_UART2_BASE        (HT_APB0_BASE + 0x07000)
#define HT_UART3_BASE        (HT_APB0_BASE + 0x08000)
#define HT_UART4_BASE        (HT_APB0_BASE + 0x09000)
#define HT_UART5_BASE        (HT_APB0_BASE + 0x00000)
#if defined  HT6x2x
#define HT_UART6_BASE        (HT_APB0_BASE + 0x18000)
#endif


#define HT_I2C_BASE          (HT_APB0_BASE + 0x0A000)
#define HT_SPI0_BASE         (HT_APB0_BASE + 0x0B000)
#if defined  HT6x2x
#define HT_SPI1_BASE         (HT_APB0_BASE + 0x17000)
#endif


#define HT_RTC_BASE          (HT_APB0_BASE + 0x0C000)
#define HT_LCD_BASE          (HT_APB0_BASE + 0x0D000)
#define HT_TBS_BASE          (HT_APB0_BASE + 0x0E000)
#define HT_PMU_BASE          (HT_APB0_BASE + 0x0F400)
#define HT_CMU_BASE          (HT_APB0_BASE + 0x0F000)
#define HT_WDT_BASE          (HT_APB0_BASE + 0x10000)
#define HT_GPIO_BASE         (HT_APB0_BASE + 0x11000)
#define HT_INT_BASE          (HT_APB0_BASE + 0x11800)


/*  GPIO Address  */    
#define HT_GPIOA_BASE        (HT_GPIO_BASE  + 0x00000)
#define HT_GPIOB_BASE        (HT_GPIO_BASE  + 0x00100)
#define HT_GPIOC_BASE        (HT_GPIO_BASE  + 0x00200)
#define HT_GPIOD_BASE        (HT_GPIO_BASE  + 0x00300)
#define HT_GPIOE_BASE        (HT_GPIO_BASE  + 0x00400)
#if defined  HT501x  ||  defined  HT502x
#define HT_GPIOF_BASE        (HT_GPIO_BASE  + 0x00600)
#elif defined  HT6x2x
#define HT_GPIOG_BASE        (HT_GPIO_BASE  + 0x00600)
#define HT_GPIOH_BASE        (HT_GPIO_BASE  + 0x00700)
#endif


#if defined  HT501x
#define HT_AES_BASE          (HT_APB0_BASE  + 0x12000)
#define HT_GHASH_BASE        (HT_APB0_BASE  + 0x12100)
#define HT_EMU_EPA_BASE      (HT_APB0_BASE  + 0x13000)
#define HT_EMU_ECA_BASE      (HT_APB0_BASE  + 0x13800)
#define HT_KEY_BASE          (HT_APB0_BASE  + 0x14000)

#define HT_DMA_BASE          (HT_APB0_BASE  + 0x16000)
#define HT_DMA_Channel0_BASE (HT_APB0_BASE  + 0x1600C)
#define HT_DMA_Channel1_BASE (HT_APB0_BASE  + 0x16024)
#define HT_DMA_Channel2_BASE (HT_APB0_BASE  + 0x1603C)

#elif defined  HT6x2x
#define HT_CRC_BASE          (HT_APB0_BASE  + 0x21000)
#define HT_AES_BASE          (HT_APB0_BASE  + 0x12000)
#define HT_RAND_BASE         (HT_APB0_BASE  + 0x12080)
#define HT_GHASH_BASE        (HT_APB0_BASE  + 0x12100)

#define HT_DMA_BASE          (HT_APB0_BASE  + 0x20000)
#define HT_DMA_Channel0_BASE (HT_APB0_BASE  + 0x2000C)
#define HT_DMA_Channel1_BASE (HT_APB0_BASE  + 0x20024)
#define HT_DMA_Channel2_BASE (HT_APB0_BASE  + 0x2003C)

#elif defined  HT502x
#define HT_AES_BASE          (HT_APB0_BASE  + 0x12000)
#define HT_RAND_BASE         (HT_APB0_BASE  + 0x12080)
#define HT_GHASH_BASE        (HT_APB0_BASE  + 0x12100)
#define HT_EMU_EPA_BASE      (HT_APB0_BASE  + 0x13000)
#define HT_EMU_ECA_BASE      (HT_APB0_BASE  + 0x13800)
#define HT_KEY_BASE          (HT_APB0_BASE  + 0x14000)

#define HT_DMA_BASE          (HT_APB0_BASE  + 0x16000)
#define HT_DMA_Channel0_BASE (HT_APB0_BASE  + 0x1600C)
#define HT_DMA_Channel1_BASE (HT_APB0_BASE  + 0x16024)
#define HT_DMA_Channel2_BASE (HT_APB0_BASE  + 0x1603C)

#define HT_ECC_BASE          (HT_APB0_BASE  + 0x17000)
#endif

/*
**********************************************************************************************************
*                                        Peripheral declaration  
**********************************************************************************************************
*/  
#define HT_TMR0              ((HT_TMR_TypeDef    *) HT_TMR0_BASE )
#define HT_TMR1              ((HT_TMR_TypeDef    *) HT_TMR1_BASE )
#define HT_TMR2              ((HT_TMR_TypeDef    *) HT_TMR2_BASE )
#define HT_TMR3              ((HT_TMR_TypeDef    *) HT_TMR3_BASE )
#if defined  HT6x2x
#define HT_TMR4              ((HT_TMR_TypeDef    *) HT_TMR4_BASE )
#define HT_TMR5              ((HT_TMR_TypeDef    *) HT_TMR5_BASE )
#endif

#define HT_UART0             ((HT_UART_TypeDef   *) HT_UART0_BASE)
#define HT_UART1             ((HT_UART_TypeDef   *) HT_UART1_BASE)
#define HT_UART2             ((HT_UART_TypeDef   *) HT_UART2_BASE)
#define HT_UART3             ((HT_UART_TypeDef   *) HT_UART3_BASE)
#define HT_UART4             ((HT_UART_TypeDef   *) HT_UART4_BASE)
#define HT_UART5             ((HT_UART_TypeDef   *) HT_UART5_BASE)
#if defined  HT6x2x
#define HT_UART6             ((HT_UART_TypeDef   *) HT_UART6_BASE)
#endif

#define HT_ISO7816_0         ((HT_ISO7816_TypeDef*) HT_UART4_BASE)
#define HT_ISO7816_1         ((HT_ISO7816_TypeDef*) HT_UART3_BASE)


#define HT_I2C               ((HT_I2C_TypeDef    *) HT_I2C_BASE  )
#define HT_SPI0              ((HT_SPI_TypeDef    *) HT_SPI0_BASE )
#if defined  HT6x2x
#define HT_SPI1              ((HT_SPI_TypeDef    *) HT_SPI1_BASE )
#endif

#define HT_RTC               ((HT_RTC_TypeDef    *) HT_RTC_BASE  )
#define HT_LCD               ((HT_LCD_TypeDef    *) HT_LCD_BASE  )
#define HT_TBS               ((HT_TBS_TypeDef    *) HT_TBS_BASE  )
#define HT_PMU               ((HT_PMU_TypeDef    *) HT_PMU_BASE  )
#define HT_CMU               ((HT_CMU_TypeDef    *) HT_CMU_BASE  )
#define HT_WDT               ((HT_WDT_TypeDef    *) HT_WDT_BASE  )
#define HT_INT               ((HT_INT_TypeDef    *) HT_INT_BASE  )


#define HT_GPIOA             ((HT_GPIO_TypeDef   *) HT_GPIOA_BASE)
#define HT_GPIOB             ((HT_GPIO_TypeDef   *) HT_GPIOB_BASE)
#define HT_GPIOC             ((HT_GPIO_TypeDef   *) HT_GPIOC_BASE)
#define HT_GPIOD             ((HT_GPIO_TypeDef   *) HT_GPIOD_BASE)
#define HT_GPIOE             ((HT_GPIO_TypeDef   *) HT_GPIOE_BASE)
#define HT_GPIOHDPORT        (*((uint32_t        *) 0x40011500)  )            /*!< ������˿�����       */
#if defined  HT501x  ||  defined HT502x
#define HT_GPIOF             ((HT_GPIO_TypeDef   *) HT_GPIOF_BASE)
#elif defined  HT6x2x
#define HT_GPIOG             ((HT_GPIO_TypeDef   *) HT_GPIOG_BASE)
#define HT_GPIOH             ((HT_GPIO_TypeDef   *) HT_GPIOH_BASE)
#endif


#if defined  HT501x  ||  defined HT502x
#define HT_EMUEPA            ((HT_EMU_EPR_TypeDef*) HT_EMU_EPA_BASE)	
#define HT_EMUECA            ((HT_EMU_ECR_TypeDef*) HT_EMU_ECA_BASE)	
#define HT_KEY               ((HT_KEY_TypeDef    *) HT_KEY_BASE  )	
#elif defined  HT6x2x
#define HT_CRC               ((HT_CRC_TypeDef    *) HT_CRC_BASE  )
#elif defined  HT6x2x  ||  defined HT502x
#define HT_RAND              ((HT_RAND_TypeDef   *) HT_RAND_BASE )
#endif

#if defined HT501x  || defined HT6x2x  ||  defined HT502x
#define HT_AES               ((HT_AES_TypeDef    *) HT_AES_BASE  )
#define HT_GHASH             ((HT_GHASH_TypeDef  *) HT_GHASH_BASE)

#define HT_DMA               ((HT_DMA_TypeDef    *) HT_DMA_BASE  )
#define HT_DMA_Channel0      ((HT_DMA_Channel_TypeDef*) HT_DMA_Channel0_BASE  )
#define HT_DMA_Channel1      ((HT_DMA_Channel_TypeDef*) HT_DMA_Channel1_BASE  )
#define HT_DMA_Channel2      ((HT_DMA_Channel_TypeDef*) HT_DMA_Channel2_BASE  )
#endif

#if defined HT502x
#define HT_ECC               ((HT_ECC_TypeDef    *) HT_ECC_BASE  )
#endif

#define HT_INFO              ((HT_INFO_TypeDef   *) (HT_INFO_BASE + 0x104) )    /*!< RTCϵ���洢��Info��  */        
/*
**********************************************************************************************************
*                               Bits Definition For HT6x1x HT6x2x HT501x
**********************************************************************************************************
*/ 


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of Timer Module  
**********************************************************************************************************
*/  
/************************  Bit definition for TMRCON register of HT_TMR_TypeDef  ************************/
#if defined HT6x2x  
#define  TMR_TMRCON                        		 ((uint32_t)0x07ff)           /*!<  ��ʱ�����ƼĴ���	  */
#else
#define  TMR_TMRCON                        		 ((uint32_t)0x00ff)           /*!<  ��ʱ�����ƼĴ���	  */
#endif
#define  TMR_TMRCON_CNTEN                        ((uint32_t)0x0001)           /*!<  ��ʱ����������ʹ��  */

#define  TMR_TMRCON_MODE                         ((uint32_t)0x0006)           /*!<  ��ʱ������ģʽ      */
#if defined HT6x1x  ||  defined HT501x  ||  defined HT502x                                                                            
#define  TMR_TMRCON_MODE_NONE                    ((uint32_t)0x0000)           /*!<Timer mode none       */
#define  TMR_TMRCON_MODE_PWM                     ((uint32_t)0x0002)           /*!<Timer mode pwm        */
#define  TMR_TMRCON_MODE_CAPTURE                 ((uint32_t)0x0004)           /*!<Timer mode capture    */
#define  TMR_TMRCON_MODE_COMPARE                 ((uint32_t)0x0006)           /*!<Timer mode compare    */
#elif defined  HT6x2x                                                         
#define  TMR_TMRCON_MODE_RECORD                  ((uint32_t)0x0000)           /*!<Timer mode EventRecord*/
#define  TMR_TMRCON_MODE_PWM                     ((uint32_t)0x0002)           /*!<Timer mode pwm        */
#define  TMR_TMRCON_MODE_CAPTURE                 ((uint32_t)0x0004)           /*!<Timer mode capture    */
#define  TMR_TMRCON_MODE_TIMING                  ((uint32_t)0x0006)           /*!<Timer mode timing only*/
#endif

#define  TMR_TMRCON_CCMODE                       ((uint32_t)0x0008)           /*!<����/�¼���ƽ��ѡ��   */
#define  TMR_TMRCON_CCMODE_RISE                  ((uint32_t)0x0000)           /*!<����/�¼���ƽ������   */
#define  TMR_TMRCON_CCMODE_FALL                  ((uint32_t)0x0008)           /*!<����/�¼���ƽ�½���   */

#define  TMR_TMRCON_PWMC                         ((uint32_t)0x0030)           /*!<PWM������ʽ           */
#define  TMR_TMRCON_PWMC_UP                      ((uint32_t)0x0000)           /*!<PWM���ϼ���           */
#define  TMR_TMRCON_PWMC_DOWM                    ((uint32_t)0x0010)           /*!<PWM���¼���           */
#define  TMR_TMRCON_PWMC_CENTER                  ((uint32_t)0x0020)           /*!<PWM������뷽ʽ       */

#define  TMR_TMRCON_PWMHL                        ((uint32_t)0x0040)           /*!<PWM��ʼ��ƽѡ��       */
#define  TMR_TMRCON_PWMHL_HIGH                   ((uint32_t)0x0000)           /*!<PWM��ʼ��ƽ��         */
#define  TMR_TMRCON_PWMHL_LOW                    ((uint32_t)0x0040)           /*!<PWM��ʼ��ƽ��         */

#if defined  HT6x1x  ||  defined HT501x                                                          
#define  TMR_TMRCON_CMPMODE                      ((uint32_t)0x0080)           /*!<����Ƚ�ģʽѡ��      */
#define  TMR_TMRCON_CMPMODE_0                    ((uint32_t)0x0000)           /*!<����Ƚ�ģʽ0         */
#define  TMR_TMRCON_CMPMODE_1                    ((uint32_t)0x0080)           /*!<����Ƚ�ģʽ1         */
#endif

#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TMR_TMRCON_CLKSEL                       ((uint32_t)0x0700)           /*!<TMR4&5ʱ��Դѡ��      */
#define  TMR_TMRCON_CLKSEL_LRC                   ((uint32_t)0x0000)           /*!<ѡ��LRC               */
#define  TMR_TMRCON_CLKSEL_LF                    ((uint32_t)0x0100)           /*!<ѡ��LF                */
#define  TMR_TMRCON_CLKSEL_HRC                   ((uint32_t)0x0200)           /*!<ѡ��HRC               */
#define  TMR_TMRCON_CLKSEL_PLL                   ((uint32_t)0x0300)           /*!<ѡ��PLL               */
#define  TMR_TMRCON_CLKSEL_MEMS                  ((uint32_t)0x0400)           /*!<ѡ��MEMS              */
#endif                                                                        /*!<Only for timer4&5     */

/************************  Bit definition for TMRDIV register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRDIV                              ((uint32_t)0xffff)           /*!<ͨ��Ԥ��Ƶ�Ĵ���      */

/************************  Bit definition for TMRPRD register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRPRD                              ((uint32_t)0xffff)           /*!<ͨ�����ڼĴ���        */

/************************  Bit definition for TMRCAP register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRCAP                              ((uint32_t)0xffff)           /*!<ͨ���������ݼĴ���    */

/************************  Bit definition for TMRCNT register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRCNT                              ((uint32_t)0xffff)           /*!<ͨ���������Ĵ���      */

/************************  Bit definition for TMRCMP register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRCMP                              ((uint32_t)0xffff)           /*!<ͨ���Ƚ����Ĵ���      */

/************************  Bit definition for TMRIE register of HT_TMR_TypeDef   ************************/
#if  defined  HT6x2x
#define  TMR_TMRIE		                         ((uint32_t)0x000f)           /*!<TMR�ж�ʹ��          */
#else
#define  TMR_TMRIE		                         ((uint32_t)0x0007)           /*!<TMR�ж�ʹ��          */
#endif
#define  TMR_TMRIE_PRDIE                         ((uint32_t)0x0001)           /*!<�����ж�ʹ��          */
#define  TMR_TMRIE_CAPIE                         ((uint32_t)0x0002)           /*!<�����ж�ʹ��          */
#define  TMR_TMRIE_CMPIE                         ((uint32_t)0x0004)           /*!<�Ƚ��ж�ʹ��          */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TMR_TMRIE_ACIE                          ((uint32_t)0x0008)           /*!<�¼������ж�ʹ��      */
#endif

/************************  Bit definition for TMRIF register of HT_TMR_TypeDef   ************************/
#if  defined  HT6x2x
#define  TMR_TMRIF		                         ((uint32_t)0x000f)           /*!<TMR�ж�ʹ��          */
#else
#define  TMR_TMRIF		                         ((uint32_t)0x0007)           /*!<TMR�ж�ʹ��          */
#endif
#define  TMR_TMRIF_PRDIF                         ((uint32_t)0x0001)           /*!<�����жϱ�־          */
#define  TMR_TMRIF_CAPIF                         ((uint32_t)0x0002)           /*!<�����жϱ�־          */
#define  TMR_TMRIF_CMPIF                         ((uint32_t)0x0004)           /*!<�Ƚ��жϱ�־          */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TMR_TMRIF_ACIF                          ((uint32_t)0x0008)           /*!<�¼������жϱ�־      */
#endif

/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of UART Module  
**********************************************************************************************************
*/  
/************************  Bit definition for MODESEL register of HT_UART_TypeDef ***********************/
#define  UART_MODESEL                            ((uint32_t)0x0001)           /*!<���ڹ���ѡ�����λ    */
#define  UART_MODESEL_UART                       ((uint32_t)0x0000)           /*!<ѡ��Uart����          */
#define  UART_MODESEL_7816                       ((uint32_t)0x0001)           /*!<ѡ��7816����          */
                                                                              /*!<Uart3&4 only          */
/************************  Bit definition for UARTCON register of HT_UART_TypeDef ***********************/                                                                            
#if  defined  HT6x2x
#define  UART_UARTCON		                     ((uint32_t)0x87ff)           /*!<Uart���ƼĴ���         */
#elif defined  HT502x
#define  UART_UARTCON		                     ((uint32_t)0x07ff)           /*!<Uart���ƼĴ���         */
#else
#define  UART_UARTCON		                     ((uint32_t)0x03ff)           /*!<Uart���ƼĴ���         */
#endif
#define  UART_UARTCON_TXEN                       ((uint32_t)0x0001)           /*!<Uart����ʹ��          */
#define  UART_UARTCON_RXEN                       ((uint32_t)0x0002)           /*!<Uart����ʹ��          */
#define  UART_UARTCON_TXIE                       ((uint32_t)0x0004)           /*!<Uart�����ж�ʹ��      */
#define  UART_UARTCON_RXIE                       ((uint32_t)0x0008)           /*!<Uart�����ж�ʹ��      */
#define  UART_UARTCON_PARITYEN                   ((uint32_t)0x0010)           /*!<Uart��żУ��ʹ��      */

#define  UART_UARTCON_PARITYSEL                  ((uint32_t)0x0060)           /*!<Uart��żУ��ѡ��      */
#define  UART_UARTCON_PARITYSEL_1                ((uint32_t)0x0060)           /*!<Uart��żУ���Ϊ1     */
#define  UART_UARTCON_PARITYSEL_0                ((uint32_t)0x0000)           /*!<Uart��żУ���Ϊ0     */
#define  UART_UARTCON_PARITYSEL_ODD              ((uint32_t)0x0020)           /*!<Uart��żУ��Ϊ��      */
#define  UART_UARTCON_PARITYSEL_EVEN             ((uint32_t)0x0040)           /*!<Uart��żУ��Ϊż      */

#define  UART_UARTCON_LENSEL                     ((uint32_t)0x0080)           /*!<Uart����ѡ��λ        */
#define  UART_UARTCON_LENSEL_7Bit                ((uint32_t)0x0080)           /*!<UartͨѶ���ݳ���Ϊ7λ */
#define  UART_UARTCON_LENSEL_8Bit                ((uint32_t)0x0000)           /*!<UartͨѶ���ݳ���Ϊ8λ */

#define  UART_UARTCON_STOPSEL                    ((uint32_t)0x0100)           /*!<Uart����ѡ��λ        */
#define  UART_UARTCON_STOPSEL_2Bit               ((uint32_t)0x0100)           /*!<UartͨѶֹͣλΪ2λ   */
#define  UART_UARTCON_STOPSEL_1Bit               ((uint32_t)0x0000)           /*!<UartͨѶֹͣλΪ1λ   */

#define  UART_UARTCON_UNEG                       ((uint32_t)0x0200)           /*!<UartͨѶ�߼�ѡ��λ    */
#define  UART_UARTCON_UNEG_NEGETIVE              ((uint32_t)0x0200)           /*!<UartͨѶΪ���߼�      */
#define  UART_UARTCON_UNEG_POSITIVE              ((uint32_t)0x0000)           /*!<UartͨѶΪ���߼�      */

#if  defined  HT6x2x                                                          
#define  UART_UARTCON_UNEG_AUTO                  ((uint32_t)0x0400)           /*!<UartͨѶ�߼��жϿ���  */ 
#define  UART_UARTCON_PRDIE                      ((uint32_t)0x8000)           /*!<Uart�����ж�ʹ��      */
#endif

#if  defined  HT502x
#define  UART_UARTCON_NEGFIX                     ((uint32_t)0x0400)           /*!<UartͨѶ�߼�����λ    */
#define  UART_UARTCON_NEGFIX_BYHARDWARE          ((uint32_t)0x0400)           /*!<UartͨѶ�߼���Ӳ������ */
#define  UART_UARTCON_NEGFIX_BYSOFTWARE          ((uint32_t)0x0000)           /*!<UartͨѶ�߼���������� */
#endif

/**********************  Bit definition for ISO7816CON register of HT_UART_TypeDef **********************/
#define  UART_ISO7816CON	                     ((uint32_t)0x03ff)           /*!<ISO7816���ƼĴ���     */

#define  UART_ISO7816CON_IEALL                   ((uint32_t)0x0007)           /*!<ISO7816�ж�ʹ��λ     */
#define  UART_ISO7816CON_TXIE                    ((uint32_t)0x0001)           /*!<�����ж�ʹ��λ        */
#define  UART_ISO7816CON_RXIE                    ((uint32_t)0x0002)           /*!<�����ж�ʹ��λ        */
#define  UART_ISO7816CON_PRDIE                   ((uint32_t)0x0004)           /*!<�����ж�ʹ��λ        */

#define  UART_ISO7816CON_ACKLEN                  ((uint32_t)0x0018)           /*!<ACK��Ӧ���ȿ���λ     */
#define  UART_ISO7816CON_ACKLEN_1Bit             ((uint32_t)0x0000)           /*!<ACK��Ӧ����1Bit       */
#define  UART_ISO7816CON_ACKLEN_1P5Bit           ((uint32_t)0x0008)           /*!<ACK��Ӧ����1.5Bit     */
#define  UART_ISO7816CON_ACKLEN_2Bit             ((uint32_t)0x0010)           /*!<ACK��Ӧ����2Bit       */

#define  UART_ISO7816CON_REPTR                   ((uint32_t)0x0060)           /*!<�Զ������ط���������  */
#define  UART_ISO7816CON_REPTR_0                 ((uint32_t)0x0000)           /*!<�Զ������ط�0��       */
#define  UART_ISO7816CON_REPTR_1                 ((uint32_t)0x0020)           /*!<�Զ������ط�1��       */
#define  UART_ISO7816CON_REPTR_2                 ((uint32_t)0x0040)           /*!<�Զ������ط�2��       */
#define  UART_ISO7816CON_REPTR_3                 ((uint32_t)0x0060)           /*!<�Զ������ط�3��       */

#define  UART_ISO7816CON_AUTOTXEN                ((uint32_t)0x0080)           /*!<�Զ��ط�ʹ��λ        */
#define  UART_ISO7816CON_AUTORXEN                ((uint32_t)0x0100)           /*!<�Զ�����ʹ��λ        */

#define  UART_ISO7816CON_7816PARITY              ((uint32_t)0x0200)           /*!<��żУ��ѡ��λ        */
#define  UART_ISO7816CON_7816PARITY_ODD          ((uint32_t)0x0200)           /*!<��У��                */
#define  UART_ISO7816CON_7816PARITY_EVEN         ((uint32_t)0x0000)           /*!<żУ��                */
                                                                              /*!<Uart3&4 only          */
/*************************  Bit definition for SREL register of HT_UART_TypeDef *************************/
#define  UART_SREL                               ((uint32_t)0xffff)           /*!<���ڲ����ʷ�����      */

/*************************  Bit definition for SBUF register of HT_UART_TypeDef *************************/
#define  UART_SBUF                               ((uint32_t)0x00ff)           /*!<�������ݻ���Ĵ���    */

/************************  Bit definition for UARTSTA register of HT_UART_TypeDef ***********************/
#if  defined  HT6x2x
#define  UART_UARTSTA                       	 ((uint32_t)0x001f)           /*!<uart״̬�Ĵ���         */
#else
#define  UART_UARTSTA                       	 ((uint32_t)0x0007)           /*!<uart״̬�Ĵ���         */
#endif
#define  UART_UARTSTA_TXIF                       ((uint32_t)0x0001)           /*!<�����жϱ�־          */
#define  UART_UARTSTA_RXIF                       ((uint32_t)0x0002)           /*!<�����жϱ�־          */
#define  UART_UARTSTA_PARITY                     ((uint32_t)0x0004)           /*!<��żУ������־      */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  UART_UARTSTA_PRDIF                      ((uint32_t)0x0008)           /*!<���������жϱ�־      */

#define  UART_UARTSTA_POLASTA                    ((uint32_t)0x0010)           /*!<����״̬λ            */
#define  UART_UARTSTA_POLASTA_POSITIVE           ((uint32_t)0x0000)           /*!<����״̬Ϊ��          */
#define  UART_UARTSTA_POLASTA_NEGETIVE           ((uint32_t)0x0010)           /*!<����״̬Ϊ��          */
#endif   

/**********************  Bit definition for ISO7816STA register of HT_UART_TypeDef **********************/
#define  UART_ISO7816STA	                     ((uint32_t)0x001f)           /*!<7816״̬�Ĵ���         */
#define  UART_ISO7816STA_TXIF                    ((uint32_t)0x0001)           /*!<�����жϱ�־          */
#define  UART_ISO7816STA_RXIF                    ((uint32_t)0x0002)           /*!<�����жϱ�־          */
#define  UART_ISO7816STA_PRDIF                   ((uint32_t)0x0004)           /*!<���������жϱ�־      */
#define  UART_ISO7816STA_TXERRSTAT               ((uint32_t)0x0008)           /*!<�������ݳ����־      */
#define  UART_ISO7816STA_RXERRSTAT               ((uint32_t)0x0010)           /*!<�������ݳ����־      */
                                                                              /*!<Uart3&4 only          */
/*************************  Bit definition for IRCON register of HT_UART_TypeDef ************************/
#define  UART_IRCON		                         ((uint32_t)0x0007)           /*!<������ƿ��ƼĴ���	  */
#define  UART_IRCON_IRTX                         ((uint32_t)0x0001)           /*!<������ƹ���ʹ�ܿ���  */

#define  UART_IRCON_IRLVL                        ((uint32_t)0x0002)           /*!<��������������ѡ��  */
#define  UART_IRCON_IRLVL_NEGETIVE               ((uint32_t)0x0002)           /*!<������                */
#define  UART_IRCON_IRLVL_POSITIVE               ((uint32_t)0x0000)           /*!<������                */

#define  UART_IRCON_IR38KSOURCE                  ((uint32_t)0x0004)           /*!<��������ź���Դѡ��  */
#define  UART_IRCON_IR38KSOURCE_HRC              ((uint32_t)0x0004)           /*!<��Դ��HRC             */
#define  UART_IRCON_IR38KSOURCE_PLL              ((uint32_t)0x0000)           /*!<��Դ��PLL             */


/*************************  Bit definition for IRDUTY register of HT_UART_TypeDef ***********************/
#define  UART_IRDUTY	                         ((uint32_t)0x0003)           /*!<�����������Ĵ���    */
#define  UART_IRDUTY_IRDUTY                      ((uint32_t)0x0003)           /*!<���Ʋ���ռ�ձ�����    */
#define  UART_IRDUTY_IRDUTY_50                   ((uint32_t)0x0000)           /*!<������Ʋ���ռ�ձ�50% */
#define  UART_IRDUTY_IRDUTY_25                   ((uint32_t)0x0001)           /*!<���Ʋ���ռ�ձ�25%     */
#define  UART_IRDUTY_IRDUTY_12P5                 ((uint32_t)0x0002)           /*!<���Ʋ���ռ�ձ�12.5%   */
#define  UART_IRDUTY_IRDUTY_6P25                 ((uint32_t)0x0003)           /*!<���Ʋ���ռ�ձ�6.25%   */

/************************  Bit definition for LOGDETNUM register of HT_UART_TypeDef *********************/
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  UART_NEGDETLEN                          ((uint32_t)0xffff)           /*!<�߼��ж�ʱ������      */
#endif
/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of I2C Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for I2CDAT register of HT_I2C_TypeDef ************************/
#define  I2C_I2CDAT                              ((uint32_t)0x00ff)           /*!<I2C���ݼĴ���         */

/*************************  Bit definition for I2CADR register of HT_I2C_TypeDef ************************/
#define  I2C_I2CADR                              ((uint32_t)0x00ff)           /*!<I2C��ַ�Ĵ���         */

/*************************  Bit definition for I2CCON register of HT_I2C_TypeDef ************************/
#define  I2C_I2CCON   	                         ((uint32_t)0x7fff)           /*!<I2C���ƼĴ���         */
#define  I2C_I2CCON_AA                           ((uint32_t)0x0004)           /*!<I2CӦ������λ         */
#define  I2C_I2CCON_SI                           ((uint32_t)0x0008)           /*!<I2C�жϱ�־λ         */
#define  I2C_I2CCON_STO                          ((uint32_t)0x0010)           /*!<I2Cֹͣ����λ         */
#define  I2C_I2CCON_STA                          ((uint32_t)0x0020)           /*!<I2C��ʼ����λ         */
#define  I2C_I2CCON_ENS1                         ((uint32_t)0x0040)           /*!<I2Cģ��ʹ��λ         */
#define  I2C_I2CCON_CR                           ((uint32_t)0x7f83)           /*!<I2Cʱ��Ƶ�ʿ���λ     */

/*************************  Bit definition for I2CSTA register of HT_I2C_TypeDef ************************/
#define  I2C_I2CSTA                              ((uint32_t)0x00f8)           /*!<I2C״̬�Ĵ���         */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of SPI Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for SPICON register of HT_SPI_TypeDef ************************/
#define  SPI_SPICON	  	                         ((uint32_t)0x00ff)           /*!<SPI���ƼĴ���         */
#define  SPI_SPICON_SPIEN                        ((uint32_t)0x0001)           /*!<SPIģ��ʹ��λ         */
#define  SPI_SPICON_MSTR                         ((uint32_t)0x0002)           /*!<SPI����ģʽѡ��λ     */
#define  SPI_SPICON_CPOL                         ((uint32_t)0x0004)           /*!<SPIʱ�Ӽ���ѡ��       */
#define  SPI_SPICON_CPHA                         ((uint32_t)0x0008)           /*!<SPIʱ����λ           */
#define  SPI_SPICON_SPR                          ((uint32_t)0x0070)           /*!<SPIʱ�����ʿ���       */
#define  SPI_SPICON_SSDIS                        ((uint32_t)0x0080)           /*!<SS����λ              */

/*************************  Bit definition for SPISTA register of HT_SPI_TypeDef ************************/
#define  SPI_SPISTA		                         ((uint32_t)0x000f)           /*!<SPI״̬�Ĵ���    */
#define  SPI_SPISTA_MODF                         ((uint32_t)0x0001)           /*!<SPIģʽ���ϱ�־λ     */
#define  SPI_SPISTA_SSERR                        ((uint32_t)0x0002)           /*!<SPIͬ���ӻ������־λ */
#define  SPI_SPISTA_WCOL                         ((uint32_t)0x0004)           /*!<SPIд��ͻ��־λ       */
#define  SPI_SPISTA_SPIF                         ((uint32_t)0x0008)           /*!<SPI���ݴ�����ɱ�־λ */

/*************************  Bit definition for SPIDAT register of HT_SPI_TypeDef ************************/
#define  SPI_SPIDAT                              ((uint32_t)0x00ff)           /*!<SPI���ݼĴ���         */

/*************************  Bit definition for SPISSN register of HT_SPI_TypeDef ************************/
#define  SPI_SPISSN_CS                           ((uint32_t)0x0001)           /*!<SPI�ӻ�ѡ�����       */

/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of RTC Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for RTCCON register of HT_RTC_TypeDef ************************/
#if defined HT502x
#define  RTC_RTCCON		                         ((uint32_t)0x006f)           /*!<RTC���ƼĴ���  */
#else
#define  RTC_RTCCON		                         ((uint32_t)0x007f)           /*!<RTC���ƼĴ���  */
#endif
#define  RTC_RTCCON_AUTOC                        ((uint32_t)0x0001)           /*!<�Զ��ֶ�����ѡ�����  */

#if defined HT501x  ||  defined  HT502x
#define  RTC_RTCCON_TOUT                         ((uint32_t)0x000E)           /*!<Tout�������          */
#define  RTC_RTCCON_TOUT_LOW                     ((uint32_t)0x0000)           /*!<Tout�����            */
#define  RTC_RTCCON_TOUT_HIGH                    ((uint32_t)0x0002)           /*!<Tout�����            */
#define  RTC_RTCCON_TOUT_LF                      ((uint32_t)0x0004)           /*!<Tout�����Ƶʱ��      */
#define  RTC_RTCCON_TOUT_1HZ                     ((uint32_t)0x0006)           /*!<Tout�����Ƶ����1hz   */
#define  RTC_RTCCON_TOUT_2HZ                     ((uint32_t)0x0008)           /*!<Tout�����Ƶ����2hz   */
#define  RTC_RTCCON_TOUT_4HZ                     ((uint32_t)0x000A)           /*!<Tout�����Ƶ����4hz   */
#define  RTC_RTCCON_TOUT_8HZ                     ((uint32_t)0x000C)           /*!<Tout�����Ƶ����8hz   */
#define  RTC_RTCCON_TOUT_RTC1HZ                  ((uint32_t)0x000E)           /*!<Tout����ڲ�RTC1hz    */
#else
#define  RTC_RTCCON_TOUT                         ((uint32_t)0x001E)           /*!<Tout�������          */
#define  RTC_RTCCON_TOUT_LOW                     ((uint32_t)0x0000)           /*!<Tout�����            */
#define  RTC_RTCCON_TOUT_HIGH                    ((uint32_t)0x0002)           /*!<Tout�����            */
#define  RTC_RTCCON_TOUT_LF                      ((uint32_t)0x0004)           /*!<Tout�����Ƶʱ��      */
#define  RTC_RTCCON_TOUT_1HZ                     ((uint32_t)0x0006)           /*!<Tout�����Ƶ����1hz   */
#define  RTC_RTCCON_TOUT_2HZ                     ((uint32_t)0x0008)           /*!<Tout�����Ƶ����2hz   */
#define  RTC_RTCCON_TOUT_4HZ                     ((uint32_t)0x000A)           /*!<Tout�����Ƶ����4hz   */
#define  RTC_RTCCON_TOUT_8HZ                     ((uint32_t)0x000C)           /*!<Tout�����Ƶ����8hz   */
#define  RTC_RTCCON_TOUT_16HZ                    ((uint32_t)0x000E)           /*!<Tout�����Ƶ����16hz  */
#define  RTC_RTCCON_TOUT_32HZ                    ((uint32_t)0x0010)           /*!<Tout�����Ƶ����32hz  */
#define  RTC_RTCCON_TOUT_64HZ                    ((uint32_t)0x0012)           /*!<Tout�����Ƶ����64hz  */
#define  RTC_RTCCON_TOUT_128HZ                   ((uint32_t)0x0014)           /*!<Tout�����Ƶ����128hz */
#endif

#define  RTC_RTCCON_RTC1EN                       ((uint32_t)0x0020)           /*!<RTC��ʱ��1ʹ��λ      */
#define  RTC_RTCCON_RTC2EN                       ((uint32_t)0x0040)           /*!<RTC��ʱ��2ʹ��λ      */


/*************************  Bit definition for RTCIE register of HT_RTC_TypeDef *************************/
#define  RTC_RTCIE                               ((uint32_t)0x00ff)           /*!<RTC�ж�ʹ��λ         */
#define  RTC_RTCIE_SECIE                         ((uint32_t)0x0001)           /*!<RTC���ж�ʹ��λ       */
#define  RTC_RTCIE_MINIE                         ((uint32_t)0x0002)           /*!<RTC�����ж�ʹ��λ     */
#define  RTC_RTCIE_HRIE                          ((uint32_t)0x0004)           /*!<RTCСʱ�ж�ʹ��λ     */
#define  RTC_RTCIE_DAYIE                         ((uint32_t)0x0008)           /*!<RTC���ж�ʹ��λ       */
#define  RTC_RTCIE_MTHIE                         ((uint32_t)0x0010)           /*!<RTC���ж�ʹ��λ       */
#define  RTC_RTCIE_RTC1IE                        ((uint32_t)0x0020)           /*!<RTC��ʱ��1�ж�ʹ��λ  */
#define  RTC_RTCIE_RTC2IE                        ((uint32_t)0x0040)           /*!<RTC��ʱ��2�ж�ʹ��λ  */
#define  RTC_RTCIE_ALMIE                         ((uint32_t)0x0080)           /*!<RTC�����ж�ʹ��λ     */

/*************************  Bit definition for RTCIF register of HT_RTC_TypeDef *************************/
#define  RTC_RTCIF                               ((uint32_t)0x00ff)           /*!<RTC�жϱ�־λ         */
#define  RTC_RTCIF_SECIF                         ((uint32_t)0x0001)           /*!<RTC���жϱ�־λ       */
#define  RTC_RTCIF_MINIF                         ((uint32_t)0x0002)           /*!<RTC�����жϱ�־λ     */
#define  RTC_RTCIF_HRIF                          ((uint32_t)0x0004)           /*!<RTCСʱ�жϱ�־λ     */
#define  RTC_RTCIF_DAYIF                         ((uint32_t)0x0008)           /*!<RTC���жϱ�־λ       */
#define  RTC_RTCIF_MTHIF                         ((uint32_t)0x0010)           /*!<RTC���жϱ�־λ       */
#define  RTC_RTCIF_RTC1IF                        ((uint32_t)0x0020)           /*!<RTC��ʱ��1�жϱ�־λ  */
#define  RTC_RTCIF_RTC2IF                        ((uint32_t)0x0040)           /*!<RTC��ʱ��2�жϱ�־λ  */
#define  RTC_RTCIF_ALMIF                         ((uint32_t)0x0080)           /*!<RTC�����жϱ�־λ     */

/*************************  Bit definition for ALMR register of HT_RTC_TypeDef **************************/
#define  RTC_ALMR                                ((uint32_t)0x1f3f)           /*!<RTC����Ĵ���         */

/************************  Bit definition for RTCTMR1 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCTMR1                             ((uint32_t)0xffff)           /*!<RTC��ʱ��1�Ĵ���      */

/************************  Bit definition for RTCTMR2 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCTMR2                             ((uint32_t)0xffff)           /*!<RTC��ʱ��2�Ĵ���      */

/**************************  Bit definition for SECR register of HT_RTC_TypeDef *************************/
#define  RTC_SECR                                ((uint32_t)0x003f)           /*!<RTC��Ĵ���           */

/**************************  Bit definition for MINR register of HT_RTC_TypeDef *************************/
#define  RTC_MINR                                ((uint32_t)0x003f)           /*!<RTC���ӼĴ���         */

/*************************  Bit definition for HOURR register of HT_RTC_TypeDef *************************/
#define  RTC_HOURR                               ((uint32_t)0x001f)           /*!<RTCСʱ�Ĵ���         */

/**************************  Bit definition for DAYR register of HT_RTC_TypeDef *************************/
#define  RTC_DAYR                                ((uint32_t)0x001f)           /*!<RTC�ռĴ���           */

/*************************  Bit definition for MONTHR register of HT_RTC_TypeDef ************************/
#define  RTC_MONTHR                              ((uint32_t)0x000f)           /*!<RTC�¼Ĵ���           */

/*************************  Bit definition for YEARR register of HT_RTC_TypeDef *************************/
#define  RTC_YEARR                               ((uint32_t)0x007f)           /*!<RTC��Ĵ���           */

/*************************  Bit definition for WEEKR register of HT_RTC_TypeDef *************************/
#define  RTC_WEEKR                               ((uint32_t)0x0007)           /*!<RTC�ܼĴ���           */

/************************  Bit definition for RTCCNTH register of HT_RTC_TypeDef ************************/
#define  RTC_RTCCNTH                             ((uint32_t)0xffff)           /*!<RTCУʱ������16λ     */

/************************  Bit definition for RTCCNTL register of HT_RTC_TypeDef ************************/
#define  RTC_RTCCNTL                             ((uint32_t)0xffff)           /*!<RTCУʱ������16λ     */

/*************************  Bit definition for RTCRD register of HT_RTC_TypeDef *************************/
#define  RTC_RTCRD_READFLAG                      ((uint32_t)0x0001)           /*!<RTC������λ           */

/*************************  Bit definition for RTCWR register of HT_RTC_TypeDef *************************/
#define  RTC_RTCWR_UPDATE                        ((uint32_t)0x0001)           /*!<RTCд����λ           */
#define  RTC_RTCWR_CLEAR                         ((uint32_t)0x0000)           /*!<RTCд����λ����       */

/***********************  Bit definition for RTCRSTFLG register of HT_RTC_TypeDef ***********************/
#define  RTC_RTCRSTFLG_SOFTReset                 ((uint32_t)0x0001)           /*!<RTC��λ��־         */
#define  RTC_RTCRSTFLG_PORReset                  ((uint32_t)0x0002)           /*!<RTCPOR��λ��־        */

/***********************  Bit definition for RTCRSTSET register of HT_RTC_TypeDef ***********************/
#define  RTC_RTCRSTSET                           ((uint32_t)0xffff)           /*!<RTCд��λ�Ĵ���       */


/***********************  Bit definition for RTCCHKSUM register of HT_RTC_TypeDef ***********************/
#if  defined  HT6x2x                                                          
#define  RTC_RTCCHKSUM                           ((uint32_t)0xffff)           /*!<RTCУ��ͼĴ���       */
#endif

#if  defined  HT6x2x || defined HT501x  ||  defined HT502x
/**************************  Bit definition for SECR2 register of HT_RTC_TypeDef ************************/
#define  RTC_SECR2                               ((uint32_t)0x003f)           /*!<RTC2��Ĵ���          */

/**************************  Bit definition for MINR2 register of HT_RTC_TypeDef ************************/
#define  RTC_MINR2                               ((uint32_t)0x003f)           /*!<RTC2���ӼĴ���        */

/*************************  Bit definition for HOURR2 register of HT_RTC_TypeDef ************************/
#define  RTC_HOURR2                              ((uint32_t)0x001f)           /*!<RTC2Сʱ�Ĵ���        */

/**************************  Bit definition for DAYR2 register of HT_RTC_TypeDef ************************/
#define  RTC_DAYR2                               ((uint32_t)0x001f)           /*!<RTC2�ռĴ���          */

/*************************  Bit definition for MONTHR2 register of HT_RTC_TypeDef ***********************/
#define  RTC_MONTHR2                             ((uint32_t)0x000f)           /*!<RTC2�¼Ĵ���          */

/*************************  Bit definition for YEARR2 register of HT_RTC_TypeDef ************************/
#define  RTC_YEARR2                              ((uint32_t)0x007f)           /*!<RTC2��Ĵ���          */

/*************************  Bit definition for WEEKR2 register of HT_RTC_TypeDef ************************/
#define  RTC_WEEKR2                              ((uint32_t)0x0007)           /*!<RTC2�ܼĴ���          */

/*************************  Bit definition for RTC2CAL register of HT_RTC_TypeDef ***********************/
#define  RTC_RTC2CAL                             ((uint32_t)0x1ffff)          /*!<LRC RTCУ���Ĵ���     */
#endif

/*************************  Bit definition for FRE_LRC register of HT_RTC_TypeDef ***********************/
#if  defined HT501x  ||  defined HT502x
#define  RTC_FRELRC                              ((uint32_t)0x1ffff)          /*!<LRC��Ƶֵ             */
#endif

#if  defined  HT6x2x  ||  defined  HT502x
/*************************  Bit definition for RTCRD2 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCRD2_READFLAG                     ((uint32_t)0x0001)           /*!<RTC2������λ          */

/*************************  Bit definition for RTCWR2 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCWR2_UPDATE                       ((uint32_t)0x0001)           /*!<RTC2д����λ          */
#define  RTC_RTCWR2_CLEAR                        ((uint32_t)0x0000)           /*!<RTC2д����λ����      */
#endif

/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of LCD Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for LCDCLK register of HT_LCD_TypeDef ************************/
#define  LCD_LCDCLK                       	   ((uint32_t)0x009f)           /*!<LCDʱ�ӼĴ���		    */

#define  LCD_LCDCLK_LCLK                       ((uint32_t)0x0007)           /*!<LCDɨ��Ƶ��ѡ��λ     */
#define  LCD_LCDCLK_LCLK_DIV64                   ((uint32_t)0x0004)           /*!<Divide Radio = 64     */
#define  LCD_LCDCLK_LCLK_DIV128                  ((uint32_t)0x0000)           /*!<Divide Radio = 128    */
#define  LCD_LCDCLK_LCLK_DIV256                  ((uint32_t)0x0001)           /*!<Divide Radio = 256    */
#define  LCD_LCDCLK_LCLK_DIV512                  ((uint32_t)0x0002)           /*!<Divide Radio = 512    */
#define  LCD_LCDCLK_LCLK_DIV1024                 ((uint32_t)0x0003)           /*!<Divide Radio = 1024   */
                                                                                                        
#define  LCD_LCDCLK_DUTY                       ((uint32_t)0x0018)           /*!<LCDDuty����ѡ��λ     */
#define  LCD_LCDCLK_DUTY_COM4                    ((uint32_t)0x0000)           /*!<4Com                  */
#define  LCD_LCDCLK_DUTY_COM6                    ((uint32_t)0x0008)           /*!<6Com                  */
#define  LCD_LCDCLK_DUTY_COM8                    ((uint32_t)0x0010)           /*!<8Com                  */
                                                                                                        
#define  LCD_LCDCLK_BIASCTL                    ((uint32_t)0x0080)           /*!<ƫѹ����ѡ��          */
#define  LCD_LCDCLK_BIASCTL_3                    ((uint32_t)0x0080)           /*!<1/3 biasƫѹ����      */
#define  LCD_LCDCLK_BIASCTL_4                    ((uint32_t)0x0000)           /*!<1/4 biasƫѹ����      */

/*************************  Bit definition for LCDCON register of HT_LCD_TypeDef ************************/

#if defined HT502x
#define  LCD_LCDCON		                         ((uint32_t)0x01ff)           /*!<LCD���ƼĴ���         */
#else
#define  LCD_LCDCON		                         ((uint32_t)0x00ff)           /*!<LCD���ƼĴ���         */
#endif


#define  LCD_LCDCON_VRSEL                      ((uint32_t)0x000f)           /*!<LCD�Աȶȵ���         */
#define  LCD_LCDCON_LC                         ((uint32_t)0x0010)           /*!<LCD���ٳ��ģʽѡ��λ */
#define  LCD_LCDCON_FC                         ((uint32_t)0x0020)           /*!<LCD���ٳ��ģʽѡ��λ */

#if defined HT502x
#define  LCD_LCDCON_FCSET                      ((uint32_t)0x00c0)           /*!<LCD���ٳ��ʱ��ѡ��λ */
#define  LCD_LCDCON_FCSET_16                     ((uint32_t)0x0000)           /*!<���ʱ�� = 1/16flcd    */
#define  LCD_LCDCON_FCSET_32                     ((uint32_t)0x0040)           /*!<���ʱ�� = 1/32flcd   */
#define  LCD_LCDCON_FCSET_64                     ((uint32_t)0x0080)           /*!<���ʱ�� = 1/64flcd   */
#define  LCD_LCDCON_FCSET_128                    ((uint32_t)0x00c0)           /*!<���ʱ�� = 1/128flcd  */
#else
#define  LCD_LCDCON_FCSET                      ((uint32_t)0x00c0)           /*!<LCD���ٳ��ʱ��ѡ��λ */
#define  LCD_LCDCON_FCSET_8                      ((uint32_t)0x0000)           /*!<���ʱ�� = 1/8flcd    */
#define  LCD_LCDCON_FCSET_16                     ((uint32_t)0x0040)           /*!<���ʱ�� = 1/16flcd   */
#define  LCD_LCDCON_FCSET_32                     ((uint32_t)0x0080)           /*!<���ʱ�� = 1/32flcd   */
#define  LCD_LCDCON_FCSET_64                     ((uint32_t)0x00c0)           /*!<���ʱ�� = 1/64flcd   */
#endif

#if defined HT502x
#define  LCD_LCDCON_FCSET2                     ((uint32_t)0x0100)           /*!<�����ѹ�ṹ����λ    */
#define  LCD_LCDCON_FCSET2_CHANGER               ((uint32_t)0x0100)           /*!<���ƺ�����ѹ�ṹ    */
#define  LCD_LCDCON_FCSET2_KEEPR                 ((uint32_t)0x0000)           /*!<����ԭ�е����ѹ�ṹ  */
#endif

/*************************  Bit definition for LCDOUT register of HT_LCD_TypeDef ************************/

#if defined HT502x

#define  LCD_LCDOUT                              ((uint32_t)0x0001)           /*!<LCD������ƼĴ���     */
#define  LCD_LCDOUT_ENABLE                       ((uint32_t)0x0001)           /*!<LCD�����           */
#define  LCD_LCDOUT_DISABLE                      ((uint32_t)0x0000)           /*!<LCD����ر�           */

#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of TBS Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for TBSCON register of HT_TBS_TypeDef ************************/
#if  defined  HT501x
#define  TBS_TBSCON		                         ((uint32_t)0xff1b)           /*!<TBS���ƼĴ���	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<�¶Ȳ�������ʹ��      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<��ص�ѹ��������ʹ��  */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0004)           /*!<��ص�ѹ��������ʹ��  */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0008)           /*!<ADC0��������ʹ��      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0010)           /*!<ADC1��������ʹ��      */
#define  TBS_TBSCON_ADC2En                       ((uint32_t)0x0020)           /*!<ADC2��������ʹ��      */
#define  TBS_TBSCON_VBATCMPEn                    ((uint32_t)0x0040)           /*!<Vbat�ȽϹ���ʹ��      */
#define  TBS_TBSCON_ADC0CMPEn                    ((uint32_t)0x0800)           /*!<ADC0�ȽϹ���ʹ��      */
#define  TBS_TBSCON_ADC1CMPEn                    ((uint32_t)0x1000)           /*!<ADC1�ȽϹ���ʹ��      */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x183f)           /*!<TBS��ģ��ʹ��λ       */
#elif  defined HT502x
#define  TBS_TBSCON		                         ((uint32_t)0x1b7f)           /*!<TBS���ƼĴ���	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<�¶Ȳ�������ʹ��      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<��ص�ѹ��������ʹ��  */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0004)           /*!<��ص�ѹ��������ʹ��  */
																						                                  /*!<  ��Ӧ�ֲ���VDDEn     */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0008)           /*!<ADC0��������ʹ��      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0010)           /*!<ADC1��������ʹ��      */
#define  TBS_TBSCON_ADC2En                       ((uint32_t)0x0020)           /*!<ADC2��������ʹ��      */
#define  TBS_TBSCON_VBATCMPEn                    ((uint32_t)0x0040)           /*!<Vbat�ȽϹ���ʹ��      */
#define  TBS_TBSCON_FILTER                       ((uint32_t)0x0300)           /*!<����¶Ȳ�����Filter�������*/
#define  TBS_TBSCON_ADC0CMPEn                    ((uint32_t)0x0800)           /*!<ADC0�ȽϹ���ʹ��      */
#define  TBS_TBSCON_ADC1CMPEn                    ((uint32_t)0x1000)           /*!<ADC1�ȽϹ���ʹ��      */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x183f)           /*!<TBS��ģ��ʹ��λ       */
#elif  defined HT6x2x
#define  TBS_TBSCON		                         ((uint32_t)0x3ffff)           /*!<TBS���ƼĴ���	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<�¶Ȳ�������ʹ��      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<��ص�ѹ��������ʹ��  */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0004)           /*!<ADC0��������ʹ��      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0008)           /*!<ADC1��������ʹ��      */
#define  TBS_TBSCON_FILTER                       ((uint32_t)0x0300)           /*!<����¶Ȳ�����Filter�������*/
#define  TBS_TBSCON_ADC3En                       ((uint32_t)0x8000)           /*!<ADC3��������ʹ��      */
#define  TBS_TBSCON_ADC4En                       ((uint32_t)0x10000)          /*!<ADC4��������ʹ��      */
#define  TBS_TBSCON_ADC5En                       ((uint32_t)0x20000)          /*!<ADC5��������ʹ��      */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0010)           /*!<VCC��������ʹ��       */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x001f)           /*!<TBS��ģ��ʹ��λ       */ 
#else
#define  TBS_TBSCON		                         ((uint32_t)0x3ffff)           /*!<TBS���ƼĴ���	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<�¶Ȳ�������ʹ��      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<��ص�ѹ��������ʹ��  */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0004)           /*!<ADC0��������ʹ��      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0008)           /*!<ADC1��������ʹ��      */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0010)           /*!<VCC��������ʹ��       */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x001f)           /*!<TBS��ģ��ʹ��λ       */  
#endif

/*************************  Bit definition for TBSIE register of HT_TBS_TypeDef *************************/
#if  defined  HT501x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS�ж�ʹ��           */
#define  TBS_TBSIE_TMPIE                         ((uint32_t)0x0001)           /*!<�¶Ȳ����ж�ʹ��      */
#define  TBS_TBSIE_VBATIE                        ((uint32_t)0x0002)           /*!<��ص�ѹ�����ж�ʹ��  */
#define  TBS_TBSIE_VCCIE                         ((uint32_t)0x0004)           /*!<VCC�����ж�ʹ��       */
#define  TBS_TBSIE_ADC0IE                        ((uint32_t)0x0008)           /*!<ADC0�����ж�ʹ��      */
#define  TBS_TBSIE_ADC1IE                        ((uint32_t)0x0010)           /*!<ADC1�����ж�ʹ��      */
#define  TBS_TBSIE_ADC2IE                        ((uint32_t)0x0020)           /*!<ADC2�����ж�ʹ��      */
#define  TBS_TBSIE_VBATCMPIE                     ((uint32_t)0x0040)           /*!<��ص�ѹ�Ƚ��ж�ʹ��  */
#define  TBS_TBSIE_VREFIE                        ((uint32_t)0x0080)           /*!<VREF�����ж�ʹ��      */
#define  TBS_TBSIE_ADC0CMPIE                     ((uint32_t)0x0100)           /*!<ADC0�Ƚ��ж�ʹ��      */
#define  TBS_TBSIE_ADC1CMPIE                     ((uint32_t)0x0200)           /*!<ADC1�Ƚ��ж�ʹ��      */
#elif defined  HT502x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS�ж�ʹ��           */
#define  TBS_TBSIE_TMPIE                         ((uint32_t)0x0001)           /*!<�¶Ȳ����ж�ʹ��      */
#define  TBS_TBSIE_VBATIE                        ((uint32_t)0x0002)           /*!<��ص�ѹ�����ж�ʹ��  */
#define  TBS_TBSIE_VCCIE                         ((uint32_t)0x0004)           /*!<VCC�����ж�ʹ��       */
																						                                  /*!<  ��Ӧ�ֲ���VDDIE     */
#define  TBS_TBSIE_ADC0IE                        ((uint32_t)0x0008)           /*!<ADC0�����ж�ʹ��      */
#define  TBS_TBSIE_ADC1IE                        ((uint32_t)0x0010)           /*!<ADC1�����ж�ʹ��      */
#define  TBS_TBSIE_ADC2IE                        ((uint32_t)0x0020)           /*!<ADC2�����ж�ʹ��      */
#define  TBS_TBSIE_VBATCMPIE                     ((uint32_t)0x0040)           /*!<��ص�ѹ�Ƚ��ж�ʹ��  */
#define  TBS_TBSIE_ADC0CMPIE                     ((uint32_t)0x0100)           /*!<ADC0�Ƚ��ж�ʹ��      */
#define  TBS_TBSIE_ADC1CMPIE                     ((uint32_t)0x0200)           /*!<ADC1�Ƚ��ж�ʹ��      */
#else
#if  defined HT6x1x
#define  TBS_TBSIE                               ((uint32_t)0x003f)           /*!<TBS�ж�ʹ��           */
#elif defined HT6x2x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS�ж�ʹ��           */
#endif
#define  TBS_TBSIE_TMPIE                         ((uint32_t)0x0001)           /*!<�¶Ȳ����ж�ʹ��      */
#define  TBS_TBSIE_VBATIE                        ((uint32_t)0x0002)           /*!<��ص�ѹ�����ж�ʹ��  */
#define  TBS_TBSIE_ADC0IE                        ((uint32_t)0x0004)           /*!<ADC0�����ж�ʹ��      */
#define  TBS_TBSIE_ADC1IE                        ((uint32_t)0x0008)           /*!<ADC1�����ж�ʹ��      */
#define  TBS_TBSIE_VBATCMPIE                     ((uint32_t)0x0010)           /*!<��ص�ѹ�Ƚ��ж�ʹ��  */
#define  TBS_TBSIE_VCCIE                         ((uint32_t)0x0020)           /*!<VCC�����ж�ʹ��       */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TBS_TBSIE_ADC0CMPIE                     ((uint32_t)0x0040)           /*!<ADC0�����Ƚ��ж�ʹ��  */
#define  TBS_TBSIE_ADC3IE                        ((uint32_t)0x0080)           /*!<ADC3�����ж�ʹ��      */
#define  TBS_TBSIE_ADC4IE                        ((uint32_t)0x0100)           /*!<ADC4�����ж�ʹ��      */
#define  TBS_TBSIE_ADC5IE                        ((uint32_t)0x0200)           /*!<ADC5�����ж�ʹ��      */
#endif
#endif

/*************************  Bit definition for TBSIF register of HT_TBS_TypeDef *************************/
#if  defined  HT501x
#define  TBS_TBSIF                               ((uint32_t)0x03ff)           /*!<TBS�жϱ�־           */
#define  TBS_TBSIF_TMPIF                         ((uint32_t)0x0001)           /*!<�¶Ȳ����жϱ�־      */
#define  TBS_TBSIF_VBATIF                        ((uint32_t)0x0002)           /*!<��ص�ѹ�����жϱ�־  */
#define  TBS_TBSIF_VCCIF                         ((uint32_t)0x0004)           /*!<VCC�����жϱ�־       */
#define  TBS_TBSIF_ADC0IF                        ((uint32_t)0x0008)           /*!<ADC0�����жϱ�־      */
#define  TBS_TBSIF_ADC1IF                        ((uint32_t)0x0010)           /*!<ADC1�����жϱ�־      */
#define  TBS_TBSIF_ADC2IF                        ((uint32_t)0x0020)           /*!<ADC2�����жϱ�־      */
#define  TBS_TBSIF_VBATCMPIF                     ((uint32_t)0x0040)           /*!<��ص�ѹ�Ƚ��жϱ�־  */
#define  TBS_TBSIF_VREFIF                        ((uint32_t)0x0080)           /*!<VREF�����жϱ�־      */
#define  TBS_TBSIF_ADC0CMPIF                     ((uint32_t)0x0100)           /*!<ADC0�Ƚ��жϱ�־      */
#define  TBS_TBSIF_ADC1CMPIF                     ((uint32_t)0x0200)           /*!<ADC1�Ƚ��жϱ�־      */
#elif defined  HT502x
#define  TBS_TBSIF_TMPIF                         ((uint32_t)0x0001)           /*!<�¶Ȳ����жϱ�־      */
#define  TBS_TBSIF_VBATIF                        ((uint32_t)0x0002)           /*!<��ص�ѹ�����жϱ�־  */
#define  TBS_TBSIF_VCCIF                         ((uint32_t)0x0004)           /*!<VCC�����жϱ�־       */
																						                                  /*!<  ��Ӧ�ֲ���VDDIF     */
#define  TBS_TBSIF_ADC0IF                        ((uint32_t)0x0008)           /*!<ADC0�����жϱ�־      */
#define  TBS_TBSIF_ADC1IF                        ((uint32_t)0x0010)           /*!<ADC1�����жϱ�־      */
#define  TBS_TBSIF_ADC2IF                        ((uint32_t)0x0020)           /*!<ADC2�����жϱ�־      */
#define  TBS_TBSIF_VBATCMPIF                     ((uint32_t)0x0040)           /*!<��ص�ѹ�Ƚ��жϱ�־  */
#define  TBS_TBSIF_ADC0CMPIF                     ((uint32_t)0x0100)           /*!<ADC0�Ƚ��жϱ�־      */
#define  TBS_TBSIF_ADC1CMPIF                     ((uint32_t)0x0200)           /*!<ADC1�Ƚ��жϱ�־      */
#else    /*!<HT6x1x && HT6x2x      */
#if  defined HT6x1x
#define  TBS_TBSIE                               ((uint32_t)0x003f)           /*!<TBS�жϱ�־           */
#elif defined HT6x2x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS�жϱ�־           */
#endif
#define  TBS_TBSIF_TMPIF                         ((uint32_t)0x0001)           /*!<�¶Ȳ����жϱ�־      */
#define  TBS_TBSIF_VBATIF                        ((uint32_t)0x0002)           /*!<��ص�ѹ�����жϱ�־  */
#define  TBS_TBSIF_ADC0IF                        ((uint32_t)0x0004)           /*!<ADC0�����жϱ�־      */
#define  TBS_TBSIF_ADC1IF                        ((uint32_t)0x0008)           /*!<ADC1�����жϱ�־      */
#define  TBS_TBSIF_VBATCMPIF                     ((uint32_t)0x0010)           /*!<��ص�ѹ�Ƚ��жϱ�־  */
#define  TBS_TBSIF_VCCIF                         ((uint32_t)0x0020)           /*!<VCC�����жϱ�־       */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TBS_TBSIF_ADC0CMPIF                     ((uint32_t)0x0040)           /*!<ADC0�����Ƚ��жϱ�־  */
#define  TBS_TBSIF_ADC3IF                        ((uint32_t)0x0080)           /*!<ADC3�����жϱ�־      */
#define  TBS_TBSIF_ADC4IF                        ((uint32_t)0x0100)           /*!<ADC4�����жϱ�־      */
#define  TBS_TBSIF_ADC5IF                        ((uint32_t)0x0200)           /*!<ADC5�����жϱ�־      */
#endif
#endif

/************************  Bit definition for TMPDAT register of HT_TBS_TypeDef *************************/
#define  TBS_TMPDAT                              ((uint32_t)0xffff)           /*!<�¶Ȳ�������Ĵ���    */

/************************  Bit definition for VBATDAT register of HT_TBS_TypeDef ************************/
#define  TBS_VBATDAT                             ((uint32_t)0xffff)           /*!<��ص�ѹ�����Ĵ���    */

/************************  Bit definition for ADC0DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC0DAT                             ((uint32_t)0xffff)           /*!<ADC0�����Ĵ���        */

/************************  Bit definition for ADC1DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC1DAT                             ((uint32_t)0xffff)           /*!<ADC1�����Ĵ���        */

/***********************  Bit definition for VBATCMP register of HT_TBS_TypeDef *************************/
#define  TBS_VBATCMP                             ((uint32_t)0xffff)           /*!<��ص�ѹ�ȽϼĴ���    */

#if  defined  HT501x ||  defined HT502x 
/***********************  Bit definition for ADC2DAT register of HT_TBS_TypeDef *************************/
#define  TBS_ADC2DAT                             ((uint32_t)0xffff)           /*!<ADC2�����Ĵ���        */
#endif

#if  defined  HT501x
/***********************  Bit definition for VREFDAT register of HT_TBS_TypeDef *************************/
#define  TBS_VREFDAT                             ((uint32_t)0xffff)           /*!<Vref�����Ĵ���        */
#endif

#if  defined  HT501x ||  defined HT502x 
/************************  Bit definition for ADC0CMP register of HT_TBS_TypeDef ************************/
#define  TBS_ADC0CMP                             ((uint32_t)0xffff)           /*!<ADC0�ȽϼĴ���        */

/************************  Bit definition for ADC1CMP register of HT_TBS_TypeDef ************************/                                                        
#define  TBS_ADC1CMP                             ((uint32_t)0xffff)           /*!<ADC1�ȽϼĴ���        */
#endif

#if defined HT6x2x
/************************  Bit definition for ADC3DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC3DAT                             ((uint32_t)0xffff)           /*!<ADC0�����Ĵ���        */

/************************  Bit definition for ADC4DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC4DAT                             ((uint32_t)0xffff)           /*!<ADC1�����Ĵ���        */

/************************  Bit definition for ADC5DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC5DAT                             ((uint32_t)0xffff)           /*!<ADC0�����Ĵ���        */
#endif

/************************  Bit definition for TBSPRD register of HT_TBS_TypeDef *************************/
#if  defined  HT501x  ||  defined  HT502x
#define  TBS_TBSPRD_TMPPRD                       ((uint32_t)0x0003)           /*!<TMP��������           */
#define  TBS_TBSPRD_TMPPRD_1S                    ((uint32_t)0x0000)           /*!<TMP���� = 1s          */
#define  TBS_TBSPRD_TMPPRD_8S                    ((uint32_t)0x0001)           /*!<TMP���� = 8s          */
#define  TBS_TBSPRD_TMPPRD_32S                   ((uint32_t)0x0002)           /*!<TMP���� = 32s         */
#define  TBS_TBSPRD_TMPPRD_125mS                 ((uint32_t)0x0003)           /*!<TMP���� = 1/8s        */

#define  TBS_TBSPRD_VBATPRD                      ((uint32_t)0x000C)           /*!<Vbat�����������      */
#define  TBS_TBSPRD_VBATPRD_1S                   ((uint32_t)0x0000)           /*!<Vbat������� = 1s     */
#define  TBS_TBSPRD_VBATPRD_4S                   ((uint32_t)0x0004)           /*!<Vbat������� = 4s     */
#define  TBS_TBSPRD_VBATPRD_8S                   ((uint32_t)0x0008)           /*!<Vbat������� = 8s     */
#define  TBS_TBSPRD_VBATPRD_32S                  ((uint32_t)0x000C)           /*!<Vbat������� = 32s    */

#define  TBS_TBSPRD_VCCPRD                       ((uint32_t)0x0030)           /*!<VCC�����������       */
#define  TBS_TBSPRD_VCCPRD_1S                    ((uint32_t)0x0000)           /*!<VCC������� = 1s      */
#define  TBS_TBSPRD_VCCPRD_4S                    ((uint32_t)0x0010)           /*!<VCC������� = 4s      */
#define  TBS_TBSPRD_VCCPRD_8S                    ((uint32_t)0x0020)           /*!<VCC������� = 8s      */
#define  TBS_TBSPRD_VCCPRD_32S                   ((uint32_t)0x0030)           /*!<VCC������� = 32s     */
																										                          /*!<  HT502x�˴�����Ӧ�ֲ���VDDPRD  */
                                                                                                        
#define  TBS_TBSPRD_ADC0PRD                      ((uint32_t)0x00C0)           /*!<ADC0�����������      */
#define  TBS_TBSPRD_ADC0PRD_1S                   ((uint32_t)0x0000)           /*!<ADC0������� = 1s     */
#define  TBS_TBSPRD_ADC0PRD_4S                   ((uint32_t)0x0040)           /*!<ADC0������� = 4s     */
#define  TBS_TBSPRD_ADC0PRD_8S                   ((uint32_t)0x0080)           /*!<ADC0������� = 8s     */
#define  TBS_TBSPRD_ADC0PRD_32S                  ((uint32_t)0x00C0)           /*!<ADC0������� = 32s    */
                                                                                                        
#define  TBS_TBSPRD_ADC1PRD                      ((uint32_t)0x0300)           /*!<ADC1�����������      */
#define  TBS_TBSPRD_ADC1PRD_1S                   ((uint32_t)0x0000)           /*!<ADC1������� = 1s     */
#define  TBS_TBSPRD_ADC1PRD_4S                   ((uint32_t)0x0100)           /*!<ADC1������� = 4s     */
#define  TBS_TBSPRD_ADC1PRD_8S                   ((uint32_t)0x0200)           /*!<ADC1������� = 8s     */
#define  TBS_TBSPRD_ADC1PRD_32S                  ((uint32_t)0x0300)           /*!<ADC1������� = 32s    */
                                                                                                      
#define  TBS_TBSPRD_ADC2PRD                      ((uint32_t)0x0C00)           /*!<ADC2�����������      */
#define  TBS_TBSPRD_ADC2PRD_1S                   ((uint32_t)0x0000)           /*!<ADC2������� = 1s     */
#define  TBS_TBSPRD_ADC2PRD_4S                   ((uint32_t)0x0400)           /*!<ADC2������� = 4s     */
#define  TBS_TBSPRD_ADC2PRD_8S                   ((uint32_t)0x0800)           /*!<ADC2������� = 8s     */
#define  TBS_TBSPRD_ADC2PRD_32S                  ((uint32_t)0x0C00)           /*!<ADC2������� = 32s    */

#if  defined  HT501x
#define  TBS_TBSPRD_VREFPRD                      ((uint32_t)0x3000)           /*!<Vref�����������      */
#define  TBS_TBSPRD_VREFPRD_1S                   ((uint32_t)0x0000)           /*!<Verf������� = 1s     */
#define  TBS_TBSPRD_VREFPRD_4S                   ((uint32_t)0x1000)           /*!<Vref������� = 4s     */
#define  TBS_TBSPRD_VREFPRD_8S                   ((uint32_t)0x2000)           /*!<Vref������� = 8s     */
#define  TBS_TBSPRD_VREFPRD_32S                  ((uint32_t)0x3000)           /*!<Vref������� = 32s    */
#endif

#else    /*!<HT6x1x && HT6x2x      */

#define  TBS_TBSPRD_TMPPRD                       ((uint32_t)0x0007)           /*!<TMP��������           */
#define  TBS_TBSPRD_TMPPRD_OSC0P5S_MEM8S         ((uint32_t)0x0000)           /*!<TMP���� = 0.5s / 8s   */
#define  TBS_TBSPRD_TMPPRD_OSC1S_MEM4S           ((uint32_t)0x0001)           /*!<TMP���� = 1s   / 4s   */
#define  TBS_TBSPRD_TMPPRD_OSC2S_MEM2S           ((uint32_t)0x0002)           /*!<TMP���� = 2s   / 2s   */
#define  TBS_TBSPRD_TMPPRD_OSC4S_MEM1S           ((uint32_t)0x0003)           /*!<TMP���� = 4s   / 1s   */
#define  TBS_TBSPRD_TMPPRD_OSC8S_MEM0P5S         ((uint32_t)0x0004)           /*!<TMP���� = 8s   / 0.5s */
#define  TBS_TBSPRD_TMPPRD_OSC16S_MEM125mS       ((uint32_t)0x0005)           /*!<TMP���� = 16s  / 125mS*/
#define  TBS_TBSPRD_TMPPRD_OSC32S_MEM31mS        ((uint32_t)0x0006)           /*!<TMP���� = 32s  / 31mS */
#define  TBS_TBSPRD_TMPPRD_OSC64S_MEM8mS         ((uint32_t)0x0007)           /*!<TMP���� = 64s  / 8mS  */

#define  TBS_TBSPRD_VBATPRD                      ((uint32_t)0x0018)           /*!<Vbat�����������      */
#define  TBS_TBSPRD_VBATPRD_1S                   ((uint32_t)0x0000)           /*!<Vbat������� = 1s     */
#define  TBS_TBSPRD_VBATPRD_2S                   ((uint32_t)0x0008)           /*!<Vbat������� = 2s     */
#define  TBS_TBSPRD_VBATPRD_8S                   ((uint32_t)0x0010)           /*!<Vbat������� = 8s     */
#define  TBS_TBSPRD_VBATPRD_16S                  ((uint32_t)0x0018)           /*!<Vbat������� = 16s    */
                                                                                                        
#define  TBS_TBSPRD_ADC0PRD                      ((uint32_t)0x0060)           /*!<ADC0�����������      */
#define  TBS_TBSPRD_ADC0PRD_1S                   ((uint32_t)0x0000)           /*!<ADC0������� = 1s     */
#define  TBS_TBSPRD_ADC0PRD_2S                   ((uint32_t)0x0020)           /*!<ADC0������� = 2s     */
#define  TBS_TBSPRD_ADC0PRD_8S                   ((uint32_t)0x0040)           /*!<ADC0������� = 8s     */
#define  TBS_TBSPRD_ADC0PRD_16S                  ((uint32_t)0x0060)           /*!<ADC0������� = 16s    */
                                                                                                        
#define  TBS_TBSPRD_ADC1PRD                      ((uint32_t)0x0180)           /*!<ADC1�����������      */
#define  TBS_TBSPRD_ADC1PRD_1S                   ((uint32_t)0x0000)           /*!<ADC1������� = 1s     */
#define  TBS_TBSPRD_ADC1PRD_2S                   ((uint32_t)0x0080)           /*!<ADC1������� = 2s     */
#define  TBS_TBSPRD_ADC1PRD_8S                   ((uint32_t)0x0100)           /*!<ADC1������� = 8s     */
#define  TBS_TBSPRD_ADC1PRD_16S                  ((uint32_t)0x0180)           /*!<ADC1������� = 16s    */

#define  TBS_TBSPRD_VCCPRD                       ((uint32_t)0x0600)           /*!<VCC�����������       */
#define  TBS_TBSPRD_VCCPRD_1S                    ((uint32_t)0x0000)           /*!<VCC������� = 1s      */
#define  TBS_TBSPRD_VCCPRD_2S                    ((uint32_t)0x0200)           /*!<VCC������� = 2s      */
#define  TBS_TBSPRD_VCCPRD_8S                    ((uint32_t)0x0400)           /*!<VCC������� = 8s      */
#define  TBS_TBSPRD_VCCPRD_16S                   ((uint32_t)0x0600)           /*!<VCC������� = 16s     */
#if defined HT6x2x 
#define  TBS_TBSPRD_ADC3PRD                      ((uint32_t)0x1800)           /*!<ADC3�����������      */
#define  TBS_TBSPRD_ADC3PRD_1S                   ((uint32_t)0x0000)           /*!<ADC3������� = 1s     */
#define  TBS_TBSPRD_ADC3PRD_2S                   ((uint32_t)0x0800)           /*!<ADC3������� = 2s     */
#define  TBS_TBSPRD_ADC3PRD_8S                   ((uint32_t)0x1000)           /*!<ADC3������� = 8s     */
#define  TBS_TBSPRD_ADC3PRD_16S                  ((uint32_t)0x1800)           /*!<ADC3������� = 16s    */

#define  TBS_TBSPRD_ADC4PRD                      ((uint32_t)0x6000)           /*!<ADC4�����������      */
#define  TBS_TBSPRD_ADC4PRD_1S                   ((uint32_t)0x0000)           /*!<ADC4������� = 1s     */
#define  TBS_TBSPRD_ADC4PRD_2S                   ((uint32_t)0x2000)           /*!<ADC4������� = 2s     */
#define  TBS_TBSPRD_ADC4PRD_8S                   ((uint32_t)0x4000)           /*!<ADC4������� = 8s     */
#define  TBS_TBSPRD_ADC4PRD_16S                  ((uint32_t)0x6000)           /*!<ADC4������� = 16s    */

#define  TBS_TBSPRD_ADC5PRD                      ((uint32_t)0x18000)           /*!<ADC5�����������      */
#define  TBS_TBSPRD_ADC5PRD_1S                   ((uint32_t)0x00000)           /*!<ADC5������� = 1s     */
#define  TBS_TBSPRD_ADC5PRD_2S                   ((uint32_t)0x08000)           /*!<ADC5������� = 2s     */
#define  TBS_TBSPRD_ADC5PRD_8S                   ((uint32_t)0x10000)           /*!<ADC5������� = 8s     */
#define  TBS_TBSPRD_ADC5PRD_16S                  ((uint32_t)0x18000)           /*!<ADC5������� = 16s    */
#endif
#endif

/*************************  Bit definition for VCCDAT register of HT_TBS_TypeDef ************************/
#define  TBS_VCCDAT                              ((uint32_t)0xffff)           /*!<VCC�����Ĵ���         */
                                                         /*!< HT6x2x               */
/************************  Bit definition for ADC0CMP register of HT_TBS_TypeDef ************************/
#if  defined  HT6x2x                                                          
#define  TBS_ADC0CMP                             ((uint32_t)0xffff)           /*!<ADC0�ȽϼĴ���        */
#define  TBS_ADC3CMP                             ((uint32_t)0xffff)           /*!<ADC3�ȽϼĴ���        */
#define  TBS_ADC4CMP                             ((uint32_t)0xffff)           /*!<ADC4�ȽϼĴ���        */
#define  TBS_ADC5CMP                             ((uint32_t)0xffff)           /*!<ADC5�ȽϼĴ���        */
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of PMU Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for PMUCON register of HT_PMU_TypeDef ************************/
#if defined HT6x2x
#define  PMU_PMUCON			                     ((uint32_t)0x803f)           /*!<PMU���ƼĴ���	      */
#elif defined  HT501x  ||  defined  HT502x
#define  PMU_PMUCON			                     ((uint32_t)0x801f)           /*!<PMU���ƼĴ���	      */
#elif defined HT6x1x
#define  PMU_PMUCON			                     ((uint32_t)0x0017)           /*!<PMU���ƼĴ���	      */
#endif
#define  PMU_PMUCON_BORDETEN                     ((uint32_t)0x0001)           /*!<BOR_DET ģ��ʹ��      */
#define  PMU_PMUCON_BORRST                       ((uint32_t)0x0002)           /*!<BOR�ж�/��λѡ��      */
#define  PMU_PMUCON_LVD0DETEN                    ((uint32_t)0x0004)           /*!<LVDIN0ģ��ʹ��        */
#if  defined  HT6x2x  ||  defined  HT501x   ||  defined  HT502x                                                  
#define  PMU_PMUCON_LVD1DETEN                    ((uint32_t)0x0008)           /*!<LVDIN1ģ��ʹ��        */
#endif
#define  PMU_PMUCON_HoldLDO                      ((uint32_t)0x0010)           /*!<Hold�´󹦺�LDO��   */
#if  defined  HT6x2x  
#define  PMU_PMUCON_POWDETEN                     ((uint32_t)0x0020)           /*!<POW_DETģ��ʹ��       */
#endif
#if  defined  HT6x2x  ||  defined HT501x  ||  defined  HT502x
#define  PMU_PMUCON_DisChargeEN                  ((uint32_t)0x8000)           /*!<��ضۻ�����ʹ��      */
#endif

/*************************  Bit definition for VDETCFG register of HT_PMU_TypeDef ***********************/
#if defined HT6x2x
#define  PMU_VDETCFG		                     ((uint32_t)0xc1ff)           /*!<��Դ��ֵⷧ���üĴ���  */
#elif  defined  HT501x
#define  PMU_VDETCFG		                     ((uint32_t)0x800f)           /*!<��Դ��ֵⷧ���üĴ���  */
#elif  defined HT6x1x
#define  PMU_VDETCFG		                     ((uint32_t)0x003f)           /*!<��Դ��ֵⷧ���üĴ���  */
#elif  defined HT502x
#define  PMU_VDETCFG		                     ((uint32_t)0x80bf)           /*!<��Դ��ֵⷧ���üĴ���  */
#endif

#if  defined  HT501x  ||  defined  HT502x  ||  defined  HT6x1x
#define  PMU_VDETCFG_BORLVL                      ((uint32_t)0x0003)           /*!<BOR_DET �����ֵ�趨  */
#define  PMU_VDETCFG_BORLVL_2V2                  ((uint32_t)0x0001)           /*!<�����ֵ = 2.2v       */
#define  PMU_VDETCFG_BORLVL_2V4                  ((uint32_t)0x0000)           /*!<�����ֵ = 2.4v       */
#define  PMU_VDETCFG_BORLVL_2V6                  ((uint32_t)0x0003)           /*!<�����ֵ = 2.6v       */
#define  PMU_VDETCFG_BORLVL_2V8                  ((uint32_t)0x0002)           /*!<�����ֵ = 2.8v       */
#elif  defined  HT6x2x
#define  PMU_VDETCFG_BORLVL                      ((uint32_t)0x0003)           /*!<BOR_DET �����ֵ�趨  */
#define  PMU_VDETCFG_BORLVL_2V0                  ((uint32_t)0x0001)           /*!<�����ֵ = 2.0v       */
#define  PMU_VDETCFG_BORLVL_2V2                  ((uint32_t)0x0000)           /*!<�����ֵ = 2.2v       */
#define  PMU_VDETCFG_BORLVL_2V6                  ((uint32_t)0x0003)           /*!<�����ֵ = 2.6v       */
#define  PMU_VDETCFG_BORLVL_2V8                  ((uint32_t)0x0002)           /*!<�����ֵ = 2.8v       */
#endif

#if  defined  HT501x
#define  PMU_VDETCFG_VCCLVL                      ((uint32_t)0x000c)           /*!<VCC_DET �����ֵ�趨  */
#define  PMU_VDETCFG_VCCLVL_2V4                  ((uint32_t)0x0000)           /*!<�����ֵ = 2.4V       */
#define  PMU_VDETCFG_VCCLVL_2V6                  ((uint32_t)0x0004)           /*!<�����ֵ = 2.6V       */
#define  PMU_VDETCFG_VCCLVL_2V8                  ((uint32_t)0x0008)           /*!<�����ֵ = 2.8V       */
#define  PMU_VDETCFG_VCCLVL_3V0                  ((uint32_t)0x000c)           /*!<�����ֵ = 3.0V       */
#else    /*!<HT6x1x && HT6x2x && HT502x      */
				 /*!<HT502x�˴�����Ӧ�ֲ��е�VSYS_LVL */
#define  PMU_VDETCFG_VCCLVL                      ((uint32_t)0x003c)           /*!<VCC_DET �����ֵ�趨  */
#define  PMU_VDETCFG_VCCLVL_2V4                  ((uint32_t)0x0000)           /*!<�����ֵ = 2.4V       */
#define  PMU_VDETCFG_VCCLVL_2V6                  ((uint32_t)0x0004)           /*!<�����ֵ = 2.6V       */
#define  PMU_VDETCFG_VCCLVL_2V8                  ((uint32_t)0x0008)           /*!<�����ֵ = 2.8V       */
#define  PMU_VDETCFG_VCCLVL_3V0                  ((uint32_t)0x000c)           /*!<�����ֵ = 3.0V       */
#define  PMU_VDETCFG_VCCLVL_3V2                  ((uint32_t)0x0010)           /*!<�����ֵ = 3.2V       */
#define  PMU_VDETCFG_VCCLVL_3V4                  ((uint32_t)0x0014)           /*!<�����ֵ = 3.4V       */
#define  PMU_VDETCFG_VCCLVL_3V6                  ((uint32_t)0x0018)           /*!<�����ֵ = 3.6V       */
#define  PMU_VDETCFG_VCCLVL_3V8                  ((uint32_t)0x001c)           /*!<�����ֵ = 3.8V       */
#define  PMU_VDETCFG_VCCLVL_4V0                  ((uint32_t)0x0020)           /*!<�����ֵ = 4.0V       */
#define  PMU_VDETCFG_VCCLVL_4V2                  ((uint32_t)0x0024)           /*!<�����ֵ = 4.2V       */
#define  PMU_VDETCFG_VCCLVL_4V4                  ((uint32_t)0x0028)           /*!<�����ֵ = 4.4V       */
#define  PMU_VDETCFG_VCCLVL_4V6                  ((uint32_t)0x002c)           /*!<�����ֵ = 4.6V       */
#define  PMU_VDETCFG_VCCLVL_4V8                  ((uint32_t)0x0030)           /*!<�����ֵ = 4.8V       */
#define  PMU_VDETCFG_VCCLVL_5V0                  ((uint32_t)0x0034)           /*!<�����ֵ = 5.0V       */
#if defined  HT6x2x                                                                                                      
#define  PMU_VDETCFG_POWLVL                      ((uint32_t)0x01c0)           /*!<POW_DET �����ֵ�趨  */
#define  PMU_VDETCFG_POWLVL_1P25mV               ((uint32_t)0x0000)           /*!<�����ֵ = 1.25mV     */
#define  PMU_VDETCFG_POWLVL_2P5mV                ((uint32_t)0x0040)           /*!<�����ֵ = 2.5mV      */
#define  PMU_VDETCFG_POWLVL_3P75mV               ((uint32_t)0x0080)           /*!<�����ֵ = 3.75mV     */
#define  PMU_VDETCFG_POWLVL_5mV                  ((uint32_t)0x00c0)           /*!<�����ֵ = 5mV        */
#define  PMU_VDETCFG_POWLVL_6P25mV               ((uint32_t)0x0100)           /*!<�����ֵ = 6.25mV     */
#define  PMU_VDETCFG_POWLVL_7P5mV                ((uint32_t)0x0140)           /*!<�����ֵ = 7.5mV      */
#define  PMU_VDETCFG_POWLVL_8P75mV               ((uint32_t)0x0180)           /*!<�����ֵ = 8.75mV     */
#define  PMU_VDETCFG_POWLVL_10mV                 ((uint32_t)0x01c0)           /*!<�����ֵ = 10mV       */
#endif
#endif

#if  defined HT6x2x  
#define  PMU_VDETCFG_DisChargeCTRL               ((uint32_t)0xc000)           /*!<��ضۻ�������Сѡ��   */
#define  PMU_VDETCFG_DisChargeCTRL_1mA           ((uint32_t)0x0000)           /*!<��ضۻ�����1mA       */
#define  PMU_VDETCFG_DisChargeCTRL_2mA           ((uint32_t)0x8000)           /*!<��ضۻ�����2mA       */
#define  PMU_VDETCFG_DisChargeCTRL_50uA          ((uint32_t)0x4000)           /*!<��ضۻ�����50uA      */
#define  PMU_VDETCFG_DisChargeCTRL_100uA         ((uint32_t)0xc000)           /*!<��ضۻ�����100uA     */
#elif  defined HT501x  ||  defined  HT502x
#define  PMU_VDETCFG_DisChargeCTRL               ((uint32_t)0x8000)           /*!<��ضۻ�������Сѡ��   */
#define  PMU_VDETCFG_DisChargeCTRL_1mA           ((uint32_t)0x0000)           /*!<��ضۻ�����1mA       */
#define  PMU_VDETCFG_DisChargeCTRL_2mA           ((uint32_t)0x8000)           /*!<��ضۻ�����2mA       */
#endif

/************************  Bit definition for VDETPCFG register of HT_PMU_TypeDef ***********************/
#define  PMU_VDETPCFG		                     ((uint32_t)0x001f)           /*!<��Դ������ڼĴ���      */

#define  PMU_VDETPCFG_VDETPRD                    ((uint32_t)0x0007)           /*!<��ʱ��������趨      */
#define  PMU_VDETPCFG_VDETPRD_16P5mS             ((uint32_t)0x0000)           /*!<��ʱ�������= 16.5ms  */
#define  PMU_VDETPCFG_VDETPRD_33mS               ((uint32_t)0x0001)           /*!<��ʱ�������= 33ms    */
#define  PMU_VDETPCFG_VDETPRD_67mS               ((uint32_t)0x0002)           /*!<��ʱ�������= 67ms    */
#define  PMU_VDETPCFG_VDETPRD_134mS              ((uint32_t)0x0003)           /*!<��ʱ�������= 134ms   */
#define  PMU_VDETPCFG_VDETPRD_268mS              ((uint32_t)0x0004)           /*!<��ʱ�������= 268ms   */
#define  PMU_VDETPCFG_VDETPRD_536mS              ((uint32_t)0x0005)           /*!<��ʱ�������= 536ms   */
#define  PMU_VDETPCFG_VDETPRD_1072mS             ((uint32_t)0x0006)           /*!<��ʱ�������= 1072ms  */
#define  PMU_VDETPCFG_VDETPRD_2144mS             ((uint32_t)0x0007)           /*!<��ʱ�������= 2144ms  */

#define  PMU_VDETPCFG_VDETTIME                   ((uint32_t)0x0018)           /*!<��ʱ���ʱ���趨��ʵ�ʼ��ʱ����ע��Ϊ׼      */
#define  PMU_VDETPCFG_VDETTIME_300uS             ((uint32_t)0x0000)           /*!<��ʱ���ʱ�� = 336us  */
#define  PMU_VDETPCFG_VDETTIME_360uS             ((uint32_t)0x0008)           /*!<��ʱ���ʱ�� = 397us  */
#define  PMU_VDETPCFG_VDETTIME_480uS             ((uint32_t)0x0010)           /*!<��ʱ���ʱ�� = 519us  */
#define  PMU_VDETPCFG_VDETTIME_1120uS            ((uint32_t)0x0018)           /*!<��ʱ���ʱ�� = 1068us  */
#define  PMU_VDETPCFG_VDETTIME_560uS             ((uint32_t)0x0018)           /*!<��ʱ���ʱ�� = 1068us�����ݾɰ汾����  */

/**************************  Bit definition for PMUIE register of HT_PMU_TypeDef ************************/
#if defined  HT6x2x   
#define  PMU_PMUIE                               ((uint32_t)0x001f)           /*!<PMU�ж�ʹ��           */
#elif defined  HT501x  ||  defined  HT502x
#define  PMU_PMUIE                               ((uint32_t)0x000f)           /*!<PMU�ж�ʹ��           */
#elif defined  HT6x1x
#define  PMU_PMUIE                               ((uint32_t)0x0007)           /*!<PMU�ж�ʹ��           */
#endif
#define  PMU_PMUIE_VCCIE                         ((uint32_t)0x0001)           /*!<VCC����ж�ʹ��       */
																																							/*!<  HT502x�˴���Ӧ�ֲ��е�VSYSIE */
#define  PMU_PMUIE_BORIE                         ((uint32_t)0x0002)           /*!<BOR����ж�ʹ��       */
#define  PMU_PMUIE_LVD0IE                        ((uint32_t)0x0004)           /*!<LVDIN0����ж�ʹ��    */
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x                                    
#define  PMU_PMUIE_LVD1IE                        ((uint32_t)0x0008)           /*!<LVDIN1����ж�ʹ��    */
#endif
#if  defined  HT6x2x
#define  PMU_PMUIE_POWIE                         ((uint32_t)0x0010)           /*!<POW����ж�ʹ��       */
#endif
/**************************  Bit definition for PMUIF register of HT_PMU_TypeDef ************************/
#if defined  HT6x2x  
#define  PMU_PMUIF								 ((uint32_t)0x001f)           /*!<PMU�жϱ�־     	  */
#elif defined  HT501x  ||  defined  Ht502x
#define  PMU_PMUIF								 ((uint32_t)0x000f)           /*!<PMU�жϱ�־     	  */
#elif defined  HT6x1x
#define  PMU_PMUIF								 ((uint32_t)0x0007)           /*!<PMU�жϱ�־     	  */
#endif
#define  PMU_PMUIF_VCCIF                         ((uint32_t)0x0001)           /*!<VCC����жϱ�־       */
																																							/*!<  HT502x�˴���Ӧ�ֲ��е�VSYSIF */
#define  PMU_PMUIF_BORIF                         ((uint32_t)0x0002)           /*!<BOR����жϱ�־       */
#define  PMU_PMUIF_LVD0IF                        ((uint32_t)0x0004)           /*!<LVDIN0����жϱ�־    */
#if  defined  HT6x2x   ||  defined  HT501x  ||  defined  HT502x                                                          
#define  PMU_PMUIF_LVD1IF                        ((uint32_t)0x0008)           /*!<LVDIN1����жϱ�־    */
#endif
#if  defined  HT6x2x  
#define  PMU_PMUIF_POWIF                         ((uint32_t)0x0010)           /*!<POW����жϱ�־       */
#endif
/*************************  Bit definition for PMUSTA register of HT_PMU_TypeDef ************************/
#if defined  HT6x2x 
#define  PMU_PMUSTA								 ((uint32_t)0x001f)           /*!<PMU״̬��־     	  */
#elif defined  HT501x  ||  defined  HT502x
#define  PMU_PMUSTA								 ((uint32_t)0x000f)           /*!<PMU״̬��־     	  */
#elif defined  HT6x1x
#define  PMU_PMUSTA								 ((uint32_t)0x0007)           /*!<PMU״̬��־     	  */
#endif
#define  PMU_PMUSTA_VCCFLG                       ((uint32_t)0x0001)           /*!<VCC��ѹ״̬@VCCLVL    */
																																							/*!<  HT502x�˴���Ӧ�ֲ��е�VSYS_FLG */
#define  PMU_PMUSTA_BORFLG                       ((uint32_t)0x0002)           /*!<VCC��ѹ״̬@BORLVL    */
#define  PMU_PMUSTA_LVD0FLG                      ((uint32_t)0x0004)           /*!<LVDIN0����״̬        */
#if  defined  HT6x2x   ||  defined  HT501x  ||  HT502x
#define  PMU_PMUSTA_LVD1FLG                      ((uint32_t)0x0008)           /*!<LVDIN1����״̬        */
#endif
#if  defined  HT6x2x   
#define  PMU_PMUSTA_POWFLG                       ((uint32_t)0x0010)           /*!<POWIN����״̬         */
                                                                              /*!< bit=1�������趨��ֵ  */
#endif
/*************************  Bit definition for WAKEIF register of HT_PMU_TypeDef ************************/
#define  PMU_WAKEIF                              ((uint32_t)0xffffff)         /*!<���ѱ�־              */
#define  PMU_WAKEIF_PMUWKIF                      ((uint32_t)0x0001)           /*!<PMU���ѱ�־           */
#define  PMU_WAKEIF_INT0WKIF                     ((uint32_t)0x0004)           /*!<INT0���ѱ�־          */
#define  PMU_WAKEIF_INT1WKIF                     ((uint32_t)0x0008)           /*!<INT1���ѱ�־          */
#define  PMU_WAKEIF_INT2WKIF                     ((uint32_t)0x0010)           /*!<INT2���ѱ�־          */
#define  PMU_WAKEIF_INT3WKIF                     ((uint32_t)0x0020)           /*!<INT3���ѱ�־          */
#define  PMU_WAKEIF_INT4WKIF                     ((uint32_t)0x0040)           /*!<INT4���ѱ�־          */
#define  PMU_WAKEIF_INT5WKIF                     ((uint32_t)0x0080)           /*!<INT5���ѱ�־          */
#define  PMU_WAKEIF_INT6WKIF                     ((uint32_t)0x0100)           /*!<INT6���ѱ�־          */
#define  PMU_WAKEIF_RX0WKIF                      ((uint32_t)0x0200)           /*!<RX0���ѱ�־           */
#define  PMU_WAKEIF_RX1WKIF                      ((uint32_t)0x0400)           /*!<RX1���ѱ�־           */
#define  PMU_WAKEIF_RX2WKIF                      ((uint32_t)0x0800)           /*!<RX2���ѱ�־           */
#define  PMU_WAKEIF_RX3WKIF                      ((uint32_t)0x1000)           /*!<RX3���ѱ�־           */
#define  PMU_WAKEIF_RX4WKIF                      ((uint32_t)0x2000)           /*!<RX4���ѱ�־           */
#define  PMU_WAKEIF_RX5WKIF                      ((uint32_t)0x4000)           /*!<RX5���ѱ�־           */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  PMU_WAKEIF_TMR4WKIF                     ((uint32_t)0x20000)          /*!<Timer4���ѱ�־        */
#define  PMU_WAKEIF_TMR5WKIF                     ((uint32_t)0x40000)          /*!<Timer5���ѱ�־        */
#endif
#define  PMU_WAKEIF_TBSWKIF                      ((uint32_t)0x80000)          /*!<TBS���ѱ�־           */
#define  PMU_WAKEIF_RTCWKIF                      ((uint32_t)0x100000)         /*!<RTC���ѱ�־           */
#if  defined  HT6x1x ||  defined HT501x
#define  PMU_WAKEIF_WDTWKIF                      ((uint32_t)0x800000)         /*!<WDT���ѱ�־           */
#endif
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  PMU_WAKEIF_RX6WKIF                      ((uint32_t)0x8000000)        /*!<RX6���ѱ�־           */
#define  PMU_WAKEIF_INT7WKIF                     ((uint32_t)0x10000000)       /*!<INT7���ѱ�־          */
#define  PMU_WAKEIF_INT8WKIF                     ((uint32_t)0x20000000)       /*!<INT8���ѱ�־          */
#define  PMU_WAKEIF_INT9WKIF                     ((uint32_t)0x40000000)       /*!<INT9���ѱ�־          */
#endif

#if  defined  HT502x
#define  PMU_WAKEIF_ARGEWKIF                     ((uint32_t)0x00000002)       /*!<AES/RAND/GHASH/ECC���ѱ�־ */
#define  PMU_WAKEIF_TMR0WKIF                     ((uint32_t)0x00008000)       /*!<Timer0���ѱ�־             */
#define  PMU_WAKEIF_TMR1WKIF                     ((uint32_t)0x00010000)       /*!<Timer1���ѱ�־             */
#define  PMU_WAKEIF_TMR2WKIF                     ((uint32_t)0x00020000)       /*!<Timer2���ѱ�־             */
#define  PMU_WAKEIF_TMR3WKIF                     ((uint32_t)0x00040000)       /*!<Timer3���ѱ�־             */
#define  PMU_WAKEIF_IICWKIF                      ((uint32_t)0x00200000)       /*!<IIC���ѱ�־                */
#define  PMU_WAKEIF_SPIWKIF                      ((uint32_t)0x00400000)       /*!<SPI���ѱ�־                */
#define  PMU_WAKEIF_SELFTESTWKIF                 ((uint32_t)0x01000000)       /*!<SelfTest���ѱ�־           */
#define  PMU_WAKEIF_EMUWKIF                      ((uint32_t)0x02000000)       /*!<EMU���ѱ�־                */
#define  PMU_WAKEIF_DMAWKIF                      ((uint32_t)0x04000000)       /*!<DMA���ѱ�־                */
#define  PMU_WAKEIF_KEYWKIF                      ((uint32_t)0x08000000)       /*!<KEY���ѱ�־                */
#define  PMU_WAKEIF_INT7WKIF                     ((uint32_t)0x10000000)       /*!<INT7���ѱ�־               */
#define  PMU_WAKEIF_INT8WKIF                     ((uint32_t)0x20000000)       /*!<INT8���ѱ�־               */
#define  PMU_WAKEIF_INT9WKIF                     ((uint32_t)0x40000000)       /*!<INT9���ѱ�־               */
#define  PMU_WAKEIF_NMIWKIF                      ((uint32_t)0x80000000)       /*!<NMI���ѱ�־                */
#endif

/*************************  Bit definition for RSTSTA register of HT_PMU_TypeDef ************************/
#define  PMU_RSTSTA                              ((uint32_t)0xffff)           /*!<��λ��־λ            */
#define  PMU_RSTSTA_PORRST                       ((uint32_t)0x0001)           /*!<POR��λ��־λ         */
#define  PMU_RSTSTA_LBORRST                      ((uint32_t)0x0002)           /*!<LBOR��λ��־λ        */
#define  PMU_RSTSTA_WDTRST                       ((uint32_t)0x0004)           /*!<WDT��λ��־λ         */
#define  PMU_RSTSTA_WakeupRST                    ((uint32_t)0x0008)           /*!<WakeUp��λ��־        */
#define  PMU_RSTSTA_ExtRST                       ((uint32_t)0x0020)           /*!<�ⲿRST��λ��־       */
#define  PMU_RSTSTA_SoftRST                      ((uint32_t)0x0040)           /*!<��λ��־            */
#define  PMU_RSTSTA_DebugRST                     ((uint32_t)0x0080)           /*!<���Ը�λ��־          */
#define  PMU_RSTSTA_BORRST                       ((uint32_t)0x0100)           /*!<BOR��λ��־           */
#define  PMU_RSTSTA_SleepFlag                    ((uint32_t)0x4000)           /*!<Sleep���Ѹ�λ��־     */
#define  PMU_RSTSTA_HoldFlag                     ((uint32_t)0x8000)           /*!<Hold���ѱ�־          */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of CMU Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for WPREG register of HT_CMU_TypeDef ************************/
#define  CMU_WPREG                               ((uint32_t)0xffff)           /*!<д�����Ĵ���          */
#define  CMU_WPREG_UnProtected                   ((uint32_t)0xA55A)           /*!<�ر�д��������        */
#define  CMU_WPREG_Protected                     ((uint32_t)0x0000)           /*!<��д��������        */
                                                                              /*!<0xA55A: Unprotected   */
                                                                              /*!<other : Protected     */
/************************  Bit definition for SYSCLKCFG register of HT_CMU_TypeDef **********************/
#if  defined  HT6x2x  ||  defined  HT6x1x  ||  defined  HT502x
#define  CMU_SYSCLKCFG_CLKSEL                    ((uint32_t)0x0007)           /*!<ϵͳʱ��ѡ�����λ    */
#elif defined HT501x
#define  CMU_SYSCLKCFG_CLKSEL                    ((uint32_t)0x0003)           /*!<ϵͳʱ��ѡ�����λ    */
#endif
#define  CMU_SYSCLKCFG_CLKSEL_LRC                ((uint32_t)0x0000)           /*!<ѡ��lrc               */
#define  CMU_SYSCLKCFG_CLKSEL_LF                 ((uint32_t)0x0001)           /*!<ѡ���Ƶosc           */
#define  CMU_SYSCLKCFG_CLKSEL_HRC                ((uint32_t)0x0002)           /*!<ѡ��hrc               */
#define  CMU_SYSCLKCFG_CLKSEL_PLL                ((uint32_t)0x0003)           /*!<ѡ��PLL               */
#if  defined  HT6x2x  ||  defined  HT6x1x
#define  CMU_SYSCLKCFG_CLKSEL_MEMS               ((uint32_t)0x0004)           /*!<ѡ��mems              */
#endif
#define  CMU_SYSCLKCFG_WCLKEN                    ((uint32_t)0x0080)           /*!<ʱ������д����λ      */

/*************************  Bit definition for JTAGSTA register of HT_CMU_TypeDef ***********************/
#define  CMU_JTAGSTA                             ((uint32_t)0x0001)           /*!<JTAG״ָ̬ʾ          */

/*************************  Bit definition for LRCADJ register of HT_CMU_TypeDef ************************/
#if defined  HT6x1x ||  defined  HT501x  ||  defined  HT502x
#define  CMU_LRCADJ                              ((uint32_t)0x000f)           /*!<lrc���Ƶ�ʵ���λ     */
#elif  defined  HT6x2x 
#define  CMU_LRCADJ                              ((uint32_t)0x00ff)           /*!<lrc���Ƶ�ʵ���λ     */
#endif                                                                            /*!<HT6x2x���ȴ���        */
/*************************  Bit definition for HRCADJ register of HT_CMU_TypeDef ************************/
#if defined  HT6x1x ||  defined  HT501x
#define  CMU_HRCADJ                              ((uint32_t)0x003f)           /*!<hrc���Ƶ�ʵ���λ     */
#elif  defined  HT6x2x  ||  defined  HT502x
#define  CMU_HRCADJ                              ((uint32_t)0x007f)           /*!<hrc���Ƶ�ʵ���λ     */
#endif                                                                              /*!<HT6x2x���ȴ���        */
/*************************  Bit definition for HRCDIV register of HT_CMU_TypeDef ************************/
#if defined  HT6x1x ||  defined  HT6x2x 
#define  CMU_HRCDIV                              ((uint32_t)0x0003)           /*!<hrcʱ�ӷ�Ƶ����       */
#define  CMU_HRCDIV_1                            ((uint32_t)0x0000)           /*!<��Ƶϵ�� = 1          */
#define  CMU_HRCDIV_2                            ((uint32_t)0x0001)           /*!<��Ƶϵ�� = 2          */
#define  CMU_HRCDIV_4                            ((uint32_t)0x0002)           /*!<��Ƶϵ�� = 4          */
#define  CMU_HRCDIV_8                            ((uint32_t)0x0003)           /*!<��Ƶϵ�� = 8          */
#endif

/*************************  Bit definition for CLKSTA register of HT_CMU_TypeDef ************************/
#if  defined  HT6x2x
#define  CMU_CLKSTA		                         ((uint32_t)0x003b)           /*!<MCUʱ��״̬�Ĵ���       */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKSTA		                         ((uint32_t)0x0039)           /*!<MCUʱ��״̬�Ĵ���       */
#elif defined  HT6x1x
#define  CMU_CLKSTA		                         ((uint32_t)0x001b)           /*!<MCUʱ��״̬�Ĵ���       */
#endif
#define  CMU_CLKSTA_LFFLAG                       ((uint32_t)0x0001)           /*!<LFͣ���־            */
#if  defined  HT6x2x  ||  defined  HT6x1x
#define  CMU_CLKSTA_LFSEL                        ((uint32_t)0x0002)           /*!<0:osc 1:mems          */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKSTA_LFSEL                        ((uint32_t)0x0000)           /*!<0:osc osc only        */
#endif
#define  CMU_CLKSTA_HRCFLAG                      ((uint32_t)0x0008)           /*!<HRCͣ���־           */
#define  CMU_CLKSTA_PLLFLAG                      ((uint32_t)0x0010)           /*!<PLLͣ���־           */
#if defined  HT6x2x   ||  defined  HT501x  ||  defined  HT502x
#define  CMU_CLKSTA_PLLLOCK                      ((uint32_t)0x0020)           /*!<PLL����״̬��־           */
#endif
/***********************  Bit definition for SYSCLKDIV register of HT_CMU_TypeDef ***********************/  
#define  CMU_SYSCLKDIV                           ((uint32_t)0x0007)           /*!<ϵͳʱ�ӷ�Ƶ����      */
#if  defined  HT501x || HT6x2x  ||  HT502x
#define  CMU_SYSCLKDIV_SEL20M                    ((uint32_t)0x0000)           /*!<PLL���20Mʱ��        */
#define  CMU_SYSCLKDIV_SEL40M                    ((uint32_t)0x0008)           /*!<PLL���40Mʱ��        */
#endif
#define  CMU_CPUCLKDIV_1                         ((uint32_t)0x0000)           /*!<��Ƶϵ�� = 1          */
#define  CMU_CPUCLKDIV_2                         ((uint32_t)0x0001)           /*!<��Ƶϵ�� = 2          */
#define  CMU_CPUCLKDIV_4                         ((uint32_t)0x0002)           /*!<��Ƶϵ�� = 4          */
#define  CMU_CPUCLKDIV_8                         ((uint32_t)0x0003)           /*!<��Ƶϵ�� = 8          */
#define  CMU_CPUCLKDIV_16                        ((uint32_t)0x0004)           /*!<��Ƶϵ�� = 16         */
#define  CMU_CPUCLKDIV_32                        ((uint32_t)0x0005)           /*!<��Ƶϵ�� = 32         */
#define  CMU_CPUCLKDIV_64                        ((uint32_t)0x0006)           /*!<��Ƶϵ�� = 64         */
#define  CMU_CPUCLKDIV_128                       ((uint32_t)0x0007)           /*!<��Ƶϵ�� = 128        */

/***********************  Bit definition for CLKOUTSEL register of HT_CMU_TypeDef ***********************/
#define  CMU_CLKOUTSEL                           ((uint32_t)0x0007)           /*!<CLKOUT���ʱ������    */
#define  CMU_CLKOUTSEL_LRC                       ((uint32_t)0x0000)           /*!<LRC --> CLKOUT        */
#define  CMU_CLKOUTSEL_LF                        ((uint32_t)0x0001)           /*!<OSC --> CLKOUT        */
#define  CMU_CLKOUTSEL_HRC                       ((uint32_t)0x0002)           /*!<HRC --> CLKOUT        */
#define  CMU_CLKOUTSEL_SYS                       ((uint32_t)0x0003)           /*!<SYS --> CLKOUT        */
#define  CMU_CLKOUTSEL_MEMS                      ((uint32_t)0x0004)           /*!<MEMS--> CLKOUT        */

/***********************  Bit definition for CLKOUTDIV register of HT_CMU_TypeDef ***********************/
#define  CMU_CLKOUTDIV                           ((uint32_t)0x000f)           /*!<CLKOUT���ʱ������    */
                                                                              /*!<��Ƶϵ�� = 2*(N+1)    */
/************************  Bit definition for CLKCTRL0 register of HT_CMU_TypeDef ***********************/
#if   defined  HT6x1x                                                         
#define  CMU_CLKCTRL0                            ((uint32_t)0x3bfe)           /*!<CLKCTRL0�Ĵ���        */
#elif defined  HT501x
#define  CMU_CLKCTRL0                            ((uint32_t)0xfbfe)           /*!<CLKCTRL0�Ĵ���        */
#elif defined  HT6x2x
#define  CMU_CLKCTRL0                            ((uint32_t)0x01f3fe)         /*!<CLKCTRL0�Ĵ���        */
#elif defined  HT502x
#define  CMU_CLKCTRL0                            ((uint32_t)0x01fffe)         /*!<CLKCTRL0�Ĵ���        */
#endif
#define  CMU_CLKCTRL0_LCDEN                      ((uint32_t)0x0002)           /*!<LCDģ��ʱ��ʹ��       */
#define  CMU_CLKCTRL0_SPI0EN                     ((uint32_t)0x0004)           /*!<SPI0ģ��ʱ��ʹ��      */
#define  CMU_CLKCTRL0_I2CEN                      ((uint32_t)0x0008)           /*!<I2Cģ��ʱ��ʹ��       */
#define  CMU_CLKCTRL0_PLLEN                      ((uint32_t)0x0010)           /*!<PLLģ��ʱ��ʹ��       */
#define  CMU_CLKCTRL0_HRCEN                      ((uint32_t)0x0020)           /*!<HRCģ��ʱ��ʹ��       */
#if defined HT6x2x || defined  HT501x  ||  defined  HT502x
#define  CMU_CLKCTRL0_PLLLOCKEN                  ((uint32_t)0x0040)           /*!<PLL����ʹ��           */
#elif	defined HT6x1x
#define  CMU_CLKCTRL0_Reserved                   ((uint32_t)0x0040)           /*!<����Ϊ1               */
#endif
#define  CMU_CLKCTRL0_LFDETEN                    ((uint32_t)0x0080)           /*!<LFʱ�Ӽ��ģ��ʹ��    */
#define  CMU_CLKCTRL0_PLLDETEN                   ((uint32_t)0x0100)           /*!<PLLʱ�Ӽ��ģ��ʹ��   */
#define  CMU_CLKCTRL0_HRCDETEN                   ((uint32_t)0x0200)           /*!<HRCʱ�Ӽ��ģ��ʹ��   */

#define  CMU_CLKCTRL0_OSC_SLP                    ((uint32_t)0x0400)           /*!<OSC�͹���ʹ�ܿ����ź�   */
#define  CMU_CLKCTRL0_OSC_SLP_LOWPOWER           ((uint32_t)0x0400)           /*!<OSCС����              */
#define  CMU_CLKCTRL0_OSC_SLP_LARGEPOWER         ((uint32_t)0x0000)           /*!<OSC�󹦺�              */

#if  defined  HT6x1x                                                         
#define  CMU_CLKCTRL0_WDTEN                      ((uint32_t)0x0800)           /*!<WDT�͹���ģʽ��ʹ��λ */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKCTRL0_KEYEN                      ((uint32_t)0x0800)           /*!<WDT�͹���ģʽ��ʹ��λ */
#endif
#define  CMU_CLKCTRL0_CLKOUTEN                   ((uint32_t)0x1000)           /*!<CLKOUT���ʹ��λ      */
#define  CMU_CLKCTRL0_1P5LBOREN                  ((uint32_t)0x2000)           /*!<�͹���1P5 LBORʹ��λ  */
#if   defined  HT6x1x                                                        
#define  CMU_CLKCTRL0_3DESRADEN                  ((uint32_t)0x4000)           /*!<3DES/�����ģ��ʹ��λ */
#else                                                                         
#define  CMU_CLKCTRL0_ADESRADEN                  ((uint32_t)0x4000)           /*!<AES/�����ģ��ʹ��λ  */
#endif
#if  defined  HT6x2x 
#define  CMU_CLKCTRL0_CRCEN                      ((uint32_t)0x8000)           /*!<CRCģ��ʹ��λ         */
#elif defined HT501x  ||  HT502x
#define  CMU_CLKCTRL0_EMUEN                      ((uint32_t)0x8000)           /*!<EMUģ��ʹ��λ         */
#endif

#if defined  HT502x
#define  CMU_CLKCTRL0_ECCEN                      ((uint32_t)0x010000)         /*!<ECCģ��ʹ��λ         */
#elif  defined  HT6x2x
#define  CMU_CLKCTRL0_HFRTC_SEL                  ((uint32_t)0x010000)         /*!<��Ƶ����1HZ��Դѡ��λ  */
#define  CMU_CLKCTRL0_HFRTC_SEL_HRC              ((uint32_t)0x010000)         /*!<HRCʱ��               */
#define  CMU_CLKCTRL0_HFRTC_SEL_PLL              ((uint32_t)0x000000)         /*!<PLLʱ��               */
#endif

/************************  Bit definition for CLKCTRL1 register of HT_CMU_TypeDef ***********************/
#if  defined  HT6x1x                                                        
#define  CMU_CLKCTRL1                            ((uint32_t)0x03ff)           /*!<CLKCTRL1�Ĵ���        */
#elif defined HT6x2x
#define  CMU_CLKCTRL1                            ((uint32_t)0x01ffff)         /*!<CLKCTRL1�Ĵ���        */
#elif defined HT501x
#define  CMU_CLKCTRL1                            ((uint32_t)0xc3ff)           /*!<CLKCTRL1�Ĵ���        */
#elif defined HT502x
#define  CMU_CLKCTRL1                            ((uint32_t)0xc3ff)           /*!<CLKCTRL1�Ĵ���        */
#endif
#define  CMU_CLKCTRL1_TMR0EN                     ((uint32_t)0x0001)           /*!<Timer0ģ��ʱ��ʹ��    */
#define  CMU_CLKCTRL1_TMR1EN                     ((uint32_t)0x0002)           /*!<Timer1ģ��ʱ��ʹ��    */
#define  CMU_CLKCTRL1_TMR2EN                     ((uint32_t)0x0004)           /*!<Timer2ģ��ʱ��ʹ��    */
#define  CMU_CLKCTRL1_TMR3EN                     ((uint32_t)0x0008)           /*!<Timer3ģ��ʱ��ʹ��    */
#define  CMU_CLKCTRL1_UART0EN                    ((uint32_t)0x0010)           /*!<UART0ģ��ʱ��ʹ��     */
#define  CMU_CLKCTRL1_UART1EN                    ((uint32_t)0x0020)           /*!<UART1ģ��ʱ��ʹ��     */
#define  CMU_CLKCTRL1_UART2EN                    ((uint32_t)0x0040)           /*!<UART2ģ��ʱ��ʹ��     */
#define  CMU_CLKCTRL1_UART3_7816_1EN             ((uint32_t)0x0080)           /*!<UART37816ģ��ʱ��ʹ�� */
#define  CMU_CLKCTRL1_UART4_7816_0EN             ((uint32_t)0x0100)           /*!<UART47816ģ��ʱ��ʹ�� */
#define  CMU_CLKCTRL1_UART5EN                    ((uint32_t)0x0200)           /*!<UART5ģ��ʱ��ʹ��     */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  CMU_CLKCTRL1_TMR4EN                     ((uint32_t)0x0400)           /*!<Timer4ģ��ʱ��ʹ��    */
#define  CMU_CLKCTRL1_TMR5EN                     ((uint32_t)0x0800)           /*!<Timer5ģ��ʱ��ʹ��    */
#define  CMU_CLKCTRL1_UART6EN                    ((uint32_t)0x1000)           /*!<UART6ģ��ʱ��ʹ��     */
#define  CMU_CLKCTRL1_SPI1EN                     ((uint32_t)0x2000)           /*!<SPI1ģ��ʱ��ʹ��      */
#define  CMU_CLKCTRL1_SOFTWDTEN                  ((uint32_t)0x4000)           /*!<SoftWDTģ��ʱ��ʹ��   */
#define  CMU_CLKCTRL1_LRCRTCEN                   ((uint32_t)0x8000)           /*!<LRCRTCģ��ʱ��ʹ��    */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKCTRL1_LRCTFREQEN                 ((uint32_t)0x4000)           /*!<LRC��Ƶģ��ʹ��       */
#define  CMU_CLKCTRL1_LRCRTCEN                   ((uint32_t)0x8000)           /*!<LRCRTCģ��ʱ��ʹ��    */
#endif

#if  defined  HT6x2x 
#define  CMU_CLKCTRL1_AUTORCCAL_EN               ((uint32_t)0x010000)         /*!<HRC/LRC�Զ�У��ʹ��    */
#endif

/************************  Bit definition for FLASHCON register of HT_CMU_TypeDef ***********************/
#define  CMU_FLASHCON                      	 	 ((uint32_t)0x0007)           /*!<Flash���ƼĴ���         */
#define  CMU_FLASHCON_FOP                        ((uint32_t)0x0003)           /*!<Flash����ģʽ         */
#define  CMU_FLASHCON_FOP_READ                   ((uint32_t)0x0000)           /*!<Flash����ֻ��ģʽ     */
#define  CMU_FLASHCON_FOP_WRITE                  ((uint32_t)0x0001)           /*!<Flash����дģʽ       */
#define  CMU_FLASHCON_FOP_PAGEERASE              ((uint32_t)0x0002)           /*!<Flash����ҳ����ģʽ   */
#define  CMU_FLASHCON_FOP_CHIPERASE              ((uint32_t)0x0003)           /*!<Flash����ȫ����ģʽ   */
                                                                                                        
#define  CMU_FLASHCON_BUSY                       ((uint32_t)0x0004)           /*!<Flash״̬ 0:���� 1:æ */

/************************  Bit definition for FLASHLOCK register of HT_CMU_TypeDef **********************/
#define  CMU_FLASHLOCK                           ((uint32_t)0xffff)           /*!<Flash�����Ĵ���       */
#define  CMU_FLASHLOCK_UnLocked                  ((uint32_t)0x7A68)           /*!<Flashд/��������      */
#define  CMU_FLASHLOCK_Locked                    ((uint32_t)0x0000)           /*!<Flash����             */
                                                                              /*!<0x7A68 : Unlock       */
                                                                              /*!<other  : Lock         */
                                                                              
/************************  Bit definition for PREFETCH register of HT_CMU_TypeDef ***********************/
#if  defined  HT6x2x                                                          
#define  CMU_PREFETCH                            ((uint32_t)0x0001)           /*!<ָ��Ԥȡʹ�ܿ���      */
#define  CMU_PREFETCH_ENABLE                     ((uint32_t)0x0001)           /*!<ָ��Ԥȡʹ��          */
#define  CMU_PREFETCH_DISABLE                    ((uint32_t)0x0000)           /*!<ָ��Ԥȡ��ֹ          */
#endif

/************************  Bit definition for FLASHDLY register of HT_CMU_TypeDef ***********************/
#if  defined  HT501x  ||  defined  HT502x
#define  CMU_FLASHDLY                            ((uint32_t)0x0001)           /*!<Flashȡָ��ȴ�����   */
#define  CMU_FLASHDLY_ENABLE                     ((uint32_t)0x0001)           /*!<Flashȡָ��ȴ�       */
#define  CMU_FLASHDLY_DISABLE                    ((uint32_t)0x0000)           /*!<Flashȡָ��ȴ�     */
#endif

#if  defined  HT502x
/************************  Bit definition for FLASHCON2 register of HT_CMU_TypeDef **********************/
#define  CMU_FLASHCON2                     	 	 ((uint32_t)0xffff)           /*!<Flash���ƼĴ���2        */
#define  CMU_FLASHCON2_FOP                       ((uint32_t)0xffff)           /*!<Flash����ģʽ         */
#define  CMU_FLASHCON2_FOP_READ                  ((uint32_t)0x0000)           /*!<Flash����ֻ��ģʽ     */
#define  CMU_FLASHCON2_FOP_WRITE                 ((uint32_t)0xca53)           /*!<Flash����дģʽ       */
#define  CMU_FLASHCON2_FOP_PAGEERASE             ((uint32_t)0xac35)           /*!<Flash����ҳ����ģʽ   */
#endif

#if  defined  HT6x2x                                                          
/************************  Bit definition for RCCALICON register of HT_CMU_TypeDef **********************/
#define  CMU_RCCALICON			                 ((uint32_t)0x001f)           /*!<HRCУ�����üĴ���   */

#define  CMU_RCCALICON_HRCAutoCali               ((uint32_t)0x0001)           /*!<HRC�Զ�������ʹ��λ   */
#define  CMU_RCCALICON_LRCAutoCali               ((uint32_t)0x0002)           /*!<LRC�Զ�������ʹ��λ   */
#define  CMU_RCCALICON_HRCCaliStart              ((uint32_t)0x0004)           /*!<HRC��Ƶʹ��           */
#define  CMU_RCCALICON_LRCCaliStart              ((uint32_t)0x0008)           /*!<LRC��Ƶʹ��           */

#define  CMU_RCCALICON_FreqSource                ((uint32_t)0x0010)           /*!<����1Hzʱ����Դѡ��   */
#define  CMU_RCCALICON_FreqSourceInner           ((uint32_t)0x0000)           /*!<����1Hzʱ����Դ�ڲ�1Hz*/
#define  CMU_RCCALICON_FreqSourceOut             ((uint32_t)0x0010)           /*!<����1Hzʱ����Դ�ⲿ1Hz*/

/************************  Bit definition for RCCALIIE register of HT_CMU_TypeDef ***********************/
#define  CMU_RCCALIIE		                    ((uint32_t)0x0003)           /*!<HRC/LRC��Ƶ�ж�ʹ��    */

#define  CMU_RCCALIIE_HRCCaliIE                  ((uint32_t)0x0001)           /*!<HRC��Ƶ�ж�ʹ��λ     */
#define  CMU_RCCALIIE_LRCCaliIE                  ((uint32_t)0x0002)           /*!<LRC��Ƶ�ж�ʹ��λ     */

/************************  Bit definition for RCCALIIF register of HT_CMU_TypeDef ***********************/
#define  CMU_RCCALIIF			                 ((uint32_t)0x0003)           /*!<HRC/LRC��Ƶ�жϱ�־λ     */

#define  CMU_RCCALIIF_HRCCaliIF                  ((uint32_t)0x0001)           /*!<HRC��Ƶ�жϱ�־λ     */
#define  CMU_RCCALIIF_LRCCaliIF                  ((uint32_t)0x0002)           /*!<LRC��Ƶ�жϱ�־λ     */

/************************  Bit definition for HRCVALUE register of HT_CMU_TypeDef ***********************/
#define  CMU_HRCVALUE                            ((uint32_t)0xffffff)         /*!<HRC��Ƶֵ             */

/************************  Bit definition for LRCVALUE register of HT_CMU_TypeDef ***********************/
#define  CMU_LRCVALUE                            ((uint32_t)0x1ffff)          /*!<LRC��Ƶֵ             */
#endif

#if defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/*************************  Bit definition for CHIPID register of HT_CMU_TypeDef ************************/
#define  CMU_CHIPID                              ((uint32_t)0xffff)           /*!<ChipID                */
#endif

/*************************  Bit definition for INFOLOCK register of HT_CMU_TypeDef ************************/
#define  CMU_INFOLOCK                            ((uint32_t)0xffff)           /*!<Info�����Ĵ���        */
#define  CMU_INFOLOCK_UnLocked                   ((uint32_t)0xf998)           /*!<Infoд/��������       */
#define  CMU_INFOLOCK_Locked                     ((uint32_t)0x0000)           /*!<Info����              */
                                                                              /*!<0xF998 : Unlock        */
                                                                              /*!<other  : Lock          */

#if defined HT502x
/*************************  Bit definition for FS1LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS1LOCK                             ((uint32_t)0xffff)           /*!<FlashSector1�����Ĵ���    */
#define  CMU_FS1LOCK_UnLocked                    ((uint32_t)0x7161)           /*!<FlashSector1д/��������   */
#define  CMU_FS1LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector1����          */
                                                                              /*!<0x7161 : Unlock           */
                                                                              /*!<other  : Lock             */
																																							
/*************************  Bit definition for FS2LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS2LOCK                             ((uint32_t)0xffff)           /*!<FlashSector2�����Ĵ���    */
#define  CMU_FS2LOCK_UnLocked                    ((uint32_t)0x7262)           /*!<FlashSector2д/��������   */
#define  CMU_FS2LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector2����          */
                                                                              /*!<0x7262 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS3LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS3LOCK                             ((uint32_t)0xffff)           /*!<FlashSector3�����Ĵ���    */
#define  CMU_FS3LOCK_UnLocked                    ((uint32_t)0x7363)           /*!<FlashSector3д/��������   */
#define  CMU_FS3LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector3����          */
                                                                              /*!<0x7363 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS4LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS4LOCK                             ((uint32_t)0xffff)           /*!<FlashSector4�����Ĵ���    */
#define  CMU_FS4LOCK_UnLocked                    ((uint32_t)0x7464)           /*!<FlashSector4д/��������   */
#define  CMU_FS4LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector4����          */
                                                                              /*!<0x7464 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS5LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS5LOCK                             ((uint32_t)0xffff)           /*!<FlashSector5�����Ĵ���    */
#define  CMU_FS5LOCK_UnLocked                    ((uint32_t)0x7565)           /*!<FlashSector5д/��������   */
#define  CMU_FS5LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector5����          */
                                                                              /*!<0x7565 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS6LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS6LOCK                             ((uint32_t)0xffff)           /*!<FlashSector6�����Ĵ���    */
#define  CMU_FS6LOCK_UnLocked                    ((uint32_t)0x7666)           /*!<FlashSector6д/��������   */
#define  CMU_FS6LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector6����          */
                                                                              /*!<0x7666 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS7LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS7LOCK                             ((uint32_t)0xffff)           /*!<FlashSector7�����Ĵ���    */
#define  CMU_FS7LOCK_UnLocked                    ((uint32_t)0x7767)           /*!<FlashSector7д/��������   */
#define  CMU_FS7LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector7����          */
                                                                              /*!<0x7767 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS8LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS8LOCK                             ((uint32_t)0xffff)           /*!<FlashSector8�����Ĵ���    */
#define  CMU_FS8LOCK_UnLocked                    ((uint32_t)0x7868)           /*!<FlashSector8д/��������   */
#define  CMU_FS8LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector8����          */
                                                                              /*!<0x7868 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS9LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS9LOCK                             ((uint32_t)0xffff)           /*!<FlashSector9�����Ĵ���    */
#define  CMU_FS9LOCK_UnLocked                    ((uint32_t)0x7969)           /*!<FlashSector9д/��������   */
#define  CMU_FS9LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector9����          */
                                                                              /*!<0x7969 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FSALOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FSALOCK                             ((uint32_t)0xffff)           /*!<FlashSectorA�����Ĵ���    */
#define  CMU_FSALOCK_UnLocked                    ((uint32_t)0x7a6a)           /*!<FlashSectorAд/��������   */
#define  CMU_FSALOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSectorA����          */
                                                                              /*!<0x7A6A : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FSBLOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FSBLOCK                             ((uint32_t)0xffff)           /*!<FlashSectorB�����Ĵ���    */
#define  CMU_FSBLOCK_UnLocked                    ((uint32_t)0x7b6b)           /*!<FlashSectorBд/��������   */
#define  CMU_FSBLOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSectorB����          */
                                                                              /*!<0x7B6B : Unlock           */
                                                                              /*!<other  : Lock             */
#endif
 
/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of WDT Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for WDTCFG register of HT_WDT_TypeDef ***********************/
#if   defined  HT6x1x 
#define  WDT_WDTCFG_INTRST                       ((uint32_t)0x0001)           /*!<WDT������λ/�ж�ѡ��  */  
#define  WDT_WDTCFG_INT                          ((uint32_t)0x0001)           /*!<WDT��������ж�       */
#define  WDT_WDTCFG_RST                          ((uint32_t)0x0000)           /*!<WDT������λ           */ 
#endif

/**************************  Bit definition for WDTCLR register of HT_WDT_TypeDef ***********************/
#define  WDT_WDTCLR                              ((uint32_t)0xffff)           /*!<WDTι����ʱ������     */

/**************************  Bit definition for WDTCNT register of HT_WDT_TypeDef ***********************/
#define  WDT_WDTCNT                              ((uint32_t)0xffff)           /*!<WDT�����Ĵ���         */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of GPIO Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for IOCFG register of HT_GPIO_TypeDef ***********************/
#define  GPIO_IOCFG                              ((uint32_t)0xffff)           /*!<0:GPIO 1:FunctionPin  */

/**************************  Bit definition for AFCFG register of HT_GPIO_TypeDef ***********************/
#define  GPIO_AFCFG                              ((uint32_t)0xffff)           /*!<0:Func1 1:Func2       */

/**************************  Bit definition for PTDIR register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTDIR                              ((uint32_t)0xffff)           /*!<0:In 1:Out  GPIOOnly  */

/**************************  Bit definition for PTUP register of HT_GPIO_TypeDef ************************/
#define  GPIO_PTUP                               ((uint32_t)0xffff)           /*!<0:Pull-up 1:NoPull-up*/

/**************************  Bit definition for PTDAT register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTDAT                              ((uint32_t)0xffff)           /*!<Data                  */

/**************************  Bit definition for PTSET register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTSET                              ((uint32_t)0xffff)           /*!<Write 1 to Set High   */

/**************************  Bit definition for PTCLR register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTCLR                              ((uint32_t)0xffff)           /*!<Write 1 to Set Low    */

/**************************  Bit definition for PTTOG register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTTOG                              ((uint32_t)0xffff)           /*!<Write 1 to Toggle     */

/**************************  Bit definition for PTOD register of HT_GPIO_TypeDef ************************/
#define  GPIO_PTOD                               ((uint32_t)0xffff)           /*!<0:OD 1:Push-Pull      */

/*********************  Bit definition for HT_GPIOHDPORT register of HT_GPIO_TypeDef ********************/
#define  GPIO_HDPORT                             ((uint32_t)0x000f)           /*!<�����ģʽ�趨        */
#define  GPIO_HDPORT_PC0                         ((uint32_t)0x0001)           /*!<PC0�����ģʽ�趨     */
#define  GPIO_HDPORT_PA6                         ((uint32_t)0x0002)           /*!<PA6�����ģʽ�趨     */
#define  GPIO_HDPORT_PA7                         ((uint32_t)0x0004)           /*!<PA7�����ģʽ�趨     */
#define  GPIO_HDPORT_PA8                         ((uint32_t)0x0008)           /*!<PA8�����ģʽ�趨     */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of INT Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for EXTIE register of HT_INT_TypeDef ************************/
#define  INT_EXTIE_FIE                           ((uint32_t)0x007f)           /*!<�½����ж�ʹ��        */
#define  INT_EXTIE_FIE_INT0                      ((uint32_t)0x0001)           /*!<INT0�½����ж�ʹ��    */
#define  INT_EXTIE_FIE_INT1                      ((uint32_t)0x0002)           /*!<INT1�½����ж�ʹ��    */
#define  INT_EXTIE_FIE_INT2                      ((uint32_t)0x0004)           /*!<INT2�½����ж�ʹ��    */
#define  INT_EXTIE_FIE_INT3                      ((uint32_t)0x0008)           /*!<INT3�½����ж�ʹ��    */
#define  INT_EXTIE_FIE_INT4                      ((uint32_t)0x0010)           /*!<INT4�½����ж�ʹ��    */
#define  INT_EXTIE_FIE_INT5                      ((uint32_t)0x0020)           /*!<INT5�½����ж�ʹ��    */
#define  INT_EXTIE_FIE_INT6                      ((uint32_t)0x0040)           /*!<INT6�½����ж�ʹ��    */

#define  INT_EXTIE_RIE                           ((uint32_t)0x7f00)           /*!<�������ж�ʹ��        */
#define  INT_EXTIE_RIE_INT0                      ((uint32_t)0x0100)           /*!<INT0�������ж�ʹ��    */
#define  INT_EXTIE_RIE_INT1                      ((uint32_t)0x0200)           /*!<INT1�������ж�ʹ��    */
#define  INT_EXTIE_RIE_INT2                      ((uint32_t)0x0400)           /*!<INT2�������ж�ʹ��    */
#define  INT_EXTIE_RIE_INT3                      ((uint32_t)0x0800)           /*!<INT3�������ж�ʹ��    */
#define  INT_EXTIE_RIE_INT4                      ((uint32_t)0x1000)           /*!<INT4�������ж�ʹ��    */
#define  INT_EXTIE_RIE_INT5                      ((uint32_t)0x2000)           /*!<INT5�������ж�ʹ��    */
#define  INT_EXTIE_RIE_INT6                      ((uint32_t)0x4000)           /*!<INT6�������ж�ʹ��    */

/**************************  Bit definition for EXTIF register of HT_INT_TypeDef ************************/
#define  INT_EXTIF_FIF                           ((uint32_t)0x007f)           /*!<�½����жϱ�־        */
#define  INT_EXTIF_FIF_INT0                      ((uint32_t)0x0001)           /*!<INT0�½����жϱ�־    */
#define  INT_EXTIF_FIF_INT1                      ((uint32_t)0x0002)           /*!<INT1�½����жϱ�־    */
#define  INT_EXTIF_FIF_INT2                      ((uint32_t)0x0004)           /*!<INT2�½����жϱ�־    */
#define  INT_EXTIF_FIF_INT3                      ((uint32_t)0x0008)           /*!<INT3�½����жϱ�־    */
#define  INT_EXTIF_FIF_INT4                      ((uint32_t)0x0010)           /*!<INT4�½����жϱ�־    */
#define  INT_EXTIF_FIF_INT5                      ((uint32_t)0x0020)           /*!<INT5�½����жϱ�־    */
#define  INT_EXTIF_FIF_INT6                      ((uint32_t)0x0040)           /*!<INT6�½����жϱ�־    */

#define  INT_EXTIF_RIF                           ((uint32_t)0x7f00)           /*!<�������жϱ�־        */
#define  INT_EXTIF_RIF_INT0                      ((uint32_t)0x0100)           /*!<INT0�������жϱ�־    */
#define  INT_EXTIF_RIF_INT1                      ((uint32_t)0x0200)           /*!<INT1�������жϱ�־    */
#define  INT_EXTIF_RIF_INT2                      ((uint32_t)0x0400)           /*!<INT2�������жϱ�־    */
#define  INT_EXTIF_RIF_INT3                      ((uint32_t)0x0800)           /*!<INT3�������жϱ�־    */
#define  INT_EXTIF_RIF_INT4                      ((uint32_t)0x1000)           /*!<INT4�������жϱ�־    */
#define  INT_EXTIF_RIF_INT5                      ((uint32_t)0x2000)           /*!<INT5�������жϱ�־    */
#define  INT_EXTIF_RIF_INT6                      ((uint32_t)0x4000)           /*!<INT6�������жϱ�־    */

/**************************  Bit definition for PINFLT register of HT_INT_TypeDef ***********************/
#define	 INT_PINFLT								 ((uint32_t)0x7f7f)    		   /*!<�ⲿ���������˲��Ĵ��� */

#define	 INT_PINFLT_INTFLT						 	 				((uint32_t)0x007f)    		   /*!<INT0-6�����˲�ʹ��   */
#define	 INT_PINFLT_INTFLT_INT0					 				((uint32_t)0x0001)    		   /*!<INT0�����˲�ʹ�� 	  */
#define	 INT_PINFLT_INTFLT_INT1					 				((uint32_t)0x0002)    		   /*!<INT1�����˲�ʹ�� 	  */
#define	 INT_PINFLT_INTFLT_INT2					 				((uint32_t)0x0004)    		   /*!<INT2�����˲�ʹ�� 	  */
#define	 INT_PINFLT_INTFLT_INT3					 				((uint32_t)0x0008)    		   /*!<INT3�����˲�ʹ�� 	  */
#define	 INT_PINFLT_INTFLT_INT4					 				((uint32_t)0x0010)    		   /*!<INT4�����˲�ʹ�� 	  */
#define	 INT_PINFLT_INTFLT_INT5					 				((uint32_t)0x0020)    		   /*!<INT5�����˲�ʹ�� 	  */
#define	 INT_PINFLT_INTFLT_INT6					 				((uint32_t)0x0040)    		   /*!<INT6�����˲�ʹ�� 	  */

#define	 INT_PINFLT_RXFLT						 		 				((uint32_t)0x7f00)    		   /*!<RX0-6�����˲�ʹ��   */
#define	 INT_PINFLT_RXFLT_RX0					 					((uint32_t)0x0100)    		   /*!<RX0�����˲�ʹ�� 	  */
#define	 INT_PINFLT_RXFLT_RX1					 					((uint32_t)0x0200)    		   /*!<RX1�����˲�ʹ�� 	  */
#define	 INT_PINFLT_RXFLT_RX2					 					((uint32_t)0x0400)    		   /*!<RX2�����˲�ʹ�� 	  */
#define	 INT_PINFLT_RXFLT_RX3					 					((uint32_t)0x0800)    		   /*!<RX3�����˲�ʹ�� 	  */
#define	 INT_PINFLT_RXFLT_RX4					 					((uint32_t)0x1000)    		   /*!<RX4�����˲�ʹ�� 	  */
#define	 INT_PINFLT_RXFLT_RX5					 					((uint32_t)0x2000)    		   /*!<RX5�����˲�ʹ�� 	  */
#define	 INT_PINFLT_RXFLT_RX6					 					((uint32_t)0x4000)    		   /*!<RX6�����˲�ʹ�� 	  */

#if  defined  HT6x2x  ||  defined  HT502x
/**************************  Bit definition for EXTIE2 register of HT_INT_TypeDef ***********************/
#define  INT_EXTIE2_FIE                          ((uint32_t)0x0007<<16)       /*!<�½����ж�ʹ��        */
#define  INT_EXTIE2_FIE_INT7                     ((uint32_t)0x0001<<16)       /*!<INT7�½����ж�ʹ��    */
#define  INT_EXTIE2_FIE_INT8                     ((uint32_t)0x0002<<16)       /*!<INT8�½����ж�ʹ��    */
#define  INT_EXTIE2_FIE_INT9                     ((uint32_t)0x0004<<16)       /*!<INT9�½����ж�ʹ��    */

#define  INT_EXTIE2_RIE                          ((uint32_t)0x0700<<16)       /*!<�������ж�ʹ��        */
#define  INT_EXTIE2_RIE_INT7                     ((uint32_t)0x0100<<16)       /*!<INT7�������ж�ʹ��    */
#define  INT_EXTIE2_RIE_INT8                     ((uint32_t)0x0200<<16)       /*!<INT8�������ж�ʹ��    */
#define  INT_EXTIE2_RIE_INT9                     ((uint32_t)0x0400<<16)       /*!<INT9�������ж�ʹ��    */

/**************************  Bit definition for EXTIF2 register of HT_INT_TypeDef ***********************/
#define  INT_EXTIF2_FIF                          ((uint32_t)0x0007<<16)       /*!<�½����жϱ�־        */
#define  INT_EXTIF2_FIF_INT7                     ((uint32_t)0x0001<<16)       /*!<INT7�½����жϱ�־    */
#define  INT_EXTIF2_FIF_INT8                     ((uint32_t)0x0002<<16)       /*!<INT8�½����жϱ�־    */
#define  INT_EXTIF2_FIF_INT9                     ((uint32_t)0x0004<<16)       /*!<INT9�½����жϱ�־    */

#define  INT_EXTIF2_RIF                          ((uint32_t)0x0700<<16)       /*!<�������жϱ�־        */
#define  INT_EXTIF2_RIF_INT7                     ((uint32_t)0x0100<<16)       /*!<INT7�������жϱ�־    */
#define  INT_EXTIF2_RIF_INT8                     ((uint32_t)0x0200<<16)       /*!<INT8�������жϱ�־    */
#define  INT_EXTIF2_RIF_INT9                     ((uint32_t)0x0400<<16)       /*!<INT9�������жϱ�־    */

/**************************  Bit definition for PINFLT2 register of HT_INT_TypeDef ***********************/
#define	 INT_PINFLT2							 							((uint32_t)0x0007<<16)    		/*!<�ⲿ���������˲��Ĵ���2 */
#define	 INT_PINFLT2_INTFLT						 					((uint32_t)0x0007<<16)    		/*!<INT7-9�����˲�ʹ��  */
#define	 INT_PINFLT2_INTFLT_INT7				 				((uint32_t)0x0001<<16)    		/*!<INT7�����˲�ʹ��    */
#define	 INT_PINFLT2_INTFLT_INT8				 				((uint32_t)0x0002<<16)    		/*!<INT8�����˲�ʹ��    */
#define	 INT_PINFLT2_INTFLT_INT9				 				((uint32_t)0x0004<<16)    		/*!<INT9�����˲�ʹ��    */
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of DMA Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/**************************  Bit definition for DMAIE register of HT_DMA_TypeDef ************************/
#define  DMA_DMAIE		                         ((uint32_t)0x0777)           /*!<DMA�ж�ʹ�ܼĴ���	 */

#define  DMA_DMAIE_TCIE0                         ((uint32_t)0x0001)           /*!<ͨ��0��������ж�ʹ�� */
#define  DMA_DMAIE_TCIE1                         ((uint32_t)0x0002)           /*!<ͨ��1��������ж�ʹ�� */
#define  DMA_DMAIE_TCIE2                         ((uint32_t)0x0004)           /*!<ͨ��2��������ж�ʹ�� */

#define  DMA_DMAIE_BCIE0                         ((uint32_t)0x0010)           /*!<ͨ��0�봫���ж�ʹ��   */
#define  DMA_DMAIE_BCIE1                         ((uint32_t)0x0020)           /*!<ͨ��1�봫���ж�ʹ��   */
#define  DMA_DMAIE_BCIE2                         ((uint32_t)0x0040)           /*!<ͨ��2�봫���ж�ʹ��   */

#define  DMA_DMAIE_TEIE0                         ((uint32_t)0x0100)           /*!<ͨ��0��������ж�ʹ�� */
#define  DMA_DMAIE_TEIE1                         ((uint32_t)0x0200)           /*!<ͨ��1��������ж�ʹ�� */
#define  DMA_DMAIE_TEIE2                         ((uint32_t)0x0400)           /*!<ͨ��2��������ж�ʹ�� */

/**************************  Bit definition for DMAIF register of HT_DMA_TypeDef ************************/
#define  DMA_DMAIF		                         ((uint32_t)0x0777)           /*!<DMA�жϱ�־�Ĵ���     */

#define  DMA_DMAIF_TCIF0                         ((uint32_t)0x0001)           /*!<ͨ��0������ɱ�־     */
#define  DMA_DMAIF_TCIF1                         ((uint32_t)0x0002)           /*!<ͨ��1������ɱ�־     */
#define  DMA_DMAIF_TCIF2                         ((uint32_t)0x0004)           /*!<ͨ��2������ɱ�־     */

#define  DMA_DMAIF_BCIF0                         ((uint32_t)0x0010)           /*!<ͨ��0�봫����ɱ�־   */
#define  DMA_DMAIF_BCIF1                         ((uint32_t)0x0020)           /*!<ͨ��1�봫����ɱ�־   */
#define  DMA_DMAIF_BCIF2                         ((uint32_t)0x0040)           /*!<ͨ��2�봫����ɱ�־   */

#define  DMA_DMAIF_TEIF0                         ((uint32_t)0x0100)           /*!<ͨ��0��������־     */
#define  DMA_DMAIF_TEIF1                         ((uint32_t)0x0200)           /*!<ͨ��1��������־     */
#define  DMA_DMAIF_TEIF2                         ((uint32_t)0x0400)           /*!<ͨ��2��������־     */

/**************************  Bit definition for CHNSTA register of HT_DMA_TypeDef ***********************/
#define  DMA_CHNSTA                        		 ((uint32_t)0x0007)           /*!<DMA״̬�Ĵ���         */
#define  DMA_CHNSTA_BUSY0                        ((uint32_t)0x0001)           /*!<ͨ��0æ��־           */
#define  DMA_CHNSTA_BUSY1                        ((uint32_t)0x0002)           /*!<ͨ��1æ��־           */
#define  DMA_CHNSTA_BUSY2                        ((uint32_t)0x0004)           /*!<ͨ��2æ��־           */
#endif

#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/*********************  Bit definition for CHNCTL register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNCTL		                         ((uint32_t)0x7fff)           /*!<DMAͨ�����ƼĴ���      */

#define  DMA_CHNCTL_CHNEN                        ((uint32_t)0x0001)           /*!<DMAͨ��ʹ��           */

#define  DMA_CHNCTL_PSIZE                        ((uint32_t)0x0006)           /*!<����λ������          */
#define  DMA_CHNCTL_PSIZE_Byte                   ((uint32_t)0x0000)           /*!<����λ�� = Byte       */
#define  DMA_CHNCTL_PSIZE_HalfWord               ((uint32_t)0x0002)           /*!<����λ�� = HalfWord   */
#define  DMA_CHNCTL_PSIZE_Word                   ((uint32_t)0x0004)           /*!<����λ�� = Word       */

#define  DMA_CHNCTL_MODE                         ((uint32_t)0x0008)           /*!<����ģʽ����          */
#define  DMA_CHNCTL_MODE_WordTransfer            ((uint32_t)0x0000)           /*!<��������              */
#define  DMA_CHNCTL_MODE_BlockTransfer           ((uint32_t)0x0008)           /*!<�鴫��                */

#define  DMA_CHNCTL_CYCLE                        ((uint32_t)0x0010)           /*!<ѭ����������          */
#define  DMA_CHNCTL_CYCLE_DISABLE                ((uint32_t)0x0000)           /*!<��ѭ��ģʽ            */
#define  DMA_CHNCTL_CYCLE_ENABLE                 ((uint32_t)0x0010)           /*!<ѭ��ģʽ              */

#define  DMA_CHNCTL_SOURCEINC                    ((uint32_t)0x0060)           /*!<Դ��ַ����ģʽ����    */
#define  DMA_CHNCTL_SOURCEINC_NOINC              ((uint32_t)0x0000)           /*!<������                */
#define  DMA_CHNCTL_SOURCEINC_INC                ((uint32_t)0x0020)           /*!<����                  */
#define  DMA_CHNCTL_SOURCEINC_BLOCKINC           ((uint32_t)0x0040)           /*!<���ݿ���ѭ������      */

#define  DMA_CHNCTL_DESTININC                    ((uint32_t)0x0180)           /*!<Ŀ�ĵ�ַ����ģʽ����  */
#define  DMA_CHNCTL_DESTININC_NOINC              ((uint32_t)0x0000)           /*!<������                */
#define  DMA_CHNCTL_DESTININC_INC                ((uint32_t)0x0080)           /*!<����                  */
#define  DMA_CHNCTL_DESTININC_BLOCKINC           ((uint32_t)0x0100)           /*!<���ݿ���ѭ������      */

#define  DMA_CHNCTL_CHANNEL                      ((uint32_t)0x3e00)           /*!<ͨ��ѡ��              */

/*********************  Bit definition for CHNSRC register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNSRC                              ((uint32_t)0xffffffff)       /*!<Դ��ַ�Ĵ���          */

/*********************  Bit definition for CHNTAR register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNTAR                              ((uint32_t)0xffffffff)       /*!<Ŀ�ĵ�ַ�Ĵ���        */

/*********************  Bit definition for CHNCNT register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNCNT                              ((uint32_t)0xffff)           /*!<������������          */

/********************  Bit definition for CHNTCCNT register of HT_DMA_Channel_TypeDef *******************/
#define  DMA_CHNTCCNT                            ((uint32_t)0xffff)           /*!<����ɴ�������        */

/*******************  Bit definition for CHNBULKNUM register of HT_DMA_Channel_TypeDef ******************/
#if defined HT6x2x
#define  DMA_CHNBULKNUM                          ((uint32_t)0xffff)           /*!<���ݿ������ݳ�������  */
#define  DMA_CHNBULKNUM_NUM                      ((uint32_t)0x00ff)           /*!<ѭ���������� */
#define  DMA_CHNBULKNUM_CYCLE                    ((uint32_t)0xff00)           /*!<���ݿ������ݳ�������  */
#else
#define  DMA_CHNBULKNUM                          ((uint32_t)0x00ff)           /*!<���ݿ������ݳ�������  */
#define  DMA_CHNBULKNUM_NUM                      ((uint32_t)0x00ff)           /*!<ѭ���������� */
#endif
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of CRC Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
/*************************  Bit definition for CRCCON register of HT_CRC_TypeDef ************************/
#define  CRC_CRCCON_MODE                         ((uint32_t)0x0003)           /*!<CRCģʽ����           */
#define  CRC_CRCCON_MODE_CCITT                   ((uint32_t)0x0000)           /*!<CRC-CCITT             */
#define  CRC_CRCCON_MODE_CRC16                   ((uint32_t)0x0001)           /*!<CRC-16                */
#define  CRC_CRCCON_MODE_CRC32                   ((uint32_t)0x0002)           /*!<CRC-32                */

#define  CRC_CRCCON_RESET                        ((uint32_t)0x0004)           /*!<CRC���㸴λ           */

/*************************  Bit definition for CRCDAT register of HT_CRC_TypeDef ************************/
#define  CRC_CRCDAT                              ((uint32_t)0xffffffff)       /*!<CRC���ݼĴ���         */
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of AES Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/*************************  Bit definition for AESCON register of HT_AES_TypeDef ************************/
#define  AES_AESCON_MODE                         ((uint32_t)0x0001)           /*!<AESģʽ����           */
#define  AES_AESCON_MODE_ENCRYPT                 ((uint32_t)0x0000)           /*!<AESģʽ������         */
#define  AES_AESCON_MODE_DECRYPT                 ((uint32_t)0x0001)           /*!<AESģʽ������         */

#define  AES_AESCON_KEYMODE                      ((uint32_t)0x0006)           /*!<��Կ��������          */
#define  AES_AESCON_KEYMODE_AES128               ((uint32_t)0x0000)           /*!<��Կ���ȣ�128bits     */
#define  AES_AESCON_KEYMODE_AES192               ((uint32_t)0x0002)           /*!<��Կ���ȣ�192bits     */
#define  AES_AESCON_KEYMODE_AES256               ((uint32_t)0x0004)           /*!<��Կ���ȣ�256bits     */

/*************************  Bit definition for AESSTR register of HT_AES_TypeDef ************************/
#define  AES_AESSTR                              ((uint32_t)0xffff)           /*!<AES�����Ĵ���         */
#define  AES_AESSTR_NormalStart                  ((uint32_t)0x8329)           /*!<��ͨ����              */
#define  AES_AESSTR_KeyStart                     ((uint32_t)0x8581)           /*!<KEY��������           */

/*************************  Bit definition for AESFLG register of HT_AES_TypeDef ************************/
#define  AES_AESFLG_BUSY                         ((uint32_t)0x0001)           /*!<AES æ��־            */

/*************************  Bit definition for AESINLL register of HT_AES_TypeDef ***********************/
#define  AES_AESINLL                             ((uint32_t)0xffffffff)       /*!<AES�������ݵ�bit0-31  */

/*************************  Bit definition for AESINML register of HT_AES_TypeDef ***********************/
#define  AES_AESINML                             ((uint32_t)0xffffffff)       /*!<AES�������ݵ�bit32-63 */

/*************************  Bit definition for AESINHM register of HT_AES_TypeDef ***********************/
#define  AES_AESINHM                             ((uint32_t)0xffffffff)       /*!<AES�������ݵ�bit64-95 */

/*************************  Bit definition for AESINHH register of HT_AES_TypeDef ***********************/
#define  AES_AESINHH                             ((uint32_t)0xffffffff)       /*!<AES�������ݵ�bit96-127*/

/************************  Bit definition for AESOUTLL register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTLL                            ((uint32_t)0xffffffff)       /*!<AES������ݵ�bit0-31  */

/************************  Bit definition for AESOUTML register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTML                            ((uint32_t)0xffffffff)       /*!<AES������ݵ�bit32-63 */

/************************  Bit definition for AESOUTHM register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTHM                            ((uint32_t)0xffffffff)       /*!<AES������ݵ�bit64-95 */

/************************  Bit definition for AESOUTHH register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTHH                            ((uint32_t)0xffffffff)       /*!<AES������ݵ�bit96-127*/

/*************************  Bit definition for AESKEY0 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY0                             ((uint32_t)0xffffffff)       /*!<KEY Part0             */

/*************************  Bit definition for AESKEY1 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY1                             ((uint32_t)0xffffffff)       /*!<KEY Part1             */

/*************************  Bit definition for AESKEY2 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY2                             ((uint32_t)0xffffffff)       /*!<KEY Part2             */

/*************************  Bit definition for AESKEY3 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY3                             ((uint32_t)0xffffffff)       /*!<KEY Part3             */

/*************************  Bit definition for AESKEY4 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY4                             ((uint32_t)0xffffffff)       /*!<KEY Part4             */

/*************************  Bit definition for AESKEY5 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY5                             ((uint32_t)0xffffffff)       /*!<KEY Part5             */

/*************************  Bit definition for AESKEY6 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY6                             ((uint32_t)0xffffffff)       /*!<KEY Part6             */

/*************************  Bit definition for AESKEY7 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY7                             ((uint32_t)0xffffffff)       /*!<KEY Part7             */
#endif


/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of RAND Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT502x                                     /*!< HT6x2x               */
/************************  Bit definition for RANDSTR register of HT_RAND_TypeDef ***********************/
#define  RAND_RANDSTR_MODESEL                    ((uint32_t)0x0003)           /*!<�����ģʽѡ�����    */
#define  RAND_RANDSTR_MODESEL_SAMPLE             ((uint32_t)0x0000)           /*!<��Ƶ������Ƶ          */
#define  RAND_RANDSTR_MODESEL_DFF                ((uint32_t)0x0001)           /*!<D������               */
#define  RAND_RANDSTR_MODESEL_XOR                ((uint32_t)0x0002)           /*!<��Ƶ������ƵXORD������*/

#define  RAND_RANDSTR_START                      ((uint32_t)0x0004)           /*!<�������������λ      */
#define  RAND_RANDSTR_BUSY                       ((uint32_t)0x0008)           /*!<���������æ��־      */

#define  RAND_RANDSTR_BACKMODE                   ((uint32_t)0x0300)           /*!<���������ģʽ      */
#define  RAND_RANDSTR_BACKMODE_LFSR              ((uint32_t)0x0000)           /*!<LFSRģʽ              */
#define  RAND_RANDSTR_BACKMODE_PSEUDO            ((uint32_t)0x0100)           /*!<α�����Ϸ�ʽ        */
#define  RAND_RANDSTR_BACKMODE_ADD               ((uint32_t)0x0200)           /*!<�������ߺͷ�ʽ        */

#define  RAND_RANDSTR_BACKEN                     ((uint32_t)0x0400)           /*!<���������ʹ��      */

/************************  Bit definition for RANDDAT register of HT_RAND_TypeDef ***********************/
#define  RAND_RANDDAT                            ((uint32_t)0xffffffff)       /*!<��������ݼĴ���      */
#endif

/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of GHASH Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/**********************  Bit definition for GHASHSTR register of HT_GHASH_TypeDef ***********************/
#define  GHASH_GHASHSTR                          ((uint32_t)0x0501)           /*!<GHASH���������Ĵ���   */

/**********************  Bit definition for GHASHFLG register of HT_GHASH_TypeDef ***********************/
#define  GHASH_GHASHFLG_BUSY                     ((uint32_t)0x0001)           /*!<GHASH����æ��־       */

/**********************  Bit definition for INPUT1LL register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1LL                          ((uint32_t)0xffffffff)       /*!<����1 bit0-31         */

/**********************  Bit definition for INPUT1ML register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1ML                          ((uint32_t)0xffffffff)       /*!<����1 bit32-63        */

/**********************  Bit definition for INPUT1HM register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1HM                          ((uint32_t)0xffffffff)       /*!<����1 bit64-95        */

/**********************  Bit definition for INPUT1HH register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1HH                          ((uint32_t)0xffffffff)       /*!<����1 bit96-127       */

/**********************  Bit definition for INPUT2LL register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2LL                          ((uint32_t)0xffffffff)       /*!<����2 bit0-31         */

/**********************  Bit definition for INPUT2ML register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2ML                          ((uint32_t)0xffffffff)       /*!<����2 bit32-63        */

/**********************  Bit definition for INPUT2HM register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2HM                          ((uint32_t)0xffffffff)       /*!<����2 bit64-95        */

/**********************  Bit definition for INPUT2HH register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2HH                          ((uint32_t)0xffffffff)       /*!<����2 bit96-127       */

/**********************  Bit definition for OUTPUTLL register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTLL                          ((uint32_t)0xffffffff)       /*!<������� bit0-31      */

/**********************  Bit definition for OUTPUTML register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTML                          ((uint32_t)0xffffffff)       /*!<������� bit32-63     */

/**********************  Bit definition for OUTPUTHM register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTHM                          ((uint32_t)0xffffffff)       /*!<������� bit64-95     */

/**********************  Bit definition for OUTPUTHH register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTHH                          ((uint32_t)0xffffffff)       /*!<������� bit96-127    */

/*********************  Bit definition for AESGHASHIE register of HT_GHASH_TypeDef **********************/
#define  GHASH_AESGHASHIE_AESIE                  ((uint32_t)0x0001)           /*!<AES�����ж�ʹ��       */
#define  GHASH_AESGHASHIE_GHASHIE                ((uint32_t)0x0002)           /*!<GHASH�����ж�ʹ��     */
#define  GHASH_AESGHASHIE_RANDIE                 ((uint32_t)0x0004)           /*!<����������ж�ʹ��    */

/*********************  Bit definition for AESGHASHIF register of HT_GHASH_TypeDef **********************/
#define  GHASH_AESGHASHIF_AESIF                  ((uint32_t)0x0001)           /*!<AES�����жϱ�־       */
#define  GHASH_AESGHASHIF_GHASHIF                ((uint32_t)0x0002)           /*!<GHASH�����жϱ�־     */
#define  GHASH_AESGHASHIF_RANDIF                 ((uint32_t)0x0004)           /*!<����������жϱ�־    */
#endif


/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of KEY Module  
**********************************************************************************************************
*/
#if  defined  HT501x  ||  defined  HT502x
/************************  Bit definition for KEYIF register of HT_KEY_TypeDef ***********************/
#define  KEY_KEYIF                               ((uint32_t)0x0001)           /*!<������־�Ĵ���    */

/************************  Bit definition for KEYSTA register of HT_KEY_TypeDef ***********************/
#define	 KEY_KEYSTA								 ((uint32_t)0xffff)           /*!<����״̬�Ĵ���    */
#define	 KEY_KEYSTA_STA0						 ((uint32_t)0x0001<<0)        /*!<����״̬STA0    */
#define	 KEY_KEYSTA_STA1						 ((uint32_t)0x0001<<1)        /*!<����״̬STA1    */
#define	 KEY_KEYSTA_STA2						 ((uint32_t)0x0001<<2)        /*!<����״̬STA2    */
#define	 KEY_KEYSTA_STA3						 ((uint32_t)0x0001<<3)        /*!<����״̬STA3    */
#define	 KEY_KEYSTA_STA4						 ((uint32_t)0x0001<<4)        /*!<����״̬STA4    */
#define	 KEY_KEYSTA_STA5						 ((uint32_t)0x0001<<5)        /*!<����״̬STA5    */
#define	 KEY_KEYSTA_STA6						 ((uint32_t)0x0001<<6)        /*!<����״̬STA6    */
#define	 KEY_KEYSTA_STA7						 ((uint32_t)0x0001<<7)        /*!<����״̬STA7    */
#define	 KEY_KEYSTA_STA8						 ((uint32_t)0x0001<<8)        /*!<����״̬STA8    */
#define	 KEY_KEYSTA_STA9						 ((uint32_t)0x0001<<9)        /*!<����״̬STA9    */
#define	 KEY_KEYSTA_STA10						 ((uint32_t)0x0001<<10)       /*!<����״̬STA10    */
#define	 KEY_KEYSTA_STA11						 ((uint32_t)0x0001<<11)       /*!<����״̬STA11   */
#define	 KEY_KEYSTA_STA12						 ((uint32_t)0x0001<<12)       /*!<����״̬STA12   */
#define	 KEY_KEYSTA_STA13						 ((uint32_t)0x0001<<13)       /*!<����״̬STA13   */
#define	 KEY_KEYSTA_STA14						 ((uint32_t)0x0001<<14)       /*!<����״̬STA14   */
#define	 KEY_KEYSTA_STA15						 ((uint32_t)0x0001<<15)       /*!<����״̬STA15   */

#endif


/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of ECC Module  
**********************************************************************************************************
*/
#if  defined  HT502x
/************************  Bit definition for ECCCON register of HT_ECC_TypeDef ***********************/
#define	 ECC_ECCCON								 ((uint32_t)0x007f)           /*!<ECCģ����ƼĴ���       */

#define	 ECC_ECCCON_OP_STR	  			 ((uint32_t)0x0001)           /*!<��ʼ����                */

#define	 ECC_ECCCON_OP_SEL	  			 ((uint32_t)0x001e)           /*!<����ģʽѡ��λ          */
#define	 ECC_ECCCON_OP_SEL_ECSM	  	 ((uint32_t)0x0000)           /*!<�������                */
#define	 ECC_ECCCON_OP_SEL_ECA 	  	 ((uint32_t)0x0002)           /*!<�������                */
#define	 ECC_ECCCON_OP_SEL_ECD 	  	 ((uint32_t)0x0004)           /*!<��������                */
#define	 ECC_ECCCON_OP_SEL_MA  	  	 ((uint32_t)0x0006)           /*!<ģ������                */
#define	 ECC_ECCCON_OP_SEL_MS  	  	 ((uint32_t)0x0008)           /*!<ģ������                */
#define	 ECC_ECCCON_OP_SEL_MM  	  	 ((uint32_t)0x000a)           /*!<ģ������                */
#define	 ECC_ECCCON_OP_SEL_MD  	  	 ((uint32_t)0x000c)           /*!<ģ������                */
#define	 ECC_ECCCON_OP_SEL_MI  	  	 ((uint32_t)0x000e)           /*!<ģ������                */
#define	 ECC_ECCCON_OP_SEL_ECDSA_S	 ((uint32_t)0x0010)           /*!<ESDAǩ������            */
#define	 ECC_ECCCON_OP_SEL_ECDSA_V	 ((uint32_t)0x0012)           /*!<ESDA��֤����            */
#define	 ECC_ECCCON_OP_SEL_PKV    	 ((uint32_t)0x0014)           /*!<��Կ��������֤           */

#define	 ECC_ECCCON_ECCIE          	 ((uint32_t)0x0020)           /*!<ECC�ж�ʹ�ܿ���          */

#define	 ECC_ECCCON_SE            	 ((uint32_t)0x0040)           /*!<˽Կѡ�����λ           */
#define	 ECC_ECCCON_SE_SECRETKEY     ((uint32_t)0x0040)           /*!<��Secret Key��Ϊ˽Կ     */
#define	 ECC_ECCCON_SE_KEYREG        ((uint32_t)0x0000)           /*!<��KEYREG�Ĵ���ֵ��Ϊ˽Կ  */

/************************  Bit definition for ECCSTA register of HT_ECC_TypeDef ***********************/
#define	 ECC_ECCSTA								 ((uint32_t)0x001f)           /*!<ECCģ��״̬�Ĵ���       */

#define	 ECC_ECCSTA_ECCFLG	  			 ((uint32_t)0x0001)           /*!<ECC������ɱ�־λ          */
#define	 ECC_ECCSTA_BUSY  	  			 ((uint32_t)0x0002)           /*!<����ģ��״̬λ             */
#define	 ECC_ECCSTA_ECDSA_V	  			 ((uint32_t)0x0004)           /*!<ECDSA��֤��־             */
#define	 ECC_ECCSTA_ECDSA_S   			 ((uint32_t)0x0008)           /*!<ECDSAǩ�����Ա�־         */
#define	 ECC_ECCSTA_PKV    	  			 ((uint32_t)0x0010)           /*!<��Կ��������֤��־λ       */

/************************  Bit definition for PXREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_PXREG								 ((uint32_t)0xffffffff)       /*!<����X����Ĵ���         */

/************************  Bit definition for PYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_PYREG								 ((uint32_t)0xffffffff)       /*!<����Y����Ĵ���         */

/************************  Bit definition for KEYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_KEYREG								 ((uint32_t)0xffffffff)       /*!<˽Կ�Ĵ���              */

/************************  Bit definition for AREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_AREG 								 ((uint32_t)0xffffffff)       /*!<��Բ���߲���A�Ĵ���         */

/************************  Bit definition for PREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_PREG 								 ((uint32_t)0xffffffff)       /*!<������P�Ĵ���               */

/************************  Bit definition for RXREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_RXREG								 ((uint32_t)0xffffffff)       /*!<ECC���������X�����������       */

/************************  Bit definition for RYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_RYREG								 ((uint32_t)0xffffffff)       /*!<ECC���������Y�����������        */

/************************  Bit definition for SXREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_SXREG								 ((uint32_t)0xffffffff)       /*!<ECC����ڶ�������X����           */

/************************  Bit definition for SYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_SYREG								 ((uint32_t)0xffffffff)       /*!<ECC���������Y�����������        */

/************************  Bit definition for MYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_MYREG								 ((uint32_t)0xffffffff)       /*!<HASH�㷨��ϢժҪ                 */



#endif

#ifdef __cplusplus
}
#endif

#endif  /* __HT6XXX_H__ */
