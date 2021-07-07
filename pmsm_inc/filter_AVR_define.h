/*
 * Filter_AVR_define.h
 *
 *  Created on: 2021年4月14日
 *      Author: sea
 */

#ifndef PMSM_INC_FILTER_AVR_DEFINE_H_
#define PMSM_INC_FILTER_AVR_DEFINE_H_

typedef struct{
    float x_in;//输入
    float x_sum;//总和
    int count;//均值滤波计数
    int n_win;//均值滤波的窗口大小
    float y;//均值滤波输出
}Filter_AVR_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define Filter_AVR_GET_DEFAULTS {0,0,0,50,0}

#endif /* PMSM_INC_FILTER_AVR_DEFINE_H_ */
