
/*
 *zhs_SVPWM_2L.c
 *
 *  Created on: 2017-1-6
 *      Author: naiyangui
 */


#include "includes.h"



void svpwm_exe(void)
{
     //SVPWM
      Svpwm.Ualpha=Udq_to_Ualphabeta.Alpha;
      Svpwm.Ubeta= Udq_to_Ualphabeta.Beta;
      Svpwm.Vdc=Adcget.Vdc;
      //Svpwm.Tp=1;
      Svpwm.Tp=2*Prd;
      svpwm_2l_calc(&Svpwm);

//      //归一化需要乘以pwm周期：2*TBPRD*TBCLK
//      Svpwm.Tcmpa=2*Prd*Svpwm.Tcmpa;
//      Svpwm.Tcmpb=2*Prd*Svpwm.Tcmpb;
//      Svpwm.Tcmpc=2*Prd*Svpwm.Tcmpc;

}

//======== Definations for functions ===========================
//**********************************
/*
  @ Description:
  @ Param
  @ Return
*/
//**********************************
void svpwm_2l_calc(SVPWM_2L *p)
{
	float temp;
	float X,Y,Z, t1,t2;
	Uint16 A,B,C,N,Sector;
	float Ta, Tb, Tc;

	float h;
//	p->Tp=1;//将整个调制周期归一化处理
	h=1.73205081*p->Tp/p->Vdc;//Vdc=311,T=1.0



//	X=_IQmpy(h,p->Ubeta);
//	Y=_IQmpy(h,(_IQmpy(_IQ(0.8660254),p->Ualpha)+_IQmpy(_IQ(0.5),p->Ubeta)));
//	Z=_IQmpy(h,(_IQmpy(_IQ(-0.8660254),p->Ualpha)+_IQmpy(_IQ(0.5),p->Ubeta)));

    X=h*p->Ubeta;
    Y=h*(0.8660254*p->Ualpha+0.5*p->Ubeta);
    Z=h*(-0.8660254*p->Ualpha+0.5*p->Ubeta);



//	if(p->Ubeta>0)
//		{A=1;}
//	else
//		{A=0;}
//
//	if( (_IQmpy(_IQ(1.73205081),p->Ualpha)- p->Ubeta)>0 )
//		{B=1;}
//	else
//		{B=0;}
//
//	if((_IQmpy(_IQ(-1.73205081),p->Ualpha)- p->Ubeta)>0)
//		{C=1;}
//	else
//		{C=0;}

    if(p->Ubeta>0)
        {A=1;}
    else
        {A=0;}

    if( (1.73205081*p->Ualpha- p->Ubeta)>0 )
        {B=1;}
    else
        {B=0;}

    if((-1.73205081*p->Ualpha- p->Ubeta)>0)
        {C=1;}
    else
        {C=0;}


	N=A+2*B+4*C;

	switch(N)
	{
		case 1:{Sector=2;break;}
		case 2:{Sector=6;break;}
		case 3:{Sector=1;break;}
		case 4:{Sector=4;break;}
		case 5:{Sector=3;break;}
		case 6:{Sector=5;break;}
		default:{;}
	}

	switch(Sector)
	{
		case 1: {t1=-Z; t2= X;break;}
		case 2: {t1= Z; t2= Y;break;}
		case 3: {t1= X; t2=-Y;break;}
		case 4: {t1=-X; t2= Z;break;}
		case 5: {t1=-Y; t2=-Z;break;}
		case 6: {t1= Y; t2=-X;break;}
		default:{;}
	}

	if((t1+t2)>p->Tp)//对过调制情况进行调整
	{

		temp=t1+t2;
		t1=t1*p->Tp/temp;
		t2=t2*p->Tp/temp;

//		temp=t1+t2;
//		t1=_IQdiv(_IQmpy(t1,p->Tp),temp);
//		t2=_IQdiv(_IQmpy(t2,p->Tp),temp);

	}


	Ta=(p->Tp-t1-t2)/4;//作用时间分配
	Tb=Ta+t1/2;
	Tc=Tb+t2/2;

//	Ta=_IQdiv((p->Tp-t1-t2),_IQ(4));
//	Tb=Ta+_IQdiv(t1,_IQ(2));
//	Tc=Tb+_IQdiv(t2,_IQ(2));

	switch(Sector)
	{
		case 1: {p->Tcmpa=Ta; p->Tcmpb=Tb; p->Tcmpc=Tc; break;}
		case 2: {p->Tcmpa=Tb; p->Tcmpb=Ta; p->Tcmpc=Tc; break;}
		case 3: {p->Tcmpa=Tc; p->Tcmpb=Ta; p->Tcmpc=Tb; break;}
		case 4: {p->Tcmpa=Tc; p->Tcmpb=Tb; p->Tcmpc=Ta; break;}
		case 5: {p->Tcmpa=Tb; p->Tcmpb=Tc; p->Tcmpc=Ta; break;}
		case 6: {p->Tcmpa=Ta; p->Tcmpb=Tc; p->Tcmpc=Tb; break;}
		default:{;}
	}
}

//==============================================================
//End of file.
//==============================================================



