# Cross-compiling the library for STM32H747I-DISCO board on Unix-like host machine

## Toolchain

You can download arm-none-eabi-g++ and other utilities for cross-compilation by the link https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads.
To install toolchain after downloading perform following steps:
* unpack downloaded tolchain

######$ tar -xvjf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2

* copy unpacked toolchain to your home directory or other place

$ cp -r gcc-arm-none-eabi-9-2019-q4-major ~

* add a directory to your toolchain in .bashrc file

######$ vim ~/.bashrc
export PATH=$HOME/gcc-arm-none-eabi-9-2019-q4-major/bin:$PATH

* check installed toolchain in a new terminal window

######$ arm-none-eabi-g++ --version
arm-none-eabi-g++ (GNU Tools for Arm Embedded Processors 9-2019-q4-major) 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599]
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## STM32CubeH7 library

A project example for STM32H747I-DISCO board uses STM32CubeH7 library from STMicroelectronics that is implemented in C language.
To download this library perform following steps:

######$ cd ~/lib-coap-cpp

######$ git submodule init

######$ git submodule update

After that the whole third-party libraries will be installed.

Then you should switch to master branch:

######$ cd libs/third-party/STM32CubeH7

######$ git checkout master

### Build

To build the library and examples use following commands:

######$ cd ~/lib-coap-cpp

######$ make build TARGET=STM32H747I-DISCO

To clean object files and static library you should perform command:

######$ make clean TARGET=STM32H747I-DISCO

### Install

To install the firmware you should copy it to the device.
Perform following steps:
* connect ST-Link USB connector by micro-USB cable to your PC

After that you will be able to see a new virtual disk "DIS_H747XI".
* copy binary file with firmware to the device

######$ cd ~/lib-coap-cpp

######$ cp build/examples/coap-client/coap-client.bin /run/media/$USER/DIS_H747XI
