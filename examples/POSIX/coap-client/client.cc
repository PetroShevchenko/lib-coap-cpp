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
#include <getopt.h>
#include <cstring>

#include "client.h"
#include "clientConnection.h"
#include "log.h"

using namespace std;
using namespace coap;

const std::string ipv6_address_default = "[::1]";
const std::string ipv4_address_default = "127.0.0.1";
const unsigned int server_port_default = 5683;
const unsigned int client_port_default = 56083;
const std::string command_file_name_default = "coap-commands.run";

static bool quit = false;
static unsigned int server_port = server_port_default;
static unsigned int client_port = client_port_default;
static bool use_ipv6 = true;
static bool use_interactive_mode = true;
static std::string ip_address = ipv6_address_default;
static std::string command_file_name = command_file_name_default;

static void usage()
{
	std::cerr << "Usage: coap-client [OPTIONS]" << std::endl;
	std::cerr << "Launch a CoAP client" << std::endl;
	std::cerr << "Options:" << std::endl;
	std::cerr << "-h,--help\t\t\tshow this message and exit" << std::endl;
	std::cerr << "-p,--port\t<PORT_NUMBER>\tset the port number of the CoAP server. Default: 5683" << std::endl;
	std::cerr << "-4,--ipv4\t<ADDRESS>\tset the IPv4 address of the CoAP server. Default: 127.0.0.1" << std::endl;
	std::cerr << "-6,--ipv6\t<ADDRESS>\tset the IPv6 address of the CoAP server. Default: [::1]" << std::endl;
	std::cerr << "-c,--client\t<PORT_NUMBER>\tset the client port number. Default: 56083" << std::endl;
	std::cerr << "-i,--interactive\t\tset the interactive mode of client running" << std::endl;
	std::cerr << "-f,--file\t<FILE_NAME>\tset the name of the file containing the sequence of commands" << std::endl;
}

static bool parse_arguments(int argc, char ** argv)
{
	int opt;
	char * endptr;
	if (argc == 1) {
		usage();
		return false;
	}
	while(true) 
	{
		int option_index = 0;// getopt_long stores the option index here 
		static struct option long_options[] = {
				{"help", no_argument, 0, 'h'},
	  			{"port", required_argument, 0, 'p'},
			  	{"ipv4", required_argument, 0, '4'},
	  			{"ipv6", required_argument, 0, '6'},
	  			{"client", required_argument, 0, 'c'},
	  			{"interactive", no_argument, 0, 'i'},
	  			{"file", required_argument, 0, 'f'},
	  			{0, 0, 0, 0}
		};
		opt = getopt_long (argc, argv, "hp:4:6:c:if:", long_options, &option_index);
		if (opt == -1) break;
		switch (opt)
		{
			case '?':
			case 'h':
				usage();
				return false;
			case 'p':
				server_port = (unsigned int)strtol(optarg, &endptr, 10);
				if (server_port == 0) {
					std::cerr << "Error: " << "Unable to convert --port " << optarg << "option value to unsigned integer type" << std::endl;
					return false;
				}
				std::clog << "-p " << server_port << std::endl;
				break;
			case '4':
				use_ipv6 = false;
				ip_address = optarg;
				std::clog << "-4 " << ip_address << std::endl;
				break;
			case '6':
				use_ipv6 = true;
				ip_address = optarg;
				std::clog << "-6 " << ip_address << std::endl;
				break;
			case 'c':
				client_port = (unsigned int)strtol(optarg, &endptr, 10);
				if (client_port == 0) {
					std::cerr << "Error: " << "Unable to convert --client " << optarg << "option value to unsigned integer type" << std::endl;
					return false;
				}
				std::clog << "-c " << client_port << std::endl;
				break;
			case 'i':
				use_interactive_mode = true;
				std::clog << "-i " << std::endl;
				break;
			case 'f':
				use_interactive_mode = false;
				command_file_name = optarg;
				std::clog << "-f " << command_file_name << std::endl;
				break;
			default:
				return false;
		}
	}
	return true;
}

int main(int argc, char ** argv)
{
	//int return_code;
	fd_set read_descriptors;
	//struct timeval tv;
	//time_t receive_timeout;

	if (parse_arguments(argc, argv) == false) {
		exit(EXIT_FAILURE);
	}

	try {		

		std::fstream file("coap-client.log", std::ios::out|std::ios::app);
		LOG_CREATE(ALL, file);

		clientConnection udp(ipv4_address_default, 5683);

		while(!quit)
		{
			FD_ZERO (&read_descriptors);
			FD_SET (STDIN_FILENO, &read_descriptors); // watch stdin (fd 0) to see when it has input
			FD_SET (udp.get_descriptor(), &read_descriptors); // watch socket descriptor to receive data from a client
	
			//tv.tv_sec = receive_timeout;
	        //tv.tv_usec = 0;

	        //TODO
	        break;
		}
		file.close();
		LOG_DELETE;
	}
	catch(error &err)
	{
		std::cerr << "The exception " << err.get_code() << " was cought " << std::endl;
		exit(EXIT_FAILURE);	
	}
	catch(...)
	{
		std::cerr << "Unknown exception was cought" << std::endl;
		exit(EXIT_FAILURE);
	}
	
    exit(EXIT_SUCCESS);
}