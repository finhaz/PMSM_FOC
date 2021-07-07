/*
 * Offline_est.c
 *
 *  Created on: 2021��2��28��
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
void parameter_ID_R(void)//ע������������Ԥ��λ���ƣ�Ϊ���ȶ�����ҪԤ��λ
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
void parameter_ID_flux(void)//IF��������Ҫ��λ
{
    if(FlagRegs.flagsystem.bit.pre_dir_sign==0)//Ԥ��λ�㷨,
    {
        predir();
        Udq_to_Ualphabeta.Angle=thetam;//2018/6/5
    }
    if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//Ԥ��λ�㷨����
    {
    }
    Ialphabeta_to_Idq.Angle=thetam;//�任�Ƕ�
    park_calc(&Ialphabeta_to_Idq); //�õ�������id��iq
    //-----------������----------------------------------------------------
    //��Ҫ�ǵ��������ڣ��������Ϊpi����������������
    current_loop();//���������Ҫ��dq����������PI�������Ƕ�
}
#endif

#ifdef ID_L
_iq thetamL=0;
_iq Ug=0;
//��һ�ε�������
int cnt1=0;//����ز�
int fcnt1=0;
_iq I1sum=0;
_iq I1n=0;
_iq I1max=0;
_iq I1min=0;
//�ڶ��ε�������
#define carriern 40
#define filtern 60
int cnt2=0;//����ز�
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

void parameter_ID_L(void)//HFI��������Ԥ��λ
{
    if(FlagRegs.flagsystem.bit.pre_dir_sign==0)//Ԥ��λ�㷨,
    {
        //iq_start_0=32768;
        predir();
        Udq_to_Ualphabeta.Angle=thetam;//2018/6/5
        Ialphabeta_to_Idq.Angle=thetam;//�任�Ƕ�
        park_calc(&Ialphabeta_to_Idq); //�õ�������id��iq
        //-----------������----------------------------------------------------
        //��Ҫ�ǵ��������ڣ��������Ϊpi����������������
        vector_controller();//���������Ҫ��dq����������PI�������Ƕ�
        thetamL=0;
        t_L=0;
    }
//    if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//Ԥ��λ�㷨
//    {
//      N_stage2=1;
//    }

    if(FlagRegs.flagsystem.bit.pre_dir_sign==1)//Ԥ��λ�㷨,
    {
        if(t_L<Ltimeup_1)
        {
            t_L++;

            thetamL = thetamL +wGAIN;
            if(thetamL>=twopi)//��ֹ�Ƕ������������ܳɹ����㣬ע�⣺����wr_startup�仯�ܴ�Ҳ����������
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

            Udq_to_Ualphabeta.As=Ug;//�õ�ua,ub,uc
            Udq_to_Ualphabeta.Bs=-Ug;
            Udq_to_Ualphabeta.Cs=0;
        }
        if(t_L>=Ltimeup_1&&t_L<Ltimeup_2)
        {
            t_L++;
            thetamL = thetamL +wGAIN;
            if(thetamL>=twopi)//��ֹ�Ƕ������������ܳɹ����㣬ע�⣺����wr_startup�仯�ܴ�Ҳ����������
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

            Udq_to_Ualphabeta.As=Ug;//�õ�ua,ub,uc
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
