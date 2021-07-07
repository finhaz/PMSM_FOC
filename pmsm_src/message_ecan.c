/*
 * Can_meesage.c
 *
 *  Created on: 2020年12月30日
 *      Author: sea
 */

#include "includes.h"     // DSP28x Headerfile


// Globals for this example
#define ParameterNumber 118 //定义通信变量个数
#define ConfirmCode  01 //SCI确认码
#define PSONumber 37

Uint32  TestMbox1 = 0;
Uint32  TestMbox2 = 0;
Uint32  TestMbox3 = 0;
Uint16 Rxdata[80];//CAN 接收缓存器 （系统级通讯协议的11个数据）
Uint16 cnt=0;
Uint16 cnt_sum=0;
Uint16 RC_flag=0;
Uint16 Send_sum=0;
unsigned int Switchsystem=0;
unsigned int TXBUF[80];//CAN 发送缓存器
unsigned int flagRC;//接收数据结束标志位
unsigned int flagSEND;//发送数据标志位
unsigned int datasum;//发送数据求和
unsigned int PackLength;     //数据包长
unsigned int SortNumber;     //站点号
unsigned int SerialNumber;   //SCI序号
unsigned int CommandCode;     //SCI命令码
unsigned int CheckCode;      //校验码
unsigned int SendData;       //发送数据
unsigned int SendDataNumber;     //发送数据个数
union FLOAT_COM  Data_get;
union FLOAT_COMF  FData_send;
union FLOAT_COMF  FData_get;
float Paramet[ParameterNumber];
float pso_t[10];
float PSO_g[4]={0,0,0,0};

//接收数据处理
void RXdeal(void)
{
    if(PackLength==5&&CommandCode==0xB1)//包长是否等于5-判断开关机程序
    {
        Data_get.bit.MEM1=Rxdata[7];
        Data_get.bit.MEM2=Rxdata[8];
        Switchsystem=Data_get.all;
    }
    if(PackLength==7)//包长是否等于7//属于主机覆盖从机，表示主机有数据修改需要覆盖对应的从机
    {
        FData_get.bit.MEM1=Rxdata[7];
        FData_get.bit.MEM2=Rxdata[8];
        FData_get.bit.MEM3=Rxdata[9];
        FData_get.bit.MEM4=Rxdata[10];
        Paramet[SerialNumber]=FData_get.all;
    }
}


//发送数据处理
void TXdeal(void)
{
   //实时运行参数回发-浮点数传输-每个数4字节
    if( (SerialNumber < 44)&&(CommandCode!=0xB1))
    {
        TXBUF[0] = 0X00FE;//包头，放到发送缓存器中。
        TXBUF[1] = 0X00FE;//包头
        TXBUF[2] = 0X00FE;//包头
        TXBUF[3] = 0X00FE;//包头
        TXBUF[4] = 0X08;//包长--------------------
        TXBUF[5] = SerialNumber;//序列号
        TXBUF[6] = CommandCode;//命令码
        TXBUF[7] = ConfirmCode;//确认码

        FData_send.all=Paramet[SerialNumber];
        TXBUF[8]=FData_send.bit.MEM1;
        TXBUF[9]=FData_send.bit.MEM2;
        TXBUF[10]=FData_send.bit.MEM3;
        TXBUF[11]=FData_send.bit.MEM4;

        datasum=TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7]+TXBUF[8]+TXBUF[9]+TXBUF[10]+TXBUF[11];
        datasum=~datasum+1;//校验码：求和取反加1
        datasum &= 0X00FF;
        TXBUF[12]=datasum;//校验码

        flagSEND = 1;//发送数据标志位
        SendDataNumber = 13;
    }
    else if ((SerialNumber < 119)&&(SerialNumber > 43))//调试参数\修正系数\启停机命令的下发回应，加了确认码
    {
        datasum=0;
        TXBUF[0] = 0XFE;//包头
        TXBUF[1] = 0XFE;//包头
        TXBUF[2] = 0XFE;//包头
        TXBUF[3] = 0XFE;//包头
        TXBUF[4] = 0X04;//包长
        TXBUF[5] = SerialNumber; //序列号
        TXBUF[6] = CommandCode; //命令码
        TXBUF[7] = ConfirmCode;
        datasum = TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7];
        datasum = (~datasum)+1; //校验码：求和取反加1
        datasum &= 0X00FF;
        TXBUF[8]=datasum; //校验码
        flagSEND = 1;
        SendDataNumber = 9;
    }
    else if (CommandCode == 0xB1)   //开关机
    {
        TXBUF[0] = 0XFE;//包头
        TXBUF[1] = 0XFE;//包头
        TXBUF[2] = 0XFE;//包头
        TXBUF[3] = 0XFE;//包头
        TXBUF[4] = 0X04;//包长
        TXBUF[5] = SerialNumber; //序列号
        TXBUF[6] = CommandCode; //命令码
        TXBUF[7] = ConfirmCode; //确认码
        datasum = TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7];
        datasum = ~datasum+1; //校验码：求和取反加1
        datasum &= 0X00FF;
        TXBUF[8]=datasum; //校验码
        flagSEND = 1;
        SendDataNumber = 9;
    }
    else if(PackLength==4)
    {
        if(Rxdata[7]==1)//1号逆变器
        {
            int i;
            TXBUF[0] = 0XFE;//包头
            TXBUF[1] = 0XFE;//包头
            TXBUF[2] = 0XFE;//包头
            TXBUF[3] = 0XFE;//包头
            TXBUF[4]=PSONumber-5;
            TXBUF[5]=200;
            TXBUF[6]=0xFF;
            TXBUF[7]=0x01;
            for(i=0;i<9;i++)
            {
                FData_send.all=pso_t[i];
                TXBUF[4*i+8]=FData_send.bit.MEM1;
                TXBUF[4*i+9]=FData_send.bit.MEM2;
                TXBUF[4*i+10]=FData_send.bit.MEM3;
                TXBUF[4*i+11]=FData_send.bit.MEM4;
            }
            TXBUF[PSONumber-1]=0XFF;//特殊校验符号
            flagSEND = 1;
            SendDataNumber = PSONumber;
        }
//        else if(Rxdata[7]==2)//2号逆变器
//        {
//            int i;
//            TXBUF[0] = 0XFE;//包头
//            TXBUF[1] = 0XFE;//包头
//            TXBUF[2] = 0XFE;//包头
//            TXBUF[3] = 0XFE;//包头
//            TXBUF[4]=PSONumber-5;
//            TXBUF[5]=200;
//            TXBUF[6]=0xFF;
//            TXBUF[7]=0x02;
//            for(i=0;i<7;i++)
//            {
//                FData_send.all=pso_t[i];
//                TXBUF[4*i+8]=FData_send.bit.MEM1;
//                TXBUF[4*i+9]=FData_send.bit.MEM2;
//                TXBUF[4*i+10]=FData_send.bit.MEM3;
//                TXBUF[4*i+11]=FData_send.bit.MEM4;
//            }
//            TXBUF[PSONumber-1]=0XFF;//特殊校验符号
//            flagSEND = 1;
//            SendDataNumber = PSONumber;
//        }
    }
}





//数据校验函数
void Checkdata(void)
{
    int i;
    if(flagRC==1)//接收队列为空,表示接受队列已完成，接收到的数据包完全正确
    {
        flagRC = 0;
        SerialNumber = Rxdata[5];//序列号//序号只有8位0~255
        CommandCode = Rxdata[6];//命令码
        CheckCode = Rxdata[PackLength+4];//校验码
        if(PackLength==19)//包长是否等于19，判断是否为粒子群反馈
        {
            int i=0;
            if(SerialNumber=200&&CheckCode==0xff)
            {
                for(i=0;i<4;i++)
                {
                    FData_get.bit.MEM1=Rxdata[4*i+7];
                    FData_get.bit.MEM2=Rxdata[4*i+8];
                    FData_get.bit.MEM3=Rxdata[4*i+9];
                    FData_get.bit.MEM4=Rxdata[4*i+10];
                    PSO_g[i]=FData_get.all;
                }
            }
        }
        else//正常数据校验
        {
            //正常数据校验
            datasum = 0;//xyy注意，此变量最好改为局部变量
            //验证接收数据是否正确，除包头和最后一个校验码外求和取反，然后与校验码比较
            for(i=4;i<(PackLength+4);i++)
            {
                datasum += Rxdata[i];
            }
            datasum =~ datasum+1;
            datasum &= 0X00FF;

            if(datasum == CheckCode) //接收数据正确（datasum =校验码）
            {
                RXdeal();
                TXdeal();
            }
        }

    }
}

//Can发送函数
void CanSend(void)
{
    struct ECAN_REGS ECanbShadow;
    Uint16 i;
    if(flagSEND==1)
    {
        flagSEND = 0;
        Send_sum=SendDataNumber>>3;
        Send_sum++;
        for(i=0; i < Send_sum; i++)
        {

            ECanbMboxes.MBOX25.MDL.byte.BYTE0=TXBUF[0+i*8];
            ECanbMboxes.MBOX25.MDL.byte.BYTE1=TXBUF[1+i*8];
            ECanbMboxes.MBOX25.MDL.byte.BYTE2=TXBUF[2+i*8];
            ECanbMboxes.MBOX25.MDL.byte.BYTE3=TXBUF[3+i*8];
            ECanbMboxes.MBOX25.MDH.byte.BYTE4=TXBUF[4+i*8];
            ECanbMboxes.MBOX25.MDH.byte.BYTE5=TXBUF[5+i*8];
            ECanbMboxes.MBOX25.MDH.byte.BYTE6=TXBUF[6+i*8];
            ECanbMboxes.MBOX25.MDH.byte.BYTE7=TXBUF[7+i*8];

            ECanbShadow.CANTRS.all = 0;
            ECanbShadow.CANTRS.bit.TRS25 = 1;             // Set TRS for mailbox under test
            ECanbRegs.CANTRS.all = ECanbShadow.CANTRS.all;

            do
            {
                ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;
            } while(ECanbShadow.CANTA.bit.TA25 == 0 );   // Wait for TA5 bit to be set..//如果线没有连接，如果线连接错误


            ECanbShadow.CANTA.all = 0;
            ECanbShadow.CANTA.bit.TA25 = 1;               // Clear TA5
            ECanbRegs.CANTA.all = ECanbShadow.CANTA.all;

        }

    }
}

//CAN接收函数
void CanRecieve(void)
{
    TestMbox1 = ECanbMboxes.MBOX16.MDL.all;
    TestMbox2 = ECanbMboxes.MBOX16.MDH.all;

    //分包接收
    if(TestMbox1==0xfefefefe)
    {

        Rxdata[0]=ECanbMboxes.MBOX16.MDL.byte.BYTE0;
        Rxdata[1]=ECanbMboxes.MBOX16.MDL.byte.BYTE1;
        Rxdata[2]=ECanbMboxes.MBOX16.MDL.byte.BYTE2;
        Rxdata[3]=ECanbMboxes.MBOX16.MDL.byte.BYTE3;

        //头一包数据分析
        Rxdata[4]=ECanbMboxes.MBOX16.MDH.byte.BYTE4;

        PackLength=Rxdata[4];

        //包长+5=帧长
        cnt_sum=(Rxdata[4]+5)>>3;
        cnt=0;
        if((Rxdata[4]+5-8*cnt_sum)>0)
        {
            cnt_sum=cnt_sum+1;
        }


        Rxdata[5]=ECanbMboxes.MBOX16.MDH.byte.BYTE5;
        Rxdata[6]=ECanbMboxes.MBOX16.MDH.byte.BYTE6;
        Rxdata[7]=ECanbMboxes.MBOX16.MDH.byte.BYTE7;

    }
    else
    {
        //第二包及之后数据处理
        Rxdata[0+cnt*8]=ECanbMboxes.MBOX16.MDL.byte.BYTE0;
        Rxdata[1+cnt*8]=ECanbMboxes.MBOX16.MDL.byte.BYTE1;
        Rxdata[2+cnt*8]=ECanbMboxes.MBOX16.MDL.byte.BYTE2;
        Rxdata[3+cnt*8]=ECanbMboxes.MBOX16.MDL.byte.BYTE3;

        Rxdata[4+cnt*8]=ECanbMboxes.MBOX16.MDH.byte.BYTE4;
        Rxdata[5+cnt*8]=ECanbMboxes.MBOX16.MDH.byte.BYTE5;
        Rxdata[6+cnt*8]=ECanbMboxes.MBOX16.MDH.byte.BYTE6;
        Rxdata[7+cnt*8]=ECanbMboxes.MBOX16.MDH.byte.BYTE7;

    }

    if(cnt<cnt_sum)
        cnt++;
    if(cnt==cnt_sum)
    {
        flagRC=1;
    }

    //TestMbox3 = ECanaMboxes.MBOX16.MSGID.all;//从外部接收邮箱16的ID，16为接收邮箱（CANMD）
}

