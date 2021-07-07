/*
 * bsp_xint.c
 *
 *  Created on: 2021��2��16��
 *      Author: sea
 */
#include "bsp.h"

//�ⲿ�жϣ����й�������
void ConfigureXint(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO25=0;
    GpioCtrlRegs.GPADIR.bit.GPIO25=0;
    //�����½��ش���XINT����Ҫ����������������·������������Բ����ڲ�����
    GpioCtrlRegs.GPAPUD.bit.GPIO25=0;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL=25;//XINT1=gpio25
    EDIS;
// Configure XINT1 Falling edge interrupt
    XIntruptRegs.XINT1CR.bit.POLARITY=0;
// Enable XINT1
    XIntruptRegs.XINT1CR.bit.ENABLE=1;

}
