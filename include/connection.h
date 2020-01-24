#ifndef CONNECTION_H
#define CONNECTION_H
#include <cstdint>
#include <string>
#include <cstddef>

namespace coap{

enum {
	BUFFER_MAX_SIZE = 4096
};	

class connection {

protected:
    std::string _address;
    int _port;
    int _descriptor;
    std::uint8_t * _buffer;
    size_t _length;

public:
	connection(std::string address, int port);
	virtual ~connection();
	virtual bool establish();
	virtual bool disconnect();
	virtual bool send();
	virtual bool receive();
	std::string get_address() const
	{
		return _address;
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
};

}//coap
#endif