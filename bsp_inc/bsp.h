#ifndef __BSP__
#define __BSP__

#include "DSP28x_Project.h"    // DSP28x Headerfile
#include "bsp_epwm.h"
#include "bsp_spi.h"
#include "bsp_eqep.h"
#include "bsp_adc.h"
#include "bsp_sci.h"
#include "bsp_xint.h"
#include "bsp_para.h"
//#include "bsp_iic_eeprom.h"
#include "bsp_iic_24c128.h"

interrupt void scirxintab_isr(void);
interrupt void xint1_isr(void);
interrupt void adca1_interrupt_isr(void);
interrupt void timer0_isr(void);

void InitBoardGpio(void);
void ConfigurePIE(void);


#endif
