/*
 * bsp_spi.h
 *
 *  Created on: 2021年1月19日
 *      Author: sea
 */

#ifndef PMSM_INC_BSP_SPI_H_
#define PMSM_INC_BSP_SPI_H_

//相关函数的声明
void ConfigureSpi(void);
void Initspigpio(void);
void W_Enable_25LC640(void);       //25LC640写使能函数
void W_Disable_25LC640(void);      //25LC640禁止 写使能函数
void Write_to_25LC640(unsigned int Addr_Num,unsigned int Data);
void Write_to_25LC640F(unsigned int Addr_Num,float Data);//25LC640写函数,写双字
unsigned int Read_From_25LC640(unsigned int Addr_Num); //25LC640读函数
float Read_From_25LC640F(unsigned int Addr_Num);//25LC640读双字函数
void Write_to_25LC640_OneByte(unsigned int Addr_Num,unsigned int Data);
unsigned int Read_From_25LC640_OneByte(unsigned int Addr_Num);
void Write_State(void);                        //写25LC640状态寄存器函数
unsigned int Read_State(void);                 //25LC640读状态寄存器函数
void Byte_Write(unsigned int Data);            //单字节的写函数
unsigned int Byte_Read(void);                  //单字节的读函数
unsigned int H_Unint(unsigned int Data);       //获取字的高字节
unsigned int L_Unint(unsigned int Data);       //获取字的低字节
unsigned int Unint(unsigned int H_DATA,unsigned int L_DATA);        //返回一个字
unsigned int Num_To_Addr(unsigned int Number);     //由序号获得EEPROM内存放的地址
//long PowerOnCheckData[75];        //变频器上电自检时从25LC640读出的数据

//另外定义eeprom的相关
void readeeprom(void);
void wreeprom(void);



#endif /* PMSM_INC_BSP_SPI_H_ */
