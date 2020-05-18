set(TEST_PATH "tests/POSIX")
set(TEST1_PROJECT_NAME "test_packet")
set(TEST2_PROJECT_NAME "test_endpoint")
set(TEST3_PROJECT_NAME "test_logging")
set(TEST4_PROJECT_NAME "test_uri")
set(TEST5_PROJECT_NAME "coap_create")


set(TEST1_SRC_LIST ${TEST_PATH}/test_packet.cc)
set(TEST2_SRC_LIST ${TEST_PATH}/test_endpoint.cc)
set(TEST3_SRC_LIST ${TEST_PATH}/test_logging.cc)
set(TEST4_SRC_LIST ${TEST_PATH}/test_uri.cc)
set(TEST5_SRC_LIST ${TEST_PATH}/coap_create.cc)

add_executable(${TEST1_PROJECT_NAME} ${TEST1_SRC_LIST})
add_executable(${TEST2_PROJECT_NAME} ${TEST2_SRC_LIST})
add_executable(${TEST3_PROJECT_NAME} ${TEST3_SRC_LIST})
add_executable(${TEST4_PROJECT_NAME} ${TEST4_SRC_LIST})
add_executable(${TEST5_PROJECT_NAME} ${TEST5_SRC_LIST})

target_link_libraries(${TEST1_PROJECT_NAME} coapcpp)
target_link_libraries(${TEST2_PROJECT_NAME} coapcpp)
target_link_libraries(${TEST3_PROJECT_NAME} coapcpp)
target_link_libraries(${TEST4_PROJECT_NAME} coapcpp)
target_link_libraries(${TEST5_PROJECT_NAME} coapcpp)
