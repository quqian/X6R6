/*sim800c.c
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "sim800c.h"
#include "led.h"
#include "server.h"



const GPRS_INIT_TAB_STR ipinit_tab[]={
    {NULL, "OK", 3000, 0, sim800_check_reset},
    //{"ATS0=1\r", "OK", 5000, 10, Sim800CheckSetATS0},
    #if (0 == TEST_AIR_WIRE)
    {"ATE0\r", "OK", 5000, 10, sim800_check_ATE},
    {"ATI\r", "OK", 1000, 10, sim800_check_ATI},						//显示产品ID信息
    {"AT+CPIN?\r", "OK", 500, 10, sim800_check_cpin},			//指示用户是否需要密码
    {"AT+CCID\r", "OK", 500, 10, sim800_check_CCID},
	{"AT+CSCLK=0\r", "OK", 1000, 10, sim800_check_CSCLK},  		//打开模块睡眠功能
	{"AT+CIPRXGET=0\r", "OK", 1000, 10, sim800_check_CIPRXGET},		//自动接收GPRS数据
	{"AT+CREG?\r","OK", 1000, 50, sim800_check_CREG},			//检查GSM网络注册状态
	{"AT+CGATT=1\r", "OK", 4000, 50, sim800_check_CGATT},		//MT附着GPRS业务
    {"AT+CGATT?\r", "+CGATT: 1", 1000, 50, sim800_check_CGATT},		//检查MS是否附着GPRS业务
	{"AT+CIPSHUT\r","SHUT OK", 1000, 200, sim800_check_CIPSHUT},
	{"AT+CIPMUX=1\r", "OK", 100, 10, sim800_check_CIPMUX},		//设置多路连接
	{"AT+CSTT=\"cmnet\"\r","OK", 1000, 50, sim800_check_CSTT},
	{"AT+CIPSTATUS\r","OK", 1000, 100, sim800_check_ack},
	{"AT+CIICR\r","OK", 10000, 50, sim800_check_CIICR},			//建立无线链路,激活移动场景
	{"AT+CIPSTATUS\r","OK", 1000, 100, sim800_check_ack},
    {"AT+CSQ\r", "OK", 1000, 10, sim800_check_CSQ},                     //查看信号强度
	{"AT+CIFSR\r","OK", 1000, 50, sim800_default},
	{NULL, NULL, 0, 0, sim800_ipopen},
    #endif
};



//******************************************************************
//! \brief  	gprs_power_off
//! \param
//! \retval
//! \note   	2g模块关机
//******************************************************************
void gprs_power_off(void)
{
    GPRS_POWER_DISABLE();
    CL_LOG("power off 2G\r\n");
}


//******************************************************************
//! \brief  	gprs_power_on
//! \param
//! \retval
//! \note   	2g模块开机
//******************************************************************
void gprs_power_on(void)
{
	//GPRS_WAKEUP();
	GPRS_POWER_ENABLE();
	vTaskDelay(3000);
	GPRS_PWRKEY_LOW();
	vTaskDelay(6000);
	GPRS_PWRKEY_HIGH();
    CL_LOG("power on 2G\r\n");
}


//******************************************************************
//! \brief  	gprs_reset
//! \param
//! \retval
//! \note   	2g模块复位
//******************************************************************
void GprsReset(void)
{
    gprs_power_off();
    vTaskDelay(5000);
    gprs_power_on();
}


int GprsCheckRes(char *cmd, char *res, uint16_t tmo)
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
            #if (1 == SIM800_DEBUG)
            printf("%02x ",c);
            #endif
            if (c) {
                if (sizeof(gprsBuffer) > cnt) {
                    gprsBuffer[cnt++] = c;
					//printf("ccccc[%d] = %02x ", cnt, c);
                }else{
                    CL_LOG("cnt=%d, sizeof(gprsBuffer) = %d,error.\n", cnt, sizeof(gprsBuffer));
					break;
                }
            }
        }
        ret = strstr((const char*)gprsBuffer, (const char*)res);
        if (ret) {
            #if (1 == SIM800_DEBUG)
            printf("\r\n");
            #endif
            retv = CL_OK;
            if (NULL != cmd) {
                if (strstr(cmd, "AT+CSQ")) {
                    ret = strstr((char*)gprsBuffer, "CSQ: ");//+CSQ: 23,99
                    if (ret != NULL) {
                        gNetSignal = atoi(&ret[5]);
            			c = atoi(&ret[6]);
                        CL_LOG("2G signal value=%d, ber=%d.\n", gNetSignal,c);
                    }
                }

				if (strstr(cmd, "AT+CCID")) {
					GetCcidSn((void*)gprsBuffer);
				}
            }
            break;
        }
		else if((sizeof(gprsBuffer) - 1) <= cnt)
        {
            CL_LOG("超出buff cnt = %d\n", cnt);
            return CL_FAIL;
        }
    }

    #if (1 == SIM800_DEBUG)
    CL_LOG("<<< [%s].\n",gprsBuffer);
    #endif

    return retv;
}


//flag = 1是发>
int GprsSendCmd(char *cmd, char *ack, uint16_t waittime, int flag)
{
	uint8_t res = 1;

    MuxSempTake(&gGprsSendMux);
    UsartSend(GPRS_UART_PORT, (void*)cmd, strlen(cmd));

    #if (1 == SIM800_DEBUG)
	CL_LOG(">>> %s.\n",cmd);
    #endif

    if ((ack==NULL) || (waittime==0)) {
        MuxSempGive(&gGprsSendMux);
        return CL_OK;
    }

    if (CL_OK == GprsCheckRes(cmd, ack, waittime)) 
	{
        res = CL_OK; /*check success, retrun 0*/
    }
	else
	{
        res = (1==flag) ? 0 : 1;
    }
	MuxSempGive(&gGprsSendMux);
	return res;
}


int CheckModeCmd(char ok, uint8_t retry, uint16_t delay)
{
    if (ok==0) {
        istage++;
        return CL_OK;
    }else if (retry > 10) {/*retry 10 times*/
        istage = SIM800_RESET; /* goto reset */
	}
    return CL_FAIL;
}


int Sim800CheckSetATS0(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_ATE(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_ATS(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_ATI(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_cpin(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CCID(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CSCLK(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CIPRXGET(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CIPQSEND(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}


int sim800_check_CSQ(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CREG(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CGATT(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CIPMODE(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CIPMUX(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CIPSHUT(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CSTT(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}

int sim800_check_CIICR(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    if (ok==0) {
        istage++;
        return CL_OK;
    }else{
        if (retry > 2) {
            //istage = SIM800_RESET; /* goto reset */
            #if (0 == TEST_AIR_WIRE)
			istage = SIM800_CGATT;
            #endif
            return CL_OK;
        }
    }
    return CL_FAIL;
}


int sim800_check_reset(char ok, uint8_t retry)
{
    CL_LOG("reset 2g module\n");
    GprsReset();
    for (int i=0; i<2; i++) {
        if (GprsSendCmd("AT\r", "OK", ipinit_tab[SIM800_RESET].wait, 0)==0) {
            CL_LOG("2G reset ok \n");
            istage++;
            return CL_OK;
        }
		CL_LOG("2G reset fail.\n");
        OS_DELAY_MS(3000);
    }
    return CL_FAIL;
}


int sim800_check_ack(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return CheckModeCmd(ok, retry, 0);
}


int SOCKET_open_set(int socket, char* addr, int port)
{
    char cmd_req[64] = {0};
    char cmd_ack[64] = {0};

    sprintf(cmd_ack, "%d, CONNECT OK", socket);
    sprintf(cmd_req,"AT+CIPSTART=%d,\"TCP\",\"%s\",%d\r", socket, addr, port);

    if(!GprsSendCmd(cmd_req, cmd_ack, 5000, 0)) {
        CL_LOG("[INFO] link %d open success,addr=%s,port=%d.\n", socket,addr,port);
        system_info.is_socket_0_ok = 1;
        return 0;
    }else{
        CL_LOG("[INFO] link %d open failure.\n", socket);
        system_info.is_socket_0_ok = 0;
        return -1;
    }
}

int sim800_ipopen(char ok, uint8_t retry)
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
            	//printf("连网IP地址%s \n",system_info.server_ip);
                res = SOCKET_open_set(0, system_info.server_ip, system_info.server_port);
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
            istage = SIM800_STATE_NUM;
        }
        break;

        case NET_STATE_FAILURE:
        {
            state=0;
            istage = SIM800_RESET;
        }
        break;
    }
    return CL_OK;
}


int sim800_default(char ok, uint8_t retry)
{
    if (istage < SIM800_STATE_NUM) {
        istage++;
    }
    return CL_OK;
}


//返回 0成功
int GprsSendData(char *data, int len, char *ack, uint16_t waittime)
{
	int res = 1;

    UsartSend(GPRS_UART_PORT, (void*)data, len);
    #if (1 == SIM800_DEBUG)
	PrintfData("GprsSendData", (void*)data, len);
    #endif
    if ((ack == NULL) || (waittime == 0)) return CL_OK;
    if (CL_OK == GprsCheckRes(NULL, ack, waittime)) {
        res = 0; /*check success, retrun 0*/
    }
	return res;
}


int SocketSendData(int socket, uint8_t* data, uint16_t len)
{
    char cmd_req[32] = {0};
	char cmd_ack[32] = {0};
    int res=0;

    if ((0 == socket) && (CL_FALSE == system_info.is_socket_0_ok)) 
	{
        CL_LOG("socket 0 is closed.\n");
        return -1;
    }

    for (int i=0; i < 4; i++) 
	{
		sprintf(cmd_ack, "%d, SEND OK", socket);
		sprintf(cmd_req,"AT+CIPSEND=%d,%d\r", socket, len);
        res = GprsSendCmd(cmd_req, ">", 1000, 1);
        if (0 != res)
		{
            CL_LOG("call GprsSendCmd=%d,error.\n",res);
        }
        
        if (CL_OK == (res = GprsSendData((char*)data, len, cmd_ack, 10000))) 
		{
//            CL_LOG("GPRS发送数据成功! \n");
            break;
        }
		else
		{
        	CL_LOG("GPRS发送数据错误! \n");
            system_info.tcp_tx_error_times++;
            OS_DELAY_MS (1000);
        }
    }

    if (res == 0) 
	{
        system_info.tcp_tx_error_times = 0;
    }
	else 
	{
        CL_LOG("send fail: res=%d,tcp_tx_error_times=%d.\n",res,system_info.tcp_tx_error_times);
        if (system_info.tcp_tx_error_times >= TX_FAIL_MAX_CNT) 
		{
            system_info.tcp_tx_error_times = 0;
			CL_LOG("send fail,try to restart net.\n");
			system_info.is_socket_0_ok = CL_FALSE;
        }
    }
    return res;
}


int GetDataFromMode(uint16_t getLen)
{
    char tmp[64] = {0};
    uint16_t len = 0;
    uint8_t retry = 0;
    uint16_t j,k;

    memset(tmp, 0, sizeof(tmp));
	sprintf(tmp,"AT+FTPGET=%d,%d\r",2, getLen);
    GprsSendCmdChkNoSpace(tmp, NULL, 1,1000);
    OS_DELAY_MS(10);
    while (retry++ < 20) {
		Feed_WDT();
        memset(gprsBuffer, 0, sizeof(gprsBuffer));
        UsartGetline(GPRS_UART_PORT, (void*)gprsBuffer, 30, 1000);
        //CL_LOG("%s",gprsBuffer);
        for(j=0,k=0; j<strlen((char *)gprsBuffer); j++) {
            if(gprsBuffer[j]!=' ')
                gprsBuffer[k++]=gprsBuffer[j];
        }
        gprsBuffer[k] = '\0';
        if (strstr((char *)gprsBuffer,"+FTPGET:2,")) {
            len = restoi((char *)gprsBuffer, "+FTPGET:2,",'\0');
            break;
        }
    }
    return len;
}


int FtpGet(const char* serv, const char* un, const char* pw, const char* file)
{
    char ipconfig[64] = {0};
    int ret=0;
    int fsize;
    uint16_t checksum;
    uint8_t fwType;
    uint8_t x6FwVer;
    int cfize;
    uint16_t cSum;
    uint8_t  getCnt;    //0:读取整体信息；1:读取第1个固件信息；2:读取第2个固件信息；3:下载第3个固件；4:下载第4个固件
    uint16_t getLen;
    int len,i;
    char *sp1;
    char tmp[64];
    uint32_t over_time;
    uint32_t appBackupRecordAddr;
    FW_INFO_STR *pFwInfo = NULL;

	gprsBuffer[0] = 0;
    GprsSendCmd("AT+CIPCLOSE=0\r","CLOSE OK",1000, 0);
    GprsSendCmd("AT+CIPSHUT\r","SHUT OK",1000, 0);
    GprsSendCmd("AT+SAPBR=1,1\r","\r\nOK\r\n",1000, 0);
	#if 0
    if (GprsSendCmd("AT+SAPBR=2,1\r","+SAPBR: 1,1",2000, 0) != 0) {
        CL_LOG("send cmd fail,error.\n");
        ret = -1;
        goto EXIT1;
    }
	#else
	for(int k = 0; k < 3; k++)
	{
		if (GprsSendCmd("AT+SAPBR=2,1\r","+SAPBR: 1,1",3000, 0) == CL_OK)
		{
			break;
		}
		else if(2 == k)
		{
			CL_LOG("send cmd fail,error.\n");
	        ret = -1;
	        goto EXIT1;
		}
		OS_DELAY_MS(50);
	}
	#endif
    GprsSendCmd("AT+FTPCID=1\r","\r\nOK\r\n",200, 0);
    GprsSendCmd("AT+FTPTYPE=\"I\"\r","\r\nOK\r\n",200, 0);
    GprsSendCmd("AT+FTPMODE=0\r","\r\nOK\r\n",200, 0);
	
    //set ftp server address and port  ftpget("118.31.246.230/2017/1001", "x5", "x543", "5751.bin", 0xa7b3);
	//设置服务器地址
	sp1 = strchr(serv, '/');
    memset(tmp, 0, sizeof(tmp));
    strncpy(tmp,serv,sp1-serv);
    sprintf(ipconfig, "AT+FTPSERV=\"%s\"\r", tmp);
    CL_LOG("serv=%s.\n",ipconfig);
    GprsSendCmd(ipconfig,"\r\nOK\r\n",200, 0);

	//设置名称
    memset(ipconfig, 0, sizeof(ipconfig));
    sprintf(ipconfig, "AT+FTPUN=\"%s\"\r", un);
    GprsSendCmd(ipconfig,"\r\nOK\r\n",100, 0);

	//设置密码
    memset(ipconfig, 0, sizeof(ipconfig));
    sprintf(ipconfig, "AT+FTPPW=\"%s\"\r", pw);
    GprsSendCmd(ipconfig,"\r\nOK\r\n",100, 0);

	//设置下载路径
    memset(tmp, 0, sizeof(tmp));
    strncpy(tmp,sp1+1,strlen(sp1+1));
    tmp[strlen(tmp)] = '/';
    memset(ipconfig, 0, sizeof(ipconfig));
    sprintf(ipconfig, "AT+FTPGETPATH=\"%s\"\r", tmp);
    CL_LOG("ftp get path=%s.\n",ipconfig);
    GprsSendCmd(ipconfig,"\r\nOK\r\n",100, 0);

	//设置文件名
    memset(ipconfig, 0, sizeof(ipconfig));
    sprintf(ipconfig, "AT+FTPGETNAME=\"%s\"\r", file);
    CL_LOG("ftp get filename=%s.\n",ipconfig);
    GprsSendCmd(ipconfig,"\r\nOK\r\n",100, 0);

    /*ftp get filesize*/
    if (GprsSendCmdChkNoSpace("AT+FTPSIZE\r", "+FTPSIZE:", 10,1000) == 0) {
		//如此处理是为了防止bin文件中含有相同常量，导致误判
		sprintf(tmp,"+FTPSIZE:1,%d,",0);
        cfize = restoi((char *)gprsBuffer, tmp,'\r');
        if (cfize == 0) {
            //文件不存在或网络异常
            CL_LOG("cfize=0, fail.\n");
            ret = -2;
            goto EXIT1;
        }
    }else {
        CL_LOG("at+ftpsize fail.\n");
        ret = -2;
        goto EXIT1;
    }
    CL_LOG("cfize=%d.\n",cfize);

    /*Open the ftp get session*/
    if (GprsSendCmdChkNoSpace("AT+FTPGET=1\r", "+FTPGET:1,1", 10, 1000) == 0) {
        CL_LOG("call GprsSendCmdChkNoSpace ok.\n");
        ret = 0;
        /*erase flash and set write pos*/
        over_time = GetRtcCount();
        getLen = 16;
        getCnt = 0;
        while(1) {
            Feed_WDT();
            if ((uint32_t)(GetRtcCount() - over_time) > 300) {
                break;
            }

            len = GetDataFromMode(getLen);
            if (len)
			{
                len = GetGPRSBuffer(gprsBuffer, len);
                if (16 == getLen)
                {
                    if (0 == getCnt)
					{
                        if ((0xaa == gprsBuffer[0]) && (0x55 == gprsBuffer[1])) 
						{
                            CL_LOG("get aa 55 ok.\n");
                            if (0 == gprsBuffer[2])
							{
                                CL_LOG("fwCnt=%d,error.\n",gprsBuffer[2]);
                                return CL_FAIL;
                            }
                            x6FwVer = gprsBuffer[3];
                            CL_LOG("升级包的版本号 x6FwVer = %d.\n", x6FwVer);
                            getCnt++;
                        }
						else{
                            CL_LOG("head=%02x,%02x,error.\n",gprsBuffer[0],gprsBuffer[1]);
                            return CL_FAIL;
                        }
                        continue;
                    }
					else if (1 == getCnt)
					{ //读第一个固件R6信息
                        pFwInfo = (void*)gprsBuffer;
                        fsize = pFwInfo->size;
                        checksum = pFwInfo->checkSum;
                        if (0 == memcmp(gpFwName[CHARGER_TYPE_R6], pFwInfo->name, strlen(gpFwName[CHARGER_TYPE_R6])))
						{
                            fwType = CHARGER_TYPE_R6;
                            getCnt++;
                            CL_LOG("check r6 fw name ok, R6升级包,fsize=%d,checksum = 0x%x.\n", fsize, checksum);
                            getLen = 512;
                            appBackupRecordAddr = AppUpBkpAddr;
                            FlashEraseAppBackup(appBackupRecordAddr, APP_FW_SIZE);
                            cSum = 0;
                            cfize = 0;
                        }
						else if (0 == memcmp(gpFwName[CHARGER_TYPE_X6], pFwInfo->name, strlen(gpFwName[CHARGER_TYPE_X6])))
						{
                            fwType = CHARGER_TYPE_X6;
                            getCnt++;
                            CL_LOG("check x6 fw name ok, X6升级包,fsize=%d,checksum = 0x%x.\n",fsize,checksum);
                            getLen = 512;
							#if 0
                            if (//(fsize == system_info.x6FwInfo.size) && (checksum == system_info.x6FwInfo.checkSum) && 
								(system_info.x6FwInfo.ver == x6FwVer))
							{
                                CL_LOG("X6固件相同不需要升级.\n");
                                return CL_OK;
                            }
							else{
								system_info.x6FwInfo.ver = 0;
								
							}
							#endif
                            memset(&system_info.x6FwInfo, 0, sizeof(system_info.x6FwInfo));
                            FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
                            appBackupRecordAddr = TERMINAL_APP_ADDR;
                            FlashEraseAppBackup(appBackupRecordAddr, SysCfgInfoAddr-TERMINAL_APP_ADDR);
                            cSum = 0;
                            cfize = 0;
                        }
						else{
                            CL_LOG("fw name=%s, 固件名字错误! .\n",pFwInfo->name);
                            return CL_FAIL;
                        }
                        continue;
                    }
                }
                for (i=0; i<len; i++) {
                    cSum += gprsBuffer[i];
                    //printf("数据 = 0x%x, [%d] = 0x%x \n", cSum, i, gprsBuffer[i]);
                    //printf("%02X ", gprsBuffer[i]);
                    //if(0 == (i+1)%16)
                    //{
                    //    printf("\n");
                    //}
                }
                cfize += len;
                CL_LOG("total %d [%d%%].\n",cfize,cfize*100/fsize);
                //PrintfData("ftpget", gprsBuffer, len);
				FlashWriteAppBackup(appBackupRecordAddr, (void*)gprsBuffer, len);
				appBackupRecordAddr += len;
                len = fsize - cfize;
                getLen = (512 <= len) ? 512 : len;

                if (cfize >= fsize) {
        			if (checksum == cSum) {
        				CL_LOG("文件大小 write file size %d, checksum %4X,ftp get success finish! .\n", fsize, checksum);
                        if (CHARGER_TYPE_R6 == fwType)
						{
        				    WriteUpdateInfo(fsize, checksum);
        			    }
						else
						{
                            system_info.x6FwInfo.size = fsize;
                            system_info.x6FwInfo.checkSum = checksum;
                            system_info.x6FwInfo.ver = x6FwVer;
							CL_LOG("升级包X6版本号[%d], [%d].\n", system_info.x6FwInfo.ver, x6FwVer);
                            system_info.x6FwInfo.sum = GetPktSum((void*)&system_info.x6FwInfo, sizeof(system_info.x6FwInfo) - 1);
							memset(&system_info.isUpgradeOk, 0, sizeof(system_info.isUpgradeOk));
                            FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
                        }
                        ret = 0;
        			}
					else
					{
        			    CL_LOG("checksum=%d != cSum=%d,error.\n",checksum,cSum);
                        ret = CL_FAIL;
                    }
                    goto EXIT1;
                }
            }
        }
    }
	else {
        CL_LOG("call GprsSendCmdChkNoSpace fail.\n");
        ret = -5;
        goto EXIT1;
    }
EXIT1:
    CL_LOG("ret=%d.\n",ret);
    return ret;
}   //FtpGet


//******************************************************************
//! \brief  	GprsInit
//! \param
//! \retval
//! \note   	2g模块初始化
//******************************************************************
int GprsInit(void)
{
	//PA1 PA2 PA3 as 2G_EN 2G_SLEEP 2G_PWRKEY
	//GPIO_Cfg(HT_GPIOA, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
	//PA10  PA13 as 2G_EN  2G_PWRKEY
	GPIO_Cfg(HT_GPIOA, GPIO_Pin_10|GPIO_Pin_13, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
    //PC14 as 2G_SLEEP
    GPIO_Cfg(HT_GPIOC, GPIO_Pin_14, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
	GPRS_WAKEUP();
    istage = SIM800_RESET;
    CL_LOG("init ok.\n");
	return CL_OK;
}


int Sim800cReconnect(void)
{
	char ok;
    uint8_t retry = 0;

    GprsInit();
	while(1) {
        Feed_WDT();
        OS_DELAY_MS(ipinit_tab[istage].nwait*10);
        if (ipinit_tab[istage].cmd) {
            ok = GprsSendCmd(ipinit_tab[istage].cmd,ipinit_tab[istage].res,ipinit_tab[istage].wait, 0);
        }

        if (ipinit_tab[istage].process) {
            if (CL_OK == ipinit_tab[istage].process(ok, retry)) {
                retry = 0;
            }else{
                retry++;
            }
        }

		if (istage == SIM800_STATE_NUM) {
			CL_LOG("2G init and set socket ok.\n");
			return CL_OK;
		}else if (SIM800_RESET == istage) {
            CL_LOG("2G init and set socket fail.\n");
            return CL_FAIL;
        }
	}
}


