/*
 * emf_obsever_define.h
 *
 *  Created on: 2021年4月9日
 *      Author: sea
 */

#ifndef PMSM_INC_EMF_OBSEVER_DEFINE_H_
#define PMSM_INC_EMF_OBSEVER_DEFINE_H_

typedef struct{
    float Ualpha;//进估算算法alpha轴电压
    float Ubeta;//进估算算法beta轴电压
    float ialpha;//进估算算法alpha轴电流
    float ibeta;//进估算算法beta轴电流
    float ialpha_est;//上一次/估算alpha轴电流
    float ibeta_est;//上一次/估算beta轴电流
    float ealpha_est;//估算alpha轴反电势
    float ebeta_est;//估算beta轴反电势
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
    float we;//瞬时估算速度
    float theta_fan; //估算角度——参与坐标变换
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
    float we_sgn;//瞬时估算速度
    float theta_sgn; //估算角度——参与坐标变换
}PLL_EMFC_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define PLL_EMFC_DEFAULTS {0,0,0,0,0,0}


#endif /* PMSM_INC_EMF_OBSEVER_DEFINE_H_ */
