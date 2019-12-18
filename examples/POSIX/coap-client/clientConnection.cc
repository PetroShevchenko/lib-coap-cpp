#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>

#include "clientConnection.h"
#include "log.h"

namespace coap {

enum {
	MAX_CLIENT_BUFFER_SIZE = 1024
};

error clientConnection::_error;

clientConnection::clientConnection(std::string address, int port):connection(address,port)
{
	_serverAddress = new struct sockaddr_in;
    _buffer = new uint8_t [MAX_CLIENT_BUFFER_SIZE];  
    _serverAddress->sin_family = AF_INET;                                                                                                                                                                                                                                                        
    _serverAddress->sin_port = htons(_port);
    inet_aton (_address.c_str(), (in_addr*)&serverAddress->sin_addr.s_addr);      	
}

clientConnection::~clientConnection()
{
	delete _serverAddress;
	delete [] _buffer;
	LOG_DELETE;	
}

bool clientConnection::fill_buffer(std::uint8_t * data, size_t size)
{
	if (data == nullptr
		|| size == 0
		|| size > MAX_CLIENT_BUFFER_SIZE) {
		LOG(ERROR, "Wrong argument : data =", data, " ,size = ", size);
		_error.set_code(WRONG_ARGUMENT);
		return false;
	}
	std::memcpy(_buffer, data, size);
	_length = size;
	LOG(INFO,"The buffer is filled")
	return true;
}

bool clientConnection::establish()
{
	_socket = socket (AF_INET, SOCK_DGRAM, 0);                                                                                                                                                                                                                                                                       

    if (_socket < 0) {
    	LOG(ERROR,"Can not create a new socket");                                                                                                                                                                                                                                                                                               
        _error.set_code(CREATE_SOCKET);                                                                                                                                                                                                                                                                    
        return false;                                                                                                                                                                                                                                                                                                
    }
    LOG(INFO,"A new socket was created");
    return true; 	
}

bool clientConnection::disconnect()
{
    close(_socket);
    LOG(INFO,"The socket was closed");
    return true;	
}

bool clientConnection::send()
{
	ssize_t sent;
	sent = sendto (_socket, _buffer, _length, MSG_CONFIRM, const_cast<struct sockaddr *>(_serverAddress), sizeof(*_serverAddress));
	if (sent != _length) {
		LOG(ERROR,"The buffer was incompletly sent, length = ", _length, " , sent = ", sent);
		_error.set_error(INCOMPLETE_SEND);
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
 	
 	memset (_buffer, 0 , MAX_CLIENT_BUFFER_SIZE);
	received = recvfrom (_socket, static_cast<char *>(_buffer), MAX_CLIENT_BUFFER_SIZE,
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