#ifndef URI_CLIENT_ENDPOINT_H
#define URI_CLIENT_ENDPOINT_H
#include <ctime>
#include "endpoint.h"
#include "uri.h"
#include "connection.h"
#include "packet.h"

namespace coap {

class uriClientEndpoint: public endpoint {

private:
	using state_t =
	enum {
		IDLE = 0,
		SEND_FIRST_REQUEST,
		RECEIVE_DATA_BLOCK,
		SEND_NEXT_REQUEST,
		ERROR,
		COMPLETE
	};
	void idle_handler();
	void send_first_request_handler();
	void receive_data_block_handler();
	void send_next_request_handler();
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

	void start()
	{
		_next_state = SEND_FIRST_REQUEST;
	}
	void stop()
	{
		_next_state = ERROR;
	}
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
	endpoint(path), _destUri(path), _state(IDLE), _next_state(IDLE), _connP(connectionP), _pduP(pduP), _attempts(0), _tv_secP(timeoutP), _mid(0), _received(false)
	{
	}
	~uriClientEndpoint()
	{
	}
	uriClientEndpoint(const uriClientEndpoint &) = delete;
	uriClientEndpoint & operator=(const uriClientEndpoint &) = delete;
};

}//coap
#endif
