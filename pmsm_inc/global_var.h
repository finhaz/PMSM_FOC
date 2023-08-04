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

//外部引用变量，此类变量在对应功能文件之外的区域应该只读，不可对其操作
extern unsigned int Switchsystem;
extern unsigned int SerialNumber;   //SCI序号
extern int32 ADCresultNEW[10]; //用于存放去偏置之后的数据（可选择减或不减直流偏置）


#ifdef Float_message
//
extern float Paramet[ParameterNumber];
//关系
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
#define weaking_start Paramet[51]//弱磁使能
#define jia_jian_su Paramet[52]//负力矩停机使能
#define Kw Paramet[53]//加速度
#define iq_start_0 Paramet[54]//起动电流
#define sudu_max Paramet[55]//给定的最终转速
#define speed_tran Paramet[56]//速度切换时刻
#define speed_driection Paramet[57]//正反转，0反转1正转
//速度环
#define kp_speed Paramet[58]
#define ki_speed Paramet[59]
#define max_speed Paramet[60]
#define min_speed Paramet[61]
//电流环
#define kp_iq Paramet[62]
#define ki_iq Paramet[63]
#define kp_id Paramet[71]
#define ki_id Paramet[72]
#define max_current Paramet[64]
#define min_current Paramet[65]
//电压外环
#define kp_weak Paramet[66]
#define ki_weak Paramet[67]
#define max_weak Paramet[68]
#define min_weak Paramet[69]

#else
//
extern int16 Paramet[ParameterNumber];
//关系
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
#define weaking_start Paramet[51]//弱磁使能
#define jia_jian_su Paramet[52]//负力矩停机使能
#define Kw Paramet[53]//加速度
#define iq_start_0 Paramet[54]//起动电流
#define sudu_max Paramet[55]//给定的最终转速
#define speed_tran Paramet[56]//速度切换时刻
#define speed_driection Paramet[57]//正反转，0反转1正转
//速度环
#define kp_speed (Paramet[58]*0.01)
#define ki_speed (Paramet[59]*0.001)
#define max_speed Paramet[60]
#define min_speed Paramet[61]
//电流环
#define kp_current (Paramet[62]*0.01)
#define ki_current (Paramet[63]*0.001)
#define max_current Paramet[64]
#define min_current Paramet[65]
//电压外环
#define kp_weak (Paramet[66]*0.001)
#define ki_weak (Paramet[67]*0.0001)
#define max_weak Paramet[68]
#define min_weak Paramet[69]

#endif

//结构体变量
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

//坐标变换
extern CLARKE_PARK Ialphabeta_to_Idq;//iα、iβ变成id、iq
extern CLARKE_PARK Udq_to_Ualphabeta;//ud、uq变成uα、uβ

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


//计数变量
extern Uint32 break_stage;
extern Uint32 zhiliu_count;
extern Uint32 stage_asr;
extern Uint32 pre_dir_count;

//指令
extern float speed_cankao;
extern float iq_buchan;
extern float iq_start;
extern float id_start;
extern float id_fankui;
extern float iq_fankui;

extern float jiaosudu1_asr;//电角速度
extern float theta;

//角度
extern float thetam_given;//给定角度
extern float delt_theta;//给定角度与估算角之差
extern float theta_buchan;//电流补偿后角度
extern float theta_buchan1;//电压补偿后角度


#endif
