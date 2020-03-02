cmake_minimum_required (VERSION 3.5)

set(PROJECT_NAME coap-client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wundef -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_CURRENT_LIST_DIR}/../../../build/lib")

set(SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/../common/clientConnection.cc
				client.cc)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../common)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../include)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} libcoapcpp.a)
