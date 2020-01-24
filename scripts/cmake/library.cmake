cmake_minimum_required (VERSION 3.5)

set(LIB_PROJECT_NAME "coapcpp")
set(LIB_SRC_LIST src/context.cc
				src/packet.cc
				src/uri.cc)

project (${LIB_PROJECT_NAME})
add_library(${LIB_PROJECT_NAME} ${LIB_SRC_LIST})