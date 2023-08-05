################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
pmsm_src/%.obj: ../pmsm_src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs1120/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccs1120/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --include_path="E:/Users/Sea/workspace_v11/pmsm_foc/device_inc" --include_path="E:/Users/Sea/workspace_v11/pmsm_foc/bsp_inc" --include_path="E:/Users/Sea/workspace_v11/pmsm_foc/pmsm_inc" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="pmsm_src/$(basename $(<F)).d_raw" --obj_directory="pmsm_src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


