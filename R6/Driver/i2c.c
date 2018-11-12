/*i2c.c
* 2017-10-23
* Copyright(C) 2017
* liutao@chargerlink.com
*/

#include "includes.h"
#include "i2c.h"

//�������ں�I2C�жϴ�����Ϣ��ȫ�ֱ���
volatile uint8_t   I2C_sla;   //I2C�����ӵ�ַ
volatile uint16_t  I2C_suba;   //2C�����ڲ��ӵ�ַ
volatile uint8_t   I2C_suba_num;  //I2C�ӵ�ַ�ֽ���
volatile uint8_t   *I2C_buf;      //���ݻ�����ָ��
volatile uint16_t  I2C_num;  	 //Ҫ��ȡ/д������ݸ���
volatile uint8_t   I2C_end;   //I2C���߽�����־��������������1
volatile uint8_t   I2C_suba_en;  //�ӵ�ַ����
uint8_t gI2cMux=0;


//******************************************************************
//! \brief  	I2C_Init
//! \param
//! \retval
//! \note   	I2Cģ���ʼ��
//******************************************************************
void I2C_Init(void)
{
	//PC13 PC14 as SCL SDA
	GPIO_Cfg(HT_GPIOC, GPIO_Pin_13|GPIO_Pin_14, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	GPIO_Cfg(HT_GPIOE, GPIO_Pin_0, GPIO_Mode_IOOUT, GPIO_Input_Up, GPIO_Output_PP);
	GPIO_ResetBits(HT_GPIOE, GPIO_Pin_0);

	HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_I2CEN, ENABLE);

	NVIC_DisableIRQ(I2C_IRQn);		//��ֹI2C�ж�
	HT_I2C->I2CCON=0x0247;

	NVIC_ClearPendingIRQ(I2C_IRQn);	//�������״̬
	NVIC_SetPriority(I2C_IRQn, 4);	//�������ȼ�
	NVIC_EnableIRQ(I2C_IRQn); 		//ʹ��I2C�ж�

	//PA11 as CTP_INT(INT6)
	GPIO_Cfg(HT_GPIOA, GPIO_Pin_11, GPIO_Mode_AF1, GPIO_Input_Up, GPIO_Output_PP);
	HT_INT->EXTIF = 0x0000;
	NVIC_ClearPendingIRQ(EXTI6_IRQn);
	NVIC_SetPriority(EXTI6_IRQn, 3);
	HT_INT->EXTIE |= 0x0040;		//INT6�½����ж�ʹ��
	NVIC_EnableIRQ(EXTI6_IRQn);
}

//******************************************************************
//! \brief  	EXTI6_IRQHandler
//! \param
//! \retval
//! \note   	�ⲿ�жϴ�����
//******************************************************************
uint8_t keyHappenEvent=0;
void EXTI6_IRQHandler(void)
{
	keyHappenEvent = 1;
	//delay_ms(100);
	HT_INT->EXTIF = 0x0000;		//���жϱ�־
}

//******************************************************************
//! \brief  	GetKeyHappenEvent
//! \param
//! \retval 	keyHappenEvent
//! \note   	��ȡ�����жϷ����¼�
//******************************************************************
uint8_t GetKeyHappenEvent(void)
{
	return keyHappenEvent;
}

//******************************************************************
//! \brief  	ClrKeyHappenEvent
//! \param
//! \retval 	keyHappenEvent
//! \note   	��������жϷ����¼�
//******************************************************************
void ClrKeyHappenEvent(void)
{
	keyHappenEvent = 0;
	return;
}
//******************************************************************
//! \brief  	I2C_Start
//! \param
//! \retval
//! \note   	start�ź�
//! \note
//******************************************************************
void I2C_Start(void)
{
 	HT_I2C->I2CCON |= 0x0020;
}
//******************************************************************
//! \brief  	I2C_Stop
//! \param
//! \retval
//! \note   	stop�ź�
//! \note
//******************************************************************
void I2C_Stop(void)
{
	HT_I2C->I2CCON |= 0x0010;
}
//******************************************************************
//! \brief  	I2C_ClrSi
//! \param
//! \retval
//! \note   	����жϱ�־
//! \note
//******************************************************************
void I2C_ClrSi(void)
{
	HT_I2C->I2CCON &= 0xffd7;

}
//******************************************************************
//! \brief  	I2C_IRQHandler
//! \param
//! \retval
//! \note   	I2C�жϺ���
//! \note
//******************************************************************
void I2C_IRQHandler(void)
{
	//����״̬�������Ӧ�Ĵ���
    switch (HT_I2C->I2CSTA&0x000000F8){
		case 0x08: //�ѷ�����ʼ����(�����ͺ������ն���)
		{
			//��д�ӻ���ַ�ѳ�ʼ��,ֱ�ӷ���
			HT_I2C->I2CDAT = I2C_sla;
			//���жϱ�־
			I2C_ClrSi();
		}
		break;

		case 0x10: //�ѷ����ظ���ʼ���� (�����ͺ������ն���)
		{
			//װ��SLA+W����SLA+R
			HT_I2C->I2CDAT = I2C_sla; //�������ߺ��ط��ӵ�ַ
			//����SI,STA
			I2C_ClrSi();
		}
		break;

		case 0x18://SLA+W�ѱ�����,ACK�ѱ�����
		case 0x28://�ѷ���I2DAT�е����ݣ��ѽ���ACK
		{
			if (I2C_suba_en == 0){
				if (I2C_num > 0){
					HT_I2C->I2CDAT = *I2C_buf++;
					I2C_ClrSi();     // ����SI,STA
					I2C_num--;
				}else{ //û�����ݷ�����
					//ֹͣ����
					I2C_Stop();    		//STO
					I2C_ClrSi();     	//����SI,STA
					I2C_end = 1;      	//�����Ѿ�ֹͣ
				}
			}

			if(I2C_suba_en == 1) {// ����ָ���ӵ�ַ������������������
				if (I2C_suba_num == 2){
					HT_I2C->I2CDAT = ((I2C_suba >> 8) & 0xff);
					I2C_ClrSi();     //����SI,STA
					I2C_suba_num--;
					break;
				}

				if(I2C_suba_num == 1){
					HT_I2C->I2CDAT = (I2C_suba & 0xff);
					I2C_ClrSi();     //����SI,STA
					I2C_suba_num--;
					break;
				}

				if (I2C_suba_num == 0){
					I2C_Start();
					HT_I2C->I2CCON &= 0xfff7;
					I2C_sla += 1;
					I2C_suba_en = 0;        //�ѷ����ӵ�ַ
					break;
				}
			}

			if (I2C_suba_en == 2){// ָ���ӵ�ַд,�ӵ�ַ��δָ��,�����ӵ�ַ
				if (I2C_suba_num > 0){
					if (I2C_suba_num == 2){
						HT_I2C->I2CDAT = ((I2C_suba >> 8) & 0xff);
						I2C_ClrSi();
						I2C_suba_num--;
						break;
					}

					if (I2C_suba_num == 1){
						HT_I2C->I2CDAT    = (I2C_suba & 0xff);
						I2C_ClrSi();
						I2C_suba_num--;
						I2C_suba_en  = 0;			 //�ѷ����ӵ�ַ
						break;
					}
				}
			}
		}
		break;

		case 0x40: //�ѷ���SLA+R,�ѽ���ACK
		{
			//��������һ���ֽ�
			if (I2C_num <= 1) {
				HT_I2C->I2CCON &= 0xfffb;    //�´η��ͷ�Ӧ���ź�
			}else{
				HT_I2C->I2CCON |= 0x04;      //�´η���Ӧ���ź�
			}
			I2C_ClrSi();       //����SI,STA
		}
		break;

		case 0x20: //�ѷ���SLA+W,�ѽ��շ�Ӧ��
		case 0x30: //�ѷ���I2DAT�е����ݣ��ѽ��շ�Ӧ��
		case 0x38: //��SLA+R/W�������ֽ��ж�ʧ�ٲ�
		case 0x48: //�ѷ���SLA+R,�ѽ��շ�Ӧ��
		{
			I2C_Stop();             //��������
			I2C_ClrSi();
			I2C_end = 0xFF;
		}
	    break;

		case 0x50: //�ѽ��������ֽڣ��ѷ���ACK
		{
			*I2C_buf++ = HT_I2C->I2CDAT;//-10;
			I2C_num--;

			if (I2C_num == 0){//�������һ���ֽ�
				HT_I2C->I2CCON &= 0xffd3;      //STA,SI,AA = 0  I2CCON
			}else{
				HT_I2C->I2CCON &= 0xffff;      //AA=1
				I2C_ClrSi();
			}
		}
		break;

		case 0x58: //�ѽ��������ֽڣ��ѷ��ط�Ӧ��
		{
			*I2C_buf++ = HT_I2C->I2CDAT;//-10;          //��ȡ���һ�ֽ�����
			I2C_Stop();             //��������
			I2C_ClrSi();
			I2C_end = 1;
		}
		break;

		default:
			break;
	}
    //�жϴ������
}

//******************************************************************
//! \brief  	I2C_ReadNByte
//! \param  	slaveAddr   		�����ӵ�ַ
//! 			subAddrType 		�ӵ�ַ�ṹ 1�����ֽڵ�ַ 3��8+X�ṹ 2��˫�ֽڵ�ַ
//!				sub_addr  			�����ڲ������ַ
//!				*rbuf    			���ݴ洢��ָ��
//!				len   				��Ҫ��ȡ�����ݵĸ���
//! \retval		TRUE  �����ɹ�		FALSE  ����ʧ��
//! \note   	I2C��ȡ����
//******************************************************************
uint8_t I2C_ReadNByte(uint8_t slaveAddr, uint8_t subAddrType, uint32_t subAddr, uint8_t *rbuf, uint32_t len)
{
    int i;
	MuxSempTake(&gI2cMux);

	// �ж϶�ȡ���ȵĺϷ���
	if(0 == len) return FALSE;

	//��������
	if (0 == subAddrType) {
		I2C_sla      = slaveAddr + 1; 				//д��������Ĵӵ�ַ��R=1
		I2C_suba     = 0;        					//�����ӵ�ַΪ0
		I2C_suba_num = 0;        					//�����ӵ�ַΪ0�ֽ�
	} else if (1 == subAddrType) {
		I2C_sla      = slaveAddr; 					//д��д�����Ĵӵ�ַ��W=0
		I2C_suba     = subAddr;        				//�����ӵ�ַ
		I2C_suba_num = 1;        					//�����ӵ�ַΪ1�ֽ�
	} else if (2 == subAddrType) {
		I2C_sla      = slaveAddr; 					//д��д�����Ĵӵ�ַ��W=0
		I2C_suba     = subAddr;        				//�����ӵ�ַ
		I2C_suba_num = 2;        					//�����ӵ�ַΪ2�ֽ�
	} else if (3 == subAddrType) { 					//�ӵ�ַ�ṹΪ8+X
		I2C_sla   = slaveAddr + ((subAddr >> 7 )& 0x0e) + 1; 	//�������Ĵӵ�ַ��R=1
		I2C_suba  = subAddr & 0x0ff;      						//�����ӵ�ַ
		I2C_suba_num = 1;        								//�����ӵ�ַΪ8+x
	}

	I2C_buf     = rbuf;       //���ݽ��ջ�����ָ��
	I2C_num     = len;        //Ҫ��ȡ�ĸ���
	I2C_suba_en = 1;          //���ӵ�ַ��
	I2C_end     = 0;

	//���STA,SI,AA��־λ
	HT_I2C->I2CCON &= 0xffd3;
	//��λSTA,����I2C����
	HT_I2C->I2CCON |= 0x60; //I2CEN

	/* �ȴ�I2C������� */
    i = 0;
	while (I2C_end == 0) {
        vTaskDelay(1);
        if (2000 < ++i) {
            break;
        }
    }

	MuxSempGive(&gI2cMux);
	if (I2C_end == 1) {
		return (TRUE);
	}else{
		return (FALSE);
	}
}

//******************************************************************
//! \brief  	I2C_WriteNByte
//! \param  	slaveAddr   			�����ӵ�ַ
//! 			subAddrType 		�ӵ�ַ�ṹ 1�����ֽڵ�ַ 3��8+X�ṹ 2��˫�ֽڵ�ַ
//!				subAddr  			�����ڲ������ַ
//!				*wBuf   			��Ҫд������ݵ�ָ��
//!				len   			��Ҫд������ݵĸ���
//! \retval		TRUE  �����ɹ�		FALSE  ����ʧ��
//! \note
//******************************************************************
uint8_t I2C_WriteNByte(uint8_t slaveAddr, uint8_t subAddrType, uint32_t subAddr, uint8_t *wBuf, uint32_t len)
{
    int i;

	MuxSempTake(&gI2cMux);
	//���ò���
	if (subAddrType == 1){ //�ӵ�ַΪ���ֽ�
		I2C_sla      = slaveAddr;     	// �������Ĵӵ�ַ
		I2C_suba     = subAddr;       	//�����ӵ�ַ
		I2C_suba_num = 1;       		//�����ӵ�ַΪ1�ֽ�
	}

	if (subAddrType == 2){ //�ӵ�ַΪ2�ֽ�
		I2C_sla      = slaveAddr;        	//�������Ĵӵ�ַ
		I2C_suba      = subAddr;       	//�����ӵ�ַ
		I2C_suba_num = 2;        			//�����ӵ�ַΪ2�ֽ�
	}

	if (subAddrType == 3){ // �ӵ�ַ�ṹΪ8+X
		I2C_sla   = slaveAddr + ((subAddr >> 7 )& 0x0e);  		//�������Ĵӵ�ַ
		I2C_suba  = subAddr & 0x0ff;      							//�����ӵ�ַ
		I2C_suba_num = 1;        									//�����ӵ�ַΪ8+X
	}

	I2C_buf     = wBuf;          		//����
	I2C_num     = len;          		//���ݸ���
	I2C_suba_en = 2;          			//���ӵ�ַ��д����
	I2C_end     = 0;

	//���STA,SI,AA��־λ
	HT_I2C->I2CCON &= 0xffd3;
	//��λSTA,����I2C����
	HT_I2C->I2CCON |= 0x60;

	//�ȴ�I2C�������
    i = 0;
	while (I2C_end == 0) {
        vTaskDelay(1);
        if (2000 < ++i) {
            CL_LOG("wait i2c fail.\n");
            break;
        }
    }

	MuxSempGive(&gI2cMux);
	if (I2C_end == 1){
		return (TRUE);
	}else{
		return (FALSE);
	}
}


