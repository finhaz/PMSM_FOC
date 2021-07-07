/*
 *     main.c
 *
 *     Created on: 2018-6-5
 *     Author: naiyangui,fin
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
    InitEQepGpio();

    ConfigureEPwm();
    ConfigureSci();
    ConfigureADC();
    ConfigureXint();
//    ConfigureSpi();
    ConfigureEQep();
    Initparameter();
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
//        WrEeprom();

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

