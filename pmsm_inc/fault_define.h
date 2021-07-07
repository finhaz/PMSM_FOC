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

//----------------����Ĵ���----------------------------------------
//���ϱ�־�Ĵ���---���ڱ���
struct  FLAGFAULT_BITS {

        Uint16 lowvoltage_dc:1;//ֱ����Ƿѹ
        Uint16 highvoltage_dc:1;//ֱ�����ѹ
        Uint16 overcurrent:1;//����

        Uint16 overcurrentA:1;
        Uint16 overcurrentB:1;
        Uint16 overcurrentC:1;

        Uint16 hightemmotor:1;//����¶ȹ���
        Uint16 hightemcircuit:1;//��·���¶ȹ���
        Uint16 overload :1 ;//����

        Uint16 ocfault:1;//Ӳ����·����
        Uint16 motorlocked:1;//�����ת
        Uint16 speedtran_fault:1;//���ջ��л�ʧ��

        Uint16 rsvd2:4;

};
//
//ϵͳ���б�־�Ĵ���
struct  FLAGSYSTEM_BITS {

        Uint16 sysonoff:1;     //�����������1-���� 0-�ػ�
        Uint16 AC0CheckFinished:1;//����У�������־λ
        Uint16 rotatdircww:1;     //��ת-1 ��ת-0
        Uint16 motor_drive_medel:1;     //��ת
        Uint16 Changphase:1;    //
        Uint16 eeprom_w:1;      //дeeprom��־λ
        Uint16 faultoccur:1;       //���˹��ϱ�־λ����ʾҪ����ͣ��
        Uint16 if_start:1;//Ԥ��λ����ģʽ�еĶ�λ0����1��־
        Uint16 pre_dir_sign:1;//Ԥ��λ����ɱ�־λ
        Uint16 speed_loop_sign:1;//�����ٶȻ���־λ
        Uint16 jieou_inuse_sign:1;//�������ñ�־λ
        Uint16 moder_of_stop:1;//��ʸ��ͣ�����ñ�־λ
        Uint16 free_stop_sign:1;//����ͣ�����ñ�־λ
        Uint16 stop_pedir_sign:1;//ͣ���׶�Ԥ��λ���ñ�־λ
        Uint16 rsvd1:2;
};

//---------------���干ͬ��--------------------------------------------
//���ϱ�־��ͬ��
union FLAGFAULT_REG {

   Uint16        all;
   struct  FLAGFAULT_BITS bit;
};

//ϵͳ���б�־��ͬ��
union FLAGSYSTEM_REG {

   Uint16        all;
   struct  FLAGSYSTEM_BITS bit;
};


//----------------����ṹ��--------------------------------------------
//��־�ṹ��
typedef struct {
	union   FLAGFAULT_REG  flagfault; //���ϱ�־
	union   FLAGSYSTEM_REG flagsystem;
}FLAG_REGS;

#define FLAG_REGS_DEFAULTS {0,0}

//-------------------------------����λ-----------------------------------//
struct  COUNT_BITS {

        Uint16 ADC_ZeroCNTloop:4;     //����ѭ������
        Uint16 rsvd1:4;
        Uint16 rsvd2:1;
        Uint16 rsvd3:7;
};
union COUNT_REG {

   Uint16        all;
   struct  COUNT_BITS bit;
};

//�����Ĵ���
typedef struct {
	union    COUNT_REG  count; //���ϱ�־
	Uint16  resvd1;
}COUNTER_REGS;

#define COUNTER_REGS_DEFAULTS {0,0}

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif /* FLAG_H_ */
