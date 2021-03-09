################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Buffahiti.c \
../source/led.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/state_machine.c \
../source/systick.c \
../source/tsi.c 

OBJS += \
./source/Buffahiti.o \
./source/led.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/state_machine.o \
./source/systick.o \
./source/tsi.o 

C_DEPS += \
./source/Buffahiti.d \
./source/led.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/state_machine.d \
./source/systick.d \
./source/tsi.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\CU_Files\MCUXpressoIDE_11.2.1_4149\workspace\Buffahiti\board" -I"C:\CU_Files\MCUXpressoIDE_11.2.1_4149\workspace\Buffahiti\source" -I"C:\CU_Files\MCUXpressoIDE_11.2.1_4149\workspace\Buffahiti" -I"C:\CU_Files\MCUXpressoIDE_11.2.1_4149\workspace\Buffahiti\drivers" -I"C:\CU_Files\MCUXpressoIDE_11.2.1_4149\workspace\Buffahiti\CMSIS" -I"C:\CU_Files\MCUXpressoIDE_11.2.1_4149\workspace\Buffahiti\utilities" -I"C:\CU_Files\MCUXpressoIDE_11.2.1_4149\workspace\Buffahiti\startup" -O0 -fno-common -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

