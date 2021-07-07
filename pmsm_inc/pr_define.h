/*
 * pr_define.h
 *
 *  Created on: 2021年3月31日
 *      Author: sea
 */

#ifndef PMSM_INC_PR_DEFINE_H_
#define PMSM_INC_PR_DEFINE_H_

typedef struct {
//  error , wg ,output_of_forward_integratorofi,output_of_backward_integrator,
//  output_of_feedback, kr , kp , wi ,prout_max,prout_min
    float   error;        //Integrator sum; 1.31 format
    float   wg;
    float   input_of_forward_integrator;
    float   output_of_forward_integratorofi;
    float   output_of_backward_integrator;          //Proportional Gain
    float   output_of_feedback;          //Integral Gain
    float   Kr;          //Anti-windup Gain
    float   Kp;      //PI Output maximum limit
    float   wi;      //PI Output minimum limit
    float   prout_max;       //给定参考值
    float   prout_min;      //反馈值
    float   PR_out;         //PI Output; 1.15 format
    }PR_CONTROL;

#define PR_CONTROL_DEFAULTS {0,0,0,0,\
                             0,0,0,\
                             0,0,0,\
                             0,0,\
                             }

#endif /* PMSM_INC_PR_DEFINE_H_ */
