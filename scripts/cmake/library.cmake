set(LIB_PROJECT_NAME "coapcpp")
set(LIB_SRC_LIST src/context.cc
				src/connection.cc
				src/packet.cc
				src/uri.cc
				src/uriClientEndpoint.cc
				src/uriServerEndpoint.cc)

project (${LIB_PROJECT_NAME})
add_library(${LIB_PROJECT_NAME} ${LIB_SRC_LIST})
