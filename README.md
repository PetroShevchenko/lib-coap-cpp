# A C++ open-source CoAP implementation

## Introduction
lib-coap-cpp is an open-source implementation of Constrained Application Protocol (CoAP).
This is a library which can be linked with your source code.
This implementation also includes some examples of a server and a client.
It uses the library WolfSSL to provide a secure DTLS connection. The C++ 11 standard is used in a greater part of this library, specialy the server side.

## Requirements
To build the library lib-coap-cpp you shall use CMake version at least 3.5 and GNU Make version 4.1 or newer.
g++ is used as a C++ compiler. It shall be at least version 7.4.
Docker container technology can be used to build the library and examples, as well as to isolate the CoAP server written in C++.

## Build \& Install
### 1.1.Build using docker containers
If you want to use building in a Docker container you should firstly install Docker.
How it perform you can read in the Docker documentation by link https://docs.docker.com/.
You are given a choice of three options for building a Docker image :
* Ubuntu based Docker image
* Debian based Docker image
* Fedora based Docker image

To select one of the build options, select the name of the Docker file in the Makefile.
Default file name is dockerfile.fedora.

#### Compiling
* cd lib-coap-cpp
* make docker-build

#### Run the Docker container
* make docker-run

After you see the prompt like this [root@89b23f25cfc7 /]# you can select the
project directory and work with the project there :

* cd /opt/lib-coap-cpp
* ./build.sh

### 1.2.Build in a Unix-like operating system
Firstly make sure that you have installed utilities described above, such as cmake, gmake, g++.
Then you should open a command terminal and compile the project.

#### Compiling
* cd lib-coap-cpp
* make build

#### Installation
* make install

### 1.3.Build for other platforms using cross-compilation
Currently, two hardware platforms are supported such as:
* Raspberry Pi 3
* STM32H747I Discovery

Two compilation methods are available for Raspberry Pi 3 board, compilation on board and cross-compilation.
If you want to use compilation on board you should set POSIX value to variable TARGET in Makefile.
Otherside, you can compile library and examples on your Linux PC using cross-compilation.
For this it is necessary to install ARM toolchain from here https://github.com/raspberrypi/tools.
How to install and setup the toolchain you can read by link
https://www.raspberrypi.org/documentation/linux/kernel/building.md.
ARM mbed-cli utility is used to build libarary and examples for STM32H747I Discovery board.
How to install mbed-cli you can know by link https://os.mbed.com/docs/mbed-os/v5.15/tools/manual-installation.html.

#### Compiling
* cd lib-coap-cpp
* Open Makefile and choose one of available values for variable TARGET: POSIX, RASPBERRY-PI-3, STM32H747I-DISCO

##### For example:
TARGET := STM32H747I-DISCO

* make build

#### Installation
To install the firmware you should copy it to the device

##### Example for Raspberry Pi 3:
cd lib-coap-cpp
scp build/examples/coap-client/coap-client /home/pi

##### Example for STM32H747I Discovery:
cd lib-coap-cpp
cp build/examples/coap-client/coap-client.bin /media/xxx

## Quickstart
TODO