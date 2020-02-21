#ifndef CONNECTION_H
#define CONNECTION_H
#include <cstdint>
#include <string>
#include <cstddef>
#include <iostream>
#include <vector>

namespace coap{

enum {
	BUFFER_MAX_SIZE = 1024
};

class connection {

public:
	using state_t =
	enum state_e {
		DISCONNECTED,
		CONNECTED,
		NEEED_TO_SEND,
		NEED_TO_RECEIVE,
		SENT,
		RECEIVED,
		TIMEOUT_EXPIRED,
		ERROR_HAS_OCCURED
	};


protected:
	std::string _hostname;
    std::string _IPv4Address;
    std::string _IPv6Address;
    int _port;
    int _descriptor;
    std::uint8_t * _buffer;
    size_t _length;
    state_t _state;
	static bool checkNumberSystem(std::size_t start_index, std::size_t end_index,
									std::string &number_string, const std::string &pattern);

public:
	connection(std::string hostname, int port):
	_hostname(hostname), _port(port), _descriptor(0), _state(connection::DISCONNECTED)
	{
		_length = BUFFER_MAX_SIZE;
		_buffer = new uint8_t [_length];
	}
	virtual ~connection()
	{
		delete [] _buffer;
	}

	virtual bool establish() {return false;}
	virtual bool disconnect() {return false;}
	virtual bool send() {return false;}
	virtual bool receive() {return false;}
    virtual bool hostname2IPAddress();

	bool isIPv4Address(std::string address);
    bool isIPv6Address(std::string address);

	std::string get_hostname() const
	{
		return _hostname;
	}
	int get_port() const
	{
		return _port;
	}
	int get_descriptor() const
	{
		return _descriptor;
	}
	std::uint8_t * get_buffer()
	{
		return _buffer;
	}
	size_t get_length() const
	{
		return _length;
	}
	state_t get_state() const
	{
		return _state;
	}
	std::string get_IPv4Address() const
	{
		return _IPv4Address;
	}
	std::string get_IPv6Address() const
	{
		return _IPv6Address;
	}
};

}//coap
#endif