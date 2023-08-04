/*
 *     main.c
 *
 *     Created on: 2018-6-5
 *     Author: naiyangui,fin
 *     ��������Ϊ2A������Ϊ8A
 *     �����ŵ�ѹ�⻷����ʱ��ĳɺ�ת���⻷һ��Ϊ5ms
 *     ��λ�Ƕ�11pi/6
 *     ��������Kp=20;Ki=0.0001;�޷�ֵ��155��
 *     ��ѹ�⻷��Kp:0.001;Ki��0.0001��������-6;
 *     ת�ٻ���Kp=0.0025(2000rad/s��ǰ)��Ki=0.00025  �޷�ֵ��=-6
 *          Kp=0.002(3000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *          Kp=0.003(4000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *          Kp=0.004(5000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *          Kp=0.005(7000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *     �ǶȲ�������������һ�����ڽǶȣ���ѹ����2�����ڽǶ�
 *     ͣ���ƶ�������pwm,��ʸ��
 *      �����ŵ�ѹ�⻷��Ϊ1�����ڵ���һ��
 *      ����Ƶ�ʸ�Ϊ10K
 *      ����ת�٣�17000rpm
 *      �����˼���б��ģ��
 *      ����ͣ��ʹ�ð汾(iq<0,����PWM����ʱֱͨ����)
 *      �ж�ʱ������
 *      �޸��л�����
 *      ����оƬ28335
 *      ʵ��ƽ̨MR2
 *      ��Ҫ�Ķ�ΪEPWM����,AD�ӿ�,SCI�ӿ�
 *      �޸�б�¼��ٵ�bug
 *      ͨѶ���ø������Խӣ��Ҿ����˲������ñ���
 *      ���Ʒ�ʽ��spwm��Ϊsvpwm
 *      ʵ��SPWM��PRģ�黯
 *      ʵ�ַ��綯�ƹ۲���ģ�黯
 *      ǰ��ŷ��/�Ľ�ŷ����ɢ���۲������
 *      SMO��MRAS�ȼ���
 *      ADRC�������
 */

#include "includes.h"    // DSP28x Headerfile

#ifdef FLASH
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#pragma CODE_SECTION(adca1_interrupt_isr, "ramfuncs");
#endif

/**
 * main.c
 */

void main(void)
{
    InitSysCtrl();

#ifdef FLASH
    // Copy time critical code and Flash setup code to RAM
    // This includes the following functions:  InitFlash();
    // The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
    // symbols are created by the linker. Refer to the device .cmd file.
    //MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);//Ҫ������ram��ĺ������ϵ��ʱ�򿽱���ȥ
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();//��׼�ĳ�ʼ��flash
#endif
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    InitBoardGpio();
    InitEPwmGpio();
    InitSciGpio();
//    Initspigpio();
//    InitEQepGpio();
    InitCpuTimers();
    InitI2CGpio();

    ConfigureEPwm();
    ConfigureSci();
    ConfigureADC();
    ConfigureXint();
//    ConfigureSpi();
    ConfigureEQep();
    ConfigCpuTimer(&CpuTimer0, 150, 1000);
    CpuTimer0Regs.TCR.bit.TSS = 0;
    ConfigI2C();
    Initparameter();
    ReadEeprom();
    ConfigurePIE();
    //��ȫ���ж�
    EINT;//ʹ��ȫ���жϣ����жϣ���CPU���ģ�
    ERTM;//ʹ��ʵʱ�жϣ�CPU���ģ�

    ///////////////////////////////////////��ʼ������
    while(1)
    {

        Checkdata();
        SciSend();

        sci_recover();
        WrEeprom();

        //�Ƿ�ͣ��
        if( FlagRegs.flagsystem.bit.sysonoff == 0)//ͣ��
        {
            SYSTEMoff();
        }
        else
        {
            if(FlagRegs.flagsystem.bit.stop_pedir_sign==0)
            {
                PWMopen();
            }
            else
            {
                if(FlagRegs.flagsystem.bit.moder_of_stop==1)//
                {
                    PWMzero();//��ʸ��
                }
                else
                {
                    PWMoff();//����PWM
                }
            }
        }
    }

}

