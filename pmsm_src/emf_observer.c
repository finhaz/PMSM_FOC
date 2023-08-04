/*
 * emf_observer.c
 *
 *  Created on: 2021年2月15日
 *      Author: sea
 */

#include "includes.h"

////20K
//#define Ls_by_T 14.75 //0.0007375H/0.00005
//#define rTbyL 0.0136//R*T/L=0.2*0.00005/0.0007375
//#define TbyL 0.0678//T/L=0.00005/0.0007375
//#define k1mT -1.7878//k1*T
//#define k2mT 11.9655//k2*T


//30K
#define Ls_by_T 22.125 //0.0007375H/0.000033
#define rTbyL 0.009//R*T/L=0.2*0.000033/0.00043
#define TbyL 0.0452//T/L=0.000033/0.0007375
#define k1mT -1.7919//k1*T
#define k2mT 17.9393//k2*T


////40K
//#define Ls_by_T 29.5 //0.0007375H/0.000025
//#define rTbyL 0.0068//R*T/L=0.2*0.000025/0.00043
//#define TbyL 0.0339//T/L=0.000033/0.0007375
//#define k1mT -1.7939//k1*T
//#define k2mT 23.9130//k2*T



//pll
void Pll_get_speed(void)
{
    float gain;

    Pll.error=(-Emf_get.ealpha_est)*cos(Pll.theta_fan)-Emf_get.ebeta_est*sin(Pll.theta_fan);
    Pll.we_I=(Pll.we_I+Pll.kiw*Pll.error);
    Pll.we=(Pll.we_I+Pll.kpw*Pll.error);

    gain=Pll.we*T;

    Pll.theta_fan=Pll.theta_fan+gain;
    if(Pll.theta_fan>=twopi)
    {
        Pll.theta_fan=Pll.theta_fan-twopi;
    }

    We_Filter.x_in=Pll.we;
//    We_Filter.n_win=400;
    Filter_AVR_CALC(&We_Filter);

}

void Pll_emf_speed(void)
{
    int sgn=1;

//    Emf_ob.Alpha=Emf_get.ealpha_est;
//    Emf_ob.Beta=Emf_get.ebeta_est;
//    Emf_ob.Angle=theta_sgn;
//    park_calc(&Emf_ob);

    PLL_e.Emf_ob_d =Emf_get.ealpha_est*cos(PLL_e.theta_sgn) + Emf_get.ebeta_est*sin(PLL_e.theta_sgn);
    PLL_e.Emf_ob_q =Emf_get.ebeta_est*cos(PLL_e.theta_sgn) - Emf_get.ealpha_est*sin(PLL_e.theta_sgn);


//    if(Emf_ob.Qs>0)
//        sgn=1;
//    if(Emf_ob.Qs<0)
//        sgn=-1;
//    we_sgn=(Emf_ob.Qs-sgn*Emf_ob.Ds)/fluxm;


//    e_d_filter=0.6*e_d_filter+0.4*Emf_ob.Ds;
//    e_q_filter=0.6*e_q_filter+0.4*Emf_ob.Qs;

    PLL_e.e_d_filter=0.6*PLL_e.e_d_filter+0.4*PLL_e.Emf_ob_d;
    PLL_e.e_q_filter=0.6*PLL_e.e_q_filter+0.4*PLL_e.Emf_ob_q;

    if(PLL_e.e_q_filter>0)
        sgn=1;
    if(PLL_e.e_q_filter<0)
        sgn=-1;
    PLL_e.we_sgn=(PLL_e.e_q_filter-sgn*PLL_e.e_d_filter)/fluxm;


    PLL_e.theta_sgn=PLL_e.theta_sgn+PLL_e.we_sgn*T;
    if(PLL_e.theta_sgn>=twopi)
    {
        PLL_e.theta_sgn=PLL_e.theta_sgn-twopi;
    }

    We_Filter2.x_in=PLL_e.we_sgn;
    Filter_AVR_CALC(&We_Filter2);
}


void Pll_gemf_speed(void)
{
    float gain;
    float emf_all;
    float w0;//增强PLL动态性，基波转速

    emf_all=Emf_get.ealpha_est*Emf_get.ealpha_est+Emf_get.ebeta_est*Emf_get.ebeta_est;
    w0=sqrt(emf_all)/fluxm;

    Pll.error=(-Emf_get.ealpha_est)*cos(Pll.theta_fan)-Emf_get.ebeta_est*sin(Pll.theta_fan);
    Pll.we_I=(Pll.we_I+Pll.kiw*Pll.error);
    Pll.we=(Pll.we_I+Pll.kpw*Pll.error)+w0;

    gain=Pll.we*T;

    Pll.theta_fan=Pll.theta_fan+gain;
    if(Pll.theta_fan>=twopi)
    {
        Pll.theta_fan=Pll.theta_fan-twopi;
    }

    We_Filter.x_in=Pll.we;
//    We_Filter.n_win=400;
    Filter_AVR_CALC(&We_Filter);

}

//反电动势观测器算法
void fandianshi_speed(void)
{

    //-----------------------算法-----------------------------------------------------------------------
    Emf_get.ialpha_est=Emf_get.ialpha;
    Emf_get.ibeta_est=Emf_get.ibeta;

    Emf_get.Ualpha=Udq_to_Ualphabeta.Alpha;
    Emf_get.Ubeta=Udq_to_Ualphabeta.Beta;
    Emf_get.ialpha=Ialphabeta_to_Idq.Alpha;
    Emf_get.ibeta=Ialphabeta_to_Idq.Beta;

    Emf_get.ealpha_est=Emf_get.Ualpha-rs*Emf_get.ialpha-Ls_by_T*(Emf_get.ialpha-Emf_get.ialpha_est);
    Emf_get.ebeta_est=Emf_get.Ubeta-rs*Emf_get.ibeta-Ls_by_T*(Emf_get.ibeta-Emf_get.ibeta_est);


    Pll_get_speed();

}

//反电动势观测器算法
void fandianshi_speed_improve(void)
{

    float Ialpha_next=0;
    float Ibeta_next=0;

    float vame=0;
    float vbme=0;


    //状态观测器
    Emf_get.Ualpha=Udq_to_Ualphabeta.Alpha;
    Emf_get.Ubeta=Udq_to_Ualphabeta.Beta;
    Emf_get.ialpha=Ialphabeta_to_Idq.Alpha;
    Emf_get.ibeta=Ialphabeta_to_Idq.Beta;


    //预报
    vame=Emf_get.Ualpha-Emf_get.ealpha_est;
    vbme=Emf_get.Ubeta-Emf_get.ebeta_est;


    Ialpha_next=Emf_get.ialpha-rTbyL*Emf_get.ialpha+TbyL*vame;
    Ibeta_next=Emf_get.ibeta-rTbyL*Emf_get.ibeta+TbyL*vbme;

    //校正


    Emf_get.ealpha_est=Emf_get.Ualpha-rs*Emf_get.ialpha-Ls_by_T*(Ialpha_next-Emf_get.ialpha_est)*0.5;
    Emf_get.ebeta_est=Emf_get.Ubeta-rs*Emf_get.ibeta-Ls_by_T*(Ibeta_next-Emf_get.ibeta_est)*0.5;

    Emf_get.ialpha_est=Emf_get.ialpha;
    Emf_get.ibeta_est=Emf_get.ibeta;

    Pll_get_speed();
//    Pll_emf_speed();
//    Pll_gemf_speed();
}


void STO_speed(void)
{
    float Ia_err=0;
    float Ib_err=0;
    float vame=0;
    float vbme=0;
    float eatoB=0;
    float ebtoA=0;

    float ialpha_next=0;
    float ibeta_next=0;
    float ealpha_next=0;
    float ebeta_next=0;

    //状态观测器
    Emf_get.Ualpha=Udq_to_Ualphabeta.Alpha;
    Emf_get.Ubeta=Udq_to_Ualphabeta.Beta;
    Emf_get.ialpha=Ialphabeta_to_Idq.Alpha;
    Emf_get.ibeta=Ialphabeta_to_Idq.Beta;

    Ia_err=Emf_get.ialpha_est-Emf_get.ialpha;
    Ib_err=Emf_get.ibeta_est-Emf_get.ibeta;
    vame=Emf_get.Ualpha-Emf_get.ealpha_est;
    vbme=Emf_get.Ubeta-Emf_get.ebeta_est;

//    Emf_get.ialpha_est=Emf_get.ialpha_est-rTbyL*Emf_get.ialpha_est+TbyL*vame+k1mT*Ia_err;
//    Emf_get.ibeta_est=Emf_get.ibeta_est -rTbyL*Emf_get.ibeta_est+TbyL*vbme+k1mT*Ib_err;

    ialpha_next=Emf_get.ialpha_est-rTbyL*Emf_get.ialpha_est+TbyL*vame+k1mT*Ia_err;
    ibeta_next=Emf_get.ibeta_est -rTbyL*Emf_get.ibeta_est+TbyL*vbme+k1mT*Ib_err;

    eatoB=Pll.we*Emf_get.ebeta_est;
    ebtoA=Pll.we*Emf_get.ealpha_est;

    eatoB=We_Filter.y*Emf_get.ebeta_est;
    ebtoA=We_Filter.y*Emf_get.ealpha_est;

//    Emf_get.ealpha_est = Emf_get.ealpha_est -eatoB*T+k2mT*Ia_err;
//    Emf_get.ebeta_est = Emf_get.ebeta_est + ebtoA*T+k2mT*Ib_err;

    ealpha_next = Emf_get.ealpha_est -eatoB*T+k2mT*Ia_err;
    ebeta_next = Emf_get.ebeta_est + ebtoA*T+k2mT*Ib_err;

    Emf_get.ialpha_est=ialpha_next;
    Emf_get.ibeta_est=ibeta_next;
    Emf_get.ealpha_est=ealpha_next;
    Emf_get.ebeta_est=ebeta_next;

    Pll_get_speed();
}



void STO_speed_Improve(void)
{
    float Ialpha_next=0;
    float Ibeta_next=0;
    float ealpha_next=0;
    float ebeta_next=0;

    float vame=0;
    float vbme=0;
    float eatoB=0;
    float ebtoA=0;
    float Ia_err=0;
    float Ib_err=0;
    float Ialpha_avr=0;
    float Ibeta_avr=0;
    float ealpha_avr=0;
    float ebeta_avr=0;

    float ialpha_next2=0;
    float ibeta_next2=0;
    float ealpha_next2=0;
    float ebeta_next2=0;


    //状态观测器
    Emf_get.Ualpha=Udq_to_Ualphabeta.Alpha;
    Emf_get.Ubeta=Udq_to_Ualphabeta.Beta;
    Emf_get.ialpha=Ialphabeta_to_Idq.Alpha;
    Emf_get.ibeta=Ialphabeta_to_Idq.Beta;


    //预报
    vame=Emf_get.Ualpha-Emf_get.ealpha_est;
    vbme=Emf_get.Ubeta-Emf_get.ebeta_est;

//    eatoB=Pll.we*Emf_get.ebeta_est;
//    ebtoA=Pll.we*Emf_get.ealpha_est;

    eatoB=We_Filter.y*Emf_get.ebeta_est;
    ebtoA=We_Filter.y*Emf_get.ealpha_est;


    Ialpha_next=Emf_get.ialpha_est-rTbyL*Emf_get.ialpha_est+TbyL*vame;
    Ibeta_next=Emf_get.ibeta_est-rTbyL*Emf_get.ibeta_est+TbyL*vbme;
    ealpha_next=Emf_get.ealpha_est -eatoB*T;
    ebeta_next=Emf_get.ebeta_est + ebtoA*T;

    //校正


    Ialpha_avr=0.5*(Ialpha_next+Emf_get.ialpha_est);
    Ibeta_avr=0.5*(Ibeta_next+Emf_get.ibeta_est);

    ealpha_avr=0.5*(ealpha_next+Emf_get.ealpha_est);
    ebeta_avr=0.5*(ebeta_next+Emf_get.ebeta_est);

//    eatoB=Pll.we*ebeta_avr;
//    ebtoA=Pll.we*ealpha_avr;

    eatoB=We_Filter.y*ebeta_avr;
    ebtoA=We_Filter.y*ealpha_avr;


    vame=Emf_get.Ualpha-ealpha_avr;
    vbme=Emf_get.Ubeta-ebeta_avr;

    Ia_err=Emf_get.ialpha_est-Emf_get.ialpha;
    Ib_err=Emf_get.ibeta_est-Emf_get.ibeta;


    ialpha_next2=Emf_get.ialpha_est-rTbyL*Ialpha_avr+TbyL*vame+k1mT*Ia_err;
    ibeta_next2=Emf_get.ibeta_est -rTbyL*Ibeta_avr+TbyL*vbme+k1mT*Ib_err;

    ealpha_next2 = Emf_get.ealpha_est -eatoB*T+k2mT*Ia_err;
    ebeta_next2 = Emf_get.ebeta_est + ebtoA*T+k2mT*Ib_err;

    Emf_get.ialpha_est=ialpha_next2;
    Emf_get.ibeta_est=ibeta_next2;
    Emf_get.ealpha_est=ealpha_next2;
    Emf_get.ebeta_est=ebeta_next2;


    Pll_get_speed();
}
