/**
  ******************************************************************************
  * @file    main.c
  * @author  liutao
  * @version V1.0
  * @date    17-October-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2017 </center></h2>
  ******************************************************************************
  */

#include "includes.h"
#include "simuart.h"
#include "blue.h"
#include "led.h"



//******************************************************************
//! \brief  	delay
//! \param  	none
//! \retval
//! \note
//! \note
//******************************************************************
void delay(volatile int tick)
{
    while(tick--);
}

//******************************************************************
//! \brief  	SystemClockInit
//! \param  	none
//! \retval
//! \note   	初始化系统时钟
//! \note
//******************************************************************
void SystemClockInit(void)
{
	HT_PMU->RSTSTA = 0x0000;
	Feed_WDT();
	EnWr_WPREG();
	HT_CMU->CLKCTRL0 &= ~0x0380;//关闭HRC_DET,PLL_DET,LF_DET
	DisWr_WPREG();
	SwitchTo_Fpll();
	Close_Hrc();
}

char *BCDToString(char *dest, unsigned char *BCD, int bytes) 
{
    char  temp[] = "0123456789ABCDEF";
    int index = 0;
    int length = 0;
    if (BCD == NULL || bytes <= 0)
        return NULL;
    
    for (index = 0; index < bytes; index++) {
        dest[length++] = temp[(BCD[index] >> 4) & 0x0F];
        dest[length++] = temp[BCD[index] & 0x0F];
    }
    dest[length] = '\0';
    return dest;
}

void ShowCostTemplate(void)
{
    int i;
    multiPower_t *pMultiPower = &system_info.cost_template.Data.powerInfo;
    segment_str  *pSegment = pMultiPower->segmet;
    unify_t *pUnify = &system_info.cost_template.Data.unifyInfo;

    if (COST_POWER == system_info.cost_template.mode) {
        for (i=0; i<pMultiPower->segmentCnt; i++) {
            CL_LOG("cost template info, power %d.\n",i+1);
            CL_LOG("startPower=%d.\n",pSegment->startPower);
            CL_LOG("endPower  =%d.\n",pSegment->endPower);
            CL_LOG("price     =%d.\n",pSegment->price);
            CL_LOG("duration  =%d.\n",pSegment->duration);
            pSegment++;
        }
    }else if (COST_UNIFY == system_info.cost_template.mode) {
        CL_LOG("cost template info, unify.\n");
        CL_LOG("price     =%d.\n",pUnify->price);
        CL_LOG("duration  =%d.\n",pUnify->duration);
    }else{
        CL_LOG("no cost template,error.\n");
    }
}

void SysCfgInit(void)
{
	uint32_t i = 0;
	uint8_t gateWayAddr[MAC_LEN];	//本地2.4mac地址
	
    system_info.is_socket_0_ok = 0;
    system_info.tcp_tx_error_times = 0;
    system_info.isRecvStartUpAck = 0;
    system_info.printfSwitch = 1;
    memcpy(system_info.server_ip, NET_SERVER_IP, strlen(NET_SERVER_IP));
    system_info.server_ip[strlen(NET_SERVER_IP)] = 0;
    system_info.server_port = NET_SERVER_PORT;

    CL_LOG("fw_version  = %03d, subVersion=%d.\n",FW_VERSION, SUB_FW_VERSION);
    CL_LOG("server_ip   = %s.\n", system_info.server_ip);
    CL_LOG("server_port = %d.\n", system_info.server_port);
    PrintfData("设备号charger sn", (void*)system_info1.station_id, sizeof(system_info1.station_id));
	#if 0
    CL_LOG("station_id[0] = %02x.\n",system_info.station_id[0]);
    CL_LOG("station_id[1] = %02x.\n",system_info.station_id[1]);
    CL_LOG("station_id[2] = %02x.\n",system_info.station_id[2]);
    CL_LOG("station_id[3] = %02x.\n",system_info.station_id[3]);
    CL_LOG("station_id[4] = %02x.\n",system_info.station_id[4]);
    CL_LOG("station_id[5] = %02x.\n",system_info.station_id[5]);
    CL_LOG("station_id[6] = %02x.\n",system_info.station_id[6]);
    CL_LOG("station_id[7] = %02x.\n",system_info.station_id[7]);
	#endif
    PrintfData("device idCode", (void*)system_info.idCode, sizeof(system_info.idCode));
    
	gateWayAddr[0] = system_info1.gwAddr[4];
	gateWayAddr[1] = system_info1.gwAddr[3];
	gateWayAddr[2] = system_info1.gwAddr[2];
	gateWayAddr[3] = system_info1.gwAddr[1];
	gateWayAddr[4] = system_info1.gwAddr[0];
	PrintfData("网关地址", (void*)gateWayAddr, sizeof(system_info1.gwAddr));
    CL_LOG("chargerCnt  = %d.\n",system_info.chargerCnt);
    for (i = 0; i < system_info.chargerCnt; i++)
	{
        PrintfData("X6设备号charger sn", (void*)system_info.chargerInfo[i].chargerSn, sizeof(system_info.chargerInfo[i].chargerSn));
    }
}

int CheckSum(uint8_t *pData, uint16_t len)
{
    int i;
    uint8_t sum = 0; 

    for (i=0; i<len; i++) 
	{
        sum += pData[i];
    }
    return sum;
}

void LoadSysCfgInfo(void)
{
	uint32_t timesFlag = 0;
	uint32_t sum[3] = {0, 1, 2};
	uint32_t i = 0;
	system_info_t SystemInfo;
	system_info_str1 System_Info1;
	
	while(1)
	{
		for(i = 0; i < 3; i++)
		{
			Feed_WDT();
			memset(&SystemInfo, 0, sizeof(SystemInfo));
			FlashReadSysInfo(&SystemInfo, sizeof(SystemInfo));
			sum[i] = CheckSum((uint8_t*)&SystemInfo, sizeof(SystemInfo));
		}
		
		if((sum[0] == sum[1]) && (sum[0] == sum[2]))
		{
			memcpy(&system_info, &SystemInfo, sizeof(SystemInfo));
			printf("读system_info数据成功! .\n");
			break;
		}
		else
		{
			printf("读system_info数据失败! .\n");
		}
		if(20 < timesFlag++)
		{
			memcpy(&system_info, &SystemInfo, sizeof(SystemInfo));
			printf("system_info读数据失败! .\n");
			break;
		}
		OS_DELAY_MS(40);
	}

	while(1)
	{
		for(i = 0; i < 3; i++)
		{
			Feed_WDT();
			memset(&System_Info1, 0, sizeof(System_Info1));
			FlashReadSysInfo1(&System_Info1, sizeof(System_Info1));
			sum[i] = CheckSum((uint8_t*)&System_Info1, sizeof(System_Info1));
			PrintfData("网关地址", System_Info1.gwAddr, sizeof(System_Info1.gwAddr));
		}
		
		if((sum[0] == sum[1]) && (sum[0] == sum[2]))
		{
			memcpy(&system_info1, &System_Info1, sizeof(System_Info1));
			printf("读system_info1数据成功! .\n");
			break;
		}
		else
		{
			printf("读system_info1数据失败! .\n");
		}
		if(20 < timesFlag++)
		{
			memcpy(&system_info1, &System_Info1, sizeof(System_Info1));
			printf("system_info1读数据失败! .\n");
			break;
		}
		OS_DELAY_MS(40);
	}
	
    if ((MAGIC_NUM_BASE) == system_info.magic_number)
	{
        CL_LOG("second start.\n");
        SysCfgInit();
    }
	else
	{
        CL_LOG("first start.\n");
        memset((void*)&system_info, 0, sizeof(system_info_t));
        system_info.magic_number = MAGIC_NUM_BASE;
        SysCfgInit();
        FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
    }
}


int StartProc(void)
{
    uint8_t  flag;
    uint32_t oldTime = 0;
	
    while(1)
	{
        Feed_WDT();
        FlashLight();
        if (system_info.is_socket_0_ok == 1)
		{
            break;
        }
        OS_DELAY_MS(1000);
    }
	//if(memcmp(system_info.saveServerIP, system_info.server_ip, strlen(NET_SERVER_IP)))
	//{
	//	CL_LOG("与上次ip地址不同，需要重新注册\n");
	//	memcpy(system_info.saveServerIP, NET_SERVER_IP, strlen(NET_SERVER_IP));
	//	memset(system_info.idCode, 0, sizeof(system_info.idCode));
	//	FlashWriteSysInfo(&system_info, sizeof(system_info), 1);
	//}
    while(1)
	{
        OS_DELAY_MS(1000);

        if (60 <= (GetRtcCount() - oldTime))
		{
			oldTime = GetRtcCount();
            CL_LOG("systick=%d.\n",GetRtcCount());
            if (memcmp(gZeroArray, system_info.idCode, sizeof(system_info.idCode))) 
			{
                flag = 0;
                SendStartUpNotice(1);
            }
			else
			{
                flag = 1;
                SendRegister();
            }
        }

        Feed_WDT();
        FlashLight();

        if ((0 == flag) && (1 == system_info.isRecvStartUpAck))
		{
            return CL_OK;
        }
    }
}


void MainTask(void)
{
    uint32_t oldTick;
	uint32_t OrderReportTick;
	uint32_t HeartBeatTick;
	uint32_t CostTemplateReqTick;
	uint8_t CostTemplateReqFlag;
    int ret = 0;
	
    printf("MainTask start!\n");
    LedInit();
    //SimuartInit();
    OS_DELAY_MS(1500);
    LED_BLUE_LIGHT_ON();
    LED_RED_LIGHT_OFF();
    LED_GREEN_LIGHT_OFF();
	
    printf("\n");
    InitTmp();
    RtcInit();
    LoadSysCfgInfo();
    BlueInit();
    sysTickRecover();
    vTaskDelay(500);
    W25Q80_Init();
	
    ret = xTaskCreate((TaskFunction_t)ServerTask, "ServerTask", 300, NULL, 1, NULL);
    vTaskDelay(500);
    ret |= xTaskCreate((TaskFunction_t)BlueTask, "BlueTask", 400, NULL, 1, NULL);
    vTaskDelay(500);
    ret |= xTaskCreate((TaskFunction_t)PktRecvTask, "PktRecvTask", 250, NULL, 1, NULL);
    vTaskDelay(500);
	CL_LOG("task init, ret=%d.\n",ret);

    StartProc();

    SendEventNotice(0, EVENT_START, 0, 0, EVENT_OCCUR, NULL);
    //ShowCostTemplate();
    OS_DELAY_MS(1000);
	while(1) 
	{
        vTaskDelay(300);
        
        //if (oldTick != GetRtcCount()) 
		{
            oldTick = GetRtcCount();
            Feed_WDT();
            FlashLight();
            if (system_info.is_socket_0_ok) 
			{
                if (20 < (oldTick - OrderReportTick)) 
				{
					OrderReportTick = oldTick;
                    OrderReport();
                }
            }
			
            if (40 < (oldTick - HeartBeatTick)) 
			{
				HeartBeatTick = oldTick;
                if (0 == gChgInfo.sendPKtFlag) 
				{
                    HeartBeatHandle();
                }
    	    }
            
			if (60 < (oldTick - CostTemplateReqTick)) 
			{
				CostTemplateReqTick = oldTick;
                CostTemplateReq();
				CheckBingRelation();
				if (60 < CostTemplateReqFlag++) 
				{
					CostTemplateReqFlag = 0;
					{
						SendReqCostTemplate(0);
					}
	            }
			}
        }
	}
}


//******************************************************************
//! \brief  	main
//! \param
//! \retval
//! \note   	主函数 初始化的函数不要进行打印
//******************************************************************
int main(void)
{
    Feed_WDT();
	SystemClockInit();
	//SysTick_Init();
	delay(0x1FFF);
    UsartInit();
	delay(0x1FFF);
    //由于系统资源限制，目前不能启动大于5个线程
    xTaskCreate((TaskFunction_t)MainTask,"MainTask",350,NULL,1,NULL);
    vTaskStartScheduler();
}


