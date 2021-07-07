/*
 *     bsp.c
 *
 *     ������ص�����������ʼ�����ú����ڴ��ļ���д
 *
 *
 */
#include "bsp.h"

void InitBoardGpio(void)
{
    //
    // Initialize GPIOs for 485�����л�
    //
    EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO61=1;



    EDIS;
}

void ConfigurePIE(void)
{
    EALLOW;
    PieVectTable.SCIRXINTC = &scirxintab_isr;//SCI�ж��ӳ����ַ
    PieVectTable.ADCINT=&adca1_interrupt_isr;//��׽�ж��ӳ����ַ
    PieVectTable.XINT1=&xint1_isr;//�ⲿ�ж�2018/1/3
    EDIS;

    //ʹ��CPU�ж�
    IER |=M_INT8;//SCI�ж�     ��CPU�ж�1~9(���뿪�Ŷ�Ӧ��CPU���жϿ�)
    IER |=M_INT1;//cputime and adc and�ⲿ�ж�
    //ʹ��PIE�ж�
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;//ʹ��PIE��Adcint9���ж�
    PieCtrlRegs.PIEIER8.bit.INTx5 = 1;//ʹ��PIE��Sci�����ж�
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;//ʹ��PIE���ⲿ�ж�
}