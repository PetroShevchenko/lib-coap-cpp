cmake_minimum_required (VERSION 3.5)
project("POSIX_examples")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/coap-client)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/json-rpc-client)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/json-rpc-server)
