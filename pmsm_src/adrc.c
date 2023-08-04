/*
 * adrc_speed.c
 *
 *  Created on: 2021年7月8日
 *      Author: sea
 */

#include "includes.h"


int sign(float num)
{
    if(num>0)
        return 1;
    else
        return -1;
}

float fal(float e0,float a0,float delta)
{
    float y;
    int sign_e0=1;

    sign_e0=sign(e0);

    if(fabsf(e0)>delta)
        y=powf((fabsf(e0)),a0)*sign_e0;
    else
        y=e0/(powf(delta,(1-a0)));
    return y;
}


float fhan(float x1,float x2,float r,float h)
{
    float d;
    float y;
    float u;
    float a;
    float a0;
    float d0;

    d=r*h;
    d0=d*h;
    y=x1+h*x2;

    a0=sqrtf(d*d+8*r*fabsf(y));

    if(fabsf(y)<=d0)
        a=x2+y/h;
    else
        a=x2+0.5*(a0-d)*sign(y);

    if(fabsf(a)<=d)
        u=-r*a/d;
    else
        u=-r*sign(a);

    y=u;
    return y;
}


void ADRC_REGULATOR_CALC(ADRC_Regulator *p)
{
    float e;
    float e1;
    float beta1;
    float beta2;
    float eso_a;

    float u0;
    float ui=0;
    float u_c;

    float v1;

    float Te_gmax;
    float Te_gmin;

    float Ui_gmax;
    float Ui_gmin;

    //Td
    v1=p->qInRef;//这里由前面的斜坡函数作为ADRC的


    //ESO
    u_c=p->qOut;//记录上一次的给定
    beta1=2*p->w0;
    beta2=p->w0*p->w0;
    e=p->z1-p->qInMeas;
    p->z1=p->z1+p->Tk_s*(p->z2-beta1*e+p->b*u_c);
    p->z2=p->z2+p->Tk_s*(-beta2*e);

    eso_a=-p->z2;//代表了状态变量变化率，可以用于计算扰动,用于补偿扰动

    Te_gmax=p->b*p->qOutMax;
    Te_gmin=p->b*p->qOutMin;

    if (eso_a>=Te_gmax)
        eso_a=Te_gmax;
    else if (eso_a<=Te_gmin)
        eso_a=Te_gmin;


    if(p->enable==1)
    {


        e1=v1-p->z1;//相当于pid的delt_speed
        //e1=v1-p->qInMeas;//相当于pid的delt_speed
    }
    else
    {
        e1=v1-p->qInMeas;//相当于pid的delt_speed
        eso_a=0;
    }




    //非线性反馈NLSEF

    u0=p->qKp*fal(e1,p->alpha_kp,p->filter_kp);

    if(p->qKi!=0)
    {
        p->qdSum=p->qdSum+e1;//累计误差

        ui=p->qKi*fal(p->qdSum,p->alpha_ki,p->filter_ki);

        //ui=p->qKi*p->qdSum;

        Ui_gmax=p->qOutMax*p->b;
        Ui_gmin=p->qOutMin*p->b;

        if (ui>=Ui_gmax)
            ui=Ui_gmax;
        else if(ui<=Ui_gmin)
            ui=Ui_gmin;
    }


    u_c=u0+ui+eso_a;

    u_c=u_c/p->b;



    if (u_c>=p->qOutMax)
        u_c=p->qOutMax;
    else if(u_c<=p->qOutMin)
        u_c=p->qOutMin;

    //ADRC输出
    p->qOut=u_c;


}





