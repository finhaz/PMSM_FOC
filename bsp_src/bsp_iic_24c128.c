/*
 * bsp_iic_24c128.c
 *
 *  Created on: 2021年9月8日
 *      Author: sea
 */


#include "includes.h"

unsigned char edata[8];
//---------------------------------------------------------------------------
// InitI2C:
//---------------------------------------------------------------------------
// This function initializes the I2C to a known state.
//
void ConfigI2C(void)
{
    // Initialize I2C-A:
   // Initialize I2C
//   InitI2CGpio();
   I2caRegs.I2CMDR.bit.IRS =0;
  // I2caRegs.I2CSAR = 0x0050;      // Slave address - EEPROM control code
   I2caRegs.I2CPSC.all = 9;         // Prescaler - need 7-12 Mhz on module clk
   I2caRegs.I2CCLKL = 15;           // NOTE: must be non zero
   I2caRegs.I2CCLKH = 15;           // NOTE: must be non zero
   I2caRegs.I2CIER.all = 0x24;      // Enable SCD & ARDY interrupts
   I2caRegs.I2CFFTX.all = 0x0000;
   I2caRegs.I2CMDR.bit.IRS = 1; // Take I2C out of reset

    //tbd...
}


Uint16 I2CA_WriteData(Uint16 slaveaddr,Uint16 addr,Uint16 len,unsigned char * str)
{
   Uint16 i;

   if (I2caRegs.I2CMDR.bit.STP == 1)
   {
      return I2C_STP_NOT_READY_ERROR;
   }

   // Setup slave address
   I2caRegs.I2CSAR = slaveaddr;

   // Check if bus busy
   if (I2caRegs.I2CSTR.bit.BB == 1)
   {
      return I2C_BUS_BUSY_ERROR;
   }

   // Setup number of bytes to send
   // MsgBuffer + Address
   I2caRegs.I2CCNT = len+2;

   // Setup data to send
   I2caRegs.I2CDXR = addr>>8;
   I2caRegs.I2CMDR.all = 0x6E20;
   while(!I2C_xrdy());
   I2caRegs.I2CDXR = addr & 0xff;

   for (i=0; i<len; i++)
   {
      while(!I2C_xrdy());
      I2caRegs.I2CDXR = *(str+i);
      if (I2caRegs.I2CSTR.bit.NACK == 1)
      return    I2C_BUS_BUSY_ERROR;
   }

   // Send start as master transmitter

   return I2C_SUCCESS;
}
//////////////////////////////////////
Uint16 Writetoeepromstr(Uint16 addr,Uint16 len,unsigned char * str)
{
    Uint16 i=0,j=0;
    j=len/8;
    if(j==0)
    {
        for(i=0;i<len;i++)
        {
            I2CA_WriteData(0x50,addr+i,len,str+i);
            DELAY_US(5000);//必须等待5MS，EEPROM规定连续写间隔
        }
    }
    else
    {
        for(i=0;i<j;i++)
        {
            I2CA_WriteData(0x50,addr+i*8,8,str+i*8);
            DELAY_US(5000);//必须等待5MS，EEPROM规定连续写间隔
        }
    }
    return 1;
}

Uint16 I2CA_ReadData(Uint16 slaveaddr,Uint16 addr,Uint16 len,unsigned char * str)
{
   Uint16 i;
   if (I2caRegs.I2CMDR.bit.STP == 1)
   {
      return I2C_STP_NOT_READY_ERROR;
   }

   I2caRegs.I2CSAR = slaveaddr;

   if (I2caRegs.I2CSTR.bit.BB == 1)
    {
         return I2C_BUS_BUSY_ERROR;
    }
      I2caRegs.I2CCNT = 2;
      I2caRegs.I2CDXR = addr >>8;
      I2caRegs.I2CMDR.all = 0x6620;         // Send data to setup RTC address
      while(!I2C_xrdy());
      I2caRegs.I2CDXR = addr&0xff;
   if (I2caRegs.I2CSTR.bit.NACK == 1)
   return   I2C_BUS_BUSY_ERROR;
   while(!I2C_xrdy());

   I2caRegs.I2CSAR = slaveaddr;
   I2caRegs.I2CCNT = len;
   I2caRegs.I2CMDR.all = 0x6C20;
   if (I2caRegs.I2CSTR.bit.NACK == 1)
        return  I2C_BUS_BUSY_ERROR;
   for(i=0;i<len;i++)
   {
      while(!I2C_rrdy());
      *(str + i) = I2caRegs.I2CDRR;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
          return    I2C_BUS_BUSY_ERROR;
   }
   DELAY_US(200);
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

void ReadfromeepromStr(Uint16 addr,Uint16 len,unsigned char * str)
{
 Uint16 i=0,j=0;
 j=len/8;
 if(j==0)
 {
 for(i=0;i<len;i++)
 I2CA_ReadData(0x50,addr+i,8,str+i);
 }
 else
 {
 for(i=0;i<j;i++)
 {
  I2CA_ReadData(0x50,addr+i*8,8,str+i*8);
   DELAY_US(20);
  }
 }

}


float Read_From_24C128F(unsigned int Addr_Num)//24C128读函数,读双字
{
    union FLOAT_COMF  GData;


    ReadfromeepromStr((Addr_Num-44)*8,8,edata);

    GData.bit.MEM1=edata[0];
    GData.bit.MEM2=edata[1];
    GData.bit.MEM3=edata[2];
    GData.bit.MEM4=edata[3];

    return GData.all;
}

void Write_to_24C128F(unsigned int Addr_Num,float Data)//24C128写函数,写双字
{
    union FLOAT_COMF  WData;

    WData.all=Data;
    edata[0]=WData.bit.MEM1;
    edata[1]=WData.bit.MEM2;
    edata[2]=WData.bit.MEM3;
    edata[3]=WData.bit.MEM4;
    Writetoeepromstr((Addr_Num-44)*8,8,edata);//一次需要写入的字节个数建议采用8的倍数的cnt，因为本函数不采用自动分页，不是8的倍数会在超出页写入最多的字节数之后覆盖掉原来的数

}

//===========================================================================
// End of file.
//===========================================================================
