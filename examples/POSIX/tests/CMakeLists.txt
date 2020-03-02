cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wundef -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_CURRENT_LIST_DIR}/../../../build/lib")

set(COMMON_SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/../common/clientConnection.cc)

set(POSIX_EXAMPLE_TEST1_NAME "test_client_connection")

set(POSIX_EXAMPLE_TEST1_SRC_LIST test_clientConnection.cc ${COMMON_SRC_LIST})

include_directories(${CMAKE_CURRENT_LIST_DIR}/../common)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../include)

project(${POSIX_EXAMPLE_TEST1_NAME})

add_executable(${POSIX_EXAMPLE_TEST1_NAME} ${POSIX_EXAMPLE_TEST1_SRC_LIST})
target_link_libraries(${PROJECT_NAME} libcoapcpp.a)