/*
 * bsp_xint.c
 *
 *  Created on: 2021年2月16日
 *      Author: sea
 */
#include "bsp.h"

//外部中断，进行过流保护
void ConfigureXint(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO25=0;
    GpioCtrlRegs.GPADIR.bit.GPIO25=0;
    //由于下降沿触发XINT，需要加上拉，但如果外电路有上拉，则可以不用内部上拉
    GpioCtrlRegs.GPAPUD.bit.GPIO25=0;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL=25;//XINT1=gpio25
    EDIS;
// Configure XINT1 Falling edge interrupt
    XIntruptRegs.XINT1CR.bit.POLARITY=0;
// Enable XINT1
    XIntruptRegs.XINT1CR.bit.ENABLE=1;

}
