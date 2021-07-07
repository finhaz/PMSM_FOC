/*
 * interrupt.c
 *
 */

#include "includes.h"

//SCI接收中断服务子程序
interrupt void scirxintab_isr(void)
{
    //中断程序部分
    SciRecieve();
    //清中断标志位
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;//将PIEACK寄存器的第九位写1，清除了PIE第九位的中断请求
    EINT;
}

//外部中断
interrupt void xint1_isr(void)
{
//	Switchsystem=0;
	FlagRegs.flagfault.bit.ocfault=1;
	//清中断标志位
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


interrupt void adca1_interrupt_isr(void)
{

	//GpioDataRegs.GPADAT.bit.GPIO31=1;
	Adcread();//采样子程序
	Adcdeal();


	//是否校零完成
	if(FlagRegs.flagsystem.bit.AC0CheckFinished==1)//交流校零已经完成
	{
		//保护
		FastFaultTest();//电流电压快速故障检测
		//停机判断
		if (FlagRegs.flagsystem.bit.faultoccur ==1) //若故障，则停机，故障停机的优先级更高
		{
//			Switchsystem = 0;//表示开关打开，需要停机
//			SYSTEMoff();//关机
		}
		FlagRegs.flagsystem.bit.sysonoff=Switchsystem;//上位机启停机命令(上位机对Switchsystem是在SCI程序中处理的)到达
		//是否停机
		if(FlagRegs.flagsystem.bit.sysonoff==1)////否，运行
		{
//---------------------控制器-----------------------------------
//-----将ABC坐标系的电流变换到αβ坐标系----------

			Ialphabeta_to_Idq.As=Adcget.Ia;
			Ialphabeta_to_Idq.Bs=Adcget.Ib;
			Ialphabeta_to_Idq.Cs=Adcget.Ic;
			clark_calc(&Ialphabeta_to_Idq);


//			ABC_control();

//			ABC_Vol_control();

//			AlphaBeta_control();

			Sensorless_control();

//			Sensor_control();

            //---------------------根据调制方式计算比较寄存器值-----------------------------------

//            spwm_exe();
//            EPwm1Regs.CMPA.half.CMPA=Spwm.Tcmpa;
//            EPwm2Regs.CMPA.half.CMPA=Spwm.Tcmpb;
//            EPwm3Regs.CMPA.half.CMPA=Spwm.Tcmpc;

            svpwm_exe();
            EPwm1Regs.CMPA.half.CMPA=Svpwm.Tcmpa;
            EPwm2Regs.CMPA.half.CMPA=Svpwm.Tcmpb;
            EPwm3Regs.CMPA.half.CMPA=Svpwm.Tcmpc;

		}
		else
		{
			SYSTEMoff();//关机
		}
	}

#ifdef Float_message
    Speedloop_ref=ASR.qInRef;
    Speedloop_meas=ASR.qInMeas;
    Speedloop_qout=ASR.qOut;
    Speed_pll=Pll.we;
    Speed_filter=jiaosudu1_asr;

    CurrDloop_ref=A_Id_R.qInRef;
    CurrDloop_meas=A_Id_R.qInMeas;
    CurrDloop_qout=A_Id_R.qOut;

    CurrQloop_ref=A_Iq_R.qInRef;
    CurrQloop_meas=A_Iq_R.qInMeas;
    CurrQloop_qout=A_Iq_R.qOut;

    Weakloop_ref=Weak_R.qInRef;
    Weakloop_meas=Weak_R.qInMeas;
    Weakloop_qout=Weak_R.qOut;

    U_Alpha=Udq_to_Ualphabeta.Alpha;
    U_Beta=Udq_to_Ualphabeta.Beta;
    U_s=Weak_R.qInMeas;

    U_dc=Adcget.Vdc;
    //I_dc=Idc;
    I_AS=Ialphabeta_to_Idq.As;
    I_BS=Ialphabeta_to_Idq.Bs;
    I_CS=Ialphabeta_to_Idq.Cs;

    FaultOccurr=FlagRegs.flagfault.all;
#else
    Speedloop_ref=ASR.qInRef;
    Speedloop_meas=ASR.qInMeas;
    Speedloop_qout=ASR.qOut;
    Speed_pll=Pll.we;
    Speed_filter=jiaosudu1_asr;

    CurrDloop_ref=A_Id_R.qInRef*100;
    CurrDloop_meas=A_Id_R.qInMeas*100;
    CurrDloop_qout=A_Id_R.qOut*100;

    CurrQloop_ref=A_Iq_R.qInRef*100;
    CurrQloop_meas=A_Iq_R.qInMeas*100;
    CurrQloop_qout=A_Iq_R.qOut*100;

    Weakloop_ref=Weak_R.qInRef*100;
    Weakloop_meas=Weak_R.qInMeas*100;
    Weakloop_qout=Weak_R.qOut*100;

    U_Alpha=Udq_to_Ualphabeta.Alpha*100;
    U_Beta=Udq_to_Ualphabeta.Beta*100;
    U_s=Weak_R.qInMeas*100;

    U_dc=Adcget.Vdc*10;
    //I_dc=Idc;
    I_AS=Ialphabeta_to_Idq.As*100;
    I_BS=Ialphabeta_to_Idq.Bs*100;
    I_CS=Ialphabeta_to_Idq.Cs*100;

    FaultOccurr=FlagRegs.flagfault.all;
#endif

    // Reinitialize for next ADC sequence
    AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;//make sure INT1 flag is cleared
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	//GpioDataRegs.GPADAT.bit.GPIO31=0;
}
