/*
 * bsp_iic_eeprom.h
 *
 *  Created on: 2021Äê9ÔÂ4ÈÕ
 *      Author: sea
 */

#ifndef BSP_INC_BSP_IIC_EEPROM_H_
#define BSP_INC_BSP_IIC_EEPROM_H_

void ConfigI2C(void);
Uint16 WriteData(Uint16 *Wdata, Uint16  RomAddress, Uint16  number);
Uint16 ReadData(Uint16  *RamAddr, Uint16    RomAddress, Uint16 number);
Uint16  I2C_xrdy(void);
Uint16  I2C_rrdy(void);
float Read_From_24C01F(unsigned int Addr_Num);
void Write_to_24C01F(unsigned int Addr_Num,float Data);

#endif /* BSP_INC_BSP_IIC_EEPROM_H_ */
