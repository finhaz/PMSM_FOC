/*
 * fault.c
 *
 *
 */

#include "includes.h"

//软件保护---用于AD中断判断进行过流保护，过压保护，欠压保护策略




//过流保护
void CurrentOverFault( )
{
    //U相过流判断
	if(ADCresultNEW[3]>vmax_current)//Paramet[current_max]由SCI通讯下发
	{
		//countover_Iu = countover_Iu+1 ;
		//if(countover_Iu>=2)
		//{
			FlagRegs.flagfault.bit.overcurrentA=1;
			SYSTEMoff();
			//countover_Iu = 0 ;
		//}
	}
	//V相过流判断
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
    //W相过流判断
    if(ADCresultNEW[5] > vmax_current)
    {
        //countover_Iw = countover_Iw +1 ;
        //if(countover_Iw>=2)//防止误判
        //{
            FlagRegs.flagfault.bit.overcurrentC=1;
            SYSTEMoff();
            //countover_Iw = 0;
        //}
    }


}

//过压保护
void VoltageOverFault( )
{
	//过压判断
	if(ADCresultNEW[1]>max_voltage)//过压判断
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

//欠压保护
void VoltageUnderFault( )
{
	//欠压判断
	if(ADCresultNEW[1]<min_voltage)//低压判断Paramet[voltage_min]
	{
	    FlagRegs.flagfault.bit.lowvoltage_dc = 1 ;
	}
	else
	{
		FlagRegs.flagfault.bit.lowvoltage_dc = 0 ;
	}
}

//////快速故障保护
void FastFaultTest( )
{

	CurrentOverFault( );//过流保护
	VoltageOverFault( );//过压保护
	VoltageUnderFault( );//欠压保护
	//故障码确认
	//判断是否发生故障，即只要发生：过压，欠压，软件过流，硬件过流，过载中的任一故障，即确认故障发生。
	if(FlagRegs.flagfault.all>0)
	{
	    FlagRegs.flagsystem.bit.faultoccur = 1 ;//表示有错误发生，需要停机
	}
	else
	{
		FlagRegs.flagsystem.bit.faultoccur = 0 ;
	}
}




