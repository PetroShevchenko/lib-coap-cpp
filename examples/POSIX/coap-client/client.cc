#include "client.h"
#include "clientConnection.h"
#include "log.h"
#include <fstream>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <iostream>
#include <cstdio> 
#include <cstdlib> 
#include <unistd.h> 
#include <cstring>

using namespace coap;

const std::string ipv6_address_default = "[::1]";
const std::string ipv4_address_default = "127.0.0.1";

static bool quit = false;

int main(int argc, char ** argv)
{
	int return_code;
	fd_set read_descriptors;
	struct timeval tv;
	time_t receive_timeout;
	connection * udpConn1P;
	connection * udpConn2P;
	std::fstream file("coap-client.log", std::ios::out|std::ios::app);
	
	LOG_CREATE(ALL, file);

	auto free_up_resources = [&]()
	{
		file.close();
		LOG_DELETE;
		delete udpConn1P;
		delete udpConn2P;
	};

	try {		

		udpConn1P = new clientConnection(ipv4_address_default, 5673);
		udpConn2P = new clientConnection(ipv4_address_default, 5683);

		while(!quit)
		{
			FD_ZERO (&read_descriptors);
			FD_SET (STDIN_FILENO, &read_descriptors); // watch stdin (fd 0) to see when it has input
			FD_SET (udpConn1P->get_descriptor(), &read_descriptors); // watch socket descriptor to receive data from a client
			FD_SET (udpConn2P->get_descriptor(), &read_descriptors); // watch socket descriptor to receive data from a client

			tv.tv_sec = receive_timeout;
	        tv.tv_usec = 0;

	        //TODO
	        break;

		}	

	}catch(...)
	{
		std::cerr << "An exception happened" << std::endl;
		free_up_resources();
		exit(EXIT_FAILURE);	
	}
	free_up_resources();
    exit(EXIT_SUCCESS);
}