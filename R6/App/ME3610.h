
#ifndef __ME3610_H__
#define __ME3610_H__


#include "includes.h"


#if (1 == TEST_AIR_WIRE)
#define ME3610_DEBUG                    1
#else
#define ME3610_DEBUG                    0
#endif

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


enum {
    ME3610_RESET = 0,
    ME3610_ATE,
    ME3610_GMM,
    #if (0 == TEST_AIR_WIRE)
	ME3610_ATI,
    ME3610_CPIN,
	ME3610_CCID,
	ME3610_CREG,
	ME3610_COPS,
	ME3610_ZPAS,
    ME3610_ZIPCFG,
	ME3610_ZIPCALL,
	ME3610_ZIPCALL0,
	ME3610_ZIPCALL1,
    ME3610_CSQ,
    ME3610_ZIPSETPRT,
    ME3610_IPOPEN,
    #endif
    ME3610_STATE_NUM
};


int ME3610_check_test(char ok, uint8_t retry);
int ME3610_check_ATE(char ok, uint8_t retry);
int ME3610_check_GMM(char ok, uint8_t retry);
int ME3610_check_ATS(char ok, uint8_t retry);
int ME3610_check_ATI(char ok, uint8_t retry);
int ME3610_check_cpin(char ok, uint8_t retry);
int ME3610_check_ack(char ok, uint8_t retry);
int ME3610_check_CCID(char ok, uint8_t retry);
int ME3610_check_CSCLK(char ok, uint8_t retry);
int ME3610_check_CGACT(char ok,uint8_t retry);
int ME3610_check_ZIPCFG(char ok,uint8_t retry);
int ME3610_check_ZIPOPEN(char ok,uint8_t retry);
int ME3610_check_ZIPCALL(char ok,uint8_t retry);
int ME3610_check_ZIPCALL0(char ok,uint8_t retry);
int ME3610_check_ZIPCALL1(char ok,uint8_t retry);
int ME3610_check_ZIPSETPRT(char ok,uint8_t retry);
int ME3610_check_CSQ(char ok, uint8_t retry);
int ME3610_check_CREG(char ok, uint8_t retry);
int ME3610GetCREG(char *pStr);
int ME3610_check_CSTT(char ok, uint8_t retry);
int ME3610_check_cgreg(char ok, uint8_t retry);
int ME3610_ipopen(char ok, uint8_t retry);
int ME3610_default(char ok, uint8_t retry);
int ME3610_check_ZPAS(char ok, uint8_t retry);
int ME3610_check_COPS(char ok, uint8_t retry);
int ME3610_check_CIPQSEND(char ok, uint8_t retry);
int ME3610GetModuleId(char *pStr);
int Me3610FtpGet(const char* serv, const char* un, const char* pw, const char* file);
int Me3610Reconnect(void);
int Me3610SocketSend(int socket, uint8_t* data, uint16_t len);
int Me3610SendCmd(char *cmd, char *ack, uint16_t waittime, int flag);
int Me3610SocketSend(int socket, uint8_t* data, uint16_t len);
int Me3610SendCmd(char *cmd, char *ack, uint16_t waittime, int flag);


#endif


