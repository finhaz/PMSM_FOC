/*
 * Adc_self_define.h
 *
 *  Created on: 2020年9月15日
 *      Author: sea
 */

#ifndef ADC_SELF_DEFINE_H_
#define ADC_SELF_DEFINE_H_

#define ADCZeroCNT0num 600//采样校零每循环采样点数
#define ADCZeroCNTloopnum 8 //采样校零循环数

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
//声明ADC_VOLT_CURRENT_GEThandle为ADC_VOLT_CURRENT_GET指针类型
//typedef ADC_VOLT_CURRENT_GET*ADC_VOLT_CURRENT_GET_handle;
//-----------------------------------------------------------------------------
//定义ADC模块的初始值
//-----------------------------------------------------------------------------
#define ADC_VOLT_CURRENT_GET_DEFAULTS {0,0,0,\
                                      0,0,0,\
                                      0,0,\
                                      0,0}



#endif /* PMSM_INC_ADC_SELF_DEFINE_H_ */
