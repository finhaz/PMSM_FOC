
#define PackHeadLength 4 //包头长度
#define PackHead 0x00FE  //包头

#include "includes.h"

//结构体变量
FLAG_REGS FlagRegs=FLAG_REGS_DEFAULTS;//标志
COUNTER_REGS CounterRegs=COUNTER_REGS_DEFAULTS;//计数

ADC_VOLT_CURRENT_GET Adcget=ADC_VOLT_CURRENT_GET_DEFAULTS;
PI_CONTROL ASR=PI_CONTROL_DEFAULTS;
PI_CONTROL A_Id_R=PI_CONTROL_DEFAULTS;
PI_CONTROL A_Iq_R=PI_CONTROL_DEFAULTS;
PI_CONTROL Weak_R=PI_CONTROL_DEFAULTS;

PI_CONTROL A_IA_R=PI_CONTROL_DEFAULTS;
PI_CONTROL A_IB_R=PI_CONTROL_DEFAULTS;
PI_CONTROL A_IC_R=PI_CONTROL_DEFAULTS;

PR_CONTROL PR_Ialpha=PR_CONTROL_DEFAULTS;
PR_CONTROL PR_Ibeta=PR_CONTROL_DEFAULTS;
ADRC_Regulator ADRC_speed=ADRC_Regulator_DEFAULTS;
ADRC_Regulator ADRC_id=ADRC_RegC_DEFAULTS;
ADRC_Regulator ADRC_iq=ADRC_RegC_DEFAULTS;

//坐标变换
CLARKE_PARK Ialphabeta_to_Idq=CLARKE_PARK_DEFAULTS;//iα、iβ变成id、iq
CLARKE_PARK Udq_to_Ualphabeta=CLARKE_PARK_DEFAULTS;//ud、uq变成uα、uβ


SVPWM_2L Svpwm=SVPWM_2L_DEFAULTS;
SPWM_2L Spwm=SPWM_2L_DEFAULTS;

EMF_Cal_GET Emf_get=EMF_CAL_DEFAULTS;
PLL_CAL_GET Pll=PLL_CAL_DEFAULTS;
PLL_EMFC_GET PLL_e=PLL_EMFC_DEFAULTS;
Filter_AVR_GET We_Filter=Filter_AVR_GET_DEFAULTS;
Filter_AVR_GET We_Filter2=Filter_AVR_GET_DEFAULTS;

FILTRATE iq_filtrate=FILTRATE_DEFAULTS_100Hz;
FILTRATE id_filtrate=FILTRATE_DEFAULTS_100Hz;

Encoder_GET encoder_cal=Encoder_GET_DEFAULTS;
Filter_AVR_GET Wm_Filter=Filter_AVR_GET_DEFAULTS;

//计数变量
Uint32 break_stage=0;
Uint32 zhiliu_count=0;
Uint32 stage_asr=0;
Uint32 pre_dir_count=0;

//指令
float speed_cankao=0;
float iq_buchan=0;
float iq_start=0;
float id_start=0;
float id_fankui=0;
float iq_fankui=0;

float jiaosudu1_asr;
float theta;

//角度
float thetam_given=0;//给定角度
float delt_theta;//给定角度与估算角之差
float theta_buchan;//电流补偿后角度
float theta_buchan1;//电压补偿后角度







