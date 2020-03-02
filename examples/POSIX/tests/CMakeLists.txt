cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wundef -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

include_directories(${CMAKE_CURRENT_LIST_DIR}/../common)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../include)

set(COMMON_SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/../common/clientConnection.cc)

set(POSIX_EXAMPLE_TEST1_NAME "test_client_connection")

set(POSIX_EXAMPLE_TEST1_SRC_LIST ${COMMON_SRC_LIST} ${CMAKE_CURRENT_LIST_DIR}/test_clientConnection.cc)

project(${POSIX_EXAMPLE_TEST1_NAME})

add_executable(${POSIX_EXAMPLE_TEST1_NAME} ${POSIX_EXAMPLE_TEST1_SRC_LIST})

target_link_libraries(${POSIX_EXAMPLE_TEST1_NAME} ${LIB_PROJECT_NAME})