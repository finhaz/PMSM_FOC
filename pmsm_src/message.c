/*
 *     message.c
 *
 *     按照通讯协议，对收到的数据进行解析
 *
 *
 */
#include "includes.h"

//#define set485 GpioDataRegs.GPBSET.bit.GPIO61= 1//发送使能
//#define clear485 GpioDataRegs.GPBCLEAR.bit.GPIO61= 1 //接收使能
#define set485 GpioDataRegs.GPBSET.bit.GPIO50= 1//发送使能
#define clear485 GpioDataRegs.GPBCLEAR.bit.GPIO50= 1 //接收使能
#define sci_buffer ScicRegs.SCIRXBUF.bit.RXDT

//通讯私有变量，外界不可随意读取和更改
unsigned int RCBUF[24];//RS485 接收缓存器 （系统级通讯协议的11个数据）
unsigned int ReciveRCOUNT=0;//RS485 接收计数器 0~11
unsigned int RC_DataCount=0;   //接收数据计数器
unsigned int TXCOUNT=0;//RS485 发送计数器
unsigned int TXBUF[30];//RS485 发送缓存器
unsigned int flagRC;//接收数据结束标志位
unsigned int flagSEND;//发送数据标志位
unsigned int datasum;//发送数据求和
unsigned int PackLength;     //数据包长
unsigned int RC_DataBUF[20];  //接收数据缓存器（包括站点号-序列号-命令码-数据高-数据低-校验码）
unsigned int RC_DataCount;   //接收数据计数器
unsigned int SortNumber;     //站点号
unsigned int CommandCode;     //SCI命令码
unsigned int CheckCode;      //校验码
unsigned int SendData;       //发送数据
unsigned int SendDataNumber;     //发送数据个数
union FLOAT_COM  Data_send;
union FLOAT_COM  Data_get;
union FLOAT_COMF  FData_send;
union FLOAT_COMF  FData_get;
//对外接口数据
unsigned int Switchsystem=0;
unsigned int SerialNumber;   //SCI序号
#ifdef Float_message
float Paramet[ParameterNumber];
#else
int16 Paramet[ParameterNumber];
#endif


//发送数据处理
void TXdeal(void)
{
//实时运行参数回发-浮点数传输-每个数4字节
    if(CommandCode==0x11)
	{
#ifdef Float_message
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
#else
        TXBUF[0] = 0X00FE;//包头，放到发送缓存器中。
        TXBUF[1] = 0X00FE;//包头
        TXBUF[2] = 0X00FE;//包头
        TXBUF[3] = 0X00FE;//包头
        TXBUF[4] = 0X06;//包长--------------------
        TXBUF[5] = SerialNumber;//序列号
        TXBUF[6] = CommandCode;//命令码
        TXBUF[7] = ConfirmCode;//确认码

        Data_send.all=Paramet[SerialNumber];
        TXBUF[8]=Data_send.bit.MEM1;
        TXBUF[9]=Data_send.bit.MEM2;

        datasum=TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7]+TXBUF[8]+TXBUF[9];
        datasum=~datasum+1;//校验码：求和取反加1
        datasum &= 0X00FF;
        TXBUF[10]=datasum;//校验码

        flagSEND = 1;//发送数据标志位
        SendDataNumber = 11;
#endif
	}
//调试参数\修正系数\启停机命令的下发回应，加了确认码
    else if (CommandCode==0x50)
	{
#ifdef Float_message
        FData_get.bit.MEM1=RC_DataBUF[2];
        FData_get.bit.MEM2=RC_DataBUF[3];
        FData_get.bit.MEM3=RC_DataBUF[4];
        FData_get.bit.MEM4=RC_DataBUF[5];
        if(Paramet[SerialNumber]!=FData_get.all)//避免无意义的EERPOM重复擦写
        {
            FlagRegs.flagsystem.bit.eeprom_w = 1;
            Paramet[SerialNumber]=FData_get.all;
        }
#else
        Data_get.bit.MEM1=RC_DataBUF[2];
        Data_get.bit.MEM2=RC_DataBUF[3];
        if(Paramet[SerialNumber]!=Data_get.all)
        {
            FlagRegs.flagsystem.bit.eeprom_w = 1;
            Paramet[SerialNumber]=Data_get.all;
        }
#endif

        FlagRegs.flagsystem.bit.eeprom_w = 1;
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
//开关机
	else if (CommandCode == 0xB1)
	{
        Data_get.bit.MEM1=RC_DataBUF[2];
        Data_get.bit.MEM2=RC_DataBUF[3];
        Switchsystem=Data_get.all;
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
}





//数据校验函数
void Checkdata(void)
{
	int i;
	if(flagRC==1)//接收队列为空,表示接受队列已完成，接收到的数据包完全正确
	{
		ReciveRCOUNT=0;
		flagRC = 0;
		SerialNumber = RC_DataBUF[0];//序列号//序号只有8位0~255
		CommandCode = RC_DataBUF[1];//命令码
		CheckCode = RC_DataBUF[PackLength-1];//校验码
		//正常数据校验
        datasum = 0;//xyy注意，此变量最好改为局部变量
        //验证接收数据是否正确，除包头和最后一个校验码外求和取反，然后与校验码比较
        for(i=0;i<(PackLength-1);i++)
        {
            datasum += RC_DataBUF[i];
        }
        datasum += PackLength;
        datasum =~ datasum+1;
        datasum &= 0X00FF;
        //datasum1 = datasum ;

        if(datasum == CheckCode) //接收数据正确（datasum =校验码）
        {
            TXdeal();
        }

	}
}



//SCI发送函数
void SciSend(void)
{
	if(flagSEND == 1)
	{
		int i=0;
		set485;
		DELAY_US(500);
		for(i=0;i<SendDataNumber;i++)
		{
			sci_sendbyte(TXBUF[i] );
		}
		DELAY_US(5000);//不同485转换芯片的延迟时间不同
		clear485;
		flagSEND = 0;
	}
}

//SCI接收函数
void SciRecieve(void)
{

	//DSPd SCI模块数据校验格式，注意寄存器限制的原因，一次只能获取一个字节
	RCBUF[ReciveRCOUNT++] = sci_buffer; //&0x00FF接收到的八位数据；按照8位8位来进行接收数据；
	//前4帧都是包头FE FE FE FE
	if((ReciveRCOUNT-1)<PackHeadLength)//包头长度总为4，与数据包长不同
	{
		//若小于包头长度，则现在仅在传送4位包头，所以对数据与既定的包头FE对比是否相同
		if(RCBUF[ReciveRCOUNT-1] != PackHead)//
		{
			//若不相同，则数据传送有误，485接收计数归零，停止接收跳出中断
			ReciveRCOUNT = 0;
		}
		//若相同，则数据传送无误
	}
	else if((ReciveRCOUNT-1) == PackHeadLength) //包长
	{
		//若等于包头长度，则现在包头已经传送完毕
		PackLength = RCBUF[ReciveRCOUNT-1];//将第二表示的包长赋给给定包长
		RC_DataCount = 0;
	}
	else if((ReciveRCOUNT-1) > PackHeadLength)
	{
		//若大于包头长度，则现在开始传送序号
		if(PackLength==3||PackLength==4||PackLength==5||PackLength==7)//包长符合
		{
			if(RC_DataCount < PackLength)
			{
				//如果计数小于包长，则表示尚未将数据部分传送完毕，故将数据置于缓存中
				RC_DataBUF[RC_DataCount++] = RCBUF[ReciveRCOUNT-1];
			}
			if(RC_DataCount == PackLength)   //一包数据接收完成
			{
				//如果计数等于包长，则表示数据已经传送完毕，
				flagRC = 1; //数据接收结束
				ReciveRCOUNT = 0;
				RC_DataCount = 0;
			}
		}
		else
		{
			ReciveRCOUNT = 0;
			RC_DataCount = 0;
		}
	}


}


