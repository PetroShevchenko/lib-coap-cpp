#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstring>
#include <cstdio> 
#include <cstdlib> 
#include <unistd.h> 
#include "connection.h"
#include "clientConnection.h"
#include "log.h"

namespace coap {

error clientConnection::_error;

clientConnection::clientConnection(std::string address, int port)
:connection(address,port)
{
	_serverAddress = new struct sockaddr_in;
    _serverAddress->sin_family = AF_INET;                                                                                                                                                                                                                                                        
    _serverAddress->sin_port = htons(_port);
    struct in_addr * inp =  (in_addr *)(&_serverAddress->sin_addr.s_addr);
    inet_aton (_address.c_str(), inp);
}

clientConnection::~clientConnection()
{
	delete _serverAddress;
	LOG_DELETE;	
}

bool clientConnection::fill_buffer(std::uint8_t * data, size_t size)
{
	if (data == nullptr
		|| size == 0
		|| size > BUFFER_MAX_SIZE) {
		LOG(ERROR, "Wrong argument : data =", data, " ,size = ", size);
		_error.set_code(WRONG_ARGUMENT);
		return false;
	}
	std::memcpy(_buffer, data, size);
	_length = size;
	LOG(INFO,"The buffer is filled");
	return true;
}

bool clientConnection::establish()
{
	_descriptor = socket (AF_INET, SOCK_DGRAM, 0);                                                                                                                                                                                                                                                                       

    if (_descriptor < 0) {
    	LOG(ERROR,"Can not create a new socket");                                                                                                                                                                                                                                                                                               
        _error.set_code(CREATE_SOCKET);                                                                                                                                                                                                                                                                    
        return false;                                                                                                                                                                                                                                                                                                
    }
    LOG(INFO,"A new socket was created");
    return true; 	
}

bool clientConnection::disconnect()
{
    close(_descriptor);
    LOG(INFO,"The socket was closed");
    return true;	
}

bool clientConnection::send()
{
	ssize_t sent;
	struct sockaddr * sap = (struct sockaddr *)_serverAddress;
	sent = sendto (_descriptor, _buffer, _length, MSG_CONFIRM, sap, sizeof(*_serverAddress));
	if (sent != (ssize_t)_length) {
		LOG(ERROR,"The buffer was incompletly sent, length = ", _length, " , sent = ", sent);
		_error.set_code(INCOMPLETE_SEND);
		return false;
	}
	LOG(INFO,"There was sent ", sent, " bytes");
	return true;
}

bool clientConnection::receive()
{	
	ssize_t received;
	socklen_t addressLength;
	struct sockaddr clientAddress;
 	
 	memset (_buffer, 0 , BUFFER_MAX_SIZE);
	received = recvfrom (_descriptor, (char *)(_buffer), BUFFER_MAX_SIZE,
							MSG_WAITALL, static_cast<struct sockaddr *>(&clientAddress), &addressLength);
	if (received == -1) {
		LOG(ERROR, "Receive error");
		_error.set_code(RECEIVE);
		return false;
	}
	_length = static_cast<size_t>(received);
	LOG(INFO,"There was received ",_length, " bytes");
	return true;
}

}//coap