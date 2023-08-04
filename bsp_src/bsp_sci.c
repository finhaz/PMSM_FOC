/*
 * bsp_sci.c
 *
 *  Created on: 2021年2月16日
 *      Author: sea
 */
#include "bsp.h"

//#define CPU_FREQ           150E6
//#define LSPCLK_FREQ     CPU_FREQ/4
//#define SCI_FREQ        9600
//#define SCI_PRD         ((LSPCLK_FREQ/(SCI_FREQ*8))-1)
//BRR =SCI_PRD;

void InitSciGpio_a(void)
{
    EALLOW;

    //
    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;  // Enable pull-up for GPIO36 (SCIRXDA)
    GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;  // Enable pull-up for GPIO35 (SCITXDA)

    //
    // Set qualification for selected pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.
    //
    GpioCtrlRegs.GPBQSEL1.bit.GPIO36 = 3;  // Asynch input GPIO36 (SCIRXDA)

    //
    // Configure SCI-A pins using GPIO regs
    // This specifies which of the possible GPIO pins will be SCI functional
    // pins.
    //
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 1;   // Configure GPIO36 to SCIRXDA
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 1;   // Configure GPIO35 to SCITXDA


    EDIS;
}

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
    if(ScicRegs.SCIRXST.bit.FE == 1)//有帧错误,复位SCI，使得通讯恢复正常
    {
        ScicRegs.SCICTL1.bit.SWRESET=0;//清零故障位，复位SCI
        DELAY_US(2);//稍等一段时间，DSP很快便可清零故障位
        ScicRegs.SCICTL1.bit.SWRESET=1;//恢复SCI原有状态
    }
}
