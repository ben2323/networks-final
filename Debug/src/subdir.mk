################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MessengerClient.cpp \
../src/UDPMessenger.cpp \
../src/client_2.cpp \
../src/main.cpp 

OBJS += \
./src/MessengerClient.o \
./src/UDPMessenger.o \
./src/client_2.o \
./src/main.o 

CPP_DEPS += \
./src/MessengerClient.d \
./src/UDPMessenger.d \
./src/client_2.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


