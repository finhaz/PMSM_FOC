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

#ifdef PR

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
    PR_Ialpha.prout_max=270;
    PR_Ialpha.prout_min=-270;

    //给定beta轴调节
    PR_Ibeta.Kr=0;
    PR_Ibeta.Kp=1;
    PR_Ibeta.wi=5;
    PR_Ibeta.prout_max=270;
    PR_Ibeta.prout_min=-270;

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
#endif
