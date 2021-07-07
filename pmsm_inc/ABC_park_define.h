/*
 * ABC_park_define.h
 *
 *  Created on: 2021年2月20日
 *      Author: sea
 */

#ifndef PMSM_INC_ABC_PARK_DEFINE_H_
#define PMSM_INC_ABC_PARK_DEFINE_H_

//坐标变换

typedef struct {  float  As;
                  float  Bs;
                  float  Cs;
                  float  Sine0;
                  float  Sine1;
                  float  Sine2;
                  float  Cosine0;
                  float  Cosine1;
                  float  Cosine2;
                  float  Angle;
                  float  Ds;
                  float  Qs;
                  float  V0;
                } ABC_PARK;




//声明ABC_PARK_handle为ABC_PARK指针类型
typedef ABC_PARK*ABC_PARK_handle;

//初始化
#define ABC_PARK_DEFAULTS {0, \
                              0, \
                              0, \
                              0, \
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

#endif /* PMSM_INC_ABC_PARK_DEFINE_H_ */
