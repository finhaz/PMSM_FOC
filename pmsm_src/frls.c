#include "includes.h"

#define Lq 0.01567
#define Ld 0.01567

//RLS变量
float pid;//id电流微分项
float piq;//iq电流微分项
float idl;//上一周期id
float iql;//上一周期iq
RLS_2nd_CAL frls_cal;
float Eyes[2][2];
float r_err;//电阻辨识误差
float flux_err;//磁链辨识误差

//直接计算磁链
float emf;//直接计算反电动势
float flux_c;//直接计算结果

FILTRATE id_filtrate=FILTRATE_DEFAULTS_20Hz;//id滤波
FILTRATE iq_filtrate=FILTRATE_DEFAULTS_20Hz;//iq滤波
FILTRATE ud_filtrate=FILTRATE_DEFAULTS_20Hz;//ud滤波
FILTRATE uq_filtrate=FILTRATE_DEFAULTS_20Hz;//uq滤波
FILTRATE we_filtrate=FILTRATE_DEFAULTS_20Hz;//we滤波

//一阶RLS
float Kk=0;
float flux_dq=0;
float pk=1;
float lmwf=0.995;
float lmwf1=1;

unsigned int fluxn_cal=0;//10000->1s
unsigned int fluxn_s=0;//10000->1s


void MatrixADD(float* A, int A_Row,int A_Col, float* B,int B_Row, int B_Col, float *Result);//矩阵加法
void MatrixSUB(float* A, int A_Row,int A_Col, float* B,int B_Row, int B_Col, float *Result);//矩阵减法
void MatrixMUL(float* A, int A_Row,int A_Col, float* B,int B_Row, int B_Col, float *Result);//矩阵乘法
void MatrixInvert(float* A, int A_Row,int A_Col,float *Result);//矩阵求逆
float MatrixNorm(float* A, float* B);//矩阵2范数，其实为向量2范数

void rls_init(void)
{

//    frls_cal.theta[0]=2;
//    frls_cal.theta[1]=4;
    frls_cal.theta[0]=0;
    frls_cal.theta[1]=0;

    frls_cal.h[0][0]=1;
    frls_cal.h[0][1]=2;
    frls_cal.h[1][0]=3;
    frls_cal.h[1][1]=4;

    frls_cal.p[0][0]=1e5;
    frls_cal.p[0][1]=0;
    frls_cal.p[1][0]=0;
    frls_cal.p[1][1]=1e5;

    Eyes[0][0]=1;
    Eyes[0][1]=0;
    Eyes[1][0]=0;
    Eyes[1][1]=1;

}

void rls_cal(void)
{

//    float r[2][2];
//    //测试矩阵运算
//    MatrixADD(frls_cal.h[0],2,2, frls_cal.p[0],2,2, r[0]);
//    MatrixMUL(frls_cal.h[0],2,2, frls_cal.p[0],2,2, r[0]);
//    MatrixInvert(frls_cal.p[0],2,2, r[0]);
//    h_r[0][0]=1;
//    h_r[0][1]=3;
//    h_r[1][0]=2;
//    h_r[1][1]=4;
//    frls_cal.y[0]=1;
//    frls_cal.y[1]=2;

//    float h_r[2][2];//h的转置矩阵
//    float temp[2][2],temp1[2][2],temp2[2][2],temp3[2][2],temp4[2][2];
//    float y_t[2],y_t1[2],y_t2[2],y_t3[2];
    float id,iq,uq;//ud,
    float we_rls;


    we_rls=jiaosudu1_asr;

    id=Ialphabeta_to_Idq.Ds;
    iq=Ialphabeta_to_Idq.Qs;

    //ud=Udq_to_Ualphabeta.Ds;
    uq=Udq_to_Ualphabeta.Qs;

//    id_filtrate.X_in=Ialphabeta_to_Idq.Ds;
//    FILTRATE_CALC(&id_filtrate);
//    id=id_filtrate.Y;
//
//    iq_filtrate.X_in=Ialphabeta_to_Idq.Qs;
//    FILTRATE_CALC(&iq_filtrate);
//    iq=iq_filtrate.Y;
//
//    uq_filtrate.X_in=Udq_to_Ualphabeta.Qs;
//    FILTRATE_CALC(&uq_filtrate);
//    uq=uq_filtrate.Y;
//
//    ud_filtrate.X_in=Udq_to_Ualphabeta.Ds;
//    FILTRATE_CALC(&ud_filtrate);
//    ud=ud_filtrate.Y;

//    we_filtrate.X_in=we;
//    FILTRATE_CALC(&we_filtrate);
//    we_rls=we_filtrate.Y;


    emf=uq-rs*iq-Lq*piq-we_rls*Ld*id;
    flux_c=emf/we_rls;

//    //考虑动态性
//    pid=(id-idl)/T;
//    piq=(iq-iql)/T;

    //不考虑动态性
    pid=0;
    piq=0;

////    h_r[0][0]=pid;
////    h_r[0][1]=we_rls*id;
////    h_r[1][0]=-we_rls*iq;
////    h_r[1][1]=piq;
//
//    h_r[0][0]=iq;
//    h_r[0][1]=we_rls;
//    h_r[1][0]=id;
//    h_r[1][1]=0;
//
//
//    frls_cal.h[0][0]=h_r[0][0];
//    frls_cal.h[0][1]=h_r[1][0];
//    frls_cal.h[1][0]=h_r[0][1];
//    frls_cal.h[1][1]=h_r[1][1];
//
//
//
//    frls_cal.y[0]=uq-we_rls*Ld*id-Lq*piq;
//    frls_cal.y[1]=ud+we_rls*Lq*iq-Ld*pid;
//
//
//
//
//    //K=Pn0*h/(I+h'*Pn0*h);
//    MatrixMUL(h_r[0],2,2, frls_cal.p[0],2,2, temp[0]);
//
//    MatrixMUL(temp[0],2,2, frls_cal.h[0],2,2, temp1[0]);
//
//    MatrixADD(Eyes[0],2,2, temp1[0],2,2, temp2[0]);
//
//    MatrixInvert(temp2[0],2,2,temp3[0]);
//
//    MatrixMUL(frls_cal.p[0],2,2, frls_cal.h[0],2,2, temp4[0]);
//
//    MatrixMUL(temp4[0],2,2, temp3[0],2,2, frls_cal.k[0]);
//
//    //Pn1=(Pn0-K*h'*Pn0)/1;
//
//    MatrixMUL(frls_cal.k[0],2,2, h_r[0],2,2, temp[0]);
//
//    MatrixMUL(temp[0],2,2, frls_cal.p[0],2,2, temp1[0]);
//
//    temp2[0][0]=frls_cal.p[0][0];
//    temp2[0][1]=frls_cal.p[0][1];
//    temp2[1][0]=frls_cal.p[1][0];
//    temp2[1][1]=frls_cal.p[1][1];
//
//    MatrixSUB(temp2[0],2,2, temp1[0],2,2, frls_cal.p[0]);
//
//    //theta1=theta0+K*(y-h'*theta0);
//
////    MatrixMUL(h_r[0],2,2, frls_cal.theta[0],1,2, y_t[0]);
////
////    MatrixSUB(frls_cal.y[0],1,2, y_t[0],1,2,y_t1[0]);
////
////    MatrixMUL(frls_cal.k[0],2,2, y_t1[0],1,2, y_t2[0]);
////
////    y_t3[0][0]=frls_cal.theta[0][0];
////    y_t3[0][1]=frls_cal.theta[0][1];
////
////    MatrixADD(y_t3[0],2,2, y_t2[0],1,2, frls_cal.theta[0]);
//
//    y_t[0]=h_r[0][0]*frls_cal.theta[0]+h_r[0][1]*frls_cal.theta[1];
//    y_t[1]=h_r[1][0]*frls_cal.theta[0]+h_r[1][1]*frls_cal.theta[1];
//
//    y_t1[0]=frls_cal.y[0]-y_t[0];
//    y_t1[1]=frls_cal.y[1]-y_t[1];
//
//    y_t2[0]=frls_cal.k[0][0]*y_t1[0]+frls_cal.k[0][1]*y_t1[1];
//    y_t2[1]=frls_cal.k[1][0]*y_t1[0]+frls_cal.k[1][1]*y_t1[1];
//
//    y_t3[0]=frls_cal.theta[0];
//    y_t3[1]=frls_cal.theta[1];
//
//    frls_cal.theta[0]=y_t3[0]+y_t2[0];
//    frls_cal.theta[1]=y_t3[1]+y_t2[1];
//
//    frls_cal.diag=MatrixNorm(y_t1, frls_cal.y);

    frls_cal.diag=fabsf(emf-we_rls*flux_dq)/emf;

    if(frls_cal.diag<0.2)
    {
        lmwf=0.999;
        //lmwf1=lmwf+(1-lmwf)*diag
        lmwf1=lmwf+(1-lmwf)*frls_cal.diag;
    }
    else
    {
        lmwf=0.995;


//        //lmwf1=lmwf+(1-lmwf)*exp(diag);
//        lmwf1=lmwf+(1-lmwf)*exp(frls_cal.diag);

//        //lmwf1=lmwf+(1-lmwf)*diag^2
//        lmwf1=lmwf+(1-lmwf)*frls_cal.diag*frls_cal.diag;

        //lmwf1=lmwf+(1-lmwf)*diag
        lmwf1=lmwf+(1-lmwf)*frls_cal.diag;

    }

    Kk=pk*we_rls/(lmwf/lmwf1+pk*we_rls*we_rls);
    flux_dq=flux_dq+Kk*(emf-we_rls*flux_dq);
    pk=(1-Kk*we_rls)*pk/lmwf;



    idl=id;
    iql=iq;


}

void MatrixADD(float* A, int A_Row,int A_Col, float* B,int B_Row, int B_Col, float *Result)
{

  int i,j;
  float sum= 0.0f;


  //r[i][j]=a[i][j]+b[i][j]
  if(A_Row == B_Row)   // dimensions of matrixes must be accordance with each other
  {
      if(A_Col == B_Col)
      {
          for( i = 0; i < A_Row; i++) // compute the first row of results
          {
               for(j = 0; j < A_Col; j++)
               {
                    sum = (*(A + i*A_Row + j))+( *(B + i*B_Row + j));
                   *(Result + i*A_Row + j) = sum;
               }
          }
      }
  }
}

void MatrixSUB(float* A, int A_Row,int A_Col, float* B,int B_Row, int B_Col, float *Result)
{

  int i,j;
  float sum= 0.0f;


  //r[i][j]=a[i][j]+b[i][j]
  if(A_Row == B_Row)   // dimensions of matrixes must be accordance with each other
  {
      if(A_Col == B_Col)
      {
          for( i = 0; i < A_Row; i++) // compute the first row of results
          {
               for(j = 0; j < A_Col; j++)
               {
                    sum = (*(A + i*A_Row + j))-( *(B + i*B_Row + j));
                   *(Result + i*A_Row + j) = sum;
               }
          }
      }
  }
}

void MatrixMUL(float* A, int A_Row,int A_Col, float* B,int B_Row, int B_Col, float *Result)
{

  int i,j,k;
  float sum;


  //r[i][j]=a[i][0]*b[0][j]+...+a[i][k]*b[k][j]
  if(A_Col == B_Row)   // dimensions of matrixes must be accordance with each other
  {
      for( i = 0; i < A_Row; i++) // compute the first row of results
      {
           for(j = 0; j < B_Col; j++)
           {
                sum = 0.0f;
                for(k = 0; k < A_Col; k++)
                {
                    sum +=   (*(A + i*A_Col + k))*( *(B + k*B_Col + j));
                    //sum = sum + temp;
                }

               *(Result + i*B_Col + j) = sum;

           }
      }

  }

}


/* 矩阵求逆暂时不考虑通用性，仅为2*2 */
void MatrixInvert(float* A, int A_Row,int A_Col,float *Result)
{
    float a,b,c,d;
    a=*(A + 0*A_Col + 0);
    b=*(A + 0*A_Col + 1);
    c=*(A + 1*A_Col + 0);
    d=*(A + 1*A_Col + 1);

    *(Result + 0*A_Col + 0)= d/(a*d - b*c);
    *(Result + 0*A_Col + 1)=-b/(a*d - b*c);
    *(Result + 1*A_Col + 0)=-c/(a*d - b*c);
    *(Result + 1*A_Col + 1)=a/(a*d - b*c);

}

//实际为向量2范数
float MatrixNorm(float* A, float* B)
{
    float diag;
    float a0,a1,b0,b1;
    a0=*(A  + 0);
    a1=*(A  + 1);
    b0=*(B  + 0);
    b1=*(B  + 1);
    diag=(float)sqrt(a0*a0+a1*a1)/sqrt(b0*b0+b1*b1);
    return diag;
}

void fluxtimec(void)
{
    fluxn_cal++;
    if(fluxn_cal==10000)
    {
        fluxn_cal=0;
        fluxn_s++;
        if(fluxn_s==8)
        {
            sudu_max=400;
        }
        else if(fluxn_s==13)
        {
            sudu_max=800;
        }
        else if(fluxn_s==18)
        {
            Switchsystem=0;
        }
    }
}
