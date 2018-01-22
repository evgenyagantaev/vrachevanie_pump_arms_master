################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/bluetooth_time_relay_object.c \
../Src/bluetooth_time_relay_task.c \
../Src/gpio.c \
../Src/inflator_obj.c \
../Src/input_analizer_obj.c \
../Src/main.c \
../Src/one_hz_timer_obj.c \
../Src/pressure_sensor_obj.c \
../Src/stm32l0xx_hal_msp.c \
../Src/stm32l0xx_it.c \
../Src/tim.c \
../Src/usart.c \
../Src/usart_obj.c 

OBJS += \
./Src/adc.o \
./Src/bluetooth_time_relay_object.o \
./Src/bluetooth_time_relay_task.o \
./Src/gpio.o \
./Src/inflator_obj.o \
./Src/input_analizer_obj.o \
./Src/main.o \
./Src/one_hz_timer_obj.o \
./Src/pressure_sensor_obj.o \
./Src/stm32l0xx_hal_msp.o \
./Src/stm32l0xx_it.o \
./Src/tim.o \
./Src/usart.o \
./Src/usart_obj.o 

C_DEPS += \
./Src/adc.d \
./Src/bluetooth_time_relay_object.d \
./Src/bluetooth_time_relay_task.d \
./Src/gpio.d \
./Src/inflator_obj.d \
./Src/input_analizer_obj.d \
./Src/main.d \
./Src/one_hz_timer_obj.d \
./Src/pressure_sensor_obj.d \
./Src/stm32l0xx_hal_msp.d \
./Src/stm32l0xx_it.d \
./Src/tim.d \
./Src/usart.d \
./Src/usart_obj.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32L051xx -I"C:/workspace/vrachevanie/vrachevanie_pump_arms_master/bpzu_00_01/Inc" -I"C:/workspace/vrachevanie/vrachevanie_pump_arms_master/bpzu_00_01/Drivers/STM32L0xx_HAL_Driver/Inc" -I"C:/workspace/vrachevanie/vrachevanie_pump_arms_master/bpzu_00_01/Drivers/STM32L0xx_HAL_Driver/Inc/Legacy" -I"C:/workspace/vrachevanie/vrachevanie_pump_arms_master/bpzu_00_01/Drivers/CMSIS/Include" -I"C:/workspace/vrachevanie/vrachevanie_pump_arms_master/bpzu_00_01/Drivers/CMSIS/Device/ST/STM32L0xx/Include"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


