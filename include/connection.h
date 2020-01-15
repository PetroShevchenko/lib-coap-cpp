#ifndef CONNECTION_H
#define CONNECTION_H
#include <cstdint>

namespace coap{

class connection {

protected:
    int _socket;
    int _port;
    std::string _address;
    std::uint8_t * _buffer;
    size_t _length;

public:
	connection(std::string address, int port): _address(address), _port(port){};		
	virtual ~connection(){};
	virtual bool establish();
	virtual bool disconnect();
	virtual bool send();
	virtual bool receive();
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