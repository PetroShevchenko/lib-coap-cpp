#ifndef CONNECTION_H
#define CONNECTION_H

namespace coap{

class connection {

protected:
    int _socket;
    int _port;
    std::string _address;
    std::uint8_t * _buffer;
    size_t _length;

public:
	virtual void establish();
	virtual void disconnect();
	virtual void send();
	virtual void receive();
};


}//coap
#endif