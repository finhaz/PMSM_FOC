/*
 * Encoder_solve_define.h
 *
 *  Created on: 2021��4��14��
 *      Author: sea
 */

#ifndef PMSM_INC_ENCODER_SOLVE_DEFINE_H_
#define PMSM_INC_ENCODER_SOLVE_DEFINE_H_

typedef struct{
    Uint32 enc_last;//��һ�εı�����ֵ
    Uint32 enc;//��ǰ������ֵ
    int32 newp;
    int32 oldp;
    Uint32 CalibratedAngle;//Raw angular offset between encoder index and phase a
    float theta_mech;//��ǰ��е�Ƕ�
    float w_mech;//��е���ٶ�
    float SpeedRpm_pr;
    Uint16 Init_IFlag;
}Encoder_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define Encoder_GET_DEFAULTS {0,0,0,0,0,0,0,0,0}

#endif /* PMSM_INC_ENCODER_SOLVE_DEFINE_H_ */
