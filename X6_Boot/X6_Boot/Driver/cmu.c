/*cmu.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#include "include.h"



static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

//******************************************************************
//! \brief  	Fsys �л���Flf��32768Hz��
//! \param  	none
//! \retval 	
//! \note   
//! \note  
//******************************************************************
void SwitchTo_Flf(void)
{
	uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
	
	//EnWr_WPREG();				
	HT_CMU->SYSCLKCFG = 0x0081;				//Fsys = Flf
	HT_CMU->SYSCLKDIV = 0x0000;				//Fcpu = Fsys
	//DisWr_WPREG();
	
	HT_CMU->WPREG = writeProtect;					/*!< �ָ�֮ǰд��������     */
}

//******************************************************************
//! \brief  	Fsys�л����ڲ�����ʱ�ӣ�11MHz��
//! \param  	none
//! \retval 	
//! \note   
//! \note  
//******************************************************************
void SwitchTo_Fhrc(void)
{
	uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
	
	//EnWr_WPREG();
	HT_CMU->CLKCTRL0 |= 0x0020;			//ʹ��HRC
	HT_CMU->SYSCLKCFG = 0x0082;			//Fsys = Fhrc
	HT_CMU->SYSCLKDIV = 0x0000;			//Fcpu  = Fsys	
	//DisWr_WPREG();
	
	HT_CMU->WPREG = writeProtect;					/*!< �ָ�֮ǰд��������     */
}

//******************************************************************
//! \brief  	Fsys�л����ڲ�PLLʱ�ӣ�22MHz��
//! \param  	none
//! \retval 	
//! \note   
//! \note  
//******************************************************************
void SwitchTo_Fpll(void)
{
    
    HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL0 |= 0x0010;            //enable PLL
    
    delay_ms(100);
   // while((HT_CMU->CLKSTA & 0x20) == 0);
    
	HT_CMU->SYSCLKCFG = 0x83;              //PLL as Fsys 
	HT_CMU->SYSCLKDIV = 0x00;			   //PLL == Fcpu == 19.66
    

}

//******************************************************************
//! \brief  	Close_Pll
//! \param  	none
//! \retval 	
//! \note   
//! \note  
//******************************************************************
void Close_Pll(void)
{
	uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
	
	//EnWr_WPREG();
	HT_CMU->CLKCTRL0 &= 0xFFEF;            //close HRC
	//DisWr_WPREG();
	
	HT_CMU->WPREG = writeProtect;					/*!< �ָ�֮ǰд��������     */
}

//******************************************************************
//! \brief  	Close_Hrc
//! \param  	none
//! \retval 	
//! \note   
//! \note  
//******************************************************************
void Close_Hrc(void)
{
	uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< ���浱ǰд����״̬     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< �ر�д��������         */
	
	//EnWr_WPREG();
	HT_CMU->CLKCTRL0 &= 0xFFDF;            //close HRC
	//DisWr_WPREG();
	
	HT_CMU->WPREG = writeProtect;					/*!< �ָ�֮ǰд��������     */
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
*                               @arg  CMU_CLKCTRL0_SPI0EN     
*                               @arg  CMU_CLKCTRL0_I2CEN     
*                               @arg  CMU_CLKCTRL0_PLLEN     
*                               @arg  CMU_CLKCTRL0_HRCEN     
*                               @arg  CMU_CLKCTRL0_PLLLOCKEN 
*                               @arg  CMU_CLKCTRL0_LFDETEN   
*                               @arg  CMU_CLKCTRL0_PLLDETEN  
*                               @arg  CMU_CLKCTRL0_HRCDETEN    
*                               @arg  CMU_CLKCTRL0_CLKOUTEN  
*                               @arg  CMU_CLKCTRL0_1P5LBOREN 
*
*           NewState           = ENABLE�� ʹ����Ӧģ��
*                              = DISABLE���ر���Ӧģ��        
*                                               
* ���ز���: ��                                 
* 
* ����˵��: �û�Ӧ��֤�˺���ִ�й����мĴ���д����״̬�����޸�
*********************************************************************************************************
*/
void HT_CMU_ClkCtrl0Config(uint32_t ClkCtrl0Module, uint8_t NewState)
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
*                               @arg  CMU_CLKCTRL1_TMR4EN
*                               @arg  CMU_CLKCTRL1_TMR5EN
*                               @arg  CMU_CLKCTRL1_UART6EN
*                               @arg  CMU_CLKCTRL1_SPI1EN
*                               @arg  CMU_CLKCTRL1_SOFTWDTEN
*                               @arg  CMU_CLKCTRL1_LRCRTCEN
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












