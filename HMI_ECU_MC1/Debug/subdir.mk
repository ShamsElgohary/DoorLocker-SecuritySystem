################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HMI_ECU.c \
../keypad.c \
../lcd.c \
../timer.c \
../uart.c 

C_DEPS += \
./HMI_ECU.d \
./keypad.d \
./lcd.d \
./timer.d \
./uart.d 

OBJS += \
./HMI_ECU.o \
./keypad.o \
./lcd.o \
./timer.o \
./uart.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


