/*
 *     main.c
 *
 *     Created on: 2018-6-5
 *     Author: naiyangui,fin
 *     启动电流为2A，保护为8A
 *     把弱磁电压外环调节时间改成和转速外环一样为5ms
 *     定位角度11pi/6
 *     电流环：Kp=20;Ki=0.0001;限幅值：155；
 *     电压外环：Kp:0.001;Ki；0.0001；限流：-6;
 *     转速环：Kp=0.0025(2000rad/s以前)；Ki=0.00025  限幅值：=-6
 *          Kp=0.002(3000rad/s以前)；Ki=0.00025   限幅值：=-6
 *          Kp=0.003(4000rad/s以前)；Ki=0.00025   限幅值：=-6
 *          Kp=0.004(5000rad/s以前)；Ki=0.00025   限幅值：=-6
 *          Kp=0.005(7000rad/s以前)；Ki=0.00025   限幅值：=-6
 *     角度补偿：电流补偿一个周期角度，电压补偿2个周期角度
 *     停机制动：封锁pwm,零矢量
 *      把弱磁电压外环改为1个周期调节一次
 *      开关频率改为10K
 *      带载转速：17000rpm
 *      增加了减速斜坡模块
 *      最终停机使用版本(iq<0,封锁PWM。延时通直流量)
 *      中断时间缩短
 *      修改切换方案
 *      主控芯片28335
 *      实验平台MR2
 *      主要改动为EPWM引脚,AD接口,SCI接口
 *      修复斜坡减速的bug
 *      通讯改用浮点数对接，且精简了部分无用变量
 *      调制方式由spwm改为svpwm
 *      实现SPWM、PR模块化
 *      实现反电动势观测器模块化
 *      前向欧拉/改进欧拉离散化观测器完成
 *      SMO、MRAS等加入
 *      ADRC初步完成
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
    //开全局中断
    EINT;//使能全局中断（开中断）（CPU级的）
    ERTM;//使能实时中断（CPU级的）

    ///////////////////////////////////////初始化结束
    while(1)
    {

        Checkdata();
        SciSend();

        sci_recover();
        WrEeprom();

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

