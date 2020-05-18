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
.PHONY:	test project all clean

#--------------------------------------------------------------------------
#	begin of user settings
#--------------------------------------------------------------------------
VERSION					:= Debug
#VERSION				:= Release

PRJ_NAME				:= tests

STM32_CUBE_H7_PATH		:= ../../libs/third-party/STM32CubeH7

MCU_CORE 				:= CORE_CM7
MCU_SERIES				:= STM32H7xx
MCU_DEVICE 				:= STM32H747xx

BOARD_NAME 				:= STM32H747I-DISCO

LD_SCRIPT 				:= stm32h747xx_flash_CM7.ld

#--------------------------------------------------------------------------
# CMSIS library
#--------------------------------------------------------------------------
CMSIS_PATH				:= $(STM32_CUBE_H7_PATH)/Drivers/CMSIS

LD_SCRIPT_PATH 			:= $(CMSIS_PATH)/Device/ST/$(MCU_SERIES)/Source/Templates/gcc/linker

INCLUDE_PATH			+= $(CMSIS_PATH)/Include
INCLUDE_PATH			+= $(CMSIS_PATH)/Device/ST/$(MCU_SERIES)/Include

VPATH					+= $(CMSIS_PATH)/Device/ST/$(MCU_SERIES)/Source/Templates
VPATH					+= $(CMSIS_PATH)/Device/ST/$(MCU_SERIES)/Source/Templates/gcc

#--------------------------------------------------------------------------
# BSP (Board Support Package)
#--------------------------------------------------------------------------
BSP_PATH 				:= $(STM32_CUBE_H7_PATH)/Drivers/BSP/$(BOARD_NAME)
INCLUDE_PATH			+= $(BSP_PATH)
INCLUDE_PATH 			+= $(STM32_CUBE_H7_PATH)/Projects/$(BOARD_NAME)/Templates/BootCM4_CM7/Common/Inc

#--------------------------------------------------------------------------
# STM32 HAL library
#--------------------------------------------------------------------------
HAL_PATH 				:= $(STM32_CUBE_H7_PATH)/Drivers/STM32H7xx_HAL_Driver

VPATH 					+= $(HAL_PATH)/Src
INCLUDE_PATH			+= $(HAL_PATH)/Inc

SRC_HAL					:= stm32h7xx_hal_adc.c
SRC_HAL					+= stm32h7xx_hal_eth.c
SRC_HAL					+= stm32h7xx_hal_lptim.c
SRC_HAL					+= stm32h7xx_hal_ramecc.c
SRC_HAL					+= stm32h7xx_hal_tim.c
SRC_HAL					+= stm32h7xx_hal_adc_ex.c
SRC_HAL					+= stm32h7xx_hal_eth_ex.c
SRC_HAL					+= stm32h7xx_hal_ltdc.c
SRC_HAL					+= stm32h7xx_hal_rcc.c
SRC_HAL					+= stm32h7xx_hal.c
SRC_HAL					+= stm32h7xx_hal_exti.c
SRC_HAL					+= stm32h7xx_hal_ltdc_ex.c
SRC_HAL					+= stm32h7xx_hal_rcc_ex.c
SRC_HAL					+= stm32h7xx_hal_cec.c
SRC_HAL					+= stm32h7xx_hal_fdcan.c
SRC_HAL					+= stm32h7xx_hal_mdios.c
SRC_HAL					+= stm32h7xx_hal_rng.c
SRC_HAL					+= stm32h7xx_hal_comp.c
SRC_HAL					+= stm32h7xx_hal_flash.c
SRC_HAL					+= stm32h7xx_hal_mdma.c
SRC_HAL					+= stm32h7xx_hal_rng_ex.c
SRC_HAL					+= stm32h7xx_hal_tim_ex.c
SRC_HAL					+= stm32h7xx_hal_cortex.c
SRC_HAL					+= stm32h7xx_hal_flash_ex.c
SRC_HAL					+= stm32h7xx_hal_mmc.c
SRC_HAL					+= stm32h7xx_hal_rtc.c
SRC_HAL					+= stm32h7xx_hal_uart.c
SRC_HAL					+= stm32h7xx_hal_crc.c
SRC_HAL					+= stm32h7xx_hal_gfxmmu.c
SRC_HAL					+= stm32h7xx_hal_mmc_ex.c
SRC_HAL					+= stm32h7xx_hal_rtc_ex.c
SRC_HAL					+= stm32h7xx_hal_uart_ex.c
SRC_HAL					+= stm32h7xx_hal_crc_ex.c
SRC_HAL					+= stm32h7xx_hal_gpio.c
SRC_HAL					+= stm32h7xx_hal_sai.c
SRC_HAL					+= stm32h7xx_hal_usart.c
SRC_HAL					+= stm32h7xx_hal_cryp.c
SRC_HAL					+= stm32h7xx_hal_hash.c
SRC_HAL					+= stm32h7xx_hal_nand.c
SRC_HAL					+= stm32h7xx_hal_sai_ex.c
SRC_HAL					+= stm32h7xx_hal_usart_ex.c
SRC_HAL					+= stm32h7xx_hal_cryp_ex.c
SRC_HAL					+= stm32h7xx_hal_hash_ex.c
SRC_HAL					+= stm32h7xx_hal_nor.c
SRC_HAL					+= stm32h7xx_hal_sd.c
SRC_HAL					+= stm32h7xx_hal_wwdg.c
SRC_HAL					+= stm32h7xx_hal_dac.c
SRC_HAL					+= stm32h7xx_hal_hcd.c
SRC_HAL					+= stm32h7xx_hal_opamp.c
SRC_HAL					+= stm32h7xx_hal_sd_ex.c
SRC_HAL					+= stm32h7xx_hal_dac_ex.c
SRC_HAL					+= stm32h7xx_hal_hrtim.c
SRC_HAL					+= stm32h7xx_hal_opamp_ex.c
SRC_HAL					+= stm32h7xx_hal_sdram.c
SRC_HAL					+= stm32h7xx_hal_dcmi.c
SRC_HAL					+= stm32h7xx_hal_hsem.c
SRC_HAL					+= stm32h7xx_hal_ospi.c
SRC_HAL					+= stm32h7xx_hal_smartcard.c
SRC_HAL					+= stm32h7xx_hal_dfsdm.c
SRC_HAL					+= stm32h7xx_hal_i2c.c
SRC_HAL					+= stm32h7xx_hal_otfdec.c
SRC_HAL					+= stm32h7xx_hal_smartcard_ex.c
SRC_HAL					+= stm32h7xx_hal_dfsdm_ex.c
SRC_HAL					+= stm32h7xx_hal_i2c_ex.c
SRC_HAL					+= stm32h7xx_hal_pcd.c
SRC_HAL					+= stm32h7xx_hal_smbus.c
SRC_HAL					+= stm32h7xx_hal_dma2d.c
SRC_HAL					+= stm32h7xx_hal_i2s.c
SRC_HAL					+= stm32h7xx_hal_pcd_ex.c
SRC_HAL					+= stm32h7xx_hal_spdifrx.c
SRC_HAL					+= stm32h7xx_hal_dma.c
SRC_HAL					+= stm32h7xx_hal_i2s_ex.c
SRC_HAL					+= stm32h7xx_hal_pssi.c
SRC_HAL					+= stm32h7xx_hal_spi.c
SRC_HAL					+= stm32h7xx_hal_dma_ex.c
SRC_HAL					+= stm32h7xx_hal_irda.c
SRC_HAL					+= stm32h7xx_hal_pwr.c
SRC_HAL					+= stm32h7xx_hal_spi_ex.c
SRC_HAL					+= stm32h7xx_hal_dsi.c
SRC_HAL					+= stm32h7xx_hal_iwdg.c
SRC_HAL					+= stm32h7xx_hal_pwr_ex.c
SRC_HAL					+= stm32h7xx_hal_sram.c
SRC_HAL					+= stm32h7xx_hal_dts.c
SRC_HAL					+= stm32h7xx_hal_jpeg.c
SRC_HAL					+= stm32h7xx_hal_qspi.c
SRC_HAL					+= stm32h7xx_hal_swpmi.c

#--------------------------------------------------------------------------
# FreeRTOS library
#--------------------------------------------------------------------------
RTOS_PATH				:= $(STM32_CUBE_H7_PATH)/Middlewares/Third_Party/FreeRTOS/Source

VPATH					+= $(RTOS_PATH)
VPATH					+= $(RTOS_PATH)/portable/MemMang
VPATH					+= $(RTOS_PATH)/portable/GCC/ARM_CM4F
VPATH					+= $(RTOS_PATH)/CMSIS_RTOS_V2

INCLUDE_PATH			+= $(VPATH)
INCLUDE_PATH			+= $(RTOS_PATH)/include

SRC_RTOS				:= heap_4.c
SRC_RTOS				+= port.c
SRC_RTOS				+= cmsis_os2.c
SRC_RTOS				+= croutine.c
SRC_RTOS				+= event_groups.c
SRC_RTOS				+= list.c
SRC_RTOS				+= queue.c
SRC_RTOS				+= stream_buffer.c
SRC_RTOS				+= tasks.c
SRC_RTOS				+= timers.c

#--------------------------------------------------------------------------
# User sources
#--------------------------------------------------------------------------
PROJECT_PATH			:= ../../tests/STM32H747I-DISCO

INCLUDE_PATH 			+= ../../include
INCLUDE_PATH 			+= ../../include/stm32h747i-disco/freertos
INCLUDE_PATH 			+= ../../include/stm32h747i-disco/hal
INCLUDE_PATH 			+= ../../include/stm32h747i-disco/lwip


LIB						+= coapcpp
LIB_PATH 				+= ../../build/lib

SRC_PATH 				:= $(PROJECT_PATH)/src
INCLUDE_PATH 			+= $(PROJECT_PATH)/include

VPATH 					+= $(SRC_PATH)

SRC 					:= $(SRC_HAL)
SRC 					+= $(SRC_RTOS)
SRC 					+= startup_stm32h747xx.s
SRC 					+= system_stm32h7xx_dualcore_boot_cm4_cm7.c
SRC 					+= rtc.c
SRC 					+= drivers.c
SRC 					+= main.cc
SRC 					+= stm32h7xx_hal_msp.c
SRC 					+= stm32h7xx_hal_timebase_tim.c
SRC 					+= stm32h7xx_it.c
SRC 					+= syscalls.c
SRC 					+= sysmem.c


LIB						:= stdc++

OPTIMIZE_LEVEL			:= 2
DEBUG_LEVEL				:= gdb

TOOL_PATH				:= '$(HOME)/gcc-arm-none-eabi-9-2019-q4-major'

DEFINE					:= USE_HAL_DRIVER
DEFINE					+= $(MCU_DEVICE)
DEFINE					+= $(MCU_CORE)

CCFLAGS 				:= -mcpu=cortex-m7
CCFLAGS 				+= -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb
CCFLAGS 				+= -specs=nano.specs


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

INC_DIR					:= include
SRC_DIR					:= src

VPATH 					+= $(SRC_DIR)

INCLUDE_PATH			+= $(INC_DIR) $(TOOL_PATH)/include
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
CPPCFLAGS				+= -std=c++11

LDFLAGS					:= -T$(LD_SCRIPT_PATH)/$(LD_SCRIPT)
LDFLAGS					+= -Wl,-Map,$(IMAGE).map,--cref -Wl,--gc-sections
LDFLAGS 				+= -Wl,--start-group -lc -lm -Wl,--end-group

OBJECTS					:= $(patsubst %.c, %.o,$(filter %.c,$(SRC)))
OBJECTS					+= $(patsubst %.cpp, %.o,$(filter %.cpp,$(SRC)))
OBJECTS					+= $(patsubst %.cc, %.o,$(filter %.cc,$(SRC)))
OBJECTS					+= $(patsubst %.s, %.o,$(filter %.s,$(SRC)))
OBJECTS					+= $(patsubst %.S, %.o,$(filter %.S,$(SRC)))

#--------------------------------------------------------------------------
#	targets
#--------------------------------------------------------------------------

test:
	$(CC) --version
	$(MAKE) --version
	@echo $(OBJECTS)


all: $(VERSION)

$(VERSION): elf bin hex s19 size

elf:$(IMAGE).elf

lst:$(IMAGE).lst

bin:$(IMAGE).bin

hex:$(IMAGE).hex

s19:$(IMAGE).s19

size:$(IMAGE).elf
	@echo $@
	$(SIZE) $(IMAGE).elf

$(IMAGE).bin:$(IMAGE).elf
	@echo $@
	$(OBJCOPY) -O binary $< $@

$(IMAGE).hex:$(IMAGE).elf
	@echo $@
	$(OBJCOPY) -O ihex $< $@

$(IMAGE).s19:$(IMAGE).elf
	@echo $@
	$(OBJCOPY) -O srec $< $@

$(IMAGE).lst:$(IMAGE).elf
	@echo $@
	$(OBJDUMP) -h -S -z $<  > $@

$(IMAGE).elf:$(OBJECTS)
	@echo $@
	$(LD) $(CCFLAGS) $(LDFLAGS) $^ -o $@ $(LIBFLAGS)

%.o:%.c
	@echo $<
	$(CC) $(CCFLAGS) $(CONLYFLAGS) -MD -c $< -o $@

%.o:%.cpp
	@echo $<
	$(CPPC) $(CPPCFLAGS) -MD -c $< -o $@

%.o:%.cc
	@echo $<
	$(CPPC) $(CPPCFLAGS) -MD -c $< -o $@

%.o:%.s
	@echo $<
	$(AS) $(ASFLAGS) -c $< -o $@

%.o:%.S
	@echo $<
	$(AS) $(ASFLAGS) -c $< -o $@

include $(wildcard $(OBJ_DIR)/$(VERSION)/*.d)

clean:
	rm -f $(OBJECTS)
	rm -f $(patsubst %.o, %.d,$(OBJECTS))
	rm -f $(IMAGE) $(IMAGE).map $(IMAGE).lst

#==========================================================================
#	End Of File
#==========================================================================
