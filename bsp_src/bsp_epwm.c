/*
 * Switch.c
 *
 */


#include "bsp.h"


void ConfigureEPwm(void)
{
    //EPWM1-3 DRIVE

    //Note that the default/2 divider for ePWMs and EMIFs
    //EPWMCLK=SYSCLKOUT=150MHZ
    //ePWM模块时基设置  // TBCLK = EPWMCLK/(HSPCLKDIV*CLKDIV)
    //PWM周期=15000个TBCLK周期100us
    //Uint16 Prd=7500;
    //Uint16 Prd_da=7500;
    // Initialize EPwm1/2/3/4/5/6/7
    //自增配置程序------------->>>>>>>>>>>>>
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;
    //EPWM Moudle 1 配置
    //TB
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm1Regs.TBCTL.bit.CLKDIV=0;
    EPwm1Regs.TBPRD=Prd;
    EPwm1Regs.TBPHS.half.TBPHS=0;//将相位寄存器值清零
    EPwm1Regs.TBCTR=0x0000;
    EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//增减模式
    EPwm1Regs.TBCTL.bit.PHSEN=TB_DISABLE;//禁止相位装载
    EPwm1Regs.TBCTL.bit.PRDLD=TB_SHADOW;//当CTR=0时，将映射寄存器中的数据装载到当前寄存器
    EPwm1Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0时发出同步信号
    //CC
    EPwm1Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA寄存器工作在 映射模式
    EPwm1Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB寄存器工作在 映射模式
    EPwm1Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//在CTR=0时装载
    EPwm1Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//在CTR=0时装载
    //AQ
    EPwm1Regs.AQCTLA.bit.CAU=AQ_SET;//当时间基准计数器的值等于CMPA的值，且正在递增计数，使EPWM1A为高电平
    EPwm1Regs.AQCTLA.bit.CAD=AQ_CLEAR;//当时间基准计数器的值等于CMPA的值，且正在递减计数，使EPWM1A为低电平

     //DB
    EPwm1Regs.AQCSFRC.all = 0x0005 ;//软件强制PWM输出为低,2017/12/26
    //EPwm1Regs.DBCTL.bit.OUT_MODE=DB_FULL_ENABLE;//使能上升沿及下降沿延时信号
    EPwm1Regs.DBCTL.bit.IN_MODE=DBA_ALL;//默认EPWM1A作为上升沿及下降沿延时的信号源
    EPwm1Regs.DBCTL.bit.POLSEL=DB_ACTV_HIC;//AHC\EPWM1B反转极性
    EPwm1Regs.DBFED=DT;//下降沿延时FED=DT个TBCLK
    EPwm1Regs.DBRED=DT;//上升沿延时RED=DT个TBCLK

    //事件触发设置ADC
    EPwm1Regs.ETSEL.bit.SOCAEN  = 1;         // 使能SOCA转换器工作
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;//计数器为0时触发转换
    EPwm1Regs.ETPS.bit.SOCAPRD  = ET_1ST; // Generate pulse on 1st event
//    EPwm1Regs.ETSEL.bit.SOCBEN  = 1;         // 使能SOCB转换器工作
//    EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTRD_CMPA;//计数器为0时触发转换
//    EPwm1Regs.ETPS.bit.SOCBPRD  = ET_1ST;
    //EPwm1Regs.CMPA.bit.CMPA  = 100;   // Set compare A value
    //EPwm1Regs.TBCTL.bit.CTRMODE=0;

    //EPWM Moudle 2 配置
    //TB
    EPwm2Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm2Regs.TBCTL.bit.CLKDIV=0;
    EPwm2Regs.TBPRD=Prd;
    EPwm2Regs.TBPHS.half.TBPHS=0;//将相位寄存器值清零
    EPwm2Regs.TBCTR=0x0000;
    EPwm2Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//增减模式
    EPwm2Regs.TBCTL.bit.PHSEN=TB_DISABLE;//禁止相位装载
    EPwm2Regs.TBCTL.bit.PRDLD=TB_SHADOW;//当CTR=0时，将映射寄存器中的数据装载到当前寄存器
    EPwm2Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0时发出同步信号
    //CC
    EPwm2Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA寄存器工作在 映射模式
    EPwm2Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB寄存器工作在 映射模式
    EPwm2Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//在CTR=0时装载
    EPwm2Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//在CTR=0时装载
    //AQ
    EPwm2Regs.AQCTLA.bit.CAU=AQ_SET;//当时间基准计数器的值等于CMPA的值，且正在递增计数，使EPWM1A为高电平
    EPwm2Regs.AQCTLA.bit.CAD=AQ_CLEAR;//当时间基准计数器的值等于CMPA的值，且正在递减计数，使EPWM1A为低电平

     //DB
    EPwm2Regs.AQCSFRC.all = 0x0005 ;//软件强制PWM输出为低,2017/12/26
    //EPwm1Regs.DBCTL.bit.OUT_MODE=DB_FULL_ENABLE;//使能上升沿及下降沿延时信号
    EPwm2Regs.DBCTL.bit.IN_MODE=DBA_ALL;//默认EPWM1A作为上升沿及下降沿延时的信号源
    EPwm2Regs.DBCTL.bit.POLSEL=DB_ACTV_HIC;//AHC\EPWM1B反转极性
    EPwm2Regs.DBFED=DT;//下降沿延时FED=DT个TBCLK
    EPwm2Regs.DBRED=DT;//上升沿延时RED=DT个TBCLK

    //EPWM Moudle 3 配置
    //TB
    EPwm3Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm3Regs.TBCTL.bit.CLKDIV=0;
    EPwm3Regs.TBPRD=Prd;
    EPwm3Regs.TBPHS.half.TBPHS=0;//将相位寄存器值清零
    EPwm3Regs.TBCTR=0x0000;
    EPwm3Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//增减模式
    EPwm3Regs.TBCTL.bit.PHSEN=TB_DISABLE;//禁止相位装载
    EPwm3Regs.TBCTL.bit.PRDLD=TB_SHADOW;//当CTR=0时，将映射寄存器中的数据装载到当前寄存器
    EPwm3Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0时发出同步信号
    //CC
    EPwm3Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA寄存器工作在 映射模式
    EPwm3Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB寄存器工作在 映射模式
    EPwm3Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//在CTR=0时装载
    EPwm3Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//在CTR=0时装载
    //AQ
    EPwm3Regs.AQCTLA.bit.CAU=AQ_SET;//当时间基准计数器的值等于CMPA的值，且正在递增计数，使EPWMA为高电平
    EPwm3Regs.AQCTLA.bit.CAD=AQ_CLEAR;//当时间基准计数器的值等于CMPA的值，且正在递减计数，使EPWMA为低电平

     //DB
    EPwm3Regs.AQCSFRC.all = 0x0005 ;//软件强制PWM输出为低,2017/12/26
    //EPwm3Regs.DBCTL.bit.OUT_MODE=DB_FULL_ENABLE;//使能上升沿及下降沿延时信号
    EPwm3Regs.DBCTL.bit.IN_MODE=DBA_ALL;//默认EPWMA作为上升沿及下降沿延时的信号源
    EPwm3Regs.DBCTL.bit.POLSEL=DB_ACTV_HIC;//AHC\EPWM1B反转极性
    EPwm3Regs.DBFED=DT;//下降沿延时FED=DT个TBCLK
    EPwm3Regs.DBRED=DT;//上升沿延时RED=DT个TBCLK


//    //EPWM7-8 DAC
//    //EPWM Moudle 7 配置
//    //TB
//    EPwm7Regs.TBCTL.bit.HSPCLKDIV=0;
//    EPwm7Regs.TBCTL.bit.CLKDIV=0;
//    EPwm7Regs.TBPRD=Prd_da;
//    EPwm7Regs.TBPHS.bit.TBPHS=0;//将相位寄存器值清零
//    EPwm7Regs.TBCTR=0x0000;
//    EPwm7Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//增减模式
//    EPwm7Regs.TBCTL.bit.PHSEN=TB_DISABLE;//禁止相位装载
//    EPwm7Regs.TBCTL.bit.PRDLD=TB_SHADOW;//当CTR=0时，将映射寄存器中的数据装载到当前寄存器
//    EPwm7Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0时发出同步信号
//    //CC
//    EPwm7Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA寄存器工作在 映射模式
//    EPwm7Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB寄存器工作在 映射模式
//    EPwm7Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//在CTR=0时装载
//    EPwm7Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//在CTR=0时装载
//    //AQ
//    EPwm7Regs.AQCTLA.bit.CAU=AQ_SET;//当时间基准计数器的值等于CMPA的值，且正在递增计数，使EPWMA为高电平
//    EPwm7Regs.AQCTLA.bit.CAD=AQ_CLEAR;//当时间基准计数器的值等于CMPA的值，且正在递减计数，使EPWMA为低电平
//    EPwm7Regs.AQCTLB.bit.CAU=AQ_SET;
//    EPwm7Regs.AQCTLB.bit.CAD=AQ_CLEAR;
//     //DB
//    //软件强制PWM输出为低
//    EPwm7Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
//    EPwm7Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
//    EPwm7Regs.DBCTL.bit.OUT_MODE=DB_DISABLE;//禁止死区模块
//
//
//    //EPWM Moudle 8 配置
//    //TB
//    EPwm8Regs.TBCTL.bit.HSPCLKDIV=0;
//    EPwm8Regs.TBCTL.bit.CLKDIV=0;
//    EPwm8Regs.TBPRD=Prd_da;
//    EPwm8Regs.TBPHS.bit.TBPHS=0;//将相位寄存器值清零
//    EPwm8Regs.TBCTR=0x0000;
//    EPwm8Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;//增减模式
//    EPwm8Regs.TBCTL.bit.PHSEN=TB_DISABLE;//禁止相位装载
//    EPwm8Regs.TBCTL.bit.PRDLD=TB_SHADOW;//当CTR=0时，将映射寄存器中的数据装载到当前寄存器
//    EPwm8Regs.TBCTL.bit.SYNCOSEL=TB_CTR_ZERO;//CTR=0时发出同步信号
//    //CC
//    EPwm8Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;//CMPA寄存器工作在 映射模式
//    EPwm8Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;//CMPB寄存器工作在 映射模式
//    EPwm8Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;//在CTR=0时装载
//    EPwm8Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;//在CTR=0时装载
//    //AQ
//    EPwm8Regs.AQCTLA.bit.CAU=AQ_SET;//当时间基准计数器的值等于CMPA的值，且正在递增计数，使EPWMA为高电平
//    EPwm8Regs.AQCTLA.bit.CAD=AQ_CLEAR;//当时间基准计数器的值等于CMPA的值，且正在递减计数，使EPWMA为低电平
//    EPwm8Regs.AQCTLB.bit.CAU=AQ_SET;
//    EPwm8Regs.AQCTLB.bit.CAD=AQ_CLEAR;
//     //DB
//    //软件强制PWM输出为低
//    EPwm8Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
//    EPwm8Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
//    EPwm8Regs.DBCTL.bit.OUT_MODE=DB_DISABLE;//使能上升沿及下降沿延时信号




    //TBD
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

//PWM封锁
void PWMoff(void)
{
    //ePWM1封锁
    EPwm1Regs.AQSFRC.bit.RLDCSF = 3 ;//选择直接加载
    EPwm1Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM2封锁
    EPwm2Regs.AQSFRC.bit.RLDCSF = 3 ;//选择直接加载
    EPwm2Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM3封锁
    EPwm3Regs.AQSFRC.bit.RLDCSF = 3 ;//选择直接加载
    EPwm3Regs.DBCTL.bit.OUT_MODE=0;

    //软件强制PWM输出为低
    EPwm1Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm1Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFB=AQ_CLEAR;

}

//零矢量,短路下桥臂
void PWMzero(void)
{
    //ePWM1封锁
    EPwm1Regs.AQSFRC.bit.RLDCSF = 3 ;//选择直接加载
    EPwm1Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM2封锁
    EPwm2Regs.AQSFRC.bit.RLDCSF = 3 ;//选择直接加载
    EPwm2Regs.DBCTL.bit.OUT_MODE=0;
    //ePWM3封锁
    EPwm3Regs.AQSFRC.bit.RLDCSF = 3 ;//选择直接加载
    EPwm3Regs.DBCTL.bit.OUT_MODE=0;

    //软件强制PWMA输出为低,PWMB输出为高
    EPwm1Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm1Regs.AQCSFRC.bit.CSFB=AQ_SET;
    EPwm2Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFB=AQ_SET;
    EPwm3Regs.AQCSFRC.bit.CSFA=AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFB=AQ_SET;
}

//打开PWM
void PWMopen(void)
{

    //ePWM1封锁
    EPwm1Regs.AQSFRC.bit.RLDCSF = 0;//
    EPwm1Regs.AQCSFRC.all = 0x0000 ;//无动作
    EPwm1Regs.DBCTL.bit.OUT_MODE=3;

    //ePWM2封锁
    EPwm2Regs.AQSFRC.bit.RLDCSF = 0 ;//
    EPwm2Regs.AQCSFRC.all = 0x0000 ;//无动作
    EPwm2Regs.DBCTL.bit.OUT_MODE=3;

    //ePWM3封锁
    EPwm3Regs.AQSFRC.bit.RLDCSF = 0 ;//
    EPwm3Regs.AQCSFRC.all = 0x0000 ;//无动作
    EPwm3Regs.DBCTL.bit.OUT_MODE=3;

}









