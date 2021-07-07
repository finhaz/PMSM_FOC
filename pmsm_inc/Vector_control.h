/*
 * Vector_control.h
 *
 *  Created on: 2017-1-6
 *      Author: naiyangui
 */

#ifndef VECTOR_CONTROL_H_
#define VECTOR_CONTROL_H_

#include "pid.h"
#include "park_clark.h"
#include "var_init.h"
#include "emf_observer.h"
#include "spwm.h"
#include "Svpwm_2L.h"
#include "IIRFilter.h"
#include "ABC_park.h"
#include "filter_windows.h"
#include "Weak_control.h"

//º¯ÊýÉùÃ÷

void predir(void);
void OpenStartUp_angle(void);
void Ramp_MACRO(void);
void speed_loop(void);

void current_loop(void);
void angle_compensate(void);
void id_setzero(void);
void set_stop(void);
void trans(void);


void breaking(void);
void Sensorless_control(void);
void ABC_control(void);
void AlphaBeta_control(void);
void Sensor_control(void);


#endif /* VECTOR_CONTROL_H_ */
