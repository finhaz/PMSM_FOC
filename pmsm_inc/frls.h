#ifndef __FRLS_H__
#define __FRLS_H__

typedef struct{float y[2];//系统输出
               float theta[2];//系统参数
               float h[2][2];//系统输入
               float p[2][2];//协方差阵
               float k[2][2];//增益矩阵k
               float diag;//误差
              }RLS_2nd_CAL;//二阶RLS


extern void rls_cal(void);
extern void rls_init(void);
extern void fluxtimec(void);

#endif
