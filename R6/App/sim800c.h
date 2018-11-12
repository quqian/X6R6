/*sim800c.h
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/
#ifndef __SIM800C_H__
#define __SIM800C_H__

#include "includes.h"

#if (1 == TEST_AIR_WIRE)
#define SIM800_DEBUG                    1
#else
#define SIM800_DEBUG                    0
#endif

#define GPRS_POWER_ENABLE()	  	            GPIO_SetBits(HT_GPIOA,GPIO_Pin_10)
#define GPRS_POWER_DISABLE()	            GPIO_ResetBits(HT_GPIOA,GPIO_Pin_10)

#define GPRS_PWRKEY_HIGH()	  		        GPIO_ResetBits(HT_GPIOA,GPIO_Pin_13)		//pwrkeyµÕ”––ß
#define GPRS_PWRKEY_LOW()	  		        GPIO_SetBits(HT_GPIOA,GPIO_Pin_13)

#define GPRS_SLEEP()	  		            GPIO_SetBits(HT_GPIOC, GPIO_Pin_14)
#define GPRS_WAKEUP()	  		            GPIO_ResetBits(HT_GPIOC, GPIO_Pin_14)


typedef enum{
    SIM800_RESET = 0,
    #if (0 == TEST_AIR_WIRE)
    SIM800_ATE,
	SIM800_ATI,
    SIM800_CPIN,
	SIM800_CCID,
	SIM800_CSCLK,
	SIM800_CIPRXGET,
	SIM800_CREG,
    SIM800_CGATT,
    SIM800_CK_CGATT,
	SIM800_CIPSHUT,
	SIM800_CIPMUX,
	SIM800_CSTT,
	SIM800_CIPSTATUS,
	SIM800_CIICR,
	SIM800_CIPSTATUS1,
    SIM800_CSQ,
	SIM800_CIFSR,
    SIM800_IPOPEN,
    #endif
    SIM800_STATE_NUM
}ip_state;



extern int GprsInit(void);
extern int GprsCheckRes(char *cmd, char *res, uint16_t tmo);
extern int GprsSendCmd(char *cmd, char *ack, uint16_t waittime, int flag);
void GprsReset(void);
int sim800_check_reset(char ok, uint8_t retry);
int sim800_check_ATE(char ok, uint8_t retry);
int sim800_check_ATS(char ok, uint8_t retry);
int sim800_check_ATI(char ok, uint8_t retry);
int sim800_check_cpin(char ok, uint8_t retry);
int sim800_check_ack(char ok, uint8_t retry);
int sim800_check_CCID(char ok, uint8_t retry);
int sim800_check_CSCLK(char ok, uint8_t retry);
int sim800_check_CIPRXGET(char ok, uint8_t retry);
int sim800_check_CSQ(char ok, uint8_t retry);
int sim800_check_CREG(char ok, uint8_t retry);
int sim800_check_CGATT(char ok, uint8_t retry);
int sim800_check_CIPMUX(char ok, uint8_t retry);
int sim800_check_CIPSHUT(char ok, uint8_t retry);
int sim800_check_CSTT(char ok, uint8_t retry);
int sim800_check_CIICR(char ok, uint8_t retry);
int sim800_check_cgreg(char ok, uint8_t retry);
int sim800_ipopen(char ok, uint8_t retry);
int sim800_default(char ok, uint8_t retry);
int sim800_check_CIPMODE(char ok, uint8_t retry);
int SocketSendData(int socket, uint8_t* data, uint16_t len);
int Sim800cReconnect(void);
int sim800_check_CIPQSEND(char ok, uint8_t retry);
int FtpGet(const char* serv, const char* un, const char* pw, const char* file);


#endif //__SIM800C_H__


