################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uLipeRTOSv2/uLipe_Core.c \
../uLipeRTOSv2/uLipe_Flags.c \
../uLipeRTOSv2/uLipe_List.c \
../uLipeRTOSv2/uLipe_Mutex.c \
../uLipeRTOSv2/uLipe_Task.c 

OBJS += \
./uLipeRTOSv2/uLipe_Core.o \
./uLipeRTOSv2/uLipe_Flags.o \
./uLipeRTOSv2/uLipe_List.o \
./uLipeRTOSv2/uLipe_Mutex.o \
./uLipeRTOSv2/uLipe_Task.o 

C_DEPS += \
./uLipeRTOSv2/uLipe_Core.d \
./uLipeRTOSv2/uLipe_Flags.d \
./uLipeRTOSv2/uLipe_List.d \
./uLipeRTOSv2/uLipe_Mutex.d \
./uLipeRTOSv2/uLipe_Task.d 


# Each subdirectory must supply rules for building sources it contributes
uLipeRTOSv2/%.o: ../uLipeRTOSv2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F429xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"C:\Users\felipe.neves\Desktop\uLipeWork\uLipe_Hello\uLipeRTOSv2" -I"C:\Users\felipe.neves\Desktop\uLipeWork\uLipe_Hello\uLipeRTOSv2\Portable\GNU_GCC\ARMCM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


