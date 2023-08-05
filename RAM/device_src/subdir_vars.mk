################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../device_src/DSP2833x_ADC_cal.asm \
../device_src/DSP2833x_CSMPasswords.asm \
../device_src/DSP2833x_CodeStartBranch.asm \
../device_src/DSP2833x_DBGIER.asm \
../device_src/DSP2833x_DisInt.asm \
../device_src/DSP2833x_usDelay.asm 

C_SRCS += \
../device_src/DSP2833x_Adc.c \
../device_src/DSP2833x_CpuTimers.c \
../device_src/DSP2833x_DefaultIsr.c \
../device_src/DSP2833x_EPwm.c \
../device_src/DSP2833x_EQep.c \
../device_src/DSP2833x_GlobalVariableDefs.c \
../device_src/DSP2833x_Gpio.c \
../device_src/DSP2833x_I2C.c \
../device_src/DSP2833x_MemCopy.c \
../device_src/DSP2833x_PieCtrl.c \
../device_src/DSP2833x_PieVect.c \
../device_src/DSP2833x_Sci.c \
../device_src/DSP2833x_SysCtrl.c \
../device_src/DSP2833x_Xintf.c 

C_DEPS += \
./device_src/DSP2833x_Adc.d \
./device_src/DSP2833x_CpuTimers.d \
./device_src/DSP2833x_DefaultIsr.d \
./device_src/DSP2833x_EPwm.d \
./device_src/DSP2833x_EQep.d \
./device_src/DSP2833x_GlobalVariableDefs.d \
./device_src/DSP2833x_Gpio.d \
./device_src/DSP2833x_I2C.d \
./device_src/DSP2833x_MemCopy.d \
./device_src/DSP2833x_PieCtrl.d \
./device_src/DSP2833x_PieVect.d \
./device_src/DSP2833x_Sci.d \
./device_src/DSP2833x_SysCtrl.d \
./device_src/DSP2833x_Xintf.d 

OBJS += \
./device_src/DSP2833x_ADC_cal.obj \
./device_src/DSP2833x_Adc.obj \
./device_src/DSP2833x_CSMPasswords.obj \
./device_src/DSP2833x_CodeStartBranch.obj \
./device_src/DSP2833x_CpuTimers.obj \
./device_src/DSP2833x_DBGIER.obj \
./device_src/DSP2833x_DefaultIsr.obj \
./device_src/DSP2833x_DisInt.obj \
./device_src/DSP2833x_EPwm.obj \
./device_src/DSP2833x_EQep.obj \
./device_src/DSP2833x_GlobalVariableDefs.obj \
./device_src/DSP2833x_Gpio.obj \
./device_src/DSP2833x_I2C.obj \
./device_src/DSP2833x_MemCopy.obj \
./device_src/DSP2833x_PieCtrl.obj \
./device_src/DSP2833x_PieVect.obj \
./device_src/DSP2833x_Sci.obj \
./device_src/DSP2833x_SysCtrl.obj \
./device_src/DSP2833x_Xintf.obj \
./device_src/DSP2833x_usDelay.obj 

ASM_DEPS += \
./device_src/DSP2833x_ADC_cal.d \
./device_src/DSP2833x_CSMPasswords.d \
./device_src/DSP2833x_CodeStartBranch.d \
./device_src/DSP2833x_DBGIER.d \
./device_src/DSP2833x_DisInt.d \
./device_src/DSP2833x_usDelay.d 

OBJS__QUOTED += \
"device_src\DSP2833x_ADC_cal.obj" \
"device_src\DSP2833x_Adc.obj" \
"device_src\DSP2833x_CSMPasswords.obj" \
"device_src\DSP2833x_CodeStartBranch.obj" \
"device_src\DSP2833x_CpuTimers.obj" \
"device_src\DSP2833x_DBGIER.obj" \
"device_src\DSP2833x_DefaultIsr.obj" \
"device_src\DSP2833x_DisInt.obj" \
"device_src\DSP2833x_EPwm.obj" \
"device_src\DSP2833x_EQep.obj" \
"device_src\DSP2833x_GlobalVariableDefs.obj" \
"device_src\DSP2833x_Gpio.obj" \
"device_src\DSP2833x_I2C.obj" \
"device_src\DSP2833x_MemCopy.obj" \
"device_src\DSP2833x_PieCtrl.obj" \
"device_src\DSP2833x_PieVect.obj" \
"device_src\DSP2833x_Sci.obj" \
"device_src\DSP2833x_SysCtrl.obj" \
"device_src\DSP2833x_Xintf.obj" \
"device_src\DSP2833x_usDelay.obj" 

C_DEPS__QUOTED += \
"device_src\DSP2833x_Adc.d" \
"device_src\DSP2833x_CpuTimers.d" \
"device_src\DSP2833x_DefaultIsr.d" \
"device_src\DSP2833x_EPwm.d" \
"device_src\DSP2833x_EQep.d" \
"device_src\DSP2833x_GlobalVariableDefs.d" \
"device_src\DSP2833x_Gpio.d" \
"device_src\DSP2833x_I2C.d" \
"device_src\DSP2833x_MemCopy.d" \
"device_src\DSP2833x_PieCtrl.d" \
"device_src\DSP2833x_PieVect.d" \
"device_src\DSP2833x_Sci.d" \
"device_src\DSP2833x_SysCtrl.d" \
"device_src\DSP2833x_Xintf.d" 

ASM_DEPS__QUOTED += \
"device_src\DSP2833x_ADC_cal.d" \
"device_src\DSP2833x_CSMPasswords.d" \
"device_src\DSP2833x_CodeStartBranch.d" \
"device_src\DSP2833x_DBGIER.d" \
"device_src\DSP2833x_DisInt.d" \
"device_src\DSP2833x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../device_src/DSP2833x_ADC_cal.asm" \
"../device_src/DSP2833x_CSMPasswords.asm" \
"../device_src/DSP2833x_CodeStartBranch.asm" \
"../device_src/DSP2833x_DBGIER.asm" \
"../device_src/DSP2833x_DisInt.asm" \
"../device_src/DSP2833x_usDelay.asm" 

C_SRCS__QUOTED += \
"../device_src/DSP2833x_Adc.c" \
"../device_src/DSP2833x_CpuTimers.c" \
"../device_src/DSP2833x_DefaultIsr.c" \
"../device_src/DSP2833x_EPwm.c" \
"../device_src/DSP2833x_EQep.c" \
"../device_src/DSP2833x_GlobalVariableDefs.c" \
"../device_src/DSP2833x_Gpio.c" \
"../device_src/DSP2833x_I2C.c" \
"../device_src/DSP2833x_MemCopy.c" \
"../device_src/DSP2833x_PieCtrl.c" \
"../device_src/DSP2833x_PieVect.c" \
"../device_src/DSP2833x_Sci.c" \
"../device_src/DSP2833x_SysCtrl.c" \
"../device_src/DSP2833x_Xintf.c" 


