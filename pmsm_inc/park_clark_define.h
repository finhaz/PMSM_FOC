/*
 * Vector_control_define.h
 *
 *  Created on: 2020年9月15日
 *      Author: sea
 */

#ifndef VECTOR_CONTROL_DEFINE_H_
#define VECTOR_CONTROL_DEFINE_H_

//--------------------定义结构体-----------------------------------

//坐标变换
//----------------------------------------------------------------
typedef struct {float  As;        // Input: phase-a stator variable
                float  Bs;            // Input: phase-b stator variable
                float  Cs;            // Input: phase-c stator variable
                float  Alpha;     // Output: stationary d-axis stator variable
                float  Beta;      // Output: stationary q-axis stator variable
                float  Angle;     // Input: rotating angle (pu)
                float  Ds;            // Output: rotating d-axis stator variable
                float  Qs;            // Output: rotating q-axis stator variable
                float  Sine;
                float  Cosine;
                } CLARKE_PARK;

//声明CLARKE_PARK_handle为CLARKE_PARK指针类型
typedef CLARKE_PARK*CLARKE_PARK_handle;
//初始化
#define CLARKE_PARK_DEFAULTS {0, \
                              0, \
                              0, \
                              0, \
                              0, \
                              0, \
                              0, \
                              0, \
                              0, \
                              0, \
                            }



#endif /* PMSM_INC_VECTOR_CONTROL_DEFINE_H_ */
