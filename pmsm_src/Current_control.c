/*
 * Current_control.c
 *
 *  Created on: 2021年4月10日
 *      Author: sea
 */
#include "includes.h"

void ABC_control(void)
{
    Ramp_MACRO();
    OpenStartUp_angle();
    A_IA_R.qInRef=iq_start_0*cos(thetam_given);
    A_IB_R.qInRef=iq_start_0*cos(thetam_given-TWObyTHREE*PI);
    A_IC_R.qInRef=iq_start_0*cos(thetam_given+TWObyTHREE*PI);


    //给定A轴调节
    A_IA_R.qKp=kp_iq;
    A_IA_R.qKi=ki_iq;
    A_IA_R.qOutMax=max_current;
    A_IA_R.qOutMin=min_current;

    //给定B轴调节
    A_IB_R.qKp=kp_iq;
    A_IB_R.qKi=ki_iq;
    A_IB_R.qOutMax=max_current;
    A_IB_R.qOutMin=min_current;

    //给定C轴调节
    A_IC_R.qKp=kp_iq;
    A_IC_R.qKi=ki_iq;
    A_IC_R.qOutMax=max_current;
    A_IC_R.qOutMin=min_current;


    A_IA_R.qInMeas=Ialphabeta_to_Idq.As;
    PI_CONTROL_CALC(&A_IA_R);
    A_IB_R.qInMeas=Ialphabeta_to_Idq.Bs;
    PI_CONTROL_CALC(&A_IB_R);
    A_IC_R.qInMeas=Ialphabeta_to_Idq.Cs;
    PI_CONTROL_CALC(&A_IC_R);

    Udq_to_Ualphabeta.As=A_IA_R.qOut;
    Udq_to_Ualphabeta.Bs=A_IB_R.qOut;
    Udq_to_Ualphabeta.Cs=A_IC_R.qOut;


    clark_calc(&Udq_to_Ualphabeta);
}

void ABC_Vol_control(void)
{
    Ramp_MACRO();
    OpenStartUp_angle();
    Udq_to_Ualphabeta.As=Paramet[70]*cos(thetam_given);
    Udq_to_Ualphabeta.Bs=Paramet[70]*cos(thetam_given-TWObyTHREE*PI);
    Udq_to_Ualphabeta.Cs=Paramet[70]*cos(thetam_given+TWObyTHREE*PI);
    clark_calc(&Udq_to_Ualphabeta);
}

void AlphaBeta_control(void)
{

    Ramp_MACRO();
    OpenStartUp_angle();


    A_IA_R.qInRef=iq_start_0*(-sin(thetam_given));
    A_IB_R.qInRef=iq_start_0*cos(thetam_given);

    //给定A轴调节
    A_IA_R.qKp=kp_iq;
    A_IA_R.qKi=ki_iq;
    A_IA_R.qOutMax=max_current;
    A_IA_R.qOutMin=min_current;


    //给定B轴调节
    A_IB_R.qKp=kp_iq;
    A_IB_R.qKi=ki_iq;
    A_IB_R.qOutMax=max_current;
    A_IB_R.qOutMin=min_current;



    A_IA_R.qInMeas=Ialphabeta_to_Idq.Alpha;
    PI_CONTROL_CALC(&A_IA_R);
    A_IB_R.qInMeas=Ialphabeta_to_Idq.Beta;
    PI_CONTROL_CALC(&A_IB_R);

    Udq_to_Ualphabeta.Alpha=A_IA_R.qOut;
    Udq_to_Ualphabeta.Beta=A_IB_R.qOut;
}



void AlphaBeta_control_PR(void)
{
    float delt_Ialpha=0;
    float delt_Ibeta=0;

    Ramp_MACRO();
    OpenStartUp_angle();

    //------------------电流环--------------------
    delt_Ialpha = iq_start_0*(-sin(thetam_given)) - Ialphabeta_to_Idq.Alpha;
    delt_Ibeta = iq_start_0*cos(thetam_given) - Ialphabeta_to_Idq.Beta;
    //  delt_Ialpha = PR_Ualpha.PR_out + PR_Ualpha2.PR_out - I_alpha;
    //  delt_Ibeta = PR_Ubeta.PR_out + PR_Ubeta2.PR_out - I_beta;
    //给定alpha轴调节
    PR_Ialpha.Kr=0;
    PR_Ialpha.Kp=1;
    PR_Ialpha.wi=5;
    PR_Ialpha.prout_max=max_current;
    PR_Ialpha.prout_min=min_current;

    //给定beta轴调节
    PR_Ibeta.Kr=0;
    PR_Ibeta.Kp=1;
    PR_Ibeta.wi=5;
    PR_Ibeta.prout_max=max_current;
    PR_Ibeta.prout_min=min_current;

    //alpha轴调节
    PR_Ialpha.wg=speed_cankao;
    PR_Ialpha.error=delt_Ialpha;
    PR_CONTROL_CALC(&PR_Ialpha);//输出d轴调制电压Ud
    //beta轴调节
    PR_Ibeta.wg=speed_cankao;
    PR_Ibeta.error=delt_Ibeta;
    PR_CONTROL_CALC(&PR_Ibeta);//输出q轴调制电压Uq

    Udq_to_Ualphabeta.Alpha=PR_Ialpha.PR_out;
    Udq_to_Ualphabeta.Beta=PR_Ibeta.PR_out;


}




//包含电流环、启动，位置观测计算，转速估测计算
//电流环控制
void current_loop(void)
{

//-----------------电流环------------------------------------------------
    park_calc(&Ialphabeta_to_Idq); //得到反馈量id、iq
//----------------pi参数----------------------------------------

    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==0&&speed_cankao>(speed_tran+500))
    {
        A_Id_R.qdSum=A_Id_R.qOut+jiaosudu1_asr*Ls*A_Iq_R.qInRef;
        A_Iq_R.qdSum=A_Iq_R.qOut-jiaosudu1_asr*(Ls*A_Id_R.qInRef+fluxm);
        FlagRegs.flagsystem.bit.jieou_inuse_sign=1;
    }

//    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==0&&speed_cankao>(speed_tran+500))
//    {
//        A_Id_R.qdSum=A_Id_R.qOut+jiaosudu1_asr*Ls*A_Iq_R.qInMeas ;
//        A_Iq_R.qdSum=A_Iq_R.qOut-jiaosudu1_asr*(Ls*A_Id_R.qInMeas +fluxm);
//        FlagRegs.flagsystem.bit.jieou_inuse_sign=1;
//    }

    //给定q轴调节
    A_Iq_R.qKp=kp_iq;
    A_Iq_R.qKi=ki_iq;
    A_Iq_R.qOutMax=max_current;
    A_Iq_R.qOutMin=min_current;
    //给定d轴调节
    A_Id_R.qKp=kp_id;
    A_Id_R.qKi=ki_id;
    A_Id_R.qOutMax=max_current;
    A_Id_R.qOutMin=min_current;


    A_Iq_R.qInMeas=Ialphabeta_to_Idq.Qs;//q反馈电流
    A_Id_R.qInMeas=Ialphabeta_to_Idq.Ds;//d反馈电流

//  //滤波后电流处理
//    id_filtrate.X_in=Ialphabeta_to_Idq.Ds;
//    FILTRATE_CALC(&id_filtrate);
//
//    iq_filtrate.X_in=Ialphabeta_to_Idq.Qs;
//    FILTRATE_CALC(&iq_filtrate);

//    A_Iq_R.qInMeas=iq_filtrate.Y ;//q反馈电流
//    A_Id_R.qInMeas=id_filtrate.Y;//d反馈电流

    PI_CONTROL_CALC(&A_Iq_R);//输出q轴调制电压Uq
    PI_CONTROL_CALC(&A_Id_R);//输出d轴调制电压Ud

    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==1)
    {
        Udq_to_Ualphabeta.Ds=A_Id_R.qOut-jiaosudu1_asr*Ls*A_Iq_R.qInRef;
        Udq_to_Ualphabeta.Qs=A_Iq_R.qOut+jiaosudu1_asr*(Ls*A_Id_R.qInRef+fluxm);
        if(Udq_to_Ualphabeta.Ds>max_current)
            Udq_to_Ualphabeta.Ds=max_current;
        if(Udq_to_Ualphabeta.Ds<min_current)
            Udq_to_Ualphabeta.Ds=min_current;

        if(Udq_to_Ualphabeta.Qs>max_current)
            Udq_to_Ualphabeta.Qs=max_current;
        if(Udq_to_Ualphabeta.Qs<min_current)
            Udq_to_Ualphabeta.Qs=min_current;
    }
    else
    {
        Udq_to_Ualphabeta.Ds=A_Id_R.qOut;
        Udq_to_Ualphabeta.Qs=A_Iq_R.qOut;
    }

    //坐标变换得到调制电压Uα、Uβ

    ipark_calc(&Udq_to_Ualphabeta);//输出Uα、Uβ


}

void current_loop_adrc(void)
{
    park_calc(&Ialphabeta_to_Idq); //得到反馈量id、iq

    //给定q轴调节
//    ADRC_iq.qKp=1000;
//    ADRC_iq.qKi=27.1186;//20K
//    ADRC_iq.qKi=18.0791;//30K
    ADRC_iq.b=1/Lq;
    ADRC_iq.Tk_s=T;
//    ADRC_iq.alpha_kp=0.8;
//    ADRC_iq.filter_kp=0.01;
//    ADRC_iq.alpha_ki=0.9;
//    ADRC_iq.filter_ki=0.01;
//    ADRC_iq.alpha_kp=1;
//    ADRC_iq.filter_kp=0.01;
//    ADRC_iq.alpha_ki=1;
//    ADRC_iq.filter_ki=0.01;

    ADRC_iq.qOutMax=max_current;
    ADRC_iq.qOutMin=min_current;

    //给定d轴调节
//    ADRC_id.qKp=1000;
//    ADRC_id.qKi=37.0302;//20K
//    ADRC_id.qKi=24.6868;//30K
    ADRC_id.b=1/Ld;
    ADRC_id.Tk_s=T;
//    ADRC_id.alpha_kp=0.8;
//    ADRC_id.filter_kp=0.01;
//    ADRC_id.alpha_ki=0.9;
//    ADRC_id.filter_ki=0.01;
//    ADRC_id.alpha_kp=1;
//    ADRC_id.filter_kp=0.01;
//    ADRC_id.alpha_ki=1;
//    ADRC_id.filter_ki=0.01;


    ADRC_id.qOutMax=max_current;
    ADRC_id.qOutMin=min_current;

    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==0&&speed_cankao>(speed_tran+500))
    {
        FlagRegs.flagsystem.bit.jieou_inuse_sign=1;
        ADRC_iq.enable=1;
        ADRC_id.enable=1;

        ADRC_iq.qdSum=(ADRC_iq.qOut*ADRC_iq.b+ADRC_iq.z2)/ADRC_iq.qKi;
        ADRC_id.qdSum=(ADRC_id.qOut*ADRC_id.b+ADRC_id.z2)/ADRC_id.qKi;

    }

//    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==0&&speed_cankao>(speed_tran+500))
//    {
//        ADRC_id.qdSum=ADRC_id.qOut+jiaosudu1_asr*Ls*ADRC_iq.qInRef;
//        ADRC_iq.qdSum=ADRC_iq.qOut-jiaosudu1_asr*(Ls*ADRC_id.qInRef+fluxm);
//        FlagRegs.flagsystem.bit.jieou_inuse_sign=1;
//    }



    ADRC_iq.qInMeas=Ialphabeta_to_Idq.Qs;//q反馈电流
    ADRC_id.qInMeas=Ialphabeta_to_Idq.Ds;//d反馈电流

//    if(ADRC_iq.enable==0)
//    {
//        ADRC_id.qOut=ADRC_id.qOut-jiaosudu1_asr*Ls*ADRC_iq.qInRef;
//        ADRC_iq.qOut=ADRC_iq.qOut+jiaosudu1_asr*(Ls*ADRC_id.qInRef+fluxm);
//
//    }

    ADRC_REGULATOR_CALC(&ADRC_iq);
    ADRC_REGULATOR_CALC(&ADRC_id);


//    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==1)
//    {
//        if(ADRC_iq.enable==0)
//        {
//            Udq_to_Ualphabeta.Ds=ADRC_id.qOut-jiaosudu1_asr*Ls*ADRC_iq.qInRef;
//            Udq_to_Ualphabeta.Qs=ADRC_iq.qOut+jiaosudu1_asr*(Ls*ADRC_id.qInRef+fluxm);
//            if(Udq_to_Ualphabeta.Ds>max_current)
//                Udq_to_Ualphabeta.Ds=max_current;
//            if(Udq_to_Ualphabeta.Ds<min_current)
//                Udq_to_Ualphabeta.Ds=min_current;
//
//            if(Udq_to_Ualphabeta.Qs>max_current)
//                Udq_to_Ualphabeta.Qs=max_current;
//            if(Udq_to_Ualphabeta.Qs<min_current)
//                Udq_to_Ualphabeta.Qs=min_current;
//        }
//        else
//        {
//            //带解耦的ESO
//            Udq_to_Ualphabeta.Ds=ADRC_id.qOut;
//            Udq_to_Ualphabeta.Qs=ADRC_iq.qOut;
//        }
//
//        if(speed_cankao>(speed_tran+1500))
//        {
//           ADRC_iq.enable=1;
//           ADRC_id.enable=1;
//        }
//    }
//    else
//    {
//        Udq_to_Ualphabeta.Ds=ADRC_id.qOut;
//        Udq_to_Ualphabeta.Qs=ADRC_iq.qOut;
//    }


    Udq_to_Ualphabeta.Ds=ADRC_id.qOut;
    Udq_to_Ualphabeta.Qs=ADRC_iq.qOut;

    //坐标变换得到调制电压Uα、Uβ

    ipark_calc(&Udq_to_Ualphabeta);//输出Uα、Uβ

}
