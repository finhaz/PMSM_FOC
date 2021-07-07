/*
 * Filter_AVR_define.h
 *
 *  Created on: 2021��4��14��
 *      Author: sea
 */

#ifndef PMSM_INC_FILTER_AVR_DEFINE_H_
#define PMSM_INC_FILTER_AVR_DEFINE_H_

typedef struct{
    float x_in;//����
    float x_sum;//�ܺ�
    int count;//��ֵ�˲�����
    int n_win;//��ֵ�˲��Ĵ��ڴ�С
    float y;//��ֵ�˲����
}Filter_AVR_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define Filter_AVR_GET_DEFAULTS {0,0,0,50,0}

#endif /* PMSM_INC_FILTER_AVR_DEFINE_H_ */
