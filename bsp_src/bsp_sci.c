/*
 * bsp_sci.c
 *
 *  Created on: 2021��2��16��
 *      Author: sea
 */
#include "bsp.h"

//#define CPU_FREQ           150E6
//#define LSPCLK_FREQ     CPU_FREQ/4
//#define SCI_FREQ        9600
//#define SCI_PRD         ((LSPCLK_FREQ/(SCI_FREQ*8))-1)
//BRR =SCI_PRD;



void ConfigureSci(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.SCICENCLK= 1;
    //SciaRegs.SCIFFTX.all=0xE040;
    //SciaRegs.SCIFFRX.all=0x2044;
    //SciaRegs.SCIFFCT.all=0x0;

    ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScicRegs.SCICTL2.all =0x0003;
    ScicRegs.SCICTL2.bit.TXINTENA =1;
    ScicRegs.SCICTL2.bit.RXBKINTENA =1;

    ScicRegs.SCIHBAUD=0x0001;  // 9600 baud @LSPCLK = 37.5MHz
                                         //(150 MHz SYSCLK).
    ScicRegs.SCILBAUD =0x00E7;

    ScicRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
    EDIS;

}

void sci_sendbyte(Uint16 byte)
{
    ScicRegs.SCITXBUF = byte;
    while(ScicRegs.SCICTL2.bit.TXRDY==0){}
}


void sci_recover(void)
{
    if(ScicRegs.SCIRXST.bit.FE == 1)//��֡����,��λSCI��ʹ��ͨѶ�ָ�����
    {
        ScicRegs.SCICTL1.bit.SWRESET=0;//�������λ����λSCI
        DELAY_US(2);//�Ե�һ��ʱ�䣬DSP�ܿ����������λ
        ScicRegs.SCICTL1.bit.SWRESET=1;//�ָ�SCIԭ��״̬
    }
}
