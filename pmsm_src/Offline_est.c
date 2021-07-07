/*
 * Offline_est.c
 *
 *  Created on: 2021年2月28日
 *      Author: sea
 */


#ifdef ID_R
#define calr_1 40000
#define calr_2 80000
Uint32 t_r=0;
Uint16 t_r_flag=0;
_iq UR1=0;
_iq UR2=0;
_iq IR1=0;
_iq IR2=0;
_iq sum1=0;
_iq sum2=0;
_iq delta_ur=0;
_iq delta_ir=0;
Uint16 cnt=0;
float rts=0;
void parameter_ID_R(void)//注入的零序电流与预定位类似，为了稳定，需要预定位
{
    if(t_r_flag==0)
    {
        t_r++;
        if(t_r<=calr_1)
        {
            EPwm1Regs.CMPA.half.CMPA=3000-100;
            UR1=Udc*100/3000;
            //IR1=-Iabc_to_Ialphabeta.Bs;
            cnt++;
            sum1-=Iabc_to_Ialphabeta.Bs;
            if(cnt==100)
            {
                IR1=sum1/100;
                sum1=0;
                cnt=0;

            }
        }
        if(t_r==calr_1)
        {
            cnt=0;
        }
        if(t_r>calr_1&&t_r<calr_2)
        {
            EPwm1Regs.CMPA.half.CMPA=3000-190;
            UR2=Udc*190/3000;
            //IR2=-Iabc_to_Ialphabeta.Bs;
            cnt++;
            sum2-=Iabc_to_Ialphabeta.Bs;
            if(cnt==100)
            {
                IR2=sum2/100;
                sum2=0;
                cnt=0;

            }
        }
        if(t_r==calr_2)
        {
            t_r_flag=1;
            cnt=0;
        }
    }
    else
    {
        delta_ur=UR2-UR1;
        delta_ir=IR2-IR1;
        rts=(float)delta_ur/delta_ir-0.05;
        N_stage2=1;
    }
}
#endif

#ifdef ID_flux
void parameter_ID_flux(void)//IF启动后，需要定位
{
    if(FlagRegs.flagsystem.bit.pre_dir_sign==0)//预定位算法,
    {
        predir();
        Udq_to_Ualphabeta.Angle=thetam;//2018/6/5
    }
    if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//预定位算法结束
    {
    }
    Ialphabeta_to_Idq.Angle=thetam;//变换角度
    park_calc(&Ialphabeta_to_Idq); //得到反馈量id、iq
    //-----------控制器----------------------------------------------------
    //主要是电流环调节，输入参数为pi参数，电流给定、
    current_loop();//输入参数主要是dq电流给定，PI参数，角度
}
#endif

#ifdef ID_L
_iq thetamL=0;
_iq Ug=0;
//第一次电流采样
int cnt1=0;//针对载波
int fcnt1=0;
_iq I1sum=0;
_iq I1n=0;
_iq I1max=0;
_iq I1min=0;
//第二次电流采样
#define carriern 40
#define filtern 60
int cnt2=0;//针对载波
int fcnt2=0;
_iq I2sum=0;
_iq I2n=0;
_iq I2max=0;
_iq I2min=0;
//wGAIN=2*PI*250*T=102944*300*0.0001=3088.32
#define wGAIN 3088
#define wf 300
#define Ltimeup_1 50000//5s
#define Ltimeup_2 100000//10s
Uint32 t_L=0;
_iq Is;
float L_cal;
#define  uset 35

void parameter_ID_L(void)//HFI本身无需预定位
{
    if(FlagRegs.flagsystem.bit.pre_dir_sign==0)//预定位算法,
    {
        //iq_start_0=32768;
        predir();
        Udq_to_Ualphabeta.Angle=thetam;//2018/6/5
        Ialphabeta_to_Idq.Angle=thetam;//变换角度
        park_calc(&Ialphabeta_to_Idq); //得到反馈量id、iq
        //-----------控制器----------------------------------------------------
        //主要是电流环调节，输入参数为pi参数，电流给定、
        vector_controller();//输入参数主要是dq电流给定，PI参数，角度
        thetamL=0;
        t_L=0;
    }
//    if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//预定位算法
//    {
//      N_stage2=1;
//    }

    if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//预定位算法,
    {
        if(t_L<Ltimeup_1)
        {
            t_L++;

            thetamL = thetamL +wGAIN;
            if(thetamL>=twopi)//防止角度溢出，软件不能成功计算，注意：由于wr_startup变化很大，也可能造成溢出
            {
                thetamL=thetamL-twopi;
            }
            Ug=uset*_IQsin(thetamL);

            if(I1max<Iabc_to_Ialphabeta.As)
                I1max=Iabc_to_Ialphabeta.As;
            if(I1min>Iabc_to_Ialphabeta.As)
                I1min=Iabc_to_Ialphabeta.As;

            cnt1++;
            if(cnt1==carriern)
            {
                I1sum+=(I1max-I1min)/2;
                cnt1=0;
                I1max=0;
                I1min=0;
                fcnt1++;
                if(fcnt1==filtern)
                {
                    I1n=I1sum/filtern;
                    fcnt1=0;
                    I1sum=0;
                }
            }

            Udq_to_Ualphabeta.As=Ug;//得到ua,ub,uc
            Udq_to_Ualphabeta.Bs=-Ug;
            Udq_to_Ualphabeta.Cs=0;
        }
        if(t_L>=Ltimeup_1&&t_L<Ltimeup_2)
        {
            t_L++;
            thetamL = thetamL +wGAIN;
            if(thetamL>=twopi)//防止角度溢出，软件不能成功计算，注意：由于wr_startup变化很大，也可能造成溢出
            {
                thetamL=thetamL-twopi;
            }
            Ug=2*uset*_IQsin(thetamL);

            if(I2max<Iabc_to_Ialphabeta.As)
                I2max=Iabc_to_Ialphabeta.As;
            if(I2min>Iabc_to_Ialphabeta.As)
                I2min=Iabc_to_Ialphabeta.As;

            cnt2++;
            if(cnt2==carriern)
            {
                I2sum+=(I2max-I2min)/2;
                cnt2=0;
                I2max=0;
                I2min=0;
                fcnt2++;
                if(fcnt2==filtern)
                {
                    I2n=I2sum/filtern;
                    fcnt2=0;
                    I2sum=0;
                }
            }

            Udq_to_Ualphabeta.As=Ug;//得到ua,ub,uc
            Udq_to_Ualphabeta.Bs=-Ug;
            Udq_to_Ualphabeta.Cs=0;
        }
        if(t_L>=Ltimeup_2)
        {
            Is=I2n-I1n;
            L_cal=(float)uset/Is*16384/6.28/wf;
            N_stage2=1;
        }
    }

}
#endif
