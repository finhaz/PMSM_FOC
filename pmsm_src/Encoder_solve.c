/*
 * Encoder_solve.c
 *
 *  Created on: 2021��4��14��
 *      Author: sea
 */




#include "includes.h"

float te_anl[40];
int i_te_anl=0;
void encoder_solver(void)
{
    Uint16 DirectionQep;
    long tmp;
//    int32 tmp1;
    long tmp_theta;


    tmp_theta=EQep1Regs.QPOSCNT+encoder_cal.CalibratedAngle;

    DirectionQep=EQep1Regs.QEPSTS.bit.QDF;

    //check an index occurrence�������¼���⣬����е��������������ƫ��
    if((EQep1Regs.QFLG.bit.IEL==1)&& encoder_cal.Init_IFlag==0)
    {
        encoder_cal.CalibratedAngle=EQep1Regs.QPOSILAT;
        EQep1Regs.QCLR.bit.IEL=1;//Clear interrupt flag
        encoder_cal.Init_IFlag=1;

    }

    if(EQep1Regs.QFLG.bit.IEL==1)
    {
        te_anl[i_te_anl]=encoder_cal.theta_mech;
        i_te_anl++;
        if(i_te_anl==40)
            i_te_anl=0;
    }


    if(tmp_theta<0)
        tmp_theta=tmp_theta+QPO_Pulse;
    else if(tmp_theta>QPO_Pulse)
        tmp_theta=tmp_theta-QPO_Pulse;

        encoder_cal.theta_mech=(tmp_theta)*twopi/QPO_Pulse;



    //��λ��ʱ�жϱ�ʶ��ͨ��EQep��λ���ڶ�ʱ������ƥ����λ�����ڸ�������
    //high speed�̶�ʱ�䣬���Ӧ������Ŀ,M������X/T
    if(EQep1Regs.QFLG.bit.UTO==1)//if uint timeout(one 100Hz period)
    {
        //Differncentiator
        encoder_cal.newp=(unsigned long)EQep1Regs.QPOSLAT;//Latched POSCNT value

        if(DirectionQep==0)// POSCNT�ݼ�����
        {
            if(encoder_cal.newp>encoder_cal.oldp)
                tmp=-(QPO_Pulse-encoder_cal.newp+encoder_cal.oldp);
            else
                tmp=encoder_cal.newp-encoder_cal.oldp;
        }
        else if(DirectionQep==1)// POSCNT��������
        {
            if(encoder_cal.newp>encoder_cal.oldp)
                tmp=encoder_cal.newp-encoder_cal.oldp;
            else
                tmp=QPO_Pulse+encoder_cal.newp-encoder_cal.oldp;
        }
        //1000Hz->T=0.001s
        //��X=tmp
        //rpm->��X/T/QPO_Pulse=tmp/0.001s/QPO_Pulse=tmp*60*2000/QPO_Pulse
        encoder_cal.SpeedRpm_pr=120000*tmp/QPO_Pulse;
        encoder_cal.oldp=encoder_cal.newp;
        EQep1Regs.QCLR.bit.UTO=1; //Clear interrupt flag
    }


//    //��λλ���¼���־λ��������������֮���ʱ����е�������
//    //low speed����������֮���Ӧ��ʱ��
//
//    if(EQep1Regs.QEPSTS.bit.UPEVNT==1)//Unit postion event
//    {
//        if(EQep1Regs.QEPSTS.bit.COEF==0)//No Caputre overflow
//            tmp1=(unsigned int)EQep1Regs.QCPRDLAT;//temp1=t2-t1
//        else                                      ///No Caputre overflow
//            tmp1=0xFFFF;
//        //CAP clock=Sysclk/2^7=150M/128
//        //Unit clock=QPO_Pulse/2^5=QPO_Pulse/32
//        //����һȦ�������1min=60s
//        // temp1 = t2-t1
//        encoder_cal.SpeedRpm_pr=549316.40625/tmp1; //60s*32*150M/128/4096
//        EQep1Regs.QEPSTS.all=0x88;//Clear Uint postion event flag and overflow error flag
//    }


    Wm_Filter.x_in=encoder_cal.SpeedRpm_pr/9.55;

    Filter_AVR_CALC(&Wm_Filter);

    encoder_cal.w_mech=Wm_Filter.y;

//    encoder_cal.w_mech=encoder_cal.SpeedRpm_pr/9.55;

//    encoder_cal.w_mech=0.98*encoder_cal.w_mech+0.02*encoder_cal.SpeedRpm_pr/9.55;

}



void encoder_reset(void)
{
    EQep1Regs.QPOSCNT=0;
//    EQep1Regs.QCLR.bit.UTO=1;
//    EQep1Regs.QPOSINIT = INDEX_OFFSET;    // eQEP Position Counter Initialization Value

    encoder_cal.enc_last=0;
    encoder_cal.enc=0;
//    encoder_cal.CalibratedAngle=INDEX_OFFSET;
    encoder_cal.Init_IFlag=0;
}


