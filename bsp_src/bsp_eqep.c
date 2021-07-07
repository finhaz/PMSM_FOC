/*
 * bsp_eqep.c
 *
 *  Created on: 2021��4��13��
 *      Author: sea
 */

#include "bsp.h"

void ConfigureEQep(void)
{
    EQep1Regs.QUPRD=75000;//150M��Ƶ����Ƶʹ��Eqep�ĵ�λ��ʱ������Ϊ150M/QUPRD
    EQep1Regs.QDECCTL.bit.QSRC = 0;//��������ģʽ

    EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;//����������QEP����ؼ���ģ�鲻Ӱ��
    EQep1Regs.QEPCTL.bit.PCRM=00;       // PCRM=00 mode - QPOSCNT reset on index event(1I�˿�ʹ�ܣ����������¼�)
    EQep1Regs.QEPCTL.bit.UTE=1;         // Unit Timeout Enable��ʹ�ܵ�λ��ʱ��
    EQep1Regs.QEPCTL.bit.QCLM=1;        // Latch on unit time out����ʱ����ʱʱ����
    EQep1Regs.QPOSMAX=QPO_Pulse;      //���������
    EQep1Regs.QEPCTL.bit.QPEN=1;        // QEP enable

    EQep1Regs.QCAPCTL.bit.UPPS=5;       // QEP�ĵ�λ��ʱ����Upevnt=Qclk/2^5�������Լ����õ����ʱ��
    EQep1Regs.QCAPCTL.bit.CCPS=7;       // QEP��CAP clock=Sysclk/2^7
    EQep1Regs.QCAPCTL.bit.CEN=1;        // QEP Capture Enable,���ڲ�׽����������ʱ��
}
