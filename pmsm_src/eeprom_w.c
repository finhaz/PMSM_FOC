/*
 * �ļ�����eeprom.c
 * ��д��: �ͳ�
 * ���ܣ�eeprom��ص��ӳ���
 */
#include "includes.h"


void ReadEeprom(void)//��ʼ���У���25LC640�ж�ȡ����ϵ������Բ���44~118
{

    int i;

    for(i=44;i<ParameterNumber;i++)
//    for(i=44;i<76;i++)
    {
//#ifndef Float_message
//        Paramet[i] = Read_From_25LC640(i);
//#else
//        Paramet[i] = Read_From_25LC640F(i);
//#endif

        Paramet[i]=Read_From_24C128F(i);

        DELAY_US(100);
    }


}
void WrEeprom(void)//ϵͳ���й����У���λ�����޸�ȷ�ϼ�����ʱ����Ҫ����һ�����ݴ����Ӧ�Ĵ洢�ռ�
{
    if(FlagRegs.flagsystem.bit.eeprom_w == 1)//�����������͵��Բ�������eeprom
    {
//#ifndef Float_message
//        Write_to_25LC640(SerialNumber, Paramet[SerialNumber]) ;
//#else
//        Write_to_25LC640F(SerialNumber, Paramet[SerialNumber]);
//#endif

        Write_to_24C128F(SerialNumber, Paramet[SerialNumber]);
        FlagRegs.flagsystem.bit.eeprom_w = 0 ;
    }
}
