#ifndef __FM175XX_H__
#define __FM175XX_H__



#define CommandReg	0x01
#define ComIEnReg	0x02
#define DivIEnReg	0x03
#define ComIrqReg	0x04
#define DivIrqReg	0x05
#define ErrorReg	0x06
#define Status1Reg	0x07
#define Status2Reg	0x08
#define FIFODataReg 0x09
#define FIFOLevelReg 0x0A       //�洢��FIFO����ֽ���
#define WaterLevelReg 0x0B
#define ControlReg	0x0C
#define BitFramingReg 0x0D
#define CollReg	0x0E
#define ModeReg 0x11
#define TxModeReg 0x12
#define RxModeReg 0x13
#define TxControlReg 0x14
#define TxAutoReg 0x15
#define TxSelReg 0x16
#define RxSelReg 0x17
#define RxThresholdReg 0x18
#define DemodReg 0x19
#define MfTxReg 0x1C
#define MfRxReg 0x1D
#define SerialSpeedReg 0x1F
#define CRCMSBReg 0x21
#define CRCLSBReg 0x22
#define ModWidthReg 0x24
#define GsNOffReg 0x23
#define TxBitPhaseReg 0x25
#define RFCfgReg 0x26
#define GsNOnReg 0x27
#define CWGsPReg 0x28
#define ModGsPReg 0x29
#define TModeReg 0x2A
#define TPrescalerReg 0x2B
#define TReloadMSBReg 0x2C
#define TReloadLSBReg 0x2D
#define TCounterValMSBReg 0x2E
#define TCounterValLSBReg 0x2F
#define TestSel1Reg 0x31
#define TestSel2Reg 0x32
#define TestPinEnReg 0x33
#define TestPinValueReg 0x34
#define TestBusReg 0x35
#define AutoTestReg 0x36
#define VersionReg 0x37
#define AnalogTestReg 0x38
#define TestDAC1Reg 0x39
#define TestDAC2Reg 0x3A
#define TestADCReg 0x3B



#define Idle	(0x00|0x20)
#define Mem		0x01
#define RandomID	0x02
#define CalcCRC	0x03
#define Transmit	0x04
#define NoCmdChange	0x07
#define Receive	0x08
#define Transceive	0x0C
#define MFAuthent	0x0E
#define SoftReset	0x0F



#define TIMEOUT_Err	0x20

#define OK      0
#define ERROR   1
/////////////////////////////////////////////////////////////////////
//ͨѶʱ���صĴ������
/////////////////////////////////////////////////////////////////////
#define MI_OK                          (char)0
#define MI_NOTAGERR                    (char)(-1)
#define MI_ERR                         (char)(-2)
#define MAXRLEN 18
#define PCD_IDLE              0x00               //ȡ����ǰ����





extern unsigned char Pcd_Comm( unsigned char Command, 
				               unsigned char *pInData, 
				               unsigned char InLenByte,
				               unsigned char *pOutData, 
				               unsigned int *pOutLenBit);
extern unsigned char Set_Rf(unsigned char mode);
extern unsigned char Read_Reg(unsigned char reg_add);
extern unsigned char Read_Reg_All(unsigned char *reg_value);
extern unsigned char Write_Reg(unsigned char reg_add,unsigned char reg_value);
extern void Write_FIFO(unsigned char length,unsigned char *fifo_data);
extern void Read_FIFO(unsigned char length,unsigned char *fifo_data);
extern unsigned char Clear_FIFO(void);
extern unsigned char Set_BitMask(unsigned char reg_add,unsigned char mask);
extern unsigned char Clear_BitMask(unsigned char reg_add,unsigned char mask);
extern unsigned char Pcd_ConfigISOType(unsigned char type);
extern unsigned char FM175XX_SoftReset(void);
extern unsigned char FM175XX_HardReset(void);
extern unsigned char FM175XX_SoftPowerdown(void);
extern unsigned char FM175XX_HardPowerdown(unsigned char mode);
extern unsigned char Pcd_SetTimer(unsigned long delaytime);
extern unsigned char Read_Ext_Reg(unsigned char reg_add);
extern unsigned char Write_Ext_Reg(unsigned char reg_add,unsigned char reg_value);
extern unsigned char Set_Ext_BitMask(unsigned char reg_add,unsigned char mask);
extern unsigned char Clear_Ext_BitMask(unsigned char reg_add,unsigned char mask);
/*****************************************************************************
** Function name:       TestSPI_Interface
** Descriptions:        
** input parameters:    None
** output parameters:   None
** Returned value:	  None
** Author:              quqian
*****************************************************************************/
extern void TestSPI_Interface(void);


int FM175XX_Init(void);

#endif



