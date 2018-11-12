#ifndef __CKB24_UM_HANDLE_H__
#define __CKB24_UM_HANDLE_H__

#include "includes.h"
#include "ckb24_um.h"
#include "24g_proto.h"
#include "uart.h"


#define CKB_MAX_PKT_LEN	256

#define OUTER_R_N                     2     //?? \r\n???
#define OUTER_CHECK_SUM               1     //?????

typedef enum {
    ENUM_FIND_AA,
    ENUM_FIND_55,
    ENUM_FIND_CHARGER_TYPE,
    ENUM_FIND_CHAGER_SN,
    ENUM_FIND_SRC,
    ENUM_FIND_DEST,
    ENUM_FIND_LEN,
    ENUM_FIND_VER,
    ENUM_FIND_SERNUM,
    ENUM_FIND_CMD,
    ENUM_RX_DATA,
    ENUM_FIND_CHK,
    ENUM_FIND_EE,
    ENUM_FIND_END,
} PROTO_MSG_ENUM;

enum {
    BT_FIND_EE,
    BT_FIND_CMD,
    BT_FIND_LEN,
    BT_RX_DATA,
    BT_FIND_CHK,
    BT_FIND_END,
};


extern void CK24_UMTask(void *argc);
/*****************************************************************************
** Function name:       SendBlueTooth2_4GPkt
** Descriptions:        ����������ݰ�
** input parameters:    nodeType:������������������������RF2.4G
                        pkt:���Э��İ�����ʼ��ַ
                        len:�ڲ�Э�����ݰ��������ĳ���, ����ͷ��data���ֺ�У���� , �����Э��data����
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern int SendBlueTooth2_4GPkt(uint8_t nodeType, BLUE_RF2_4G_PKT_STR *pkt, uint16_t len);
//void RecvTask(void *argc);
extern int CheckIdlePin(void);



#endif
