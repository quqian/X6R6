#include "m1_card.h"
#include "system_info.h"

/****************************************************************************************/
/*���ƣ�TypeA_Request																	*/
/*���ܣ�TypeA_Request��ƬѰ��															*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pTagType[0],pTagType[1] =ATQA                                         		*/
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_Request(unsigned char *pTagType) {
    unsigned char   result, send_buff[1], rece_buff[2];
    unsigned int   rece_bitlen = 0;
    Clear_BitMask(TxModeReg, 0x80);//�ر�TX CRC
    Clear_BitMask(RxModeReg, 0x80);//�ر�RX CRC
    Set_BitMask(RxModeReg, 0x08);//�ر�λ����
    Write_Reg(BitFramingReg, 0x07);
    send_buff[0] = 0x26;
    Pcd_SetTimer(1);

    result = Pcd_Comm(Transceive, send_buff, 1, rece_buff, &rece_bitlen);  //����REQA������ѯ
    //PrintfData("quqian00TypeA_Request", rece_buff, 16);
    if ((result == OK) && (rece_bitlen == 2 * 8)) {
        *pTagType = rece_buff[0];
        *(pTagType + 1) = rece_buff[1];
        return OK;
    }
    return ERROR;
}
/****************************************************************************************/
/*���ƣ�TypeA_WakeUp																	*/
/*���ܣ�TypeA_WakeUp��ƬѰ��															*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pTagType[0],pTagType[1] =ATQA                                         		*/
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
//unsigned char TypeA_WakeUp(unsigned char *pTagType) {
//    unsigned char   result, send_buff[1], rece_buff[2];
//    unsigned int   rece_bitlen;
//    Clear_BitMask(TxModeReg, 0x80);//�ر�TX CRC
//    Clear_BitMask(RxModeReg, 0x80);//�ر�RX CRC
//    Set_BitMask(RxModeReg, 0x08);//�ر�λ����
//    Clear_BitMask(Status2Reg, 0x08);
//    Write_Reg(BitFramingReg, 0x07);
//    send_buff[0] = 0x52;
//    Pcd_SetTimer(2);
//    result = Pcd_Comm(Transceive, send_buff, 1, rece_buff, &rece_bitlen);

//    if ((result == OK) && (rece_bitlen == 2 * 8)) {
//        *pTagType = rece_buff[0];
//        *(pTagType + 1) = rece_buff[1];
//    }
//    return result;
//}
/****************************************************************************************/
/*���ƣ�TypeA_Anticollision																*/
/*���ܣ�TypeA_Anticollision��Ƭ����ͻ													*/
/*���룺selcode =0x93��0x95��0x97														*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pSnr[0],pSnr[1],pSnr[2],pSnr[3]pSnr[4] =UID                            		*/
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_Anticollision(unsigned char selcode, unsigned char *pSnr) {
    unsigned char   result, send_buff[7], rece_buff[5], CollPos, NVB, Row, Col, Bit_Framing, Send_Len, Snr[5], i, Rece_Len;
    unsigned int   rece_bitlen = 0;

    memset(rece_buff, 0x00, 5);
    memset(Snr, 0x00, 5);//��ʱUID
    memset(pSnr, 0x00, 5);
    NVB = 0x20;//NVB��ʼֵ
    Bit_Framing = 0x00;//Bit_Framing��ʼֵ

    Row = 0;//�У�0Ϊ�޳�ͻ��bit1~bit8
    Col = 0;//�У�0Ϊ�޳�ͻ��1~4
    CollPos = 0;
    //4��8�У���32��λ�ã���ӦCollPos��ֵ 0~31
    Clear_BitMask(TxModeReg, 0x80);
    Clear_BitMask(RxModeReg, 0x80);

    Write_Reg(CollReg, 0x80);//��ͻλ��Ľ�������Ϊ0

    Write_Reg(MfRxReg, 0x00);//ʹ����żУ��


    while (1) {
        send_buff[0] = selcode;

        send_buff[1] = NVB;

        Write_Reg(BitFramingReg, Bit_Framing);//���÷��͡����յ�bit����

        memcpy(send_buff + 2, pSnr, Col);

        Send_Len = Col + 2;//���÷����ֽ���	

        Pcd_SetTimer(1);

        result = Pcd_Comm(Transceive, send_buff, Send_Len, rece_buff, &rece_bitlen);
        //PrintfData("quqian101TypeA_Anticollision", rece_buff, 16);
        if (result == TIMEOUT_Err) {
            return ERROR;
        }


        if (result & 0x08)//CollErr
        {
            //���ֳ�ͻ����
            CollPos = Read_Reg(CollReg) & 0x1F;//��ȡ��ͻλ��

            TypeA_Set_NVB(CollPos, &NVB, &Row, &Col);//�����´ν�����Ҫ��NVB�����汾���յ���bit����������

            TypeA_Set_Bit_Framing(CollPos, &Bit_Framing);//�����´ν�����Ҫ���͡����յ�bit����

            memcpy(Snr, rece_buff, Col);//������յ��Ĳ���UID��Snr

            Snr[Col - 1] = Snr[Col - 1] & Row;//������յ�����ȷbit

            for (i = 0; i < Col; i++) {
                pSnr[i] = pSnr[i] | Snr[i];//���Snr��pSnr�����浽pSnr
            }
        } else {
            //û�з��ֳ�ͻ
            if (rece_bitlen > 8)
                Rece_Len = rece_bitlen / 8;
            else
                Rece_Len = 1;

            if (Col == 0) {
                //�ϴν����޳�ͻ
                if ((Rece_Len + Col) != 5) {
                    return ERROR;
                }
                memcpy(pSnr, rece_buff, Rece_Len);//����rece buff�� pSnr
            } else {
                //�ϴν����г�ͻ	
                if ((Rece_Len + Col) != 6) {

                    return ERROR;
                }


                memcpy(Snr + Col - 1, rece_buff, Rece_Len);//����rece buff�� Snr

                for (i = 0; i < Rece_Len; i++) {
                    pSnr[i + Col - 1] = pSnr[i + Col - 1] | Snr[i + Col - 1];//���Snr��pSnr�����浽pSnr
                }
            }
            if (pSnr[4] == (pSnr[0] ^ pSnr[1] ^ pSnr[2] ^ pSnr[3])) {
                return OK;
            } else {
                return ERROR;
            }
        }

    }
   // return result;
}


void TypeA_Set_NVB(unsigned char collpos, unsigned char *nvb, unsigned char *row, unsigned char *col) {
    if (collpos == 0) {
        *nvb = 0x58;
        *col = 4;
    }
    if ((collpos > 0) && (collpos < 9)) //1~8
    {
        *nvb = 0x20 + (collpos); //21~28
        *col = 1;
    }
    if ((collpos > 8) && (collpos < 17))//9~16
    {
        *nvb = 0x30 + (collpos - 8);//31~38
        *col = 2;
    }
    if ((collpos > 16) && (collpos < 25))//17~24
    {
        *nvb = 0x40 + (collpos - 16);//41~48
        *col = 3;
    }
    if ((collpos > 24) && (collpos < 32))//25~31
    {
        *nvb = 0x50 + (collpos - 24);//51~57
        *col = 4;
    }

    switch ((*nvb) & 0x07) {
        case 1:
            *row = 0x01;
            break;
        case 2:
            *row = 0x03;
            break;
        case 3:
            *row = 0x07;
            break;
        case 4:
            *row = 0x0F;
            break;
        case 5:
            *row = 0x1F;
            break;
        case 6:
            *row = 0x3F;
            break;
        case 7:
            *row = 0x7F;
            break;
        case 8:
            *row = 0xFF;
            break;
    }
    return;
}

void TypeA_Set_Bit_Framing(unsigned char collpos, unsigned char *bit_framing) {
    if (collpos == 0) {
        *bit_framing = 0x00;
        return;
    }
    if ((collpos > 0) && (collpos < 9)) //1~8
    {
        *bit_framing = ((collpos) << 4) + (collpos);
        return;
    }
    if ((collpos > 8) && (collpos < 17))//9~16
    {
        *bit_framing = (((collpos - 8)) << 4) + (collpos - 8);
        return;
    }
    if ((collpos > 16) && (collpos < 25))//17~24
    {
        *bit_framing = (((collpos - 16)) << 4) + (collpos - 16);

        return;
    }
    if ((collpos > 24) && (collpos < 32))//25~31
    {
        *bit_framing = (((collpos - 24)) << 4) + (collpos - 24);
        return;
    }
    return;
}


/****************************************************************************************/
/*���ƣ�TypeA_Select																	*/
/*���ܣ�TypeA_Select��Ƭѡ��															*/
/*���룺selcode =0x93��0x95��0x97														*/
/*      pSnr[0],pSnr[1],pSnr[2],pSnr[3]pSnr[4] =UID 			    			     	*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pSak[0],pSak[1],pSak[2] =SAK			                            		*/
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_Select(unsigned char selcode, unsigned char *pSnr, unsigned char *pSak) {
    unsigned char   result, i, send_buff[7], rece_buff[5];
    unsigned int   rece_bitlen;
    Write_Reg(BitFramingReg, 0x00);
    Set_BitMask(TxModeReg, 0x80);
    Set_BitMask(RxModeReg, 0x80);

    send_buff[0] = selcode;
    send_buff[1] = 0x70;

    for (i = 0; i < 5; i++) {
        send_buff[i + 2] = *(pSnr + i);
    }

    Pcd_SetTimer(1);
    result = Pcd_Comm(Transceive, send_buff, 7, rece_buff, &rece_bitlen);
    //PrintfData("quqian11TypeA_Select", rece_buff, 16);
    if (result == OK) {
        *pSak = rece_buff[0];
    }
    return result;
}
/****************************************************************************************/
/*���ƣ�TypeA_Halt																		*/
/*���ܣ�TypeA_Halt��Ƭֹͣ																*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       											                            		*/
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_Halt(void) {
    unsigned char   result, send_buff[2], rece_buff[1];
    unsigned int   rece_bitlen;
    send_buff[0] = 0x50;
    send_buff[1] = 0x00;

    Write_Reg(BitFramingReg, 0x00);
    Set_BitMask(TxModeReg, 0x80);
    Set_BitMask(RxModeReg, 0x80);
    Clear_BitMask(Status2Reg, 0x08);
    Pcd_SetTimer(1);
    result = Pcd_Comm(Transceive, send_buff, 2, rece_buff, &rece_bitlen);
    //PrintfData("quqian22TypeA_Halt", rece_buff, 16);
    return result;
}

/****************************************************************************************/
/*���ƣ�TypeA_CardActivate																*/
/*���ܣ�TypeA_CardActivate��Ƭ����														*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����	pTagType[0],pTagType[1] =ATQA 											 	*/
/*	       	pSnr[0],pSnr[1],pSnr[2],pSnr[3]pSnr[4] =UID 		                   		*/
/*	       	pSak[0],pSak[1],pSak[2] =SAK			                            		*/
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_CardActivate(unsigned char *pTagType, unsigned char *pSnr, unsigned char *pSak) {
    unsigned char   result;

    result = TypeA_Request(pTagType);//Ѱ�� Standard	 send request command Standard mode
    
    if (result != OK) {
        return ERROR;
    }
    if (pTagType[1] != 0x00) {
        return ERROR;
    }
    if ((pTagType[0] & 0xC0) == 0x00)   //M1��
    {
        result = TypeA_Anticollision(0x93, pSnr);//����ͻ��֤
        if (result != OK) {
            return ERROR;
        }
        result = TypeA_Select(0x93, pSnr, pSak);//ѡ������������ͻ��֤
        if (result != OK) {
            return ERROR;
        }
    }
    return result;
}







/*****************************************************************************************/
/*���ƣ�Mifare_Auth																		 */
/*���ܣ�Mifare_Auth��Ƭ��֤																 */
/*���룺mode����֤ģʽ��0��key A��֤��1��key B��֤����sector����֤�������ţ�0~15��		 */
/*		*mifare_key��6�ֽ���֤��Կ���飻*card_uid��4�ֽڿ�ƬUID����						 */
/*���:																					 */
/*		OK    :��֤�ɹ�																	 */
/*		ERROR :��֤ʧ��																  	 */
/*****************************************************************************************/
 unsigned char Mifare_Auth(unsigned char mode,unsigned char sector,unsigned char *mifare_key,unsigned char *card_uid)
{
	unsigned char   send_buff[12],rece_buff[1],result;
	unsigned int   rece_bitlen;
	if(mode==0x0)
		send_buff[0]=0x60;//kayA��֤
	if(mode==0x1)
		send_buff[0]=0x61;//keyB��֤
  	send_buff[1]=sector*4;
	send_buff[2]=mifare_key[0];
	send_buff[3]=mifare_key[1];
	send_buff[4]=mifare_key[2];
	send_buff[5]=mifare_key[3];
	send_buff[6]=mifare_key[4];
	send_buff[7]=mifare_key[5];
	send_buff[8]=card_uid[0];
	send_buff[9]=card_uid[1];
	send_buff[10]=card_uid[2];
	send_buff[11]=card_uid[3];

	Pcd_SetTimer(1);
	Clear_FIFO();
	result =Pcd_Comm(MFAuthent,send_buff,12,rece_buff,&rece_bitlen);//Authent��֤
    //PrintfData("quqian33Mifare_Auth", rece_buff, 16);

	if (result==OK)
    {
        if(Read_Reg(Status2Reg) & 0x08)//�жϼ��ܱ�־λ��ȷ����֤���
            return OK;
        else
            return ERROR;
    }
	return ERROR;
}
/*****************************************************************************************/
/*���ƣ�Mifare_Blockset																	 */
/*���ܣ�Mifare_Blockset��Ƭ��ֵ����														 */
/*���룺block����ţ�*buff����Ҫ���õ�4�ֽ���ֵ����										 */
/*																						 */
/*���:																					 */
/*		OK    :���óɹ�																	 */
/*		ERROR :����ʧ��																	 */
/*****************************************************************************************/
 unsigned char Mifare_Blockset(unsigned char block,unsigned char *buff)
 {
  unsigned char   block_data[16],result;
	block_data[0]=buff[3];
	block_data[1]=buff[2];
	block_data[2]=buff[1];
	block_data[3]=buff[0];
	block_data[4]=~buff[3];
	block_data[5]=~buff[2];
	block_data[6]=~buff[1];
	block_data[7]=~buff[0];
   	block_data[8]=buff[3];
	block_data[9]=buff[2];
	block_data[10]=buff[1];
	block_data[11]=buff[0];
	block_data[12]=block;
	block_data[13]=~block;
	block_data[14]=block;
	block_data[15]=~block;
  result= Mifare_Blockwrite(block,block_data);
  return result;
 }

/*****************************************************************************************/
/*���ƣ�Mifare_Blockread																 */
/*���ܣ�Mifare_Blockread��Ƭ�������													 */
/*���룺block����ţ�0x00~0x3F����buff��16�ֽڶ�����������								 */
/*���:																					 */
/*		OK    :�ɹ�																		 */
/*		ERROR :ʧ��																		 */
/*****************************************************************************************/
unsigned char Mifare_Blockread(unsigned char block,unsigned char *buff)
{	
	unsigned char   send_buff[2],result;
	unsigned int   rece_bitlen;
	Pcd_SetTimer(1);
	send_buff[0]=0x30;//30 ����
	send_buff[1]=block;//���ַ
	Clear_FIFO();
	result =Pcd_Comm(Transceive,send_buff,2,buff,&rece_bitlen);//
    CL_LOG("rece_bitlen: %d ,result = %d \n", rece_bitlen, result);
	if ((result!=OK )||(rece_bitlen!=16*8)) //���յ������ݳ���Ϊ16
		return ERROR;
	return OK;
}

/*****************************************************************************************/
/*���ƣ�mifare_blockwrite																 */
/*���ܣ�Mifare��Ƭд�����																 */
/*���룺block����ţ�0x00~0x3F����buff��16�ֽ�д����������								 */
/*���:																					 */
/*		OK    :�ɹ�																		 */
/*		ERROR :ʧ��																		 */
/*****************************************************************************************/
unsigned char Mifare_Blockwrite(unsigned char block,unsigned char *buff)
{
	unsigned char   result,send_buff[16],rece_buff[16];
	unsigned int   rece_bitlen;
	Pcd_SetTimer(1);
	send_buff[0] = 0xa0;//a0 д��
	send_buff[1] = block;//���ַ

	result = Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);//
    //CL_LOG("rece_bitlen: %d ,result = %d ,rece_buff[0] = %d\n", rece_bitlen, result, rece_buff[0]);
	if ((result!=OK )|((rece_buff[0]&0x0F)!=0x0A))	//���δ���յ�0x0A����ʾ��ACK
    {
        //CL_LOG("Mifare_Blockwrite ����1:\n");
        return(ERROR);
    }   
	
	
	Pcd_SetTimer(5);
	result =Pcd_Comm(Transceive,buff,16,rece_buff,&rece_bitlen);//
    //CL_LOG("qa rece_bitlen: %d ,result = %d ,rece_buff[0] = %d\n", rece_bitlen, result, rece_buff[0]);
	if ((result!=OK )|((rece_buff[0]&0x0F)!=0x0A)) //���δ���յ�0x0A����ʾ��ACK
	{
        //CL_LOG("Mifare_Blockwrite ����2:\n");
        return(ERROR);
    }
    
	return OK;
}
/*****************************************************************************************/
/*���ƣ�																				 */
/*���ܣ�Mifare ��Ƭ��ֵ����																 */
/*���룺block����ţ�0x00~0x3F����buff��4�ֽ���ֵ��������								 */
/*���:																					 */
/*		OK    :�ɹ�																		 */
/*		ERROR :ʧ��																		 */
/*****************************************************************************************/

unsigned char Mifare_Blockinc(unsigned char block,unsigned char *buff)
{	
	unsigned char   result,send_buff[2],rece_buff[1];
	unsigned int   rece_bitlen;
	Pcd_SetTimer(5);
	send_buff[0]=0xc1;//
	send_buff[1]=block;//���ַ
	Clear_FIFO();
	result=Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    //PrintfData("quqian66Mifare_Blockinc1", rece_buff, 16);
	if ((result!=OK )|((rece_buff[0]&0x0F)!=0x0A))	//���δ���յ�0x0A����ʾ��ACK
		return ERROR;
	Pcd_SetTimer(5);
	Clear_FIFO();
	Pcd_Comm(Transceive,buff,4,rece_buff,&rece_bitlen);
    //PrintfData("quqian66Mifare_Blockinc2", rece_buff, 16);
	return result;
}

/*****************************************************************************************/
/*���ƣ�mifare_blockdec																	 */
/*���ܣ�Mifare ��Ƭ��ֵ����																 */
/*���룺block����ţ�0x00~0x3F����buff��4�ֽڼ�ֵ��������								 */
/*���:																					 */
/*		OK    :�ɹ�																		 */
/*		ERROR :ʧ��																		 */
/*****************************************************************************************/

unsigned char Mifare_Blockdec(unsigned char block,unsigned char *buff)
{	
	unsigned char   result,send_buff[2],rece_buff[1];
	unsigned int   rece_bitlen;
	Pcd_SetTimer(5);
	send_buff[0]=0xc0;//
	send_buff[1]=block;//���ַ
	Clear_FIFO();
	result=Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    //PrintfData("quqian77Mifare_Blockdec1", rece_buff, 16);
	if ((result!=OK )|((rece_buff[0]&0x0F)!=0x0A))	//���δ���յ�0x0A����ʾ��ACK
		return ERROR;
	Pcd_SetTimer(5);
	Clear_FIFO();
	Pcd_Comm(Transceive,buff,4,rece_buff,&rece_bitlen);
    //PrintfData("quqian77Mifare_Blockdec2", rece_buff, 16);
	return result;
}

/*****************************************************************************************/
/*���ƣ�mifare_transfer																	 */
/*���ܣ�Mifare ��Ƭtransfer����															 */
/*���룺block����ţ�0x00~0x3F��														 */
/*���:																					 */
/*		OK    :�ɹ�																		 */
/*		ERROR :ʧ��																		 */
/*****************************************************************************************/

unsigned char Mifare_Transfer(unsigned char block)
{	
	unsigned char    result,send_buff[2],rece_buff[1];
	unsigned int    rece_bitlen;
	Pcd_SetTimer(5);
	send_buff[0]=0xb0;//
	send_buff[1]=block;//���ַ
	Clear_FIFO();
	result=Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    //PrintfData("quqian88Mifare_Transfer", rece_buff, 16);
	if ((result!=OK )|((rece_buff[0]&0x0F)!=0x0A))	//���δ���յ�0x0A����ʾ��ACK
		return ERROR;
	return result;
}

/*****************************************************************************************/
/*���ƣ�mifare_restore																	 */
/*���ܣ�Mifare ��Ƭrestore����															 */
/*���룺block����ţ�0x00~0x3F��														 */
/*���:																					 */
/*		OK    :�ɹ�																		 */
/*		ERROR :ʧ��																		 */
/*****************************************************************************************/

unsigned char Mifare_Restore(unsigned char block)
{	
	unsigned char    result,send_buff[4],rece_buff[1];
	unsigned int    rece_bitlen;
	Pcd_SetTimer(5);
	send_buff[0]=0xc2;//
	send_buff[1]=block;//���ַ
	Clear_FIFO();
	result=Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    //PrintfData("quqian99Mifare_Restore1", rece_buff, 16);
	if ((result!=OK )|((rece_buff[0]&0x0F)!=0x0A))	//���δ���յ�0x0A����ʾ��ACK
		return ERROR;
	Pcd_SetTimer(5);
	send_buff[0]=0x00;
	send_buff[1]=0x00;
	send_buff[2]=0x00;
	send_buff[3]=0x00;
	Clear_FIFO();
	Pcd_Comm(Transceive,send_buff,4,rece_buff,&rece_bitlen);
    //PrintfData("quqian99Mifare_Restore2", rece_buff, 16);
	return result;
}

