/*
 * Can_meesage.c
 *
 *  Created on: 2020��12��30��
 *      Author: sea
 */

#include "includes.h"     // DSP28x Headerfile


// Globals for this example
#define ParameterNumber 118 //����ͨ�ű�������
#define ConfirmCode  01 //SCIȷ����
#define PSONumber 37

Uint32  TestMbox1 = 0;
Uint32  TestMbox2 = 0;
Uint32  TestMbox3 = 0;
Uint16 Rxdata[80];//CAN ���ջ����� ��ϵͳ��ͨѶЭ���11�����ݣ�
Uint16 cnt=0;
Uint16 cnt_sum=0;
Uint16 RC_flag=0;
Uint16 Send_sum=0;
unsigned int Switchsystem=0;
unsigned int TXBUF[80];//CAN ���ͻ�����
unsigned int flagRC;//�������ݽ�����־λ
unsigned int flagSEND;//�������ݱ�־λ
unsigned int datasum;//�����������
unsigned int PackLength;     //���ݰ���
unsigned int SortNumber;     //վ���
unsigned int SerialNumber;   //SCI���
unsigned int CommandCode;     //SCI������
unsigned int CheckCode;      //У����
unsigned int SendData;       //��������
unsigned int SendDataNumber;     //�������ݸ���
union FLOAT_COM  Data_get;
union FLOAT_COMF  FData_send;
union FLOAT_COMF  FData_get;
float Paramet[ParameterNumber];
float pso_t[10];
float PSO_g[4]={0,0,0,0};

//�������ݴ���
void RXdeal(void)
{
    if(PackLength==5&&CommandCode==0xB1)//�����Ƿ����5-�жϿ��ػ�����
    {
        Data_get.bit.MEM1=Rxdata[7];
        Data_get.bit.MEM2=Rxdata[8];
        Switchsystem=Data_get.all;
    }
    if(PackLength==7)//�����Ƿ����7//�����������Ǵӻ�����ʾ�����������޸���Ҫ���Ƕ�Ӧ�Ĵӻ�
    {
        FData_get.bit.MEM1=Rxdata[7];
        FData_get.bit.MEM2=Rxdata[8];
        FData_get.bit.MEM3=Rxdata[9];
        FData_get.bit.MEM4=Rxdata[10];
        Paramet[SerialNumber]=FData_get.all;
    }
}


//�������ݴ���
void TXdeal(void)
{
   //ʵʱ���в����ط�-����������-ÿ����4�ֽ�
    if( (SerialNumber < 44)&&(CommandCode!=0xB1))
    {
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
    }
    else if ((SerialNumber < 119)&&(SerialNumber > 43))//���Բ���\����ϵ��\��ͣ��������·���Ӧ������ȷ����
    {
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
    else if (CommandCode == 0xB1)   //���ػ�
    {
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
    else if(PackLength==4)
    {
        if(Rxdata[7]==1)//1�������
        {
            int i;
            TXBUF[0] = 0XFE;//��ͷ
            TXBUF[1] = 0XFE;//��ͷ
            TXBUF[2] = 0XFE;//��ͷ
            TXBUF[3] = 0XFE;//��ͷ
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
            TXBUF[PSONumber-1]=0XFF;//����У�����
            flagSEND = 1;
            SendDataNumber = PSONumber;
        }
//        else if(Rxdata[7]==2)//2�������
//        {
//            int i;
//            TXBUF[0] = 0XFE;//��ͷ
//            TXBUF[1] = 0XFE;//��ͷ
//            TXBUF[2] = 0XFE;//��ͷ
//            TXBUF[3] = 0XFE;//��ͷ
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
//            TXBUF[PSONumber-1]=0XFF;//����У�����
//            flagSEND = 1;
//            SendDataNumber = PSONumber;
//        }
    }
}





//����У�麯��
void Checkdata(void)
{
    int i;
    if(flagRC==1)//���ն���Ϊ��,��ʾ���ܶ�������ɣ����յ������ݰ���ȫ��ȷ
    {
        flagRC = 0;
        SerialNumber = Rxdata[5];//���к�//���ֻ��8λ0~255
        CommandCode = Rxdata[6];//������
        CheckCode = Rxdata[PackLength+4];//У����
        if(PackLength==19)//�����Ƿ����19���ж��Ƿ�Ϊ����Ⱥ����
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
        else//��������У��
        {
            //��������У��
            datasum = 0;//xyyע�⣬�˱�����ø�Ϊ�ֲ�����
            //��֤���������Ƿ���ȷ������ͷ�����һ��У���������ȡ����Ȼ����У����Ƚ�
            for(i=4;i<(PackLength+4);i++)
            {
                datasum += Rxdata[i];
            }
            datasum =~ datasum+1;
            datasum &= 0X00FF;

            if(datasum == CheckCode) //����������ȷ��datasum =У���룩
            {
                RXdeal();
                TXdeal();
            }
        }

    }
}

//Can���ͺ���
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
            } while(ECanbShadow.CANTA.bit.TA25 == 0 );   // Wait for TA5 bit to be set..//�����û�����ӣ���������Ӵ���


            ECanbShadow.CANTA.all = 0;
            ECanbShadow.CANTA.bit.TA25 = 1;               // Clear TA5
            ECanbRegs.CANTA.all = ECanbShadow.CANTA.all;

        }

    }
}

//CAN���պ���
void CanRecieve(void)
{
    TestMbox1 = ECanbMboxes.MBOX16.MDL.all;
    TestMbox2 = ECanbMboxes.MBOX16.MDH.all;

    //�ְ�����
    if(TestMbox1==0xfefefefe)
    {

        Rxdata[0]=ECanbMboxes.MBOX16.MDL.byte.BYTE0;
        Rxdata[1]=ECanbMboxes.MBOX16.MDL.byte.BYTE1;
        Rxdata[2]=ECanbMboxes.MBOX16.MDL.byte.BYTE2;
        Rxdata[3]=ECanbMboxes.MBOX16.MDL.byte.BYTE3;

        //ͷһ�����ݷ���
        Rxdata[4]=ECanbMboxes.MBOX16.MDH.byte.BYTE4;

        PackLength=Rxdata[4];

        //����+5=֡��
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
        //�ڶ�����֮�����ݴ���
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

    //TestMbox3 = ECanaMboxes.MBOX16.MSGID.all;//���ⲿ��������16��ID��16Ϊ�������䣨CANMD��
}

