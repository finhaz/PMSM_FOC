/*
 * filter_windows_define.h
 *
 *  Created on: 2021��2��28��
 *      Author: sea
 */

#ifndef PMSM_INC_FILTER_WINDOWS_DEFINE_H_
#define PMSM_INC_FILTER_WINDOWS_DEFINE_H_

typedef struct {  float  value_buf[50];
                  float  value_in;
                  float  value_out;
                  int index;
                } HUACHUANG;


//����FILTRATE_handleΪFILTRATEָ������
typedef HUACHUANG* HUACHUANG_handle;
#define HUACHUANG_DEFAULTS {0,0,0,0,0,0,0,0,0,0,\
                            0,0,0,0,0,0,0,0,0,0,\
                            0,0,0,0,0,0,0,0,0,0,\
                            0,0,0,0,0,0,0,0,0,0,\
                            0,0,0,0,0,0,0,0,0,0,\
                            0,0,0}

#endif /* PMSM_INC_FILTER_WINDOWS_DEFINE_H_ */
