/*
 * adrc_define.h
 *
 *  Created on: 2021年7月13日
 *      Author: sea
 */

#ifndef PMSM_INC_ADRC_DEFINE_H_
#define PMSM_INC_ADRC_DEFINE_H_

//ADRC调节器:TD环节，PID控制，ESO前馈
typedef struct {
    float   qdSum;        //Integrator sum; 1.31 format
    float   currentError;
    float   U;
    float   qKp;          //Proportional Gain
    float   qKi;          //Integral Gain
    float   qKd;          //Der Gain
    float   qOutMax;      //PI Output maximum limit
    float   qOutMin;      //PI Output minimum limit
    float   qInRef;       //给定参考值
    float   qInMeas;      //反馈值
    float   qOut;         //ADRC Output; 1.15 format
    float   z1;           //ESO的z1
    float   z2;           //ESO的z2,扩张状态变量
    float   w0;           //ESO带宽
    int enable;            //ESO标志位
    float   Tk_s;         //ESO的执行周期
    float   b;            //模型参数

    float alpha_kp;
    float filter_kp;

    float alpha_ki;
    float filter_ki;

    float alpha_kd;
    float filter_kd;

    }ADRC_Regulator;
//声明ADRC_Regulator_handle为ADRC_Regulator指针类型
typedef ADRC_Regulator* ADRC_handle;
#define ADRC_Regulator_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,0,0,0.9,0.01,0,0,0,0,}
#define ADRC_RegC_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,10000,0,0,0,0.8,0.01,0.8,0.01,0,0,}

#endif /* PMSM_INC_ADRC_DEFINE_H_ */
