################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
embedded2020.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
embedded2020.list \

OBJCOPY_BIN += \
embedded2020.bin \


# All Target
all: embedded2020.elf secondary-outputs

# Tool invocations
embedded2020.elf: $(OBJS) $(USER_OBJS) /home/dante/Egyetem/MSc/1-felev/Architektúrák\ és\ beágyazott\ rendszerek/embedded2020/embedded2020/STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "embedded2020.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"/home/dante/Egyetem/MSc/1-felev/Architektúrák és beágyazott rendszerek/embedded2020/embedded2020/STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="embedded2020.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

embedded2020.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "embedded2020.list"
	@echo 'Finished building: $@'
	@echo ' '

embedded2020.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "embedded2020.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
