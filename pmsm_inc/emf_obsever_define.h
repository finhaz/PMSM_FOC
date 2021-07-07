/*
 * emf_obsever_define.h
 *
 *  Created on: 2021��4��9��
 *      Author: sea
 */

#ifndef PMSM_INC_EMF_OBSEVER_DEFINE_H_
#define PMSM_INC_EMF_OBSEVER_DEFINE_H_

typedef struct{
    float Ualpha;//�������㷨alpha���ѹ
    float Ubeta;//�������㷨beta���ѹ
    float ialpha;//�������㷨alpha�����
    float ibeta;//�������㷨beta�����
    float ialpha_est;//��һ��/����alpha�����
    float ibeta_est;//��һ��/����beta�����
    float ealpha_est;//����alpha�ᷴ����
    float ebeta_est;//����beta�ᷴ����
}EMF_Cal_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define EMF_CAL_DEFAULTS {0,0,0,0,0,0,0,0}


typedef struct{
    float kpw;
    float kiw;
    float error;
    float we_I;
    float we;//˲ʱ�����ٶ�
    float theta_fan; //����Ƕȡ�����������任
}PLL_CAL_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define PLL_CAL_DEFAULTS {8,0.1,0,0,0,0}

typedef struct{
    float e_d_filter;
    float e_q_filter;
    float Emf_ob_d;
    float Emf_ob_q;
    float we_sgn;//˲ʱ�����ٶ�
    float theta_sgn; //����Ƕȡ�����������任
}PLL_EMFC_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define PLL_EMFC_DEFAULTS {0,0,0,0,0,0}


#endif /* PMSM_INC_EMF_OBSEVER_DEFINE_H_ */
