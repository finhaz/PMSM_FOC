/*
 * ROGI.c
 *
 *  Created on: 2021��6��7��
 *      Author: lenovo
 */


#include "includes.h"
//�ṹ�����
ROGI_CONTROL ROGI_q_R=ROGI_CONTROL_DEFAULTS;
ROGI_CONTROL ROGI_d_R=ROGI_CONTROL_DEFAULTS;







//ROGI��������г��
void ROGI_d_CONTROL_Postive( ROGI_CONTROL *p)
{

   //   d��ROGI  ����

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg1 / p->qKI * p->out_of_forward_integrate_q1;
    p->U1=(p->currentError - p->out_of_forward_integrate_d1 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 + p->out_of_j_feedback);
    p->out_of_forward_integrate_d1=p->out_of_forward_integrate_d1 + p->U2;
    p->qOut1= p->out_of_forward_integrate_d1;

    //����޷�
    if( p->qOut1 > p->qOutMax)
    {  p->qOut1=p->qOutMax;}

    else if( p->qOut1 < p->qOutMin)
    {    p->qOut1=p->qOutMin;}

   //         PCIcontroller_d()
  //      out_of_j_feedback=wg/ki*out_of_forward_integrate_q;
  //      out_of_forward_integrate_d=out_of_forward_integrate_d+ki*Tk*((delt-out_of_forward_integrate_d/ki)*wc+out_of_j_feedback);
  //          PCI_out=out_of_forward_integrate_d;
  //          if( PCI_out> pciout_max)   % ����޷�
  //              PCI_out = pciout_max;
  //        elseif ( PCI_out < pciout_min )
  //             PCI_out =pciout_min;
  //         end
}



void ROGI_d_CONTROL_Negative( ROGI_CONTROL *p)
{

   //   d��ROGI  ����

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg2 / p->qKI * p->out_of_forward_integrate_q2;
    p->U1=(p->currentError - p->out_of_forward_integrate_d2 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 + p->out_of_j_feedback);
    p->out_of_forward_integrate_d2=p->out_of_forward_integrate_d2 + p->U2;
    p->qOut2= p->out_of_forward_integrate_d2;

    //����޷�
    if( p->qOut2 > p->qOutMax)
    {  p->qOut2=p->qOutMax;}

    else if( p->qOut2 < p->qOutMin)
    {    p->qOut2=p->qOutMin;}

}




void ROGI_q_CONTROL_Postive( ROGI_CONTROL *p)
{

   //   q��ROGI  ����

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg1 / p->qKI * p->out_of_forward_integrate_d1;
    p->U1=(p->currentError - p->out_of_forward_integrate_q1 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 - p->out_of_j_feedback);
    p->out_of_forward_integrate_q1=p->out_of_forward_integrate_q1 + p->U2;
    p->qOut1= p->out_of_forward_integrate_q1;

    //����޷�
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
   //   if( PCI_out> pciout_max)   % ����޷�
   //       PCI_out = pciout_max;
  //  elseif ( PCI_out < pciout_min )
   //      PCI_out =pciout_min;


}

void ROGI_q_CONTROL_Negative( ROGI_CONTROL *p)
{

   //   q��ROGI  ����

    p->currentError =p->qInRef - p->qInMeas;
    p->out_of_j_feedback=p->qWg1 / p->qKI * p->out_of_forward_integrate_d2;
    p->U1=(p->currentError - p->out_of_forward_integrate_q2 / p->qKI) * p->qWC;
    p->U2=p->qKI * p->qTk * ( p->U1 - p->out_of_j_feedback);
    p->out_of_forward_integrate_q2=p->out_of_forward_integrate_q2 + p->U2;
    p->qOut2= p->out_of_forward_integrate_q2;

    //����޷�
    if( p->qOut2 > p->qOutMax)
    {  p->qOut2=p->qOutMax;}

    else if( p->qOut2 < p->qOutMin)
    {    p->qOut2=p->qOutMin;}


}

//������
void current_loop_rogi(void)
{
    ///PI������
    park_calc(&Ialphabeta_to_Idq); //�õ�������id��iq
    //����q�����
    A_Iq_R.qKp=kp_iq;
    A_Iq_R.qKi=ki_iq;
    A_Iq_R.qOutMax=max_current;
    A_Iq_R.qOutMin=min_current;
    //����d�����
    A_Id_R.qKp=kp_id;
    A_Id_R.qKi=ki_id;
    A_Id_R.qOutMax=max_current;
    A_Id_R.qOutMin=min_current;

       //q�����
       A_Iq_R.qInMeas=Ialphabeta_to_Idq.Qs;//q��������,����pi������������Ʋ����Ѹ�
       PI_CONTROL_CALC(&A_Iq_R);//���q����Ƶ�ѹUq
       //d�����
       A_Id_R.qInMeas=Ialphabeta_to_Idq.Ds;//d��������,����pi������������Ʋ����Ѹ�
       PI_CONTROL_CALC(&A_Id_R);//���d����Ƶ�ѹUd



        //����ROGI����/////
      if (sign_ROGI==1)          //sign_ROGI=1,����ROGI��=0���ر�ROGI
       {

         //г��Ƶ��
          ROGI_d_R.qWg1 = 6*jiaosudu1_asr;    //����
          ROGI_d_R.qWg2 =-6*jiaosudu1_asr;    //����
          ROGI_q_R.qWg1 = 6*jiaosudu1_asr;    //����
          ROGI_q_R.qWg2 =-6*jiaosudu1_asr;    //����

          ROGI_q_R.qTk = 0.0001;
          ROGI_d_R.qTk = 0.0001;
         //����q�����
         ROGI_q_R.qKI=KI_current;
         ROGI_q_R.qWC=WC_current;
         ROGI_q_R.qOutMax=max_current1;
         ROGI_q_R.qOutMin=min_current1;
         //����d�����
         ROGI_d_R.qKI=KI_current;
         ROGI_d_R.qWC=WC_current;
         ROGI_d_R.qOutMax=max_current1;
         ROGI_d_R.qOutMin=min_current1;
         //q�����
         ROGI_q_R.qInRef=A_Iq_R.qInRef;
         ROGI_q_R.qInMeas=Ialphabeta_to_Idq.Qs;//q��������,����pi������������Ʋ����Ѹ�
         ROGI_q_CONTROL_Postive(&ROGI_q_R);
         ROGI_q_CONTROL_Negative(&ROGI_q_R);//���q����Ƶ�ѹ
         //d�����
         ROGI_d_R.qInRef=A_Id_R.qInRef;
         ROGI_d_R.qInMeas=Ialphabeta_to_Idq.Ds;//d��������,����pi������������Ʋ����Ѹ�
         ROGI_d_CONTROL_Postive(&ROGI_d_R);
         ROGI_d_CONTROL_Negative(&ROGI_d_R);//���d����Ƶ�ѹ

         float qOut_com1;
         float qOut_com2;
         float dOut_com1;
         float dOut_com2;
         qOut_com1=ROGI_q_R.qOut1*cos(theta_c)-ROGI_d_R.qOut1*sin(theta_c);
         qOut_com2=ROGI_q_R.qOut2*cos(theta_c)+ROGI_d_R.qOut2*sin(theta_c);

         dOut_com1=ROGI_d_R.qOut1*cos(theta_c)+ROGI_q_R.qOut1*sin(theta_c);
         dOut_com2=ROGI_d_R.qOut2*cos(theta_c)-ROGI_q_R.qOut2*sin(theta_c);

         //PCI_qout_com1=qoofi1*cos(theta_c)-doofi1*sin(theta_c); %%%������cos��+jsin��
         //PCI_qout_com2=qoofi2*cos(theta_c)+doofi2*sin(theta_c); %%%������cos��-jsin��
        // PCI_dout_com1=doofi1*cos(theta_c)+qoofi1*sin(theta_c);
         //PCI_dout_com2=doofi2*cos(theta_c)-qoofi2*sin(theta_c);

         //����任�õ����Ƶ�ѹU����U��
         Udq_to_Ualphabeta.Ds=dOut_com1+dOut_com2+A_Id_R.qOut;
         Udq_to_Ualphabeta.Qs=qOut_com1+qOut_com2+A_Iq_R.qOut;
       }

      else
      {
           Udq_to_Ualphabeta.Ds=A_Id_R.qOut;  //PI���
           Udq_to_Ualphabeta.Qs=A_Iq_R.qOut;
      }

       ipark_calc(&Udq_to_Ualphabeta);//���U����U��

 }

//ROGI����������
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
