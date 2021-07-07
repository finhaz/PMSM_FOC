/*
 * Adc_self.c
 *
 */

#include "includes.h"

//�ڲ���������粻�������޸�
//AD����
Uint32 AC_Zero[10][8];
Uint32 AC_ZeroMean[10];
Uint16 AC_Zero_yu[10][8];
Uint16 AC_Zero_yuz[10];
Uint16 ADCresult[10] ;    //2
Uint16 ADCZeroCNT0;      //6����У��������������600
int32 ADCresultNEW[10]; //���ڴ��ȥƫ��֮������ݣ���ѡ����򲻼�ֱ��ƫ�ã�

void  Adcread()
{
    ADCresult[1]=ADC_UDC;//UDCLINK
    ADCresult[2]=ADC_UMAIN;//BOOST_IN
    ADCresult[3]=ADC_IA;//Iu
    ADCresult[4]=ADC_IB;//Iv
    ADCresult[5]=ADC_IC;//Iw
    ADCresult[6]=ADC_Speed;//Speed
    ADCresult[7]=ADC_Theta;//Theta
}

//adc���ݴ���

void  Adcdeal()
{
	 //AD�����ļ��㼰У��
    if(FlagRegs.flagsystem.bit.AC0CheckFinished==1)  //1�������������Ҳ����ﵽҪ��ֵ
    {
		//IAD=I*0.075*4096/3
		//UDC=U*0.00305*4096/3
		//����ֱ��ƫ�õ���

		//ֱ�����ѹ
        //ע�⣺������������·ǰ��Ҫ��ƫ�����ﵽAC_ZeroMean[1]
		ADCresultNEW[1]=ADCresult[1]-AC_ZeroMean[1];
		Adcget.Vdc=ADCresultNEW[1]*U_ADC;

        ADCresultNEW[2]= ADCresult[2]-AC_ZeroMean[2];
        Adcget.UMAINS=ADCresultNEW[2]*U_ADC;

		//U�����
		ADCresultNEW[3]= ADCresult[3]- AC_ZeroMean[3];//
		Adcget.Ia=ADCresultNEW[3]*I_ADC;
		//V�����
		ADCresultNEW[4]= ADCresult[4]- AC_ZeroMean[4];//
		Adcget.Ib=ADCresultNEW[4]*I_ADC;
		//W�����
		ADCresultNEW[5]= ADCresult[5]- AC_ZeroMean[5];//
		Adcget.Ic=ADCresultNEW[5]*I_ADC;

		Adcget.speed=ADCresult[6]*0.2556;//1/4096*3*10000/3/9.55
		Adcget.theta=ADCresult[7]*0.00153398;//1/4096*3*360/3/360*(2pi)


    }
    else
    {

		Uint16 i;
		Uint16 j;


		ADCZeroCNT0 +=1;
		for(i=0;i<10;i++)//10��������
		{
			AC_Zero[i][CounterRegs.count.bit.ADC_ZeroCNTloop] += ADCresult[i];
		}
		if(  ADCZeroCNT0 == ADCZeroCNT0num )//600��ѭ��
		{
			ADCZeroCNT0 = 0;
			for(i=0;i<10;i++)
			{
				//ȡ����
				AC_Zero_yu[i][CounterRegs.count.bit.ADC_ZeroCNTloop] = AC_Zero[i][CounterRegs.count.bit.ADC_ZeroCNTloop] % ADCZeroCNT0num;
				//��������
				AC_Zero[i][CounterRegs.count.bit.ADC_ZeroCNTloop] = AC_Zero[i][CounterRegs.count.bit.ADC_ZeroCNTloop]/ ADCZeroCNT0num;
			}
			CounterRegs.count.bit.ADC_ZeroCNTloop += 1;
		}

		if(CounterRegs.count.bit.ADC_ZeroCNTloop == ADCZeroCNTloopnum )	//8��ѭ��������һ��ѭ����600����
		{
			for(i=0;i<10;i++)
			{
				for(j=0;j<ADCZeroCNTloopnum ;j++)
				{
					AC_Zero_yuz[i] =AC_Zero_yuz[i] + AC_Zero_yu[i][j];//�ۼ�����
					AC_ZeroMean[i] = AC_ZeroMean[i] + AC_Zero[i][j] ;//�ۼ�������
				}
				AC_Zero_yuz[i] = AC_Zero_yuz[i]/ADCZeroCNT0num;
				AC_ZeroMean[i] = (AC_ZeroMean[i]+AC_Zero_yuz[i])/ADCZeroCNTloopnum ;
			}

			AC_ZeroMean[1] = 0;//Udc
			AC_ZeroMean[2] = 0;//Boost_in

			FlagRegs.flagsystem.bit.AC0CheckFinished = 1;
			CounterRegs.count.bit.ADC_ZeroCNTloop = 0;
		}


    }

}

void ADCzero()
{
	//ADC������ʼ��
	Adcget.Ia=0;
	Adcget.Ib=0;
	Adcget.Ic=0;
	Adcget.Vdc=0;
	CounterRegs.count.bit.ADC_ZeroCNTloop = 0;//У���������xѭ��
	ADCZeroCNT0=0;//У�������������
	int i,j;
	for(i=0;i<10;i++)
	{
		ADCresultNEW[1]=0;
		ADCresultNEW[3]=0;
		ADCresultNEW[4]=0;
		ADCresultNEW[5]=0;
		for(j=0;j<8;j++)
		{
			AC_Zero[i][j] = 0 ;
			AC_Zero_yu[i][j] = 0 ;
		}
	}
	for(i=0;i<10;i++)
	{
		AC_ZeroMean[i]=0;
		AC_Zero_yuz[i]= 0 ;
	}

}

