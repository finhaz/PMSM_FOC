/*
 * IIR_Filter.c
 *
 *  Created on: 2021Äê2ÔÂ20ÈÕ
 *      Author: sea
 */

#include "includes.h"

//ÂË²¨Æ÷
void FILTRATE_CALC(FILTRATE*p)
{
//  X_last1=X_last;
//  X_last=X;
//  X=X_in;
//  Y_last1=Y_last;
//  Y_last=Y;
//  Y=a1*X+a2*X_last+a3*X_last1-b1*Y_last-b2*Y_last1;
//  float tmp1,tmp2,tmp3,tmp4,tmp5;


    p->X_last1=p->X_last;
    p->X_last=p->X;
    p->X=p->X_in;
    p->Y_last1=p->Y_last;
    p->Y_last=p->Y;

    p->Y=p->a1*p->X+p->a2*p->X_last+p->a3*p->X_last1-p->b1*p->Y_last-p->b2*p->Y_last1;

}
