# Open-source CoAP implementation in C++

## Introduction
lib-coap-cpp is an open-source implementation of Constrained Application Protocol (CoAP).
This is a library which can be linked with your source code.
This implementation also includes some examples of a server and a client.
It uses the library WolfSSL to provide a secure DTLS connection. The C++ 11 standard is used in a greater part of this library, specialy the server side.

## Requirements
To build the library lib-coap-cpp you shall use CMake version at least 3.5 and GNU Make version 4.1 or newer.
g++ is used as a C++ compiler. It shall be at least version 7.4.
Docker container technology can be used to build the library and examples, as well as to isolate the CoAP server written in C++.

## License
This library is provided under Apache license version 2.0.
LICENSE file contains license terms.
All third-party libraries are added as git modules and distributed under their own licenses.

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
**$ cd ~/lib-coap-cpp**
**$ make docker-build**

#### Run the Docker container
**$ make docker-run**

After you see the prompt like this [root@89b23f25cfc7 /]# you can select the
project directory and work with the project there :

**$ cd /opt/lib-coap-cpp**
**$ ./build.sh**

### 1.2.Build in a Unix-like operating system
First, make sure that you have installed utilities described above, such as cmake, gmake, g++.
Then you should open a command terminal and compile the project.

#### Compiling
**$ cd ~/lib-coap-cpp**
**$ make build**

#### Installation
**$ make install**

### 1.3.Build for other platforms using cross-compilation
Currently, two hardware platforms are supported such as:
* Raspberry Pi 3
* STM32H747I Discovery

Two compilation methods are available for Raspberry Pi 3 board, compilation on board and cross-compilation.
If you want to use compilation on board you should set POSIX value to variable TARGET in Makefile.

Otherside, you can compile library and examples on your Unix-like operation system using cross-compilation.
For this it is necessary to install ARM toolchain from here
https://github.com/raspberrypi/tools.

How to install and setup the toolchain you can read in README.md file from the directory examples/RASPBERRY-PI-3.

Other ARM toolchain called arm-none-eabi is used to build libarary and examples for STM32H747I Discovery board.
Here is used STM32CubeH7 library from manufacturer company STMicroelectronics.

How to install and use ARM toolchain and STM32CubeH7 library you can know from README.md located in examples/STM32H747I-DISCO directory.

#### Compiling
**$ cd lib-coap-cpp**
* Choose TARGET: POSIX, RASPBERRY-PI-3, STM32H747I-DISCO

###### For example TARGET is STM32H747I-DISCO

**$ make build TARGET=STM32H747I-DISCO**

#### Installation
To install the firmware you should copy it to the device

###### Example for Raspberry Pi 3:
**$ cd ~/lib-coap-cpp**
**$ scp build/examples/coap-client/coap-client /home/pi**

###### Example for STM32H747I Discovery:
**$ cd ~/lib-coap-cpp**
**$ cp build/examples/coap-client/coap-client.bin /run/media/$USER/DIS_H747XI**

## Quickstart
Let's consider an example of a coap client for POSIX.
The client loads some file from a COAP server using BLOCK-WISE transfers.

#### Server
Use an example cf-simplefile-server from the repository of Eclipse Californium as a COAP file server.
Read README.md from https://github.com/eclipse/californium to build the server.
Prepare some file to download from the server by COAP BLOCK-WISE protocol:
**$ mkdir -p ~/californium/demo-apps/cf-simplefile-server/target/data**
**$ cp example.bin ~/californium/demo-apps/cf-simplefile-server/target/data**


Then launch cf-simplefile-server using following sequense of commands:
**$ cd ~/californium/demo-apps/cf-simplefile-server/target**
**$ java -jar cf-simplefile-server-2.3.0-SNAPSHOT.jar**


#### Client
Build the whole lib-coap-cpp project or only examples if the library was built earlier:
**$ cd ~/lib-coap-cpp**
**$ make examples**


When the server is running you should launch the client:
**$ cd ~/lib-coap-cpp/build/examples/coap-client**
**$ ./coap-client -p 5683 -4 127.0.0.1 -f data/example.bin**


When the file is loaded you can see following message "Block-wise transfer completed".
If the file is not received you will see another message "An error occurred while receiving the file".
To read more details please open the log-file coap-client.log.
