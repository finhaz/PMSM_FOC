/*
 * SOGI.c
 *
 *  Created on: 2021Äê2ÔÂ28ÈÕ
 *      Author: sea
 */

#include "includes.h"

void SOGI_CALC(SOGI*p)
{
////ahphav
//  e_n1=(Uo_Alpha-ahphav)*sqrt(2)-ahphaqv;
//  e_n2=e_n1*w;
//  ahphav=ahphav+e_n2*1e-6*100;
////ahphaqv
//  e_n3=w*ahphav;
//  ahphaqv=ahphaqv+e_n3*1e-6*100;
//ahphav
    p->e_n1=(p->Uo_in-p->v)*SQRT2-p->qv;
    p->e_n2=p->e_n1*p->w_in;
    p->v=p->v+p->e_n2*T;
//ahphaqv
    p->e_n3=p->w_in*p->v;
    p->qv=p->qv+p->e_n3*T;

}
