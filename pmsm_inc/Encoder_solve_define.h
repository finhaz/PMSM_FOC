/*
 * Encoder_solve_define.h
 *
 *  Created on: 2021年4月14日
 *      Author: sea
 */

#ifndef PMSM_INC_ENCODER_SOLVE_DEFINE_H_
#define PMSM_INC_ENCODER_SOLVE_DEFINE_H_

typedef struct{
    Uint32 enc_last;//上一次的编码器值
    Uint32 enc;//当前编码器值
    int32 newp;
    int32 oldp;
    Uint32 CalibratedAngle;//Raw angular offset between encoder index and phase a
    float theta_mech;//当前机械角度
    float w_mech;//机械角速度
    float SpeedRpm_pr;
    Uint16 Init_IFlag;
}Encoder_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define Encoder_GET_DEFAULTS {0,0,0,0,0,0,0,0,0}

#endif /* PMSM_INC_ENCODER_SOLVE_DEFINE_H_ */
