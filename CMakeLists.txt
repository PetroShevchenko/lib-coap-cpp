cmake_minimum_required (VERSION 3.5)

set(PROJECT_NAME "test_log")
set(SRC_LIST tests/test_logging.cc
				src/packet.cc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wundef -Wno-missing-field-initializers")

project (${PROJECT_NAME})

include_directories(include)

add_executable(${PROJECT_NAME} ${SRC_LIST})
