/*rtc.c
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/


#include "includes.h"
#include "rtc.h"
#include <time.h>

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};
static const uint16_t TAB_DFx[10] =
{
	0x0000, 0x0000,
	0x007F, 0xDC38,
	0x007E, 0xDBF3,
	0x0000, 0x4C66,
	0x0000, 0x0384
};

__IO uint32_t gSysSecond = 0;

char gTimeStr[16];

/****************************************************************/


void RTC_IRQHandler(void)
{
    if (HT_RTC->RTCIF & 0x01) {
        HT_RTC->RTCIF &= (~0x01);
        gSysSecond++;
    }
}


inline uint32_t GetRtcCount(void)
{
    return gSysSecond;
}

int LinuxTickToDay(uint32_t tick, uint8_t *pDay)
{
    struct tm *time_now;
    time_t time_unix;

	time_unix = tick;
	time_now = localtime(&time_unix);
    pDay[0] = time_now->tm_wday;
    pDay[1] = time_now->tm_year - 100;
    pDay[2] = time_now->tm_mon+1;
    pDay[3] = time_now->tm_mday;
    pDay[4] = time_now->tm_hour;
    pDay[5] = time_now->tm_min;
    pDay[6] = time_now->tm_sec;
    return CL_OK;
}


//******************************************************************
//! \brief  	SetRtcTime
//! \param
//! \retval
//! \note
//******************************************************************
void SetRtcTime(void* pRTCTime)
{
    /*  assert_param  */

    uint8_t *pRTCWrite = pRTCTime;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];
                                                       /*!< 保存当前写保护状态    */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;             /*!< 关闭写保护功能        */

    HT_RTC->RTCWR = RTC_RTCWR_CLEAR;                   /*!< 清除写标志            */
                                                       /*!< 开始写                */
    HT_RTC->WEEKR  = pRTCWrite[0] & RTC_WEEKR;      //1-7
    HT_RTC->YEARR  = pRTCWrite[1] & RTC_YEARR;		//0-99
    HT_RTC->MONTHR = pRTCWrite[2] & RTC_MONTHR;		//1-12
    HT_RTC->DAYR   = pRTCWrite[3] & RTC_DAYR;		//1-28/29/30/31
    HT_RTC->HOURR  = pRTCWrite[4] & RTC_HOURR;		//0-23
    HT_RTC->MINR   = pRTCWrite[5] & RTC_MINR;		//0-59
    HT_RTC->SECR   = pRTCWrite[6] & RTC_SECR;		//0-59

    HT_RTC->RTCWR = RTC_RTCWR_UPDATE;                  /*!< 写完成                */

    HT_CMU->WPREG = writeProtect;                      /*!< 恢复之前写保护设置    */

}

void SetRtcCount(uint32_t tick)
{
    uint8_t day[8];

    gSysSecond = tick;
    LinuxTickToDay(tick+8*60*60, day);
    SetRtcTime(day);
}

//******************************************************************
//! \brief  	GetRtcTime
//! \param
//! \retval
//! \note
//******************************************************************
void GetRtcTime(void* pRTCTime)
{
    /*  assert_param  */
    uint8_t *pRTCRead = pRTCTime;

    HT_RTC->RTCRD = RTC_RTCRD_READFLAG;                /*!< 设置开始读标志        */
    while(HT_RTC->RTCRD & RTC_RTCRD_READFLAG);         /*!< 等待可以读            */
                                                       /*!< 开始读                */
    pRTCRead[0] = HT_RTC->WEEKR & RTC_WEEKR;
    pRTCRead[1] = HT_RTC->YEARR & RTC_YEARR;
    pRTCRead[2] = HT_RTC->MONTHR& RTC_MONTHR;
    pRTCRead[3] = HT_RTC->DAYR  & RTC_DAYR;
    pRTCRead[4] = HT_RTC->HOURR & RTC_HOURR;
    pRTCRead[5] = HT_RTC->MINR  & RTC_MINR;
    pRTCRead[6] = HT_RTC->SECR  & RTC_SECR;
}


uint32_t GetTimeStamp(void)
{
    struct tm time_now;
    uint8_t day[8];

    memset(&time_now,0,sizeof(struct tm));
    GetRtcTime(day);
    time_now.tm_wday = day[0];
    time_now.tm_year = day[1]+100;
    time_now.tm_mon =  day[2]-1;
    time_now.tm_mday = day[3];
    time_now.tm_hour = day[4]-8;
    time_now.tm_min =  day[5];
    time_now.tm_sec =  day[6];
    return mktime(&time_now);
}


char* GetCurrentTime(void)
{
    uint8_t day[8];

    GetRtcTime(day);
    sprintf(gTimeStr,"%02u-%02u %02u:%02u:%02u", day[2], day[3], day[4], day[5], day[6]);
    return gTimeStr;
}

/*******************************************************************************
功能描述：	RTC 自动补偿
*******************************************************************************/
void SetRtcCompensate (void)
{
	uint8_t i;
	uint32_t ichecksum;

	//TPS config-----------------------------------------
	HT_TBS->TBSCON|=0x0101;   //Chop都打开  2次平均输出   osr=64
  	HT_TBS->TBSPRD=0x03; //    //1s打开8次
 	//RTC  configure-------------------------------------
  	HT_RTC->RTCCON&=~0x18; // [TOUT]
  	HT_RTC->RTCCON|=0x06;
  	HT_RTC->DFIH=0;
  	HT_RTC->DFIL=0;
	for(i=0,ichecksum=0;i<14;i++)
	{
    		ichecksum +=HT_INFO->DataArry[i];
	}

	if((ichecksum==0)||(ichecksum!= HT_INFO->DataArry[14]))   //Info中未存储数据
	{
    	//系数用默认值
    		HT_RTC->Toff	= C_Toff;
      		HT_RTC->MCON01=C_MCON01;
      		HT_RTC->MCON23=C_MCON23;
      		HT_RTC->MCON45=C_MCON45;

      		HT_RTC->DFAH=TAB_DFx[0];
      		HT_RTC->DFAL=TAB_DFx[1];

      		HT_RTC->DFBH=TAB_DFx[2];
      		HT_RTC->DFBL=TAB_DFx[3];

      		HT_RTC->DFCH=TAB_DFx[4];
      		HT_RTC->DFCL=TAB_DFx[5];

      		HT_RTC->DFDH=TAB_DFx[6];
      		HT_RTC->DFDL=TAB_DFx[7];

      		HT_RTC->DFEH=TAB_DFx[8];
      		HT_RTC->DFEL=TAB_DFx[9];
	}
	else
	{
    		HT_RTC->DFAH = HT_INFO->Muster.iDFAH;
    		HT_RTC->DFAL = HT_INFO->Muster.iDFAL;
    		HT_RTC->DFBH = HT_INFO->Muster.iDFBH;
    		HT_RTC->DFBL = HT_INFO->Muster.iDFBL;
    		HT_RTC->DFCH = HT_INFO->Muster.iDFCH;
    		HT_RTC->DFCL = HT_INFO->Muster.iDFCL;
    		HT_RTC->DFDH = HT_INFO->Muster.iDFDH;
    		HT_RTC->DFDL = HT_INFO->Muster.iDFDL;
    		HT_RTC->DFEH = HT_INFO->Muster.iDFEH;
    		HT_RTC->DFEL = HT_INFO->Muster.iDFEL;

    		HT_RTC->Toff = HT_INFO->Muster.iToff;

    	//	HT_RTC->MCON01 = HT_INFO->Muster.iMCON01;
    	//	HT_RTC->MCON23 = HT_INFO->Muster.iMCON23;
    	//	HT_RTC->MCON45 = HT_INFO->Muster.iMCON45;
    	    HT_RTC->MCON01=C_MCON01;
      		HT_RTC->MCON23=C_MCON23;
      		HT_RTC->MCON45=C_MCON45;

	}
}

int RtcInit(void)
{
    HT_RTC->RTCIF = 0x00;
    HT_RTC->RTCIE=0;
    HT_RTC->RTCCON|=0x20;
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 2);
    HT_RTC->RTCIE =0x01;
    NVIC_EnableIRQ(RTC_IRQn);
    CL_LOG("init ok.\n");
    return CL_OK;
}



