#include "includes.h"


typedef struct{
    float id_mars;//上一次/估算alpha轴电流
    float iq_mars;//上一次/估算beta轴电流
    float theta_mars;
    float wr_mars;
    float Kp;
    float Ki;
    float W_I;
}MRAS_Cal_GET;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define MRAS_Cal_DEFAULTS {0,0,0,0,1,0.1,0}

MRAS_Cal_GET Mras_ob=MRAS_Cal_DEFAULTS;



//MARS
void MARS_speed(void)
{

    CLARKE_PARK Imras=CLARKE_PARK_DEFAULTS;
    CLARKE_PARK Umras=CLARKE_PARK_DEFAULTS;
    float delt_W;
    float ud_s;
    float uq_s;
    float id_s;
    float iq_s;
    float we_c;


    Umras.Alpha=Udq_to_Ualphabeta.Alpha;
    Umras.Beta=Udq_to_Ualphabeta.Beta;
    Imras.Alpha=Ialphabeta_to_Idq.Alpha;
    Imras.Beta=Ialphabeta_to_Idq.Beta;
    we_c=Mras_ob.wr_mars;
//    we_c=We_Filter.y;

    //参考模型
    Umras.Angle=Mras_ob.theta_mars;
    park_calc(&Umras);
    ud_s=Umras.Ds+rs*fluxm/Ld;
    uq_s=Umras.Qs;

    Imras.Angle=Mras_ob.theta_mars;
    park_calc(&Imras);
    id_s=Imras.Ds+fluxm/Ld;
    iq_s=Imras.Qs;

    //可调模型
    Mras_ob.id_mars=Mras_ob.id_mars+T*(-rs*Mras_ob.id_mars/Ld+we_c*Mras_ob.iq_mars* Lq/Ld+ud_s/Ld);
    Mras_ob.iq_mars=Mras_ob.iq_mars+T*(-rs*Mras_ob.iq_mars/Lq-we_c*Mras_ob.id_mars* Ld/Lq+uq_s/Lq);


    //自适应律
    delt_W=(((id_s-Mras_ob.id_mars)*Mras_ob.iq_mars*Lq/Ld))-(((iq_s-Mras_ob.iq_mars)*Mras_ob.id_mars*Ld/Lq));

    //delt_W=((id_s-Mras_ob.id_mars)*Mras_ob.iq_mars)-((iq_s-Mras_ob.iq_mars)*Mras_ob.id_mars);

    Mras_ob.W_I= Mras_ob.W_I+Mras_ob.Ki*delt_W;
    Mras_ob.wr_mars=Mras_ob.Kp*delt_W+Mras_ob.W_I;

    Mras_ob.theta_mars=Mras_ob.theta_mars+Mras_ob.wr_mars*T;
    if(Mras_ob.theta_mars>=twopi)
    {
        Mras_ob.theta_mars=Mras_ob.theta_mars-twopi;
    }

    We_Filter.x_in=Mras_ob.wr_mars;
    Filter_AVR_CALC(&We_Filter);
}


void MARS_reset(void)
{
    Mras_ob.id_mars=0;
    Mras_ob.iq_mars=0;
    Mras_ob.theta_mars=0;
    Mras_ob.wr_mars=0;
    Mras_ob.W_I=0;
}
