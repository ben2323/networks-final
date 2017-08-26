################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Handlers/InfoHandler.cpp \
../src/Handlers/SessionsHandler.cpp 

OBJS += \
./src/Handlers/InfoHandler.o \
./src/Handlers/SessionsHandler.o 

CPP_DEPS += \
./src/Handlers/InfoHandler.d \
./src/Handlers/SessionsHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/Handlers/%.o: ../src/Handlers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


