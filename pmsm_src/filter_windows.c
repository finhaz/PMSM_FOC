/*
 * filter_windows.c
 *
 *  Created on: 2021Äê2ÔÂ28ÈÕ
 *      Author: sea
 */
#include "includes.h"

//------------------------------------------------»¬´°ÂË²¨
void HUACHUANG_CALC(HUACHUANG*p)
{
    int count=0;
    float sum=0;
    p->value_buf[p->index++]=p->value_in;
    if (p->index==50)
        {
            p->index=0;
        }
    for (count=0;count<50;count++)
        sum+=p->value_buf[count];
        p->value_out=sum/50;
}
