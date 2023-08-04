/*
 * Speed_control.c
 *
 *  Created on: 2021��7��13��
 *      Author: sea
 */

#include "includes.h"

#define J 0.0002293
#define Be 0.0001119375


void speed_loop_adrc(void)
{
    stage_asr++;
    if(stage_asr==50)//�ٶȻ�
    {
        stage_asr=0;

        ADRC_speed.qInMeas=jiaosudu1_asr;
        ADRC_speed.qInRef=speed_cankao;
        ADRC_speed.enable=1;
        ADRC_speed.qKp=56.6507;
        //ADRC_speed.qKi=0.0001;
        ADRC_speed.qOutMax=max_speed;
        ADRC_speed.qOutMin=min_speed;
        ADRC_speed.b=1.5*pn*fluxm/J;
        ADRC_speed.Tk_s=T*50;

        //ADRC_speed.alpha_kp=1;
        //ADRC_speed.filter_kp=0.01;

        ADRC_speed.qOut=iq_fankui;

        ADRC_REGULATOR_CALC(&ADRC_speed);//ת�ٻ�

    }

    ASR.qOut=ADRC_speed.qOut;
}


//�ٶȻ�����
void speed_loop(void)
{
    stage_asr++;
    if(stage_asr==50)//�ٶȻ�
    {
        stage_asr=0;

        ASR.qInMeas=jiaosudu1_asr;
        ASR.qInRef=speed_cankao;
        ASR.qKp=kp_speed;//_IQ(0.0025);//�ٶȻ�����ϵ��_IQ(0.1);
        ASR.qKi=ki_speed;//_IQ(0.00025);//_IQ(0.001);//�ٶȻ�����ϵ��_IQ(0.0005)
        ASR.qOutMax=max_speed;
        ASR.qOutMin=min_speed;
        PI_CONTROL_CALC(&ASR);//����������
    }

    iq_fankui=ASR.qOut;//����q�����
}
