/*
 * bsp_adc.c
 *
 *  Created on: 2021年2月16日
 *      Author: sea
 */
#include "bsp.h"

void ConfigureADC(void)
{

    EALLOW;
    SysCtrlRegs.HISPCP.all = 0x3;//HSPCLK = SYSCLKOUT/2*HISPCP = 150/(2*3)   = 25.0 MHz
    EDIS;
    InitAdc();  // For this example, init the ADC

   //CHANNEL
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 0x1;//0-dual, 1-Cascaded
    AdcRegs.ADCMAXCONV.all = 0x000F;       // Setup 16 conv's on SEQ
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // Setup ADCINA0 as 1st SEQ conv.
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // Setup ADCINA1 as 2nd SEQ conv.
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2; // Setup ADCINA2 as 3rd SEQ conv.
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3; // Setup ADCINA3 as 4th SEQ conv.
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4; // Setup ADCINA4 as 5th SEQ conv.
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5; // Setup ADCINA5 as 6th SEQ conv.
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x6; // Setup ADCINA6 as 7th SEQ conv.
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x7; // Setup ADCINA7 as 8th SEQ conv.
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0x8; // Setup ADCINB0 as 9th SEQ conv.
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0x9; // Setup ADCINB1 as 10th SEQ conv.
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0xA; // Setup ADCINB2 as 11th SEQ conv.
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 0xB; // Setup ADCINB3 as 12th SEQ conv.
    AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 0xC; // Setup ADCINB4 as 13th SEQ conv.
    AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 0xD; // Setup ADCINB5 as 14th SEQ conv.
    AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 0xE; // Setup ADCINB6 as 15th SEQ conv.
    AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 0xF; // Setup ADCINB7 as 16th SEQ conv.

//    AdcRegs.ADCTRL1.bit.ACQ_PS=1;
    AdcRegs.ADCTRL1.bit.ACQ_PS=7;

    //触发源设置
//    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;//软触发（用于级联、双通道1#）
//    AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1;//软触发（用于双通道2#）
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;//ePWM触发（用于双通道1#）
//    AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1;//ePWM触发（用于双通道2#）
//    AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ = 1;//ePWM触发（用于级联）
    //中断源设置
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1; //用于级联、双通道1#
//    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1; //双通道2#
}
