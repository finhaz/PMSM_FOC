#ifndef __GOF__
#define __GOF__




#include "math.h"

#include "Adc_self_define.h"
#include "fault_define.h"
#include "message_define.h"
#include "park_clark_define.h"
#include "pid_define.h"
#include "Svpwm_2L_define.h"
#include "IIRFilter_define.h"
#include "ABC_park_define.h"
#include "filter_windows_define.h"
#include "sogi_define.h"
#include "pr_define.h"
#include "spwm_define.h"
#include "emf_obsever_define.h"
#include "Encoder_solve_define.h"
#include "filter_AVR_define.h"
#include "adrc_define.h"

//�ⲿ���ñ�������������ڶ�Ӧ�����ļ�֮�������Ӧ��ֻ�������ɶ������
extern unsigned int Switchsystem;
extern unsigned int SerialNumber;   //SCI���
extern int32 ADCresultNEW[10]; //���ڴ��ȥƫ��֮������ݣ���ѡ����򲻼�ֱ��ƫ�ã�


#ifdef Float_message
//
extern float Paramet[ParameterNumber];
//��ϵ
#define Speedloop_ref Paramet[1]
#define Speedloop_meas Paramet[2]
#define Speedloop_qout Paramet[3]
#define Speed_pll Paramet[4]
#define Speed_filter Paramet[5]

#define CurrDloop_ref Paramet[6]
#define CurrDloop_meas Paramet[7]
#define CurrDloop_qout Paramet[8]

#define CurrQloop_ref Paramet[9]
#define CurrQloop_meas Paramet[10]
#define CurrQloop_qout Paramet[11]

#define Weakloop_ref Paramet[12]
#define Weakloop_meas Paramet[13]
#define Weakloop_qout Paramet[14]

#define U_Alpha Paramet[15]
#define U_Beta Paramet[16]
#define U_s Paramet[17]
#define U_dc Paramet[18]
#define I_dc Paramet[19]

#define I_AS Paramet[20]
#define I_BS Paramet[21]
#define I_CS Paramet[22]

#define FaultOccurr Paramet[43]

//#define OvLoad_current Paramet[44]
#define vmax_current Paramet[44]
#define M Paramet[45]
#define OvLoad_time Paramet[46]
#define min_voltage Paramet[47]
#define max_voltage Paramet[48]
#define id_mode Paramet[49]
#define id_ratio Paramet[50]
#define weaking_start Paramet[51]//����ʹ��
#define jia_jian_su Paramet[52]//������ͣ��ʹ��
#define Kw Paramet[53]//���ٶ�
#define iq_start_0 Paramet[54]//�𶯵���
#define sudu_max Paramet[55]//����������ת��
#define speed_tran Paramet[56]//�ٶ��л�ʱ��
#define speed_driection Paramet[57]//����ת��0��ת1��ת
//�ٶȻ�
#define kp_speed Paramet[58]
#define ki_speed Paramet[59]
#define max_speed Paramet[60]
#define min_speed Paramet[61]
//������
#define kp_iq Paramet[62]
#define ki_iq Paramet[63]
#define kp_id Paramet[71]
#define ki_id Paramet[72]
#define max_current Paramet[64]
#define min_current Paramet[65]
//��ѹ�⻷
#define kp_weak Paramet[66]
#define ki_weak Paramet[67]
#define max_weak Paramet[68]
#define min_weak Paramet[69]

#else
//
extern int16 Paramet[ParameterNumber];
//��ϵ
#define Speedloop_ref Paramet[1]
#define Speedloop_meas Paramet[2]
#define Speedloop_qout Paramet[3]
#define Speed_pll Paramet[4]
#define Speed_filter Paramet[5]

#define CurrDloop_ref Paramet[6]
#define CurrDloop_meas Paramet[7]
#define CurrDloop_qout Paramet[8]

#define CurrQloop_ref Paramet[9]
#define CurrQloop_meas Paramet[10]
#define CurrQloop_qout Paramet[11]

#define Weakloop_ref Paramet[12]
#define Weakloop_meas Paramet[13]
#define Weakloop_qout Paramet[14]

#define U_Alpha Paramet[15]
#define U_Beta Paramet[16]
#define U_s Paramet[17]
#define U_dc Paramet[18]
#define I_dc Paramet[19]

#define I_AS Paramet[20]
#define I_BS Paramet[21]
#define I_CS Paramet[22]

#define FaultOccurr Paramet[43]

//#define OvLoad_current Paramet[44]
#define vmax_current Paramet[44]
#define M (Paramet[45]*0.01)
#define OvLoad_time Paramet[46]
//#define Udclink_lv Paramet[47]
//#define Udclink_ov Paramet[48]
#define min_voltage Paramet[47]
#define max_voltage Paramet[48]
#define id_mode Paramet[49]
#define id_ratio Paramet[50]
#define weaking_start Paramet[51]//����ʹ��
#define jia_jian_su Paramet[52]//������ͣ��ʹ��
#define Kw Paramet[53]//���ٶ�
#define iq_start_0 Paramet[54]//�𶯵���
#define sudu_max Paramet[55]//����������ת��
#define speed_tran Paramet[56]//�ٶ��л�ʱ��
#define speed_driection Paramet[57]//����ת��0��ת1��ת
//�ٶȻ�
#define kp_speed (Paramet[58]*0.01)
#define ki_speed (Paramet[59]*0.001)
#define max_speed Paramet[60]
#define min_speed Paramet[61]
//������
#define kp_current (Paramet[62]*0.01)
#define ki_current (Paramet[63]*0.001)
#define max_current Paramet[64]
#define min_current Paramet[65]
//��ѹ�⻷
#define kp_weak (Paramet[66]*0.001)
#define ki_weak (Paramet[67]*0.0001)
#define max_weak Paramet[68]
#define min_weak Paramet[69]

#endif

//�ṹ�����
extern FLAG_REGS FlagRegs;
extern COUNTER_REGS CounterRegs;

extern ADC_VOLT_CURRENT_GET Adcget;
extern PI_CONTROL ASR;
extern PI_CONTROL A_Id_R;
extern PI_CONTROL A_Iq_R;
extern PI_CONTROL Weak_R;

extern PI_CONTROL A_IA_R;
extern PI_CONTROL A_IB_R;
extern PI_CONTROL A_IC_R;

extern PR_CONTROL PR_Ialpha;
extern PR_CONTROL PR_Ibeta;
extern ADRC_Regulator ADRC_speed;
extern ADRC_Regulator ADRC_id;
extern ADRC_Regulator ADRC_iq;

//����任
extern CLARKE_PARK Ialphabeta_to_Idq;//i����i�±��id��iq
extern CLARKE_PARK Udq_to_Ualphabeta;//ud��uq���u����u��

extern SVPWM_2L Svpwm;
extern SPWM_2L Spwm;

extern EMF_Cal_GET Emf_get;
extern PLL_CAL_GET Pll;
extern PLL_EMFC_GET PLL_e;
extern Filter_AVR_GET We_Filter;
extern Filter_AVR_GET We_Filter2;

extern FILTRATE iq_filtrate;
extern FILTRATE id_filtrate;

extern Encoder_GET encoder_cal;
extern Filter_AVR_GET Wm_Filter;


//��������
extern Uint32 break_stage;
extern Uint32 zhiliu_count;
extern Uint32 stage_asr;
extern Uint32 pre_dir_count;

//ָ��
extern float speed_cankao;
extern float iq_buchan;
extern float iq_start;
extern float id_start;
extern float id_fankui;
extern float iq_fankui;

extern float jiaosudu1_asr;//����ٶ�
extern float theta;

//�Ƕ�
extern float thetam_given;//�����Ƕ�
extern float delt_theta;//�����Ƕ�������֮��
extern float theta_buchan;//����������Ƕ�
extern float theta_buchan1;//��ѹ������Ƕ�


#endif
