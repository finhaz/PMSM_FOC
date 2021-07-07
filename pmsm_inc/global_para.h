#ifndef __GOFP__
#define __GOFP__

#define Float_message 1

//����
#define  ONEbyHALF   0.5    /* 1/2 */
#define  ONEbySQRT3   0.57735026918963    /* 1/sqrt(3) */
#define  SQRT3byTWO   0.8660254    /* sqrt(3)/2 */
#define  TWObyTHREE   0.66666666666667    /* 2/3 */
#define  ONEbyTHREE   0.33333333333333    /* 1/3 */
#define  PI           3.141592654
#define  PIby2       1.570796327
#define  PIby180      0.01745329
#define  c180byPI      57.29578
#define  SQRT2      1.41421356

////����   q14��ʽ
//#define  ONEbyHALF   8192    /* 1/2 */
//#define  ONEbySQRT3   9459    /* 1/sqrt(3) */
//#define  SQRT3byTWO   14189    /* sqrt(3)/2 */
//#define  TWObyTHREE   10922    /* 2/3 */
//#define  ONEbyTHREE   5461    /* 1/3 */
//#define  PI           3.141592654
//#define  PIby180      0.01745329
//#define  c180byPI      57.29578

//�������
#define Ld 0.0005401
#define Lq 0.0007375
#define fluxm 0.0433
#define Ls 0.0007375
#define rs 0.2 //ŷķ
#define pn 1 //������



//����
#define twopi (2*PI)//102944
#define pibytwo 1.57

//��ѹ
//udcΪĸ�ߵ�ѹ
//udc_halfΪĸ�ߵ�ѹһ�루SPWM��
//#define udc_half 270
//#define PI_max_ha 270



//����
//IAD_A=I*0.075*4096/3
//IAD_B=I*0.067*4096/3
//IAD_C=I*0.0717*4096/3
//UDC=U*0.00305*4096/3
#define I_ADC 0.009766
#define I_ADC_A 0.009766
#define I_ADC_B 0.010932
#define I_ADC_C 0.010215
#define U_ADC 0.23999


////����
////IAD_A=I*0.05*4096/3
////IAD_B=I*0.067*4096/3
////IAD_C=I*0.0717*4096/3
////UDC=U*0.005*4096/3
//#define I_ADC 0.014648438
//#define I_ADC_A 0.009766
//#define I_ADC_B 0.010932
//#define I_ADC_C 0.010215
//#define U_ADC 0.146484375


//Ԥ��λ
//�����㷽����ʵ��ʱ��/��������,��0.5s->0.5/0.00005��
#define timeup_1 5000//0.25s,��һ��0.25s
#define timeup_2 10000//0.5s���ڶ���0.5-0.25=0.25s
//#define speed_tran 1200//�ٶ��л�ʱ��
#define Udc_limit 580//������ʸ���ĵ�ѹ������ֵ
#define time_break 50000//5s���Ⲩ����ͣ��ʱ��
#define iq_pre_break 0.5//ͣ��Ԥ��λ����

////����������ֵ(����ֵ)
////IAD=I*0.075*4096/3
//#define vmax_current 2048 //15A
////��ѹ������ֵ(����ֵ)
////UDC=U*3/983*4096/3=U*0.00305*4096/3
//#define max_voltage 2499 //600V
////#define min_voltage 1874 //450V
//#define min_voltage 1540 //380V

//d������л������0
#define id_level 0
//Ԥ��λ���ýǶ�
//#define thetam_predir 5.7595865//��Ӧ330�ȣ������Ƕ�
#define thetam_predir 0//��Ӧ0��,��е/����0��

#endif

