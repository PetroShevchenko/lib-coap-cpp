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
1. Ubuntu based Docker image
2. Debian based Docker image
3. Fedora based Docker image
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
* make

### 1.2.Build in Ubuntu Linux
TODO
## QuickstartYODO