/*
 *     bsp.c
 *
 *     所有相关的外设驱动初始化配置函数在此文件编写
 *
 *
 */
#include "bsp.h"

void InitBoardGpio(void)
{
    //
    // Initialize GPIOs for 485方向切换
    //
    EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO61=1;

    GpioCtrlRegs.GPAMUX2.bit.GPIO24=0;
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO24=1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO50=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;
    //GpioDataRegs.GPBDAT.bit.GPIO50 = 0;
    //GpioDataRegs.GPBSET.bit.GPIO50=1;//发送使能
    GpioDataRegs.GPBCLEAR.bit.GPIO50=1;//接收使能

    EDIS;
}

void ConfigurePIE(void)
{
    EALLOW;
    PieVectTable.SCIRXINTC = &scirxintab_isr;//SCI中断子程序地址
    PieVectTable.ADCINT=&adca1_interrupt_isr;//捕捉中断子程序地址
    PieVectTable.XINT1=&xint1_isr;//外部中断2018/1/3
    PieVectTable.TINT0=&timer0_isr;
    EDIS;

    //使能CPU中断
    IER |=M_INT8;//SCI中断     开CPU中断1~9(必须开放对应的CPU级中断口)
    IER |=M_INT1;//cputime and adc and外部中断
    //使能PIE中断
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;//使能PIE级Adcint9的中断
    PieCtrlRegs.PIEIER8.bit.INTx5 = 1;//使能PIE级Sci接收中断
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;//使能PIE级外部中断
    PieCtrlRegs.PIEIER1.bit.INTx7=1;
}
