
#ifndef ROGI_H_
#define ROGI_H_

#include "includes.h"    // DSP28x Headerfile //�������ͷ�ļ���Ż�ʶ��int32


//--------------------����ṹ��-----------------------------------

//ROGI�������ṹ��
typedef struct {
    float   qdSum;        //Integrator sum; 1.31 format
    float   currentError;
    float   U1;
    float   out_of_forward_integrate_q1;   //����q�����
    float   out_of_forward_integrate_d1;   //����d�����
    float   out_of_forward_integrate_q2;   //����q�����
    float   out_of_forward_integrate_d2;  //����d�����
    float   out_of_j_feedback;
    float   U2;
    float   qTk;


    float   qWg1;          //����Ƶ��   6we
    float   qWg2;          //����Ƶ��   -6we
    float   qKI;          //Integral Gain
    float   qWC;          //Anti-windup Gain
    float   qOutMax;      //ROGI Output maximum limit
    float   qOutMin;      //ROGI Output minimum limit
    float   qInRef;       //�����ο�ֵ
    float   qInMeas;      //����ֵ
    float   qOut1;         //ROGI Output; 1.15 format
    float   qOut2;
    }ROGI_CONTROL;

//����ROGI_CONTROL_handleΪROGI_CONTROLָ������

typedef ROGI_CONTROL*ROGI_CONTROL_handle;

#define ROGI_CONTROL_DEFAULTS {0,0,0,0,0,\
                             0,0,0,0,0,\
                             0,0,0,0,0,\
                             0,0,0,0,0,\
                             }

//�ṹ�����
extern ROGI_CONTROL ROGI_q_R;
extern ROGI_CONTROL ROGI_d_R;

//��������
//void ROGI_d_CONTROL_Postive( ROGI_CONTROL *p);           //����дҲ���ԣ������ټ�typedef ROGI_CONTROL*ROGI_CONTROL_handle;���������
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


#define ParameterNumber 118 //����ͨ�ű�������
extern float Paramet[ParameterNumber];

#define KI_current Paramet[86]
#define WC_current Paramet[87]
#define max_current1 Paramet[88]
#define min_current1 -Paramet[89]
#define sign_ROGI  Paramet[85]     ///ROGI�������ã�1������0�ر�
#define  theta_c   Paramet[84]     //�����ǣ���ֵ������Ϊ0



















#endif
