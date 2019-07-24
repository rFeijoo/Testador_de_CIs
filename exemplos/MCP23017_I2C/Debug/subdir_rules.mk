################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="/Applications/ti/ccs900/ccs/ccs_base/msp430/include" --include_path="/Users/rfeijo/Documents/CCS/workspace/OLED_I2C_teste" --include_path="/Applications/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="/Applications/ti/ccs900/ccs/ccs_base/msp430/include" --include_path="/Users/rfeijo/Documents/CCS/workspace/OLED_I2C_teste" --include_path="/Applications/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


