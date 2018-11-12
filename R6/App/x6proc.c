
#include "includes.h"
#include "x6proc.h"
#include "server.h"
#include "bluetooth.h"
#include "proto.h"
#include "blue.h"


inline int GetTerminalIdByGunId(uint8_t gunIndex)
{
    return (gunIndex - 1) >> 1;
}


inline int GetGunIdByTerminalId(uint8_t nodeId)
{
    return (nodeId<<1)+1;
}


int ProcGunStatus(X6_GUN_STAUS_STR *pStatus)
{
    uint8_t  gunId = pStatus->gunId - 1;

    if (1 == pStatus->status)
	{
        memcpy(&gun_info[gunId].gunStatus, pStatus, sizeof(X6_GUN_STAUS_STR));

		return sizeof(X6_GUN_STAUS_STR);
    }
	else
	{
        gun_info[gunId].gunStatus.gunId = pStatus->gunId;
        gun_info[gunId].gunStatus.status = pStatus->status;
        gun_info[gunId].gunStatus.faultCode = pStatus->faultCode;

		return sizeof(X6_GUN_STAUS_IDEL_STR);
    }
}

/*****************************************************************************
** Function name:       CheckUpgradeWaitTimer
** Descriptions:        //升级过程中，大于50s没有接收到数据说明 X6 已经断电或离线了
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
#define UPGRADE_WAIT_TIME_OUT       35
void CheckUpgradeWaitTimer(void)
{
    if((1 == gChgInfo.flagBit.Upgrade2_4Gflag) || (1 == gChgInfo.flagBit.isSendFwUpgradeOk))    //如果2.4G处于升级状态
    {
        if(GetRtcCount() >= gChgInfo.gUpgradeWaitTimerTick)
        {
            if (GetRtcCount() - gChgInfo.gUpgradeWaitTimerTick >= UPGRADE_WAIT_TIME_OUT)  //大于50s说明 R6 已经断电或离线了
            {
                CL_LOG("超时了! \n");
                gChgInfo.flagBit.Upgrade2_4Gflag = 0;
                gChgInfo.flagBit.isSendFwUpgradeOk = 0;
                gChgInfo.gUpgradeWaitTimerTick = GetRtcCount();
            }
        }
        else    //计数器溢出了
        {   
            if (0xffffffff - gChgInfo.gUpgradeWaitTimerTick + GetRtcCount() >= UPGRADE_WAIT_TIME_OUT)  //大于50s说明 R6 已经断电或离线了
            {
                CL_LOG("超时了2222! \n");
                gChgInfo.flagBit.Upgrade2_4Gflag = 0;
                gChgInfo.flagBit.isSendFwUpgradeOk = 0;
                gChgInfo.gUpgradeWaitTimerTick = GetRtcCount();
            }
        }
    }
    
    return;
}

/*****************************************************************************
** Function name:       ClearUpgradeWaitTimer
** Descriptions:        //重新复位等待时间
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
void ClearUpgradeWaitTimer(void)
{
    gChgInfo.gUpgradeWaitTimerTick = GetRtcCount();   //重新复位等待时间
    
    return;
}

//pPkt前面预留有蓝牙2.4G报文头； len:去掉报文头的数据域长度
int SendX6Pkt(uint8_t nodeIndex, uint16_t sn, X6_PKT_STR *pPkt, uint16_t len)
{
    uint8_t *pbuff = (void*)pPkt;
    BLUE_PKT_STR *pBule = (void*)(pbuff - sizeof(BLUE_HEAD_STR));
    //uint64_t addr24g = 0;

    uint8_t  ppaddr[6];
    ppaddr[0] = system_info1.gwAddr[4];//0x01;
    ppaddr[1] = system_info1.gwAddr[3];//0x02;
    ppaddr[2] = system_info1.gwAddr[2];//0x03;
    ppaddr[3] = system_info1.gwAddr[1];//0x04;
    ppaddr[4] = system_info1.gwAddr[0];//0xc0;
    ppaddr[5] = 0x00;

    
    pPkt->head.ee = 0xee;
    pPkt->head.len = len+3;
    pPkt->head.sn = sn;
    pbuff[sizeof(X6_PKT_HEAD_STR)+len] = GetPktSum((void*)&pPkt->head.sn, len+2);
    pBule->head.addr[0] = ppaddr[0];//(uint8_t)(addr24g >> 32);
    pBule->head.addr[1] = ppaddr[1];//(uint8_t)(addr24g >> 24);
    pBule->head.addr[2] = ppaddr[2];//(uint8_t)(addr24g >> 16);
    pBule->head.addr[3] = ppaddr[3];//(uint8_t)(addr24g >> 8);
    pBule->head.addr[4] = ppaddr[4] + (nodeIndex + 1);//(uint8_t)(addr24g >> 0) + (nodeIndex + 1);
    pBule->head.addr[MAC_LEN] = 0;
    
    //CL_LOG("节点 nodeIndex = %d ! \n", nodeIndex);
    SendBluePkt(NODE_24G, pBule, sizeof(X6_PKT_HEAD_STR)+len+1);
    PrintfData("外层包地址 addr", pBule->head.addr, sizeof(pBule->head.addr));
    
    return CL_OK;
}


int SendX6CostTemplate(uint8_t index, BLUE_PKT_STR *pBlue)
{
	int ret = CL_OK;
	uint16_t temp;
	uint32_t k = 0;
	
    X6_COST_TEMPLATE_REQ_STR *pCost = (void*)&pBlue->data[sizeof(X6_PKT_HEAD_STR)];
    X6_PKT_STR *pX6Pkt = (void*)pBlue->data;
    if (0 == system_info.cost_template.template_id)
    {
        CL_LOG("暂时没有计费模板, 请求服务器下发! \n");

        SendReqCostTemplate(0);
        
        return CL_FAIL;
    }
    CL_LOG("给X6发送计费模板! \n");
    pCost->gunId = GetGunIdByTerminalId(index);
    pCost->template_id = system_info.cost_template.template_id;
    pCost->mode = system_info.cost_template.mode;
	pCost->type = system_info.chargerMethod;
	pCost->chargerStartingGold = system_info.chargerStartingGold;
	CL_LOG("gunId = [%d], template_id = [%d], 固定收费1实时收费2起步金收费3 = [%d]! 起步金 = [%d]\n", pCost->gunId, pCost->template_id,pCost->type, pCost->chargerStartingGold);
	if(COST_POWER == pCost->mode)
	{
		temp = sizeof(segment_str)*system_info.cost_template.Data.powerInfo.segmentCnt + sizeof(system_info.cost_template.Data.powerInfo.segmentCnt);
		memcpy(&pCost->Data.powerInfo, &system_info.cost_template.Data.powerInfo, temp);
		temp = temp + sizeof(X6_COST_TEMPLATE_REQ_STR) - sizeof(system_info.cost_template.Data);
		CL_LOG("分功率计费cost mode=%d.\n", pCost->mode);
		CL_LOG("分功率计费功率段数目segmentCnt = [%d] .\n", system_info.cost_template.Data.powerInfo.segmentCnt);
		#if 1
		for(k = 0; k < system_info.cost_template.Data.powerInfo.segmentCnt; k++)
		{
			CL_LOG("序号[%d], price = [%d], duration = [%d], startPower = [%d], endPower = [%d] .\n", k, pCost->Data.powerInfo.segmet[k].price, pCost->Data.powerInfo.segmet[k].duration, pCost->Data.powerInfo.segmet[k].startPower, pCost->Data.powerInfo.segmet[k].endPower);
		}
		#endif
	}
	else if(COST_UNIFY == pCost->mode)
	{
		temp = sizeof(unify_t);
		memcpy(&pCost->Data.unifyInfo, &system_info.cost_template.Data.unifyInfo, sizeof(unify_t));
		temp = temp + sizeof(X6_COST_TEMPLATE_REQ_STR) - sizeof(system_info.cost_template.Data);
		CL_LOG("统一计费 cost mode=%d.\n", pCost->mode);
		CL_LOG("统一计费 price = [%d], duration = [%d] .\n", pCost->Data.unifyInfo.price, pCost->Data.unifyInfo.duration);
	}
	else
	{
		ret = CL_FAIL;
	}
	if(CL_OK == ret)
	{
		//memcpy(&pCost->Data.powerInfo, &system_info.cost_template.Data.powerInfo, sizeof(X6_multiPower_t));
		pX6Pkt->head.cmd = CMD_COST_DOWN;
		SendX6Pkt(index, system_info.x6PktSn++, (void*)pBlue->data, sizeof(COST_TEMPLATE_REQ_STR));
		PrintfData("SendX6CostTemplate", (void*)pBlue, sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(COST_TEMPLATE_REQ_STR));
	}
    
    return ret;
}

int GetIsMyTurnToUpgrade(uint8_t gunId)
{
	uint8_t returnFlag = 0;
	uint8_t Index = 1;
	static uint32_t timeTicks[MAX_TERMINAL_NUM] = {0,};
	static uint32_t x6_package_num[MAX_TERMINAL_NUM] = {0,};
	uint8_t nodeIndex = GetTerminalIdByGunId(gunId);	

//	CL_LOG("升级节点[0x%x]\n", nodeIndex);
//	CL_LOG("[0x%x], [0x%x]\n", nodeIndex, system_info.isUpgradeOk.UpgradeFinish[nodeIndex]);
	for(Index = 0; Index < nodeIndex; Index++)
	{
		if(x6_package_num[Index] != gChgInfo.terminalStatus[Index].recvCnt)
		{
			x6_package_num[Index] = gChgInfo.terminalStatus[Index].recvCnt;
			timeTicks[Index] = GetRtcCount();
			gChgInfo.UpgradeFlag = 1;
			printf("节点[0x%x], x6_package_num[Index] = %d\n", Index, x6_package_num[Index]);
		}
		
		if((GetRtcCount() - timeTicks[Index] > 60*5))
		{
			CL_LOG("[0x%x]节点5分钟没有收到X6数据\n", Index);
			gChgInfo.UpgradeFlag = 0;
		}
		
		if((system_info.isUpgradeOk.UpgradeFinish[Index] == 0) && (gChgInfo.UpgradeFlag == 1))
		{
			CL_LOG("nodeIndex444\n");
			returnFlag++;
		}
		CL_LOG("[0x%x], [0x%x]\n", Index, system_info.isUpgradeOk.UpgradeFinish[Index]);
	}
	
	if((nodeIndex + 1) == system_info.chargerCnt)
	{
		if((system_info.isUpgradeOk.UpgradeFinish[nodeIndex] == 1))
		{
			CL_LOG("nodeIndex111\n");
			returnFlag = 1;
		}
	}
	if((system_info.chargerCnt > 1) && (0 == nodeIndex) && (system_info.isUpgradeOk.UpgradeFinish[0] == 1))
	{
		CL_LOG("nodeIndex333\n");
		returnFlag = 1;
	}
	if(system_info.isUpgradeOk.UpgradeFinish[nodeIndex] == 1)
	{
		CL_LOG("nodeIndex555\n");
		returnFlag = 1;
	}
	CL_LOG("返回标志returnFlag = [0x%x]\n", returnFlag);
	
	return returnFlag;
}


//pPkt前面有2.4G报文头  type = 0:心跳应答；2:枚举
int SendX6HeartBeatReqPkt(X6_PKT_STR *pPkt, uint8_t gunId)
{
    X6_HB_REQ_STR *pHeartBeatReq = (void*)pPkt->data;
	uint8_t nodeIndex = GetTerminalIdByGunId(gunId);
	
    pHeartBeatReq->sysTime = (0 == system_info.isRecvStartUpAck) ? 0 : GetRtcCount();
    
    pHeartBeatReq->id = system_info.cost_template.template_id;
    //CL_LOG("心跳包系统时间 sysTime = 0x%x, 计费模板id = 0x%x\n", pHeartBeatReq->sysTime, pHeartBeatReq->id);
    CL_LOG("心跳包 gunId = 0x%x\n", gunId);
    if((1 == gChgInfo.flagBit.Upgrade2_4Gflag) && (1 == gChgInfo.flagBit.isSendFwUpgradeOk))
    {
        pHeartBeatReq->x6Ver = 0;
		CL_LOG("X6升级版本号1[%d], [%d]\n", pHeartBeatReq->x6Ver, system_info.x6FwInfo.ver);
    }
    else
	{
		if((0 == GetIsMyTurnToUpgrade(gunId)))
		{
			pHeartBeatReq->x6Ver = system_info.x6FwInfo.ver;
			CL_LOG("X6升级版本号2[%d], [%d]\n", pHeartBeatReq->x6Ver, system_info.x6FwInfo.ver);
		}
		else
		{
			pHeartBeatReq->x6Ver = 0;
			CL_LOG("X6升级版本号3[%d], [%d]\n", pHeartBeatReq->x6Ver, system_info.x6FwInfo.ver);
		}
    }
	memcpy(pHeartBeatReq->gwAddr, &system_info1.station_id[3], sizeof(pHeartBeatReq->gwAddr));
    pHeartBeatReq->startGunIndex = gunId;
    pPkt->head.cmd = CMD_HEART_BEAT;
    SendX6Pkt((gunId-1)>>1, system_info.x6PktSn++, (void*)pPkt, sizeof(X6_HB_REQ_STR));
    PrintfData("心跳包SendX6HeartBeatReqPkt", (void*)((uint8_t*)pPkt-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(X6_HB_REQ_STR)+4);
    return CL_OK;
}

//pPkt前面有2.4G报文头
int ScanCodeStartChargerAck(X6_PKT_STR *pPkt, uint8_t sn, uint8_t gunId)
{
    ACK_X6_RECEIVED_SCAN_CODE_STR *pX6ScanCodeChargerAck = (void*)pPkt->data;

    pPkt->head.cmd = CMD_SCAN_CODE_START_CHARGE;
	pX6ScanCodeChargerAck->gunId = gunId;
    pX6ScanCodeChargerAck->result = CL_OK;
    SendX6Pkt((gunId-1)>>1, sn, (void*)pPkt, sizeof(ACK_X6_RECEIVED_SCAN_CODE_STR));
	CL_LOG("给X6发送已经收到开启充电应答!\n");
	
    return CL_OK;
}

void RecvX6ScanCodeStartChargerAck(uint8_t             gunId, uint8_t sn)
{
    uint8_t  buff[RF24G_BUFF_PKT_LEN_SEND_TO_X6] = {0,};
	CL_LOG("给X6发送已经收到开启充电应答!\n");
	ScanCodeStartChargerAck((void*)&buff[sizeof(BLUE_HEAD_STR)], sn, gunId);
}

//pPkt前面有2.4G报文头
int SendX6AuthReqAck(X6_PKT_STR *pPkt, uint8_t sn, uint8_t gunId)
{
    X6_AUTH_ACK_STR *pX6AuthAck = (void*)pPkt->data;

    pPkt->head.cmd = CMD_AUTH_REQ;
    pX6AuthAck->gunId = gunId;
    pX6AuthAck->result = CL_OK;
	if ((system_info.is_socket_0_ok == 1) && (1 == system_info.isRecvStartUpAck))
	{
		pX6AuthAck->result = CL_OK;
	}
	else
	{
		pX6AuthAck->result = 0xae;
	}
    SendX6Pkt((gunId-1)>>1, sn, (void*)pPkt, sizeof(X6_AUTH_ACK_STR));
    PrintfData("SendX6AuthReqAck, 给X6发送没有后台鉴权的应答", (void*)((uint8_t*)pPkt-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(X6_AUTH_ACK_STR));
    return CL_OK;
}

void RecvX6AuthReqProc(X6_PKT_STR *pMsg)
{
    uint8_t  buff[RF24G_BUFF_PKT_LEN_SEND_TO_X6] = {0};
    TERMINAL_STATUS_STR *pTerMinal = NULL;
    X6_AUTH_REQ_STR *pX6AuthReq = (void*)pMsg->data;
    static uint8_t SerialNum = 255;
	uint8_t nodeIndex = GetTerminalIdByGunId(pX6AuthReq->gunId);	
	static uint8_t timesFlag[MAX_TERMINAL_NUM] = {0,};
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    if((SerialNum != pMsg->head.sn) || (((uint32_t)30) <= (GetRtcCount() - gChgInfo.gCardAuthNoticeTick[nodeIndex])))
    {
AuthReqFlag:
		timesFlag[nodeIndex] = 0;
    	gChgInfo.gCardAuthNoticeTick[nodeIndex] = GetRtcCount();
    	CL_LOG("SerialNum = %d, sn=%d!\n", SerialNum, pMsg->head.sn);
        if ((0 == pX6AuthReq->gunId) || ((system_info.chargerCnt << 1) < pX6AuthReq->gunId))
        {
            CL_LOG("gunId=%d,error.\n",pX6AuthReq->gunId);
            return;
        }
        pTerMinal = &gChgInfo.terminalStatus[GetTerminalIdByGunId(pX6AuthReq->gunId)];
        pTerMinal->heartBeatStatus = 2;
		pTerMinal->sendCnt = 0;
        SendX6AuthReqAck((void*)&buff[sizeof(BLUE_HEAD_STR)], pMsg->head.sn, pX6AuthReq->gunId);
        
        SendCardAuthReq(pMsg);

        PrintfData("RecvX6AuthReqProc,收到X6鉴权请求", (void*)pMsg, sizeof(X6_PKT_HEAD_STR)+sizeof(X6_AUTH_REQ_STR));
        SerialNum = pMsg->head.sn;
    }
    else
	{
        CL_LOG("SerialNum = %d, sn=%d.错误!\n", SerialNum, pMsg->head.sn);
		if(2 <= timesFlag[nodeIndex]++)
		{
			goto AuthReqFlag;
		}
    }
}


void RecvX6AuthResultProc(X6_PKT_STR *pMsg)
{
    gChgInfo.authAckRecvFlag = 1;
    PrintfData("RecvX6AuthResultProc, 收到X6的后台鉴权过的应答", pMsg->data, sizeof(X6_AUTH_RESULT_ACK_STR));
}


void RecvX6ReqStartChargingAck(X6_PKT_STR *pMsg)
{
    TERMINAL_STATUS_STR *pTerMinal = NULL;
    X6_START_ACK_STR *pX6Msg = (void*)pMsg->data;
	uint32_t i = 0;
	uint8_t nodeIndex = GetTerminalIdByGunId(pX6Msg->gunId);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    if ((0 == pX6Msg->gunId) || ((system_info.chargerCnt << 1) < pX6Msg->gunId)) 
	{
        CL_LOG("gunId=%d,error.\n",pX6Msg->gunId);
        return;
    }
	
    pTerMinal = &gChgInfo.terminalStatus[GetTerminalIdByGunId(pX6Msg->gunId)];
    pTerMinal->heartBeatStatus = 2;
	pTerMinal->sendCnt = 0;
    gChgInfo.reqX6StartCharging = 1;
	
	for(i = 0; i < 2; i++)
	{
		RecvX6ScanCodeStartChargerAck(pX6Msg->gunId, pMsg->head.sn);
		SendStartChargingAck(pMsg);
	}
	
    PrintfData("RecvX6ReqStartChargingAck, 收到X6的请求开启充电应答", (void*)pMsg, sizeof(X6_PKT_HEAD_STR)+sizeof(X6_START_ACK_STR));
}


void RecvX6ReqStopChargingAck(X6_PKT_STR *pMsg)
{
    TERMINAL_STATUS_STR *pTerMinal = NULL;
    X6_STOP_ACK_STR *pX6Msg = (void*)pMsg->data;
	uint8_t i = 0;
	uint8_t nodeIndex = GetTerminalIdByGunId(pX6Msg->gunId);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    PrintfData("RecvX6ReqStopChargingAck", (void*)pMsg, sizeof(X6_PKT_HEAD_STR)+sizeof(X6_STOP_ACK_STR));
    if ((0 == pX6Msg->gunId) || ((system_info.chargerCnt << 1) < pX6Msg->gunId)) 
	{
        CL_LOG("gunId=%d,error.\n",pX6Msg->gunId);
        return;
    }
	for(i = 0; i < 3; i++)
	{
		SendStopChargingAck(pMsg);
	}
    pTerMinal = &gChgInfo.terminalStatus[GetTerminalIdByGunId(pX6Msg->gunId)];
    pTerMinal->heartBeatStatus = 2;
	pTerMinal->sendCnt = 0;
    gChgInfo.reqX6StopCharging = 1;
}


//pPkt前面有2.4G报文头
int SendX6StartChargingAck(X6_PKT_STR *pPkt, uint8_t sn, uint8_t gunId)
{
    X6_START_NOTICE_ACK_STR *pX6StartNoticeAck = (void*)pPkt->data;

    pPkt->head.cmd = CMD_REPORT_POWER_ON;
    pX6StartNoticeAck->gunId = gunId;
    pX6StartNoticeAck->result = CL_OK;
    SendX6Pkt(GetTerminalIdByGunId(gunId), sn, (void*)pPkt, sizeof(X6_START_NOTICE_ACK_STR));
    PrintfData("SendX6StartChargingAck, 给X6发送充电开启应答", (uint8_t*)pPkt-sizeof(BLUE_HEAD_STR), sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(X6_START_NOTICE_ACK_STR));
    return CL_OK;
}


void RecvX6StartChargingNotice(X6_PKT_STR *pMsg)
{
    uint8_t  buff[RF24G_BUFF_PKT_LEN_SEND_TO_X6] = {0};
    TERMINAL_STATUS_STR *pTerMinal = NULL;
    X6_START_NOTICE_STR *pX6Msg = (void*)pMsg->data;

	uint8_t nodeIndex = GetTerminalIdByGunId(pX6Msg->gunId);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    PrintfData("RecvX6StartChargingNotice, 收到X6的充电开启通知", (void*)pMsg, sizeof(X6_PKT_HEAD_STR)+sizeof(X6_START_NOTICE_STR));
    CL_LOG("sn=%d.\n",pMsg->head.sn);
    if ((0 == pX6Msg->gunId) || ((system_info.chargerCnt << 1) < pX6Msg->gunId)) 
	{
        CL_LOG("gunId=%d,error.\n",pX6Msg->gunId);
        return;
    }

    pTerMinal = &gChgInfo.terminalStatus[GetTerminalIdByGunId(pX6Msg->gunId)];
    pTerMinal->heartBeatStatus = 2;
	pTerMinal->sendCnt = 0;
    memcpy(gun_info[pX6Msg->gunId-1].order, pX6Msg->orderId, sizeof(pX6Msg->orderId));
    SendX6StartChargingAck((void*)&buff[sizeof(BLUE_HEAD_STR)], pMsg->head.sn, pX6Msg->gunId);
    //if (pMsg->head.sn != gChgInfo.startChargingNoticeSn)
	{
        gChgInfo.startChargingNoticeSn = pMsg->head.sn;
        SendStartChargingNotice(pMsg);
        SendHeartBeatFun(pX6Msg->gunId);
    }
	//else{
    //    if (20 < gChgInfo.lastRecvStartChargingTimeCnt)
	//	{
    //        gChgInfo.startChargingNoticeSn = pMsg->head.sn;
    //        gChgInfo.lastRecvStartChargingTimeCnt = 0;
    //        SendStartChargingNotice(pMsg);
    //        SendHeartBeatFun(pX6Msg->gunId);
    //    }
    //    CL_LOG("repeat recv start charging notice,gunId=%d,pkt sn=%d,local sn=%d.\n",pX6Msg->gunId,pMsg->head.sn,gChgInfo.startChargingNoticeSn);
    //}
}


uint16_t GetHistoryOrderNum(void)
{
    ORDER_STR *pOrder = &system_info.orderInfo;
    uint16_t orderCnt;

	if((pOrder->wIndex > RECORD_MAX_BLOCK_NUM) || (pOrder->rIndex > RECORD_MAX_BLOCK_NUM))
	{
        CL_LOG("历史订单pOrder->wIndex = [%d], pOrder->rIndex = [%d],错误!\n", pOrder->wIndex, pOrder->rIndex);
		pOrder->wIndex = 0;
		pOrder->rIndex = 0;
		FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
	}
	
    if (pOrder->wIndex >= pOrder->rIndex)
	{
        orderCnt = pOrder->wIndex - pOrder->rIndex + 1;
    }
	else
	{
        orderCnt = pOrder->wIndex + RECORD_MAX_BLOCK_NUM - pOrder->rIndex + 1;
    }
	
	return orderCnt;
}

//直接进行订单缓存，另外的线程发送
void RecvX6StopChargingNotice(X6_PKT_STR *pMsg)
{
    X6_STOP_NOTICE_STR *pStopNotice = (void*)pMsg->data;
    STOP_CHARGING_NOTICE_REQ_STR stopNotice;
    X6_STOP_NOTICE_ACK_STR *pStopNoticeAck = (void*)pMsg->data;
    TERMINAL_STATUS_STR *pTerMinal = NULL;

	uint8_t nodeIndex = GetTerminalIdByGunId(pStopNotice->gunId);	
	if(RECORD_MAX_BLOCK_NUM <= GetHistoryOrderNum())
	{
		return;
	}
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    PrintfData("RecvX6StopChargingNotice充电结束通知,接收X6的订单号", (void*)pMsg, sizeof(X6_PKT_HEAD_STR)+sizeof(X6_STOP_NOTICE_STR));
    CL_LOG("sn=%d.\n",pMsg->head.sn);
    if ((0 == pStopNotice->gunId) || ((system_info.chargerCnt << 1) < pStopNotice->gunId))
	{
        CL_LOG("gunId=%d,error.\n",pStopNotice->gunId);
        return;
    }
    pTerMinal = &gChgInfo.terminalStatus[GetTerminalIdByGunId(pStopNotice->gunId)];
    pTerMinal->heartBeatStatus = 2;
	pTerMinal->sendCnt = 0;

    if ((gChgInfo.stopChargingNoticeSn != pMsg->head.sn) || (((uint32_t)60) < (GetRtcCount() - gChgInfo.gPowerOffNoticeTick[nodeIndex]))) 
	{
		gChgInfo.gPowerOffNoticeTick[nodeIndex] = GetRtcCount();
        gChgInfo.stopChargingNoticeSn = pMsg->head.sn;
        memset(&stopNotice, 0, sizeof(stopNotice));
        stopNotice.gun_id = pStopNotice->gunId;
        stopNotice.stop_reason = pStopNotice->stopReason;
        stopNotice.reasonDetail = pStopNotice->reasonDetal;
        memcpy(stopNotice.order, pStopNotice->orderId, sizeof(stopNotice.order));
        stopNotice.startTime = pStopNotice->startTime;
        stopNotice.stop_time = pStopNotice->startTime + pStopNotice->chargingTime * 60;
        stopNotice.startElec = 0;
        stopNotice.stop_power = pStopNotice->elec;
        stopNotice.charger_cost = pStopNotice->money;
        stopNotice.template_id = system_info.cost_template.template_id;
        stopNotice.chargingPower = pStopNotice->power;
        FlashWriteOrder(&stopNotice, sizeof(stopNotice));
		pStopNoticeAck->result = 0;
    }
	else
	{
        CL_LOG("接收订单序列号相同,gunid=%d.\n",pStopNotice->gunId);
		pStopNoticeAck->result = 1;
    }
    //发送应答
    pStopNoticeAck->gunId = pStopNotice->gunId;
	CL_LOG("repeat recv stop charging notice,gunid=%d.\n",pStopNotice->gunId);
	CL_LOG("给X6发送订单应答.\n");
    SendX6Pkt((pStopNoticeAck->gunId-1) >> 1, pMsg->head.sn, (void*)pMsg, sizeof(X6_STOP_NOTICE_ACK_STR));
    PrintfData("RecvX6StopChargingNotice and send ack", (uint8_t*)pMsg-sizeof(BLUE_HEAD_STR), sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(X6_STOP_NOTICE_ACK_STR));
    SendHeartBeatFun(pStopNotice->gunId);
}


void RecvX6HeartBeatProc(X6_PKT_STR *pMsg)
{
    int i;
    //uint8_t  index;
    X6_HB_ACK_STR *pData = (void*)pMsg->data;
    TERMINAL_STATUS_STR *pTerMinal = NULL;
	uint8_t nodeIndex = GetTerminalIdByGunId(pData->gunStatus[0].gunId);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    PrintfData("接收X6发送过来的心跳应答RecvX6HeartBeatProc", (void*)pMsg, sizeof(X6_PKT_HEAD_STR)+pMsg->head.len);
	//index = GetTerminalIdByGunId(pData->gunStatus[0].gunId);
	#if 0
	if((system_info.isUpgradeOk.x6package_num[index] == system_info.x6_package_num) && (0 != system_info.x6_package_num))
	{
		system_info.isUpgradeOk.UpgradeFinish[index] = 1;
		FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
	}
	else
	{
		system_info.isUpgradeOk.UpgradeFinish[index] = 0;
		FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
	}
	#endif
    if ((0 == pData->gunCnt) || (2 < pData->gunCnt))
	{
        CL_LOG("gunCnt=%d,error.\n",pData->gunCnt);
        return;
    }

    //CL_LOG("index=%d.\n",index);
    if (system_info.chargerCnt < (nodeIndex + 1))
	{
        CL_LOG("gunId=%d,error.\n",pData->gunStatus[0].gunId);
        return;
    }

    pTerMinal = &gChgInfo.terminalStatus[nodeIndex];
    pTerMinal->sendCnt = 0;
    if (2 != pTerMinal->heartBeatStatus) 
	{
        CL_LOG("terminal enum ok,index=%d.\n",GetTerminalIdByGunId(pData->gunStatus[0].gunId));
        pTerMinal->heartBeatStatus = 2;
    }

    nodeIndex = 0;
    for (i=0; i < pData->gunCnt; i++)
	{
        nodeIndex = ProcGunStatus((void*)((uint8_t*)pData->gunStatus + nodeIndex));
    }
}


void CostTemplateDownAck(X6_PKT_STR *pMsg)
{
    COST_TEMPLATE_ACK_STR *pCostAck = (void*)pMsg->data;
	uint8_t nodeIndex = GetTerminalIdByGunId(pCostAck->gunId);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    PrintfData("CostTemplateDownAck", (void*)pCostAck, sizeof(COST_TEMPLATE_ACK_STR));
}


void X6ReqCostTemplate(X6_PKT_STR *pMsg)
{
    REQ_COST_TEMPLATE_STR *pCostReq = (void*)pMsg->data;
    TERMINAL_STATUS_STR *pTerMinal = NULL;

	uint8_t nodeIndex = GetTerminalIdByGunId(pCostReq->gun_id);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    CL_LOG("X6请求计费模板 system_info.chargerCnt = 0x%x, pCostReq->gun_id = 0x%x.\n", system_info.chargerCnt, pCostReq->gun_id);
    if ((0 == pCostReq->gun_id) || ((system_info.chargerCnt << 1) < pCostReq->gun_id))
	{
        CL_LOG("gunId=%d,error.\n",pCostReq->gun_id);
        return;
    }
    pTerMinal = &gChgInfo.terminalStatus[GetTerminalIdByGunId(pCostReq->gun_id)];
    pTerMinal->heartBeatStatus = 2;
	pTerMinal->sendCnt = 0;
    SendX6CostTemplate((pCostReq->gun_id-1)>>1, (void*)((uint8_t*)pMsg - sizeof(BLUE_HEAD_STR)));
}

/*****************************************************************************
** Function name:       SendX6UpgradeReqAck
** Descriptions:        //给X6发送2.4G升级请求应答
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
int SendX6UpgradeReqAck(X6_PKT_STR *pPkt, uint8_t sn, uint8_t gunId, uint8_t result)
{
    ACK_X6_UPGRADE_STR_BTRF2_4G_TYPE *pX6UpgradeAck = (void*)pPkt->data;

    pPkt->head.cmd = CMD_FW_UPGRADE_REQ_24G;
    pX6UpgradeAck->result = result;
    CL_LOG("R6给X6回复升级请求应答 0x0C! gunId = %d \n", gunId);
    SendX6Pkt((gunId-1)>>1, sn, (void*)pPkt, sizeof(ACK_X6_UPGRADE_STR_BTRF2_4G_TYPE));

    return CL_OK;
}

//pPkt前面有2.4G报文头
int SendX6BindReq(X6_PKT_STR *pPkt, uint8_t sn, uint8_t gunId)
{
    R6X6_BIND_REQ_STR_24G_TYPE *pX6BindReq = (void*)pPkt->data;

    pPkt->head.cmd = CMD_ENUM_BIND_REQ;
    pX6BindReq->ServerTime = GetRtcCount();
    SendX6Pkt((gunId-1)>>1, sn, (void*)pPkt, sizeof(X6_AUTH_ACK_STR));
    PrintfData("SendX6AuthReqAck, 给X6发送绑定请求", (void*)((uint8_t*)pPkt-sizeof(BLUE_HEAD_STR)), sizeof(BLUE_HEAD_STR)+sizeof(X6_PKT_HEAD_STR)+sizeof(X6_AUTH_ACK_STR));
    return CL_OK;
}

/*****************************************************************************
** Function name:       ReceiveX6FwUpgrade2_4gAck
** Descriptions:        //收到X6发送过来的固件升级应答
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
void ReceiveX6FwUpgrade2_4gAck(X6_PKT_STR *pMsg)
{
    X6_FW_UPGRADE_ACK_STR_24G *pFwUpgradeAck  = (void*)pMsg->data;

    if(0 == pFwUpgradeAck->UpgeadeState)
    {
        CL_LOG("收到X6发送过来的固件升级应答, 可升级标志! \n");
        gChgInfo.flagBit.isSendFwUpgradeOk = 1;
        gChgInfo.flagBit.Upgrade2_4Gflag = 1;   //能升级
    }
    else{
        CL_LOG("收到X6发送过来的固件升级应答, 不可以升级标志! \n");
        //gChgInfo.flagBit.isSendFwUpgradeOk = 0;
        gChgInfo.flagBit.Upgrade2_4Gflag = 0;   //不能升级
    }
}

/*****************************************************************************
** Function name:       ReceiveX6BindAck
** Descriptions:        //收到X6发送过来的绑定应答
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
void ReceiveX6BindAck(X6_PKT_STR *pMsg)
{
    R6X6_BIND_ACK_STR_24G_TYPE *pX6BindAck  = (void*)pMsg->data;
    static uint8_t SerialNum = 205;

	PrintfData("绑定接收X6设备号", (uint8_t *)pX6BindAck->deviceID, sizeof(pX6BindAck->deviceID));
	CL_LOG("绑定接收X6起始枪头号baseGunSn = %d \n", pX6BindAck->baseGunSn);
	if(SerialNum == pMsg->head.sn)
    {
        CL_LOG("收到X6绑定序列号相同的包, 返回! SerialNum = %d \n", SerialNum);
        return;
    }
	CL_LOG("接收报文序列号sn = %d \n", pMsg->head.sn);
	if(0 == memcmp(system_info.chargerInfo[GetTerminalIdByGunId(pX6BindAck->baseGunSn)].chargerSn, pX6BindAck->deviceID, 5))
    {
        if(SerialNum != pMsg->head.sn)
        {
            SerialNum = pMsg->head.sn;
            gChgInfo.flagBit.ReceiveX6BindMsg = 1;
        }
    }
}

/*****************************************************************************
** Function name:       ReceiveX6FwUpgradeDownload2_4gAck
** Descriptions:        //收到X6发送过来的固件下发应答
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
void ReceiveX6FwUpgradeDownload2_4gAck(X6_PKT_STR *pMsg)
{
    X6_DOWN_FW_ACK_STR_24G *pFwUpgradeDownloadAck  = (void*)pMsg->data;
    static uint8_t SerialNum = 255;

	uint8_t nodeIndex = GetTerminalIdByGunId(pFwUpgradeDownloadAck->gunId);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    PrintfData("收到X6发送过来的固件下发应答ReceiveX6FwUpgradeDownload2_4gAck", (void*)pMsg, sizeof(X6_PKT_HEAD_STR) + sizeof(X6_DOWN_FW_ACK_STR_24G) + 3);
    if(SerialNum == pMsg->head.sn)
    {
        CL_LOG("收到X6发送过来的固件下发应答, 收到X6序列号相同的包, 返回! \n");
        return;
    }
    if(0 == pFwUpgradeDownloadAck->result)
    {
        if(SerialNum != pMsg->head.sn)
        {
            SerialNum = pMsg->head.sn;
            
            gChgInfo.current_package[GetTerminalIdByGunId(pFwUpgradeDownloadAck->gunId)] = pFwUpgradeDownloadAck->package_num;
			system_info.isUpgradeOk.x6package_num[GetTerminalIdByGunId(pFwUpgradeDownloadAck->gunId)] = pFwUpgradeDownloadAck->package_num;
            CL_LOG("收到X6发送过来的固件下发应答, 可以升级标志! X6请求包序号 %d, gunId = %d\n", gChgInfo.current_package[GetTerminalIdByGunId(pFwUpgradeDownloadAck->gunId)], pFwUpgradeDownloadAck->gunId);
        }
    }
    else if((1 == pFwUpgradeDownloadAck->result) || (2 == pFwUpgradeDownloadAck->result))
    {
		if((2 == pFwUpgradeDownloadAck->result) && (0 != system_info.x6_package_num) &&
			(system_info.isUpgradeOk.x6package_num[GetTerminalIdByGunId(pFwUpgradeDownloadAck->gunId)] == system_info.x6_package_num))
		{
			system_info.isUpgradeOk.UpgradeFinish[GetTerminalIdByGunId(pFwUpgradeDownloadAck->gunId)] = 1;
		}
        CL_LOG("收到X6发送过来的固件下发应答, 不可以升级标志! \n");
        gChgInfo.flagBit.isSendFwUpgradeOk = 0;
        gChgInfo.flagBit.Upgrade2_4Gflag = 0;
    }
    //else if(2 == pFwUpgradeDownloadAck->result)
    //{
    //    CL_LOG("收到X6发送过来的固件下发应答, 关闭升级标志! \n");
    //    gChgInfo.flagBit.isSendFwUpgradeOk = 0;
    //    gChgInfo.flagBit.Upgrade2_4Gflag = 0;   //不能升级
    //}
    FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
}

/*****************************************************************************
** Function name:       SendX6FwUpgradeDownload2_4gNotice
** Descriptions:        //给X6下发要升级的分片包
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
void SendX6FwUpgradeDownload2_4gNotice(uint8_t gunId)
{
    uint8_t  buff[RF24G_BUFF_PKT_LEN_SEND_TO_X6] = {0};
    uint32_t  OffsetAdd;
    X6_PKT_STR *pPkt = (void*)&buff[sizeof(BLUE_HEAD_STR)];
    X6_DOWN_FW_REQ_STR_24G *pFwUpgradeDownloadAck  = (void*)pPkt->data;
    uint8_t  X6NodeIndex = GetTerminalIdByGunId(gunId);

    pPkt->head.cmd = CMD_FW_DOWN_LOAD_24G;
    
    #if 0
    if(0 != (system_info.x6FwInfo.size % PIECE_PKT_LEN_24G_SEND_TO_X6))
    {
        system_info.x6_package_num = (system_info.x6FwInfo.size / PIECE_PKT_LEN_24G_SEND_TO_X6) + 1;
    }
    else{
        system_info.x6_package_num = system_info.x6FwInfo.size / PIECE_PKT_LEN_24G_SEND_TO_X6;
    }
    #endif
    
    if(gChgInfo.current_package[X6NodeIndex] > system_info.x6_package_num)
    {
        CL_LOG("X6请求的包序号大于最大包序号! current_package[%d] = %d, x6_package_num = %d \n", gunId, gChgInfo.current_package[X6NodeIndex], system_info.x6_package_num);
        gChgInfo.current_package[X6NodeIndex] = 0;	//清除变量
        return;
    }
    OffsetAdd = (gChgInfo.current_package[X6NodeIndex] - 1) * PIECE_PKT_LEN_24G_SEND_TO_X6;
    HT_Flash_ByteRead(&pFwUpgradeDownloadAck->data[0], TERMINAL_APP_ADDR + OffsetAdd, PIECE_PKT_LEN_24G_SEND_TO_X6);
    pFwUpgradeDownloadAck->checkSum = GetPktSum(&pFwUpgradeDownloadAck->data[0], PIECE_PKT_LEN_24G_SEND_TO_X6);
    pFwUpgradeDownloadAck->len = PIECE_PKT_LEN_24G_SEND_TO_X6;
    pFwUpgradeDownloadAck->package_num = gChgInfo.current_package[X6NodeIndex];
    CL_LOG("X6请求包序号! current_package[%d] = %d \n", X6NodeIndex, gChgInfo.current_package[X6NodeIndex]);
    SendX6Pkt(X6NodeIndex, system_info.x6PktSn++, (void*)pPkt, sizeof(X6_DOWN_FW_REQ_STR_24G));
}

/*****************************************************************************
** Function name:       SendX6FwUpgrade2_4gNotice
** Descriptions:        //给X6发送升级的文件大小, 固件分片总数, 校验码, 固件版本号
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
void SendX6FwUpgrade2_4gNotice(X6_PKT_STR *pPkt, uint8_t gunId)
{
    X6_FW_UPGRADE_NOTI_REQ_STR_24G *pFwUpgradeNotice  = (void*)pPkt->data;
    uint32_t i;
    
    pFwUpgradeNotice->file_size = system_info.x6FwInfo.size;
    if(0 != (system_info.x6FwInfo.size % PIECE_PKT_LEN_24G_SEND_TO_X6))
    {
        system_info.x6_package_num = (system_info.x6FwInfo.size / PIECE_PKT_LEN_24G_SEND_TO_X6) + 1;
    }
    else{
        system_info.x6_package_num = system_info.x6FwInfo.size / PIECE_PKT_LEN_24G_SEND_TO_X6;
    }
    //CL_LOG("R6给X6发送升级的文件大小, 固件分片总数, 校验码, 固件版本号 0x0A! gunId = %d \n", gunId);
    if((0 < system_info.x6FwInfo.size) && (0 < system_info.x6_package_num))
    {
        pFwUpgradeNotice->package_num = system_info.x6_package_num;
        //pFwUpgradeNotice->checkSum = 0;
        pFwUpgradeNotice->checkSum = system_info.x6FwInfo.checkSum;
        pFwUpgradeNotice->fwverson = system_info.x6FwInfo.ver;
        CL_LOG("文件大小size = %d, 分片总数num = %d, 校验码Sum = 0x%x, 版本号fwver = %d \n", pFwUpgradeNotice->file_size, pFwUpgradeNotice->package_num, pFwUpgradeNotice->checkSum, pFwUpgradeNotice->fwverson);
        pPkt->head.cmd = CMD_DFU_DOWN_FW_INFO;
        gChgInfo.flagBit.isSendFwUpgradeOk = 0;
        for (i = 0; i < (RESENT_24G_MSG_CNT*RETRY_24G_MAX_CNT); i++)
        {
            if (0 == (i % RESENT_24G_MSG_CNT))
            {
                SendX6Pkt((gunId-1)>>1, system_info.x6UpgradePktSn, (void*)pPkt, sizeof(X6_FW_UPGRADE_NOTI_REQ_STR_24G));
            }
            vTaskDelay(RETRY_24G_DELAY_TIME);
            if (1 == gChgInfo.flagBit.isSendFwUpgradeOk) 
			{
                break;
            }
        }
        system_info.x6UpgradePktSn++;
    }
    else{
        CL_LOG("文件大小size = %d, 分片总数num = %d, 校验码Sum = 0x%x, 版本号fwver = %d \n", system_info.x6FwInfo.size, system_info.x6_package_num, system_info.x6FwInfo.checkSum, system_info.x6FwInfo.ver);
    }
}

/*****************************************************************************
** Function name:       X6Req2_4gUpgrade
** Descriptions:        //X6请求固件升级
** input parameters:    pMsg
** output parameters:   None
** Returned value:		None
** Author:              quqian
*****************************************************************************/
void X6Req2_4gUpgrade(X6_PKT_STR *pMsg)
{
    uint8_t  buff[RF24G_BUFF_PKT_LEN_SEND_TO_X6] = {0};
    X6_REQ_UPGRADE_STR_BTRF2_4G_TYPE *pUpgradeReq = (void*)pMsg->data;
	static uint8_t timesFlag[MAX_TERMINAL_NUM];
    uint8_t result;
    uint8_t nodeIndex = GetTerminalIdByGunId(pUpgradeReq->gun_id);
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    #if 1
    if((pUpgradeReq->ReqFwVer == system_info.x6FwInfo.ver) && (0 != system_info.x6FwInfo.ver))
    {
        CL_LOG("版本号相同, 不允许升级 timesFlag[%d] = %d\n", nodeIndex, timesFlag[nodeIndex]);
        if((5 < timesFlag[nodeIndex]++) || (system_info.isUpgradeOk.x6package_num[nodeIndex] == system_info.x6_package_num))
        {
            system_info.isUpgradeOk.UpgradeFinish[nodeIndex] = 1;	//
        }
        result = 1;
    }
    else{
        CL_LOG("版本号不相同, 允许升级 \n");
        result = 0;
    }
    #endif
    CL_LOG("升级版本号 ReqFwVer = %d ,老版本号 ver = %d \n", pUpgradeReq->ReqFwVer, system_info.x6FwInfo.ver);
    //if(GetRtcCount() < timeTicks)
    //{
    //    timeTicks = GetRtcCount();
    //}
    //if(((155 < (GetRtcCount() - timeTicks)) && (gChgInfo.gun_id != pUpgradeReq->gun_id)) || (gChgInfo.gun_id == pUpgradeReq->gun_id))
    //{
    //    timeTicks = GetRtcCount();
        SendX6UpgradeReqAck((void*)&buff[sizeof(BLUE_HEAD_STR)], system_info.x6PktSn++, pUpgradeReq->gun_id, result);
    //}
    //else{
    //    SendX6UpgradeReqAck((void*)&buff[sizeof(BLUE_HEAD_STR)], system_info.x6PktSn++, pUpgradeReq->gun_id, 1);
    //    CL_LOG("上个枪头正在升级! \n");
    //    return;
    //}
    
    if(0 == result)
    {
        gChgInfo.gun_id = pUpgradeReq->gun_id;
        SendX6FwUpgrade2_4gNotice((void*)&buff[sizeof(BLUE_HEAD_STR)], pUpgradeReq->gun_id);
    }
}


void RxX6EventNotice(X6_PKT_STR *pMsg)
{
    X6_EVENT_NOTICE_REQ_STR *pEventNotice = (void*)pMsg->data;
    TERMINAL_STATUS_STR *pTerMinal = NULL;

	uint8_t nodeIndex = GetTerminalIdByGunId(pEventNotice->gunId);	
	
	gChgInfo.terminalStatus[nodeIndex].recvCnt++;
    PrintfData("RxX6EventNotice", (void*)pEventNotice, sizeof(X6_EVENT_NOTICE_REQ_STR));
    if ((0 == pEventNotice->gunId) || ((system_info.chargerCnt << 1) < pEventNotice->gunId)) 
	{
        CL_LOG("gunId=%d,error.\n",pEventNotice->gunId);
        return;
    }
    pTerMinal = &gChgInfo.terminalStatus[GetTerminalIdByGunId(pEventNotice->gunId)];
    pTerMinal->heartBeatStatus = 2;
	pTerMinal->sendCnt = 0;
}


void RecvX6Data(uint8_t *pBuff, uint16_t len)
{
    X6_PKT_STR *pMsg = (void*)pBuff;

	CL_LOG("收到X6的2.4g数据 cmd = %d \n", pMsg->head.cmd);
	gChgInfo.ReceiveR624gDataFlag = 0xa5;
    switch (pMsg->head.cmd) 
	{
        case CMD_AUTH_REQ:
        RecvX6AuthReqProc(pMsg);
        break;

        case CMD_AUTH_RESULT:
        RecvX6AuthResultProc(pMsg);
        break;

        case CMD_REMOTE_SET_POWER_ON:
        RecvX6ReqStartChargingAck(pMsg);
        break;

        case CMD_REMOTE_SET_POWER_OFF:
        RecvX6ReqStopChargingAck(pMsg);
        break;

        case CMD_REPORT_POWER_ON:
        RecvX6StartChargingNotice(pMsg);
        break;

        case CMD_REPORT_POWER_OFF:
        RecvX6StopChargingNotice(pMsg);
        break;

        case CMD_HEART_BEAT:
        RecvX6HeartBeatProc(pMsg);
        break;

        case CMD_COST_DOWN:
        CostTemplateDownAck(pMsg);
        break;

        case CMD_COST_REQ:
        X6ReqCostTemplate(pMsg);
        break;

        case CMD_EVENT_NOTICE:
        RxX6EventNotice(pMsg);
        break;

        case CMD_DFU_DOWN_FW_INFO:
            ClearUpgradeWaitTimer();
            ReceiveX6FwUpgrade2_4gAck(pMsg);
        break;

        case CMD_FW_DOWN_LOAD_24G:
            ClearUpgradeWaitTimer();
            ReceiveX6FwUpgradeDownload2_4gAck(pMsg);
        break;
        
        case CMD_FW_UPGRADE_REQ_24G:
            ClearUpgradeWaitTimer();
            X6Req2_4gUpgrade(pMsg);
        break;
		case CMD_ENUM_BIND_REQ:
			ReceiveX6BindAck(pMsg);
        break;
    }
}


