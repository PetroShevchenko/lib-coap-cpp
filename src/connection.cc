#include "connection.h"

connection::connection(std::string address, int port): _address(address), _port(port)
{
	_length = BUFFER_MAX_SIZE;
	_buffer = new uint8_t [_length];
}	

virtual connection::~connection()
{
	delete [] _buffer;
}



