################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SIM300SendReceiveMsgs.c \
../USART.c \
../lcd.c \
../sim300.c 

C_DEPS += \
./SIM300SendReceiveMsgs.d \
./USART.d \
./lcd.d \
./sim300.d 

OBJS += \
./SIM300SendReceiveMsgs.o \
./USART.o \
./lcd.o \
./sim300.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


