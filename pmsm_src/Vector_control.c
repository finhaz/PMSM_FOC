/*
 * Vector_control.c
 *
 */

#include "includes.h"

#define ADRC_CURRENT 1

//�ǶȲ���
void angle_compensate(void)
{
    float gain2;
    gain2=jiaosudu1_asr*T;

//    theta_buchan=theta+gain2;
//    theta_buchan1=theta+gain2*2;

    theta_buchan=theta;
    theta_buchan1=theta+gain2*1.5;

    if(theta_buchan>=twopi)
    {
        theta_buchan=theta_buchan-twopi;
    }
    if(theta_buchan1>=twopi)
    {
        theta_buchan1=theta_buchan1-twopi;
    }

//    theta_buchan=theta;
//    theta_buchan1=theta;
}





//IF������
void OpenStartUp_angle(void)
{
    float gain1;
	//�Ƕȹ���---thetam_given��Ҫ�趨��ʼֵ����ת�ӳ�ʼλ�ý�-90��Ƕ�
	gain1=speed_cankao*T;
	thetam_given = thetam_given +gain1;//�����Ƕ�λ����Ҫ��ʵ�ʽǶ�λ�ã���λ�Ƕȣ��ͺ�90��
	if(thetam_given>=twopi)//��ֹ�Ƕ������������ܳɹ����㣬ע�⣺����wr_startup�仯�ܴ�Ҳ����������
	{
		thetam_given=thetam_given-twopi;
	}
	Ialphabeta_to_Idq.Angle =thetam_given;//�任�Ƕ�
	Udq_to_Ualphabeta.Angle =thetam_given+(gain1*1.5);//2018/6/5�����𶯵ĽǶ�
//	Udq_to_Ualphabeta.Angle =thetam_given;
	A_Iq_R.qInRef=iq_start_0;    //����q�����
	A_Id_R.qInRef=0;//����d�����
    ADRC_iq.qInRef=iq_start_0;//����
    ADRC_id.qInRef=0;//����
}

//-------------------------Ԥ��λ------------------------------------------------------------------------

/////////////////////Ԥ��λ�㷨//////////////////////
//���ڲ�����d�����������q������������Ƕȸ�������������Ҫʸ�����ƺ�����ִ��
void predir(void)
{
//----------------Ԥ��λ��ʼ-----------------------
	pre_dir_count++;
	if( pre_dir_count<=timeup_1 )
	{
	    thetam_given= thetam_predir-PI;
	}
	else if((pre_dir_count>=timeup_1)&&(pre_dir_count<=timeup_2))//120��Ԥ��λ
	{
	    thetam_given= thetam_predir-PIby2;
	}
//--------------Ԥ��λ����-----------------------------
	else if(pre_dir_count>timeup_2)//Ԥ��λ����
	{
		FlagRegs.flagsystem.bit.pre_dir_sign =1;//Ԥ��λ��������
		//��ʼ��������pi������
		A_Id_R.qdSum=0;
		A_Iq_R.qdSum=0;
	}
	Ialphabeta_to_Idq.Angle=thetam_given ;//Ԥ��λʱ���õĽǶȱȵ��ʼ�ջ��ͺ�90��
	Udq_to_Ualphabeta.Angle=thetam_given;//2018/6/5
//------------------Ԥ��λ��������ڲ���------------------------------------------------------
	A_Iq_R.qInRef=iq_start_0;//����
	A_Id_R.qInRef=0;//����

	ADRC_iq.qInRef=iq_start_0;//����
    ADRC_id.qInRef=0;//����
}

//�ٶ�б��
void Ramp_MACRO(void)
{
	//--------------б��---------------------------
	//speed_cankao=sudu_max;//��Ծ

	if(((speed_cankao-sudu_max)<-0.5)||((speed_cankao-sudu_max)>0.5))
	{
	    if (speed_cankao<sudu_max)
	    {
	        speed_cankao=speed_cankao+Kw*T;//б��
	    }
	    else if(speed_cankao>sudu_max)
	    {
	        speed_cankao=speed_cankao-Kw*T;//б��
	    }
	}
	else
	{
		speed_cankao=sudu_max;
	}
}

//id����
void id_setzero(void)
{
	if(id_start>id_level)
	{
		id_start=id_start-id_ratio*T;
	}
	else
	{
		id_start=id_level;
	}
	id_fankui=id_start;
}

//״̬�л�����IFתΪ��λ�ñջ�
void trans(void)
{
    CLARKE_PARK Utran=CLARKE_PARK_DEFAULTS;
	if(speed_cankao>=speed_tran&&FlagRegs.flagsystem.bit.speed_loop_sign==0)//�л��׶ο���
	{
	    FlagRegs.flagsystem.bit.speed_loop_sign=1;

	    Utran.Angle=thetam_given;//2018/6/5
		A_Iq_R.qInRef=iq_start_0;    //����q�����
		A_Id_R.qInRef=0;//_IQ(0);         //����d�����
		Ialphabeta_to_Idq.Angle=thetam_given;//�任�Ƕ�

#ifdef ADRC_CURRENT
        current_loop_adrc();
#else
        current_loop();
#endif

	    //�����л�ʱ��ʹ�ã������л�ǰ��U����U�²���
		Utran.Alpha=Udq_to_Ualphabeta.Alpha;
		Utran.Beta=Udq_to_Ualphabeta.Beta;
	    //���������U����U�£��������Ƕȣ��������Ƕ��¶�Ӧ��Ud��Uq
	    //���л�ʱ�����Ǵ����������ֵ���������U����U�²���
	    //Utran.Angle=Pll.theta_fan;
	    Utran.Angle=theta;
	    park_calc(&Utran);

		//�������öϵ����л�ʱ�̵�iq��id�Ĳο�����
		iq_start=iq_start_0*cos(delt_theta);
		id_start=iq_start_0*sin(delt_theta);
		ASR.qdSum=iq_start;
		ADRC_speed.qdSum=iq_start*ADRC_speed.b/ADRC_speed.qKi;
		A_Iq_R.qdSum=Utran.Qs;
		A_Id_R.qdSum=Utran.Ds;

		ADRC_iq.qdSum=Utran.Qs*ADRC_iq.b/ADRC_iq.qKi;
		ADRC_id.qdSum=Utran.Ds*ADRC_id.b/ADRC_id.qKi;

		iq_fankui=iq_start;
	}
}

////������ͣ��
//void set_stop(void)
//{
//    float delt_U;
//    sudu_max=0;
//	delt_U=Adcget.Vdc-Udc_limit;
//	if(delt_U<0)
//	{
//		iq_buchan=0;
//	}
//	else
//	{
//		iq_buchan=delt_U/5;
//	}
//	//iq_fankui=iq_fankui+iq_buchan;//����q�����
//
//
//	if( iq_fankui<=0)
//	{
//	    if(jiaosudu1_asr<200)
//		{
//	        FlagRegs.flagsystem.bit.free_stop_sign=1;
//		}
//	}
//
//}

//������ͣ��
void set_stop(void)
{
    sudu_max=0;

    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==1&&speed_cankao<(speed_tran+500))
    {
        A_Id_R.qdSum=A_Id_R.qOut-jiaosudu1_asr*Ls*A_Iq_R.qInMeas ;
        A_Iq_R.qdSum=A_Iq_R.qOut+jiaosudu1_asr*(Ls*A_Id_R.qInMeas +fluxm);
        FlagRegs.flagsystem.bit.jieou_inuse_sign=0;
    }

    if(jiaosudu1_asr<speed_tran)
    {
        FlagRegs.flagsystem.bit.speed_loop_sign=0;
        FlagRegs.flagsystem.bit.if_start=0;

        thetam_given=theta_buchan;
        iq_start_0=iq_fankui;

    }

}







//--------------------ȫ������---------------------------------------
//------------------------ʸ�����Ʋ���---------------
//----------------------��ʼλ�ù۲�+λ�ù۲���+IF��--------
void Sensorless_control(void)
{
    if(FlagRegs.flagsystem.bit.free_stop_sign==0)
    {
        if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//Ԥ��λ�㷨����
        {
            Ramp_MACRO();
            Paramet_Update();
            if(FlagRegs.flagsystem.bit.if_start==0)//IF����
            {
                OpenStartUp_angle();
                delt_theta= Pll.theta_fan-thetam_given;//������������������
            }

            //fandianshi_speed();
            fandianshi_speed_improve();
            //STO_speed();
            //STO_speed_Improve();
            //SMO_speed();
            //MARS_speed();


            jiaosudu1_asr=We_Filter.y;
            theta=Pll.theta_fan;

            //jiaosudu1_asr=We_Filter2.y;
            //theta=PLL_e.theta_sgn;

//            //λ�ô���������
//            //����λ�õ������������ƣ�����Ԥ��λ��Ŀ���ǽ���������ܶ�λ����е���
//            //Ȼ��EQEP��IF�׶���index��⵽�������������������Ӷ�ʵ�ֳ�ʼԤ��λ
//            encoder_solver();
//            jiaosudu1_asr=encoder_cal.w_mech*pn;
//            theta=encoder_cal.theta_mech*pn;


            trans();//��λ���л�
            if(FlagRegs.flagsystem.bit.speed_loop_sign==1)
            {
                FlagRegs.flagsystem.bit.if_start=1;//if�����������л�Ϊ��λ�ñջ�
                id_setzero();//-----------------------��������ֵ����Ϊ0---------------------------------------
                //speed_loop();//--�ٶȻ�+��ѹ�⻷10ms-
                speed_loop_adrc();

                //����ģ��
                weak_loop();
                //weak_loop_y();

                angle_compensate();//�ǶȲ���
                if(jia_jian_su==1)//��������ͣ��
                {
                    set_stop();//ͣ��q���������
                }
                A_Iq_R.qInRef=iq_fankui;
                A_Id_R.qInRef=id_fankui;

                ADRC_iq.qInRef=iq_fankui;
                ADRC_id.qInRef=id_fankui;

                Ialphabeta_to_Idq.Angle =theta_buchan;//�任�Ƕ�
                Udq_to_Ualphabeta.Angle=theta_buchan1;
            }
        }
        else//Ԥ��λ�㷨
        {
           predir();
        }

        //����������
#ifdef ADRC_CURRENT
        current_loop_adrc();
#else
        current_loop();
#endif
        //current_loop_rogi();
    }
    else if( FlagRegs.flagsystem.bit.free_stop_sign==1)
    {
        breaking();
    }
}


////������֮������ͣ�����Ҽ���Ԥ��λ
//void breaking(void)
//{
//	zhiliu_count+=1;
//	if(zhiliu_count<50000)
//	{
//	    FlagRegs.flagsystem.bit.stop_pedir_sign=1;
//		PIZero();
//	}
//	else
//	{
//	    FlagRegs.flagsystem.bit.stop_pedir_sign=0;
//		//ֱͨ����
//		break_stage+=1;
//		if(break_stage<time_break)//ֱ����
//		{
//			A_Iq_R.qInRef=iq_pre_break;//����q�����
//			A_Id_R.qInRef=0;
//
//			Ialphabeta_to_Idq.Angle=0;//�任�Ƕ�
//			Udq_to_Ualphabeta.Angle=0;
//			current_loop();//���������Ҫ��dq����������PI�������Ƕ�
//		}
//		else
//		{
//			Switchsystem = 0;//
//		}
//	}
//}

//������֮������ͣ��
void breaking(void)
{
    FlagRegs.flagsystem.bit.stop_pedir_sign=1;
    PIZero();
    Switchsystem = 0;
}


