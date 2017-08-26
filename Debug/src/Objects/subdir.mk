################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Objects/Room.cpp \
../src/Objects/User.cpp 

OBJS += \
./src/Objects/Room.o \
./src/Objects/User.o 

CPP_DEPS += \
./src/Objects/Room.d \
./src/Objects/User.d 


# Each subdirectory must supply rules for building sources it contributes
src/Objects/%.o: ../src/Objects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


