#include <arpa/inet.h>
#include "endpoint.h"
#include "uriClientEndpoint.h"
#include "log.h"

LOG_USING_NAMESPACE
LOG_EXTERN_DECLARE

namespace coap {
#if 0
bool uriClientEndpoint::request(method_code_t method, std::string params)
{
	return false;
}
#endif
void uriClientEndpoint::transaction_step()
{
	switch(_next_state)
	{
		case IDLE:
			idle_handler();
			break;

		case SEND_FIRST_REQUEST:
			send_first_request_handler();
			break;

		case RECEIVE_DATA_BLOCK:
			receive_data_block_handler();
			break;

		case SEND_NEXT_REQUEST:
			send_next_request_handler();
			break;

		case ERROR:
			error_handler();
			break;

		case COMPLETE:
			complete_handler();
			break;

		default:
			_next_state = ERROR;
			break;
	}
}

void uriClientEndpoint::idle_handler()
{
	LOG(DEBUGGING,"idle_handler");
	_state = _next_state = IDLE;
}

void uriClientEndpoint::send_first_request_handler()
{
	LOG(DEBUGGING,"send_first_request_handler");
	_state = SEND_FIRST_REQUEST;

	// create GET request
	_pduP->clean_options();
	// add option URI_PORT
	std::uint16_t port = (std::uint16_t)_connP->get_port();
	port = htons(port);
	_pduP->add_option(URI_PORT, (const std::uint8_t *)&port, sizeof(std::uint16_t));
	// add options URI_PATH
	for (auto opt : _destUri.get_uri().asString)
	{
		_pduP->add_option(URI_PATH, (const std::uint8_t *)opt.c_str(), opt.length());
	}
	// generate a new message id
	_mid = _pduP->generate_message_id();
	_pduP->make_request(CONFIRMABLE, GET, _mid, 0, nullptr, 0);
	//determine buffer size
	std::size_t length = 0;
	if (!_pduP->serialize (nullptr, &length, true)) {
		LOG(logging::ERROR, "Unable to determine buffer size");
		_next_state = ERROR;
		return;
	}
	// serialize the buffer to send packet
	if (!_pduP->serialize (_connP->get_buffer(), &length, false)) {
		LOG(logging::ERROR, "Unable to serialize the coap packet");
		_next_state = ERROR;
		return;
	}
	_connP->set_length(length);
	// send packet
	if (!_connP->send()) {
		LOG(logging::ERROR, "Unable to send the coap packet");
		_next_state = ERROR;
		return;
	}
	// wait 5 seconds to receive the response from the server
	set_tv_sec(5);
	_next_state = RECEIVE_DATA_BLOCK;
}

void uriClientEndpoint::receive_data_block_handler()
{
	LOG(DEBUGGING,"receive_data_block_handler");
	_state = RECEIVE_DATA_BLOCK;

	if (_received) {
		_received = false;
		if (!_connP->receive()) {
			LOG(logging::ERROR, "unable to receive the coap packet");
			_next_state = ERROR;
			return;
		}
		LOG(DEBUGGING,"COAP packet has been received");
		set_tv_sec(0);
		_next_state = COMPLETE;
	}
	else {
		if (++_attempts > 20) {
			_attempts = 0;
			_next_state = COMPLETE;
		}
		else {
			LOG(DEBUGGING, "Trying to send the packet again...");
			_next_state = SEND_FIRST_REQUEST;
		}
	}
}

void uriClientEndpoint::send_next_request_handler()
{
	LOG(DEBUGGING,"send_next_request_handler");
	_state = SEND_NEXT_REQUEST;
	_next_state = RECEIVE_DATA_BLOCK;
}

void uriClientEndpoint::error_handler()
{
	LOG(DEBUGGING,"error_handler");
	_state = ERROR;
	_next_state = IDLE;
}

void uriClientEndpoint::complete_handler()
{
	LOG(DEBUGGING,"complete_handler");
	_state = COMPLETE;
	_next_state = IDLE;
}

}//coap