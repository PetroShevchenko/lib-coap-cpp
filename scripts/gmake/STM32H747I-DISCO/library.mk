#==========================================================================
#
# Copyright (C) 2020 Petro Shevchenko <developer@cxemotexnika.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#==========================================================================
.PHONY:	test all clean

#--------------------------------------------------------------------------
#	begin of user settings
#--------------------------------------------------------------------------
VERSION					:= Debug
#VERSION				:= Release

PRJ_NAME				:= coapcpp

MCU_CORE 				:= CORE_CM7
MCU_SERIES				:= STM32H7xx
MCU_DEVICE 				:= STM32H747xx

BOARD_NAME 				:= STM32H747I_DISCO

#--------------------------------------------------------------------------
# User sources
#--------------------------------------------------------------------------
SRC_PATH 				:= ../../src

STM32_CUBE_H7_PATH		:= ../../libs/third-party/STM32CubeH7
LWIP_PATH 				:= $(STM32_CUBE_H7_PATH)/Middlewares/Third_Party/LwIP
RTOS_PATH 				:= $(STM32_CUBE_H7_PATH)/Middlewares/Third_Party/FreeRTOS/Source
HAL_PATH 				:= $(STM32_CUBE_H7_PATH)/Drivers/STM32H7xx_HAL_Driver
CMSIS_PATH				:= $(STM32_CUBE_H7_PATH)/Drivers/CMSIS


VPATH					+= $(SRC_PATH)
VPATH					+= $(SRC_PATH)/json-rpc
VPATH					+= $(SRC_PATH)/lwm2m-core

INCLUDE_PATH			+= ../../include
INCLUDE_PATH			+= ../../include/stm32h747i-disco/lwip
INCLUDE_PATH			+= ../../include/stm32h747i-disco/freertos
INCLUDE_PATH			+= ../../include/stm32h747i-disco/hal
INCLUDE_PATH			+= $(LWIP_PATH)/src/include
INCLUDE_PATH			+= $(LWIP_PATH)/system
INCLUDE_PATH			+= $(RTOS_PATH)/CMSIS_RTOS_V2
INCLUDE_PATH			+= $(RTOS_PATH)/include
INCLUDE_PATH			+= $(RTOS_PATH)/portable/MemMang
INCLUDE_PATH			+= $(RTOS_PATH)/portable/GCC/ARM_CM4F
INCLUDE_PATH			+= $(HAL_PATH)/Inc
INCLUDE_PATH			+= $(CMSIS_PATH)/Include
INCLUDE_PATH			+= $(CMSIS_PATH)/Device/ST/$(MCU_SERIES)/Include

SRC 					:= blockwise.cc
SRC 					+= connection.cc
SRC 					+= context.cc
SRC 					+= packet.cc
SRC 					+= uri.cc
SRC 					+= uriClientEndpoint.cc
SRC 					+= uriServerEndpoint.cc

LIB						:= stdc++

OPTIMIZE_LEVEL			:= 2
DEBUG_LEVEL				:= gdb

TOOL_PATH				:= '$(HOME)/gcc-arm-none-eabi-9-2019-q4-major'
SHELL					:= '/usr/bin/bash'

DEFINE					:= USE_HAL_DRIVER
DEFINE					+= $(MCU_DEVICE)
DEFINE					+= $(MCU_CORE)
DEFINE 					+= $(BOARD_NAME)

CCFLAGS 				:= -mcpu=cortex-m7
CCFLAGS 				+= -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb
CCFLAGS 				+= -specs=nano.specs
#LIBFLAGS				:=


#--------------------------------------------------------------------------
#	end of user settings
#--------------------------------------------------------------------------

CROSS_PREFIX			:= arm-none-eabi-

ROOT					:= $(shell pwd)

CC						:= $(CROSS_PREFIX)gcc
AS						:= $(CROSS_PREFIX)g++
LD						:= $(CROSS_PREFIX)g++
CPPC					:= $(CROSS_PREFIX)g++
AR						:= $(CROSS_PREFIX)ar
RANLIB					:= $(CROSS_PREFIX)ranlib
SIZE					:= $(CROSS_PREFIX)size
OBJCOPY					:= $(CROSS_PREFIX)objcopy
OBJDUMP					:= $(CROSS_PREFIX)objdump


INCLUDE_PATH			+= $(TOOL_PATH)/include
LIB_PATH				+= $(TOOL_PATH)/lib

LIBFLAGS				+= $(addprefix -L,$(LIB_PATH)) $(addprefix -l,$(LIB))
ARFLAGS					:= rcs

CCFLAGS					+= -Wall
CCFLAGS					+= $(addprefix -D,$(DEFINE)) $(addprefix -I,$(INCLUDE_PATH))
CCFLAGS					+= -ffunction-sections -fdata-sections

CONLYFLAGS				+= -std=gnu11

ASFLAGS					+= -Wall
ASFLAGS					+= $(addprefix -D,$(DEFINE)) $(addprefix -I,$(INCLUDE_PATH))

ifeq ($(VERSION),Debug)
IMAGE					:= $(PRJ_NAME)
CCFLAGS					+= -g$(DEBUG_LEVEL) -O0 -DDEBUG
ASFLAGS					+= -g$(DEBUG_LEVEL) -O0 -DDEBUG
endif

ifeq ($(VERSION),Release)
IMAGE					:= $(PRJ_NAME)
CCFLAGS					+= -O$(OPTIMIZE_LEVEL)
ASFLAGS					+= -O0
endif

CPPCFLAGS				+= $(CCFLAGS)
CPPCFLAGS				+= -Weffc++ -Wextra -Wpedantic -Wshadow -Wundef -Wno-missing-field-initializers
CPPCFLAGS				+= -std=c++14

LDFLAGS					:= -T$(LD_SCRIPT_PATH)/$(LD_SCRIPT)
LDFLAGS					+= -Wl,-Map,$(IMAGE).map,--cref -Wl,--gc-sections
LDFLAGS 				+= -Wl,--start-group -lc -lm -Wl,--end-group

OBJECTS					:= $(patsubst %.c, %.o,$(filter %.c,$(SRC)))
OBJECTS					+= $(patsubst %.cpp, %.o,$(filter %.cpp,$(SRC)))
OBJECTS					+= $(patsubst %.cc, %.o,$(filter %.cc,$(SRC)))
OBJECTS					+= $(patsubst %.s, %.o,$(filter %.s,$(SRC)))
OBJECTS					+= $(patsubst %.S, %.o,$(filter %.S,$(SRC)))

LIBRARY 				:= lib$(PRJ_NAME).a

#--------------------------------------------------------------------------
#	targets
#--------------------------------------------------------------------------

test:
	$(CC) --version
	$(MAKE) --version
	@echo $(OBJECTS)

all: $(VERSION)

$(VERSION): lib size

lib: $(LIBRARY)
	$(RANLIB) $(LIBRARY)

lst:$(IMAGE).lst

$(IMAGE).lst: $(LIBRARY)
	@echo $@
	$(OBJDUMP) -D $< > $@

size: $(LIBRARY)
	@echo $@
	$(SIZE) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	@echo $@
	$(AR) $(ARFLAGS) $@ $^

%.o:%.c
	@echo $<
	$(CC) $(CCFLAGS) $(CONLYFLAGS) -MD -c $< -o $@

%.o:%.cpp
	$(CPPC) $(CPPCFLAGS) -MD -c $< -o $@

%.o:%.cc
	@echo $<
	$(CPPC) $(CPPCFLAGS) -MD -c $< -o $@

%.o:%.s
	$(AS) $(ASFLAGS) -c $< -o $@

%.o:%.S
	@echo $<
	$(AS) $(ASFLAGS) -c $< -o $@

include $(wildcard *.d)

clean:
	rm -f $(OBJECTS)
	rm -f $(patsubst %.o, %.d,$(OBJECTS))
	rm -f $(LIBRARY) $(IMAGE).map $(IMAGE).lst

#==========================================================================
#	End Of File
#==========================================================================
