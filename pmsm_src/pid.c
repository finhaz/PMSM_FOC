/*
 * pid.c
 *
 *  Created on: 2021��2��14��
 *      Author: sea
 */
#include "includes.h"



//pi������
void PI_CONTROL_CALC( PI_CONTROL*p)
{
   //�������
   //Error  = Reference - Measurement
    p->currentError =p->qInRef-p->qInMeas;// pParm->qInRef - pParm->qInMeas;
    //�������
    //U = Ki * Err
    //U =  _IQmpy(currentError ,p->qKi) ;
    p->U = p->currentError  *p->qKi;
    //p->U=_IQmpy(p->currentError ,p->qKi);

    //Sum = Sum + U = Sum + Ki * Err - Kc * Exc
    p->qdSum=p->qdSum+p->U;// sum = sum + U;�ۼӻ���ֵ
   //�����޷�
    if(p->qdSum>p->qOutMax)
       p->qdSum=p->qOutMax;
    if(p->qdSum<p->qOutMin)
       p->qdSum=p->qOutMin;
      //�������
    //U  = Sum + Kp * Error
    //U =   _IQmpy(currentError ,p->qKp) ;
    p->U = p->currentError *p->qKp;
    //p->U=_IQmpy( p->currentError,p->qKp);
    p->U = p->qdSum+p->U; // U = U + sum;
    //����޷�
    if(  p->U >p->qOutMax)
        p->qOut=p->qOutMax;

    else if( p->U <p->qOutMin)
        p->qOut=p->qOutMin;
    else
        p->qOut=p->U;
}
