/*
 * spwm_deine.h
 *
 *  Created on: 2021Äê3ÔÂ31ÈÕ
 *      Author: sea
 */

#ifndef PMSM_INC_SPWM_DEFINE_H_
#define PMSM_INC_SPWM_DEFINE_H_

//-----------------------------------------------------------------------------
//  Define the structure of the POSSPEED Object
//-----------------------------------------------------------------------------
typedef struct  {
                float  Ua;  // Input: reference a-axis phase voltage
                float  Ub;   // Input: reference b-axis phase voltage
                float  Uc;   // Input: reference c-axis phase voltage
                float  Udc;   // Input: DC voltage
                float  Tp;       // Input: PWM Modulation Period
                float Module;
                float  m_sin_a;   // Output: reference phase-a switching function
                float  m_sin_b;   // Output: reference phase-b switching function
                float  m_sin_c;   // Output: reference phase-c switching function
                float  Tcmpa;   // Output: reference phase-a switching function
                float  Tcmpb;   // Output: reference phase-b switching function
                float  Tcmpc;   // Output: reference phase-c switching function
                } SPWM_2L;
//-----------------------------------------------------------------------------
// Define a POSSPEED_handle
//-----------------------------------------------------------------------------
typedef SPWM_2L *SPWM_2L_handle;

//-----------------------------------------------------------------------------
// Default initializer for the POSSPEED Object.
//-----------------------------------------------------------------------------
#define SPWM_2L_DEFAULTS {0,0,0,0,0,\
                           0,0,0,0}


#endif /* PMSM_INC_SPWM_DEFINE_H_ */
