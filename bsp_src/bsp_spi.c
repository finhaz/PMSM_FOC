/*
 * bsp_spi.c
 *
 *  Created on: 2021年1月19日
 *      Author: sea
 */

#include "bsp.h"
#include "message_define.h"
#define Float_message 1

//FM25CL64的操作码定义
//spi虽然配置为8bit收发，但是dsp是16位收发
//而且dsp在发数据时候是高位数据有效，收数据是低位数据有效。
#define Read_CMD 0x0300        //读25LC640命令0000 0011
#define Write_CMD 0x0200       //写25LC640命令0000 0010
#define Wren_CMD 0x0600        //写25LC640使能命令0000 0110
#define Wrdi_CMD 0x0400        //复位 写25LC640使能命令0000 0100(禁止写操作)
#define Rdsr_CMD 0x0500        //读25LC640状态寄存器命令0000 0101（读状态，看有没有读取结束）
#define Wrsr_CMD 0x0100        //写25LC640状态寄存器命令0000 0001（写状态，看有没有写结束）
#define State 0x0200           //写入25LC640状态寄存器的数据
#define CS_Set GpioDataRegs.GPASET.bit.GPIO19 = 1        //GPIO19引脚输出为1
#define CS_Clear GpioDataRegs.GPACLEAR.bit.GPIO19 = 1    //GPIO19引脚输出为0




/*****************************************SPI初始化函数********************************************************/
void ConfigureSpi(void)
{
    EALLOW;


    // Set reset low before configuration changesSPI复位
    // Clock polarity (0 == rising, 1 == falling)设置时钟为上升沿
    // 8-bit character8位字符长度
    // Disable loop-back
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpiaRegs.SPICCR.bit.SPICHAR = 7;
    SpiaRegs.SPICCR.bit.SPILBK = 0;
    //SpiaRegs.SPICCR.all=0x0007;

    // 使能主模式，标准相位// 使能Talk,SPIINT屏蔽
    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 1;
    SpiaRegs.SPICTL.bit.SPIINTENA = 0;
    //SpiaRegs.SPICTL.all=0x0006;

    SpiaRegs.SPIBRR=0x7F;     //设置波特率,SPIBRR>=3,LSP/(SPIBRR+1);SPIBRR=0,1,2,LSP/4
    //SpiaRegs.SPICCR.all=0x0087;  //使SPI退出复位状态//使能自测试模式

    SpiaRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;

    EDIS;



}

void Initspigpio(void)
{
    EALLOW;

     //
     // Enable internal pull-up for the selected pins
     //
     // Pull-ups can be enabled or disabled by the user.
     // This will enable the pullups for the specified pins.
     // Comment out other unwanted lines.
     //
     GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;  // Enable pull-up on GPIO16 (SPISIMOA)
 //  GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   // Enable pull-up on GPIO5 (SPISIMOA)
     GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;  // Enable pull-up on GPIO17 (SPISOMIA)
 //  GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   // Enable pull-up on GPIO3 (SPISOMIA)
     GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up on GPIO18 (SPICLKA)

     //
     // Set qualification for selected pins to asynch only
     //
     // This will select asynch (no qualification) for the selected pins.
     // Comment out other unwanted lines.
     //
     GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
 //  GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;  // Asynch input GPIO5 (SPISIMOA)
     GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
 //  GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;  // Asynch input GPIO3 (SPISOMIA)
     GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)

     //
     //Configure SPI-A pins using GPIO regs
     //
     // This specifies which of the possible GPIO pins will be SPI functional
     // pins.
     // Comment out other unwanted lines.
     //
     GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
     GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
     GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA


     // Enable pull-up on GPIO19 (SPISTEA)
     GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;
     GpioCtrlRegs.GPAMUX2.bit.GPIO19=0;
     GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;
     GpioDataRegs.GPASET.bit.GPIO19=1;

     EDIS;
}

/////////////////////////////////////////
//子函数说明：这是一个带输入输出变量的子函数，H_Unint(data);这个式子会返回一个值，因此使用时应该data1 = H_Unint(data)；data变量本身也会改变
unsigned int H_Unint(unsigned int Data)     //获取Data高字节 XX00格式
{
    Data = Data & 0xFF00;
    return(Data);
}
///////////////////////////////////////////
unsigned int L_Unint(unsigned int Data)     //获取Data低字节 XX00格式
{
    Data = Data << 8;
    Data = Data & 0xFF00;
    return(Data);
}
///////////////////////////////////////////
//子函数说明：这个子函数会将两个00XX格式的数据合并为一个字，并返回结果值
unsigned int Unint(unsigned int H_DATA,unsigned int L_DATA)       //合并，返回一个字 XXXX格式
{
    unsigned int Temp;

    H_DATA = H_DATA << 8;           //SPI接收到的数据格式为00XX，XX00+00YY=XXYY
    H_DATA = H_DATA & 0xFF00;       //XX00格式
    Temp = H_DATA + L_DATA;

    return(Temp);
}

///////////////////////////////////////////
void Byte_Write(unsigned int Data)            //单字节的发送函数
{
    unsigned int Temp;

     SpiaRegs.SPITXBUF = Data&0xFF00;           //把数据放到发送缓冲寄存器中,准备发送

//     while(1)
//     {
//         if(SpiaRegs.SPISTS.bit.INT_FLAG == 1)
//             break;
//     }
     while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);//当SPISTS的INT_FLAG=1时表示已经发送完成

     Temp = SpiaRegs.SPIRXBUF;           //虚读SPIRXBUF寄存器,以清除SPI中断标志
     Temp = Temp;
}
////////////////////////////////////////////
unsigned int Byte_Read(void)                 //单字节的接收函数
{
     unsigned int Temp;

     SpiaRegs.SPITXBUF = 0x0000;             //伪写（SPI本身的特点，发送8位的同时接收了8位数据，格式为00XX）

//     while(1)
//     {
//         if(SpiaRegs.SPISTS.bit.INT_FLAG == 1)
//             break;
//     }
     while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);//当SPISTS的INT_FLAG=1时表示已经接收完成

     Temp = SpiaRegs.SPIRXBUF&0x00ff;               //从SPIRXBUF寄存器取数,以清除SPI中断标志，接收到,不是虚读

     return(Temp);
}
////////////////////////////////////////////
void Write_State(void)                       //写25LC640状态寄存器函数
{
     CS_Clear;                       //CS=0，有效
     DELAY_US(15);
     Byte_Write(Wrsr_CMD);           //写EEPROM状态寄存器命令
     Byte_Write(State);              //向EEPROM状态寄存器写入数据0x02
     CS_Set;                         //CS=1
}
////////////////////////////////////////////
unsigned int Read_State(void)               //读25LC640状态寄存器函数
{
     unsigned char data;

     CS_Clear;                         //CS=0
     DELAY_US(15);
     Byte_Write(Rdsr_CMD);             //写"读EEPROM状态寄存器命令"
     data = Byte_Read();
     CS_Set;                           //CS=1

     return(data);
}
////////////////////////////////////////////
void W_Enable_25LC640(void)       //25LC640写使能函数
{
     CS_Clear;                    //CS=0
     DELAY_US(15);
     Byte_Write(Wren_CMD);       //写使能命令字
     CS_Set;                     //CE=1
     DELAY_US(15);
}
////////////////////////////////////////////
void W_Disable_25LC640(void)      //25LC640禁止 写使能函数
{
     CS_Clear;                    //CS=0
     DELAY_US(15);
     Byte_Write(Wrdi_CMD);       //写禁止写命令字
     CS_Set;                     //CE=1
}

//上位机下发序号为44-118，对应放入
//25LC640的Page0-Page9，每页存放32字节，每个参数为2字节的话，即16个参数
unsigned int Num_To_Addr(unsigned int Number)
{
     unsigned int Number_Temp;
     unsigned int Page;
     unsigned int Addr_in_Page;
     unsigned int Data_Addr;

     Number_Temp = Number - 44;          //从输入的44-118   变为了   序号为0-74

     Page = Number_Temp / 16;              //..........,单字，每页16个参数，共需5页放完
     Addr_in_Page = Number_Temp % 16;     //求余数，获得数据在每页的序列（0-15）
     Data_Addr = 32 * Page + 2 * Addr_in_Page;      //数据在每页的起始地址

     //Page = Number_Temp / 8;             //求数据所在的页,双字，每页8个参数，共需10页放完
     //Addr_in_Page = Number_Temp % 8;     //求余数，获得数据在每页的序列
     //Data_Addr = 32 * Page + 4 * Addr_in_Page;      //数据在每页的起始地址

     return(Data_Addr);
}

////////////////////////////////////////////
void Write_to_25LC640(unsigned int Addr_Num,unsigned int Data)      //25LC640写函数,写单字
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int H_Data,L_Data;
     unsigned int flag;

     Data_Addr = Num_To_Addr(Addr_Num);    //获得数据写入的起始地址
     H_Addr = H_Unint(Data_Addr);          //数据写入的起始地址的高字节
     L_Addr = L_Unint(Data_Addr);          //数据写入的起始地址的低字节
     H_Data = H_Unint(Data);               //数据的高字节0000
     L_Data = L_Unint(Data);               //数据的低字节0100

     W_Enable_25LC640();   //使能25LC640写操作

     CS_Clear;                  //CS=0
     DELAY_US(15);
     Byte_Write(Write_CMD);     //输出写EEPROM命令
     Byte_Write(H_Addr);        //输出地址的高字节
     Byte_Write(L_Addr);        //输出地址的低字节
     Byte_Write(H_Data);
     Byte_Write(L_Data);
     CS_Set;                    //CS=1

     while(1)
     {
        flag = Read_State();         //读状态寄存器
        flag = flag & 0x0001;          // WIP表示是否正在被写
        if(flag == 0)                //当寄存器的WIP=0说明已经写完
            break;
     }

}



////////////////////////////////////////////
//入口参数：Addr_Num为参数的编号
unsigned int Read_From_25LC640(unsigned int Addr_Num)               //25LC640读函数
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int H_Data,L_Data,Data;

     Data_Addr = Num_To_Addr(Addr_Num);       //获得数据读取的起始地址 //上位机下发序号为44-118，对应放入
     L_Addr = L_Unint(Data_Addr);             //数据读取的起始地址的低字节
     H_Addr = H_Unint(Data_Addr);             //数据读取的起始地址的高字节

     CS_Clear;                      //CS=0
     DELAY_US(15);
     Byte_Write(Read_CMD);          //输出读EEPROM命令
     Byte_Write(H_Addr);            //输出地址的高字节
     Byte_Write(L_Addr);            //输出地址的低字节
     H_Data = Byte_Read();          //输入字的高字节
     L_Data = Byte_Read();          //输入字的低字节
     CS_Set;                        //CS=1

     Data = Unint(H_Data,L_Data);

     return(Data);
}

#ifdef Byte_message

//上位机下发序号为44-118，对应放入
//25LC640每个参数为1字节的话
unsigned int Num_To_AddrO(unsigned int Number)
{
     unsigned int Number_Temp;
     unsigned int Data_Addr;

     Number_Temp = Number - 44;          //从输入的44-118   变为了   序号为0-74

     Data_Addr = Number_Temp;      //数据在每页的起始地址


     return(Data_Addr);
}

////////////////////////////////////////////
void Write_to_25LC640_OneByte(unsigned int Addr_Num,unsigned int Data)      //25LC640写函数,写单字
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     //unsigned int H_Data,L_Data;
     unsigned int flag=1;

     Data_Addr = Num_To_AddrO(Addr_Num);    //获得数据写入的起始地址
     H_Addr = H_Unint(Data_Addr);          //数据写入的起始地址的高字节
     L_Addr = L_Unint(Data_Addr);          //数据写入的起始地址的低字节

     W_Enable_25LC640();   //使能25LC640写操作

     CS_Clear;                  //CS=0
     DELAY_US(15);
     Byte_Write(Write_CMD);     //输出写EEPROM命令
     Byte_Write(H_Addr);        //输出地址的高字节
     Byte_Write(L_Addr);        //输出地址的低字节
     Byte_Write(Data);
     DELAY_US(15);
     CS_Set;                    //CS=1

//     while(1)
//     {
//        flag = Read_State();         //读状态寄存器
//        flag = flag & 0x0001;          // WIP表示是否正在被写
//        if(flag == 0)                //当寄存器的WIP=0说明已经写完
//            break;
//     }
     while(flag==1)
     {
         flag = Read_State();         //读状态寄存器
         flag = flag & 0x0001;          // WIP表示是否正在被写
     }

}



////////////////////////////////////////////
//入口参数：Addr_Num为参数的编号
unsigned int Read_From_25LC640_OneByte(unsigned int Addr_Num)               //25LC640读函数
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int Data;

     Data_Addr = Num_To_AddrO(Addr_Num);       //获得数据读取的起始地址 //上位机下发序号为44-118，对应放入
     L_Addr = L_Unint(Data_Addr);             //数据读取的起始地址的低字节
     H_Addr = H_Unint(Data_Addr);             //数据读取的起始地址的高字节


     W_Disable_25LC640();   //禁止25LC640写操作

     CS_Clear;                      //CS=0
     DELAY_US(15);
     Byte_Write(Read_CMD);          //输出读EEPROM命令
     Byte_Write(H_Addr);            //输出地址的高字节
     Byte_Write(L_Addr);            //输出地址的低字节
     Data = Byte_Read();          //输入字节
     DELAY_US(15);
     CS_Set;                        //CS=1

     return(Data);
}
#endif

#ifdef Float_message

//上位机下发序号为44-118，对应放入
//25LC640的Page0-Page9，每页存放32字节，每个参数为4字节的话，即8个参数
unsigned int Num_To_AddrF(unsigned int Number)
{
     unsigned int Number_Temp;
//     unsigned int Page;
//     unsigned int Addr_in_Page;
     unsigned int Data_Addr;

     Number_Temp = Number - 44;          //从输入的44-118   变为了   序号为0-74

//     Page = Number_Temp / 16;              //..........,单字，每页16个参数，共需5页放完
//     Addr_in_Page = Number_Temp % 16;     //求余数，获得数据在每页的序列（0-15）
//     Data_Addr = 32 * Page + 2 * Addr_in_Page;      //数据在每页的起始地址

//     Page = Number_Temp / 8;             //求数据所在的页,双字，每页8个参数，共需10页放完
//     Addr_in_Page = Number_Temp % 8;     //求余数，获得数据在每页的序列
//     Data_Addr = 32 * Page + 4 * Addr_in_Page;      //数据在每页的起始地址

     Data_Addr=Number_Temp*4;

     return(Data_Addr);
}

void Write_to_25LC640F(unsigned int Addr_Num,float Data)      //25LC640写函数,写双字
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int flag;
     union FLOAT_COMF  FData_w;
     unsigned int u1,u2,u3,u4;

     Data_Addr = Num_To_AddrF(Addr_Num);    //获得数据写入的起始地址
     H_Addr = H_Unint(Data_Addr);          //数据写入的起始地址的高字节
     L_Addr = L_Unint(Data_Addr);          //数据写入的起始地址的低字节
     FData_w.all=Data;
     u4=FData_w.bit.MEM4<<8;
     u3=FData_w.bit.MEM3<<8;
     u2=FData_w.bit.MEM2<<8;
     u1=FData_w.bit.MEM1<<8;

     W_Enable_25LC640();   //使能25LC640写操作

     CS_Clear;                  //CS=0
     DELAY_US(15);
     Byte_Write(Write_CMD);     //输出写EEPROM命令
     Byte_Write(H_Addr);        //输出地址的高字节
     Byte_Write(L_Addr);        //输出地址的低字节
     Byte_Write(u4);
     Byte_Write(u3);
     Byte_Write(u2);
     Byte_Write(u1);
     CS_Set;                    //CS=1

     while(1)
     {
        flag = Read_State();         //读状态寄存器
        flag = flag & 0x0001;          // WIP表示是否正在被写
        if(flag == 0)                //当寄存器的WIP=0说明已经写完
            break;
     }

}

////////////////////////////////////////////
//入口参数：Addr_Num为参数的编号
float Read_From_25LC640F(unsigned int Addr_Num)               //25LC640读函数
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     union FLOAT_COMF  FData_r;

     Data_Addr = Num_To_AddrF(Addr_Num);       //获得数据读取的起始地址 //上位机下发序号为44-118，对应放入
     L_Addr = L_Unint(Data_Addr);             //数据读取的起始地址的低字节
     H_Addr = H_Unint(Data_Addr);             //数据读取的起始地址的高字节

     CS_Clear;                      //CS=0
     DELAY_US(15);
     Byte_Write(Read_CMD);          //输出读EEPROM命令
     Byte_Write(H_Addr);            //输出地址的高字节
     Byte_Write(L_Addr);            //输出地址的低字节
     FData_r.bit.MEM4 = Byte_Read();
     FData_r.bit.MEM3 = Byte_Read();
     FData_r.bit.MEM2 = Byte_Read();
     FData_r.bit.MEM1 = Byte_Read();
     CS_Set;                        //CS=1

     return(FData_r.all);
}
//==========
#endif
