

#ifndef __ME3612_H__
#define __ME3612_H__


#include "includes.h"

#define ME3612_DEBUG                    0

//电源
#define GPRS_POWER_ENABLE()	  	        GPIO_SetBits(HT_GPIOA,GPIO_Pin_10)
#define GPRS_POWER_DISABLE()	        GPIO_ResetBits(HT_GPIOA,GPIO_Pin_10)

//开机
#define GPRS_PWRKEY_HIGH()	  		    GPIO_ResetBits(HT_GPIOA,GPIO_Pin_13)		//pwrkey低有效
#define GPRS_PWRKEY_LOW()	  		    GPIO_SetBits(HT_GPIOA,GPIO_Pin_13)

//复位
#define GPRS_RESET_HIGH()				GPIO_SetBits(HT_GPIOE,GPIO_Pin_3)
#define GPRS_RESET_LOW()				GPIO_ResetBits(HT_GPIOE,GPIO_Pin_3)


#define GPRS_WEAK_IN_HIGH()	  		    GPIO_SetBits(HT_GPIOC, GPIO_Pin_14)
#define GPRS_WEAK_IN_LOW()	  		    GPIO_ResetBits(HT_GPIOC, GPIO_Pin_14)



enum{
    ME3612_RESET = 0,
    ME3612_ATE,
    ME3612_GMM,
	ME3612_ATI,
	ME3612_GMR,
	ME3612_AT_ZGETICCID,
	ME3612_AT_ZIOTPREF,
	ME3612_AT_ZIOTPREF1,
	ME3612_AT_CPSMS,
	ME3612_AT_ZPCHSC,
	ME3612_AT_CFUN0,
	ME3612_AT_CFUN1,
	ME3612_AT_CSQ,
	ME3612_AT_CGDCONT,
	ME3612_AT_CGDCONT1,
	ME3612_AT_ZSNT,
    ME3612_AT_ZSNT1,
	ME3612_AT_CPIN,
	ME3612_AT_ZPAS,
	ME3612_AT_COPS,
	ME3612_AT_ZIPCALL1,
	ME3612_IPOPEN,
    GPRS_STATE_NUM
};



extern int GprsInit(void);
extern int GprsCheckRes(char *cmd, char *res, uint16_t tmo);
extern int GprsSendCmd(char *cmd, char *ack, uint16_t waittime, int flag);
void GprsReset(void);
int ME3612_check_test(char ok, uint8_t retry);
int ME3612_check_ATE(char ok, uint8_t retry);
int ME3612_check_GMM(char ok, uint8_t retry);
int ME3612_check_ATI(char ok, uint8_t retry);
int ME3612_check_GMR(char ok, uint8_t retry);
int ME3612_check_CPIN(char ok, uint8_t retry);
int ME3612_check_ack(char ok, uint8_t retry);
int ME3612_check_CCID(char ok, uint8_t retry);
int ME3612_check_ZSNT(char ok, uint8_t retry);
int ME3612_check_ZSNT1(char ok, uint8_t retry);
int ME3612_check_ZIOTPREF(char ok, uint8_t retry);
int ME3612_check_ZIOTPREF1(char ok, uint8_t retry);
int ME3612_check_CPSMS(char ok, uint8_t retry);
int ME3612_check_CFUN0(char ok, uint8_t retry);
int ME3612_check_CFUN1(char ok, uint8_t retry);
int ME3612_check_ZPCHSC(char ok, uint8_t retry);
int ME3612_check_CGDCONT(char ok, uint8_t retry);
int ME3612_check_ZPAS(char ok, uint8_t retry);
int ME3612_check_ZIPCALL0(char ok,uint8_t retry);
int ME3612_check_ZIPCALL1(char ok,uint8_t retry);
int ME3612_check_CSQ(char ok, uint8_t retry);
int ME3612_ipopen(char ok, uint8_t retry);
int ME3612_default(char ok, uint8_t retry);

int ME3612GetCREG(char *pStr);
int ME3612GetModuleId(char *pStr);

int SocketSendData(int socket, uint8_t* data, uint16_t len);
int Me3612Reconnect(void);
int Me3612SocketSend(int socket, uint8_t* data, uint16_t len);


#endif


