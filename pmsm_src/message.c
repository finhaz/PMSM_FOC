/*
 *     message.c
 *
 *     ����ͨѶЭ�飬���յ������ݽ��н���
 *
 *
 */
#include "includes.h"

//#define set485 GpioDataRegs.GPBSET.bit.GPIO61= 1//����ʹ��
//#define clear485 GpioDataRegs.GPBCLEAR.bit.GPIO61= 1 //����ʹ��
#define set485 GpioDataRegs.GPBSET.bit.GPIO50= 1//����ʹ��
#define clear485 GpioDataRegs.GPBCLEAR.bit.GPIO50= 1 //����ʹ��
#define sci_buffer ScicRegs.SCIRXBUF.bit.RXDT

//ͨѶ˽�б�������粻�������ȡ�͸���
unsigned int RCBUF[24];//RS485 ���ջ����� ��ϵͳ��ͨѶЭ���11�����ݣ�
unsigned int ReciveRCOUNT=0;//RS485 ���ռ����� 0~11
unsigned int RC_DataCount=0;   //�������ݼ�����
unsigned int TXCOUNT=0;//RS485 ���ͼ�����
unsigned int TXBUF[30];//RS485 ���ͻ�����
unsigned int flagRC;//�������ݽ�����־λ
unsigned int flagSEND;//�������ݱ�־λ
unsigned int datasum;//�����������
unsigned int PackLength;     //���ݰ���
unsigned int RC_DataBUF[20];  //�������ݻ�����������վ���-���к�-������-���ݸ�-���ݵ�-У���룩
unsigned int RC_DataCount;   //�������ݼ�����
unsigned int SortNumber;     //վ���
unsigned int CommandCode;     //SCI������
unsigned int CheckCode;      //У����
unsigned int SendData;       //��������
unsigned int SendDataNumber;     //�������ݸ���
union FLOAT_COM  Data_send;
union FLOAT_COM  Data_get;
union FLOAT_COMF  FData_send;
union FLOAT_COMF  FData_get;
//����ӿ�����
unsigned int Switchsystem=0;
unsigned int SerialNumber;   //SCI���
#ifdef Float_message
float Paramet[ParameterNumber];
#else
int16 Paramet[ParameterNumber];
#endif


//�������ݴ���
void TXdeal(void)
{
//ʵʱ���в����ط�-����������-ÿ����4�ֽ�
    if(CommandCode==0x11)
	{
#ifdef Float_message
		TXBUF[0] = 0X00FE;//��ͷ���ŵ����ͻ������С�
		TXBUF[1] = 0X00FE;//��ͷ
		TXBUF[2] = 0X00FE;//��ͷ
		TXBUF[3] = 0X00FE;//��ͷ
		TXBUF[4] = 0X08;//����--------------------
		TXBUF[5] = SerialNumber;//���к�
		TXBUF[6] = CommandCode;//������
		TXBUF[7] = ConfirmCode;//ȷ����

		FData_send.all=Paramet[SerialNumber];
		TXBUF[8]=FData_send.bit.MEM1;
		TXBUF[9]=FData_send.bit.MEM2;
		TXBUF[10]=FData_send.bit.MEM3;
		TXBUF[11]=FData_send.bit.MEM4;

		datasum=TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7]+TXBUF[8]+TXBUF[9]+TXBUF[10]+TXBUF[11];
		datasum=~datasum+1;//У���룺���ȡ����1
		datasum &= 0X00FF;
		TXBUF[12]=datasum;//У����

		flagSEND = 1;//�������ݱ�־λ
		SendDataNumber = 13;
#else
        TXBUF[0] = 0X00FE;//��ͷ���ŵ����ͻ������С�
        TXBUF[1] = 0X00FE;//��ͷ
        TXBUF[2] = 0X00FE;//��ͷ
        TXBUF[3] = 0X00FE;//��ͷ
        TXBUF[4] = 0X06;//����--------------------
        TXBUF[5] = SerialNumber;//���к�
        TXBUF[6] = CommandCode;//������
        TXBUF[7] = ConfirmCode;//ȷ����

        Data_send.all=Paramet[SerialNumber];
        TXBUF[8]=Data_send.bit.MEM1;
        TXBUF[9]=Data_send.bit.MEM2;

        datasum=TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7]+TXBUF[8]+TXBUF[9];
        datasum=~datasum+1;//У���룺���ȡ����1
        datasum &= 0X00FF;
        TXBUF[10]=datasum;//У����

        flagSEND = 1;//�������ݱ�־λ
        SendDataNumber = 11;
#endif
	}
//���Բ���\����ϵ��\��ͣ��������·���Ӧ������ȷ����
    else if (CommandCode==0x50)
	{
#ifdef Float_message
        FData_get.bit.MEM1=RC_DataBUF[2];
        FData_get.bit.MEM2=RC_DataBUF[3];
        FData_get.bit.MEM3=RC_DataBUF[4];
        FData_get.bit.MEM4=RC_DataBUF[5];
        if(Paramet[SerialNumber]!=FData_get.all)//�����������EERPOM�ظ���д
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
		TXBUF[0] = 0XFE;//��ͷ
		TXBUF[1] = 0XFE;//��ͷ
		TXBUF[2] = 0XFE;//��ͷ
		TXBUF[3] = 0XFE;//��ͷ
		TXBUF[4] = 0X04;//����
		TXBUF[5] = SerialNumber; //���к�
		TXBUF[6] = CommandCode; //������
		TXBUF[7] = ConfirmCode;
		datasum = TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7];
		datasum = (~datasum)+1; //У���룺���ȡ����1
		datasum &= 0X00FF;
		TXBUF[8]=datasum; //У����
		flagSEND = 1;
		SendDataNumber = 9;
	}
//���ػ�
	else if (CommandCode == 0xB1)
	{
        Data_get.bit.MEM1=RC_DataBUF[2];
        Data_get.bit.MEM2=RC_DataBUF[3];
        Switchsystem=Data_get.all;
		TXBUF[0] = 0XFE;//��ͷ
		TXBUF[1] = 0XFE;//��ͷ
		TXBUF[2] = 0XFE;//��ͷ
		TXBUF[3] = 0XFE;//��ͷ
		TXBUF[4] = 0X04;//����
		TXBUF[5] = SerialNumber; //���к�
		TXBUF[6] = CommandCode; //������
		TXBUF[7] = ConfirmCode; //ȷ����
		datasum = TXBUF[4]+TXBUF[5]+TXBUF[6]+TXBUF[7];
		datasum = ~datasum+1; //У���룺���ȡ����1
		datasum &= 0X00FF;
		TXBUF[8]=datasum; //У����
		flagSEND = 1;
		SendDataNumber = 9;
	}
}





//����У�麯��
void Checkdata(void)
{
	int i;
	if(flagRC==1)//���ն���Ϊ��,��ʾ���ܶ�������ɣ����յ������ݰ���ȫ��ȷ
	{
		ReciveRCOUNT=0;
		flagRC = 0;
		SerialNumber = RC_DataBUF[0];//���к�//���ֻ��8λ0~255
		CommandCode = RC_DataBUF[1];//������
		CheckCode = RC_DataBUF[PackLength-1];//У����
		//��������У��
        datasum = 0;//xyyע�⣬�˱�����ø�Ϊ�ֲ�����
        //��֤���������Ƿ���ȷ������ͷ�����һ��У���������ȡ����Ȼ����У����Ƚ�
        for(i=0;i<(PackLength-1);i++)
        {
            datasum += RC_DataBUF[i];
        }
        datasum += PackLength;
        datasum =~ datasum+1;
        datasum &= 0X00FF;
        //datasum1 = datasum ;

        if(datasum == CheckCode) //����������ȷ��datasum =У���룩
        {
            TXdeal();
        }

	}
}



//SCI���ͺ���
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
		DELAY_US(5000);//��ͬ485ת��оƬ���ӳ�ʱ�䲻ͬ
		clear485;
		flagSEND = 0;
	}
}

//SCI���պ���
void SciRecieve(void)
{

	//DSPd SCIģ������У���ʽ��ע��Ĵ������Ƶ�ԭ��һ��ֻ�ܻ�ȡһ���ֽ�
	RCBUF[ReciveRCOUNT++] = sci_buffer; //&0x00FF���յ��İ�λ���ݣ�����8λ8λ�����н������ݣ�
	//ǰ4֡���ǰ�ͷFE FE FE FE
	if((ReciveRCOUNT-1)<PackHeadLength)//��ͷ������Ϊ4�������ݰ�����ͬ
	{
		//��С�ڰ�ͷ���ȣ������ڽ��ڴ���4λ��ͷ�����Զ�������ȶ��İ�ͷFE�Ա��Ƿ���ͬ
		if(RCBUF[ReciveRCOUNT-1] != PackHead)//
		{
			//������ͬ�������ݴ�������485���ռ������㣬ֹͣ���������ж�
			ReciveRCOUNT = 0;
		}
		//����ͬ�������ݴ�������
	}
	else if((ReciveRCOUNT-1) == PackHeadLength) //����
	{
		//�����ڰ�ͷ���ȣ������ڰ�ͷ�Ѿ��������
		PackLength = RCBUF[ReciveRCOUNT-1];//���ڶ���ʾ�İ���������������
		RC_DataCount = 0;
	}
	else if((ReciveRCOUNT-1) > PackHeadLength)
	{
		//�����ڰ�ͷ���ȣ������ڿ�ʼ�������
		if(PackLength==3||PackLength==4||PackLength==5||PackLength==7)//��������
		{
			if(RC_DataCount < PackLength)
			{
				//�������С�ڰ��������ʾ��δ�����ݲ��ִ�����ϣ��ʽ��������ڻ�����
				RC_DataBUF[RC_DataCount++] = RCBUF[ReciveRCOUNT-1];
			}
			if(RC_DataCount == PackLength)   //һ�����ݽ������
			{
				//����������ڰ��������ʾ�����Ѿ�������ϣ�
				flagRC = 1; //���ݽ��ս���
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


