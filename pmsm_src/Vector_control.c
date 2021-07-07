/*
 * Vector_control.c
 *
 */

#include "includes.h"

//角度补偿
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


//包含电流环、启动，位置观测计算，转速估测计算
//电流环控制
void current_loop(void)
{

//-----------------电流环------------------------------------------------
    park_calc(&Ialphabeta_to_Idq); //得到反馈量id、iq
//----------------pi参数----------------------------------------

//    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==0&&speed_cankao>(speed_tran+500))
//    {
//        A_Id_R.qdSum=A_Id_R.qOut+jiaosudu1_asr*Ls*A_Iq_R.qInRef;
//        A_Iq_R.qdSum=A_Iq_R.qOut-jiaosudu1_asr*(Ls*A_Id_R.qInRef+fluxm);
//        FlagRegs.flagsystem.bit.jieou_inuse_sign=1;
//    }

    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==0&&speed_cankao>(speed_tran+500))
    {
        A_Id_R.qdSum=A_Id_R.qOut+jiaosudu1_asr*Ls*A_Iq_R.qInMeas ;
        A_Iq_R.qdSum=A_Iq_R.qOut-jiaosudu1_asr*(Ls*A_Id_R.qInMeas +fluxm);
        FlagRegs.flagsystem.bit.jieou_inuse_sign=1;
    }

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


    A_Iq_R.qInMeas=Ialphabeta_to_Idq.Qs;//q反馈电流
    A_Id_R.qInMeas=Ialphabeta_to_Idq.Ds;//d反馈电流

//	//滤波后电流处理
//    id_filtrate.X_in=Ialphabeta_to_Idq.Ds;
//    FILTRATE_CALC(&id_filtrate);
//
//    iq_filtrate.X_in=Ialphabeta_to_Idq.Qs;
//    FILTRATE_CALC(&iq_filtrate);

//    A_Iq_R.qInMeas=iq_filtrate.Y ;//q反馈电流
//    A_Id_R.qInMeas=id_filtrate.Y;//d反馈电流

	PI_CONTROL_CALC(&A_Iq_R);//输出q轴调制电压Uq
	PI_CONTROL_CALC(&A_Id_R);//输出d轴调制电压Ud

    if(FlagRegs.flagsystem.bit.jieou_inuse_sign==1)
    {
        Udq_to_Ualphabeta.Ds=A_Id_R.qOut-jiaosudu1_asr*Ls*A_Iq_R.qInRef;
        Udq_to_Ualphabeta.Qs=A_Iq_R.qOut+jiaosudu1_asr*(Ls*A_Id_R.qInRef+fluxm);
        if(Udq_to_Ualphabeta.Ds>max_current)
            Udq_to_Ualphabeta.Ds=max_current;
        if(Udq_to_Ualphabeta.Ds<min_current)
            Udq_to_Ualphabeta.Ds=min_current;

        if(Udq_to_Ualphabeta.Qs>max_current)
            Udq_to_Ualphabeta.Qs=max_current;
        if(Udq_to_Ualphabeta.Qs<min_current)
            Udq_to_Ualphabeta.Qs=min_current;
    }
    else
    {
        Udq_to_Ualphabeta.Ds=A_Id_R.qOut;
        Udq_to_Ualphabeta.Qs=A_Iq_R.qOut;
    }

    //坐标变换得到调制电压Uα、Uβ

	ipark_calc(&Udq_to_Ualphabeta);//输出Uα、Uβ


}


//IF开环起动
void OpenStartUp_angle(void)
{
    float gain1;
	//角度构建---thetam_given需要设定初始值，即转子初始位置角-90电角度
	gain1=speed_cankao*T;
	thetam_given = thetam_given +gain1;//启动角度位置需要比实际角度位置（定位角度）滞后90度
	if(thetam_given>=twopi)//防止角度溢出，软件不能成功计算，注意：由于wr_startup变化很大，也可能造成溢出
	{
		thetam_given=thetam_given-twopi;
	}
	Ialphabeta_to_Idq.Angle =thetam_given;//变换角度
	Udq_to_Ualphabeta.Angle =thetam_given+(gain1*1.5);//2018/6/5补偿起动的角度
//	Udq_to_Ualphabeta.Angle =thetam_given;
	A_Iq_R.qInRef=iq_start_0;    //给定q轴电流
	A_Id_R.qInRef=0;//给定d轴电流
}

//-------------------------预定位------------------------------------------------------------------------

/////////////////////预定位算法//////////////////////
//出口参数：d轴电流给定，q轴电流给定，角度给定，函数后需要矢量控制函数来执行
void predir(void)
{
//----------------预定位开始-----------------------
	pre_dir_count++;
	if( pre_dir_count<=timeup_1 )
	{
	    thetam_given= thetam_predir-PI;
	}
	else if((pre_dir_count>=timeup_1)&&(pre_dir_count<=timeup_2))//120度预定位
	{
	    thetam_given= thetam_predir-PIby2;
	}
//--------------预定位结束-----------------------------
	else if(pre_dir_count>timeup_2)//预定位结束
	{
		FlagRegs.flagsystem.bit.pre_dir_sign =1;//预定位计数结束
		//初始化电流环pi调节器
		A_Id_R.qdSum=0;
		A_Iq_R.qdSum=0;
	}
	Ialphabeta_to_Idq.Angle=thetam_given ;//预定位时设置的角度比电机始终会滞后90度
	Udq_to_Ualphabeta.Angle=thetam_given;//2018/6/5
//------------------预定位控制器入口参数------------------------------------------------------
	A_Iq_R.qInRef=iq_start_0;//给定
	A_Id_R.qInRef=0;//给定
}

//速度斜坡
void Ramp_MACRO(void)
{
	//--------------斜坡---------------------------
	//speed_cankao=sudu_max;//阶跃

	if(((speed_cankao-sudu_max)<-0.5)||((speed_cankao-sudu_max)>0.5))
	{
	    if (speed_cankao<sudu_max)
	    {
	        speed_cankao=speed_cankao+Kw*T;//斜坡
	    }
	    else if(speed_cankao>sudu_max)
	    {
	        speed_cankao=speed_cankao-Kw*T;//斜坡
	    }
	}
	else
	{
		speed_cankao=sudu_max;
	}
}

//id清零
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

//状态切换，从IF转为无位置闭环
void trans(void)
{
    CLARKE_PARK Utran=CLARKE_PARK_DEFAULTS;
//    test_Iq=iq_start_0*cos(delt_theta);
//    test_Id=iq_start_0*sin(delt_theta);
//    test_Te=1.5*2*0.024258*test_Iq;
	if(speed_cankao>=speed_tran&&FlagRegs.flagsystem.bit.speed_loop_sign==0)//切换阶段开启
	{
	    FlagRegs.flagsystem.bit.speed_loop_sign=1;

	    Utran.Angle=thetam_given;//2018/6/5
		A_Iq_R.qInRef=iq_start_0;    //给定q轴电流
		A_Id_R.qInRef=0;//_IQ(0);         //给定d轴电流
		Ialphabeta_to_Idq.Angle=thetam_given;//变换角度
		current_loop();

	    //用于切换时候使用，保持切换前后Uα、Uβ不变
		Utran.Alpha=Udq_to_Ualphabeta.Alpha;
		Utran.Beta=Udq_to_Ualphabeta.Beta;
	    //根据输出的Uα、Uβ，代入估算角度，算出估算角度下对应的Ud和Uq
	    //在切换时将它们代入电流环中值，保持输出Uα、Uβ不变
	    //Utran.Angle=Pll.theta_fan;
	    Utran.Angle=theta;
	    park_calc(&Utran);

		//可以设置断点检测切换时刻的iq和id的参考给定
		iq_start=iq_start_0*cos(delt_theta);
		id_start=iq_start_0*sin(delt_theta);
		ASR.qdSum=iq_start;
		A_Iq_R.qdSum=Utran.Qs;
		A_Id_R.qdSum=Utran.Ds;
		iq_fankui=iq_start;
	}
}

////负力矩停机
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
//	//iq_fankui=iq_fankui+iq_buchan;//给定q轴电流
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

//负力矩停机
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


//速度环控制
void speed_loop(void)
{
    stage_asr++;
	if(stage_asr==50)//速度环
	{
	    stage_asr=0;

		ASR.qInMeas=jiaosudu1_asr;
		ASR.qInRef=speed_cankao;
		ASR.qKp=kp_speed;//_IQ(0.0025);//速度环积分系数_IQ(0.1);
		ASR.qKi=ki_speed;//_IQ(0.00025);//_IQ(0.001);//速度环比例系数_IQ(0.0005)
		ASR.qOutMax=max_speed;
		ASR.qOutMin=min_speed;
		PI_CONTROL_CALC(&ASR);//电流环给定
	}

	iq_fankui=ASR.qOut;//给定q轴电流
}






//--------------------全速运行---------------------------------------
//------------------------矢量控制部分---------------
//----------------------初始位置观测+位置观测器+IF起动--------
void Sensorless_control(void)
{
    if(FlagRegs.flagsystem.bit.free_stop_sign==0)
    {
        if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//预定位算法结束
        {
            Ramp_MACRO();
            Paramet_Update();
            if(FlagRegs.flagsystem.bit.if_start==0)//IF启动
            {
                OpenStartUp_angle();
                delt_theta= Pll.theta_fan-thetam_given;//计算估算角与给定角相差
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

            ////位置传感器反馈
            //与无位置的启动策略类似，这里预定位的目的是将电机尽可能定位到机械零点
            //然后EQEP在IF阶段用index检测到该零点与编码器零点的误差，从而实现初始预定位
            //encoder_solver();
            //jiaosudu1_asr=encoder_cal.w_mech*pn;
            //theta=encoder_cal.theta_mech*pn;


            trans();//无位置切换
            if(FlagRegs.flagsystem.bit.speed_loop_sign==1)
            {
                FlagRegs.flagsystem.bit.if_start=1;//if启动结束，切换为无位置闭环
                id_setzero();//-----------------------电流给定值调节为0---------------------------------------
                speed_loop();//--速度环+电压外环10ms-

                //弱磁模块
                //weak_loop();
                weak_loop_y();

                angle_compensate();//角度补偿
                if(jia_jian_su==1)//允许负力矩停机
                {
                    set_stop();//停机q轴电流设置
                }
                A_Iq_R.qInRef=iq_fankui;
                A_Id_R.qInRef=id_fankui;
                Ialphabeta_to_Idq.Angle =theta_buchan;//变换角度
                Udq_to_Ualphabeta.Angle=theta_buchan1;
            }
        }
        else//预定位算法
        {
           predir();
        }
        //-----------控制器----------------------------------------------------
        //主要是电流环调节，输入参数为pi参数，电流给定、
        current_loop();//输入参数主要是dq电流给定，PI参数，角度
        //current_loop_rogi();
    }
    else if( FlagRegs.flagsystem.bit.free_stop_sign==1)
    {
        breaking();
    }
}


////负力矩之后，自由停机并且加入预定位
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
//		//通直流量
//		break_stage+=1;
//		if(break_stage<time_break)//直流量
//		{
//			A_Iq_R.qInRef=iq_pre_break;//给定q轴电流
//			A_Id_R.qInRef=0;
//
//			Ialphabeta_to_Idq.Angle=0;//变换角度
//			Udq_to_Ualphabeta.Angle=0;
//			current_loop();//输入参数主要是dq电流给定，PI参数，角度
//		}
//		else
//		{
//			Switchsystem = 0;//
//		}
//	}
//}

//负力矩之后，自由停机
void breaking(void)
{
    FlagRegs.flagsystem.bit.stop_pedir_sign=1;
    PIZero();
    Switchsystem = 0;
}


