
#ifndef __ME3630_H__
#define __ME3630_H__


#include "includes.h"


#if (1 == TEST_AIR_WIRE)
#define ME3630_DEBUG                    1
#else
#define ME3630_DEBUG                    0
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
    ME3630_RESET = 0,
    ME3630_ATE,
    ME3630_GMM,
    #if (0 == TEST_AIR_WIRE)
	ME3630_ATI,
    ME3630_CPIN,
	ME3630_CCID,
	ME3630_CREG,
	ME3630_COPS,
	ME3630_ZPAS,
	ME3630_ZIPCALL,
	ME3630_ZIPCALL0,
	ME3630_ZIPCALL1,
    ME3630_CSQ,
    ME3630_IPOPEN,
    #endif
    ME3630_STATE_NUM
};


int ME3630_check_test(char ok, uint8_t retry);
int ME3630_check_ATE(char ok, uint8_t retry);
int ME3630_check_GMM(char ok, uint8_t retry);
int ME3630_check_ATS(char ok, uint8_t retry);
int ME3630_check_ATI(char ok, uint8_t retry);
int ME3630_check_cpin(char ok, uint8_t retry);
int ME3630_check_ack(char ok, uint8_t retry);
int ME3630_check_CCID(char ok, uint8_t retry);
int ME3630_check_CSCLK(char ok, uint8_t retry);
int ME3630_check_CGACT(char ok,uint8_t retry);
int ME3630_check_ZIPCFG(char ok,uint8_t retry);
int ME3630_check_ZIPOPEN(char ok,uint8_t retry);
int ME3630_check_ZIPCALL(char ok,uint8_t retry);
int ME3630_check_ZIPCALL0(char ok,uint8_t retry);
int ME3630_check_ZIPCALL1(char ok,uint8_t retry);
int ME3630_check_CSQ(char ok, uint8_t retry);
int ME3630_check_CREG(char ok, uint8_t retry);
int ME3630_check_CSTT(char ok, uint8_t retry);
int ME3630_check_cgreg(char ok, uint8_t retry);
int ME3630_ipopen(char ok, uint8_t retry);
int ME3630_default(char ok, uint8_t retry);
int ME3630_check_ZPAS(char ok, uint8_t retry);
int ME3630_check_COPS(char ok, uint8_t retry);
int ME3630_check_CIPQSEND(char ok, uint8_t retry);
int Me3630SendCmd(char *cmd, char *ack, uint16_t waittime, int flag);
int ME3630GetCREG(char *pStr);
int ME3630GetModuleId(char *pStr);
int Me3630FtpGet(const char* serv, const char* un, const char* pw, const char* file);
int Me3630Reconnect(void);
int Me3630SocketSend(int socket, uint8_t* data, uint16_t len);


#endif


