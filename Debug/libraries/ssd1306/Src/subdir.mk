################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/ssd1306/Src/fonts.cpp \
../libraries/ssd1306/Src/format.cpp \
../libraries/ssd1306/Src/ssd1306.cpp 

OBJS += \
./libraries/ssd1306/Src/fonts.o \
./libraries/ssd1306/Src/format.o \
./libraries/ssd1306/Src/ssd1306.o 

CPP_DEPS += \
./libraries/ssd1306/Src/fonts.d \
./libraries/ssd1306/Src/format.d \
./libraries/ssd1306/Src/ssd1306.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/ssd1306/Src/%.o: ../libraries/ssd1306/Src/%.cpp libraries/ssd1306/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=c++17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../libraries/ssd1306/Inc -I../libraries/menu/Inc -I../libraries/button/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

