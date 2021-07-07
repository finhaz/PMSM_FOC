/*
 * bsp_eqep.c
 *
 *  Created on: 2021年4月13日
 *      Author: sea
 */

#include "bsp.h"

void ConfigureEQep(void)
{
    EQep1Regs.QUPRD=75000;//150M主频，分频使得Eqep的单位定时器周期为150M/QUPRD
    EQep1Regs.QDECCTL.bit.QSRC = 0;//正交计数模式

    EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;//仿真挂起对于QEP的相关计数模块不影响
    EQep1Regs.QEPCTL.bit.PCRM=00;       // PCRM=00 mode - QPOSCNT reset on index event(1I端口使能，发生索引事件)
    EQep1Regs.QEPCTL.bit.UTE=1;         // Unit Timeout Enable，使能单位定时器
    EQep1Regs.QEPCTL.bit.QCLM=1;        // Latch on unit time out，定时器超时时锁存
    EQep1Regs.QPOSMAX=QPO_Pulse;      //最大脉冲数
    EQep1Regs.QEPCTL.bit.QPEN=1;        // QEP enable

    EQep1Regs.QCAPCTL.bit.UPPS=5;       // QEP的单位定时器，Upevnt=Qclk/2^5，它可以计数得到间隔时间
    EQep1Regs.QCAPCTL.bit.CCPS=7;       // QEP的CAP clock=Sysclk/2^7
    EQep1Regs.QCAPCTL.bit.CEN=1;        // QEP Capture Enable,用于捕捉两个脉冲间隔时间
}
