#include "emu_info.h"
#include "flash_usr.h"
#include "system_info.h"
#include "charger.h"

GUN_STATUS_STR gun[GUN_NUM_MAX];
EMU_CTRL_STR  gEmuCtrl[GUN_NUM_MAX];

void ReadEMU_Info(void);

//��ʼ����
uint32_t startElec[GUN_NUM_MAX];

//��������ۼӵ���--��λ0.01KWH/360000
uint64_t SumElec[GUN_NUM_MAX];

uint64_t SumElec_Last[GUN_NUM_MAX];//��¼��һ��ֵ�������ж��Ƿ񳬹�һ����Χ�������

//�����仯��
uint32_t EP_delta[GUN_NUM_MAX];


//��ȡ��������
void ReadEMU_Info(void)
{
    for(int i =0;i<GUN_NUM_MAX;i++)
    {
        gun[i].current = ReadRMSI(i+1);
        gun[i].power = ReadPower(i+1);
        gun[i].voltage = ReadRMSU();
    }
}
					

/*****************************************************************************
** Function name:       CalcuElec
** Descriptions:        //ʵʱ�������---��Ҫ�ڶ�ʱ����1s����һ��
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
void CalcuElec(void)
{
	uint32_t i = 0;
	
    ReadEMU_Info();
	
    for (i = 0; i < GUN_NUM_MAX; i++)
	{
        EP_delta[i] += (uint32_t)(gun[i].power*1);
        if(EP_delta[i] >= 360000)
		{//0.01�ȵı仯��
            SumElec[i] += EP_delta[i];
            EP_delta[i] = 0;
            gun[i].elec= startElec[i] + SumElec[i]/360000;
        }
		EMU_CTRL_STR *pEmu = &gEmuCtrl[i];
		//��ѹ
		if(gun[i].voltage >= 2600) 
		{
			if (5 <= ++pEmu->overvolcount) 
			{
				setbit(gun[i].status,2);
			}
		}

		if(gun[i].voltage <= 1600)
		{
			if (5 <= ++pEmu->lowvolCnt) 
			{
				setbit(gun[i].status,3);
			}
			//����˿�۶�
			if(gun[i].voltage < 300)
			{
				if (5 <= ++pEmu->fuseCnt) 
				{
					setbit(gun[i].status,3);
					gChgInfo.RF24GCardFlag.fuseState = 1;
				}
			}
			else
			{//Ƿѹ
				if (5 <= ++pEmu->lowvolCnt) 
				{
					setbit(gun[i].status,3);
				}
			}
		}
		//
		if(gun[i].voltage > 1600 && gun[i].voltage < 2600)	//��ѹ������ΧΪ 160V`260V
		{
			pEmu->overvolcount = 0;
			pEmu->lowvolCnt = 0;
			clrbit(gun[i].status, 2);
			clrbit(gun[i].status, 3);
			gChgInfo.RF24GCardFlag.fuseState = 0;
		}
		//����
		if(gun[i].current >= 6000) 	//�������Ϊ6A
		{
			if (5 <= ++pEmu->overcurcount)
			{
				setbit(gun[i].status,1);
			}
		}
		else
		{
			pEmu->overcurcount = 0;
			clrbit(gun[i].status, 1);
		}
    }
}

/*****************************************************************************
** Function name:       SaveElec
** Descriptions:        һ��Сʱ����һ�ε�����gun[i].elec�� flash
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
void SaveElec(void)
{
    EE_ENERGY_ST st;
    st.flag = FLAG_ELEC;
    for(int i = 0; i < GUN_NUM_MAX; i++)
    {
        st.elec[i] = gun[i].elec;
    }
	FlashWriteData(ENERGY_PART, (void*)&st, sizeof(st));
}

void ReadElec(void)
{
    EE_ENERGY_ST st;
	EE_ENERGY_ST EnegyStruct;
	
	memset(&st, 0, sizeof(EE_ENERGY_ST));
    //ǹͷEMU���ݳ�ʼ��
	memset(&gun,0,sizeof(GUN_STATUS_STR));
	ReadFlashInfoData(ENERGY_PART, (uint8_t*)&st, (uint8_t*)&EnegyStruct , sizeof(st));
    if(FLAG_ELEC == st.flag)
	{

        for(int i = 0;i<GUN_NUM_MAX;i++)
		{
            gun[i].elec = st.elec[i];
            startElec[i] = gun[i].elec;
        }
    }
	else
	{
        for(int i = 0;i<GUN_NUM_MAX;i++)
		{
            gun[i].elec= 0;
            startElec[i] = 0;
        }
    }
}


//��Ҫ���������е��ã����������eeprom
void ProcessElec(void)
{
	uint32_t i = 0;
	
	for(i = 0; i < GUN_NUM_MAX; i++)
  	{
		if(SumElec[i] - SumElec_Last[i] >= 360000)
		{//����0.01�ȵ����ı仯�ͱ���һ��
      		SaveElec();//�������---����������нӿڵĵ���
      		for(int i = 0;i<GUN_NUM_MAX;i++)
			{
				SumElec_Last[i] = SumElec[i];
      		}
      		break;
		}
	}
}

//gun_num 1-MAX
int GetGunStatus(int gun_id, GUN_STATUS_STR *pGunStatus)
{
    memcpy((void*)pGunStatus, (void*)&gun[gun_id-1], sizeof(GUN_STATUS_STR));
	
    return CL_OK;
}

