
#ifndef ROGI_H_
#define ROGI_H_

#include "includes.h"    // DSP28x Headerfile //加入这个头文件后才会识别到int32


//--------------------定义结构体-----------------------------------

//ROGI调节器结构体
typedef struct {
    float   qdSum;        //Integrator sum; 1.31 format
    float   currentError;
    float   U1;
    float   out_of_forward_integrate_q1;   //正序q轴积分
    float   out_of_forward_integrate_d1;   //正序d轴积分
    float   out_of_forward_integrate_q2;   //负序q轴积分
    float   out_of_forward_integrate_d2;  //负序d轴积分
    float   out_of_j_feedback;
    float   U2;
    float   qTk;


    float   qWg1;          //正序频率   6we
    float   qWg2;          //负序频率   -6we
    float   qKI;          //Integral Gain
    float   qWC;          //Anti-windup Gain
    float   qOutMax;      //ROGI Output maximum limit
    float   qOutMin;      //ROGI Output minimum limit
    float   qInRef;       //给定参考值
    float   qInMeas;      //反馈值
    float   qOut1;         //ROGI Output; 1.15 format
    float   qOut2;
    }ROGI_CONTROL;

//声明ROGI_CONTROL_handle为ROGI_CONTROL指针类型

typedef ROGI_CONTROL*ROGI_CONTROL_handle;

#define ROGI_CONTROL_DEFAULTS {0,0,0,0,0,\
                             0,0,0,0,0,\
                             0,0,0,0,0,\
                             0,0,0,0,0,\
                             }

//结构体变量
extern ROGI_CONTROL ROGI_q_R;
extern ROGI_CONTROL ROGI_d_R;

//函数声明
//void ROGI_d_CONTROL_Postive( ROGI_CONTROL *p);           //这样写也可以，不用再加typedef ROGI_CONTROL*ROGI_CONTROL_handle;这条语句了
void ROGI_d_CONTROL_Postive(ROGI_CONTROL_handle);
void ROGI_q_CONTROL_Postive(ROGI_CONTROL_handle);
void ROGI_d_CONTROL_Negative(ROGI_CONTROL_handle);
void ROGI_q_CONTROL_Negative(ROGI_CONTROL_handle);
void ROGIZero();
void current_loop_rogi();
/*
extern int32 KI_current;////////////////////////ROGI
extern int32 WC_current;
extern int32 max_current1;
extern int32 min_current1;
*/


#define ParameterNumber 118 //定义通信变量个数
extern float Paramet[ParameterNumber];

#define KI_current Paramet[86]
#define WC_current Paramet[87]
#define max_current1 Paramet[88]
#define min_current1 -Paramet[89]
#define sign_ROGI  Paramet[85]     ///ROGI启动设置，1启动，0关闭
#define  theta_c   Paramet[84]     //补偿角，初值可以设为0



















#endif
