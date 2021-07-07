/*
 * ROGI.c
 *
 *  Created on: 2021年6月7日
 *      Author: lenovo
 */


#include "includes.h"
//结构体变量
ROGI_CONTROL ROGI_q_R=ROGI_CONTROL_DEFAULTS;
ROGI_CONTROL ROGI_d_R=ROGI_CONTROL_DEFAULTS;







//ROGI调节器抑谐波
void ROGI_d_CONTROL_Postive( ROGI_CONTROL *p)
{

   //   d轴ROGI  正序

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg1 / p->qKI * p->out_of_forward_integrate_q1;
    p->U1=(p->currentError - p->out_of_forward_integrate_d1 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 + p->out_of_j_feedback);
    p->out_of_forward_integrate_d1=p->out_of_forward_integrate_d1 + p->U2;
    p->qOut1= p->out_of_forward_integrate_d1;

    //输出限幅
    if( p->qOut1 > p->qOutMax)
    {  p->qOut1=p->qOutMax;}

    else if( p->qOut1 < p->qOutMin)
    {    p->qOut1=p->qOutMin;}

   //         PCIcontroller_d()
  //      out_of_j_feedback=wg/ki*out_of_forward_integrate_q;
  //      out_of_forward_integrate_d=out_of_forward_integrate_d+ki*Tk*((delt-out_of_forward_integrate_d/ki)*wc+out_of_j_feedback);
  //          PCI_out=out_of_forward_integrate_d;
  //          if( PCI_out> pciout_max)   % 输出限幅
  //              PCI_out = pciout_max;
  //        elseif ( PCI_out < pciout_min )
  //             PCI_out =pciout_min;
  //         end
}



void ROGI_d_CONTROL_Negative( ROGI_CONTROL *p)
{

   //   d轴ROGI  负序

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg2 / p->qKI * p->out_of_forward_integrate_q2;
    p->U1=(p->currentError - p->out_of_forward_integrate_d2 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 + p->out_of_j_feedback);
    p->out_of_forward_integrate_d2=p->out_of_forward_integrate_d2 + p->U2;
    p->qOut2= p->out_of_forward_integrate_d2;

    //输出限幅
    if( p->qOut2 > p->qOutMax)
    {  p->qOut2=p->qOutMax;}

    else if( p->qOut2 < p->qOutMin)
    {    p->qOut2=p->qOutMin;}

}




void ROGI_q_CONTROL_Postive( ROGI_CONTROL *p)
{

   //   q轴ROGI  正序

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg1 / p->qKI * p->out_of_forward_integrate_d1;
    p->U1=(p->currentError - p->out_of_forward_integrate_q1 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 - p->out_of_j_feedback);
    p->out_of_forward_integrate_q1=p->out_of_forward_integrate_q1 + p->U2;
    p->qOut1= p->out_of_forward_integrate_q1;

    //输出限幅
    if( p->qOut1 > p->qOutMax)
    {  p->qOut1=p->qOutMax;}

    else if( p->qOut1 < p->qOutMin)
    {    p->qOut1=p->qOutMin;}

   //         PCIcontroller_q()
   // Tk=1e-4;
   //   out_of_j_feedback=wg/ki*out_of_forward_integrate_a;
   // out_of_forward_integrate_b=out_of_forward_integrate_b+ki*Tk*((delt-out_of_forward_integrate_b/ki)*wc-out_of_j_feedback);
   //  % out_of_forward_integrate=out_of_forward_integrate_a+ki*(delt+out_of_j_feedback);
   //   PCI_out=out_of_forward_integrate_b;
   //   if( PCI_out> pciout_max)   % 输出限幅
   //       PCI_out = pciout_max;
  //  elseif ( PCI_out < pciout_min )
   //      PCI_out =pciout_min;


}

void ROGI_q_CONTROL_Negative( ROGI_CONTROL *p)
{

   //   q轴ROGI  正序

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg1 / p->qKI * p->out_of_forward_integrate_d2;
    p->U1=(p->currentError - p->out_of_forward_integrate_q2 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 - p->out_of_j_feedback);
    p->out_of_forward_integrate_q2=p->out_of_forward_integrate_q2 + p->U2;
    p->qOut2= p->out_of_forward_integrate_q2;

    //输出限幅
    if( p->qOut2 > p->qOutMax)
    {  p->qOut2=p->qOutMax;}

    else if( p->qOut2 < p->qOutMin)
    {    p->qOut2=p->qOutMin;}


}

//电流环
void current_loop_rogi(void)
{
    ///PI调节器
    park_calc(&Ialphabeta_to_Idq); //得到反馈量id、iq
    //给定q轴调节
    A_Iq_R.qKp=kp_iq;
    A_Iq_R.qKi=ki_iq;
    A_Iq_R.qOutMax=max_current;
    A_Iq_R.qOutMin=min_current;
    //给定d轴调节
    A_Id_R.qKp=kp_id;
    A_Id_R.qKi=ki_id;
    A_Id_R.qOutMax=max_current;
    A_Id_R.qOutMin=min_current;

       //q轴调节
       A_Iq_R.qInMeas=Ialphabeta_to_Idq.Qs;//q反馈电流,其它pi输出参数、控制参数已给
       PI_CONTROL_CALC(&A_Iq_R);//输出q轴调制电压Uq
       //d轴调节
       A_Id_R.qInMeas=Ialphabeta_to_Idq.Ds;//d反馈电流,其它pi输出参数、控制参数已给
       PI_CONTROL_CALC(&A_Id_R);//输出d轴调制电压Ud



        //加入ROGI控制/////
      if (sign_ROGI==1)          //sign_ROGI=1,开启ROGI；=0，关闭ROGI
       {

         //谐波频率
          ROGI_d_R.qWg1 = 6*jiaosudu1_asr;    //正序
          ROGI_d_R.qWg2 =-6*jiaosudu1_asr;    //负序
          ROGI_q_R.qWg1 = 6*jiaosudu1_asr;    //正序
          ROGI_q_R.qWg2 =-6*jiaosudu1_asr;    //负序

          ROGI_q_R.qTk = 0.0001;
          ROGI_d_R.qTk = 0.0001;
         //给定q轴调节
         ROGI_q_R.qKI=KI_current;
         ROGI_q_R.qWC=WC_current;
         ROGI_q_R.qOutMax=max_current1;
         ROGI_q_R.qOutMin=min_current1;
         //给定d轴调节
         ROGI_d_R.qKI=KI_current;
         ROGI_d_R.qWC=WC_current;
         ROGI_d_R.qOutMax=max_current1;
         ROGI_d_R.qOutMin=min_current1;
         //q轴调节
         ROGI_q_R.qInRef=A_Iq_R.qInRef;
         ROGI_q_R.qInMeas=Ialphabeta_to_Idq.Qs;//q反馈电流,其它pi输出参数、控制参数已给
         ROGI_q_CONTROL_Postive(&ROGI_q_R);
         ROGI_q_CONTROL_Negative(&ROGI_q_R);//输出q轴调制电压
         //d轴调节
         ROGI_d_R.qInRef=A_Id_R.qInRef;
         ROGI_d_R.qInMeas=Ialphabeta_to_Idq.Ds;//d反馈电流,其它pi输出参数、控制参数已给
         ROGI_d_CONTROL_Postive(&ROGI_d_R);
         ROGI_d_CONTROL_Negative(&ROGI_d_R);//输出d轴调制电压

         float qOut_com1;
         float qOut_com2;
         float dOut_com1;
         float dOut_com2;
         qOut_com1=ROGI_q_R.qOut1*cos(theta_c)-ROGI_d_R.qOut1*sin(theta_c);
         qOut_com2=ROGI_q_R.qOut2*cos(theta_c)+ROGI_d_R.qOut2*sin(theta_c);

         dOut_com1=ROGI_d_R.qOut1*cos(theta_c)+ROGI_q_R.qOut1*sin(theta_c);
         dOut_com2=ROGI_d_R.qOut2*cos(theta_c)-ROGI_q_R.qOut2*sin(theta_c);

         //PCI_qout_com1=qoofi1*cos(theta_c)-doofi1*sin(theta_c); %%%正序下cosφ+jsinφ
         //PCI_qout_com2=qoofi2*cos(theta_c)+doofi2*sin(theta_c); %%%负序下cosφ-jsinφ
        // PCI_dout_com1=doofi1*cos(theta_c)+qoofi1*sin(theta_c);
         //PCI_dout_com2=doofi2*cos(theta_c)-qoofi2*sin(theta_c);

         //坐标变换得到调制电压Uα、Uβ
         Udq_to_Ualphabeta.Ds=dOut_com1+dOut_com2+A_Id_R.qOut;
         Udq_to_Ualphabeta.Qs=qOut_com1+qOut_com2+A_Iq_R.qOut;
       }

      else
      {
           Udq_to_Ualphabeta.Ds=A_Id_R.qOut;  //PI输出
           Udq_to_Ualphabeta.Qs=A_Iq_R.qOut;
      }

       ipark_calc(&Udq_to_Ualphabeta);//输出Uα、Uβ

 }

//ROGI调节器清零
void ROGIZero()
{

    //ROGI_q_R
    ROGI_q_R.qInMeas=0;
    ROGI_q_R.qInRef=0;

    ROGI_q_R.out_of_forward_integrate_q1=0;
    ROGI_q_R.out_of_forward_integrate_q2=0;

    //ROGI_d_R
    ROGI_d_R.qInMeas=0;
    ROGI_d_R.qInRef=0;

    ROGI_d_R.out_of_forward_integrate_d1=0;
    ROGI_d_R.out_of_forward_integrate_d2=0;
}
