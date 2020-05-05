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

	// create get request
	_pduP->clean_options();

	for (auto opt : _destUri.get_uri().asString)
	{
		_pduP->add_option(URI_PATH, (const std::uint8_t *)opt.c_str(), opt.length());
	}

	LOG(DEBUGGING,"Path options :", _pduP->get_message_options());

	//TODO
	_next_state = RECEIVE_DATA_BLOCK;
}

void uriClientEndpoint::receive_data_block_handler()
{
	LOG(DEBUGGING,"receive_data_block_handler");
	_state = RECEIVE_DATA_BLOCK;
	if (++_attempts > 20) {
		_attempts = 0;
		_next_state = COMPLETE;
	}
	else {
		_next_state = SEND_NEXT_REQUEST;
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