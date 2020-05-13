#ifndef CONNECTION_H
#define CONNECTION_H
#include <cstdint>
#include <string>
#include <cstddef>
#include <iostream>
#include <vector>
#include <climits>
#include "error.h"

namespace coap{

enum {
	BUFFER_MAX_SIZE = 2500
};

class connection {

private:
	static error _error;

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
	static bool isIPv4Address(std::string address);
	static bool isIPv6Address(std::string address);

protected:
	std::string _hostname;
    std::string _IPv4Address;
    std::string _IPv6Address;
    int _port;
    int _descriptor;
    std::uint8_t * _buffer;
    size_t _length;
    state_t _state; //curent state
    state_t _next_state; // next state
	static bool checkNumberSystem(std::size_t start_index, std::size_t end_index,
									std::string &number_string, const std::string &pattern);

public:

	connection(std::string hostname, int port):
	_hostname(hostname), _port(port), _descriptor(0), _state(DISCONNECTED), _next_state(DISCONNECTED)
	{
		if (port < 0 || port > USHRT_MAX) {
			_error.set_code(PORT_NUMBER);
			throw &_error;
		}
		_length = BUFFER_MAX_SIZE;
		_buffer = new uint8_t [_length];
	}

	virtual ~connection()
	{
		delete [] _buffer;
	}

	virtual bool establish() = 0;
	virtual bool disconnect() = 0;
	virtual bool send() = 0;
	virtual bool receive() = 0;
	virtual bool hostname2IPAddress() = 0;

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
	state_t get_next_state() const
	{
		return _next_state;
	}
	void set_state(state_t state)
	{
		_state = state;
	}
	void set_next_state(state_t next_state)
	{
		_next_state = next_state;
	}
	void set_length(size_t length)
	{
		_length = length;;
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