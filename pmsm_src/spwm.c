/*
 * spwm.c
 *
 *  Created on: 2021年2月15日
 *      Author: sea
 */
#include "includes.h"

//SPWM调制
void spwm_exe(void)
{
    float udc_half;
    float Udc_M;

    iclark_calc(&Udq_to_Ualphabeta);

    Spwm.Ua=Udq_to_Ualphabeta.As;
    Spwm.Ub=Udq_to_Ualphabeta.Bs;
    Spwm.Uc=Udq_to_Ualphabeta.Cs;
    Spwm.Module=M;
    Spwm.Udc=Adcget.Vdc;

    udc_half=0.5*Spwm.Udc;
    Udc_M=Spwm.Module*udc_half;

    //ABC限幅
    if(Spwm.Ua>Udc_M)
    {
        Spwm.Ua=Udc_M;
    }
    if(Spwm.Ua<-Udc_M)
    {
        Spwm.Ua=-Udc_M;
    }

    if(Spwm.Ub>Udc_M)
    {
        Spwm.Ub=Udc_M;
    }
    if(Spwm.Ub<-Udc_M)
    {
        Spwm.Ub=-Udc_M;
    }

    if(Spwm.Uc>Udc_M)
    {
        Spwm.Uc=Udc_M;
    }
    if(Spwm.Uc<-Udc_M)
    {
        Spwm.Uc=-Udc_M;
    }

    Spwm.m_sin_a=Spwm.Ua/udc_half;
    Spwm.m_sin_b=Spwm.Ub/udc_half;
    Spwm.m_sin_c=Spwm.Uc/udc_half;

    Spwm.Tcmpa=Prd*0.5*(1-Spwm.m_sin_a);
    Spwm.Tcmpb=Prd*0.5*(1-Spwm.m_sin_b);
    Spwm.Tcmpc=Prd*0.5*(1-Spwm.m_sin_c);
}
