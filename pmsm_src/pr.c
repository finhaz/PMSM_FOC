/*
 * pr.c
 *
 *  Created on: 2021年3月31日
 *      Author: sea
 */

#include "includes.h"

void PR_CONTROL_CALC(PR_CONTROL*p)
{
//   input_of_forward_integrator=2*wi*kr*error-output_of_feedback;
//   output_of_forward_integratorofi=output_of_forward_integratorofi+input_of_forward_integrator*1e-6*100;
//   output_of_backward_integrator=output_of_backward_integrator+output_of_forward_integratorofi*wg*wg*1e-6*100;
//   output_of_feedback=output_of_backward_integrator+2*wi*output_of_forward_integratorofi;
//   PR_out=output_of_forward_integratorofi+kp*error;
//   if( PR_out> prout_max)   % 输出限幅
//         PR_out = prout_max;
//   elseif (  PR_out < prout_min )
//        PR_out =prout_min;
//   end
    p->input_of_forward_integrator=2*p->wi*p->Kr*p->error-p->output_of_feedback;
    p->output_of_forward_integratorofi=p->output_of_forward_integratorofi+p->input_of_forward_integrator*T;
    p->output_of_backward_integrator=p->output_of_backward_integrator+p->output_of_forward_integratorofi*p->wg*p->wg*T;
    p->output_of_feedback=p->output_of_backward_integrator+2*p->wi*p->output_of_forward_integratorofi;
    p->PR_out=p->output_of_forward_integratorofi+p->error*p->Kp;
     if( p->PR_out> p->prout_max)
             p->PR_out = p->prout_max;
     else if (  p->PR_out < p->prout_min )
             p->PR_out =p->prout_min;

}
