/*
 * fault.c
 *
 *
 */

#include "includes.h"

//�������---����AD�ж��жϽ��й�����������ѹ������Ƿѹ��������




//��������
void CurrentOverFault( )
{
    //U������ж�
	if(ADCresultNEW[3]>vmax_current)//Paramet[current_max]��SCIͨѶ�·�
	{
		//countover_Iu = countover_Iu+1 ;
		//if(countover_Iu>=2)
		//{
			FlagRegs.flagfault.bit.overcurrentA=1;
			SYSTEMoff();
			//countover_Iu = 0 ;
		//}
	}
	//V������ж�
	if(ADCresultNEW[4] > vmax_current)
	{
		//countover_Iv = countover_Iv +1 ;
		//if(countover_Iv>=2)
		//{
			FlagRegs.flagfault.bit.overcurrentB=1;
			SYSTEMoff();
			//countover_Iv = 0;
		//}
	}
    //W������ж�
    if(ADCresultNEW[5] > vmax_current)
    {
        //countover_Iw = countover_Iw +1 ;
        //if(countover_Iw>=2)//��ֹ����
        //{
            FlagRegs.flagfault.bit.overcurrentC=1;
            SYSTEMoff();
            //countover_Iw = 0;
        //}
    }


}

//��ѹ����
void VoltageOverFault( )
{
	//��ѹ�ж�
	if(ADCresultNEW[1]>max_voltage)//��ѹ�ж�
	{
        FlagRegs.flagfault.bit.highvoltage_dc = 1 ;
        FlagRegs.flagsystem.bit.moder_of_stop=1;
        SYSTEMoff();
	}
//	else
//	{
//        FlagRegs.flagfault.bit.highvoltage_dc = 0 ;
//        // moder_of_stop=0;
//	}
}

//Ƿѹ����
void VoltageUnderFault( )
{
	//Ƿѹ�ж�
	if(ADCresultNEW[1]<min_voltage)//��ѹ�ж�Paramet[voltage_min]
	{
	    FlagRegs.flagfault.bit.lowvoltage_dc = 1 ;
	}
	else
	{
		FlagRegs.flagfault.bit.lowvoltage_dc = 0 ;
	}
}

//////���ٹ��ϱ���
void FastFaultTest( )
{

	CurrentOverFault( );//��������
	VoltageOverFault( );//��ѹ����
	VoltageUnderFault( );//Ƿѹ����
	//������ȷ��
	//�ж��Ƿ������ϣ���ֻҪ��������ѹ��Ƿѹ�����������Ӳ�������������е���һ���ϣ���ȷ�Ϲ��Ϸ�����
	if(FlagRegs.flagfault.all>0)
	{
	    FlagRegs.flagsystem.bit.faultoccur = 1 ;//��ʾ�д���������Ҫͣ��
	}
	else
	{
		FlagRegs.flagsystem.bit.faultoccur = 0 ;
	}
}




