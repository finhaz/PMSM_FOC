/*
 * pid_define.h
 *
 *  Created on: 2021年2月16日
 *      Author: sea
 */

#ifndef PMSM_INC_PID_DEFINE_H_
#define PMSM_INC_PID_DEFINE_H_

//pi调节器
typedef struct {
    float   qdSum;        //Integrator sum; 1.31 format
    float   currentError;
    float   U;
    float   qKp;          //Proportional Gain
    float   qKi;          //Integral Gain
    float   qKc;          //Anti-windup Gain
    float   qOutMax;      //PI Output maximum limit
    float   qOutMin;      //PI Output minimum limit
    float   qInRef;       //给定参考值
    float   qInMeas;      //反馈值
    float   qOut;         //PI Output; 1.15 format
    }PI_CONTROL;
//声明PI_CONTROL_handle为PI_CONTROL指针类型
typedef PI_CONTROL*PI_CONTROL_handle;
#define PI_CONTROL_DEFAULTS {0,0,0,\
                             0,0,0,\
                             0,0,0,\
                             0,0,\
                             }

#endif /* PMSM_INC_PID_DEFINE_H_ */
