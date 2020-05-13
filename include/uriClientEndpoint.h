#ifndef URI_CLIENT_ENDPOINT_H
#define URI_CLIENT_ENDPOINT_H
#include <ctime>
#include <fstream>
#include "endpoint.h"
#include "uri.h"
#include "connection.h"
#include "packet.h"
#include "blockwise.h"

namespace coap {

class uriClientEndpoint: public endpoint {

private:
	using state_t =
	enum {
		IDLE = 0,
		SEND_REQUEST,
		RECEIVE_DATA_BLOCK,
		ERROR,
		COMPLETE
	};
	void idle_handler();
	void send_request_handler();
	void receive_data_block_handler();
	void error_handler();
	void complete_handler();

private:
	uri _destUri; // destination URI
	state_t _state;// current state
	state_t _next_state; //next state
	connection *_connP; // only one connection
	packet * _pduP; // COAP packet
	unsigned long _attempts; // attempt counter
	time_t * _tv_secP; // timeout in seconds
	std::uint16_t _mid; // message ID
	bool _received; // receive packet flag
	std::fstream * _downloaded_file;//downloaded file
	block2 * _block2;//blockwise transfer

	static error _error;

public:
	//bool request(method_code_t method, std::string params);
	void registration_step(){}
	void transaction_step();
	bool isComplete()
	{
		return (_state == COMPLETE);
	}
	bool isError()
	{
		return (_state == ERROR);
	}

	void start();
	void stop();

	void set_received(bool state)
	{
		_received = state;
	}
	bool get_received() const
	{
		return _received;
	}
	void set_tv_sec(time_t seconds)
	{
		if (seconds > 60) seconds = 60;
		*_tv_secP = seconds;
	}

public:
	uriClientEndpoint(std::string path, connection * connectionP, packet * pduP, time_t * timeoutP) :
	endpoint(path), _destUri(path), _state(IDLE), _next_state(IDLE), _connP(connectionP), _pduP(pduP), 
	_attempts(0), _tv_secP(timeoutP), _mid(0), _received(false), _downloaded_file(nullptr), _block2(nullptr)
	{
		_downloaded_file = new std::fstream(_destUri.get_uri().asString[_destUri.get_uri().asString.size() - 1], std::ios::out|std::ios::app|std::ios::binary);
		_block2 = new block2(); 
	}
	~uriClientEndpoint()
	{
		_downloaded_file->close();
		delete _downloaded_file;
		delete _block2;
	}
	uriClientEndpoint(const uriClientEndpoint &) = delete;
	uriClientEndpoint & operator=(const uriClientEndpoint &) = delete;
};

}//coap
#endif
