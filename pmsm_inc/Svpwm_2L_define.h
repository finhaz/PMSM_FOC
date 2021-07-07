/*
 * Svpwm_2L_define.h
 *
 *  Created on: 2020Äê9ÔÂ15ÈÕ
 *      Author: sea
 */

#ifndef PMSM_INC_SVPWM_2L_DEFINE_H_
#define PMSM_INC_SVPWM_2L_DEFINE_H_

//-----------------------------------------------------------------------------
//  Define the structure of the POSSPEED Object
//-----------------------------------------------------------------------------
typedef struct  {
                float  Ualpha;  // Input: reference alpha-axis phase voltage
                float  Ubeta;   // Input: reference beta-axis phase voltage
                float  Vdc;   // Input: DC voltage
                float  Tp;       // Input: PWM Modulation Period
                float  Tcmpa;   // Output: reference phase-a switching function
                float  Tcmpb;   // Output: reference phase-b switching function
                float  Tcmpc;   // Output: reference phase-c switching function
                                 // void (*calc)();     // Pointer to calculation function
                } SVPWM_2L;
//-----------------------------------------------------------------------------
// Define a POSSPEED_handle
//-----------------------------------------------------------------------------
typedef SVPWM_2L *SVPWM_2L_handle;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
//#define SVPWM_2L_DEFAULTS {0,0,0,1.0,\
//                           1,1,1,\
//                           (void (*)(Uint32))svpwm_2l_calc}
#define SVPWM_2L_DEFAULTS {0,0,0,0,\
                           0,0,0}

#endif /* PMSM_INC_SVPWM_2L_DEFINE_H_ */
