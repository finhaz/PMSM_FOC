/*
 * sogi_define.h
 *
 *  Created on: 2021��2��28��
 *      Author: sea
 */

#ifndef PMSM_INC_SOGI_DEFINE_H_
#define PMSM_INC_SOGI_DEFINE_H_

//------------------------------------------SOGI
typedef struct {  float  Uo_in;
                  float  w_in;
                  float  v;
                  float  qv;
                  float  e_out;
                  float  e_n1;
                  float  e_n2;
                  float  e_n3;
                } SOGI;


//����FILTRATE_handleΪFILTRATEָ������
typedef SOGI* SOGI_handle;
#define SOGI_DEFAULTS {0,0,0,0,0,0,0,0}

#endif /* PMSM_INC_SOGI_DEFINE_H_ */
