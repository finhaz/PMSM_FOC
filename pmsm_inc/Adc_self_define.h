/*
 * Adc_self_define.h
 *
 *  Created on: 2020��9��15��
 *      Author: sea
 */

#ifndef ADC_SELF_DEFINE_H_
#define ADC_SELF_DEFINE_H_

#define ADCZeroCNT0num 600//����У��ÿѭ����������
#define ADCZeroCNTloopnum 8 //����У��ѭ����

typedef struct{float Vdc;
               float UMAINS;
               float Ia;
               float Ib;
               float Ic;
               float Idc;
               float speed;
               float theta;
               float FO;
               float Jizhun;
              }ADC_VOLT_CURRENT_GET;
//-----------------------------------------------------------------------------
//����ADC_VOLT_CURRENT_GEThandleΪADC_VOLT_CURRENT_GETָ������
//typedef ADC_VOLT_CURRENT_GET*ADC_VOLT_CURRENT_GET_handle;
//-----------------------------------------------------------------------------
//����ADCģ��ĳ�ʼֵ
//-----------------------------------------------------------------------------
#define ADC_VOLT_CURRENT_GET_DEFAULTS {0,0,0,\
                                      0,0,0,\
                                      0,0,\
                                      0,0}



#endif /* PMSM_INC_ADC_SELF_DEFINE_H_ */
