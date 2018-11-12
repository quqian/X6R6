/*server.c
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "server.h"
#include "led.h"
#include "blue.h"
#include "ME3612.h"
#include "ME3630.h"
#include "rtc.h"
#include "ME3610.h"

const uint8_t gConstZero[64] = {0};
                //        0  1    2     3    4     5     6    7    8
const char *gpFwName[] = {"","X9","X10","R6","X6P","X6L","R8","R9","X6"};


uint8_t  gNetSignal = 0;
system_info_t system_info;
system_info_str1 system_info1;
uint8_t  gPrintBuff[OUT_NET_PKT_LEN];
uint16_t gWrite = 0;
uint8_t gGprsSendMux = 0;
FIFO_S_t gSocketPktRxCtrl;
__IO uint8_t gGprsRxBuff[OUT_NET_PKT_LEN] = {0};
uint8_t  gprsBuffer[BUFFER_SIZE];
uint16_t gGprsRxLen;
uint32_t gSimStatus = 0;
uint8_t istage;
CHG_INFO_STR gChgInfo;
gun_info_t gun_info[MAX_TERMINAL_NUM<<1];
uint8_t  gProtoRecvBuff0[IN_QUEUE_NET_PKT_LEN];
uint8_t  gProtoRecvBuff1[IN_QUEUE_NET_PKT_LEN];
uint8_t  gProtoRecvBuff2[IN_QUEUE_NET_PKT_LEN];
uint8_t  gProtoRecvBuff3[IN_QUEUE_NET_PKT_LEN];
QUEUE_STR gQueueInfo;
uint8_t  gCurrentModuleInitType = 0;        //用于初始化的时候，当前正在进行通讯模块初始化的类型
uint8_t gGetModuleType = 0;                 //用于通讯模块返回的设备类型


void Delay_uSec(uint8_t mSec)
{
    uint32_t	i;
    while (mSec--) {
        for (i = 0; i < 3; i++) { //Fcpu=11010048Hz
            __NOP();
            __NOP();
            __NOP();
            __NOP();
        }
    }
}


#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
    if (gWrite < sizeof(gPrintBuff)) {
        gPrintBuff[gWrite++] = ch;
        if ('\n' == ch) {
            UsartSend(DEBUG_UART, gPrintBuff, gWrite);
            gWrite = 0;
        }
    }else{
		UsartSend(DEBUG_UART, gPrintBuff, sizeof(gPrintBuff));
        gWrite = 0;
	}
    return ch;
}


void PrintfData(void *pfunc, uint8_t *pdata, int len)
{
	#if 1
	int i;
	CL_LOG("call by %s,len=%d,pdata:",(char*)pfunc,len);
    Feed_WDT();
	for (i=0; i<len; i++) {
		printf("%02x",pdata[i]);
	}
	printf("\n");
	#endif
}


int MuxSempTake(uint8_t *pSemp)
{
    while (1 == *pSemp) {
        Feed_WDT();
        vTaskDelay(2);
    }
    *pSemp = 1;
    return CL_OK;
}


void MuxSempGive(uint8_t *pSemp)
{
    *pSemp = 0;
}


// 0x30 -> 0; a -> 0x0a; A -> 0x0a
uint8_t Val(uint8_t ch)
{
    uint8_t val = (uint8_t)-1;

    if ((ch >= '0') && (ch <='9')) {
        val = ch - '0';
        return val;
    }
    if ((ch >= 'A') && (ch <='F')) {
        val = ch - 'A' + 10;
        return val;
    }
    if ((ch >= 'a') && (ch <='f')) {
        val = ch - 'a' + 10;
        return val;
    }
    return val;
}


// des  src:  len:为 src的字符串长度
void DeviceBcd2str(char *des, unsigned char *src , int len)
{
    int i;
    unsigned char *p;
    char tmp[4];

    p = src;
    for(i=0; i<len; i++) {
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%02x", *p++);
        strcat(des, tmp);
    }
}


// 0x12  <= {0x31,0x32}   0x1210 <= {0x31,0x32,0x31} 0x1210 <= {0x31,0x32,0x31,0x30}
// srcLen : pbSrc的长度
int StrToHex(uint8_t *pbDest, const char *pbSrc, int srcLen)
{
    int32_t i=0,j=0;
    uint8_t chl,chh;

    memset(pbDest, 0, srcLen/2);
    while (i < srcLen) {
        if ((i+1) == srcLen) {  //最后一个,是奇数
            chl = Val(pbSrc[i]);
            pbDest[j] = chl<<4;
            j ++;
        }else{
            chh = Val(pbSrc[i]);
            chl = Val(pbSrc[i+1]);
            pbDest[j] = (chh << 4) | chl;
            j ++;
        }
        i = i+2;
    }
    return j;
}


void GetCcidSn(char *pStr)
{
    int i;
    int flag = 0;

    for (i=0; i<strlen(pStr); i++) {
        switch (flag) {
            case 0:
                if ((0x0a == pStr[i]) || (0x0d == pStr[i])) {
                    flag = 1;
                }
                break;
            case 1:
                if ((0x30 <= pStr[i]) && (pStr[i] <= 0x39)) {
                    if (memcmp(system_info.iccid, &pStr[i], ICCID_LEN)) {
                        memcpy(system_info.iccid, &pStr[i], ICCID_LEN);
                        system_info.iccid[ICCID_LEN] = 0;
                        FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
                    }
                    CL_LOG("SIM iccid: %s.\n", system_info.iccid);
                    return;
                }
                break;
        }
    }
}


void FlashLight(void)
{
    static uint8_t lightStatus = 0;

    if (0 == lightStatus) 
	{
        LED_BLUE_LIGHT_ON();
        lightStatus = 1;
    }
	else
	{
        LED_BLUE_LIGHT_OFF();
        lightStatus = 0;
    }

    if (gChgInfo.netLedStatus) 
	{
        LED_GREEN_LIGHT_ON();
        gChgInfo.netLedStatus = 0;
    }
	else
	{
        LED_GREEN_LIGHT_OFF();
    }

    if (gChgInfo.blueLedStatus) 
	{
        LED_RED_LIGHT_ON();
        gChgInfo.blueLedStatus = 0;
    }
	else
	{
        LED_RED_LIGHT_OFF();
    }
}


void sysTickRecover(void)
{
    gSysSecond = GetTimeStamp();
    CL_LOG("gSysSecond=%d.\n",gSysSecond);
}


int SendSocketData(uint8_t socket, uint8_t *pData, uint16_t length)
{
    int ret = CL_FAIL;

    if (NET_MODE_2G == system_info.localNetType) 
	{
        //CL_LOG("网络模式 NET_MODE_2G.\n");
        ret = SocketSendData(socket, pData, length);
    }
    #if USE_ME3610_4G_NET_MODE
	else if (NET_MODE_4G_ME3610 == system_info.localNetType) 
	{
        CL_LOG("网络模式 NET_MODE_4G_ME3610.\n");
        ret = Me3610SocketSend(socket, pData, length);
    }
    #endif
	else if(NET_MODE_4G_ME3630 == system_info.localNetType) 
	{
        CL_LOG("网络模式 NET_MODE_4G_ME3630.\n");
        ret = Me3630SocketSend(socket, pData, length);
    }
	#if USE_ME3612_4G_NET_MODE
	else if (NET_MODE_NBIOT_3612 == system_info.localNetType) 
	{
        CL_LOG("网络模式 NET_MODE_NBIOT_3612.\n");
        ret = Me3612SocketSend(socket, pData, length);
    }
	#endif
	else
	{
        CL_LOG("localType=%d,error 没有网.\n",system_info.localNetType);
    }
	
    return ret;
}


int trim(char * data)
{
    int i,j;

    for(i=0,j=0; i<strlen((char *)data); i++) {
        if (data[i] != ' ')
            data[j++] = data[i];
    }
    data[j] = '\0';
    return 0;
}


uint8_t * GprsCmdChkNoSpace(char * ack)
{
    char * strx;
    uint8_t data;
    uint8_t flag = 0;

    while ((UsartGetOneData(GPRS_UART_PORT, (void*)&data)) == CL_OK) {
        //接收到一个字符
        if (gGprsRxLen < BUFFER_SIZE) {
            flag = 1;
            gprsBuffer[gGprsRxLen++] = data;
        }
        OS_DELAY_MS(2);
    }

    if (flag) {
        gprsBuffer[gGprsRxLen] = 0;//添加结束符
        trim((char *)gprsBuffer);
        CL_LOG("gprsBuffer=%s.\n",gprsBuffer);
        strx = strstr((const char*)gprsBuffer, (const char*)ack);
        return (uint8_t*)strx;
    }
    return NULL;
}


int GprsSendCmdChkNoSpace(char * cmd, char * ack, int waitCnt, int waittime)
{
    int i;
    int k;

    for (i=0; i<3; i++) {
        gGprsRxLen = 0;
        UsartSend(GPRS_UART_PORT, (void *)cmd, strlen(cmd));
        if (ack) {		//需要等待应答
            for (k=0; k<waitCnt; k++) {
                OS_DELAY_MS(waittime);
                if (GprsCmdChkNoSpace(ack)) {
                    //得到有效数据
                    return 0;
                }
            }
        }else{
            return 0;
        }
    }
    return -1;
}


int restoi(char* buf, char* start, char end)
{
    char i;
    char *p = strstr(buf, start);

    p+=strlen(start);
    if(!p) return 0;
    for(i=0; i<10; i++) {
        if(p[i]==end)
        {
            p[i]='\0';
            break;
        }
    }
    return atoi(p);
}


int GetGPRSBuffer(uint8_t *buf, uint16_t len)
{
    uint8_t c;
    int i;
    int timeOut;

    for (i=0; i<len;) {
        Feed_WDT();
        timeOut = 2000;
        while (timeOut) {
            if (UsartGetOneData(GPRS_UART_PORT, &c) == 0) {
                buf[i++] = c;
                break;
            }else{
                if (--timeOut < 3) {
                    CL_LOG("timeOut=%d,get data error.\n",timeOut);
                    return i;
                }
            }
        }
    }
    return i;
}


int UsartGetline(uint8_t port, char *pBuff, uint16_t len, uint16_t tmo)
{
    int i = 0;
    uint8_t c;

    if (tmo == 0xffff) {
        while (UsartGetOneData(port, &c) == 0) {
            if (c == '\n')
                goto getline;
            pBuff[i++] = c;
        }
    }else{
        while (--tmo) {
            OS_DELAY_MS(2);
            while (UsartGetOneData(port, &c) == 0) {
                if (c == '\n')
                    goto getline;
                pBuff[i++] = c;
                len--;
                if (len == 1)
                    goto getline;
            }
        }
    }
getline:
    pBuff[i] = '\0';
    if (i>0 && pBuff[i-1]=='\r')
        pBuff[--i]='\0';
    return i;
}


int OpenNet(void)
{
    uint8_t  i;
    uint8_t  temp = 0;
    uint8_t  funIndex[] = {NET_MODE_2G, NET_MODE_4G_ME3630, NET_MODE_NBIOT_3612, NET_MODE_4G_ME3610};
    int (*afun[])() = {NULL,Sim800cReconnect,Me3630Reconnect,NULL,NULL};

    if (system_info.localNetType && (system_info.localNetType < NET_MODE_ALL)) {
        temp = funIndex[0];
        funIndex[0] = system_info.localNetType;
        funIndex[system_info.localNetType-1] = temp;
    }

    i = 1;
    while (1) {
        gCurrentModuleInitType = funIndex[i-1];
        if (afun[funIndex[i-1]]) {
            if (CL_OK == afun[funIndex[i-1]]()) {
                if (system_info.localNetType != funIndex[i-1]) {
                    system_info.localNetType = funIndex[i-1];
                    FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
                }
                return CL_OK;
            }
        }
        if (NET_MODE_ALL <= ++i) {
            return CL_FAIL;
        }
    }
}


void SetUpgradeInfo(DOWN_FW_REQ_STR *pfwInfo, uint8_t chargerType)
{
    char url[50] = {0};
    char usrName[6] = {0};
    char psw[6] = {0};
    char fileName[10] = {0};
    int i;

    gChgInfo.sendPKtFlag = 1;

    memcpy(url, pfwInfo->url, sizeof(pfwInfo->url));
    memcpy(usrName, pfwInfo->usrName, sizeof(pfwInfo->usrName));
    memcpy(psw, pfwInfo->psw, sizeof(pfwInfo->psw));
    memcpy(fileName, pfwInfo->fileName, sizeof(pfwInfo->fileName));
    CL_LOG("url=%s,usrName=%s,psw=%s,fileName=%s,checkSum=0x%x .\n",url,usrName,psw,fileName,pfwInfo->checkSum);
    
    for (i = 0; i < 3; i++)
	{
        if (NET_MODE_2G == system_info.localNetType) 
		{
            CL_LOG("后台请求升级, 开始升级! \n");
            if (CL_OK == FtpGet(url, usrName, psw, fileName)) 
			{
                break;
            }
		}
		#if USE_ME3610_4G_NET_MODE
		else if (NET_MODE_4G_ME3610 == system_info.localNetType) 
		{
		    CL_LOG("后台请求升级, 开始升级! \n");
            if (CL_OK == Me3610FtpGet(url, usrName, psw, fileName)) 
			{
                break;
            }
        }
		#endif
		else if (NET_MODE_4G_ME3630 == system_info.localNetType) 
		{
            CL_LOG("后台请求升级, 开始升级! \n");
            if (CL_OK == Me3630FtpGet(url, usrName, psw, fileName)) 
			{
                break;
            }
        }
        OS_DELAY_MS(5000);
    }
    NVIC_SystemReset();
}


void ServerRecvProc(void)
{
    static uint8_t  buff[OUT_NET_PKT_LEN];
    uint8_t  *pktBuff = &buff[16];
    uint8_t  data;
    static uint8_t  step = FIND_AA;
    static uint8_t  len;
    static uint8_t  pktLen;
    static uint8_t  length;
    static uint8_t  sum;

    while (CL_OK == FIFO_S_Get(&gSocketPktRxCtrl, &data))
	{
        switch (step) {
            case FIND_AA:
                if (data == 0xAA) 
				{
                    step = FIND_55;
                    pktBuff[0] = 0xAA;
                    pktLen = 1;
                }
				else
				{
                    //printf("%02x ",data);
                }
                break;

            case FIND_55:
                if (data == 0x55) 
				{
                    step = FIND_CHARGER_TYPE;
                    pktBuff[1] = 0x55;
                    pktLen++;
                }
				else if (data == 0xAA) 
				{
                    step = FIND_55;
                    pktBuff[0] = 0xAA;
                    CL_LOG("can not find 55.\n");
                }
				else
				{
                    step = FIND_AA;
                    CL_LOG("can not find 55.\n");
                }
                break;

            case FIND_CHARGER_TYPE:
                pktBuff[pktLen++] = data;
                len = 0;
                step = FIND_CHAGER_SN;
                break;

            case FIND_CHAGER_SN:
                pktBuff[pktLen++] = data;
                if (CHARGER_SN_LEN == ++len) 
				{
                    len = 0;
                    step = FIND_LEN;
                }
                break;

            case FIND_LEN:
                pktBuff[pktLen++] = data;
                if (2 == ++len) 
				{
                    length = (pktBuff[pktLen-1]<<8) | pktBuff[pktLen-2];
                    if (length >= (sizeof(buff)-8)) 
					{
                        CL_LOG("length=%d,error.\n",length);
                        step = FIND_AA;
                    }
					else
					{
                        sum = 0;
                        step = FIND_VER;
                    }
                }
                break;

            case FIND_VER:
                pktBuff[pktLen++] = data;
                len = 0;
                sum += data;
                step = FIND_SERNUM;
                break;

            case FIND_SERNUM:
                pktBuff[pktLen++] = data;
                sum += data;
                if (2 == ++len) 
				{
                    step = FIND_CMD;
                }
                break;

            case FIND_CMD:
                pktBuff[pktLen++] = data;
                sum += data;
                if (4 > length) 
				{
                    CL_LOG("length=%d,error.\n",length);
                    if (FIND_AA == data) 
					{
                        step = FIND_55;
                        pktBuff[0] = 0xAA;
                        pktLen = 1;
                    }
					else
					{
                        step = FIND_AA;
                    }
                }
				else
				{
                    len = length - 4;
                    step = len ? RX_DATA : FIND_CHK;
                }
                break;

            case RX_DATA:
                pktBuff[pktLen++] = data;
                sum += data;
                if (1 == --len) 
				{
                    step = FIND_CHK;
                }
                break;

            case FIND_CHK:
                pktBuff[pktLen++] = data;
                if (data == sum)
				{
                    gChgInfo.netLedStatus = 1;
                    RecvServerData(pktBuff, pktLen);
                }
				else
				{
                    CL_LOG("recv data checksum error,sum=%#x,pkt sum=%#x.\n",sum,data);
                }
                step = FIND_AA;
                break;

            default:
                step = FIND_AA;
                break;
        }
    }
}


void ServerTask(void)
{
    memset(&gChgInfo, 0, sizeof(gChgInfo));
    gChgInfo.authReqSn = 0xf0;
    gChgInfo.startChargingNoticeSn = 0xf0;
    gChgInfo.stopChargingNoticeSn = 0xf0;
    FIFO_S_Init(&gSocketPktRxCtrl, (void*)gGprsRxBuff, sizeof(gGprsRxBuff));
    gQueueInfo.queueCnt = QUEUE_BUFF_CNT;
    gQueueInfo.writeIndex = 0;
    gQueueInfo.readIndex = 0;
    gQueueInfo.queue[0] = gProtoRecvBuff0;
    gQueueInfo.queue[1] = gProtoRecvBuff1;
    gQueueInfo.queue[2] = gProtoRecvBuff2;
    gQueueInfo.queue[3] = gProtoRecvBuff3;

    while (1) 
	{
        vTaskDelay(100);
        //if ((0 != system_info.server_ip[0]) && (0 != system_info.server_port)) 
        #if 0
		if((0 != memcmp(system_info.server_ip, NET_SERVER_IP, strlen(NET_SERVER_IP))) || 
			(NET_SERVER_PORT != system_info.server_port))
		{
			
			CL_LOG("server_ip	= %s.\n", system_info.server_ip);
			CL_LOG("server_port = %d.\n", system_info.server_port);
			CL_LOG("server_ip与宏定义的不相同\n");
			memcpy(system_info.server_ip, NET_SERVER_IP, strlen(NET_SERVER_IP));
			system_info.server_ip[strlen(NET_SERVER_IP)] = 0;
    		system_info.server_port = NET_SERVER_PORT;
		}
		#endif
        if (CL_FALSE == system_info.is_socket_0_ok) 
		{
            if (CL_OK == OpenNet()) 
			{
                system_info.is_socket_0_ok = CL_TRUE;
            }
        }
		else
		{
            ServerRecvProc();
        }
    }
}

int SendCsqCmd(void)
{
    int ret = CL_OK;

    if (NET_MODE_2G == system_info.localNetType)
    {
        GprsSendCmd("AT+CSQ\r","\r\nOK\r\n",100, 0);
    }
    else if (NET_MODE_4G_ME3630 == system_info.localNetType) 
    {
        Me3630SendCmd("AT+CSQ\r","OK",100, 0);
    }
    #if USE_ME3610_4G_NET_MODE
    else if (NET_MODE_4G_ME3610 == system_info.localNetType) 
    {
        Me3610SendCmd("AT+CSQ\r","OK",100, 0);
    }
    #endif
    else
    {
        ret = CL_FAIL;
    }
    return ret;
}

int SendIccidCmd(void)
{
    int ret = CL_OK;

    if (NET_MODE_2G == system_info.localNetType) 
    {
    	GprsSendCmd("AT+CCID\r","OK",100, 0);
    }
    else if (NET_MODE_4G_ME3630 == system_info.localNetType) 
    {
    	Me3630SendCmd("AT+ZGETICCID\r","OK",100, 0);
    }
    #if USE_ME3610_4G_NET_MODE
    else if (NET_MODE_4G_ME3610 == system_info.localNetType) 
    {
    	Me3610SendCmd("AT+ZGETICCID\r","OK",100, 0);
    }
    #endif
    else
    {
        ret = CL_FAIL;
    }
    return ret;
}

int NetModeTest(OUT_PKT_STR *pBuff)
{
    TEST_NET_MODE_STR *pNetModeTest = (void*)pBuff->data;

    SendCsqCmd();
    SendIccidCmd();
    vTaskDelay(200);
    pNetModeTest->result = 0;
    pNetModeTest->csq = gNetSignal;
    memcpy(pNetModeTest->ccid, system_info.iccid, ICCID_LEN);
	SendDebugPkt(0, (void*)pBuff, sizeof(TEST_NET_MODE_STR));
    return CL_OK;
}

void SetPcbCode(OUT_PKT_STR *pBuff)
{
    WRITE_PCB_REQ_STR *pWpcb = (void*)pBuff->data;
    WRITE_PCB_ACK_STR *pRpcb = (void*)pBuff->data;

    memcpy(system_info.pcbCode, pWpcb->code, sizeof(pWpcb->code));
    FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
    pRpcb->type = pWpcb->type;
    pRpcb->result = 0;
    SendDebugPkt(0, (void*)pBuff, sizeof(WRITE_PCB_ACK_STR));
}

void GetPcbCode(OUT_PKT_STR *pBuff)
{
    READ_PCB_ACK_STR *pRpcbAck = (void*)pBuff->data;

    memcpy(pRpcbAck->code, system_info.pcbCode, sizeof(system_info.pcbCode));
    SendDebugPkt(0, (void*)pBuff, sizeof(READ_PCB_ACK_STR));
}

void GetFwVersion(OUT_PKT_STR *pBuff)
{
    pBuff->data[1] = (uint8_t)(FW_VERSION>>8);
    pBuff->data[2] = (uint8_t)(FW_VERSION);
    pBuff->data[3] = (uint8_t)SUB_FW_VERSION;
    SendDebugPkt(0, pBuff, 4);
}

int ProcTerminal(OUT_PKT_STR *pBuff, uint16_t len)
{
    TERMINAL_STR *pTerMinal = (void*)pBuff->data;

    if (pTerMinal->cnt <= MAX_TERMINAL_NUM) 
	{
        memcpy(system_info.chargerInfo, pTerMinal->chargerInfo, pTerMinal->cnt*sizeof(CHARGER_STR));
        system_info.chargerCnt = pTerMinal->cnt;
        return CL_OK;
    }
	else
	{
        CL_LOG("cnt=%d,error.\n",pTerMinal->cnt);
        return CL_FAIL;
    }
}

void SetRtcTick(OUT_PKT_STR *pBuff)
{
    uint32_t time = (pBuff->data[0]<<24) | (pBuff->data[1]<<16) | (pBuff->data[2]<<8) | pBuff->data[3];

    SetRtcCount(time);
    pBuff->data[0] = 0;
    SendDebugPkt(0, (void*)pBuff, 1);
}

void GetRtcTick(OUT_PKT_STR *pBuff)
{
    uint32_t time = GetRtcCount();

    pBuff->data[0] = time>>24;
    pBuff->data[1] = time>>16;
    pBuff->data[2] = time>>8;
    pBuff->data[3] = time;
    SendDebugPkt(0, (void*)pBuff, 4);
}

int FlashTest(void)
{



    return CL_OK;
}

int SendDebugPkt(uint8_t nodeIndex, OUT_PKT_STR *pFrame, uint16_t len)
{
    pFrame->head.aa = 0xaa;
    pFrame->head.five = 0x55;
    pFrame->head.dest[0] = nodeIndex;
    pFrame->head.len = 4 + len;
    pFrame->head.ver = 1;
    pFrame->data[len] = GetPktSum((void*)&pFrame->head.ver, len + 3);
    UsartFlush(DEBUG_UART);
    return UsartSend(DEBUG_UART, (void*)pFrame, sizeof(OUT_NET_HEAD_STR) + len + 1);
}


int SetListenMode(OUT_PKT_STR *pBuff)
{
    int ret = CL_FAIL;
    LISTEN_STR *pListen = (void*)pBuff->data;

    if (LOCAL_NET != system_info.netType) 
	{
        CL_LOG("netType=%d,error.\n",system_info.netType);
        CL_LOG("设置结果ret=%d, 0:成功; 非0:失败.\n",ret);
        return ret;
    }

    if (1 == pListen->modeType) 
	{
        if (NET_MODE_2G == system_info.localNetType) 
		{
            if (1 == pListen->netType) 
			{
                CL_LOG("设置2g移动自动接听.\n");
                ret = GprsSendCmd("ATS0=1\r", "OK", 5000, 0);
            }
        }
    }
    else if (2 == pListen->modeType) 
    {
        if (NET_MODE_4G_ME3630 == system_info.localNetType) 
        {
            if (1 == pListen->netType) 
            {
                CL_LOG("设置3630移动自动接听.\n");
                ret = Me3630SendCmd("AT+ZMBN=4\r","OK",5000, 0);         //测试GSM+TD-LTE
            }
            else if (3 == pListen->netType) 
            {
                CL_LOG("设置3630联通自动接听.\n");
                ret = Me3630SendCmd("AT+ZMBN=5\r","OK",5000, 0);         //WCDMA+FDD LTE
            }
            else if (2 == pListen->netType) 
            {
                CL_LOG("设置3630电信自动接听.\n");
                ret = Me3630SendCmd("AT+ZMBN=8\r","OK",5000, 0);         //CDMA+EVDO
            }
        }
        #if USE_ME3610_4G_NET_MODE
        else if (NET_MODE_4G_ME3610 == system_info.localNetType) 
        {
            if (2 == pListen->netType) 
            {
                CL_LOG("设置3610电信自动接听.\n");
                ret = Me3610SendCmd("AT+ZMBN=8\r","OK",5000, 0);         //CDMA+EVDO
            }
        }
        #endif
    }
    CL_LOG("设置结果ret=%d, 0:成功; 非0:失败.\n",ret);
    return ret;
}

void LedTest(OUT_PKT_STR *pBuff)
{
    int result = 1;
    LED_REQ_STR *pLedReq = (void*)pBuff->data;

    switch (pLedReq->ledSn) {
        case 0:
            pLedReq->status ? LED_BLUE_LIGHT_ON() : LED_BLUE_LIGHT_OFF();
            result = 0;
            break;
        case 1:
            pLedReq->status ? LED_GREEN_LIGHT_ON() : LED_GREEN_LIGHT_OFF();
            result = 0;
            break;
        case 2:
            pLedReq->status ? LED_RED_LIGHT_ON() : LED_RED_LIGHT_OFF();
            result = 0;
            break;
    }
    pBuff->data[0] = result;
    SendDebugPkt(0, (void*)pBuff, 1);
}

void NetConnect(OUT_PKT_STR *pBuff)
{
    #if 1
    pBuff->data[0] = system_info.isRecvStartUpAck ? 0 : 1;
    #else
	if((system_info.isRecvStartUpAck == 1) && (system_info.is_socket_0_ok == 1))
	{
		pBuff->data[0] = 0;
	}
	else
	{
		pBuff->data[0] = 1;
	}
    #endif
    SendDebugPkt(0, (void*)pBuff, 1);
}

void SetMode4gTo2g(OUT_PKT_STR *pBuff)
{
    int ret = CL_FAIL;

    if (LOCAL_NET != system_info.netType) 
    {
        CL_LOG("netType=%d,设置结果ret=%d, 0:成功; 非0:失败.\n",system_info.netType,ret);
        return;
    }

    if (NET_MODE_4G_ME3630 == system_info.localNetType) 
    {
        CL_LOG("设置3630 4g to 2g.\n");
        ret = Me3630SendCmd("ata\r","OK",5000, 0);
    }
    #if USE_ME3610_4G_NET_MODE
    else if (NET_MODE_4G_ME3610 == system_info.localNetType) 
    {
        CL_LOG("设置3610 4g to 2g.\n");
        ret = Me3610SendCmd("ata\r","OK",5000, 0);
    }
    #endif
    CL_LOG("设置结果ret=%d, 0:成功; 非0:失败.\n",ret);
}

void  GetChargerSn(OUT_PKT_STR *pBuff)
{
    memcpy(pBuff->data, system_info1.station_id, sizeof(system_info1.station_id));
    SendDebugPkt(0, (void*)pBuff, sizeof(system_info1.station_id));
}

void GetChargerDeviceId(OUT_PKT_STR *pBuff)
{
    memcpy(pBuff->data, system_info.idCode, sizeof(system_info.idCode));
    SendDebugPkt(0, (void*)pBuff, sizeof(system_info.idCode));
}

void TestStatus(OUT_PKT_STR *pBuff)
{
    system_info.printfSwitch = pBuff->data[0] ? 1 : 0;
	if(pBuff->data[0] == 0)
	{
		CL_LOG("打开上位机检测!\n");
		pBuff->data[1] = 0;
		gChgInfo.ProductToolTestFlag = 0xa5;
	}
	else
	{
		CL_LOG("关闭上位机检测!\n");
		pBuff->data[1] = 1;
		gChgInfo.ProductToolTestFlag = 0;
	}
    SendDebugPkt(0, (void*)pBuff, 2);
}

int QueListenMode(OUT_PKT_STR *pBuff)
{
    int ret = CL_OK;
    LISTEN_STR *pListen = (void*)pBuff->data;

    if (LOCAL_NET != system_info.netType) 
	{
        CL_LOG("netType=%d,error.\n",system_info.netType);
        return CL_FAIL;
    }

    if (1 == pListen->modeType) 
	{
        if (NET_MODE_2G == system_info.localNetType) 
		{
            if (1 == pListen->netType) 
			{
                ret = CL_OK;
                //发送自动接听指令
            }
        }
    }
    else if (2 == pListen->modeType) 
    {
        if (NET_MODE_4G_ME3630 == system_info.localNetType) 
        {
            if (1 == pListen->netType) 
            {
                CL_LOG("查询3630移动自动接听.\n");
                ret = Me3630SendCmd("AT+ZMBN=?\r","OK",5000, 0);
            }
            else if (3 == pListen->netType) 
            {
                CL_LOG("查询3630联通自动接听.\n");
                ret = Me3630SendCmd("AT+ZMBN=?\r","OK",5000, 0);
            }
            else if (2 == pListen->netType) 
            {
                CL_LOG("查询3630电信自动接听.\n");
                ret = Me3630SendCmd("AT+ZMBN=?\r","OK",5000, 0);
            }
            else
            {
                ret = CL_FAIL;
            }
        }
        #if USE_ME3610_4G_NET_MODE
        else if (NET_MODE_4G_ME3610 == system_info.localNetType) 
        {
            if (2 == pListen->netType) 
            {
                CL_LOG("查询3630电信自动接听.\n");
                ret = Me3610SendCmd("AT+ZMBN=?\r","OK",5000, 0);         //CDMA+EVDO
            }
            else 
            {
                ret = CL_FAIL;
            }
        }
        #endif
    }
    else 
    {
        ret = CL_FAIL;
    }
    CL_LOG("查询结果ret=%d, 0:成功; 1:失败.\n",ret);
    return ret;
}


void DebugPktProc(OUT_PKT_STR *pBuff, uint16_t len)
{
    uint8_t writeFlash = 1;
    int ret = CL_OK;
	uint8_t gateWayAddr[MAC_LEN];	//2.4网关地址
	
    switch (pBuff->head.cmd) 
	{
		case TEST_CMD_2G:
            NetModeTest(pBuff);
            return;
        case TEST_CMD_PCB_CODE:
            SetPcbCode(pBuff);
            return;
        case DEBUG_CMD_SN:
            memcpy(system_info1.station_id, pBuff->data, sizeof(system_info1.station_id));
            memset(system_info.idCode, 0, sizeof(system_info.iccid));
            
			writeFlash = 2;
            break;
		case TEST_CMD_BLE:
			pBuff->data[0] = gChgInfo.isBlueToothInit;
			SendDebugPkt(0, pBuff, 1);
			return;
		case DEBUG_CMD_24G_COMMUNICATION:		//24g通信测试
			if((0xa5 == gChgInfo.ReceiveR624gDataFlag))
			{
				gChgInfo.ReceiveR624gDataFlag = 0;
				pBuff->data[0] = 1;
			}
			else
			{
				pBuff->data[0] = 0;
			}
			SendDebugPkt(0, pBuff, 1);
		case TEST_CMD_FW_VERSION:
            GetFwVersion(pBuff);
            return;
        case TEST_CMD_READ_PCB:
            GetPcbCode(pBuff);
            return;
        case DEBUG_CMD_DEV_TYPE:
            break;
        case DEBUG_CMD_GW_ADDR:
			PrintfData("网关地址", system_info1.gwAddr, sizeof(system_info1.gwAddr));
            memcpy(gateWayAddr, pBuff->data, sizeof(system_info1.gwAddr));
			system_info1.gwAddr[0] = gateWayAddr[4];
			system_info1.gwAddr[1] = gateWayAddr[3];
			system_info1.gwAddr[2] = gateWayAddr[2];
			system_info1.gwAddr[3] = gateWayAddr[1];
			system_info1.gwAddr[4] = gateWayAddr[0];
            writeFlash = 2;
			if(gChgInfo.ProductToolTestFlag == 0xa5)
			{
				gChgInfo.ProductToolTestFlag = 0;
				gBlueCtrl.resetMode = 1;
			}
            break;
        case DEBUG_CMD_TER_MINAL:
            ret = ProcTerminal(pBuff, len);
            writeFlash = ret;
            break;
        case DEBUG_CMD_SERVER_ADDR:
            if (sizeof(system_info.server_ip) > strlen((void*)pBuff->data)) 
			{
                memcpy(system_info.server_ip, pBuff->data, strlen((void*)pBuff->data));
                system_info.server_ip[strlen((void*)pBuff->data)] = 0;
                writeFlash = 0;
            }
            break;
        case DEBUG_CMD_SERVER_PORT:
            system_info.server_port = (pBuff->data[1]<<8) | pBuff->data[0];
            writeFlash = 0;
            break;
        case DEBUG_CMD_PRINTF:
            system_info.printfSwitch = pBuff->data[0];
            writeFlash = 0;
            break;
        case DEBUG_CMD_REBOOT:
            SendDebugPkt(0, pBuff, 1);
            vTaskDelay(1000);
            NVIC_SystemReset();
            break;
        case DEBUG_CMD_DEVICE_ID:
            system_info.idCode[0] = 0;
            system_info.idCode[1] = 0;
            memcpy(&system_info.idCode[2], pBuff->data, 6);
            writeFlash = 0;
            break;
		case DEBUG_CMD_SET_RTC:
            SetRtcTick(pBuff);
            return;
        case DEBUG_CMD_RED_RTC:
            GetRtcTick(pBuff);
            return;
        case DEBUG_CMD_FLASH:
            pBuff->data[0] = FlashTest();
            SendDebugPkt(0, (void*)pBuff, 1);
            return;
        case DEBUG_CMD_SET_LISTEN_MODE:
            system_info.printfSwitch = 1;
            ret = SetListenMode(pBuff);
            break;
        case DEBUG_CMD_QUE_LISTEN_MODE:
            system_info.printfSwitch = 1;
            ret = QueListenMode(pBuff);
            break;
		case DEBUG_CMD_LED:
            LedTest(pBuff);
            return;
        //case DEBUG_CMD_485:
         //   Com485Test(pBuff);
         //   return;
        case DEBUG_CMD_NET_CONNECT:
            NetConnect(pBuff);
            return;
        case DEBUG_CMD_4G_TO_2G:
            SetMode4gTo2g(pBuff);
            return;
        case DEBUG_CMD_GET_CHARGER_SN:
            GetChargerSn(pBuff);
            return;
        case DEBUG_CMD_GET_DEVICE_ID:
            GetChargerDeviceId(pBuff);
            return;
        //case START_AGE_TEST:
         //   StartAgeTest(pBuff);
        //    return;
        //case GET_AGE_TEST_RESULT:
        //    GetAgeTestResult(pBuff);
        //    return;
        case TEST_CMD_STATUS:
            TestStatus(pBuff);
            return;
    }

    if (0 == writeFlash) 
	{
        FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
		//FlashWriteSysInfo1(&system_info1, sizeof(system_info1), 1);
    }
	else
	{
		FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
		FlashWriteSysInfo1(&system_info1, sizeof(system_info1), 1);
	}
    pBuff->data[0] = ret;
    SendDebugPkt(0, pBuff, 1);
}


void DebugRecvProc(void)
{
    static uint8_t  step = FIND_AA;
    static uint8_t  buff[OUT_NET_PKT_LEN];
    static uint16_t pktLen;
    static uint16_t len;
    static uint8_t  sum;
    uint8_t data;
    uint8_t *pBuff = buff;

    while (CL_OK == UsartGetOneData(DEBUG_UART, &data))
	{
        switch (step) 
		{
            case FIND_AA:
                if (0xaa == data) 
				{
                    pktLen = 0;
                    pBuff[pktLen++] = data;
                    step = FIND_55;
                }
                break;

            case FIND_55:
                if (0x55 == data) 
				{
                    step = FIND_SRC;
                    pBuff[pktLen++] = data;
                    len = 0;
                }
				else 
				{
                    step = FIND_55;
                }
                break;

            case FIND_SRC:
                pBuff[pktLen++] = data;
                if (5 <= ++len) 
				{
                    step = FIND_DEST;
                    len = 0;
                }
                break;

            case FIND_DEST:
                pBuff[pktLen++] = data;
                if (5 <= ++len) 
				{
                    step = FIND_LEN;
                    len = 0;
                }
                break;

            case FIND_LEN:
                pBuff[pktLen++] = data;
                if (2 <= ++len) 
				{
                    len = (pBuff[pktLen-1]<<8) | pBuff[pktLen-2];
                    if (sizeof(buff) <= len) 
					{
                        CL_LOG("len=%d,error.\n",len);
                        step = FIND_AA;
                    }
					else
					{
                        step = FIND_VER;
                        sum = 0;
                    }
                }
                break;

            case FIND_VER:
                pBuff[pktLen++] = data;
                sum += data;
                step = FIND_SERNUM;
                break;

            case FIND_SERNUM:
                pBuff[pktLen++] = data;
                sum += data;
                step = FIND_CMD;
                break;

            case FIND_CMD:
                pBuff[pktLen++] = data;
                sum += data;
                len = (len >= 4) ? (len - 4) : 0;
                step = len ? RX_DATA : FIND_CHK;
                break;

            case RX_DATA:
                pBuff[pktLen++] = data;
                sum += data;
                if (0 == --len) 
				{
                    step = FIND_CHK;
                }
                break;

           case FIND_CHK:
                if (sum == data) 
				{
                    PrintfData("DebugRecvProc", pBuff, pktLen);
                    system_info.printfSwitch = 0;
                    DebugPktProc((OUT_PKT_STR*)pBuff, pktLen);
                }
				else
				{
                    CL_LOG("sum=%02x,pkt sum=%02x,error,drop pkt.\n",sum,data);
                }
                step = FIND_AA;
                break;
        }
    }
}


void PktInQueue(PKT_STR *pPkt, uint8_t gunId)
{
    uint8_t  msgCnt;
    QUEUE_STR *pQueue = &gQueueInfo;
    MSG_STR *pMsg = NULL;
	static uint32_t CheckTimes = 0;

	while(1)
	{
		if (pQueue->writeIndex >= pQueue->readIndex)
		{
			msgCnt = pQueue->writeIndex - pQueue->readIndex + 1;
		}
		else
		{
			msgCnt = pQueue->writeIndex + pQueue->queueCnt + 1 - pQueue->readIndex;
		}
		if ((QUEUE_BUFF_CNT <= msgCnt) && (CheckTimes++ > 150))
		{
			CheckTimes = 0;
			CL_LOG("msgCnt=%d,error, writeIndex = %d, readIndex = %d.\n",msgCnt, pQueue->writeIndex, pQueue->readIndex);
			return;
		}
		else if (QUEUE_BUFF_CNT > msgCnt)
		{
			CheckTimes = 0;
			break;
		}
		vTaskDelay(10);
	}
	
    pMsg = (void*)pQueue->queue[pQueue->writeIndex];
    pMsg->gunId = gunId;
    pMsg->cmd = pPkt->head.cmd;
    CL_LOG("保存收到的后台鉴权结果, 待会发送pMsg->gunId = %d, pMsg->cmd = %d \n", pMsg->gunId, pMsg->cmd);
    CL_LOG("msgCnt = %d, len = %d!\n", msgCnt, pPkt->head.len);
    memcpy(pMsg->data, pPkt->data, pPkt->head.len);

    if (QUEUE_BUFF_CNT <= ++pQueue->writeIndex)
	{
        //CL_LOG("写指针清零!写指针清零!\n");
        pQueue->writeIndex = 0;
    }
}


