/*
 * bsp_sci.h
 *
 *  Created on: 2021��4��15��
 *      Author: sea
 */

#ifndef BSP_INC_BSP_SCI_H_
#define BSP_INC_BSP_SCI_H_

void ConfigureSci(void);
void sci_sendbyte(Uint16 byte);
void sci_recover(void);

#endif /* BSP_INC_BSP_SCI_H_ */
