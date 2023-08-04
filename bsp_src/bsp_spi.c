/*
 * bsp_spi.c
 *
 *  Created on: 2021��1��19��
 *      Author: sea
 */

#include "bsp.h"
#include "message_define.h"
#define Float_message 1

//FM25CL64�Ĳ����붨��
//spi��Ȼ����Ϊ8bit�շ�������dsp��16λ�շ�
//����dsp�ڷ�����ʱ���Ǹ�λ������Ч���������ǵ�λ������Ч��
#define Read_CMD 0x0300        //��25LC640����0000 0011
#define Write_CMD 0x0200       //д25LC640����0000 0010
#define Wren_CMD 0x0600        //д25LC640ʹ������0000 0110
#define Wrdi_CMD 0x0400        //��λ д25LC640ʹ������0000 0100(��ֹд����)
#define Rdsr_CMD 0x0500        //��25LC640״̬�Ĵ�������0000 0101����״̬������û�ж�ȡ������
#define Wrsr_CMD 0x0100        //д25LC640״̬�Ĵ�������0000 0001��д״̬������û��д������
#define State 0x0200           //д��25LC640״̬�Ĵ���������
#define CS_Set GpioDataRegs.GPASET.bit.GPIO19 = 1        //GPIO19�������Ϊ1
#define CS_Clear GpioDataRegs.GPACLEAR.bit.GPIO19 = 1    //GPIO19�������Ϊ0




/*****************************************SPI��ʼ������********************************************************/
void ConfigureSpi(void)
{
    EALLOW;


    // Set reset low before configuration changesSPI��λ
    // Clock polarity (0 == rising, 1 == falling)����ʱ��Ϊ������
    // 8-bit character8λ�ַ�����
    // Disable loop-back
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpiaRegs.SPICCR.bit.SPICHAR = 7;
    SpiaRegs.SPICCR.bit.SPILBK = 0;
    //SpiaRegs.SPICCR.all=0x0007;

    // ʹ����ģʽ����׼��λ// ʹ��Talk,SPIINT����
    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 1;
    SpiaRegs.SPICTL.bit.SPIINTENA = 0;
    //SpiaRegs.SPICTL.all=0x0006;

    SpiaRegs.SPIBRR=0x7F;     //���ò�����,SPIBRR>=3,LSP/(SPIBRR+1);SPIBRR=0,1,2,LSP/4
    //SpiaRegs.SPICCR.all=0x0087;  //ʹSPI�˳���λ״̬//ʹ���Բ���ģʽ

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
//�Ӻ���˵��������һ������������������Ӻ�����H_Unint(data);���ʽ�ӻ᷵��һ��ֵ�����ʹ��ʱӦ��data1 = H_Unint(data)��data��������Ҳ��ı�
unsigned int H_Unint(unsigned int Data)     //��ȡData���ֽ� XX00��ʽ
{
    Data = Data & 0xFF00;
    return(Data);
}
///////////////////////////////////////////
unsigned int L_Unint(unsigned int Data)     //��ȡData���ֽ� XX00��ʽ
{
    Data = Data << 8;
    Data = Data & 0xFF00;
    return(Data);
}
///////////////////////////////////////////
//�Ӻ���˵��������Ӻ����Ὣ����00XX��ʽ�����ݺϲ�Ϊһ���֣������ؽ��ֵ
unsigned int Unint(unsigned int H_DATA,unsigned int L_DATA)       //�ϲ�������һ���� XXXX��ʽ
{
    unsigned int Temp;

    H_DATA = H_DATA << 8;           //SPI���յ������ݸ�ʽΪ00XX��XX00+00YY=XXYY
    H_DATA = H_DATA & 0xFF00;       //XX00��ʽ
    Temp = H_DATA + L_DATA;

    return(Temp);
}

///////////////////////////////////////////
void Byte_Write(unsigned int Data)            //���ֽڵķ��ͺ���
{
    unsigned int Temp;

     SpiaRegs.SPITXBUF = Data&0xFF00;           //�����ݷŵ����ͻ���Ĵ�����,׼������

//     while(1)
//     {
//         if(SpiaRegs.SPISTS.bit.INT_FLAG == 1)
//             break;
//     }
     while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);//��SPISTS��INT_FLAG=1ʱ��ʾ�Ѿ��������

     Temp = SpiaRegs.SPIRXBUF;           //���SPIRXBUF�Ĵ���,�����SPI�жϱ�־
     Temp = Temp;
}
////////////////////////////////////////////
unsigned int Byte_Read(void)                 //���ֽڵĽ��պ���
{
     unsigned int Temp;

     SpiaRegs.SPITXBUF = 0x0000;             //αд��SPI������ص㣬����8λ��ͬʱ������8λ���ݣ���ʽΪ00XX��

//     while(1)
//     {
//         if(SpiaRegs.SPISTS.bit.INT_FLAG == 1)
//             break;
//     }
     while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);//��SPISTS��INT_FLAG=1ʱ��ʾ�Ѿ��������

     Temp = SpiaRegs.SPIRXBUF&0x00ff;               //��SPIRXBUF�Ĵ���ȡ��,�����SPI�жϱ�־�����յ�,�������

     return(Temp);
}
////////////////////////////////////////////
void Write_State(void)                       //д25LC640״̬�Ĵ�������
{
     CS_Clear;                       //CS=0����Ч
     DELAY_US(15);
     Byte_Write(Wrsr_CMD);           //дEEPROM״̬�Ĵ�������
     Byte_Write(State);              //��EEPROM״̬�Ĵ���д������0x02
     CS_Set;                         //CS=1
}
////////////////////////////////////////////
unsigned int Read_State(void)               //��25LC640״̬�Ĵ�������
{
     unsigned char data;

     CS_Clear;                         //CS=0
     DELAY_US(15);
     Byte_Write(Rdsr_CMD);             //д"��EEPROM״̬�Ĵ�������"
     data = Byte_Read();
     CS_Set;                           //CS=1

     return(data);
}
////////////////////////////////////////////
void W_Enable_25LC640(void)       //25LC640дʹ�ܺ���
{
     CS_Clear;                    //CS=0
     DELAY_US(15);
     Byte_Write(Wren_CMD);       //дʹ��������
     CS_Set;                     //CE=1
     DELAY_US(15);
}
////////////////////////////////////////////
void W_Disable_25LC640(void)      //25LC640��ֹ дʹ�ܺ���
{
     CS_Clear;                    //CS=0
     DELAY_US(15);
     Byte_Write(Wrdi_CMD);       //д��ֹд������
     CS_Set;                     //CE=1
}

//��λ���·����Ϊ44-118����Ӧ����
//25LC640��Page0-Page9��ÿҳ���32�ֽڣ�ÿ������Ϊ2�ֽڵĻ�����16������
unsigned int Num_To_Addr(unsigned int Number)
{
     unsigned int Number_Temp;
     unsigned int Page;
     unsigned int Addr_in_Page;
     unsigned int Data_Addr;

     Number_Temp = Number - 44;          //�������44-118   ��Ϊ��   ���Ϊ0-74

     Page = Number_Temp / 16;              //..........,���֣�ÿҳ16������������5ҳ����
     Addr_in_Page = Number_Temp % 16;     //�����������������ÿҳ�����У�0-15��
     Data_Addr = 32 * Page + 2 * Addr_in_Page;      //������ÿҳ����ʼ��ַ

     //Page = Number_Temp / 8;             //���������ڵ�ҳ,˫�֣�ÿҳ8������������10ҳ����
     //Addr_in_Page = Number_Temp % 8;     //�����������������ÿҳ������
     //Data_Addr = 32 * Page + 4 * Addr_in_Page;      //������ÿҳ����ʼ��ַ

     return(Data_Addr);
}

////////////////////////////////////////////
void Write_to_25LC640(unsigned int Addr_Num,unsigned int Data)      //25LC640д����,д����
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int H_Data,L_Data;
     unsigned int flag;

     Data_Addr = Num_To_Addr(Addr_Num);    //�������д�����ʼ��ַ
     H_Addr = H_Unint(Data_Addr);          //����д�����ʼ��ַ�ĸ��ֽ�
     L_Addr = L_Unint(Data_Addr);          //����д�����ʼ��ַ�ĵ��ֽ�
     H_Data = H_Unint(Data);               //���ݵĸ��ֽ�0000
     L_Data = L_Unint(Data);               //���ݵĵ��ֽ�0100

     W_Enable_25LC640();   //ʹ��25LC640д����

     CS_Clear;                  //CS=0
     DELAY_US(15);
     Byte_Write(Write_CMD);     //���дEEPROM����
     Byte_Write(H_Addr);        //�����ַ�ĸ��ֽ�
     Byte_Write(L_Addr);        //�����ַ�ĵ��ֽ�
     Byte_Write(H_Data);
     Byte_Write(L_Data);
     CS_Set;                    //CS=1

     while(1)
     {
        flag = Read_State();         //��״̬�Ĵ���
        flag = flag & 0x0001;          // WIP��ʾ�Ƿ����ڱ�д
        if(flag == 0)                //���Ĵ�����WIP=0˵���Ѿ�д��
            break;
     }

}



////////////////////////////////////////////
//��ڲ�����Addr_NumΪ�����ı��
unsigned int Read_From_25LC640(unsigned int Addr_Num)               //25LC640������
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int H_Data,L_Data,Data;

     Data_Addr = Num_To_Addr(Addr_Num);       //������ݶ�ȡ����ʼ��ַ //��λ���·����Ϊ44-118����Ӧ����
     L_Addr = L_Unint(Data_Addr);             //���ݶ�ȡ����ʼ��ַ�ĵ��ֽ�
     H_Addr = H_Unint(Data_Addr);             //���ݶ�ȡ����ʼ��ַ�ĸ��ֽ�

     CS_Clear;                      //CS=0
     DELAY_US(15);
     Byte_Write(Read_CMD);          //�����EEPROM����
     Byte_Write(H_Addr);            //�����ַ�ĸ��ֽ�
     Byte_Write(L_Addr);            //�����ַ�ĵ��ֽ�
     H_Data = Byte_Read();          //�����ֵĸ��ֽ�
     L_Data = Byte_Read();          //�����ֵĵ��ֽ�
     CS_Set;                        //CS=1

     Data = Unint(H_Data,L_Data);

     return(Data);
}

#ifdef Byte_message

//��λ���·����Ϊ44-118����Ӧ����
//25LC640ÿ������Ϊ1�ֽڵĻ�
unsigned int Num_To_AddrO(unsigned int Number)
{
     unsigned int Number_Temp;
     unsigned int Data_Addr;

     Number_Temp = Number - 44;          //�������44-118   ��Ϊ��   ���Ϊ0-74

     Data_Addr = Number_Temp;      //������ÿҳ����ʼ��ַ


     return(Data_Addr);
}

////////////////////////////////////////////
void Write_to_25LC640_OneByte(unsigned int Addr_Num,unsigned int Data)      //25LC640д����,д����
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     //unsigned int H_Data,L_Data;
     unsigned int flag=1;

     Data_Addr = Num_To_AddrO(Addr_Num);    //�������д�����ʼ��ַ
     H_Addr = H_Unint(Data_Addr);          //����д�����ʼ��ַ�ĸ��ֽ�
     L_Addr = L_Unint(Data_Addr);          //����д�����ʼ��ַ�ĵ��ֽ�

     W_Enable_25LC640();   //ʹ��25LC640д����

     CS_Clear;                  //CS=0
     DELAY_US(15);
     Byte_Write(Write_CMD);     //���дEEPROM����
     Byte_Write(H_Addr);        //�����ַ�ĸ��ֽ�
     Byte_Write(L_Addr);        //�����ַ�ĵ��ֽ�
     Byte_Write(Data);
     DELAY_US(15);
     CS_Set;                    //CS=1

//     while(1)
//     {
//        flag = Read_State();         //��״̬�Ĵ���
//        flag = flag & 0x0001;          // WIP��ʾ�Ƿ����ڱ�д
//        if(flag == 0)                //���Ĵ�����WIP=0˵���Ѿ�д��
//            break;
//     }
     while(flag==1)
     {
         flag = Read_State();         //��״̬�Ĵ���
         flag = flag & 0x0001;          // WIP��ʾ�Ƿ����ڱ�д
     }

}



////////////////////////////////////////////
//��ڲ�����Addr_NumΪ�����ı��
unsigned int Read_From_25LC640_OneByte(unsigned int Addr_Num)               //25LC640������
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int Data;

     Data_Addr = Num_To_AddrO(Addr_Num);       //������ݶ�ȡ����ʼ��ַ //��λ���·����Ϊ44-118����Ӧ����
     L_Addr = L_Unint(Data_Addr);             //���ݶ�ȡ����ʼ��ַ�ĵ��ֽ�
     H_Addr = H_Unint(Data_Addr);             //���ݶ�ȡ����ʼ��ַ�ĸ��ֽ�


     W_Disable_25LC640();   //��ֹ25LC640д����

     CS_Clear;                      //CS=0
     DELAY_US(15);
     Byte_Write(Read_CMD);          //�����EEPROM����
     Byte_Write(H_Addr);            //�����ַ�ĸ��ֽ�
     Byte_Write(L_Addr);            //�����ַ�ĵ��ֽ�
     Data = Byte_Read();          //�����ֽ�
     DELAY_US(15);
     CS_Set;                        //CS=1

     return(Data);
}
#endif

#ifdef Float_message

//��λ���·����Ϊ44-118����Ӧ����
//25LC640��Page0-Page9��ÿҳ���32�ֽڣ�ÿ������Ϊ4�ֽڵĻ�����8������
unsigned int Num_To_AddrF(unsigned int Number)
{
     unsigned int Number_Temp;
//     unsigned int Page;
//     unsigned int Addr_in_Page;
     unsigned int Data_Addr;

     Number_Temp = Number - 44;          //�������44-118   ��Ϊ��   ���Ϊ0-74

//     Page = Number_Temp / 16;              //..........,���֣�ÿҳ16������������5ҳ����
//     Addr_in_Page = Number_Temp % 16;     //�����������������ÿҳ�����У�0-15��
//     Data_Addr = 32 * Page + 2 * Addr_in_Page;      //������ÿҳ����ʼ��ַ

//     Page = Number_Temp / 8;             //���������ڵ�ҳ,˫�֣�ÿҳ8������������10ҳ����
//     Addr_in_Page = Number_Temp % 8;     //�����������������ÿҳ������
//     Data_Addr = 32 * Page + 4 * Addr_in_Page;      //������ÿҳ����ʼ��ַ

     Data_Addr=Number_Temp*4;

     return(Data_Addr);
}

void Write_to_25LC640F(unsigned int Addr_Num,float Data)      //25LC640д����,д˫��
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     unsigned int flag;
     union FLOAT_COMF  FData_w;
     unsigned int u1,u2,u3,u4;

     Data_Addr = Num_To_AddrF(Addr_Num);    //�������д�����ʼ��ַ
     H_Addr = H_Unint(Data_Addr);          //����д�����ʼ��ַ�ĸ��ֽ�
     L_Addr = L_Unint(Data_Addr);          //����д�����ʼ��ַ�ĵ��ֽ�
     FData_w.all=Data;
     u4=FData_w.bit.MEM4<<8;
     u3=FData_w.bit.MEM3<<8;
     u2=FData_w.bit.MEM2<<8;
     u1=FData_w.bit.MEM1<<8;

     W_Enable_25LC640();   //ʹ��25LC640д����

     CS_Clear;                  //CS=0
     DELAY_US(15);
     Byte_Write(Write_CMD);     //���дEEPROM����
     Byte_Write(H_Addr);        //�����ַ�ĸ��ֽ�
     Byte_Write(L_Addr);        //�����ַ�ĵ��ֽ�
     Byte_Write(u4);
     Byte_Write(u3);
     Byte_Write(u2);
     Byte_Write(u1);
     CS_Set;                    //CS=1

     while(1)
     {
        flag = Read_State();         //��״̬�Ĵ���
        flag = flag & 0x0001;          // WIP��ʾ�Ƿ����ڱ�д
        if(flag == 0)                //���Ĵ�����WIP=0˵���Ѿ�д��
            break;
     }

}

////////////////////////////////////////////
//��ڲ�����Addr_NumΪ�����ı��
float Read_From_25LC640F(unsigned int Addr_Num)               //25LC640������
{
     unsigned int H_Addr,L_Addr,Data_Addr;
     union FLOAT_COMF  FData_r;

     Data_Addr = Num_To_AddrF(Addr_Num);       //������ݶ�ȡ����ʼ��ַ //��λ���·����Ϊ44-118����Ӧ����
     L_Addr = L_Unint(Data_Addr);             //���ݶ�ȡ����ʼ��ַ�ĵ��ֽ�
     H_Addr = H_Unint(Data_Addr);             //���ݶ�ȡ����ʼ��ַ�ĸ��ֽ�

     CS_Clear;                      //CS=0
     DELAY_US(15);
     Byte_Write(Read_CMD);          //�����EEPROM����
     Byte_Write(H_Addr);            //�����ַ�ĸ��ֽ�
     Byte_Write(L_Addr);            //�����ַ�ĵ��ֽ�
     FData_r.bit.MEM4 = Byte_Read();
     FData_r.bit.MEM3 = Byte_Read();
     FData_r.bit.MEM2 = Byte_Read();
     FData_r.bit.MEM1 = Byte_Read();
     CS_Set;                        //CS=1

     return(FData_r.all);
}
//==========
#endif
