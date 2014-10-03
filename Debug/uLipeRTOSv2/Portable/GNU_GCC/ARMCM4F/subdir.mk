################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../uLipeRTOSv2/Portable/GNU_GCC/ARMCM4F/AsmStuff.S 

OBJS += \
./uLipeRTOSv2/Portable/GNU_GCC/ARMCM4F/AsmStuff.o 

S_UPPER_DEPS += \
./uLipeRTOSv2/Portable/GNU_GCC/ARMCM4F/AsmStuff.d 


# Each subdirectory must supply rules for building sources it contributes
uLipeRTOSv2/Portable/GNU_GCC/ARMCM4F/%.o: ../uLipeRTOSv2/Portable/GNU_GCC/ARMCM4F/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g3 -x assembler-with-cpp -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F429xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"C:\Users\felipe.neves\Desktop\uLipeWork\uLipe_Hello\uLipeRTOSv2\Portable\GNU_GCC\ARMCM4F" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


