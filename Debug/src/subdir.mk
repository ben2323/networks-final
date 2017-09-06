################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MessengerClient.cpp \
../src/UDPMessenger.cpp \
../src/main.cpp 

OBJS += \
./src/MessengerClient.o \
./src/UDPMessenger.o \
./src/main.o 

CPP_DEPS += \
./src/MessengerClient.d \
./src/UDPMessenger.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -g -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/UDPMessenger.o: ../src/UDPMessenger.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -g -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/UDPMessenger.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


