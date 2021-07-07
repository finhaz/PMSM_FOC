/*
 * bsp_spi.h
 *
 *  Created on: 2021��1��19��
 *      Author: sea
 */

#ifndef PMSM_INC_BSP_SPI_H_
#define PMSM_INC_BSP_SPI_H_

//��غ���������
void ConfigureSpi(void);
void Initspigpio(void);
void W_Enable_25LC640(void);       //25LC640дʹ�ܺ���
void W_Disable_25LC640(void);      //25LC640��ֹ дʹ�ܺ���
void Write_to_25LC640(unsigned int Addr_Num,unsigned int Data);
void Write_to_25LC640F(unsigned int Addr_Num,float Data);//25LC640д����,д˫��
unsigned int Read_From_25LC640(unsigned int Addr_Num); //25LC640������
float Read_From_25LC640F(unsigned int Addr_Num);//25LC640��˫�ֺ���
void Write_to_25LC640_OneByte(unsigned int Addr_Num,unsigned int Data);
unsigned int Read_From_25LC640_OneByte(unsigned int Addr_Num);
void Write_State(void);                        //д25LC640״̬�Ĵ�������
unsigned int Read_State(void);                 //25LC640��״̬�Ĵ�������
void Byte_Write(unsigned int Data);            //���ֽڵ�д����
unsigned int Byte_Read(void);                  //���ֽڵĶ�����
unsigned int H_Unint(unsigned int Data);       //��ȡ�ֵĸ��ֽ�
unsigned int L_Unint(unsigned int Data);       //��ȡ�ֵĵ��ֽ�
unsigned int Unint(unsigned int H_DATA,unsigned int L_DATA);        //����һ����
unsigned int Num_To_Addr(unsigned int Number);     //����Ż��EEPROM�ڴ�ŵĵ�ַ
//long PowerOnCheckData[75];        //��Ƶ���ϵ��Լ�ʱ��25LC640����������

//���ⶨ��eeprom�����
void readeeprom(void);
void wreeprom(void);



#endif /* PMSM_INC_BSP_SPI_H_ */
