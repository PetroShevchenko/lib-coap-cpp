#include "client.h"
#include "clientConnection.h"
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

static bool quit = false;

int main(int argc, char ** argv)
{
	int return_code;
	fd_set read_descriptors;
	struct timeval tv;
	time_t receive_timeout;

	clientConnection udpConnection1("0.0.0.0", 5673);
	clientConnection udpConnection2("0.0.0.0", 5683);

	while(!quit)
	{
		FD_ZERO (&read_descriptors);
		FD_SET (STDIN, &read_descriptors); // watch stdin (fd 0) to see when it has input
		FD_SET (udpConnection1.get_descriptor(), &read_descriptors); // watch socket descriptor to receive data from a client
		FD_SET (udpConnection2.get_descriptor(), &read_descriptors); // watch socket descriptor to receive data from a client

		tv.tv_sec = receive_timeout;
        tv.tv_usec = 0;

	}

    exit(EXIT_SUCCESS);
}