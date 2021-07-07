/*
 * para_init.c
 *
 *  Created on: 2021年2月15日
 *      Author: sea
 */

#include "includes.h"



//参数更新
void Paramet_Update(void)
{
//    if(speed_cankao<3000)
//    {
//        Paramet[I_kp]=0.6;
//        Paramet[I_ki]=0.0005;
//    }
//    else if(speed_cankao<3000)
//    {
//        Paramet[I_kp]=0.108;
//        Paramet[I_ki]=0.002;
//    }
}

//参数初始化
void Initparameter(void)
{
    ADCzero();
    PIZero();
    PRZero();
    VectorControl_zero();

    FlagRegs.flagfault.all = 0 ;
    FlagRegs.flagsystem.bit.sysonoff = 0;//停机
    FlagRegs.flagsystem.bit.faultoccur = 0 ;//故障未发生
    FlagRegs.flagsystem.bit.pre_dir_sign = 0 ;//先预定位
    FlagRegs.flagsystem.bit.AC0CheckFinished=0;//采样校零未完成
    FlagRegs.flagsystem.bit.moder_of_stop=0;

    Switchsystem=0;//停机
    //系统参数数组初始化
    int i ;
    for(i=0;i<44;i++)
    {
        Paramet[i]=0;
    }

    vmax_current=3000;
    max_voltage=2499;
    min_voltage=1540;

    iq_start_0=10;
    sudu_max=200;



    //速度环参数

//    kp_speed=0.04;
//    ki_speed=0.001;

//    kp_speed=0.002;
//    ki_speed=0.0001;

    kp_speed=0.4;
    ki_speed=0.000001;

    max_speed=25;
    min_speed=-25;

    //电流环参数
//    kp_iq=0.108;
//    ki_iq=0.002;
//
//    kp_id=0.108;
//    ki_id=0.002;

    kp_iq=0.07;
    ki_iq=0.0005;

    kp_id=0.07;
    ki_id=0.0005;

    max_current=296.13;
    min_current=-max_current;

    weaking_start=1;
    jia_jian_su=0;
    id_ratio=10;

    M=0.95;

//    //直接给id指令
//    kp_weak=0.001;
//    ki_weak=0.0001;
//    max_weak=0;
//    min_weak=-18;

    //超前角指令
    kp_weak=0.001;
    ki_weak=0.0001;
    max_weak=0;
    min_weak=-1.57;

    id_mode=0;

    Kw=200;
    speed_tran=500;



}

//PI调节器清零
void PIZero(void)
{
    ASR.qdSum=0;
    A_Id_R.qdSum=0;
    A_Iq_R.qdSum=0;
    Weak_R.qdSum=0;

    A_IA_R.qdSum=0;
    A_IB_R.qdSum=0;
    A_IC_R.qdSum=0;
}


void PRZero(void)
{
    //电流alpha
    PR_Ialpha.input_of_forward_integrator=0;
    PR_Ialpha.output_of_backward_integrator=0;
    PR_Ialpha.output_of_feedback=0;
    PR_Ialpha.output_of_forward_integratorofi=0;
    PR_Ialpha.error=0;
    PR_Ialpha.PR_out=0;
    //电流beta
    PR_Ibeta.input_of_forward_integrator=0;
    PR_Ibeta.output_of_backward_integrator=0;
    PR_Ibeta.output_of_feedback=0;
    PR_Ibeta.output_of_forward_integratorofi=0;
    PR_Ibeta.error=0;
    PR_Ibeta.PR_out=0;
}

//归零初始化
void VectorControl_zero(void)
{

    FlagRegs.flagsystem.bit.pre_dir_sign=0;
    FlagRegs.flagsystem.bit.if_start=0;
    FlagRegs.flagsystem.bit.speed_loop_sign=0;
    FlagRegs.flagsystem.bit.jieou_inuse_sign=0;
    FlagRegs.flagsystem.bit.free_stop_sign=0;
    FlagRegs.flagsystem.bit.stop_pedir_sign=0;

    Emf_get.ealpha_est=0;
    Emf_get.ebeta_est=0;
    Emf_get.ialpha_est=0;
    Emf_get.ibeta_est=0;

    Pll.theta_fan=0;
    Pll.we_I=0;
    Pll.we=0;

    We_Filter.count=0;
    We_Filter.x_sum=0;


    stage_asr=0;

    thetam_given=0;
    speed_cankao=0;
    pre_dir_count=0;


    delt_theta=0;
    id_fankui=0;

    break_stage=0;
    zhiliu_count=0;

    encoder_reset();
    MARS_reset();
}


void SYSTEMoff(void)
{
    if(FlagRegs.flagsystem.bit.moder_of_stop==1)//
    {
        PWMzero();//零矢量
    }
    else
    {
        PWMoff();//封锁PWM
    }
    PIZero();
    VectorControl_zero();
}

