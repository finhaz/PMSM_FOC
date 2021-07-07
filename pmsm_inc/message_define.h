/*
 * message_define.h
 *
 *  Created on: 2020��9��15��
 *      Author: sea
 */

#ifndef MESSAGE_DEFINE_H_
#define MESSAGE_DEFINE_H_


#define PackHeadLength 4 //��ͷ����
#define PackHead 0x00FE  //��ͷ
#define ParameterNumber 118 //����ͨ�ű�������
#define SortNo 0x01//վ��ţ���������δʹ��
#define On_off 81   //���ػ�
#define ConfirmCode  01 //SCIȷ����

//----------------------------------int communication
struct FLOAT_COMMUNICATION_BITS {     // bits  description
    Uint16  MEM1:8;      // 7:0
    Uint16  MEM2:8;   // 15:8

};

union FLOAT_COM{
   int16                all;
   struct FLOAT_COMMUNICATION_BITS   bit;
};

//----------------------------------float communication
struct FLOAT_COMMUNICATION_BITSF {     // bits  description
    Uint16  MEM1:8;      // 7:0
    Uint16  MEM2:8;   // 15:8
    Uint16  MEM3:8;      //23:16
    Uint16  MEM4:8;   // 31:24
};

union FLOAT_COMF {
   float                all;
   struct FLOAT_COMMUNICATION_BITSF   bit;
};


#endif /* PMSM_INC_MESSAGE_DEFINE_H_ */
