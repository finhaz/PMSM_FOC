/*
 * ABC_park_define.h
 *
 *  Created on: 2021��2��20��
 *      Author: sea
 */

#ifndef PMSM_INC_ABC_PARK_DEFINE_H_
#define PMSM_INC_ABC_PARK_DEFINE_H_

//����任

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




//����ABC_PARK_handleΪABC_PARKָ������
typedef ABC_PARK*ABC_PARK_handle;

//��ʼ��
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
