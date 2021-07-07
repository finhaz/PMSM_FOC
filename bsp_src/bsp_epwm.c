/*
 * Switch.c
 *
 */


#include "bsp.h"


void ConfigureEPwm(void)
{
    //EPWM1-3 DRIVE

    //Note that the default/2 divider for ePWMs and EMIFs
    //EPWMCLK=SYSCLKOUT=150MHZ
    //ePWMģ��ʱ������  // TBCLK = EPWMCLK/(HSPCLKDIV*CLKDIV)
    //PWM����=15000��TBCLK����100us
    //Uint16 Prd=7500;
    //Uint16 Prd_da=7500;
    // Initialize EPwm1/2/3/4/5/6/7
    //�������ó���------------->>>>>>>>>>>>>
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;
    //EPWM Moudle 1 ����
    //TB
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm1Regs.TBCTL.bit.CLKDIV=0;
    EPwm1Regs.TBPRD=Prd;
    EPwm1Regs.TBPHS.half.TBPHS=0;//����λ�Ĵ���ֵ����
    EPwm1Regs.TBCTR=0x0000;
    EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//����ģʽ
    EPwm1Regs.TBCTL.bit.PHSEN=TB_DISABLE;//��ֹ��λװ��
    EPwm1Regs.TBCTL.bit.PRDLD=TB_SHADOW;//��CTR=0ʱ����ӳ��Ĵ����е�����װ�ص���ǰ�Ĵ���
    EPwm1Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0ʱ����ͬ���ź�
    //CC
    EPwm1Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA�Ĵ��������� ӳ��ģʽ
    EPwm1Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB�Ĵ��������� ӳ��ģʽ
    EPwm1Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
    EPwm1Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
    //AQ
    EPwm1Regs.AQCTLA.bit.CAU=AQ_SET;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵ���������ʹEPWM1AΪ�ߵ�ƽ
    EPwm1Regs.AQCTLA.bit.CAD=AQ_CLEAR;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵݼ�������ʹEPWM1AΪ�͵�ƽ

     //DB
    EPwm1Regs.AQCSFRC.all = 0x0005 ;//���ǿ��PWM���Ϊ��,2017/12/26
    //EPwm1Regs.DBCTL.bit.OUT_MODE=DB_FULL_ENABLE;//ʹ�������ؼ��½�����ʱ�ź�
    EPwm1Regs.DBCTL.bit.IN_MODE=DBA_ALL;//Ĭ��EPWM1A��Ϊ�����ؼ��½�����ʱ���ź�Դ
    EPwm1Regs.DBCTL.bit.POLSEL=DB_ACTV_HIC;//AHC\EPWM1B��ת����
    EPwm1Regs.DBFED=DT;//�½�����ʱFED=DT��TBCLK
    EPwm1Regs.DBRED=DT;//��������ʱRED=DT��TBCLK

    //�¼���������ADC
    EPwm1Regs.ETSEL.bit.SOCAEN  = 1;         // ʹ��SOCAת��������
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;//������Ϊ0ʱ����ת��
    EPwm1Regs.ETPS.bit.SOCAPRD  = ET_1ST; // Generate pulse on 1st event
//    EPwm1Regs.ETSEL.bit.SOCBEN  = 1;         // ʹ��SOCBת��������
//    EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTRD_CMPA;//������Ϊ0ʱ����ת��
//    EPwm1Regs.ETPS.bit.SOCBPRD  = ET_1ST;
    //EPwm1Regs.CMPA.bit.CMPA  = 100;   // Set compare A value
    //EPwm1Regs.TBCTL.bit.CTRMODE=0;

    //EPWM Moudle 2 ����
    //TB
    EPwm2Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm2Regs.TBCTL.bit.CLKDIV=0;
    EPwm2Regs.TBPRD=Prd;
    EPwm2Regs.TBPHS.half.TBPHS=0;//����λ�Ĵ���ֵ����
    EPwm2Regs.TBCTR=0x0000;
    EPwm2Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//����ģʽ
    EPwm2Regs.TBCTL.bit.PHSEN=TB_DISABLE;//��ֹ��λװ��
    EPwm2Regs.TBCTL.bit.PRDLD=TB_SHADOW;//��CTR=0ʱ����ӳ��Ĵ����е�����װ�ص���ǰ�Ĵ���
    EPwm2Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0ʱ����ͬ���ź�
    //CC
    EPwm2Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA�Ĵ��������� ӳ��ģʽ
    EPwm2Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB�Ĵ��������� ӳ��ģʽ
    EPwm2Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
    EPwm2Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
    //AQ
    EPwm2Regs.AQCTLA.bit.CAU=AQ_SET;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵ���������ʹEPWM1AΪ�ߵ�ƽ
    EPwm2Regs.AQCTLA.bit.CAD=AQ_CLEAR;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵݼ�������ʹEPWM1AΪ�͵�ƽ

     //DB
    EPwm2Regs.AQCSFRC.all = 0x0005 ;//���ǿ��PWM���Ϊ��,2017/12/26
    //EPwm1Regs.DBCTL.bit.OUT_MODE=DB_FULL_ENABLE;//ʹ�������ؼ��½�����ʱ�ź�
    EPwm2Regs.DBCTL.bit.IN_MODE=DBA_ALL;//Ĭ��EPWM1A��Ϊ�����ؼ��½�����ʱ���ź�Դ
    EPwm2Regs.DBCTL.bit.POLSEL=DB_ACTV_HIC;//AHC\EPWM1B��ת����
    EPwm2Regs.DBFED=DT;//�½�����ʱFED=DT��TBCLK
    EPwm2Regs.DBRED=DT;//��������ʱRED=DT��TBCLK

    //EPWM Moudle 3 ����
    //TB
    EPwm3Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm3Regs.TBCTL.bit.CLKDIV=0;
    EPwm3Regs.TBPRD=Prd;
    EPwm3Regs.TBPHS.half.TBPHS=0;//����λ�Ĵ���ֵ����
    EPwm3Regs.TBCTR=0x0000;
    EPwm3Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//����ģʽ
    EPwm3Regs.TBCTL.bit.PHSEN=TB_DISABLE;//��ֹ��λװ��
    EPwm3Regs.TBCTL.bit.PRDLD=TB_SHADOW;//��CTR=0ʱ����ӳ��Ĵ����е�����װ�ص���ǰ�Ĵ���
    EPwm3Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0ʱ����ͬ���ź�
    //CC
    EPwm3Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA�Ĵ��������� ӳ��ģʽ
    EPwm3Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB�Ĵ��������� ӳ��ģʽ
    EPwm3Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
    EPwm3Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
    //AQ
    EPwm3Regs.AQCTLA.bit.CAU=AQ_SET;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵ���������ʹEPWMAΪ�ߵ�ƽ
    EPwm3Regs.AQCTLA.bit.CAD=AQ_CLEAR;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵݼ�������ʹEPWMAΪ�͵�ƽ

     //DB
    EPwm3Regs.AQCSFRC.all = 0x0005 ;//���ǿ��PWM���Ϊ��,2017/12/26
    //EPwm3Regs.DBCTL.bit.OUT_MODE=DB_FULL_ENABLE;//ʹ�������ؼ��½�����ʱ�ź�
    EPwm3Regs.DBCTL.bit.IN_MODE=DBA_ALL;//Ĭ��EPWMA��Ϊ�����ؼ��½�����ʱ���ź�Դ
    EPwm3Regs.DBCTL.bit.POLSEL=DB_ACTV_HIC;//AHC\EPWM1B��ת����
    EPwm3Regs.DBFED=DT;//�½�����ʱFED=DT��TBCLK
    EPwm3Regs.DBRED=DT;//��������ʱRED=DT��TBCLK


//    //EPWM7-8 DAC
//    //EPWM Moudle 7 ����
//    //TB
//    EPwm7Regs.TBCTL.bit.HSPCLKDIV=0;
//    EPwm7Regs.TBCTL.bit.CLKDIV=0;
//    EPwm7Regs.TBPRD=Prd_da;
//    EPwm7Regs.TBPHS.bit.TBPHS=0;//����λ�Ĵ���ֵ����
//    EPwm7Regs.TBCTR=0x0000;
//    EPwm7Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//����ģʽ
//    EPwm7Regs.TBCTL.bit.PHSEN=TB_DISABLE;//��ֹ��λװ��
//    EPwm7Regs.TBCTL.bit.PRDLD=TB_SHADOW;//��CTR=0ʱ����ӳ��Ĵ����е�����װ�ص���ǰ�Ĵ���
//    EPwm7Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0ʱ����ͬ���ź�
//    //CC
//    EPwm7Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA�Ĵ��������� ӳ��ģʽ
//    EPwm7Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB�Ĵ��������� ӳ��ģʽ
//    EPwm7Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
//    EPwm7Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
//    //AQ
//    EPwm7Regs.AQCTLA.bit.CAU=AQ_SET;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵ���������ʹEPWMAΪ�ߵ�ƽ
//    EPwm7Regs.AQCTLA.bit.CAD=AQ_CLEAR;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵݼ�������ʹEPWMAΪ�͵�ƽ
//    EPwm7Regs.AQCTLB.bit.CAU=AQ_SET;
//    EPwm7Regs.AQCTLB.bit.CAD=AQ_CLEAR;
//     //DB
//    //���ǿ��PWM���Ϊ��
//    EPwm7Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
//    EPwm7Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
//    EPwm7Regs.DBCTL.bit.OUT_MODE=DB_DISABLE;//��ֹ����ģ��
//
//
//    //EPWM Moudle 8 ����
//    //TB
//    EPwm8Regs.TBCTL.bit.HSPCLKDIV=0;
//    EPwm8Regs.TBCTL.bit.CLKDIV=0;
//    EPwm8Regs.TBPRD=Prd_da;
//    EPwm8Regs.TBPHS.bit.TBPHS=0;//����λ�Ĵ���ֵ����
//    EPwm8Regs.TBCTR=0x0000;
//    EPwm8Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//����ģʽ
//    EPwm8Regs.TBCTL.bit.PHSEN=TB_DISABLE;//��ֹ��λװ��
//    EPwm8Regs.TBCTL.bit.PRDLD=TB_SHADOW;//��CTR=0ʱ����ӳ��Ĵ����е�����װ�ص���ǰ�Ĵ���
//    EPwm8Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0ʱ����ͬ���ź�
//    //CC
//    EPwm8Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA�Ĵ��������� ӳ��ģʽ
//    EPwm8Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB�Ĵ��������� ӳ��ģʽ
//    EPwm8Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
//    EPwm8Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//��CTR=0ʱװ��
//    //AQ
//    EPwm8Regs.AQCTLA.bit.CAU=AQ_SET;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵ���������ʹEPWMAΪ�ߵ�ƽ
//    EPwm8Regs.AQCTLA.bit.CAD=AQ_CLEAR;//��ʱ���׼��������ֵ����CMPA��ֵ�������ڵݼ�������ʹEPWMAΪ�͵�ƽ
//    EPwm8Regs.AQCTLB.bit.CAU=AQ_SET;
//    EPwm8Regs.AQCTLB.bit.CAD=AQ_CLEAR;
//     //DB
//    //���ǿ��PWM���Ϊ��
//    EPwm8Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
//    EPwm8Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
//    EPwm8Regs.DBCTL.bit.OUT_MODE=DB_DISABLE;//ʹ�������ؼ��½�����ʱ�ź�




    //TBD
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

//PWM����
void PWMoff(void)
{
    //ePWM1����
    EPwm1Regs.AQSFRC.bit.RLDCSF = 3 ;//ѡ��ֱ�Ӽ���
    EPwm1Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM2����
    EPwm2Regs.AQSFRC.bit.RLDCSF = 3 ;//ѡ��ֱ�Ӽ���
    EPwm2Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM3����
    EPwm3Regs.AQSFRC.bit.RLDCSF = 3 ;//ѡ��ֱ�Ӽ���
    EPwm3Regs.DBCTL.bit.OUT_MODE=0;

    //���ǿ��PWM���Ϊ��
    EPwm1Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm1Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;

}

//��ʸ��,��·���ű�
void PWMzero(void)
{
    //ePWM1����
    EPwm1Regs.AQSFRC.bit.RLDCSF = 3 ;//ѡ��ֱ�Ӽ���
    EPwm1Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM2����
    EPwm2Regs.AQSFRC.bit.RLDCSF = 3 ;//ѡ��ֱ�Ӽ���
    EPwm2Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM3����
    EPwm3Regs.AQSFRC.bit.RLDCSF = 3 ;//ѡ��ֱ�Ӽ���
    EPwm3Regs.DBCTL.bit.OUT_MODE=0;

    //���ǿ��PWMA���Ϊ��,PWMB���Ϊ��
    EPwm1Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm1Regs.AQCSFRC.bit.CSFB=AQ_SET;
    EPwm2Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFB=AQ_SET;
    EPwm3Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFB=AQ_SET;
}

//��PWM
void PWMopen(void)
{

    //ePWM1����
    EPwm1Regs.AQSFRC.bit.RLDCSF = 0;//
    EPwm1Regs.AQCSFRC.all = 0x0000 ;//�޶���
    EPwm1Regs.DBCTL.bit.OUT_MODE=3;

    //ePWM2����
    EPwm2Regs.AQSFRC.bit.RLDCSF = 0 ;//
    EPwm2Regs.AQCSFRC.all = 0x0000 ;//�޶���
    EPwm2Regs.DBCTL.bit.OUT_MODE=3;

    //ePWM3����
    EPwm3Regs.AQSFRC.bit.RLDCSF = 0 ;//
    EPwm3Regs.AQCSFRC.all = 0x0000 ;//�޶���
    EPwm3Regs.DBCTL.bit.OUT_MODE=3;

}









