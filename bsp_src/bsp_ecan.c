/*
 * bsp_ecan.c
 *
 *  Created on: 2020年12月30日
 *      Author: sea
 */
#include "bsp.h"     // DSP28x Headerfile

void ConfigureEcan(void)
{
    struct ECAN_REGS ECanbShadow;
    // Step 4. Initialize all the Device Peripherals:
    // This function is found in DSP2833x_InitPeripherals.c
    // InitPeripherals(); // Not required for this example

    // In this case just initalize eCAN-A and eCAN-B
    // This function is in DSP2833x_ECan.c
    InitECan();
    //ErrorCount = 0;
    //PassCount = 0;

    /* Write to the MSGID field  */

    ECanbMboxes.MBOX16.MSGID.all = 0x95555554; // Extended Identifier
    ECanbMboxes.MBOX25.MSGID.all = 0x95555555; // Extended Identifier

    /* Configure Mailbox under test as a Re/Transmit mailbox */
    EALLOW;
    ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
    ECanbShadow.CANMD.bit.MD16 = 1; //rx
    ECanbShadow.CANMD.bit.MD25 = 0;//tx
    ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;


    //without considering id and the data can be received
    ECanbMboxes.MBOX16.MSGID.bit.AME = 1;
    ECanbLAMRegs.LAM16.all = 0xFFFFFFFF;

    /* Enable Mailbox under test */

    ECanbShadow.CANME.all = ECanbRegs.CANME.all;
    ECanbShadow.CANME.bit.ME16 = 1;
    ECanbShadow.CANME.bit.ME25 = 1;
    ECanbRegs.CANME.all = ECanbShadow.CANME.all;


    /* Write to DLC field in Master Control reg */
    //ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX25.MSGCTRL.bit.DLC = 8;



    ECanbRegs.CANRMP.all = 0xFFFFFC00;

    /* Write to the mailbox RAM field */

//    ECanaMboxes.MBOX16.MDL.all = 0x555;
//    ECanaMboxes.MBOX16.MDH.all = 0x555;

    ECanbMboxes.MBOX25.MDL.all = 0x55555555;
    ECanbMboxes.MBOX25.MDH.all = 0xAAAAAAAA;

    EDIS;

    EALLOW;

    ECanbShadow.CANMIM.all=ECanbRegs.CANMIM.all;
    ECanbShadow.CANMIM.bit.MIM16=1;
    ECanbRegs.CANMIM.all=ECanbShadow.CANMIM.all;

    ECanbShadow.CANMIL.all = ECanbRegs.CANMIL.all;
    ECanbShadow.CANMIL.all = 0;              // 1-32号邮箱中断在中断线0上产生
    ECanbRegs.CANMIL.all = ECanbShadow.CANMIL.all;

    ECanbShadow.CANGIM.all = ECanbRegs.CANGIM.all;
    ECanbShadow.CANGIM.bit.I0EN = 1 ;              //  中断线0使能
    ECanbRegs.CANGIM.all = ECanbShadow.CANGIM.all;

    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    ECanbShadow.CANMC.bit.STM = 0;    // 0-Normal
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    EDIS;

}
