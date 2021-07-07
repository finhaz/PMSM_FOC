/*
 * 文件名：eeprom.c
 * 编写者: 纪辰
 * 功能：eeprom相关的子程序
 */
#include "includes.h"

void ReadEeprom(void)//初始化中，从25LC640中读取修正系数与调试参数44~118
{
    int i;
    for(i=44;i<ParameterNumber;i++)
    {
#ifndef Float_message
        Paramet[i] = Read_From_25LC640(i);
#else
        Paramet[i] = Read_From_25LC640F(i);
#endif
    }
}
void WrEeprom(void)//系统运行过程中，上位机有修改确认键按下时，需要将这一个数据存入对应的存储空间
{
    if(FlagRegs.flagsystem.bit.eeprom_w == 1)//将修正参数和调试参数存入eeprom
    {
#ifndef Float_message
        Write_to_25LC640(SerialNumber, Paramet[SerialNumber]) ;
#else
        Write_to_25LC640F(SerialNumber, Paramet[SerialNumber]);
#endif
        FlagRegs.flagsystem.bit.eeprom_w = 0 ;
    }
}
