/*
 * adrc_define.h
 *
 *  Created on: 2021��7��13��
 *      Author: sea
 */

#ifndef PMSM_INC_ADRC_DEFINE_H_
#define PMSM_INC_ADRC_DEFINE_H_

//ADRC������:TD���ڣ�PID���ƣ�ESOǰ��
typedef struct {
    float   qdSum;        //Integrator sum; 1.31 format
    float   currentError;
    float   U;
    float   qKp;          //Proportional Gain
    float   qKi;          //Integral Gain
    float   qKd;          //Der Gain
    float   qOutMax;      //PI Output maximum limit
    float   qOutMin;      //PI Output minimum limit
    float   qInRef;       //�����ο�ֵ
    float   qInMeas;      //����ֵ
    float   qOut;         //ADRC Output; 1.15 format
    float   z1;           //ESO��z1
    float   z2;           //ESO��z2,����״̬����
    float   w0;           //ESO����
    int enable;            //ESO��־λ
    float   Tk_s;         //ESO��ִ������
    float   b;            //ģ�Ͳ���

    float alpha_kp;
    float filter_kp;

    float alpha_ki;
    float filter_ki;

    float alpha_kd;
    float filter_kd;

    }ADRC_Regulator;
//����ADRC_Regulator_handleΪADRC_Regulatorָ������
typedef ADRC_Regulator* ADRC_handle;
#define ADRC_Regulator_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,0,0,0.9,0.01,0,0,0,0,}
#define ADRC_RegC_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,10000,0,0,0,0.8,0.01,0.8,0.01,0,0,}

#endif /* PMSM_INC_ADRC_DEFINE_H_ */
