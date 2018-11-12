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
* File         : ht6xxx_rtc.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_RTC_C

#include "ht6xxx_rtc.h"

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


/*
*********************************************************************************************************
*                                         INITIAL RTC MODULE
*
* ����˵��: ��ʼ��RTCģ��
*
* ��ڲ���: RTC_InitStruct     RTC��ʼ���ṹ��ָ�룬��Ҫ����4������: 
*                              1) RTC_TMR1    : RTC��ʱ��1��ʼ���ṹ����Ҫ����ʹ���붨ʱ��������������
*                              2) RTC_TMR2    : RTC��ʱ��2��ʼ���ṹ����Ҫ����ʹ���붨ʱ��������������
*                              3) RTC_Alarm   : RTC�������� 
*                              4) RTC_TOUT    : RTCTout�������                         
*
* ���ز���: ��                                      
* 
* ����˵��: �ж�������ֱ�Ӹ�ֵ��δ�����õ�λ�ᱻֱ��дΪ0
*********************************************************************************************************
*/
void HT_RTC_Init(RTC_InitTypeDef* RTC_InitStruct)
{
    /*  assert_param  */
    

    if(RTC_InitStruct->RTC_TMR1.NewState != DISABLE)                 /*!< ����RTC��ʱ��1         */
    {
        HT_RTC->RTCTMR1 = (uint32_t)RTC_InitStruct->RTC_TMR1.Count;  /*!< ����RTC��ʱ��1��ʱ���� */    
        HT_RTC->RTCCON |= RTC_RTCCON_RTC1EN;                         /*!< ʹ��RTC��ʱ��1         */     
    }
    else
    {
        HT_RTC->RTCCON &= ~RTC_RTCCON_RTC1EN;                        /*!< �ر�RTC��ʱ��1         */
    }   

    if(RTC_InitStruct->RTC_TMR2.NewState != DISABLE)                 /*!< ����RTC��ʱ��2         */
    {
        HT_RTC->RTCTMR2 = (uint32_t)RTC_InitStruct->RTC_TMR2.Count;  /*!< ����RTC��ʱ��2��ʱ���� */    
        HT_RTC->RTCCON |= RTC_RTCCON_RTC2EN;                         /*!< ʹ��RTC��ʱ��2         */     
    }
    else
    {
        HT_RTC->RTCCON &= ~RTC_RTCCON_RTC2EN;                        /*!< �ر�RTC��ʱ��2         */
    }   
                                                                     /*!< ��������Ĵ���         */
    HT_RTC->ALMR = (uint32_t)(RTC_InitStruct->RTC_Alarm.HourAndMinute & RTC_ALMR);
    
    HT_RTC->RTCCON &= ~(uint32_t)RTC_RTCCON_TOUT;
    HT_RTC->RTCCON |= (uint32_t)RTC_InitStruct->RTC_TOUT;            /*!< ����TOUT���           */   
} 

/*
*********************************************************************************************************
*                                         READ RTC TIME
*
* ����˵��: ��RTCʱ��Ĵ�����������/��/��/��/ʱ/��/�빲7���Ĵ���
*
* ��ڲ���: pRTCTime    void*�ͱ�������ָ��������ڴ洢RTCʱ�䣬�ڳ�����ʵ�ʱ�����uint8_t*������
*                       �洢ѭ�����£�
*                       pRTCTime[0]: WEEKR
*                       pRTCTime[1]: YEARR
*                       pRTCTime[2]: MONTHR
*                       pRTCTime[3]: DAYR
*                       pRTCTime[4]: HOURR
*                       pRTCTime[5]: MINR
*                       pRTCTime[6]: SECR
*
* ���ز���: ��                                      
* 
* ����˵��: �û�Ӧ���������ڶ��Ĺ�����ȥд��Щ�Ĵ���
*********************************************************************************************************
*/
void HT_RTC_Read(void* pRTCTime)
{
    /*  assert_param  */
    
    uint8_t *pRTCRead = pRTCTime;
    
    HT_RTC->RTCRD = RTC_RTCRD_READFLAG;                /*!< ���ÿ�ʼ����־        */
    while(HT_RTC->RTCRD & RTC_RTCRD_READFLAG);         /*!< �ȴ����Զ�            */
                                                       /*!< ��ʼ��                */
    pRTCRead[0] = HT_RTC->WEEKR & RTC_WEEKR;      
    pRTCRead[1] = HT_RTC->YEARR & RTC_YEARR;
    pRTCRead[2] = HT_RTC->MONTHR& RTC_MONTHR;
    pRTCRead[3] = HT_RTC->DAYR  & RTC_DAYR;
    pRTCRead[4] = HT_RTC->HOURR & RTC_HOURR;
    pRTCRead[5] = HT_RTC->MINR  & RTC_MINR;
    pRTCRead[6] = HT_RTC->SECR  & RTC_SECR;

} 

/*
*********************************************************************************************************
*                                         WRITE RTC TIME     
*
* ����˵��: дRTCʱ��Ĵ�����������/��/��/��/ʱ/��/�빲7���Ĵ���
*
* ��ڲ���: pRTCTime    void*�ͱ�������ָ��������ڴ洢RTCʱ�䣬�ڳ�����ʵ�ʱ�����uint8_t*������
*                       �洢ѭ�����£�
*                       pRTCTime[0]: WEEKR
*                       pRTCTime[1]: YEARR
*                       pRTCTime[2]: MONTHR
*                       pRTCTime[3]: DAYR
*                       pRTCTime[4]: HOURR
*                       pRTCTime[5]: MINR
*                       pRTCTime[6]: SECR
*
* ���ز���: ��                                      
* 
* ����˵��: 1) ����һ�θ������м�ʱ�Ĵ���������û�����²��ּĴ������û����ô˺���ǰ���ȶ������м�ʱ�Ĵ�����
*              Ȼ�������Ҫ���µļĴ��������һ��д�롣
*           2) �û�Ӧ������д�Ĺ����ж���Щ�Ĵ���
*           3) �û�Ӧ��֤д�����ݵ���Ч��
*********************************************************************************************************
*/
void HT_RTC_Write(void* pRTCTime)
{
    /*  assert_param  */
    
    uint8_t *pRTCWrite = pRTCTime;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    
                                                       /*!< ���浱ǰд����״̬    */ 
    HT_CMU->WPREG = CMU_WPREG_UnProtected;             /*!< �ر�д��������        */
    
    HT_RTC->RTCWR = RTC_RTCWR_CLEAR;                   /*!< ���д��־            */
                                                       /*!< ��ʼд                */
    HT_RTC->WEEKR  = pRTCWrite[0] & RTC_WEEKR;      
    HT_RTC->YEARR  = pRTCWrite[1] & RTC_YEARR;
    HT_RTC->MONTHR = pRTCWrite[2] & RTC_MONTHR;
    HT_RTC->DAYR   = pRTCWrite[3] & RTC_DAYR;
    HT_RTC->HOURR  = pRTCWrite[4] & RTC_HOURR;
    HT_RTC->MINR   = pRTCWrite[5] & RTC_MINR;
    HT_RTC->SECR   = pRTCWrite[6] & RTC_SECR;
    
    HT_RTC->RTCWR = RTC_RTCWR_UPDATE;                  /*!< д���                */
    
    HT_CMU->WPREG = writeProtect;                      /*!< �ָ�֮ǰд��������    */

} 

/*
*********************************************************************************************************
*                                    RTC TIMER CONFIGURE     
*
* ����˵��: ����RTC��ʱ��1���߶�ʱ��2��ʹ��״̬�Լ���ʱ����
*
* ��ڲ���: RTCTMRx    ѡ��RTC��ʱ��1����RTC��ʱ��2
*
*           RTCTMR_InitStruct      RTC��ʱ����ʼ���ṹ��ָ�룬��Ҫ������������������
*                                  a) SetState  : ���ö�ʱ��ʹ�ܻ��߹ر�
*                                  b) Count     : ���ö�ʱ����ʱ����                                   
*                                                 ��RTC��ʱ��1��ÿһ��Count��ʾ1s
*                                                 ��RTC��ʱ��2��ÿһ��Count��ʾ0.0625s
*
* ���ز���: ��                                      
* 
* ����˵��: ʵ�ʶ�ʱ����Ϊ(1+COUNT)������1-65536֮��
*********************************************************************************************************
*/
void HT_RTC_TMRConfig(RTCTMR_SelectTypeDef RTCTMRx, RTCTMR_InitTypeDef* RTCTMR_InitStruct)
{
    /*  assert_param  */
                           
    
    if(RTCTMR_InitStruct->NewState != DISABLE)
    {
        if (RTCTMRx == RTCTMR1)
            HT_RTC->RTCTMR1 = (uint32_t)RTCTMR_InitStruct->Count;  /*!< ����RTC��ʱ��1��ʱ����  */
        else
            HT_RTC->RTCTMR2 = (uint32_t)RTCTMR_InitStruct->Count;  /*!< ����RTC��ʱ��2��ʱ����  */
        
        HT_RTC->RTCCON |= (uint32_t)RTCTMRx;                       /*!< ʹ��RTC��ʱ��           */
            
    }
    else
    {
            HT_RTC->RTCCON &= ~(uint32_t)RTCTMRx;                  /*!< �ر�RTC��ʱ��           */
    }

}

/*
*********************************************************************************************************
*                                       SET TOUT    
*
* ����˵��: ����Tout����ض��ź�
*
* ��ڲ���: RTCToutSet   Tout������ò���
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_RTC_ToutSet(RTCTout_TypeDef RTCToutSet)
{
    /*  assert_param  */
    
    HT_RTC->RTCCON &= ~(uint32_t)RTC_RTCCON_TOUT;
    HT_RTC->RTCCON |= (uint32_t)RTCToutSet; 
}

/*
*********************************************************************************************************
*                                        SET ALARM    
*
* ����˵��: ��������
*
* ��ڲ���: RTCAlarmSet  �洢���������ʱ���
*
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_RTC_AlarmSet(RTCAlarm_TypeDef RTCAlarmSet)
{
    /*  assert_param  */
    
    HT_RTC->ALMR = (uint32_t)(RTCAlarmSet.HourAndMinute & RTC_ALMR);
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE RTC INTERRUPT    
*
* ����˵��: ʹ�ܻ��߹ر�RTC�ж�
*
* ��ڲ���: ITEn       RTC�ж�����λ������Ϊ���²����������
*                        @arg RTC_RTCIE_SECIE
*                        @arg RTC_RTCIE_MINIE
*                        @arg RTC_RTCIE_HRIE
*                        @arg RTC_RTCIE_DAYIE
*                        @arg RTC_RTCIE_MTHIE
*                        @arg RTC_RTCIE_RTC1IE
*                        @arg RTC_RTCIE_RTC2IE
*                        @arg RTC_RTCIE_ALMIE
*
*           NewState   = ENABLE�� ʹ���ж�
*                      = DISABLE���ر��ж�
* ���ز���: ��                                      
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_RTC_ITConfig(uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_RTC->RTCIE |= (uint32_t)ITEn;            /*!< ʹ��RTC�ж�           */
    }
    else
    {
        HT_RTC->RTCIE &= ~(uint32_t)ITEn;           /*!< �ر�RTC�ж�           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED RTC INTERRUPT FLAG STATUS
*
* ����˵��: ��ȡ��ӦRTC�жϱ�־״̬
*
* ��ڲ���: ITFlag     ��Ҫ����ĳ��RTC�жϣ�����Ϊ���²���:
*                        @arg RTC_RTCIF_SECIF
*                        @arg RTC_RTCIF_MINIF
*                        @arg RTC_RTCIF_HRIF
*                        @arg RTC_RTCIF_DAYIF
*                        @arg RTC_RTCIF_MTHIF
*                        @arg RTC_RTCIF_RTC1IF
*                        @arg RTC_RTCIF_RTC2IF
*                        @arg RTC_RTCIF_ALMIF
*
* ���ز���: ITStatus    = SET��  ��Ӧ�жϱ�־����
*                       = RESET����Ӧ�жϱ�־δ����
* 
* ����˵��: ��
*********************************************************************************************************
*/
ITStatus HT_RTC_ITFlagStatusGet(uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (HT_RTC->RTCIF & ITFlag)
    {       
        return SET;                        /*!< RTC Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< RTC Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR RTC INTERRUPT FLAG
*
* ����˵��: ���RTC�жϱ�־
*
* ��ڲ���: ITFlag     ��Ҫ�����ĳ��RTC�жϱ�־������Ϊ���²����������:
*                        @arg RTC_RTCIF_SECIF
*                        @arg RTC_RTCIF_MINIF
*                        @arg RTC_RTCIF_HRIF
*                        @arg RTC_RTCIF_DAYIF
*                        @arg RTC_RTCIF_MTHIF
*                        @arg RTC_RTCIF_RTC1IF
*                        @arg RTC_RTCIF_RTC2IF
*                        @arg RTC_RTCIF_ALMIF
*
* ���ز���: ��
* 
* ����˵��: ��
*********************************************************************************************************
*/
void HT_RTC_ClearITPendingBit(uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    HT_RTC->RTCIF  &= ~((uint32_t)ITFlag);                  /*!< Clear RTC Interrupt Flag       */
    
}

/*
*********************************************************************************************************
*                                LOAD RTC COFFIENT FROM INFO FLASH
*
* ����˵��: ��InfoFlash��װ��RTC����ϵ��
*
* ��ڲ���: ��
*
* ���ز���: Bool    = FALSE��Info�д洢��ϵ���쳣��װ��ʧ��
*                   = TRUE ��Info�д洢��ϵ��������װ�سɹ�
* 
* ����˵��: �û�Ӧ��֤�ڵ��ô˺���ʱFlash���ڶ�״̬
*********************************************************************************************************
*/
Bool HT_RTC_LoadInfoData(void)
{
    uint32_t i, chksum;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    
                                                           /*!< ���浱ǰд����״̬    */    
	
    for(i=0,chksum=0; i<14; i++)
    {
        chksum += HT_INFO->DataArry[i];
    }
    if((chksum != HT_INFO->DataArry[14])||(HT_INFO->DataArry[14]==0))                     
    {
		/*!< Info��ϵ���쳣        */
        /*!< �û����ڴ˽�һ�����Ӵ������   */
		#if defined HT502x
        HT_RTC->MCON01=0;
        HT_RTC->MCON23=0;
        HT_RTC->MCON45=0x8600;
        HT_RTC->DFAH=0;
        HT_RTC->DFAL=0;
        HT_RTC->DFBH=0x7F&0xFFFF;
        HT_RTC->DFBL=0xD8B3&0xFFFF;
        HT_RTC->DFCH=0x7E&0xFFFF;
        HT_RTC->DFCL=0xD76B&0xFFFF;
        HT_RTC->DFDH=0x0&0xFFFF;
        HT_RTC->DFDL=0x5194&0xFFFF;
        HT_RTC->DFEH=0x0&0xFFFF;
        HT_RTC->DFEL=0x0BB8&0xFFFF;
		#endif
		return FALSE;
    }
    else                                                   /*!< Info��ϵ������        */
    {
        HT_CMU->WPREG = CMU_WPREG_UnProtected;             /*!< �ر�д��������        */
        
        if (HT_RTC->DFAH    != HT_INFO->Muster.iDFAH)   HT_RTC->DFAH    = HT_INFO->Muster.iDFAH;
        if (HT_RTC->DFAL    != HT_INFO->Muster.iDFAL)   HT_RTC->DFAL    = HT_INFO->Muster.iDFAL;
        if (HT_RTC->DFBH    != HT_INFO->Muster.iDFBH)   HT_RTC->DFBH    = HT_INFO->Muster.iDFBH;
        if (HT_RTC->DFBL    != HT_INFO->Muster.iDFBL)   HT_RTC->DFBL    = HT_INFO->Muster.iDFBL;
        if (HT_RTC->DFCH    != HT_INFO->Muster.iDFCH)   HT_RTC->DFCH    = HT_INFO->Muster.iDFCH;
        if (HT_RTC->DFCL    != HT_INFO->Muster.iDFCL)   HT_RTC->DFCL    = HT_INFO->Muster.iDFCL;
        if (HT_RTC->DFDH    != HT_INFO->Muster.iDFDH)   HT_RTC->DFDH    = HT_INFO->Muster.iDFDH;
        if (HT_RTC->DFDL    != HT_INFO->Muster.iDFDL)   HT_RTC->DFDL    = HT_INFO->Muster.iDFDL;
        if (HT_RTC->DFEH    != HT_INFO->Muster.iDFEH)   HT_RTC->DFEH    = HT_INFO->Muster.iDFEH;
        if (HT_RTC->DFEL    != HT_INFO->Muster.iDFEL)   HT_RTC->DFEL    = HT_INFO->Muster.iDFEL;
        if (HT_RTC->Toff    != HT_INFO->Muster.iToff)   HT_RTC->Toff    = HT_INFO->Muster.iToff;
        if (HT_RTC->MCON01  != HT_INFO->Muster.iMCON01) HT_RTC->MCON01  = HT_INFO->Muster.iMCON01;
        if (HT_RTC->MCON23  != HT_INFO->Muster.iMCON23) HT_RTC->MCON23  = HT_INFO->Muster.iMCON23;
        if (HT_RTC->MCON45  != HT_INFO->Muster.iMCON45) HT_RTC->MCON45  = HT_INFO->Muster.iMCON45;
                                                           /*!< װ��ϵ��              */
        
        HT_CMU->WPREG = writeProtect;                      /*!< �ָ�֮ǰд��������    */

        return TRUE;
    }
}

