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
    //MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);//要拷贝到ram里的函数在上电的时候拷贝过去
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();//标准的初始化flash
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
    //开全局中断
    EINT;//使能全局中断（开中断）（CPU级的）
    ERTM;//使能实时中断（CPU级的）

    ///////////////////////////////////////初始化结束
    while(1)
    {

        Checkdata();
        SciSend();

        sci_recover();
//        WrEeprom();

        //是否停机
        if( FlagRegs.flagsystem.bit.sysonoff == 0)//停机
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
                    PWMzero();//零矢量
                }
                else
                {
                    PWMoff();//封锁PWM
                }
            }
        }
    }

}

