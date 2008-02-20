################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gen_hashmap.c \
../src/gen_vector.c 

OBJS += \
./src/gen_hashmap.o \
./src/gen_vector.o 

C_DEPS += \
./src/gen_hashmap.d \
./src/gen_vector.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/jon/Documents/workspace/genc/src" -I"/Users/jon/Documents/workspace/genc/src/test" -I"/Users/jon/Documents/workspace/genc/src/test/c" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


