/*
 * pid.c
 *
 *  Created on: 2021年2月14日
 *      Author: sea
 */
#include "includes.h"



//pi调节器
void PI_CONTROL_CALC( PI_CONTROL*p)
{
   //计算误差
   //Error  = Reference - Measurement
    p->currentError =p->qInRef-p->qInMeas;// pParm->qInRef - pParm->qInMeas;
    //计算积分
    //U = Ki * Err
    //U =  _IQmpy(currentError ,p->qKi) ;
    p->U = p->currentError  *p->qKi;
    //p->U=_IQmpy(p->currentError ,p->qKi);

    //Sum = Sum + U = Sum + Ki * Err - Kc * Exc
    p->qdSum=p->qdSum+p->U;// sum = sum + U;累加积分值
   //积分限幅
    if(p->qdSum>p->qOutMax)
       p->qdSum=p->qOutMax;
    if(p->qdSum<p->qOutMin)
       p->qdSum=p->qOutMin;
      //计算比例
    //U  = Sum + Kp * Error
    //U =   _IQmpy(currentError ,p->qKp) ;
    p->U = p->currentError *p->qKp;
    //p->U=_IQmpy( p->currentError,p->qKp);
    p->U = p->qdSum+p->U; // U = U + sum;
    //输出限幅
    if(  p->U >p->qOutMax)
        p->qOut=p->qOutMax;

    else if( p->U <p->qOutMin)
        p->qOut=p->qOutMin;
    else
        p->qOut=p->U;
}
