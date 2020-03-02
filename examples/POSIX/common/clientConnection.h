#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H
#include "coapcpp.h"

namespace coap {

class clientConnection : public connection {

private:
	LOG_CREATE(ALL,std::clog);

protected:
	struct sockaddr_in6 * _serverIPv6Address;
	struct sockaddr_in * _serverIPv4Address;

	static error _error;

public:
	clientConnection(std::string hostname, int port);
	~clientConnection();
	bool establish();
	bool disconnect();
	bool send();
	bool receive();
	bool fill_buffer(std::uint8_t *, size_t);
	bool hostname2IPAddress();
};

}//coap
#endif