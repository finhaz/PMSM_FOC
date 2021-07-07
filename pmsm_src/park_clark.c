/*
 * park_clark.c
 *
 *  Created on: 2021年2月14日
 *      Author: sea
 */
#include "includes.h"


/*////////////////////////////////////////////////////////////////////////////////////////////////////////
 * ----------------------坐标变换部分------------------------------------------------------------------------
 */
//clark
void clark_calc(CLARKE_PARK *p)
{
    p->Alpha=p->As;
    p->Beta=(p->Bs-p->Cs)*ONEbySQRT3;
}
//iclark
void iclark_calc(CLARKE_PARK *p)
{

    p->As = p->Alpha ;
    p->Bs = - p->Alpha *ONEbyHALF  + p->Beta*SQRT3byTWO;
    p->Cs = - p->Alpha * ONEbyHALF  - p->Beta *SQRT3byTWO;
}
//park
void park_calc(CLARKE_PARK *p)
{

     p->Sine = sin(p->Angle);
     p->Cosine = cos(p->Angle);


     p->Ds =p->Alpha*p->Cosine + p->Beta*p->Sine;
     p->Qs =p->Beta*p->Cosine - p->Alpha*p->Sine;
}
//ipark
void ipark_calc(CLARKE_PARK *p)
{

    p->Sine = sin(p->Angle);
    p->Cosine = cos(p->Angle);

    p->Alpha =p->Ds*p->Cosine - p->Qs*p->Sine;
    p->Beta  = p->Qs*p->Cosine + p->Ds*p->Sine;
}
