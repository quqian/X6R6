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
* File         : HT6xxx_cmu.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_CMU_C

#include "ht6xxx_cmu.h"

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

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/

static void Wait2mS(void);

/*
*********************************************************************************************************
*                                         INITIAL CMU MODULE
*
* ����˵��: ��ʼ��ʱ������ģ��
*
* ��ڲ���: CMU_InitStruct     CMUģ���ʼ���ṹ��ָ�룬��Ҫ����3������: 
*                              1) SysClkSel   : ϵͳʱ�ӣ�Fsys��Դѡ�񣬿�Ϊ����һ��������
*                                               a) SysFLRC    LRC--->Fsys
*                                               b) SysFLF     LF---->Fsys
*                                               c) SysFHRC    HRC--->Fsys
*                                               d) SysFPLL    PLL--->Fsys
*                                               e) SysFMEMS   Mems-->Fsys                                               
*                              2) HRCDiv      : ��ƵRC��Ƶ����� SysClkSel = FHRC����Fsys = HRC/HRCDiv
*                              3) CPUDiv      : CPUʱ�ӷ�Ƶ����  Fcpu = Fsys/CPUDiv                            
*
* ���ز���: ��                                      
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
void HT_CMU_Init(CMU_InitTypeDef* CMU_InitStruct)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
#if  defined  HT501x  || HT6x2x  ||  HT502x   
        if((CMU_InitStruct->SysClkSel & CMU_SYSCLKDIV_SEL40M)==0)          /*!< ѡ��PLLʱ��20M         */ 
        {
            CMU_InitStruct->CPUDiv &= ~CMU_SYSCLKDIV_SEL40M;
        }
        else                                                               /*!< ����ѡ��PLLʱ��X2 40M  */
        {
            CMU_InitStruct->CPUDiv |= CMU_SYSCLKDIV_SEL40M;
        }
#endif	
		HT_CMU->SYSCLKDIV = CMU_InitStruct->CPUDiv;                            /*!< CPUʱ�ӷ�Ƶ����        */
    
    if((CMU_InitStruct->SysClkSel & CMU_SYSCLKCFG_CLKSEL) == SysPLL)       /*!< Fpll ---> Fsys         */
    {
        HT_CMU->CLKCTRL0 |= CMU_CLKCTRL0_PLLEN;                            /*!< PLLʹ��                */
        Wait2mS();                                                         /*!< �ȴ�2ms                */
        HT_CMU->SYSCLKCFG = (CMU_SYSCLKCFG_CLKSEL_PLL | CMU_SYSCLKCFG_WCLKEN);
                                                                           /*!< ����ϵͳΪPLLʱ��      */  
    }
    else if((CMU_InitStruct->SysClkSel & CMU_SYSCLKCFG_CLKSEL) == CMU_SYSCLKCFG_CLKSEL_HRC)                       
    {                                                                      /*!< Fhrc ---> Fsys         */
        HT_CMU->CLKCTRL0 |= CMU_CLKCTRL0_HRCEN;                            /*!< HRCʹ��                */
        HT_CMU->HRCDIV    = CMU_InitStruct->SysClkSel>>4;                  /*!< HRCʱ�ӷ�Ƶ����        */  
        Wait2mS();                                                         /*!< �ȴ�2ms                */
        HT_CMU->SYSCLKCFG = (CMU_SYSCLKCFG_CLKSEL_HRC | CMU_SYSCLKCFG_WCLKEN);   
                                                                           /*!< ����ϵͳΪHRCʱ��      */      
    }
    else                                                                   /*!< Fsys = FLrc/FLf/Fmems  */
    {
        HT_CMU->SYSCLKCFG = (CMU_InitStruct->SysClkSel | CMU_SYSCLKCFG_WCLKEN);   
                                                                           /*!< ����ϵͳΪ�趨ʱ��     */
    }
      
    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
} 

/*
*********************************************************************************************************
*                                     GET FSYS CLOCK FREQUENCY
*
* ����˵��: ��ȡϵͳʱ��Ƶ�ʣ���Fsys����Fcpu����������HzΪ��λ��Ƶ��ֵ
*
* ��ڲ���: ��                         
*
* ���ز���: sysclk   ϵͳʱ�ӣ�Fsys��Ƶ�ʣ�32λ�޷���������HzΪ��λ                                   
* 
* ����˵��: ���ϵͳʱ��Դ�Ǹ�ƵRC���ߵ�ƵRC�����ص�ʱ��Ƶ�ʽ�������ȫ׼ȷ���Ը�ƵRC������2%���ҵ���
*           �Ե�ƵRC�����г���10%����
*********************************************************************************************************
*/
uint32_t HT_CMU_SysClkGet(void)
{
    /*  assert_param  */
      
    uint32_t sysclk, clksel;       
    
    clksel = HT_CMU->SYSCLKCFG & CMU_SYSCLKCFG_CLKSEL;
    
    switch(clksel)
    {
        case CMU_SYSCLKCFG_CLKSEL_LRC:               /*!< ϵͳʱ��ԴΪLRC     */
        case CMU_SYSCLKCFG_CLKSEL_LF:                /*!< ϵͳʱ��ԴΪLF      */
             sysclk = 32768;
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_HRC:               /*!< ϵͳʱ��ԴΪHRC     */
#if defined  HT6x1x
				sysclk = 8000000>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT6x2x
				sysclk = 11010048>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT501x
				sysclk = 14000000;
#elif defined  HT502x
				sysclk = 8800000;
#endif
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_PLL:               /*!< ϵͳʱ��ԴΪPLL     */
#if  defined  HT501x || HT502x
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 sysclk =  32768*600*2;  
             }
             else
             {
                 sysclk =  32768*600;
             }
#elif defined HT6x2x
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 sysclk =  32768*672*2;  
             }
             else
             {
                 sysclk =  32768*672;
             }
#else        
             sysclk = 32768*672;
#endif        
        break;
        
        default:                                     /*!< ϵͳʱ��ԴΪMems    */
             sysclk = 524288;                        /*!< HT501x ������       */
        break;
    }
    
    return sysclk;                                   /*!< ����ϵͳʱ��Ƶ��    */
} 

/*
*********************************************************************************************************
*                                     GET CPU CLOCK FREQUENCY
*
* ����˵��: ��ȡCPUʱ��Ƶ�ʣ�������HzΪ��λ��Ƶ��ֵ
*
* ��ڲ���: ��                         
*
* ���ز���: cpuclk   CPUʱ�ӣ�Fcpu��Ƶ�ʣ�32λ�޷���������HzΪ��λ                                   
* 
* ����˵��: ���ϵͳʱ��Դ�Ǹ�ƵRC���ߵ�ƵRC�����ص�ʱ��Ƶ�ʽ�������ȫ׼ȷ���Ը�ƵRC������2%���ҵ���
*           �Ե�ƵRC�����г���10%����
*********************************************************************************************************
*/
uint32_t HT_CMU_CPUClkGet(void)
{
    /*  assert_param  */
      
    uint32_t cpuclk, clksel, cpuclkdiv;       
    
    clksel = HT_CMU->SYSCLKCFG & CMU_SYSCLKCFG_CLKSEL;
	  cpuclkdiv = HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV;          /*!< ��ȡCPUʱ�ӷ�Ƶ    */
    
    switch(clksel)
    {
        case CMU_SYSCLKCFG_CLKSEL_LRC:               /*!< ϵͳʱ��ԴΪLRC     */
        case CMU_SYSCLKCFG_CLKSEL_LF:                /*!< ϵͳʱ��ԴΪLF      */
             cpuclk = 32768;
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_HRC:               /*!< ϵͳʱ��ԴΪHRC     */
#if defined  HT6x1x
				cpuclk = 8000000>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT6x2x
				cpuclk = 11010048>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT501x
				cpuclk = 14000000;
#elif defined  HT502x
				cpuclk = 8800000;
#endif
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_PLL:               /*!< ϵͳʱ��ԴΪPLL     */
#if  defined  HT501x  ||  HT502x 
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 cpuclk =  32768*600*2;  
             }
             else
             {
                 cpuclk =  32768*600;
             }
#elif defined HT6x2x
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 cpuclk =  32768*672*2;  
             }
             else
             {
                 cpuclk =  32768*672;
             }
#else        
             cpuclk = 32768*672;
#endif
        break;
        
        default:                                     /*!< ϵͳʱ��ԴΪMems    */
             cpuclk = 524288;         							 /*!< HT501x ������       */
        break;
    }
    cpuclk = cpuclk >> cpuclkdiv;         					/*!< ����CPUʱ�ӷ�Ƶ    */
    return cpuclk;                                   /*!< ����ϵͳʱ��Ƶ��    */
} 

/*
*********************************************************************************************************
*                                  SET CLKOUT TO SPECIFIC FREQUENCY 
*
* ����˵��: ����Clkout���Ƶ��
*
* ��ڲ���: Clkout_InitTypeDef Clkoutģ���ʼ���ṹ��ָ�룬��Ҫ����2������: 
*                              1) ClkoutSel     Clkoutʱ��Դѡ�񣬿�Ϊ����һ��������
*                                               a) CMU_CLKOUTSEL_LRC    LRC--->Clkout
*                                               b) CMU_CLKOUTSEL_LF     LF---->Clkout
*                                               c) CMU_CLKOUTSEL_HRC    HRC--->Clkout
*                                               d) CMU_CLKOUTSEL_SYS    SYS--->Clkout
*                                               e) CMU_CLKOUTSEL_MEMS   Mems-->Clkout                                               
*                              2) ClkoutDiv     Clkout���ʱ�ӷ�Ƶ���ã���Ƶϵ��Ϊ��2*(ClkoutDiv[3..0]+1)
*                                               
* ���ز���: ��                                 
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
void HT_CMU_ClkoutSet(Clkout_InitTypeDef* Clkout_InitStruct)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
    
    HT_CMU->CLKOUTSEL = Clkout_InitStruct->ClkoutSel;                      /*!< ����Clkout���ʱ��Դ   */
    
    HT_CMU->CLKOUTDIV = Clkout_InitStruct->ClkoutDiv & CMU_CLKOUTDIV;      /*!< Clkout���ʱ�ӷ�Ƶ���� */
    
    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
} 

/*
*********************************************************************************************************
*                                  CONFIGUE CLKCTRL0 REGISTER 
*
* ����˵��: ����ClkCtrl0�Ĵ�����ʹ�ܻ��߹ر�
*
* ��ڲ���: ClkCtrl0Module     ClkCtrl0�Ĵ���ģ�����λ������Ϊ���²����������: 
*                               @arg  CMU_CLKCTRL0         �������п���λ
*                               @arg  CMU_CLKCTRL0_LCDEN     
*                               @arg  CMU_CLKCTRL0_SPIEN     
*                               @arg  CMU_CLKCTRL0_I2CEN     
*                               @arg  CMU_CLKCTRL0_PLLEN     
*                               @arg  CMU_CLKCTRL0_HRCEN     
*                               @arg  CMU_CLKCTRL0_PLLLOCKEN 
*                               @arg  CMU_CLKCTRL0_LFDETEN   
*                               @arg  CMU_CLKCTRL0_PLLDETEN  
*                               @arg  CMU_CLKCTRL0_HRCDETEN  
*                               @arg  CMU_CLKCTRL0_WDTEN     
*                               @arg  CMU_CLKCTRL0_CLKOUTEN  
*                               @arg  CMU_CLKCTRL0_1P5LBOREN 
*                               @arg  CMU_CLKCTRL0_3DESRADEN (Only for HT6x1x)
*                               @arg  CMU_CLKCTRL0_ADESRADEN (Only for HT501x, HT6x2x, HT502x)
*                               @arg  CMU_CLKCTRL0_CRCEN     (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL0_EMUEN     (Only for HT501x, HT502x)
*                               @arg  CMU_CLKCTRL0_ECCEN     (Only for HT502x)
*
*           NewState           = ENABLE�� ʹ����Ӧģ��
*                              = DISABLE���ر���Ӧģ��        
*                                               
* ���ز���: ��                                 
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
void HT_CMU_ClkCtrl0Config(uint32_t ClkCtrl0Module, FunctionalState NewState)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
    
    ClkCtrl0Module &= CMU_CLKCTRL0;
    
    if (NewState != DISABLE)
    {       
        HT_CMU->CLKCTRL0 |= ClkCtrl0Module;                                /*!< ʹ��Ctrl0����Ӧģ��    */ 
    }
    else
    {
#if defined HT6x1x	
        ClkCtrl0Module &= ~CMU_CLKCTRL0_Reserved;                          /*!< Bit6 ����Ϊ1           */
#endif
        HT_CMU->CLKCTRL0 &= ~ClkCtrl0Module;                               /*!< �ر�Ctrl0����Ӧģ��    */ 
    }     
     
    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
} 

/*
*********************************************************************************************************
*                                  CONFIGUE CLKCTRL0 REGISTER 
*
* ����˵��: ����ClkCtrl1�Ĵ�����ʹ�ܻ��߹ر�
*
* ��ڲ���: ClkCtrl1Module     ClkCtrl1�Ĵ���ģ�����λ������Ϊ���²����������:
*                               @arg  CMU_CLKCTRL1            �������п���λ
*                               @arg  CMU_CLKCTRL1_TMR0EN      
*                               @arg  CMU_CLKCTRL1_TMR1EN      
*                               @arg  CMU_CLKCTRL1_TMR2EN      
*                               @arg  CMU_CLKCTRL1_TMR3EN      
*                               @arg  CMU_CLKCTRL1_UART0EN     
*                               @arg  CMU_CLKCTRL1_UART1EN     
*                               @arg  CMU_CLKCTRL1_UART2EN     
*                               @arg  CMU_CLKCTRL1_UART37816EN 
*                               @arg  CMU_CLKCTRL1_UART47816EN 
*                               @arg  CMU_CLKCTRL1_UART5EN  
*                               @arg  CMU_CLKCTRL1_TMR4EN         (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_TMR5EN         (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_UART6EN        (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_SPI1EN         (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_SOFTWDTEN      (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_LRCRTCEN       (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_LRCTFREQEN     (Only for HT501x, HT502x)
*                               @arg  CMU_CLKCTRL1_LRCRTCEN       (Only for HT501x, HT502x)
*                               @arg  CMU_CLKCTRL1_AUTORCCAL_EN   (Only for HT6x2x)
*
*           NewState           = ENABLE�� ʹ����Ӧģ��
*                              = DISABLE���ر���Ӧģ��        
*                                               
* ���ز���: ��                                 
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
void HT_CMU_ClkCtrl1Config(uint32_t ClkCtrl1Module, FunctionalState NewState)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
      
    ClkCtrl1Module &= CMU_CLKCTRL1;
    
    if (NewState != DISABLE)
    {       
        HT_CMU->CLKCTRL1 |= ClkCtrl1Module;                                /*!< ʹ��Ctrl1����Ӧģ��    */ 
    }
    else
    {
        HT_CMU->CLKCTRL1 &= ~ClkCtrl1Module;                               /*!< �ر�Ctrl1����Ӧģ��    */ 
    }     
     
    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
} 

/*
*********************************************************************************************************
*                                      GET JTAG STATUS
*
* ����˵��: ��ȡJTAG״̬
*
* ��ڲ���: ��
*
* ���ز���: Bool    = TRUE�� оƬ���ڵ���״̬
*                   = FALSE��оƬ��������״̬
* 
* ����˵��: ��
*********************************************************************************************************
*/
Bool HT_CMU_JTAGStatusGet()
{
    /*  assert_param  */
    
    return (HT_CMU->JTAGSTA ? TRUE : FALSE);
 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED CLOCK STOP FLAG
*
* ����˵��: ��ȡ��Ӧʱ��ͣ���־
*
* ��ڲ���: STAFlag     ��Ҫ����ĳ��ʱ��ͣ���־������Ϊ���²���:
*                        @arg CMU_CLKSTA_LFFLAG
*                        @arg CMU_CLKSTA_HRCFLAG
*                        @arg CMU_CLKSTA_PLLFLAG
*						 @arg CMU_CLKSTA_PLLLOCK
*
* ���ز���: FlagStatus  = SET��  ��Ӧʱ�ӷ���ͣ��
*                       = RESET����Ӧʱ��δͣ��
* 
* ����˵��: �û�Ӧ��ʱ��ͣ����ʹ��λ���ܵõ���Ч�ı�־���μ�HT_CMU_ClkCtrl0Config()
*********************************************************************************************************
*/
FlagStatus HT_CMU_StopFlagGet(uint8_t STAFlag)
{
    /*  assert_param  */
    
    if (HT_CMU->CLKSTA & STAFlag)
    {       
        return SET;                        /*!< Clock Stop Flag is set   */
    }
    else
    {
        return RESET;                      /*!< Clock Stop Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                            Flash delay set
*
* ����˵��: ����flash_dly
*
* ��ڲ���: STAFlag     ��Ҫ����ĳ��ʱ��ͣ���־������Ϊ���²���:
*          NewState           = ENABLE�� ʹ����Ӧģ��
*                             = DISABLE���ر���Ӧģ��  
*********************************************************************************************************
*/

void	HT_CMU_FlashDly_Set(FunctionalState NewState)
{
#if  defined  HT501x	||  HT502x
		uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
    
    if (NewState != DISABLE)
    {       
        HT_CMU->FLASHDLY = CMU_FLASHDLY_ENABLE;                           /*!< ʹ��FLASH_DLY          */ 
    }
    else
    {
        HT_CMU->FLASHDLY = CMU_FLASHDLY_DISABLE;                          /*!< �ر�Ctrl1����Ӧģ��    */ 
    }  

    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
#endif
}
/*
*********************************************************************************************************
*                            Flash prefetch set
*
* ����˵��: ����prefetch
*
* ��ڲ���: STAFlag     ��Ҫ����ĳ��ʱ��ͣ���־������Ϊ���²���:
*          NewState           = ENABLE�� ʹ����Ӧģ��
*                             = DISABLE���ر���Ӧģ��  
*********************************************************************************************************
*/
void	HT_CMU_Prefetch_Set(FunctionalState NewState)
{
#if  defined  HT6x2x	
		uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
    
    if (NewState != DISABLE)
    {       
        HT_CMU->PREFETCH = CMU_PREFETCH_ENABLE;                           /*!< ʹ��prefetch         */ 
    }
    else
    {
        HT_CMU->PREFETCH = CMU_PREFETCH_DISABLE;                          /*!< �ر�prefetch    */ 
    }  

    HT_CMU->WPREG = writeProtect;                                          /*!< �ָ�֮ǰд��������     */
#endif
}
/*
*********************************************************************************************************
*                                     Wait For 2 MILLISECOND
*
* ����˵��: �ȴ�2ms���ӳ٣�ģ���ڲ�������
*
* ��ڲ���: ��                         
*
* ���ز���: ��                                   
* 
* ����˵��: �ӳ�ʱ��ֻ�Ǵ����ȷ��ע��Ŀǰû�е��ԣ��ӳ�ʱ�仹δȷ����
*********************************************************************************************************
*/
void Wait2mS(void)
{
    
    uint32_t delay;
    
    delay = HT_CMU_SysClkGet();                             /*!< ��ȡ��ǰϵͳʱ��Fsys     */
    delay = delay>>(HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV );    /*!< ��ȡ��ǰCPUʱ��Fcpu      */
    
    delay = delay>>10;                                      /*!< 500 X 2                  */
    
    while(delay--);                                         /*!< �ȴ�2ms                  */       
}

