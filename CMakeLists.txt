cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wundef -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(LIB_PROJECT_NAME "coapcpp")
set(LIB_SRC_LIST src/context.cc
				src/connection.cc
				src/packet.cc
				src/uri.cc
				src/uriClientEndpoint.cc
				src/uriServerEndpoint.cc
				src/blockwise.cc)

project (${LIB_PROJECT_NAME})

add_library(${LIB_PROJECT_NAME} STATIC ${LIB_SRC_LIST})

include(${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/tests.cmake)
