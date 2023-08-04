/*
 * bsp_iic_24c128.h
 *
 *  Created on: 2021Äê9ÔÂ8ÈÕ
 *      Author: sea
 */

#ifndef BSP_INC_BSP_IIC_24C128_H_
#define BSP_INC_BSP_IIC_24C128_H_

void ConfigI2C(void);
Uint16  I2C_xrdy(void);
Uint16  I2C_rrdy(void);
Uint16 Writetoeepromstr(Uint16 addr,Uint16 len,unsigned char * str);
void ReadfromeepromStr(Uint16 addr,Uint16 len,unsigned char * str);
float Read_From_24C128F(unsigned int Addr_Num);
void Write_to_24C128F(unsigned int Addr_Num,float Data);

#endif /* BSP_INC_BSP_IIC_24C128_H_ */
