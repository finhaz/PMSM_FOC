#ifndef __FRLS_H__
#define __FRLS_H__

typedef struct{float y[2];//ϵͳ���
               float theta[2];//ϵͳ����
               float h[2][2];//ϵͳ����
               float p[2][2];//Э������
               float k[2][2];//�������k
               float diag;//���
              }RLS_2nd_CAL;//����RLS


extern void rls_cal(void);
extern void rls_init(void);
extern void fluxtimec(void);

#endif
