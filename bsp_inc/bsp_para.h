/*
 * bsp_para.h
 *
 *  Created on: 2021��4��18��
 *      Author: sea
 */

#ifndef BSP_INC_BSP_PARA_H_
#define BSP_INC_BSP_PARA_H_

//ADCͨ��
//#define ADC_IA AdcMirror.ADCRESULT4
//#define ADC_IB AdcMirror.ADCRESULT5
#define ADC_IA AdcMirror.ADCRESULT0
#define ADC_IB AdcMirror.ADCRESULT1
#define ADC_IC AdcMirror.ADCRESULT2
#define ADC_UDC AdcMirror.ADCRESULT3
#define ADC_UMAIN AdcMirror.ADCRESULT3
#define ADC_Speed AdcMirror.ADCRESULT5
#define ADC_Theta AdcMirror.ADCRESULT6

//����
//����Ƶ��Ϊfk,�򿪹�����ΪT
//�����ز�ģʽ��
//PrdΪ��������ֵ��Ӧ�����ڼĴ���ֵ
//Prd_HALF��Ӧ��ֵ��ʾ�������ڵ�һ�루SPWM��
//DTΪ�����Ĵ���ֵ��DT��TBCLKΪ����ʱ��
////10K
//#define Prd 7500
//#define T 0.0001
//20k
#define Prd 3750
#define T 0.00005
////30k
//#define Prd 2500
//#define T 0.000033
////40k
//#define Prd 1875
//#define T 0.000025
//#define Prd_da 7500
//#define DT 50
#define DT 10
#define QPO_Pulse 4096//�ı�Ƶ����4*1024
#define INDEX_OFFSET 3756




#endif /* BSP_INC_BSP_PARA_H_ */
