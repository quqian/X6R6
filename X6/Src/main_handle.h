#ifndef __MAIN_HANDLE_H__
#define __MAIN_HANDLE_H__

#include "stdint.h"


#define STR_MAC_LENTH                       11
#define TIME_OF_SECONDS                     2

#define CHARGING_FULL_TIME                  (45*60) //��



#define RECORD_BLOCK_SIZE                           30720     //��������40960�ֽ�(30k)
#define SPI_FLASH_PAGE_SIZE					        1024    //flashҳ��С
#define	RECORD_BLOCK_SIZE_PER_PAGE		            128     //һ��������󻺴�
#define	PER_PAGE_RECORD_COUNT		                (uint32_t)(8)     //ÿҳ�ܴ洢�Ķ�����
#define RECORD_MAX_BLOCK_NUM                        (uint32_t)(240)     //����¼��������

extern uint32_t isCheckTimeOverFlag;

/*****************************************************************************
** Function name:       SaveOrderReport
** Descriptions:        //���涩����¼
** input parameters:    None
** output parameters:   None
** Returned value:	    None
** Author:              quqian
*****************************************************************************/
extern void SaveOrderReport(uint8_t gun_id);
/*****************************************************************************
** Function name:       SendOrderReport
** Descriptions:        //���ͼ�¼����
** input parameters:    None
** output parameters:   None
** Returned value:	    None
** Author:              quqian
*****************************************************************************/
extern int SendOrderReport(void);
/*****************************************************************************
** Function name:       OpenCheckTimeOver
** Descriptions:        �򿪼��ˢ�������������������Ƿ�ʱ, ��ʱ��������ʧ��
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void OpenCheckTimeOver(void);

/*****************************************************************************
** Function name:       CloseCheckTimeOver
** Descriptions:        �رռ��ˢ�������������������Ƿ�ʱ
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void CloseCheckTimeOver(void);
/*****************************************************************************
** Function name:       isTheSameCard
** Descriptions:        ������ͬ���� 1
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern int isTheSameCard(void);

/*****************************************************************************
** Function name:       SetReadCardFlag
** Descriptions:        
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void SetReadCardFlag(void);

/*****************************************************************************
** Function name:       CleanReadCardFlag
** Descriptions:        
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void CleanReadCardFlag(void);

/*****************************************************************************
** Function name:       GetReadCardFlag
** Descriptions:        
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern int GetReadCardFlag(void);

/*****************************************************************************
** Function name:       SetReadCardTime
** Descriptions:        
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void SetReadCardTime(void);

/*****************************************************************************
** Function name:       CleanReadCardTime
** Descriptions:        
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void CleanReadCardTime(void);

/*****************************************************************************
** Function name:       GetReadCardTime
** Descriptions:        ���Ϊ 1 ��ʾ n ��û�ж�����, ���жϿ��Ѿ��ƿ��������ͬһ�ſ���
                        ˵����ˢ���ڶ��Σ��������־��Ŀ�ľ���Ϊ�˷�ֹͬһ�ſ�ˢ���ε�ʱ��Ҫ�ȴ� T ��
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern int GetReadCardTime(void);
extern void Sc8042bSpeechStartChargerFail(void);
extern void GreenLedToggle(void);
extern void RestoreGunWithChargingCtrlProc(void);

#endif  /* __MAIN_HANDLE_H__ */

