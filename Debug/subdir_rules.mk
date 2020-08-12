################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me -O3 --opt_for_speed=0 --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar" --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_3_40_00_05/source" --include_path="C:/ti/simplelink_cc32xx_sdk_3_40_00_05/source/ti/posix/ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/jackz/workspace_v9/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/include" --define=_SF_DEBUG_ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me -O3 --opt_for_speed=0 --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar" --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_3_40_00_05/source" --include_path="C:/ti/simplelink_cc32xx_sdk_3_40_00_05/source/ti/posix/ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/jackz/workspace_v9/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/include" --define=_SF_DEBUG_ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1879844321:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1879844321-inproc

build-1879844321-inproc: ../common.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs930/ccs/utils/sysconfig/sysconfig_cli.bat" -s "C:/ti/simplelink_cc32xx_sdk_3_40_00_05/.metadata/product.json" -o "syscfg" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_net_config.c: build-1879844321 ../common.syscfg
syscfg/ti_drivers_net_wifi_config.c: build-1879844321
syscfg/ti_drivers_config.c: build-1879844321
syscfg/ti_drivers_config.h: build-1879844321
syscfg/syscfg_c.rov.xs: build-1879844321
syscfg/: build-1879844321

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me -O3 --opt_for_speed=0 --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar" --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_3_40_00_05/source" --include_path="C:/ti/simplelink_cc32xx_sdk_3_40_00_05/source/ti/posix/ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/jackz/workspace_v9/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.5.LTS/include" --define=_SF_DEBUG_ -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/jackz/workspace_v9/mqtt_client_lidar/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


