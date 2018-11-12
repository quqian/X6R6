/*proto.c
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "proto.h"
#include "blue.h"


uint8_t gProtoSendMux = 0;
uint8_t gProtoSendBuff[OUT_NET_PKT_LEN];
uint8_t  gSendHearBeatCnt = 0;



int GetPktSum(uint8_t *pData, uint16_t len)
{
    int i;
    uint8_t sum = 0;

    for (i=0; i<len; i++) {
        sum += pData[i];
    }
    return sum;
}


//pData 指向静荷 len静荷长度
int SendProtoPkt(uint16_t sn, uint8_t cmd, PKT_STR *pPkt, uint16_t len)
{
    uint8_t *pbuff = (void*)pPkt;

    if (gChgInfo.sendPKtFlag)
	{
        CL_LOG("sendPKtFlag=%d, can not send net pkt.\n",gChgInfo.sendPKtFlag);
        return CL_FAIL;
    }
    pPkt->head.aa = 0xaa;
    pPkt->head.five = 0x55;
    pPkt->head.type = CHARGER_TYPE;
    if (MQTT_CMD_REGISTER == cmd)
	{
        //CL_LOG("注册 MQTT_CMD_REGISTER.\n");
        memcpy(pPkt->head.chargerSn, system_info1.station_id, sizeof(pPkt->head.chargerSn));
    }
	else
	{
        memcpy(pPkt->head.chargerSn, system_info.idCode, sizeof(system_info.idCode));
    }
    pPkt->head.len = len + 5;
    pPkt->head.ver = 1;
    pPkt->head.sn = sn;
    pPkt->head.cmd = cmd;
    pbuff[sizeof(PKT_HEAD_STR) + len] = GetPktSum((void*)&pPkt->head.ver, len+4);
    PrintfData("SendProtoPkt, 给服务器发送数据", (void*)pPkt, sizeof(PKT_HEAD_STR)+len+1);
    return SendSocketData(0, (void*)pPkt, sizeof(PKT_HEAD_STR)+len+1);
}


//调用该函数需要注意是否发生递归
int SendSysLog(uint8_t gunId, uint8_t logType, char *pStr)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    SYS_LOG_STR *msg = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    msg->gun_id = gunId;
    msg->logType = logType;
    memcpy(msg->data, pStr, strlen(pStr));
    SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_SYS_LOG, pkt, 1+1+strlen(pStr));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int SendRegister(void)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    REGISTER_REQ_STR *msg = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    memcpy(msg->device_type, STATION_MACHINE_TYPE, strlen(STATION_MACHINE_TYPE));
    memset(msg->register_code, 0, sizeof(msg->register_code));
    memcpy(msg->register_code, REGISTER_CODE, strlen(REGISTER_CODE));
    PrintfData("设备注册SendRegister", (void*)msg, sizeof(REGISTER_REQ_STR));
	SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_REGISTER, pkt, sizeof(REGISTER_REQ_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int SendStartUpNotice(int flag)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
	START_UP_REQ_STR *mqtt_start_up_req = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    memcpy(mqtt_start_up_req->device_type, STATION_MACHINE_TYPE, strlen(STATION_MACHINE_TYPE));
    memcpy((void*)mqtt_start_up_req->chargerSn, (void*)(system_info1.station_id), sizeof(mqtt_start_up_req->chargerSn));
	mqtt_start_up_req->fw_version = FW_VERSION;
    mqtt_start_up_req->fw_1_ver = 0;
    memcpy(mqtt_start_up_req->sim_iccid, system_info.iccid, ICCID_LEN);
    mqtt_start_up_req->onNetWay = system_info.netType;
    mqtt_start_up_req->modeType = system_info.localNetType;
	mqtt_start_up_req->login_reason = flag;
    mqtt_start_up_req->gun_number = system_info.chargerCnt << 1;
    mqtt_start_up_req->device_status = gChgInfo.netStatus;
    mqtt_start_up_req->statistics_info[0] = gSimStatus;
    mqtt_start_up_req->statistics_info[1] = gSimStatus>>8;
    mqtt_start_up_req->statistics_info[2] = gSimStatus>>16;
    mqtt_start_up_req->statistics_info[3] = gSimStatus>>24;
    PrintfData("设备登录SendStartUpNotice", (void*)mqtt_start_up_req, sizeof(START_UP_REQ_STR));
	SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_START_UP, pkt, sizeof(START_UP_REQ_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}

int SendCardAuthReq(X6_PKT_STR *pPkt)
{
    char  buff[10] = {0};
    PKT_STR *pkt = (void*)gProtoSendBuff;
    X6_AUTH_REQ_STR *pX6AuthReq = (void*)pPkt->data;
    CARD_AUTH_REQ_STR *pAuthReq = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    pAuthReq->gun_id = pX6AuthReq->gunId;
    pAuthReq->card_type = 0;
    pAuthReq->optType = pX6AuthReq->optType;
    DeviceBcd2str(buff, &pX6AuthReq->userId[3], 5);
    memcpy(pAuthReq->card_id, &buff[1], 9);
    pAuthReq->card_id[9] = 0;
    pAuthReq->card_psw[0] = 0;
    pAuthReq->mode = pX6AuthReq->mode;
    pAuthReq->chargingPara = pX6AuthReq->para;
	printf("给后台发送刷卡鉴权! gunId = %d \n", pX6AuthReq->gunId);
	#if 1
    SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_CARD_ID_REQ, pkt, sizeof(CARD_AUTH_REQ_STR));
    #else	//去掉后台干扰
    SYS_PKT_STR Buff;
    SYS_PKT_STR *pBuff = &Buff;
    X6_AUTH_RESULT_REQ_STR *pX6AuthResultReq = (void*)pBuff->data;
	static uint16_t lefeMoney111 = 56600;

	
    pBuff->x6Head.cmd = CMD_AUTH_RESULT;
    pX6AuthResultReq->gunId = pX6AuthReq->gunId;
    pX6AuthResultReq->result = 0;//pAuthAck->result;
    pX6AuthResultReq->cardType = 1;//pAuthAck->cardType;
    lefeMoney111 -= 100;
    pX6AuthResultReq->lefeMoney = lefeMoney111;//pAuthAck->user_momey;
    if(10000 >= lefeMoney111)
	{
		lefeMoney111 = 50000;
	}
    CL_LOG("给给给给给 gunId =%d, result =%d\n",pX6AuthResultReq->gunId, pX6AuthResultReq->result);
    //memcpy(pX6AuthResultReq->orderId, pAuthAck->order, sizeof(pX6AuthResultReq->orderId));
    gChgInfo.authAckRecvFlag = 0;
    for (int i = 0; i < (RESENT_24G_MSG_CNT*RETRY_24G_MAX_CNT); i++) {
        if (0 == (i % RESENT_24G_MSG_CNT)) {
            SendX6Pkt((pX6AuthReq->gunId-1)>>1, system_info.x6AuthResultPktSn, (void*)&pBuff->x6Head, sizeof(X6_AUTH_RESULT_REQ_STR));
            PrintfData("X6AuthResult, 发送给X6后台鉴权卡的结果", (void*)pBuff, sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(X6_AUTH_RESULT_REQ_STR));
        }
        vTaskDelay(RETRY_24G_DELAY_TIME);
        if (1 == gChgInfo.authAckRecvFlag) {
            break;
        }
    }
    system_info.x6AuthResultPktSn++;
    #endif
    
    
    
    PrintfData("SendCardAuthReq, 给后台发送鉴权请求", (void*)pAuthReq, sizeof(CARD_AUTH_REQ_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int SendEventNotice(uint8_t gunId, uint8_t event, uint8_t para1, uint32_t para2, uint8_t status, char *pDisc)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
	EVENT_NOTICE_STR *eventNotice = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    eventNotice->gun_id = gunId;
    eventNotice->code = event;
    eventNotice->para1 = para1;
    eventNotice->para2 = para2;
    eventNotice->status = status;
    eventNotice->level = EVENT_ALARM;
    memset(eventNotice->discrip, 0, sizeof(eventNotice->discrip));
    memcpy(eventNotice->discrip, pDisc, strlen(pDisc));
    PrintfData("SendEventNotice", (void*)eventNotice, sizeof(EVENT_NOTICE_STR));
    SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_EVENT_NOTICE, pkt, sizeof(EVENT_NOTICE_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int SendStartChargingNotice(X6_PKT_STR *pPkt)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    START_CHARGING_NOTICE_STR *pStartNotice = (void*)pkt->data;
    X6_START_NOTICE_STR *pX6StartNotice = (void*)pPkt->data;

    MuxSempTake(&gProtoSendMux);
    pStartNotice->gun_id = pX6StartNotice->gunId;
    pStartNotice->user_type = 0;
    pStartNotice->optType = 1;
    pStartNotice->user_account[0] = 0;
    memcpy(pStartNotice->order, pX6StartNotice->orderId, sizeof(pStartNotice->order));
    pStartNotice->start_time = pX6StartNotice->startTime;
	CL_LOG("充电开启时间start_time = [%d]\n", pStartNotice->start_time);
    pStartNotice->start_power = 0;
    pStartNotice->money = 0;
    PrintfData("SendStartChargingNotice, 给后台发送充电开启通知", (void*)pkt->data, sizeof(START_CHARGING_NOTICE_STR));
    SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_REPORT_POWER_ON, pkt, sizeof(START_CHARGING_NOTICE_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int SendStartChargingAck(X6_PKT_STR *pPkt)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    START_CHARGING_ACK_STR *pStartAck = (void*)pkt->data;
    X6_START_ACK_STR *pX6StartAck = (void*)pPkt->data;

    MuxSempTake(&gProtoSendMux);
    pStartAck->gun_id = pX6StartAck->gunId;
    pStartAck->result = pX6StartAck->result;
    pStartAck->failReason = pX6StartAck->reason;
    //CL_LOG("sn=%d.\n",gChgInfo.terminalStatus[pStartAck->gun_id-1].reqStartChargingSn);
    SendProtoPkt(gChgInfo.terminalStatus[pStartAck->gun_id-1].reqStartChargingSn, MQTT_CMD_REMOTE_SET_POWER_ON, pkt, sizeof(START_CHARGING_ACK_STR));
    PrintfData("SendStartChargingAck", (void*)pPkt->data, sizeof(START_CHARGING_ACK_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int SendStopChargingNotice(STOP_CHARGING_NOTICE_REQ_STR *pPkt)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;

    MuxSempTake(&gProtoSendMux);
	memcpy(pkt->data, pPkt, sizeof(STOP_CHARGING_NOTICE_REQ_STR));
    PrintfData("SendStopChargingNotice", (void*)pkt->data, sizeof(STOP_CHARGING_NOTICE_REQ_STR));
	SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_REPORT_POWER_OFF, pkt, sizeof(STOP_CHARGING_NOTICE_REQ_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int SendStopChargingAck(X6_PKT_STR *pPkt)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    STOP_CHARGING_ACK_STR *pStopAck = (void*)pkt->data;
    X6_STOP_ACK_STR *pX6StopAck = (void*)pPkt->data;

    MuxSempTake(&gProtoSendMux);
    pStopAck->gun_id = pX6StopAck->gunId;
    pStopAck->result = pX6StopAck->result;
    PrintfData("SendStopChargingAck", (void*)pkt->data, sizeof(STOP_CHARGING_ACK_STR));
    SendProtoPkt(gChgInfo.terminalStatus[pStopAck->gun_id-1].reqStopChargingSn, MQTT_CMD_REMOTE_SET_POWER_OFF, pkt, sizeof(STOP_CHARGING_ACK_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


int MakeGunHearBeatInfo(GUN_HEART_BEAT_STR *pInfo, uint8_t gun_id)
{
    X6_GUN_STAUS_STR *pGunInfo = &gun_info[gun_id-1].gunStatus;
    int len = 4;

    pInfo->gunIndex = gun_id;
	#if 1
    if (1 == gChgInfo.terminalStatus[GetTerminalIdByGunId(gun_id)].heartBeatStatus)		//长时间没有收到X6的2.4g数据
	{
        pInfo->chipTemp = 0;
        pInfo->status = PLUG_FAULT;
        pInfo->faultCode = 3;
		CL_LOG("由于长时间没有收到X6数据，gun_id[%d], 充电口状态[%d], faultCode[%d].\n", gun_id, pInfo->status, pInfo->faultCode);
        return len;
    }
	#endif
    if (1 == pGunInfo->status)	//插口充电中
	{
        pInfo->chipTemp = 0;
        pInfo->status = PLUG_USED_OPEN;
        pInfo->faultCode = 0;
        pInfo->voltage = pGunInfo->voltage;
        pInfo->current = pGunInfo->current;
        pInfo->power = pGunInfo->power;
        pInfo->elec = pGunInfo->elec;
        pInfo->money = pGunInfo->money;
		CL_LOG("心跳包发送功率:[%d, %d, %d, %d, %d, %d].\n", pInfo->gunIndex, pInfo->power, pInfo->voltage, pInfo->current, pInfo->elec, pInfo->money);
		CL_LOG("插口充电中, gun_id[%d], 充电口状态[%d], faultCode[%d].\n", gun_id, pInfo->status, pInfo->faultCode);
        memcpy(pInfo->order, gun_info[gun_id-1].order, sizeof(pInfo->order));
        len = sizeof(GUN_HEART_BEAT_STR);
    }
	else if (2 == pGunInfo->status) //插口故障
	{
        pInfo->chipTemp = 0;
        pInfo->status = PLUG_FAULT;
        pInfo->faultCode = pGunInfo->faultCode;
		CL_LOG("插口故障, gun_id[%d], 充电口状态[%d], faultCode[%d].\n", gun_id, pInfo->status, pInfo->faultCode);
    }
	else{
        pInfo->chipTemp = 0;
        pInfo->status = PLUG_IDLE;
        pInfo->faultCode = pGunInfo->faultCode;
		CL_LOG("插头空闲, gun_id[%d], 充电口状态[%d], faultCode[%d].\n", gun_id, pInfo->status, pInfo->faultCode);
    }
    return len;
}


int SendHeartBeatFun(uint8_t gun_id)
{
	int len;
    int i;
    uint8_t  startGunIndex;
    PKT_STR *pkt = (void*)gProtoSendBuff;
	HEART_BEAT_REQ_STR *mqtt_heart_beat = (void*)pkt->data;
    GUN_HEART_BEAT_STR *pGunInfo = mqtt_heart_beat->gunStatus;

    MuxSempTake(&gProtoSendMux);
    mqtt_heart_beat->netSigle = gNetSignal;
    mqtt_heart_beat->envTemp = GetCpuTemp() + 50;
	mqtt_heart_beat->Status = 0;
    mqtt_heart_beat->gunCnt = system_info.chargerCnt;

    if (0 == gun_id) 
	{
        if (0 == gChgInfo.sendHbStartGunIndex) 
		{
            startGunIndex = 1;
            gChgInfo.sendHbStartGunIndex = 1;
        }
		else
		{
            startGunIndex = system_info.chargerCnt + 1;
            gChgInfo.sendHbStartGunIndex = 0;
        }
    }
	else
	{
        startGunIndex = ((mqtt_heart_beat->gunCnt+1) <= gun_id) ? system_info.chargerCnt + 1 : 1;
    }

    len = 0;
    for (i = 1; i <= mqtt_heart_beat->gunCnt; i++)
	{
        len += MakeGunHearBeatInfo((void*)((char*)pGunInfo+len), startGunIndex++);
    }
    PrintfData("SendHeartBeatFun给后台发送心跳包", (void*)mqtt_heart_beat, len+7);
    SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_HEART_BEAT, pkt, len+7);
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


void HeartBeatHandle(void)
{
	if ((system_info.is_socket_0_ok == 1) && (1 == system_info.isRecvStartUpAck))
	{
		SendHeartBeatFun(0);
        gSendHearBeatCnt++;
        if (3 < gSendHearBeatCnt) 
		{
            gSendHearBeatCnt = 0;
            system_info.is_socket_0_ok = CL_FALSE;
            CL_LOG("long time no recv heart beat ack,error.\n");
            gSimStatus |= (1<<SIM800_STATE_NUM);
        }
	}
}


int SendReqCostTemplate(uint8_t gunId)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    REQ_COST_TEMPLATE_STR *reqCostTemplate = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    reqCostTemplate->gun_id = gunId;
    PrintfData("SendReqCostTemplate,给服务器发送计费模板请求", (void*)reqCostTemplate, sizeof(REQ_COST_TEMPLATE_STR));
    SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_COST_REQ, pkt, sizeof(REQ_COST_TEMPLATE_STR));
    MuxSempGive(&gProtoSendMux);
    
    return CL_OK;
}


void CostTemplateReq(void)
{
    if ((COST_POWER != system_info.cost_template.mode) && (COST_UNIFY != system_info.cost_template.mode))
	{
        CL_LOG("向服务器请求计费模板谁no cost template, send req cost template.\n");
        SendReqCostTemplate(0);
    }
}


int SendReqBingRelation(uint8_t gunId)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    REQ_TERMINAL_STR *reqTerminal = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    reqTerminal->gun_id = gunId;
    PrintfData("SendReqBingRelation", (void*)reqTerminal, sizeof(REQ_TERMINAL_STR));
    SendProtoPkt(system_info.mqtt_sn++, MQTT_CMD_REQ_BIND_RELATION, pkt, sizeof(REQ_TERMINAL_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}


void CheckBingRelation(void)
{
    if (0 == system_info.chargerCnt) {
        SendReqBingRelation(0);
    }
}


int SendCostTemplateAck(uint32_t sn, uint8_t result, uint8_t gunId)
{
    PKT_STR *pkt = (void*)gProtoSendBuff;
    COST_TEMPLATE_ACK_STR *mqtt_cost_down_ack = (void*)pkt->data;

    MuxSempTake(&gProtoSendMux);
    mqtt_cost_down_ack->gunId = gunId;
    mqtt_cost_down_ack->result = result;
    PrintfData("SendCostTemplateAck", (void*)mqtt_cost_down_ack, sizeof(COST_TEMPLATE_ACK_STR));
	SendProtoPkt(sn, MQTT_CMD_COST_DOWN, pkt, sizeof(COST_TEMPLATE_ACK_STR));
    MuxSempGive(&gProtoSendMux);
    return CL_OK;
}

#if 1
int CostTempCopy(COST_TEMPLATE_HEAD_STR *pcost)
{
    int ret = CL_OK;
    uint16_t temp;
	uint32_t k = 0;
		
    if (COST_POWER == pcost->mode) 
	{
		multiPower_t *powerInfo = (void*)pcost->Data;
		CL_LOG("fwq 分功率计费cost mode=%d.\n", pcost->mode);
		CL_LOG("fwq 分功率计费功率段数目 segmentCnt = [%d] .\n", powerInfo->segmentCnt);
		if (COST_TEMP_CNT < powerInfo->segmentCnt) 
		{
			CL_LOG("segmentCnt=%d,error.\n", powerInfo->segmentCnt);
			ret = 1;
		}
		else
		{
			#if 1
			for(k = 0; k < powerInfo->segmentCnt; k++)
			{
				CL_LOG("fwq 序号[%d], price = [%d], duration = [%d], startPower = [%d], endPower = [%d] .\n", k, powerInfo->segmet[k].price, powerInfo->segmet[k].duration, powerInfo->segmet[k].startPower, powerInfo->segmet[k].endPower);
			}
			#endif
			temp = sizeof(segment_str)*powerInfo->segmentCnt + sizeof(powerInfo->segmentCnt);
			memcpy(&system_info.cost_template.Data.powerInfo, powerInfo, temp);
			system_info.cost_template.mode = COST_POWER;
		}
	}
	else if (COST_UNIFY == pcost->mode) 
	{//统一计费
		unify_t *unifyInfo = (void*)pcost->Data;
		CL_LOG("fwq统一计费 cost mode=%d.\n", pcost->mode);
		CL_LOG("fwq统一计费 price = [%d]分, duration = [%d]分钟 .\n", unifyInfo->price, unifyInfo->duration);
        temp = sizeof(unify_t);
		memcpy(&system_info.cost_template.Data.unifyInfo, unifyInfo, sizeof(unify_t));
		system_info.cost_template.mode = COST_UNIFY;
	}
	else
	{
        CL_LOG("fwq cost mode=%d,error.\n",pcost->mode);
       //OptFailNotice(205);
        ret = 1;
    }

	if (CL_OK == ret)
	{
        system_info.chargerMethod = pcost->Data[temp];
        system_info.chargerStartingGold = pcost->Data[temp+1] | (pcost->Data[temp+2]<<8);
        system_info.cost_template.template_id = pcost->template_id;
		system_info.cost_template.gunId = pcost->gunId;
		CL_LOG("计费模板 id = %d, 1固定收费2实时收费3起步金收费 = %d, 起步金 = %d.\n", system_info.cost_template.template_id, system_info.chargerMethod, system_info.chargerStartingGold);
		if (CL_OK != FlashWriteSysInfo(&system_info, sizeof(system_info), 1)) 
		{
            SendEventNotice(0, EVENT_CHIP_FAULT, 2, 2, EVENT_OCCUR, NULL);
        }
    }
	
    return ret;
}
#endif 

void CostTemplateProc(PKT_STR *pPkt)
#if 1
{
	int ret;
	COST_TEMPLATE_HEAD_STR *pcost = (void*)pPkt->data;

	ret = CostTempCopy(pcost);
	
	SendCostTemplateAck(pPkt->head.sn, ret, pcost->gunId);
}
#else
{
    int ret = CL_OK;
    int i;
    COST_TEMPLATE_REQ_STR *p_mqtt_cost_down_req = (void*)pPkt->data;
    segment_str *pSegment = NULL;

    if (COST_POWER == p_mqtt_cost_down_req->mode)
	{
        if (COST_TEMP_CNT < p_mqtt_cost_down_req->Data.powerInfo.segmentCnt)
		{
            CL_LOG("segmentCnt=%d,error.\n",p_mqtt_cost_down_req->Data.powerInfo.segmentCnt);
            ret = 1;
        }
		else
		{
            for (i=0; i<p_mqtt_cost_down_req->Data.powerInfo.segmentCnt; i++) 
			{
                pSegment = &p_mqtt_cost_down_req->Data.powerInfo.segmet[i];
                if (0 == pSegment->duration) 
				{
                    CL_LOG("duration=%d,error.\n",pSegment->duration);
                    ret = 2;
                    break;
                }
            }
            if (CL_OK == ret) 
			{
                memcpy(&system_info.cost_template.Data.powerInfo, &p_mqtt_cost_down_req->Data.powerInfo, sizeof(multiPower_t));
                system_info.cost_template.mode = COST_POWER;
            }
        }
    }
	else if (COST_UNIFY == p_mqtt_cost_down_req->mode) 
	{
        if (0 == p_mqtt_cost_down_req->Data.unifyInfo.duration) 
		{
            CL_LOG("duration=%d,error.\n",pSegment->duration);
            ret = 2;
        }
		else
		{
            memcpy(&system_info.cost_template.Data.unifyInfo, &p_mqtt_cost_down_req->Data.unifyInfo, sizeof(unify_t));
            system_info.cost_template.mode = COST_UNIFY;
        }
    }
	else
	{
        CL_LOG("cost mode=%d,error.\n",p_mqtt_cost_down_req->mode);
        ret = 3;
    }

    if (ret == CL_OK) 
	{
        system_info.cost_template.template_id = p_mqtt_cost_down_req->template_id;
		system_info.chargerMethod = p_mqtt_cost_down_req->chargerMethod;
		system_info.chargerStartingGold = p_mqtt_cost_down_req->chargerStartingGold;
		CL_LOG("system_info.cost_template.template_id = %d,system_info.chargerMethod = %d,system_info.chargerStartingGold = %d,mode = %d.\n", system_info.cost_template.template_id, system_info.chargerMethod, system_info.chargerStartingGold, system_info.cost_template.mode);
        if (CL_OK != FlashWriteSysInfo(&system_info, sizeof(system_info), 1)) 
		{
            SendEventNotice(0, EVENT_CHIP_FAULT, 2, 2, EVENT_OCCUR, NULL);
        }
    }
    SendCostTemplateAck(pPkt->head.sn, ret, p_mqtt_cost_down_req->gunId);
}
#endif

void RemoteCtrlProc(PKT_STR *pFrame)
{
    switch (pFrame->data[0])
	{
        case SYSTEM_REBOOT:
            CL_LOG("reboot system now.\n");
            vTaskDelay(1000);
            NVIC_SystemReset();
		break;
		//case CTRL_SET_DISTURBING_TIME:
		//	system_info.disturbingStartTime = pFrame->data[1];
		//	system_info.disturbingStopTime = pFrame->data[2];
		//	CL_LOG("语音禁播开启时间=%d, 结束时间=%d.\n", system_info.disturbingStartTime,system_info.disturbingStopTime);
		//	FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
		//break;
    }
}


void RegisterAckProc(PKT_STR *pPkt)
{
    REGISTER_ACK_STR *pRegister = (void*)pPkt->data;

    if (0 == pRegister->result) {
        PrintfData("idcode:", (void*)pRegister->idcode, 8);
        memcpy((void*)system_info.idCode, (void*)pRegister->idcode, sizeof(system_info.idCode));
        FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
        CL_LOG("get register dcode, reboot system now.\n");
        vTaskDelay(1000);
        NVIC_SystemReset();
    }
	else
	{
        CL_LOG("register ack result=%d,error.\n",pRegister->result);
    }
}


void StartUpAckProc(PKT_STR *pPkt)
{
    START_UP_ACK_STR *pStartUp = (void*)pPkt->data;

	//CL_LOG("设备登录服务器时间 = %d.\n", pStartUp->time_utc);
    SetRtcCount(pStartUp->time_utc);
    gBlueCtrl.lastRecv24gTime = pStartUp->time_utc;
    gSimStatus = 0;
    if (0 == pStartUp->result) 
	{
        CL_LOG("设备登录服务器成功.\n");
        system_info.isRecvStartUpAck = 1;
    }
	else
	{
        CL_LOG("start up ack result=%d,error.\n",pStartUp->result);
    }
}


void CardAuthAckProc(PKT_STR *pPkt)
{
	SYS_PKT_STR syspPkt;
    CARD_AUTH_ACK_STR* pAuthAck = (void*)pPkt->data;
    
    CL_LOG("收到服务器卡鉴权应答, gun_id = %d, heartBeatStatus = %d.\n", pAuthAck->gun_id, gChgInfo.terminalStatus[GetTerminalIdByGunId(pAuthAck->gun_id)].heartBeatStatus);
    //if (2 == gChgInfo.terminalStatus[GetTerminalIdByGunId(pAuthAck->gun_id)].heartBeatStatus)
    {
    	CL_LOG("卡片余额[%d]\n", pAuthAck->user_momey);
    	#if 1
        PktInQueue(pPkt, pAuthAck->gun_id);
		SendX6ReqPkt(&syspPkt, ONE_TIMES);
        CL_LOG("给X6发送后台鉴权结果\n");
        //PktInQueue(pPkt, pAuthAck->gun_id);
		#else
		//SendX6AuthResult(pPkt);
		#endif
    }
	//else{
    //    CL_LOG("terminal status error,drop pkt.\n");
    //}
	
}


void ReqStartCharging(PKT_STR *pPkt)
{
//    uint8_t  len = sizeof(X6_PKT_HEAD_STR);
    SYS_PKT_STR syspPkt;
    START_CHARGING_REQ_STR* startChargingReq = (void*)pPkt->data;
    //START_CHARGING_ACK_STR *pMsg = (void*)pPkt->data;
    //X6_PKT_STR *pX6Msg = (void*)(pPkt->data - len);

	CL_LOG("扫码开启充电金额 = [%d]分\n", startChargingReq->chargingPara);
    //if (2 == gChgInfo.terminalStatus[GetTerminalIdByGunId(startChargingReq->gun_id)].heartBeatStatus)
    {
        gChgInfo.terminalStatus[startChargingReq->gun_id-1].reqStartChargingSn = pPkt->head.sn;
        //CL_LOG("sn=%d.\n",pPkt->head.sn);
        PktInQueue(pPkt, startChargingReq->gun_id);
        SendX6ReqPkt(&syspPkt, ONE_TIMES);
        CL_LOG("给X6下发开启充电\n");
        //PktInQueue(pPkt, startChargingReq->gun_id);
    }
    //else{
    //    CL_LOG("发送开启充电应答\n");
    //    pMsg->gun_id = startChargingReq->gun_id;
    //    pMsg->result = 1;
    //    pMsg->failReason = START_FAIL_OFF_LINE;
    //    SendStartChargingAck(pX6Msg);
    //}
}


void ReqStopCharging(PKT_STR *pPkt)
{
//    uint8_t  len = sizeof(X6_PKT_HEAD_STR);
	SYS_PKT_STR syspPkt;
    STOP_CHARGING_REQ_STR* stopChargingReq = (void*)pPkt->data;
//    STOP_CHARGING_ACK_STR *pMsg = (void*)pPkt->data;
//    X6_PKT_STR *pX6Msg = (void*)(pPkt->data - len);

    //if (2 == gChgInfo.terminalStatus[GetTerminalIdByGunId(stopChargingReq->gun_id)].heartBeatStatus) {
        gChgInfo.terminalStatus[stopChargingReq->gun_id-1].reqStopChargingSn = pPkt->head.sn;
		PktInQueue(pPkt, stopChargingReq->gun_id);
        SendX6ReqPkt(&syspPkt, ONE_TIMES);
        CL_LOG("给X6下发停止充电请求\n");
        //PktInQueue(pPkt, stopChargingReq->gun_id);
    //}
	//else{
    //    pMsg->gun_id = stopChargingReq->gun_id;
     //   pMsg->result = 1;
    //    SendStopChargingAck(pX6Msg);
   // }
}


void StopChargingNoticeAck(PKT_STR *pPkt)
{
    ORDER_STR *pOrder = &system_info.orderInfo;
    STOP_CHARGING_NOTICE_ACK_STR *pStopNoticeAck = (void*)pPkt->data;

    if (pOrder->sendCnt && pOrder->gunId)
	{
        if (pStopNoticeAck->gun_id == pOrder->gunId) 
		{
            if (RECORD_MAX_BLOCK_NUM <= ++pOrder->rIndex)
			{
                pOrder->rIndex = 0;
            }
            pOrder->gunId = 0;
            pOrder->sendCnt = 0;
            FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
        }
    }
}


void HeartBeatAckProc(PKT_STR *pPkt)
{
    int cnt;
    uint32_t now = GetRtcCount();
    HEART_BEAT_ACK_STR *pHeartBeatAck = (HEART_BEAT_ACK_STR*)pPkt->data;
    if (pHeartBeatAck->time) {
        cnt = (now > pHeartBeatAck->time) ? now - pHeartBeatAck->time : pHeartBeatAck->time - now;
        if (30 < cnt) {
            SetRtcCount(pHeartBeatAck->time+2);
            gBlueCtrl.lastRecv24gTime = pHeartBeatAck->time+2;
            CL_LOG("set rtc time.\n");
        }
    }
    CL_LOG("heart-beat ack.\n");
    gSendHearBeatCnt = 0;
}


int SendBingRelationAck(PKT_STR *pPkt, uint8_t result)
{
    BIND_TERMINAL_ACK_STR *pBindTerminalAck = (void*)pPkt->data;

    pBindTerminalAck->result = result;
    SendProtoPkt(pPkt->head.sn, MQTT_CMD_BIND_RELATION, (void*)pPkt, sizeof(BIND_TERMINAL_ACK_STR));
    PrintfData("SendBingRelationAck", (void*)pPkt, sizeof(PKT_HEAD_STR)+sizeof(BIND_TERMINAL_ACK_STR)+1);
    return CL_OK;
}

int SendDevBingRelation(PKT_STR *pPkt)
{
    DEV_BIND_TERMINAL_STR *pDevBindTerminal = (void*)pPkt->data;
	uint32_t i = 0;

	CL_LOG("给后台发送x6个数 = %d \n", system_info.chargerCnt);
	memcpy(pDevBindTerminal->chargerInfo, system_info.chargerInfo, system_info.chargerCnt*sizeof(CHARGER_STR));
	pDevBindTerminal->cnt = system_info.chargerCnt;
	for (i = 0; i < system_info.chargerCnt; i++)
	{
		PrintfData("设备绑定关系反馈x6设备号", (void*)pDevBindTerminal->chargerInfo[i].chargerSn, sizeof(pDevBindTerminal->chargerInfo[i].chargerSn));
	}

    SendProtoPkt(pPkt->head.sn, MQTT_CMD_DEV_BIND_RELATION, (void*)pPkt, sizeof(DEV_BIND_TERMINAL_STR));
    //PrintfData("设备绑定关系反馈", (void*)pPkt, sizeof(PKT_HEAD_STR) + system_info.chargerCnt * sizeof(CHARGER_STR)+ 1+2);
	
    return CL_OK;
}

void ReceiveDevBindRelationAck(PKT_STR *pPkt)
{
    DEV_BIND_TERMINAL_ACK_STR *pDevBindTerminalAck = (void*)pPkt->data;

	if(0 == pDevBindTerminalAck->result)
	{
		//绑定成功
		CL_LOG("绑定或解绑成功 绑定或解绑成功 绑定或解绑成功.\n");
		vTaskDelay(300);
		gBlueCtrl.resetMode = 1;
	}
}

void SetBindReqFlag(void)
{
	gChgInfo.flagBit.X6R6BindRelation = 1;

	return;

}

void ClearBindReqFlag(void)
{
	gChgInfo.flagBit.X6R6BindRelation = 0;

	return;

}

uint8_t GetBindReqFlag(void)
{
	return gChgInfo.flagBit.X6R6BindRelation;

}

void BindRelation(PKT_STR *pPkt)
{
    TERMINAL_STR *pTerminal = (void*)pPkt->data;
    uint32_t i = 0;

	CL_LOG("X6个数为 cnt=%d.\n",pTerminal->cnt);
    if (MAX_TERMINAL_NUM < pTerminal->cnt) 
	{
        CL_LOG("terminal cnt=%d,配置终端数目大于最大值error.\n",pTerminal->cnt);
        SendBingRelationAck(pPkt, 1);
        return;
    }
    for (i = 0; i < pTerminal->cnt; i++) {
        PrintfData("BindRelation", (void*)pTerminal->chargerInfo[i].chargerSn, sizeof(pTerminal->chargerInfo[i].chargerSn));
    }
    memcpy(system_info.chargerInfo, pTerminal->chargerInfo, pTerminal->cnt*sizeof(CHARGER_STR));
    system_info.chargerCnt = pTerminal->cnt;
	SendBingRelationAck(pPkt, 0);
	
    FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
	vTaskDelay(500);
	SendDevBingRelation(pPkt);
	//CL_LOG("给X6发送绑定确认请求.\n");
	//SetBindReqFlag();
	//SendX6R6BindReq(pPkt);
    //CL_LOG("set binding info,reset system.\n");
    //vTaskDelay(1000);
    //NVIC_SystemReset();
}


int RecvServerData(void *pPkt, uint16_t len)
{
    PKT_STR* pFrame = pPkt;

    if (CHARGER_TYPE != pFrame->head.type) 
	{
        CL_LOG("type=%d,error, pkt drop.\n",pFrame->head.type);
        return CL_FAIL;
    }

    if (MQTT_CMD_REGISTER == pFrame->head.cmd)
	{
        if (memcmp(pFrame->head.chargerSn, system_info1.station_id, sizeof(pFrame->head.chargerSn))) 
		{
            CL_LOG("sn diff error, pkt drop,cmd=%d.\n",pFrame->head.cmd);
            return CL_FAIL;
        }
    }
	else
	{
        if (memcmp(pFrame->head.chargerSn, system_info.idCode, 8)) 
		{
            CL_LOG("idCode diff error, pkt drop,cmd=%d.\n",pFrame->head.cmd);
            return CL_FAIL;
        }
    }
	CL_LOG("收到后台数据命令cmd=%d.\n",pFrame->head.cmd);
    switch (pFrame->head.cmd) 
	{
        case MQTT_CMD_REGISTER:
			PrintfData("RecvServerData: 设备注册应答register ack", (void*)pFrame->data, len);
            RegisterAckProc(pFrame);
            break;

        case MQTT_CMD_START_UP:
			PrintfData("RecvServerData: 设备登录应答start up ack", (void*)pFrame->data, len);
            StartUpAckProc((void*)pFrame);
            break;

        case MQTT_CMD_CARD_ID_REQ:
			PrintfData("RecvServerData: card auth ack", (void*)pFrame->data, len);
            CardAuthAckProc(pFrame);
            break;

        case MQTT_CMD_REMOTE_SET_POWER_ON:
			PrintfData("收到服务器开启充电请求", (void*)pFrame->data, len);
            ReqStartCharging(pFrame);
            break;

        case MQTT_CMD_REMOTE_SET_POWER_OFF:
			PrintfData("RecvServerData: recv power off req", (void*)pFrame->data, len);
            ReqStopCharging(pFrame);
            break;

        case MQTT_CMD_REPORT_POWER_ON:
            PrintfData("RecvServerData: recv start charging notice ack", (void*)pFrame->data, len);
            break;

        case MQTT_CMD_REPORT_POWER_OFF:
			PrintfData("RecvServerData: stop charging notice ack", (void*)pFrame->data, len);
            StopChargingNoticeAck(pFrame);
            break;

        case MQTT_CMD_HEART_BEAT:
			PrintfData("RecvServerData:接收到后台心跳包", (void*)pFrame->data, len);
            HeartBeatAckProc(pFrame);
            break;

        case MQTT_CMD_COST_DOWN:
			PrintfData("RecvServerData: 服务器下发计费模板", (void*)pFrame->data, len);
            CostTemplateProc((void*)pFrame);
            break;

        case MQTT_CMD_DFU_DOWN_FW_INFO:
			gChgInfo.flagBit.Upgrade2_4Gflag = 0;
			gChgInfo.flagBit.isSendFwUpgradeOk = 0;
            SetUpgradeInfo((void*)pFrame->data, CHARGER_TYPE_R6);
            break;

        case MQTT_CMD_REMOTE_CTRL:
			PrintfData("RecvServerData: recv remote ctrl", (void*)pFrame->data, len);
            RemoteCtrlProc((void*)pFrame);
            break;

        case MQTT_CMD_EVENT_NOTICE:
            //event_notice_ack_t* pevent_notice_ack = (event_notice_ack_t*)p_cmd_frame->data;
            //CL_LOG("recv event notice ack, ret=%d.\n",pevent_notice_ack->result);
            break;

        case MQTT_CMD_BIND_RELATION:
			//PrintfData("RecvServerData: bind relation", (void*)pFrame->data, len);
            BindRelation((void*)pFrame);
            break;
		case MQTT_CMD_DEV_BIND_RELATION:
            PrintfData("接收到后台设备绑定关系反馈", (void*)pFrame->data, len);
			ReceiveDevBindRelationAck((void*)pFrame);
            break;
        default:
            CL_LOG("mqtt unkown command %d.\n",pFrame->head.cmd);
            break;
    }
    return 0;
}


