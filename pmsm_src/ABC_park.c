/*
 * Positve_3to2.c
 *
 *  Created on: 2021年2月20日
 *      Author: sea
 */

#include "includes.h"

//正序abc to dq
void abc_dq0p(ABC_PARK *p)
{

//   p->Sine = _IQsin(p->Angle);
//   p->Cosine = _IQcos(p->Angle);
//
//
//   p->Ds =_IQmpy(p->Alpha,p->Cosine) + _IQmpy(p->Beta,p->Sine);
//   p->Qs =_IQmpy(p->Beta,p->Cosine) - _IQmpy(p->Alpha,p->Sine);

     p->Sine0 = sin(p->Angle);
     p->Sine1 = sin(p->Angle-TWObyTHREE*PI);
     p->Sine2 = sin(p->Angle+TWObyTHREE*PI);
     p->Cosine0 = cos(p->Angle);
     p->Cosine1 = cos(p->Angle-TWObyTHREE*PI);
     p->Cosine2 = cos(p->Angle+TWObyTHREE*PI);

     p->Ds =TWObyTHREE*(p->As*p->Cosine0+p->Bs*p->Cosine1+p->Cs*p->Cosine2);
     p->Qs =TWObyTHREE*(p->As*p->Sine0+p->Bs*p->Sine1+p->Cs*p->Sine2);
     p->V0 =ONEbyTHREE*(p->As+p->Bs+p->Cs);

}
//正序dq to abc
void iabc_dq0p(ABC_PARK *p)
{

//   p->Sine = _IQsin(p->Angle);
//   p->Cosine = _IQcos(p->Angle);
//
//
//   p->Ds =_IQmpy(p->Alpha,p->Cosine) + _IQmpy(p->Beta,p->Sine);
//   p->Qs =_IQmpy(p->Beta,p->Cosine) - _IQmpy(p->Alpha,p->Sine);

     p->Sine0 = sin(p->Angle);
     p->Sine1 = sin(p->Angle-TWObyTHREE*PI);
     p->Sine2 = sin(p->Angle+TWObyTHREE*PI);
     p->Cosine0 = cos(p->Angle);
     p->Cosine1 = cos(p->Angle-TWObyTHREE*PI);
     p->Cosine2 = cos(p->Angle+TWObyTHREE*PI);

     p->As =p->Ds*p->Cosine0+p->Qs*p->Sine0+p->V0;
     p->Bs =p->Ds*p->Cosine1+p->Qs*p->Sine1+p->V0;
     p->Cs =p->Ds*p->Cosine2+p->Qs*p->Sine2+p->V0;

}
//负序abc to dq
void abc_dq0n(ABC_PARK *p)
{

//   p->Sine = _IQsin(p->Angle);
//   p->Cosine = _IQcos(p->Angle);
//
//
//   p->Ds =_IQmpy(p->Alpha,p->Cosine) + _IQmpy(p->Beta,p->Sine);
//   p->Qs =_IQmpy(p->Beta,p->Cosine) - _IQmpy(p->Alpha,p->Sine);

     p->Sine0 = sin(p->Angle);
     p->Sine1 = sin(p->Angle-TWObyTHREE*PI);
     p->Sine2 = sin(p->Angle+TWObyTHREE*PI);
     p->Cosine0 = cos(p->Angle);
     p->Cosine1 = cos(p->Angle-TWObyTHREE*PI);
     p->Cosine2 = cos(p->Angle+TWObyTHREE*PI);

     p->Ds =TWObyTHREE*(p->As*p->Cosine0+p->Bs*p->Cosine1+p->Cs*p->Cosine2);
     p->Qs =-TWObyTHREE*(p->As*p->Sine0+p->Bs*p->Sine1+p->Cs*p->Sine2);
     p->V0 =ONEbyTHREE*(p->As+p->Bs+p->Cs);

}
//负序dq to abc
void iabc_dq0n(ABC_PARK *p)
{

//   p->Sine = _IQsin(p->Angle);
//   p->Cosine = _IQcos(p->Angle);
//
//
//   p->Ds =_IQmpy(p->Alpha,p->Cosine) + _IQmpy(p->Beta,p->Sine);
//   p->Qs =_IQmpy(p->Beta,p->Cosine) - _IQmpy(p->Alpha,p->Sine);

     p->Sine0 = sin(p->Angle);
     p->Sine1 = sin(p->Angle-TWObyTHREE*PI);
     p->Sine2 = sin(p->Angle+TWObyTHREE*PI);
     p->Cosine0 = cos(p->Angle);
     p->Cosine1 = cos(p->Angle-TWObyTHREE*PI);
     p->Cosine2 = cos(p->Angle+TWObyTHREE*PI);

     p->As =p->Ds*p->Cosine0-p->Qs*p->Sine0+p->V0;
     p->Bs =p->Ds*p->Cosine1-p->Qs*p->Sine1+p->V0;
     p->Cs =p->Ds*p->Cosine2-p->Qs*p->Sine2+p->V0;

}
