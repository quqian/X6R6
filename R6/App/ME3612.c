
#include "includes.h"
#include "ME3612.h"
#include "server.h"


#if USE_ME3612_4G_NET_MODE
const GPRS_INIT_TAB_STR gMe3612IinitTab[] = {
    {NULL, "OK", 3000, 0, ME3612_check_test},
    {"ATE0\r", "OK", 2000, 10, ME3612_check_ATE},								//使能回显
    {"AT+GMM\r","OK",1000,3,ME3612_check_GMM},
	{"ATI\r", "OK", 1000, 10, ME3612_check_ATI},								//显示产品ID信息
	{"AT+GMR\r", "OK", 1000, 10, ME3612_check_ATE},
	{"AT+ZGETICCID\r", "OK", 500, 10, ME3612_check_CCID},						//查询SIM卡的CCID
	{"AT+ZIOTPREF=2\r", "OK", 2000, 10, ME3612_check_ZIOTPREF},					//LTE制式切换为LTE NB1
	{"AT+ZIOTPREF?\r", "OK", 2000, 10, ME3612_check_ZIOTPREF1},					//查询制式结果
	{"AT+CPSMS=0\r", "OK", 2000, 10, ME3612_check_CPSMS},						//关闭PSM功耗测试
	{"AT+ZPCHSC=0\r", "OK", 2000, 10, ME3612_check_ZPCHSC},						//跟运营商确认扰码算法
	{"AT+CFUN=0\r", "OK", 2000, 10, ME3612_check_CFUN0},
	{"AT+CFUN=1\r", "OK", 2000, 10, ME3612_check_CFUN1},
	{"AT+CSQ\r", "OK", 1000, 10, ME3612_check_CSQ},             				//查看信号强度
	{"AT+CGDCONT=1,\"IP\",\"\"\r", "OK", 2000, 10, ME3612_check_CGDCONT},	    //如果网络不支持IPV6,设置仅支持IPV4
	{"AT+CGDCONT?\r", "OK", 2000, 10, ME3612_check_CGDCONT},
	{"AT+ZSNT=6,0,0\r", "OK", 2000, 10, ME3612_check_ZSNT},						//设置网络模式LTE_ONLY
	{"AT+ZSNT?\r", "OK", 2000, 10, ME3612_check_ZSNT1},							//查询网络设置结果
    {"AT+CPIN?\r", "READY", 2000, 10, ME3612_check_CPIN},						//检测卡
	{"AT+ZPAS?\r", "LTE Cat.NB1", 3000, 10, ME3612_check_ZPAS},					//查询模块网络状态
	{"AT+COPS?\r", "OK", 2000, 10, ME3612_check_CPSMS},
	{"AT+ZIPCALL=1\r", "ZIPCALL", 500, 10, ME3612_check_ZIPCALL1},				//获取网络IP
	{NULL, NULL, 0, 0, ME3612_ipopen},
};


#if 0
//******************************************************************
//! \brief  	gprs_power_off
//! \param
//! \retval
//! \note   	4g模块关机
//******************************************************************
void gprs_power_off(void)
{
	CL_LOG("4G modle is power off.\n");

	//通过AT指令关机 关机时间15s
	for (int i=0; i<2; i++) {
        if (GprsSendCmd("AT+ZTURNOFF\r", "OK", 1000, 0)==0) {
            CL_LOG("4G power off ok.please wait 15s...\n");
			break;
        }
		CL_LOG("4G power off failed.retry:%d\n",i+1);
        OS_DELAY_MS(3000);
    }
	vTaskDelay(1000*15);

//通过硬件引脚关机 关机时间22s
//	GPRS_PWRKEY_LOW();
//	vTaskDelay(3000);
//	GPRS_PWRKEY_HIGH();
//	vTaskDelay(1000*22);

	//关闭电源
    GPRS_POWER_DISABLE();
    CL_LOG("power off 4G\r\n");
}
#endif


//******************************************************************
//! \brief  	gprs_power_on
//! \param
//! \retval
//! \note   	4g模块开机
//******************************************************************
void Me3612PowerOn(void)
{
	//打开电源
	GPRS_POWER_ENABLE();
	vTaskDelay(3000);
	GPRS_PWRKEY_LOW();
	vTaskDelay(200);	//拉低0.2s
	GPRS_PWRKEY_HIGH();
	vTaskDelay(1000*20);//等待开机完成
    CL_LOG("power on 4G\r\n");
}


//******************************************************************
//! \brief  	gprs_reset
//! \param
//! \retval
//! \note   	4g模块复位
//******************************************************************
void Me3612Reset(void)
{
	//
	//关闭电源
    GPRS_POWER_DISABLE();
	vTaskDelay(1000);
	Me3612PowerOn();

//	GPRS_RESET_HIGH();
//	vTaskDelay(1000);
//	GPRS_RESET_LOW();
//	vTaskDelay(10000);
}


int ME3612GetCREG(char *pStr)
{
    char *p = strstr((void *)pStr,"+CREG: ");
    int  state = 0;
    int  netstate = 0;
    if(p)
    {
        sscanf(p,"+CREG: %d,%d",&state,&netstate);
        CL_LOG("CREG : state = %d,netstate = %d\r\n",state,netstate);
        
        if((netstate == 1) || (netstate == 5))
        {
            return CL_OK;
        }
    }
    return CL_FAIL;
}

int ME3612GetModuleId(char *pStr)
{
    // \r\nME3610\r\nOK\r\n
    char *p = pStr;
    char moduleId[8] = {0};
    if(p)
    {
        sscanf(p,"\n%[^\r]",moduleId);
        CL_LOG("get Module ID : %s\r\n",moduleId);
        if(strncmp(moduleId,"ME3612",strlen("ME3612")) == 0)
        {
            gGetModuleType = NET_MODE_NBIOT_3612;
            if(gGetModuleType == gCurrentModuleInitType)
                return CL_OK;
        }
    }
    return CL_FAIL;
}


int Me3612CheckRes(char *cmd, char *res, uint16_t tmo)
{
    char *ret=NULL;
    uint8_t c;
    int cnt = 0;
    int retv = CL_FAIL;

    memset(gprsBuffer, 0, sizeof(gprsBuffer));
    //CL_LOG("gprs_check_res : recv data=");
    for(uint16_t time = 0; time < tmo; time+= 50) {
        vTaskDelay(50);
        while(UsartGetOneData(GPRS_UART_PORT, &c) == 0) {
            #if (1 == ME3612_DEBUG)
            printf("%02x ",c);
            #endif
            if (c) {
                if (sizeof(gprsBuffer) > cnt) {
                    gprsBuffer[cnt++] = c;
                }else{
                    CL_LOG("cnt=%d,error.\n",cnt);
                }
            }
        }

        ret = strstr((const char*)gprsBuffer, (const char*)res);
        if (ret) {
            #if (1 == ME3612_DEBUG)
            printf("\r\n");
            #endif
            retv = CL_OK;
            if (NULL != cmd) {
                if (strstr(cmd, "AT+CSQ")) {
                    ret = strstr((char*)gprsBuffer, "CSQ: ");//+CSQ: 23,99
                    if (ret != NULL) {
                        gNetSignal = atoi(&ret[5]);
            			c = atoi(&ret[6]);
                        CL_LOG("4G signal value=%d, ber=%d.\n", gNetSignal,c);
                    }
                }else if (strstr(cmd, "AT+ZGETICCID")) {
					GetCcidSn((void*)gprsBuffer);
				}else if(strstr(cmd, "AT+ZIPCALL?")){
					//断开状态
					if(strstr((void*)gprsBuffer,"+ZIPCALL: 0")){
						istage++;
					}
				}
                else if(strstr(cmd,"AT+CREG")) {
                    retv = ME3612GetCREG((void*)gprsBuffer);
                }
                else if(strstr(cmd,"AT+GMM")) {
                    retv = ME3612GetModuleId((void*)gprsBuffer);
                }
            }
            break;
        }
    }

    #if (1 == ME3612_DEBUG)
    CL_LOG("<<< [%s].\n",gprsBuffer);
    #endif

    return retv;
}


//flag = 1是发>
int Me3612SendCmd(char *cmd, char *ack, uint16_t waittime, int flag)
{
	uint8_t res = 1;

    MuxSempTake(&gGprsSendMux);
    UsartSend(GPRS_UART_PORT, (void*)cmd, strlen(cmd));

    #if (1 == ME3612_DEBUG)
	CL_LOG(">>> %s.\n",cmd);
    #endif

    if ((ack==NULL) || (waittime==0)) {
        MuxSempGive(&gGprsSendMux);
        return CL_OK;
    }

    if (CL_OK == Me3612CheckRes(cmd, ack, waittime)) {
        res = CL_OK; /*check success, retrun 0*/
    }else{
        res = (1==flag) ? 0 : 1;
    }
	MuxSempGive(&gGprsSendMux);
	return res;
}


int Me3612CheckModeCmd(char ok, uint8_t retry, uint16_t delay)
{
    if (ok==0) {
        istage++;
        return CL_OK;
    }else if (retry > 10) {/*retry 10 times*/
        gSimStatus |= (1<<istage);
        istage = ME3612_RESET; /* goto reset */
	}
    return CL_FAIL;
}


int ME3612_check_ATE(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_GMM(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ATI(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_GMR(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_CPIN(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_CCID(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZSNT(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZSNT1(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZIOTPREF(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZIOTPREF1(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_CPSMS(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_CFUN0(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_CFUN1(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZPCHSC(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_CGDCONT(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZPAS(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZIPCALL0(char ok,uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_ZIPCALL1(char ok,uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}

int ME3612_check_CSQ(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}


int ME3612_check_test(char ok, uint8_t retry)
{
    CL_LOG("reset 4g module\n");
    Me3612Reset();
    for (int i=0; i<2; i++) {
        if (Me3612SendCmd("AT\r", "OK", gMe3612IinitTab[ME3612_RESET].wait, 0)==0) {
            CL_LOG("4G Test ok ...\n");
            istage++;
            return CL_OK;
        }
		CL_LOG("4G Test fail..\n");
        OS_DELAY_MS(3000);
    }
    gSimStatus |= (1<<istage);
    return CL_FAIL;
}


int ME3612_check_ack(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return Me3612CheckModeCmd(ok, retry, 0);
}


int Me3612OpenSocket(int socket, char* addr, int port)
{
    char cmd_req[64] = {0};

    sprintf(cmd_req,"AT+ZIPOPEN=%d,0,%s,%d\r", socket, addr, port);
    if (!Me3612SendCmd(cmd_req, "OK", 5000, 0)) {
        CL_LOG("[INFO] link %d open success,addr=%s,port=%d.\n", socket,addr,port);
        system_info.is_socket_0_ok = 1;
        return 0;
    }else{
        CL_LOG("[INFO] link %d open failure.\n", socket);
        system_info.is_socket_0_ok = 0;
        return -1;
    }
}

int ME3612_ipopen(char ok, uint8_t retry)
{
    static uint32_t state=0;
	int res = 0;
    int ready = 0;

    CL_LOG("in.\n");
    switch(state) {
        case NET_STATE_SOCKET0_OPEN:
        {
            ready = CL_FALSE;
            for(int i = 0; i < 10; i++)
            {
                res = Me3612OpenSocket(1, system_info.server_ip, system_info.server_port);
                if(!res)
                {
                    ready = CL_TRUE;
                    break;
                }
                OS_DELAY_MS(1000);
            }

            if(CL_TRUE == ready)
            {
                system_info.is_socket_0_ok = 1;
                state = NET_STATE_READY;
            }
            else
            {
                state = NET_STATE_FAILURE;
            }
        }
        break;

        case NET_STATE_READY:
        {
			state = NET_STATE_SOCKET0_OPEN;
            istage = GPRS_STATE_NUM;
        }
        break;

        case NET_STATE_FAILURE:
        {
            state=0;
            gSimStatus |= (1<<istage);
            istage = ME3612_RESET;
        }
        break;
    }
    return CL_OK;
}


int ME3612_default(char ok, uint8_t retry)
{
    if (istage < GPRS_STATE_NUM) {
        istage++;
    }
    return CL_OK;
}


//返回 0成功
int Me3612SendData(char *data, int len, char *ack, uint16_t waittime)
{
	int res = 1;

    UsartSend(GPRS_UART_PORT, (void*)data, len);
    #if (1 == ME3612_DEBUG)
	PrintfData("Me3612SendData", (void*)data, len);
    #endif
    if ((ack == NULL) || (waittime == 0)) return CL_OK;
    if (CL_OK == Me3612CheckRes(NULL, ack, waittime)) {
        res = 0; /*check success, retrun 0*/
    }
	return res;
}


int Me3612SocketSend(int socket, uint8_t* data, uint16_t len)
{
    char cmd_req[32] = {0};
    int res=0;

    if ((0 == socket) && (CL_FALSE == system_info.is_socket_0_ok)) 
	{
        CL_LOG("socket 0 is closed.\n");
        return -1;
    }

    for (int i = 0; i < 3; i++) 
	{
		sprintf(cmd_req,"AT+ZIPSENDRAW=%d,%d\r", 1, len);
        res = Me3612SendCmd(cmd_req, ">", 1000, 1);
        if (0 != res) 
		{
            CL_LOG("call GprsSendCmd=%d,error.\n",res);
        }

        if (CL_OK == (res = Me3612SendData((char*)data, len, "OK", 10000))) 
		{
            break;
        }
		else
		{
            system_info.tcp_tx_error_times++;
            OS_DELAY_MS (1000);
        }
    }

    if (res == 0) {
        system_info.tcp_tx_error_times = 0;
    }else {
        CL_LOG("send fail: res=%d,tcp_tx_error_times=%d.\n",res,system_info.tcp_tx_error_times);
        if (system_info.tcp_tx_error_times >= TX_FAIL_MAX_CNT) {
            system_info.tcp_tx_error_times = 0;
			CL_LOG("send fail,try to restart net.\n");
			system_info.is_socket_0_ok = CL_FALSE;
            gSimStatus |= (1<<(GPRS_STATE_NUM+1));
        }
    }
    return res;
}


int Me3612FtpGet(const char* serv, const char* un, const char* pw, const char* file, uint16_t chsum_in, uint32_t addr)
{
    return CL_OK;
}


//******************************************************************
//! \brief  	GprsInit
//! \param
//! \retval
//! \note   	2g模块初始化
//******************************************************************
int Me3612Init(void)
{
	//PA10--VBAT(POWER)  PA13--PWRKEY
	GPIO_Cfg(HT_GPIOA, GPIO_Pin_10|GPIO_Pin_13, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);

	//PE3--RESET_N
	GPIO_Cfg(HT_GPIOE, GPIO_Pin_3, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);

    //PC14 as weak_in
    GPIO_Cfg(HT_GPIOC, GPIO_Pin_14, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);

	//PC13--weak_out 输入模式
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_13, GPIO_Mode_IOIN, GPIO_Input_Up, GPIO_Output_PP);

	//复位脚置低
	GPRS_RESET_LOW();

	//WEAK_IN置高
	GPRS_WEAK_IN_HIGH();

	istage = ME3612_RESET;
	//gprs_power_on();
    CL_LOG("init ok.\n");
	return CL_OK;
}


int Me3612Reconnect(void)
{
	char ok;
    uint8_t retry = 0;

    Me3612Init();
	while(1) {
        Feed_WDT();
        OS_DELAY_MS(gMe3612IinitTab[istage].nwait*10);
        if (gMe3612IinitTab[istage].cmd) {
            ok = Me3612SendCmd(gMe3612IinitTab[istage].cmd,gMe3612IinitTab[istage].res,gMe3612IinitTab[istage].wait, 0);
        }

        if (gMe3612IinitTab[istage].process) {
            if (CL_OK == gMe3612IinitTab[istage].process(ok, retry)) {
                retry = 0;
            }else{
                retry++;
            }
        }

		if (istage == GPRS_STATE_NUM) {
			CL_LOG("4G init and set socket ok.\n");
			return CL_OK;
		}else if (ME3612_RESET == istage) {
            CL_LOG("4G init and set socket fail.\n");
            return CL_FAIL;
        }
	}
}
#endif


