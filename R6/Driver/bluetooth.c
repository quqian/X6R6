/*cmu.c
* 2017-10-17
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "bluetooth.h"
#include "led.h"
#include "blue.h"
//#include "usart.h"

uint8_t gBlueSendMux = 0;
uint8_t gBlueRecvData[64];
uint8_t gBlueInitStep = 0;
uint8_t const gZeroArray[8] = {0};



const BlUE_INITTAB gBlueInitTab[]={
    {NULL,              "OK",   3000,   0,  BuleCheckReset},
    {"AT+ECHO=1\r\n",   "OK",   3500,   10, BuleModeEcho},
    {"AT\r\n",          "OK",   3000,   10, BuleModeTest},
    {"AT+DEVT=1\r\n",   "OK",   3000,   10, BuleSetDevt},
    {"AT+NAME?\r\n",    "OK",   3000,   10, BuleModeName},	
    {NULL,              "OK",   3000,   10, BuleSetName},
    {NULL,              "OK",   3000,   10, BuleSetGWMac},
    {"AT+PAIR?\r\n",    "OK",   3000,   10, BuleModePair},
    {"AT+CONS?\r\n",    "OK",   3000,   10, BuleModeCons},
    {"AT+VERS?\r\n",    "OK",   3000,   10, BuleModeVers},
    {"AT+GWID?\r\n",    "OK",   3000,   10, BuleModeAddr},
    {NULL,              "OK",   3000,   10, BuleSetX6Count},
    {"AT+BAUD?\r\n",    "OK",   3000,   10, BuleModeBaud},
    {"AT+ECHO?\r\n",    "OK",   3000,   10, BuleModeEcho},
    {"AT+DEVT?\r\n",    "OK",   3000,   10, BuleModeDevt},
    //{"AT+RSTF\r\n",   "OK",       3000,   10, BuleModeRstf},
    //{"AT+INFO?\r\n",  "OK",       5000,   10, BuleModeInfo},
};


void BuleReset(void)
{
	CL_LOG("复位蓝牙reset blue module.\n");
    BLUE_DISENABLE();
    vTaskDelay(2000);
    BLUE_ENABLE();
    vTaskDelay(2000);
}


//len为报文静荷长度
int SendBluePkt(uint8_t nodeType, BLUE_PKT_STR *pFrame, uint16_t len)
{
    int i = 0;
    
    MuxSempTake(&gBlueSendMux);
    BLUE_CD_HIGHT();
    //vTaskDelay(2);
    while (GET_BLUE_IRQ()) 
	{
        vTaskDelay(10);
        if (1000 <= ++i) 
		{
            CL_LOG("CKB24 wait idle timeout, 24G IRQ引脚没有拉低,错误.\n");
            MuxSempGive(&gBlueSendMux);
            return CL_FAIL;
        }
    }
    vTaskDelay(50);
    while (GET_BLUE_IRQ()) 
	{
        vTaskDelay(10);
        if (1000 <= ++i) 
		{
            CL_LOG("CKB24 wait idle timeout, 24G IRQ引脚没有拉低,错误.\n");
            MuxSempGive(&gBlueSendMux);
            return CL_FAIL;
        }
    }

    pFrame->head.ab = 0xab;
    pFrame->head.cd = 0xcd;
    pFrame->head.target = nodeType;
	//if(nodeType == NODE_24G)
    //{
    //    memcpy(pkt->head.addr, system_info._24gMacAddr, MAC_LEN);
    //}
    pFrame->head.len = len;
    pFrame->data[len] = GetPktSum((void*)pFrame, sizeof(BLUE_HEAD_STR)+len);
    pFrame->data[len+1] = '\r';
    pFrame->data[len+2] = '\n';
    UsartSend(BLUE_TOOTH, (void*)pFrame, sizeof(BLUE_HEAD_STR)+len+1+2);
    //PrintfData("SendBluePkt", (void*)pFrame, sizeof(BLUE_HEAD_STR)+len+3);
    MuxSempGive(&gBlueSendMux);
	//vTaskDelay(300);
	
    return CL_OK;
}

//告诉2.4G模块X6个数
int SetX6Count(uint8_t Count)
{
    uint32_t i = 0;
    char buf[32];
    memset(buf, 0, 32);
    sprintf(buf,"AT+CNT=%02d\r\n", Count);
    for (i = 0; i < 2; i++)
    {
        if (BlueSendCmd(buf, "OK", 2000) == CL_OK)
        {
            return CL_OK;
        }
		OS_DELAY_MS(2500);
    }
    return CL_FAIL;
}

//设置2.4G gwmac地址
int SetGateWay24GMacAddr(char *mac, uint8_t cmd)
{
    uint32_t i = 0;
    char buf[32];
    memset(buf, 0, 32);
    if(USER_CMD_ADDR == cmd)
    {
        sprintf(buf,"AT+ADDR=%s\r\n",mac);
    }
    else{
        sprintf(buf,"AT+GWID=%s\r\n",mac);
    }
    
    for (i = 0; i < 2; i++)
    {
        if (BlueSendCmd(buf, "OK", 2000) == CL_OK)
        {
            return CL_OK;
        }
		OS_DELAY_MS(1000);
    }
    return CL_FAIL;
}

//获取2.4G gwmac地址
int GetGateWay24GMacAddr(uint8_t cmd)
{
    uint32_t i = 0;
    char buf[32];
    memset(buf,0,32);
    if(USER_CMD_ADDR == cmd)
    {
        strcpy(buf, "AT+ADDR?\r\n");
    }
    else{
        strcpy(buf,"AT+GWID?\r\n");
    }
    
    for (i = 0; i < 2; i++)
    {
        if (BlueSendCmd(buf, "OK", 2000) == CL_OK)
        {
            return CL_OK;
        }
		OS_DELAY_MS(1000);
    }
    return CL_FAIL;
}

//获取蓝牙名称
int GetBlueName(void)
{
    uint32_t i = 0;
    char buf[32];
    memset(buf,0,32);
    //sprintf(buf,"AT+NAME?\r\n");
    strcpy(buf, "AT+NAME?\r\n");
    for (i = 0; i < 2; i++)
    {
        if (BlueSendCmd(buf, "OK", 2000) == CL_OK)
        {
            return CL_OK;
        }
		OS_DELAY_MS(1000);
    }
    return CL_FAIL;
}

int BlueCheckRes(char *cmd, char *res, uint16_t tmo, uint8_t *pbuff, uint16_t size)
{
    char *ret=NULL;
    uint8_t c;
    int cnt = 0;
    int retv = CL_FAIL;

    memset(pbuff, 0, size);
    //CL_LOG("gprs_check_res : recv data=");
    for(uint16_t time = 0; time < tmo; time+= 50)
    {
        vTaskDelay(50);
        while(UsartGetOneData(BLUE_TOOTH, &c) == 0)
        {
            #if BLUE_DEBUG
            printf("%02x ",c);
            #endif
            if (c)
            {
                if (size > cnt) 
				{
                    pbuff[cnt++] = c;
                }
				else
				{
					memset((void*)gUartPortAddr[BLUE_TOOTH].rxBuffCtrl.pStartAddr, 0, sizeof(UART0_RxBuff));
                    CL_LOG("cnt=%d,error.\n",cnt);
                }
            }
        }
        ret = strstr((const char*)pbuff, (const char*)res);
        if (ret)
        {
            if(strstr(cmd,"AT+NAME?"))
            {
                ret = strstr((const char*)pbuff, (const char*)"+NAME:");
                if(ret)
                {
                    //if((0x0d == pbuff[10]) && (0x0a == pbuff[11]))
                    {
                        CL_LOG("蓝牙名字1 = 0x%x.\n", pbuff[7]);
                        CL_LOG("蓝牙名字2 = 0x%x.\n", pbuff[8]);
                        CL_LOG("蓝牙名字3 = 0x%x.\n", pbuff[9]);
                        CL_LOG("蓝牙名字4 = 0x%x.\n", pbuff[10]);
                        CL_LOG("蓝牙名字5 = 0x%x.\n", pbuff[11]);
                        CL_LOG("蓝牙名字6 = 0x%x.\n", pbuff[12]);
                        CL_LOG("蓝牙名字7 = 0x%x.\n", pbuff[13]);
                        CL_LOG("蓝牙名字8 = 0x%x.\n", pbuff[14]);
                        CL_LOG("蓝牙名字9 = 0x%x.\n", pbuff[15]);
                        CL_LOG("蓝牙名字10 = 0x%x.\n", pbuff[16]);
                        CL_LOG("蓝牙名字11 = 0x%x.\n", pbuff[17]);
                        CL_LOG("蓝牙名字12 = 0x%x.\n", pbuff[18]);
                        CL_LOG("蓝牙名字 blue name:%s.\n",pbuff);
                        //memcpy(system_info.blue_name,&pbuff[sizeof("+NAME:")],10);
                        //CL_LOG("蓝牙名字 system_info.blue_name :%s.\n",system_info.blue_name);
                    }
                }
			}
            //获取网关mac
			if(strstr(cmd,"AT+GWID?"))
            {
				uint8_t num = sizeof("+GWID:");
				//memset(temp,0,20);
				//sscanf((char*)pbuff,"+ADDR: %s",temp);
				//StringToBCD(gwMac,temp);
                
				//CL_LOG("gwmac:%s.\n",temp);
                ret = strstr((const char*)pbuff, (const char*)"+GWID:");
                if(ret)
                {
                    //if((0x0d == pbuff[10]) && (0x0a == pbuff[11]))
                    {
                    #if 1
                        CL_LOG("24G网关地址1 = 0x%x.\n", pbuff[num]);
                        CL_LOG("24G网关地址2 = 0x%x.\n", pbuff[num + 1]);
                        CL_LOG("24G网关地址3 = 0x%x.\n", pbuff[num + 2]);
                        CL_LOG("24G网关地址4 = 0x%x.\n", pbuff[num + 3]);
                        CL_LOG("24G网关地址5 = 0x%x.\n", pbuff[num + 4]);
                        CL_LOG("24G网关地址6 = 0x%x.\n", pbuff[num + 5]);
                        CL_LOG("24G网关地址7 = 0x%x.\n", pbuff[num + 6]);
                        CL_LOG("24G网关地址8 = 0x%x.\n", pbuff[num + 7]);
                        CL_LOG("24G网关地址9 = 0x%x.\n", pbuff[num + 8]);
                        CL_LOG("24G网关地址10 = 0x%x.\n", pbuff[num + 9]);
                    #endif
                        CL_LOG("24G网关地址 :%s.\n",pbuff);
                        //StringToBCD(gwMac, &pbuff[num]);
                        //PrintfData("24G网关地址3", gwMac, 5);
                        //CL_LOG("24G网关地址  2:%s.\n",gwMac);
                    }
                }
			}
            retv = CL_OK;
            break;
        }
    }

    CL_LOG("<<< [%s].\n",pbuff);
    return retv;
}


int BlueSendCmd(char *cmd, char *ack, uint16_t waittime)
{
	uint8_t res = 1;

    BLUE_CD_LOW();
    vTaskDelay(2);
    //UsartFlush(BLUE_TOOTH);
    UsartSend(BLUE_TOOTH, (void*)cmd, strlen(cmd));

    #if BLUE_DEBUG
	CL_LOG(">>> %s.\n",cmd);
    #endif

    if ((ack==NULL) || (waittime==0)) 
	{
        BLUE_CD_HIGHT();
        return CL_OK;
    }

    if (CL_OK == BlueCheckRes(cmd, ack, waittime, gBlueRecvData, sizeof(gBlueRecvData))) 
	{
        res = CL_OK; /*check success, retrun 0*/
    }
    BLUE_CD_HIGHT();
	return res;
}


int BuleCheckReset(char ok, uint8_t retry)
#if 0
{
    BuleReset();
    for (int i=0; i<2; i++)
    {
        if (BlueSendCmd("AT+RSET\r\n", "OK", gBlueInitTab[gBlueInitStep].wait)==0)
        {
            CL_LOG("blue reset ok.\n");
            gBlueInitStep++;
            return CL_OK;
        }
        CL_LOG("blue reset fail,retry.\n");
        OS_DELAY_MS(4000);
    }
    CL_LOG("blue reset fail.\n");
    return CL_FAIL;
}
#else
{
    BuleReset();
	
    for (int i=0; i<2; i++)
    {
        if (BlueSendCmd("AT\r\n", "OK", gBlueInitTab[gBlueInitStep].wait)==0)
        {
            CL_LOG("blue test ok.\n");
            gBlueInitStep++;
            return CL_OK;
        }
        CL_LOG("blue reset fail,retry.\n");
        OS_DELAY_MS(2000);
    }
    CL_LOG("blue reset fail.\n");
    return CL_FAIL;
}
#endif

int CheckBlueModeCmd(char ok, uint8_t retry, uint16_t delay)
{
    if (ok==0) 
	{
        gBlueInitStep++;
        return CL_OK;
    }
	else if (retry > 10) 
	{
        gBlueInitStep = BLUE_RESET;
	}
    return CL_FAIL;
}


int BuleModeTest(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleSetDevt(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleSetName(char ok, uint8_t retry)
{
    char buff[32] = {0};
    char name[12] = {0};

    if (0 == memcmp(&system_info1.station_id[3], gZeroArray, 5)) 
	{
        CL_LOG("charger sn is all zero, no need to set blue name.\n");
        gBlueInitStep++;
        return CL_OK;
    }
    DeviceBcd2str(name, (void*)&system_info1.station_id[3], 5);
    if (0 == memcmp(gBlueCtrl.blueName, name, 10)) 
	{
        CL_LOG("name is ok, no need to set blue name.\n");
        gBlueInitStep++;
        return CL_OK;
    }
    sprintf(buff,"AT+NAME=%s\r\n", name);
    for (int i=0; i<4; i++) 
	{
        if (BlueSendCmd(buff, "OK", gBlueInitTab[gBlueInitStep].wait)==0) 
		{
            CL_LOG("set name=%s ok.\n",name);
            gBlueInitStep++;
            return CL_OK;
        }
        CL_LOG("set name=%s fail,retry.\n",name);
        OS_DELAY_MS(4000);
    }
    CL_LOG("set name fail.\n");
    return CL_FAIL;
}


int BuleModePair(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleModeCons(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}

int isArraryEmpty(uint8_t *array,int len)
{
	for(int i = 0;i<len ; i++){
		if(array[i] != 0x00){
			return CL_FAIL;
		}
	}
	return CL_OK;
}

int BuleSetX6Count(char ok, uint8_t retry)
{
	uint8_t Count = 0;

	CL_LOG("设置x6数目: 0x%x .\n", system_info.chargerCnt);
	if((0 == system_info.chargerCnt) || (0xff == system_info.chargerCnt))
	{
		Count = 3;
	}
	else
	{
		Count = system_info.chargerCnt;
	}
    if(CL_OK == SetX6Count(Count))
    {
        gBlueInitStep++;
        CL_LOG("set X6Count success.\n");
        return CL_OK;
    }
    else
	{
		CL_LOG("set X6Count fail.\n");
        gBlueInitStep = BLUE_RESET;
	}
    
    return CL_FAIL;
}


int BuleSetGWMac(char ok, uint8_t retry)
{
	CL_LOG("设置网关地址 \n");
    if(isArraryEmpty(system_info1.gwAddr, MAC_LEN) == CL_OK)
	{
		CL_LOG("_24gMacAddr is all zero, no need to set gw mac.\n");
		gBlueInitStep++;
        return CL_OK;
	}
	
	char str_mac[STR_MAC_LENTH];
    BCDToString(str_mac, system_info1.gwAddr, MAC_LEN);
	if(SetGateWay24GMacAddr(str_mac, USER_CMD_GWID) == CL_OK)
    {
		CL_LOG("set gwmac success.\n");
		gBlueInitStep++;
        return CL_OK;
	}
	CL_LOG("set gwmac fail.\n");
    return CL_FAIL;
}

int BuleModeName(char ok, uint8_t retry)
{
    char *pStr = NULL;
    char buff[40] = {0};

    CL_LOG("in.\n");
    if (CL_OK == CheckBlueModeCmd(ok, retry, 0)) {
        pStr = strstr((void*)gBlueRecvData, "NAME:");
        if (NULL != pStr) {
            sscanf(pStr, "NAME: %s",buff);
            memset(gBlueCtrl.blueName, 0, sizeof(gBlueCtrl.blueName));
            memcpy(gBlueCtrl.blueName, buff, 10);
        }
        return CL_OK;
    }
    return CL_FAIL;
}


int BuleModeVers(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleModeAddr(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleModeBaud(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleModeEcho(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleModeDevt(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleModeInfo(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleModeRstf(char ok, uint8_t retry)
{
    CL_LOG("in.\n");
    return CheckBlueModeCmd(ok, retry, 0);
}


int BuleReconnect(void)
{
	char ok;
    uint8_t retry = 0;
	gChgInfo.isBlueToothInit = 0;
	
	gBlueInitStep = BLUE_RESET;
	while(1) 
	{
        Feed_WDT();
        OS_DELAY_MS(gBlueInitTab[gBlueInitStep].nwait*10);
        if (gBlueInitTab[gBlueInitStep].cmd)
		{
            ok = BlueSendCmd(gBlueInitTab[gBlueInitStep].cmd,gBlueInitTab[gBlueInitStep].res,gBlueInitTab[gBlueInitStep].wait);
        }
        CL_LOG("gBlueInitStep = %d \n", gBlueInitStep);
        if (gBlueInitTab[gBlueInitStep].process) 
		{
            if (CL_OK == gBlueInitTab[gBlueInitStep].process(ok, retry)) 
			{
                retry = 0;
            }
			else
			{
                retry++;
            }
        }

		if (gBlueInitStep == BLUE_STATE_NUM) 
		{
			CL_LOG("blue init ok.\n");
			gChgInfo.isBlueToothInit = 1;
			return CL_OK;
		}
	}
}


