/*
 * filter_AVR.c
 *
 *  Created on: 2021年4月14日
 *      Author: sea
 */

#include "includes.h"

//均值滤波
void Filter_AVR_CALC(Filter_AVR_GET*p)
{
    p->x_sum+=p->x_in;
    p->count++;
    if(p->count==p->n_win)
    {
        p->y=p->x_sum/p->n_win;
        p->count=0;
        p->x_sum=0;
    }
}
