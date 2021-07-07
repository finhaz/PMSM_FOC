/*
 * vc_position.c
 *
 *  Created on: 2021年4月12日
 *      Author: sea
 */
#include "includes.h"

//电压外环弱磁_id指令
void weak_loop(void)
{
    float Udc_M;
    float Us;
    float iq_temp;
    float i_temp;


    Udc_M=M*0.577*Adcget.Vdc;
    Us=sqrt(Udq_to_Ualphabeta.Ds*Udq_to_Ualphabeta.Ds+Udq_to_Ualphabeta.Qs*Udq_to_Ualphabeta.Qs);
    Weak_R.qInMeas=Us;//_IQsqrt((_IQmpy(Ud,Ud)+_IQmpy(Uq,Uq)));

    if(weaking_start==1)//允许弱磁
    {
        Weak_R.qInRef=Udc_M;
        Weak_R.qKp=kp_weak;//;//163;
        Weak_R.qKi=ki_weak;//16;
        Weak_R.qOutMax=0;//max_weak;//
        Weak_R.qOutMin=min_weak;//-16384;//-32768;//
        PI_CONTROL_CALC(&Weak_R);
        id_fankui=(id_start+Weak_R.qOut);


    }

    if(id_start==0)
    {
        i_temp=ASR.qOut*ASR.qOut-id_fankui*id_fankui;
        iq_temp=sqrt(i_temp);
        //给定q轴电流
        if(ASR.qOut>0)
            iq_fankui=iq_temp;
        else
            iq_fankui=-iq_temp;
    }

}


//电压外环弱磁_超前角
void weak_loop_y(void)
{
    float Udc_M;
    float Us;
    float id_comp;

    Udc_M=M*0.577*Adcget.Vdc;
    Us=sqrt(Udq_to_Ualphabeta.Ds*Udq_to_Ualphabeta.Ds+Udq_to_Ualphabeta.Qs*Udq_to_Ualphabeta.Qs);
    Weak_R.qInMeas=Us;

    if(weaking_start==1)//允许弱磁
    {
        Weak_R.qInRef=Udc_M;
        Weak_R.qKp=kp_weak;
        Weak_R.qKi=ki_weak;
        Weak_R.qOutMax=0;
        Weak_R.qOutMin=min_weak;
        PI_CONTROL_CALC(&Weak_R);

        id_comp=ASR.qOut*sin(Weak_R.qOut);

//        if(id_comp<=0)
//            id_fankui=id_start+id_comp;
//        else
//            id_fankui=id_start;

        id_fankui=id_start+id_comp;

        iq_fankui=ASR.qOut*cos(Weak_R.qOut);
    }
}

