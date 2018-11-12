
#include "includes.h"
#include "ME3630.h"



const GPRS_INIT_TAB_STR gMe3630InitTab[] = {
    {NULL, "OK", 3000, 0, ME3630_check_test},					//
    {"ATE0\r", "OK", 5000, 10, ME3630_check_ATE},				//
    {"AT+GMM\r","OK",1000,3,ME3630_check_GMM},
    #if (0 == TEST_AIR_WIRE)
    {"ATI\r", "OK", 3000, 10, ME3630_check_ATI},				//显示产品ID信息
    {"AT+CPIN?\r", "READY", 500, 10, ME3630_check_cpin},		//指示用户是否需要密码
    {"AT+ZGETICCID\r", "OK", 500, 10, ME3630_check_CCID},		//查询SIM卡的CCID
	{"AT+CREG?\r","OK", 1000, 50, ME3630_check_CREG},			//检查GSM网络注册状态
	{"AT+COPS?\r","OK",1000,50,ME3630_check_COPS},				//获取 网络状态
	{"AT+ZPAS?\r","OK",1000,50,ME3630_check_ZPAS},				//获取 网络状态
	{"AT+ZIPCALL?\r","OK",3000,50,ME3630_check_ZIPCALL},		//查看链接状态---如果链路是打开需要关闭后再连接
	{"AT+ZIPCALL=0\r","OK",5000,100,ME3630_check_ZIPCALL0},		//关闭链接
	{"AT+ZIPCALL=1\r","OK",5000,100,ME3630_check_ZIPCALL1},		//建立链路 获取IP地址
    {"AT+CSQ\r", "OK", 1000, 10, ME3630_check_CSQ},             //查看信号强度
	{NULL, NULL, 0, 0, ME3630_ipopen},
    #endif
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
        if (Me3630SendCmd("AT+ZTURNOFF\r", "OK", 1000, 0)==0) {
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
void MeGprsPowerOn(void)
{
	//打开电源
	GPRS_POWER_ENABLE();
	vTaskDelay(3000);
	GPRS_PWRKEY_LOW();
	vTaskDelay(200);	//拉低0.2s
	GPRS_PWRKEY_HIGH();
	vTaskDelay(1000*15);//等待开机完成
    CL_LOG("power on 4G\r\n");
}


//******************************************************************
//! \brief  	gprs_reset
//! \param
//! \retval
//! \note   	4g模块复位
//******************************************************************
void Me3630Reset(void)
{
	//
	//关闭电源
    GPRS_POWER_DISABLE();
	vTaskDelay(1000);
	MeGprsPowerOn();
//	GPRS_RESET_HIGH();
//	vTaskDelay(1000);
//	GPRS_RESET_LOW();
//	vTaskDelay(10000);
}


int ME3630GetCREG(char *pStr)
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

int ME3630GetModuleId(char *pStr)
{
    char *p = pStr;
    char moduleId[8] = {0};
    if(p)
    {
        sscanf(p,"\n%[^\r]",moduleId);
        CL_LOG("get Module ID : %s\r\n",moduleId);
        if(strncmp(moduleId,"ME3630",strlen("ME3630")) == 0)
        {
            gGetModuleType = NET_MODE_4G_ME3630;
            if(gGetModuleType == gCurrentModuleInitType)
                return CL_OK;
        }
    }
    return CL_FAIL;
}

int Me3630CheckRes(char *cmd, char *res, uint16_t tmo)
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
            #if (1 == ME3630_DEBUG)
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
            #if (1 == ME3630_DEBUG)
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
                }

				else if (strstr(cmd, "AT+ZGETICCID")) {
					GetCcidSn((void*)gprsBuffer);
				}

				else if(strstr(cmd, "AT+ZIPCALL?")){

					//断开状态
					if(strstr((void*)gprsBuffer,"+ZIPCALL: 0")){
						istage++;
					}
				}
                else if(strstr(cmd,"AT+CREG")) {
                    retv = ME3630GetCREG((void*)gprsBuffer);
                }
                else if(strstr(cmd,"AT+GMM")) {
                    retv = ME3630GetModuleId((void*)gprsBuffer);
                }
            }
            break;
        }
    }

    #if (1 == ME3630_DEBUG)
    CL_LOG("<<< [%s].\n",gprsBuffer);
    #endif

    return retv;
}


//flag = 1是发>
int Me3630SendCmd(char *cmd, char *ack, uint16_t waittime, int flag)
{
	uint8_t res = 1;

    MuxSempTake(&gGprsSendMux);
    UsartSend(GPRS_UART_PORT, (void*)cmd, strlen(cmd));

    #if (1 == ME3630_DEBUG)
	CL_LOG(">>> %s.\n",cmd);
    #endif

    if ((ack==NULL) || (waittime==0)) {
        MuxSempGive(&gGprsSendMux);
        return CL_OK;
    }

    if (CL_OK == Me3630CheckRes(cmd, ack, waittime)) {
        res = CL_OK; /*check success, retrun 0*/
    }else{
        res = (1==flag) ? 0 : 1;
    }
	MuxSempGive(&gGprsSendMux);
	return res;
}

int Me3630trim(char * data)
{
    int i,j;

    for(i=0,j=0; i<strlen((char *)data); i++) {
        if (data[i] != ' ')
            data[j++] = data[i];
    }
    data[j] = '\0';
    return 0;
}


int Me3630CheckResnospace(char * ack)
{
    uint8_t data;
    uint8_t flag = 0;

    while (UsartGetOneData(GPRS_UART_PORT, &data) == 0) 
	{
        if (gGprsRxLen < BUFFER_SIZE) 
		{
            flag = 1;
            gprsBuffer[gGprsRxLen] = data;
			gGprsRxLen++;
        }
        //OS_DELAY_MS(1);
        Feed_WDT();
    }

    if (flag) 
	{
        //gprsBuffer[gGprsRxLen] = 0;
        //Me3630trim((char *)gprsBuffer);
        return 1;
    }
	
    return 0;
}


//flag = 1是发>
int Me3630SendCmdnospace(char * cmd, char * ack, int waitCnt, int waittime)
{
	int i;
    int k;

	MuxSempTake(&gGprsSendMux);
    for (i=0; i<3; i++) 
	{
        gGprsRxLen = 0;
        UsartSend(GPRS_UART_PORT, (void *)cmd, strlen(cmd));
    	if (ack) 
		{
        	for (k = 0; k < waitCnt; k++) 
			{
            	OS_DELAY_MS(waittime);
                Feed_WDT();
            	if (Me3630CheckResnospace(ack)) 
				{
					MuxSempGive(&gGprsSendMux);
                	return 0;
            	}
        	}
    	}
		else
		{
    		MuxSempGive(&gGprsSendMux);
        	return 0;
    	}
    }
	MuxSempGive(&gGprsSendMux);
    return -1;
}


int ME3630CheckModeCmd(char ok, uint8_t retry, uint16_t delay)
{
    if (ok==0) {
        istage++;
        return CL_OK;
    }else if (retry > 10) {/*retry 10 times*/
        gSimStatus |= (1<<istage);
        istage = ME3630_RESET; /* goto reset */
	}
    return CL_FAIL;
}


int ME3630_check_ATE(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_GMM(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_ATI(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_cpin(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_CCID(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}


int ME3630_check_ZIPOPEN(char ok,uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}


int ME3630_check_ZIPCALL(char ok,uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_ZIPCALL0(char ok,uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_ZIPCALL1(char ok,uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}


int ME3630_check_CSQ(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_CREG(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_COPS(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}

int ME3630_check_ZPAS(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}


int ME3630_check_test(char ok, uint8_t retry)
{
    CL_LOG("reset ME3630 module\n");
    Me3630Reset();
    for (int i=0; i<2; i++) {
        if (Me3630SendCmd("AT\r", "OK", gMe3630InitTab[ME3630_RESET].wait, 0)==0) {
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


int ME3630_check_ack(char ok, uint8_t retry)
{
	CL_LOG("in.\n");
    return ME3630CheckModeCmd(ok, retry, 0);
}


int Me3630OpenSet(int socket, char* addr, int port)
{
    char cmd_req[64] = {0};

    sprintf(cmd_req,"AT+ZIPOPEN=%d,0,%s,%d\r", socket, addr, port);
    if(!Me3630SendCmd(cmd_req, "OK", 5000, 0)) {
        CL_LOG("[INFO] link %d open success,addr=%s,port=%d.\n", socket,addr,port);
        system_info.is_socket_0_ok = 1;
        return 0;
    }else{
        CL_LOG("[INFO] link %d open failure.\n", socket);
        system_info.is_socket_0_ok = 0;
        return -1;
    }
}


int ME3630_ipopen(char ok, uint8_t retry)
{
    static uint32_t state=0;
	int res = 0;
    int ready = 0;

    //CL_LOG("in.\n");
    switch(state) {
        case NET_STATE_SOCKET0_OPEN:
        {
            ready = CL_FALSE;
            for(int i = 0; i < 10; i++)
            {
                res = Me3630OpenSet(1, system_info.server_ip, system_info.server_port);
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
            istage = ME3630_STATE_NUM;
        }
        break;

        case NET_STATE_FAILURE:
        {
            state=0;
            gSimStatus |= (1<<istage);
            istage = ME3630_RESET;
        }
        break;
    }
    return CL_OK;
}


int ME3630_default(char ok, uint8_t retry)
{
    if (istage < ME3630_STATE_NUM) {
        istage++;
    }
    return CL_OK;
}


//返回 0成功
int Me3630SendData(char *data, int len, char *ack, uint16_t waittime)
{
	int res = 1;

    UsartSend(GPRS_UART_PORT, (void*)data, len);
    #if (1 == ME3630_DEBUG)
	PrintfData("Me3630", (void*)data, len);
    #endif
    if ((ack == NULL) || (waittime == 0)) return CL_OK;
    if (CL_OK == Me3630CheckRes(NULL, ack, waittime)) {
        res = 0; /*check success, retrun 0*/
    }
	return res;
}


int Me3630SocketSend(int socket, uint8_t* data, uint16_t len)
{
    char cmd_req[32] = {0};
    int res=0;

    if ((0 == socket) && (CL_FALSE == system_info.is_socket_0_ok)) 
	{
        CL_LOG("socket 0 is closed.\n");
        return -1;
    }

    for (int i = 0; i < 4; i++) 
	{
		sprintf(cmd_req,"AT+ZIPSENDRAW=%d,%d\r", 1, len);
        res = Me3630SendCmd(cmd_req, ">", 1000, 1);
        if (0 != res) 
		{
            CL_LOG("call Me3630SendCmd=%d,error.\n",res);
        }

        if (CL_OK == (res = Me3630SendData((char*)data, len, "OK", 10000))) 
		{
            break;
        }
		else
		{
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
            gSimStatus |= (1<<(ME3630_STATE_NUM+1));
        }
    }
    return res;
}


int Me3630FtpGet(const char* serv, const char* un, const char* pw, const char* file)
{
    char ipconfig[64] = {0};
	uint8_t x6FwVer;
    int ret=0;
    int fsize;
    int cfize=0;
    uint16_t chsum_in;
    uint16_t checksum;
    uint8_t  getCnt;
    uint8_t  fwCnt;
    uint8_t  fwType;
    uint32_t getLen;
    uint32_t offset;
    int i;
    char *sp1;
    char tmp[64];
    uint32_t over_time;
    uint32_t appBackupRecordAddr;
    FW_INFO_STR *pFwInfo = NULL;

	gprsBuffer[0] = 0;
    Me3630SendCmd("AT$ZFTPCLOSE\r","OK",1000,0);	//AT$ZFTPCLOSE, OK
	Me3630SendCmd("AT+ZIPCLOSE=1\r","OK",1000,0);	//AT+ZIPCLOSE=1, OK
	Me3630SendCmd("AT+CSQ\r","OK",1000,0);
	Me3630SendCmd("AT+ZPAS?\r","OK",1000,0);
	Me3630SendCmd("AT+CGDCONT=1,\"IP\",\"cmnet\"\r","OK",1000,0);
	Me3630SendCmd("AT$ZPDPACT=1\r","OK",1000,0);
    sp1 = strchr(serv, '/');//查找出现"/"的位置
    memset(tmp, 0, sizeof(tmp));
    strncpy(tmp,serv,sp1-serv);
	strcat(tmp,",");
	strcat(tmp,un);
	strcat(tmp,",");
	strcat(tmp,pw);
    sprintf(ipconfig, "AT$ZFTPCFG=%s\r", tmp);//AT$ZFTPCFG=118.31.246.230,x5,x543
    CL_LOG("zftpcfg=%s.\n",ipconfig);
    Me3630SendCmd(ipconfig,"\r\nOK\r\n",1000,0);//OK

	/*ftp get filesize*/
	memset(tmp, 0, sizeof(tmp));
    strncpy(tmp,sp1+1,strlen(sp1+1));
    tmp[strlen(tmp)] = '/';
	strcat(tmp,file);
	memset(ipconfig, 0, sizeof(ipconfig));
    sprintf(ipconfig, "AT$ZFTPSIZE=%s\r", tmp);
	CL_LOG("ftp get file size=%s.\n",ipconfig);
	if (Me3630SendCmdnospace(ipconfig, "FTPSIZE:",50,100) == 0) {//AT$ZFTPSIZE=TMP/T.LOG
		sprintf(tmp,"FTPSIZE:");
        fsize = restoi((char *)gprsBuffer, tmp,'\r');
        if (fsize==0) {
            CL_LOG("fsize=0, fail.\n");
            ret = -2;
            goto EXIT1;
        }
    }
    CL_LOG("fsize=%d.\n",fsize);

	Me3630SendCmd("AT+ZIPCLOSE=1\r","OK",1000,0);	//AT+ZIPCLOSE=1, OK
	Me3630SendCmd("AT+ZIPCALL=0\r","OK",1000,0);	//AT+ZIPCLOSE=1, OK
	Me3630SendCmd("AT+GMR\r","OK",1000,0);	//AT+ZIPCLOSE=1, OK
    //Me3630SendCmd("AT+ZCDRUN?\r","OK",1000,0);	//AT+ZIPCLOSE=1, OK
    //Me3630SendCmd("AT$ZFTPCLOSE\r","OK",1000,0);	//AT+ZIPCLOSE=1, OK
    //Me3630SendCmd("AT+ZIPSETRPT=?\r","OK",1000,0);	//AT+ZIPSETRPT=0, OK
    //Me3630SendCmd("AT+ZIPSETRPT=0\r","OK",1000,0);	//AT+ZIPSETRPT=0, OK
    //Me3630SendCmd("AT+ZIPSETRPT=?\r","OK",1000,0);	//AT+ZIPSETRPT=0, OK

    ret = 0;
    over_time = GetRtcCount();
    getLen = 16;
    getCnt = 0;
    offset = 0;
    while(1) {
        Feed_WDT();
        if (300 < (uint32_t)(GetRtcCount() - over_time)) 
		{
            CL_LOG("download timeOut,error.\n");
            break;
        }

        memset(tmp, 0, sizeof(tmp));
        strncpy(tmp,sp1+1,strlen(sp1+1));
        tmp[strlen(tmp)] = '/';
    	strcat(tmp,file);
    	memset(ipconfig, 0, sizeof(ipconfig));
        sprintf(ipconfig, "AT$ZFTPGET=%s,%d,%d\r", tmp, offset, getLen);
        //CL_LOG("ftp get filename=%s.\n",ipconfig);
        if (Me3630SendCmdnospace(ipconfig,"\r\nOK\r\n",100, 50) == 0) 
		{//AT$ZFTPGET=TMP/T.LOG
            offset += getLen;
            if (16 == getLen) 
			{
                if (0 == getCnt) 
				{
                    if ((0xaa == gprsBuffer[2]) && (0x55 == gprsBuffer[3])) 
					{
                        fwCnt = gprsBuffer[4];
                        getCnt++;
                        CL_LOG("get aa 55 ok,fwCnt=%d.\n",fwCnt);
                        if (0 == fwCnt)
						{
                            CL_LOG("fwCnt=%d,error.\n",fwCnt);
                            return CL_FAIL;
                        }
						x6FwVer = gprsBuffer[5];
						CL_LOG("升级包的版本号 x6FwVer = %d.\n", x6FwVer);
                    }
					else
					{
                        CL_LOG("head=%02x,%02x,%02x,%02x,error.\n",  gprsBuffer[1], gprsBuffer[2], gprsBuffer[3], gprsBuffer[4]);
                        return CL_FAIL;
                    }
                }
				else if (1 == getCnt) 
				{ //读第一个固件信息
                    pFwInfo = (void*)&gprsBuffer[2];
                    fsize = pFwInfo->size;
                    chsum_in = pFwInfo->checkSum;
                    if (0 == memcmp(gpFwName[CHARGER_TYPE_R6], pFwInfo->name, strlen(gpFwName[CHARGER_TYPE_R6]))) 
					{
                        fwType = CHARGER_TYPE_R6;
                        getCnt++;
                        CL_LOG("check r6 fw name ok,fsize=%d,chsum_in=%d.\n",fsize,chsum_in);
                        getLen = 512;
                        appBackupRecordAddr = AppUpBkpAddr;
                        FlashEraseAppBackup(appBackupRecordAddr, APP_FW_SIZE);
                        checksum = 0;
                        cfize = 0;
                    }
					else if (0 == memcmp(gpFwName[CHARGER_TYPE_X6], pFwInfo->name, strlen(gpFwName[CHARGER_TYPE_X6]))) 
					{
                        fwType = CHARGER_TYPE_X6;
                        getCnt++;
                        CL_LOG("check x6 fw name ok,fsize=%d,chsum_in=%d.\n",fsize,chsum_in);
                        getLen = 512;
                        //if ((fsize == system_info.x6FwInfo.size) && (chsum_in == system_info.x6FwInfo.checkSum)) {
                        //    CL_LOG("fw is the same, no need to download.\n");
                        //    return CL_OK;
                        //}
                        memset(&system_info.x6FwInfo, 0, sizeof(system_info.x6FwInfo));
                        FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
                        appBackupRecordAddr = TERMINAL_APP_ADDR;
                        FlashEraseAppBackup(appBackupRecordAddr, SysCfgInfoAddr-TERMINAL_APP_ADDR);
                        checksum = 0;
                        cfize = 0;
                    }
					else
					{
                        CL_LOG("fw name=%s, error.\n",pFwInfo->name);
                        return CL_FAIL;
                    }
                }
                continue;
            }

            for (i = 0; i < getLen; i++)
			{
                checksum += gprsBuffer[i+2];
            }
            cfize += getLen;
            CL_LOG("total %d,get %d, [%d%%].\n", fsize, cfize, cfize*100/fsize);
            //PrintfData("ftpget", gprsBuffer, len);
			FlashWriteAppBackup(appBackupRecordAddr, &gprsBuffer[2], getLen);
			appBackupRecordAddr += getLen;
            getLen = fsize - cfize;
            getLen = (512 < getLen) ? 512 : getLen;

            if (cfize >= fsize)
            {
    			if (checksum == chsum_in)
                {
    				CL_LOG("write file size %d, checksum %4X,fwType = %d,ftp get success finish! .\n", fsize, checksum, fwType);
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
                        system_info.x6FwInfo.sum = GetPktSum((void*)&system_info.x6FwInfo, sizeof(system_info.x6FwInfo)-1);
						memset(&system_info.isUpgradeOk, 0, sizeof(system_info.isUpgradeOk));
                        FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
                    }
                    ret = 0;
    			}
				else
				{
    			    CL_LOG("checksum=%d != chsum_in=%d,error.\n",checksum,chsum_in);
                    ret = CL_FAIL;
                }
                goto EXIT1;
            }
        }
		else
		{
            CL_LOG("call Me3630SendCmdnospace fail.\n");
        }
    }
EXIT1:
    CL_LOG("ret=%d.\n",ret);
    return ret;
}   //Me3630FtpGet


//******************************************************************
//! \brief  	GprsInit
//! \param
//! \retval
//! \note   	2g模块初始化
//******************************************************************
int Me3630Init(void)
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

	istage = ME3630_RESET;

	//gprs_power_on();

    CL_LOG("GprsInit ok.\n");
	return CL_OK;
}


int Me3630Reconnect(void)
{
	char ok;
    uint8_t retry = 0;

    Me3630Init();
	while(1) {
        Feed_WDT();
        OS_DELAY_MS(gMe3630InitTab[istage].nwait*10);
        if (gMe3630InitTab[istage].cmd) {
            ok = Me3630SendCmd(gMe3630InitTab[istage].cmd,gMe3630InitTab[istage].res,gMe3630InitTab[istage].wait, 0);
        }

        if (gMe3630InitTab[istage].process) {
            if (CL_OK == gMe3630InitTab[istage].process(ok, retry)) {
                retry = 0;
            }else{
                retry++;
            }
        }

		if (istage == ME3630_STATE_NUM) {
			CL_LOG("me3630 init and set socket ok.\n");
			return CL_OK;
		}else if (ME3630_RESET == istage) {
            CL_LOG("me3630 init and set socket fail.\n");
            return CL_FAIL;
        }
	}
}


