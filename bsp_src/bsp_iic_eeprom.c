/*
 * bsp_iic_eeprom.c
 *
 *  Created on: 2021年9月4日
 *      Author: sea
 */

#include "bsp.h"
#include "message_define.h"

void ConfigI2C(void)
{
    // Initialize I2C-A:
   // Initialize I2C
   I2caRegs.I2CMDR.bit.IRS =0;
   I2caRegs.I2CSAR = 0x0050;        // Slave address - EEPROM control code
   I2caRegs.I2CPSC.all = 9;         // Prescaler - need 7-12 Mhz on module clk
   I2caRegs.I2CCLKL = 15;           // NOTE: must be non zero
   I2caRegs.I2CCLKH = 15;           // NOTE: must be non zero
   I2caRegs.I2CIER.all = 0x00;      // Enable SCD & ARDY interrupts

   I2caRegs.I2CMDR.bit.IRS = 1; // Take I2C out of reset

    //tbd...
}

Uint16 WriteData(Uint16 *Wdata, Uint16  RomAddress, Uint16  number)
{
   Uint16 i;
   if (I2caRegs.I2CSTR.bit.BB == 1)
   {
      return I2C_BUS_BUSY_ERROR;
   }
   while(!I2C_xrdy());
   I2caRegs.I2CSAR = 0x50;
   I2caRegs.I2CCNT = number + 1;
   I2caRegs.I2CDXR = RomAddress;
   I2caRegs.I2CMDR.all = 0x6E20;
   for (i=0; i<number; i++)
   {
      while(!I2C_xrdy());
      I2caRegs.I2CDXR = *Wdata;
      Wdata++;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
          return    I2C_BUS_BUSY_ERROR;
   }
   return I2C_SUCCESS;
}

Uint16 ReadData(Uint16  *RamAddr, Uint16    RomAddress, Uint16 number)
{
   Uint16  i,Temp;

   if (I2caRegs.I2CSTR.bit.BB == 1)
   {
       return I2C_BUS_BUSY_ERROR;
   }
   while(!I2C_xrdy());
   I2caRegs.I2CSAR = 0x50;
   I2caRegs.I2CCNT = 1;
   I2caRegs.I2CDXR = RomAddress;
   I2caRegs.I2CMDR.all = 0x6620;
   if (I2caRegs.I2CSTR.bit.NACK == 1)
        return  I2C_BUS_BUSY_ERROR;
   DELAY_US(50);
   while(!I2C_xrdy());
   I2caRegs.I2CSAR = 0x50;
   I2caRegs.I2CCNT = number;
   I2caRegs.I2CMDR.all = 0x6C20;
   if (I2caRegs.I2CSTR.bit.NACK == 1)
        return  I2C_BUS_BUSY_ERROR;
   for(i=0;i<number;i++)
   {
      while(!I2C_rrdy());
      Temp = I2caRegs.I2CDRR;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
          return    I2C_BUS_BUSY_ERROR;
      *RamAddr = Temp;
      RamAddr++;
   }
   return I2C_SUCCESS;
}


Uint16  I2C_xrdy(void)
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;
    return t;
}

Uint16  I2C_rrdy(void)
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;
    return t;
}

float Read_From_24C01F(unsigned int Addr_Num)//24C01读函数,读双字
{
    union FLOAT_COMF  Data;
    Uint16 data[4];

    if(Addr_Num<76)//空间有限，就128字节
    {
        ReadData(data,(Addr_Num-44)*4,4);
        Data.bit.MEM1=data[0];
        Data.bit.MEM2=data[1];
        Data.bit.MEM3=data[2];
        Data.bit.MEM4=data[3];
    }


    return Data.all;
}

void Write_to_24C01F(unsigned int Addr_Num,float Data)//24C01写函数,写双字
{
    union FLOAT_COMF  WData;
    Uint16 data[4];
    if(Addr_Num<76)//空间有限，就128字节
    {
        WData.all=Data;
        data[0]=WData.bit.MEM1;
        data[1]=WData.bit.MEM2;
        data[2]=WData.bit.MEM3;
        data[3]=WData.bit.MEM4;
        WriteData(data,(Addr_Num-44)*4,4);
    }
}

