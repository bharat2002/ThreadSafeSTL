################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Main.cpp \
../src/ThreadSafeVector.cpp 

CPP_DEPS += \
./src/Main.d \
./src/ThreadSafeVector.d 

OBJS += \
./src/Main.o \
./src/ThreadSafeVector.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Main.d ./src/Main.o ./src/ThreadSafeVector.d ./src/ThreadSafeVector.o

.PHONY: clean-src

