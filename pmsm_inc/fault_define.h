/*
 * flag.h
 *
 *  Created on: 2017-1-6
 *      Author: naiyangui
 */

#ifndef FLAG_H_
#define FLAG_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------定义寄存器----------------------------------------
//故障标志寄存器---用于保护
struct  FLAGFAULT_BITS {

        Uint16 lowvoltage_dc:1;//直流侧欠压
        Uint16 highvoltage_dc:1;//直流侧过压
        Uint16 overcurrent:1;//过流

        Uint16 overcurrentA:1;
        Uint16 overcurrentB:1;
        Uint16 overcurrentC:1;

        Uint16 hightemmotor:1;//电机温度过高
        Uint16 hightemcircuit:1;//电路板温度过高
        Uint16 overload :1 ;//过载

        Uint16 ocfault:1;//硬件电路故障
        Uint16 motorlocked:1;//电机堵转
        Uint16 speedtran_fault:1;//开闭环切换失败

        Uint16 rsvd2:4;

};
//
//系统运行标志寄存器
struct  FLAGSYSTEM_BITS {

        Uint16 sysonoff:1;     //运行与否命令1-开机 0-关机
        Uint16 AC0CheckFinished:1;//采样校零结束标志位
        Uint16 rotatdircww:1;     //正转-1 反转-0
        Uint16 motor_drive_medel:1;     //反转
        Uint16 Changphase:1;    //
        Uint16 eeprom_w:1;      //写eeprom标志位
        Uint16 faultoccur:1;       //极端故障标志位，表示要故障停机
        Uint16 if_start:1;//预定位与起动模式中的定位0和起动1标志
        Uint16 pre_dir_sign:1;//预定位标完成标志位
        Uint16 speed_loop_sign:1;//切入速度环标志位
        Uint16 jieou_inuse_sign:1;//解耦启用标志位
        Uint16 moder_of_stop:1;//零矢量停机启用标志位
        Uint16 free_stop_sign:1;//自由停机启用标志位
        Uint16 stop_pedir_sign:1;//停机阶段预定位启用标志位
        Uint16 rsvd1:2;
};

//---------------定义共同体--------------------------------------------
//故障标志共同体
union FLAGFAULT_REG {

   Uint16        all;
   struct  FLAGFAULT_BITS bit;
};

//系统运行标志共同体
union FLAGSYSTEM_REG {

   Uint16        all;
   struct  FLAGSYSTEM_BITS bit;
};


//----------------定义结构体--------------------------------------------
//标志结构体
typedef struct {
	union   FLAGFAULT_REG  flagfault; //故障标志
	union   FLAGSYSTEM_REG flagsystem;
}FLAG_REGS;

#define FLAG_REGS_DEFAULTS {0,0}

//-------------------------------计数位-----------------------------------//
struct  COUNT_BITS {

        Uint16 ADC_ZeroCNTloop:4;     //采样循环计数
        Uint16 rsvd1:4;
        Uint16 rsvd2:1;
        Uint16 rsvd3:7;
};
union COUNT_REG {

   Uint16        all;
   struct  COUNT_BITS bit;
};

//计数寄存器
typedef struct {
	union    COUNT_REG  count; //故障标志
	Uint16  resvd1;
}COUNTER_REGS;

#define COUNTER_REGS_DEFAULTS {0,0}

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif /* FLAG_H_ */
