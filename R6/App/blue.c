
#include "includes.h"
#include "blue.h"
#include "bluetooth.h"
#include "led.h"


BLUE_CTRL_STR gBlueCtrl;
uint8_t gBuleRxData[OUT_NET_PKT_LEN];
uint8_t gBuleSendBuff[OUT_NET_PKT_LEN];
uint8_t gRouteRxData[OUT_NET_PKT_LEN];
uint8_t gBtRxData[64];


//2.4G测试地址 9999999999
void Send24GTestPkt(void)
{
    uint8_t  buff[20];
    BLUE_PKT_STR *pPkt = (void*)buff;

    memset(pPkt->head.addr, 0x99, 5);
    pPkt->head.addr[5] = 0;
    SendBluePkt(NODE_24G, pPkt, 10);
}


void ReqX6StartCharging(SYS_PKT_STR *pBuff, MSG_STR *pMsg, uint8_t SendFlag)
{
    uint32_t i = 0;
    REQ_START_CHARGING_STR *pStartCharging = (void*)pBuff->data;
    START_CHARGING_REQ_STR *pStartChargingReq = (void*)pMsg->data;

    pBuff->x6Head.cmd = CMD_REMOTE_SET_POWER_ON;
    pStartCharging->gunId = pMsg->gunId;
    pStartCharging->mode = pStartChargingReq->mode;
    pStartCharging->para = pStartChargingReq->chargingPara;
    memcpy(pStartCharging->orderId, pStartChargingReq->order, sizeof(pStartCharging->orderId));
    if (1 == gChgInfo.reqX6StartCharging)
    {
        gChgInfo.reqX6StartCharging = 0;
        return;
    }
    gChgInfo.reqX6StartCharging = 0;
    for (i = 0; i<(RESENT_24G_MSG_CNT*RETRY_24G_MAX_CNT*3); i++)
	{
        if (0 == (i % RESENT_24G_MSG_CNT))
		{
            SendX6Pkt((pMsg->gunId-1)>>1, system_info.x6SetPowerOnPktSn, (void*)&pBuff->x6Head, sizeof(REQ_START_CHARGING_STR));
            CL_LOG("StartCharging SendFlag = %d \n", SendFlag);
            PrintfData("ReqX6StartCharging, 给X6发送开启充电请求", (void*)pBuff, sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(REQ_START_CHARGING_STR));
            if(ONE_TIMES == SendFlag)
            {
                return;
            }
        }
        vTaskDelay(RETRY_24G_DELAY_TIME);
        if (1 == gChgInfo.reqX6StartCharging)
		{
			gChgInfo.reqX6StartCharging = 0;
            break;
        }
    }
    system_info.x6SetPowerOnPktSn++;
}

void ReqX6StopCharging(SYS_PKT_STR *pBuff, MSG_STR *pMsg, uint8_t SendFlag)
{
    uint32_t i = 0;
    REQ_STOP_CHARGING_STR *pStopCharging = (void*)pBuff->data;

    pBuff->x6Head.cmd = CMD_REMOTE_SET_POWER_OFF;
    pStopCharging->gunId = pMsg->gunId;
    pStopCharging->reason = STOP_PHONE;
    if (1 == gChgInfo.reqX6StopCharging)
    {
        gChgInfo.reqX6StopCharging = 0;
        return;
    }
    gChgInfo.reqX6StopCharging = 0;
    for (i = 0; i < (RESENT_24G_MSG_CNT*RETRY_24G_MAX_CNT*5); i++)
	{
        if (0 == (i % RESENT_24G_MSG_CNT))
		{
            SendX6Pkt((pMsg->gunId-1)>>1, system_info.x6SetPowerOffPktSn, (void*)&pBuff->x6Head, sizeof(REQ_STOP_CHARGING_STR));
            PrintfData("ReqX6StopCharging给X6发送停止充电请求", (void*)pBuff, sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(REQ_STOP_CHARGING_STR));
            CL_LOG("StopCharging SendFlag = %d \n", SendFlag);
            if(ONE_TIMES == SendFlag)
            {
                return;
            }
        }
        vTaskDelay(RETRY_24G_DELAY_TIME);
        if (1 == gChgInfo.reqX6StopCharging) 
		{
			gChgInfo.reqX6StopCharging = 0;
            break;
        }
    }
    system_info.x6SetPowerOffPktSn++;
}


void X6AuthResult(SYS_PKT_STR *pBuff, MSG_STR *pMsg, uint8_t SendFlag)
{
    uint32_t i = 0;
    X6_AUTH_RESULT_REQ_STR *pX6AuthResultReq = (void*)pBuff->data;
    CARD_AUTH_ACK_STR *pAuthAck = (void*)pMsg->data;

    pBuff->x6Head.cmd = CMD_AUTH_RESULT;
    pX6AuthResultReq->gunId = pMsg->gunId;
    pX6AuthResultReq->result = pAuthAck->result;
    pX6AuthResultReq->cardType = pAuthAck->cardType;
    pX6AuthResultReq->lefeMoney = pAuthAck->user_momey;
    memcpy(pX6AuthResultReq->orderId, pAuthAck->order, sizeof(pX6AuthResultReq->orderId));
	
    if (1 == gChgInfo.authAckRecvFlag)
    {
        gChgInfo.authAckRecvFlag = 0;
        return;
    }
    gChgInfo.authAckRecvFlag = 0;
    for (i = 0; i < (RESENT_24G_MSG_CNT*RETRY_24G_MAX_CNT*2); i++)
    {
        if (0 == (i % RESENT_24G_MSG_CNT))
        {
            SendX6Pkt((pMsg->gunId-1)>>1, system_info.x6AuthResultPktSn, (void*)&pBuff->x6Head, sizeof(X6_AUTH_RESULT_REQ_STR));
            PrintfData("X6AuthResult, 给X6发送后台鉴权卡的结果", (void*)pBuff, sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(X6_AUTH_RESULT_REQ_STR));
            CL_LOG("AuthResult SendFlag = %d \n", SendFlag);
            if(ONE_TIMES == SendFlag)
            {
                return;
            }
        }
        vTaskDelay(RETRY_24G_DELAY_TIME);
        if (1 == gChgInfo.authAckRecvFlag)
		{
			gChgInfo.authAckRecvFlag = 0;
            break;
        }
    }
    system_info.x6AuthResultPktSn++;
}


int SendX6ReqPkt(SYS_PKT_STR *pBuff, uint8_t SendFlag)
{
    QUEUE_STR *pQueue = &gQueueInfo;
    MSG_STR *pMsg = (void*)pQueue->queue[pQueue->readIndex];

    if (pQueue->writeIndex == pQueue->readIndex) 
	{
        return CL_FAIL;
    }
	CL_LOG("writeIndex = %d, readIndex = %d\n", pQueue->writeIndex, pQueue->readIndex);
    switch (pMsg->cmd)
	{
        case MQTT_CMD_CARD_ID_REQ:
            X6AuthResult(pBuff, pMsg, SendFlag);
			//vTaskDelay(2000);
            break;
        case MQTT_CMD_REMOTE_SET_POWER_ON:
            ReqX6StartCharging(pBuff, pMsg, SendFlag);
			//vTaskDelay(2000);
            break;
        case MQTT_CMD_REMOTE_SET_POWER_OFF:
            ReqX6StopCharging(pBuff, pMsg, SendFlag);
			//vTaskDelay(2000);
            break;
    }
	if(MANY_TIMES == SendFlag)
	{
		if (QUEUE_BUFF_CNT <= ++pQueue->readIndex)
		{
			pQueue->readIndex = 0;
		}
	}
	
    return CL_OK;
}


void ShowNetStatistics(void)
{
    int i;
    TERMINAL_STATUS_STR *pStatus = gChgInfo.terminalStatus;

    CL_LOG("=============== send and recv info ===========\n");
    for (i=0; i<system_info.chargerCnt; i++) {
        PrintfData("ShowNetStatistics", system_info.chargerInfo[i].chargerSn, sizeof(system_info.chargerInfo[i].chargerSn));
        CL_LOG("node=%d,status=%d,send=%d,recv=%d,ok rate=%02f.\n",i,pStatus->heartBeatStatus,pStatus->sendCnt,pStatus->recvCnt,(float)pStatus->recvCnt/(float)pStatus->sendCnt);
        pStatus++;
    }
}


//枚举2.4G x6节点
void Blue24GNetProc(uint32_t second)
{
    LOOP_SEND_STR *pLoopSend = &gChgInfo.loopSend;
    SYS_PKT_STR  sysPkt;
    uint8_t gunId = GetGunIdByTerminalId((uint8_t)(pLoopSend->nodeIndex));
    TERMINAL_STATUS_STR *pStatus = &gChgInfo.terminalStatus[pLoopSend->nodeIndex];

    if (2 != gBlueCtrl.resetMode) {
        return;
    }
    if ((0 == system_info.chargerCnt) || (MAX_TERMINAL_NUM < system_info.chargerCnt)) 
	{
        //CL_LOG("chargerCnt=%d,error.\n", system_info.chargerCnt);
        return;
    }

	//if(1 != GetBindReqFlag())	//确定R6X6绑定关系时暂时不发送心跳包
	{
		if (0 == (second % 0x05))
		{
			CL_LOG("24g发送心跳包 gunId = %d \n", gunId);
			SendX6HeartBeatReqPkt((void*)&sysPkt.x6Head, gunId);
			pStatus->sendCnt++;
			if (DETECT_SEND_PKT_CNT < pStatus->sendCnt)
			{
				pStatus->heartBeatStatus = 1;
				CL_LOG("terminal %d change to error.\n",pLoopSend->nodeIndex);
				pStatus->sendCnt = 0;
			}
	
			if (system_info.chargerCnt <= ++pLoopSend->nodeIndex) {
				pLoopSend->nodeIndex = 0;
			}
		}
	}
}


void Recv24gData(void)
{
	uint8_t BlueRouteRxData[OUT_NET_PKT_LEN];
    uint8_t  *pBuff = &BlueRouteRxData[sizeof(BLUE_HEAD_STR)];
    static uint8_t  len;
    static uint16_t pktLen;
    uint8_t  data;
    static uint8_t  step = FIND_24G_EE;
    static uint8_t  sum;
    static uint32_t time;
    uint8_t pktNum = 0;
    #if 0
    if (1 != gBlueCtrl.resetMode)
	{
        if (system_info.chargerCnt)
		{
            if (300 < (uint32_t)(GetRtcCount() - gBlueCtrl.lastRecv24gTime))
			{
                gBlueCtrl.lastRecv24gTime = GetRtcCount();
                CL_LOG("blue recv break.\n");
                gBlueCtrl.resetMode = 1;
            }
        }
    }
	#endif
    if (FIND_24G_EE != step) 
	{
        if (4 < (GetRtcCount() - time)) 
		{
            step = FIND_24G_EE;
            CL_LOG("long time recv not complite,丢包.\n");
        }
		CL_LOG("FIND_24G_EE != step.\n");
    }

    while (CL_OK == FIFO_S_Get(&gBlueCtrl.rx24GBuff, &data)) 
	{
        time = GetRtcCount();
        switch (step) {
            case FIND_24G_EE:
                if (0xee == data) 
				{
                    pktLen = 0;
                    pBuff[pktLen++] = data;
                    step = FIND_24G_LEN;
                }
                break;

            case FIND_24G_LEN:
                pBuff[pktLen++] = data;
                len = data;
                if (sizeof(BlueRouteRxData) <= data) 
				{
                    CL_LOG("len=%d,error.\n",data);
                    step = FIND_24G_EE;
                }
				else
				{
                    step = FIND_24G_VER;
                    sum = 0;
                }
                break;

            case FIND_24G_VER:
                pBuff[pktLen++] = data;
                sum += data;
                step = FIND_24G_CMD;
                break;

            case FIND_24G_CMD:
                pBuff[pktLen++] = data;
                sum += data;
                len = (len >= 3) ? (len - 3) : 0;
                step = len ? FIND_24G_RX_DATA : FIND_24G_CHK;
                break;

            case FIND_24G_RX_DATA:
                pBuff[pktLen++] = data;
                sum += data;
                if (0 == --len) {
                    step = FIND_24G_CHK;
                }
                break;

           case FIND_24G_CHK:
                if (sum == data) 
				{
                    //PrintfData("RouteRecvTask", pBuff, pktLen);
                    gBlueCtrl.lastRecv24gTime = GetRtcCount();
                    RecvX6Data((void*)pBuff, pktLen);
                }
				else
				{
                    CL_LOG("sum=%02x,pkt sum=%02x,error,drop pkt.\n",sum,data);
                }
                step = FIND_24G_EE;
                if(RECEIVE_MAX_PKT_24G <= ++pktNum)
                {
                    pktNum = 0;
                    return;
                }
                break;
        }
    }
}


void BlueTerminal(BLUE_TERMINAL_STR *pTerminal)
{
    if (MAX_TERMINAL_NUM < pTerminal->num) 
	{
        return;
    }

    memcpy(system_info.chargerInfo[pTerminal->num].chargerSn, pTerminal->chargerSn, sizeof(pTerminal->chargerSn));
    //if (system_info.chargerCnt < pTerminal->num)
    {
        system_info.chargerCnt = pTerminal->num;
    }
    FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
}

//手机时间暂时不处理， pMsg前面预留有 BLUE_HEAD_STR 的内存
void ShakeReqProc(BLUE_PROTO_STR *pMsg)
{
    BLUE_SHAKE_ACK_STR *pAck = (void*)pMsg->data;

    memset(pAck, 0, sizeof(BLUE_SHAKE_ACK_STR));
    memcpy(pAck->name, CHARGER_NAME, strlen(CHARGER_NAME));
    memcpy(pAck->chargerSn, &system_info1.station_id[3], sizeof(pAck->chargerSn));
    pAck->fwVer = FW_VERSION;
    pAck->portCnt = system_info.chargerCnt << 1;
    pAck->startNo = 1;
    pMsg->head.len = sizeof(BLUE_SHAKE_ACK_STR);
    pMsg->data[pMsg->head.len] = GetPktSum((void*)pMsg, sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len);
    SendBluePkt(NODE_BLUE, (void*)((uint8_t*)pMsg-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len+1);
	PrintfData("回应蓝牙握手包", (void*)((uint8_t*)pMsg-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_HEAD_STR) + sizeof(BLUE_PROTO_HEAD_STR) + sizeof(BLUE_SHAKE_ACK_STR) + 4);
}


void BtSetChargerSn(BLUE_PROTO_STR *pMsg)
{
    memcpy(system_info1.station_id, pMsg->data, sizeof(system_info1.station_id));
    memset(system_info.idCode, 0, sizeof(system_info.idCode));
    FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
	FlashWriteSysInfo1(&system_info1, sizeof(system_info1), 1);
    pMsg->head.len = 1;
    pMsg->data[0] = 0;
    pMsg->data[pMsg->head.len] = GetPktSum((void*)pMsg, sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len);
    SendBluePkt(NODE_BLUE, (void*)((uint8_t*)pMsg-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len+1);
    CL_LOG("set charger sn, reboot system now.\n");
    vTaskDelay(1000);
    //NVIC_SystemReset();
}

void BtSetGateWayAddr(BLUE_PROTO_STR *pMsg)
{
	BLUE_2_4G_ADDR_STR *p2_4gAddr = (void*)pMsg->data;
	uint8_t CompareGWAddr[MAC_LEN];
	//uint8_t BlueToothResetFlag = 0;
	
	CompareGWAddr[0] = p2_4gAddr->GateWay24gAddr[4];
	CompareGWAddr[1] = p2_4gAddr->GateWay24gAddr[3];
	CompareGWAddr[2] = p2_4gAddr->GateWay24gAddr[2];
	CompareGWAddr[3] = p2_4gAddr->GateWay24gAddr[1];
	CompareGWAddr[4] = p2_4gAddr->GateWay24gAddr[0];
	
    PrintfData("蓝牙设置网关地址", (void*)p2_4gAddr->GateWay24gAddr, sizeof(system_info1.gwAddr));
    memcpy(system_info1.gwAddr, CompareGWAddr, sizeof(system_info1.gwAddr));
    FlashWriteSysInfo1(&system_info1, sizeof(system_info1), 1);
    pMsg->head.len = 1;
    pMsg->data[0] = 0;
    pMsg->data[pMsg->head.len] = GetPktSum((void*)pMsg, sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len);
    SendBluePkt(NODE_BLUE, (void*)((uint8_t*)pMsg-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len+1);
	CL_LOG("设置2.4g网关地址返回应答\n ");
    vTaskDelay(2000);
	if(0 != memcmp(system_info1.gwAddr, CompareGWAddr, sizeof(system_info1.gwAddr)))
	{
		gBlueCtrl.resetMode = 1;
	}
	
    //NVIC_SystemReset();
}

void BtSetChargerDeviceId(BLUE_PROTO_STR *pMsg)
{
    system_info.idCode[0] = 0;
    system_info.idCode[1] = 0;
    memcpy(&system_info.idCode[2], pMsg->data, sizeof(system_info.idCode));
    FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
    pMsg->head.len = 1;
    pMsg->data[0] = 0;
    pMsg->data[pMsg->head.len] = GetPktSum((void*)pMsg, sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len);
    SendBluePkt(NODE_BLUE, (void*)((uint8_t*)pMsg-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len+1);
    CL_LOG("set charger device id, reboot system now.\n");
    vTaskDelay(1000);
    NVIC_SystemReset();
}


void SendBtRemoCtrlAck(BLUE_PROTO_STR *pMsg, uint8_t result)
{
    REMO_CTRL_ACK_STR *pAck = (void*)pMsg->data;

    pAck->result = 0;
    pMsg->head.len = sizeof(REMO_CTRL_ACK_STR);
    pMsg->data[pMsg->head.len] = GetPktSum((void*)pMsg, sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len);
    SendBluePkt(NODE_BLUE, (void*)((uint8_t*)pMsg-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_PROTO_HEAD_STR)+pMsg->head.len+1);
}


void BtRemoCtrlReset(BLUE_PROTO_STR *pMsg)
{
    SendBtRemoCtrlAck(pMsg, CL_OK);
    vTaskDelay(1000);
    NVIC_SystemReset();
}


void BtRemoCtrlPrint(BLUE_PROTO_STR *pMsg)
{
    REMO_CTRL_REQ_STR *pReq = (void*)pMsg->data;

    system_info.printfSwitch = pReq->para;
    SendBtRemoCtrlAck(pMsg, CL_OK);
}


// pMsg前面预留有 BLUE_HEAD_STR 的内存
void RemoCtrlProc(BLUE_PROTO_STR *pMsg)
{
    REMO_CTRL_REQ_STR *pReq = (void*)pMsg->data;

    switch (pReq->optCode) {
        case SYSTEM_REBOOT:
            BtRemoCtrlReset(pMsg);
            break;
        case CTRL_SET_PRINT_SWITCH:
            BtRemoCtrlPrint(pMsg);
            break;
    }
}


// pPkt前面预留有 BLUE_HEAD_STR 的内存
void BluePktProc(BLUE_PROTO_STR *pPkt, uint8_t len)
{
	CL_LOG("蓝牙接收数据命令 pPkt->head.cmd = 0x%x\n", pPkt->head.cmd);
    switch (pPkt->head.cmd)
	{
        case BT_SHAKE_REQ:
            ShakeReqProc(pPkt);
        break;
        case BT_REQ_BREAK:
            CL_LOG("req break.\n");
        break;
        case BT_CMD_SET_CHARGER_SN:
            BtSetChargerSn(pPkt);
			CL_LOG("设置设备号.\n");
        break;
		case BT_CMD_SET_GW_ADDR:
            BtSetGateWayAddr(pPkt);
			CL_LOG("设置网关地址.\n");
        break;
        case BT_CMD_SET_DEVICE_ID:
            BtSetChargerDeviceId(pPkt);
        break;
        case BT_CMD_SET_TERMINAL_INFO:
            BlueTerminal((void*)pPkt);
        break;
        case BT_CMD_REMO_CTRL:
            RemoCtrlProc(pPkt);
        break;
		//case BT_CMD_SET_GW_ADDR:
			
        //break;
    }
}


//手机蓝牙数据接收处理
void RecvBtData(void)
{
	uint8_t BtRxData[64];
    uint8_t  *pBuff = &BtRxData[sizeof(BLUE_HEAD_STR)];
    static uint8_t  len;
    static uint8_t  pktLen;
    uint8_t  data;
    static uint8_t  step = BT_FIND_EE;
    static uint8_t  sum;

    while (CL_OK == FIFO_S_Get(&gBlueCtrl.rxBtBuff, &data))
	{
		//printf("B = [%02x]", data);
        switch (step)
        {
            case BT_FIND_EE:
                if (0xee == data)
                {
                    pktLen = 0;
                    pBuff[pktLen++] = data;
                    step = BT_FIND_CMD;
                    sum = 0xee;
                }
                break;

            case BT_FIND_CMD:
                pBuff[pktLen++] = data;
                sum += data;
                step = BT_FIND_LEN;
                break;

            case BT_FIND_LEN:
                pBuff[pktLen++] = data;
                sum += data;
                len = data;
                if (sizeof(gBtRxData) < len) 
				{
                    step = BT_FIND_EE;
                }
				else
				{
                    step = BT_RX_DATA;
                }
                break;

            case BT_RX_DATA:
                pBuff[pktLen++] = data;
                sum += data;
                if (0 == --len) 
				{
                    step = FIND_CHK;
                }
                break;

           case FIND_CHK:
                pBuff[pktLen++] = data;
                if (sum == data) 
				{
					PrintfData("RecvBtData蓝牙接收数据包", pBuff, pktLen);
                    BluePktProc((void*)pBuff, pktLen);
                }
				else
				{
                    PrintfData("RecvBtData, error", pBuff, pktLen);
                    CL_LOG("sum=%02x,pkt sum=%02x,error,drop pkt.\n",sum,data);
                }
                step = BT_FIND_EE;
                break;
        }
    }
}


void BlueTask(void)
{
    SYS_PKT_STR syspPkt;
    uint16_t second = 0;
    uint32_t old;
    uint32_t UpgradeTicks = 0;
    
    while (1)
	{
        vTaskDelay(100);
		DebugRecvProc();
        if (old != GetRtcCount())
		{
            old = GetRtcCount();
            Feed_WDT();
            #if 1
            Blue24GNetProc(second);
            #endif
            //if (0 == (second & 0x01f))
			//{
            //    ShowNetStatistics();
            //}
            //if (0 == (second % 2))
            {
                SendX6ReqPkt(&syspPkt, MANY_TIMES);
            }
            second++;
        }

        if((1 == gChgInfo.flagBit.Upgrade2_4Gflag) && (1 == gChgInfo.flagBit.isSendFwUpgradeOk))
        {
            if (GetRtcCount() - UpgradeTicks >= 5)
            {
                UpgradeTicks = GetRtcCount();
                Recv24gData();
                CL_LOG("gChgInfo.gun_id = %d \n", gChgInfo.gun_id);
                SendX6FwUpgradeDownload2_4gNotice(gChgInfo.gun_id);
            }
        }
        CheckUpgradeWaitTimer();
    }
}


void BlueRecvProc(void)
{
    uint8_t  *pktBuff = gBuleRxData;
    static uint8_t  step = BLUE_AB;
    static uint8_t  target;
    static uint16_t len;
    static uint8_t  sum;
    uint8_t  data;
    static uint16_t pktLen;
    BLUE_PKT_STR *pPkt = NULL;
	BLUETOOTH_PKT_RECEIVE_STR *pBluetoothPkt = NULL;
    static uint32_t time;
    uint8_t pktNum = 0;
    
    if (BLUE_AB != step) 
    {
        if (4 < (GetRtcCount() - time)) 
        {
            CL_LOG("long time recv not complite,drop pkt.\n");
            step = BLUE_AB;
        }
    }

    while (CL_OK == UsartGetOneData(BLUE_TOOTH, &data))
	{
        printf("H = %02x ", data);
        time = GetRtcCount();
        switch (step) 
		{
            case BLUE_AB:
                if (data == 0xAB) 
				{
                    step = BLUE_CD;
                    pktBuff[0] = 0xAB;
                    pktLen = 1;
                    sum = 0xAB;
                }
                break;

            case BLUE_CD:
                if (data == 0xcd) 
				{
                    step = BLUE_TARGET;
                    pktBuff[1] = 0xcd;
                    pktLen++;
                    sum += data;
                }
				else if (data == 0xAB) 
				{
                    step = BLUE_CD;
                    pktBuff[0] = 0xAB;
                    pktLen = 1;
                    sum = 0xAB;
                    CL_LOG("can not find cd.\n");
                }
				else
				{
                    step = BLUE_AB;
                    CL_LOG("can not find cd.\n");
                }
                break;

            case BLUE_TARGET:
                pktBuff[pktLen++] = data;
                sum += data;
                target = data;
                if (NODE_BLUE == data) 
				{
                    step = BLUE_LEN;
                }
				else
				{
                    step = BLUE_ADDR;
                }
                len = 0;
                break;

            case BLUE_ADDR:
                pktBuff[pktLen++] = data;
                sum += data;
                if (6 == ++len) 
				{
                    len = 0;
                    step = BLUE_LEN;
                }
                break;

            case BLUE_LEN:
                pktBuff[pktLen++] = data;
                sum += data;
                if (2 == ++len) 
				{
                    len = (pktBuff[pktLen-1]<<8) | pktBuff[pktLen-2];
                    if (OUT_NET_PKT_LEN < len) 
					{
                        step = BLUE_AB;
                        CL_LOG("len = %d,接收数据包的长度错误 \n", len);
                    }
					else
					{
                        step = BLUE_RX_DATA;
                    }
                }
                break;

            case BLUE_RX_DATA:
                pktBuff[pktLen++] = data;
                sum += data;
                if (0 == --len) 
				{
                    step = BLUE_CHK;
                }
                break;

            case BLUE_CHK:
                pktBuff[pktLen++] = data;
                if (data == sum) 
				{
                    gChgInfo.blueLedStatus = 1;
                    step = BLUE_END;
                    len = 0;
                }
				else
				{
                    CL_LOG("接收数据校验错误,sum=0x%x, data=0x%x.\n",sum,data);
                    step = BLUE_AB;
                }
                break;

            case BLUE_END:
                if (2 == ++len)
				{
                    if (NODE_24G == target)
					{
						pPkt = (void*)pktBuff;
                        //PrintfData("BlueTask 2.4G recv", pPkt->data, pPkt->head.len);
                        for (len=0; len<pPkt->head.len; ) 
						{
                            if (CL_OK == FIFO_S_Put(&gBlueCtrl.rx24GBuff, pPkt->data[len])) 
							{
                                len++;
                            }
							else
							{
                                CL_LOG("2.4g buff over flow error.\n");
                                vTaskDelay(2);
                            }
                        }
                    }
					else 
					{
                    	pBluetoothPkt = (void*)pktBuff;
                        for (len=0; len < pBluetoothPkt->head.len; )
						{
							printf("data= [%02x]", pBluetoothPkt->data[len]);
                            if (CL_OK == FIFO_S_Put(&gBlueCtrl.rxBtBuff, pBluetoothPkt->data[len]))
							{
                                len++;
                            }
							else
							{
                                PrintfData("BlueTask bt recv", pBluetoothPkt->data, pBluetoothPkt->head.len);
                                CL_LOG("bt buff over flow error.\n");
                                vTaskDelay(2);
                            }
                        }
                    }
                    step = BLUE_AB;
                    if(RECEIVE_MAX_PKT_24G <= ++pktNum)
                    {
                        pktNum = 0;
                        return;
                    }
                }
                break;

            default:
                step = BLUE_AB;
                break;
        }
    }
}


void PktRecvTask(void)
{
    uint32_t oldTime;

    gBlueCtrl.lastRecv24gTime = GetRtcCount();
    while (1)
	{
        gBlueCtrl.resetMode = 1;
        BuleReconnect();
        gBlueCtrl.resetMode = 2;
        
        while (1)
		{
            vTaskDelay(100);
			
            if (oldTime != GetRtcCount())
			{
                oldTime = GetRtcCount();
                Feed_WDT();
                if (1 == gBlueCtrl.resetMode)
				{
                    break;
                }
            }
			if(1 == gChgInfo.isBlueToothInit)
			{
				BlueRecvProc();
				Recv24gData();
				RecvBtData();
			}
        }
    }
}


int BlueInit(void)
{
    memset(&gBlueCtrl, 0, sizeof(gBlueCtrl));
    FIFO_S_Init(&gBlueCtrl.rx24GBuff, (void*)gRouteRxData, sizeof(gRouteRxData));
    FIFO_S_Init(&gBlueCtrl.rxBtBuff, (void*)gBtRxData, sizeof(gBtRxData));
    GPIO_Cfg(HT_GPIOD, GPIO_Pin_9|GPIO_Pin_11, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
    GPIO_Cfg(HT_GPIOD, GPIO_Pin_10, GPIO_Mode_IOIN, GPIO_Input_Up, GPIO_Output_PP);
	//GPRS_WAKEUP();
    BLUE_CD_LOW();
    return CL_OK;
}


