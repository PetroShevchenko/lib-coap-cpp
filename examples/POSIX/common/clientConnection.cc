#include <netdb.h>
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
#include "coapcpp.h"
#include "clientConnection.h"

#ifndef MSG_CONFIRM
#define MSG_CONFIRM 0
#endif

LOG_USING_NAMESPACE
LOG_EXTERN_DECLARE

namespace coap {

error clientConnection::_error;

clientConnection::clientConnection(std::string hostname, int port)
:connection(hostname,port), _serverIPv6Address(new struct sockaddr_in6), _serverIPv4Address(new struct sockaddr_in), _use_IPv6(false)
{
}

clientConnection::~clientConnection()
{
	delete _serverIPv6Address;
	delete _serverIPv4Address;
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
	struct in_addr * inp;
	//LOG(DEBUGGING,"_IPv6Address.size(1) = ", _IPv6Address.size());
	//LOG(DEBUGGING,"_IPv4Address.size(1) = ", _IPv4Address.size());

	if (!hostname2IPAddress()) {
		LOG(ERROR,"Unable to resolve IP address");
		_error.set_code(RESOLVE_ADDRESS);
		return false;
	}
	//LOG(DEBUGGING,"_IPv6Address.size(2) = ", _IPv6Address.size());
	//LOG(DEBUGGING,"_IPv4Address.size(2) = ", _IPv4Address.size());

	if (_use_IPv6)
	{
	    _serverIPv6Address->sin6_family = AF_INET6;
	    _serverIPv6Address->sin6_port = htons(_port);
	    inp =  (in_addr *)(&_serverIPv6Address->sin6_addr.s6_addr);
	    inet_aton (_IPv6Address.c_str(), inp);
	}
	else
	{
	    _serverIPv4Address->sin_family = AF_INET;
	    _serverIPv4Address->sin_port = htons(_port);
	    inp =  (in_addr *)(&_serverIPv4Address->sin_addr.s_addr);
	    inet_aton (_IPv4Address.c_str(), inp);
	}

	_descriptor = socket ((_use_IPv6 ? AF_INET6: AF_INET), SOCK_DGRAM, 0);

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
	struct sockaddr * sap;
	size_t sz;
	//LOG(DEBUGGING,"_serverIPv6Address =", _serverIPv6Address);
	//LOG(DEBUGGING,"_serverIPv4Address =", _serverIPv4Address);
	if (_use_IPv6) {
		sap = (struct sockaddr *)_serverIPv6Address;
		sz = sizeof(*_serverIPv6Address);
	}
	else
	{
		sap = (struct sockaddr *)_serverIPv4Address;
		sz = sizeof(*_serverIPv4Address);
	}
	LOG(DEBUGGING,"sap =", sap);
	LOG(DEBUGGING,"sz =", sz);
	LOG(DEBUGGING,"_buffer =", _buffer);
	LOG(DEBUGGING,"_length =", _length);

	sent = sendto (_descriptor, _buffer, _length, MSG_CONFIRM, sap, sz);
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

bool clientConnection::hostname2IPAddress()
{
	struct addrinfo hints;
	struct addrinfo * servinfo = nullptr;
	char port_str[8];
	char address_str[40];
	int result;
	bool status = false;

	if (_hostname.size() == 0) return false;
	if (connection::isIPv6Address(_hostname)) {
		_IPv6Address = _hostname;
		_use_IPv6 = true;
		return true;
	}
	if (connection::isIPv4Address(_hostname)) {
		_IPv4Address = _hostname;
		_use_IPv6 = false;
		return true;
	}
	memset(address_str, 0, sizeof(address_str));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	snprintf((char *)port_str, sizeof(port_str), "%d", _port);

	result = getaddrinfo(_hostname.c_str(), port_str, &hints, &servinfo);

	if (result == 0) {
		for (struct addrinfo * p = servinfo; p != nullptr; p = p->ai_next)
		{
			inet_ntop(p->ai_family, p->ai_family == AF_INET ?
					(void *) & ((struct sockaddr_in *) p->ai_addr)->sin_addr :
					(void *) & ((struct sockaddr_in6 *) p->ai_addr)->sin6_addr, address_str, sizeof(address_str));
			if (p->ai_family == AF_INET6) {
				_IPv6Address = address_str;
				_use_IPv6 = true;
				status = true;
			}
			else if (p->ai_family == AF_INET) {
				_IPv4Address = address_str;
				_use_IPv6 = false;
				status = true;
			}
		}
	}
	else status = false;

	if (servinfo != nullptr) {
		freeaddrinfo(servinfo);
	}
	return status;
}

}//coap
