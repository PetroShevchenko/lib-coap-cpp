#include <arpa/inet.h>
#include "endpoint.h"
#include "uriClientEndpoint.h"
#include "log.h"

LOG_USING_NAMESPACE
LOG_EXTERN_DECLARE

namespace coap {

enum {
	RECEIVE_MAX_ATTEMPTS = 4,
	RECEVE_MAX_TIMEOUT_SECONDS = 2
};

error uriClientEndpoint::_error;

void uriClientEndpoint::start()
{
	LOG(INFO,"Start receiving file");
	std::cout << "Start receiving file. Wait...\n";
	_next_state = SEND_REQUEST;
}
void uriClientEndpoint::stop()
{
	LOG(INFO,"Stop receiving file");
	_next_state = ERROR;
}

void uriClientEndpoint::transaction_step()
{
	switch(_next_state)
	{
		case IDLE:
			idle_handler();
			break;

		case SEND_REQUEST:
			send_request_handler();
			break;

		case RECEIVE_DATA_BLOCK:
			receive_data_block_handler();
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

void uriClientEndpoint::send_request_handler()
{
	LOG(DEBUGGING,"send_request_handler");
	_state = SEND_REQUEST;

	// create GET request
	if (!_block2->set_header ((std::uint16_t)_connP->get_port(), _destUri, *_pduP)) {
		LOG(logging::ERROR, "Unable to set BLOCK2 header");
		_error.set_code(CREATE_BLOCK_OPTION);
		_next_state = ERROR;
		return;		
	}
	LOG(DEBUGGING, "mid = ", _mid);
	if (_mid == 0) {
		// generate a new message id
		_mid = _pduP->generate_message_id();
	}
	_pduP->make_request(CONFIRMABLE, GET, _mid, 0, nullptr, 0);
	//determine buffer size
	std::size_t length = 0;
	if (!_pduP->serialize (nullptr, &length, true)) {
		LOG(logging::ERROR, "Unable to determine buffer size");
		_error.set_code(COAP_SERIALIZE);
		_next_state = ERROR;
		return;
	}
	// serialize the buffer to send packet
	if (!_pduP->serialize (_connP->get_buffer(), &length, false)) {
		LOG(logging::ERROR, "Unable to serialize the coap packet");
		_error.set_code(COAP_SERIALIZE);
		_next_state = ERROR;
		return;
	}
	_connP->set_length(length);
	// send packet
	if (!_connP->send()) {
		LOG(logging::ERROR, "Unable to send the coap packet");
		_error.set_code(COAP_SEND);
		_next_state = ERROR;
		return;
	}
	// wait some seconds to receive the response from the server
	set_tv_sec(RECEVE_MAX_TIMEOUT_SECONDS);
	_next_state = RECEIVE_DATA_BLOCK;
}

void uriClientEndpoint::receive_data_block_handler()
{
	LOG(DEBUGGING,"receive_data_block_handler");
	_state = RECEIVE_DATA_BLOCK;

	auto retry = [&]()
	{
		LOG(DEBUGGING,"attempts = ", _attempts);
		if (++_attempts > RECEIVE_MAX_ATTEMPTS) {
			_attempts = 0;
			_error.set_code(COAP_TIMEOUT);
			_next_state = ERROR;
		}
		else {
			LOG(DEBUGGING, "Trying to send the packet again...");
			_next_state = SEND_REQUEST;
		}		
	};

	if (_received) {
		_received = false;
		if (!_connP->receive()) {
			LOG(logging::ERROR, "unable to receive the coap packet");
			_error.set_code(COAP_RECEIVE);
			_next_state = ERROR;
			return;
		}
		LOG(DEBUGGING,"COAP packet has been received");
		set_tv_sec(0);
		if (!_pduP->parse(_connP->get_buffer(), _connP->get_length())) {
			LOG(logging::ERROR, "Unable to parse received buffer");
			_error.set_code(RECEIVED_PACKET);
			_next_state = ERROR;
			return;
		}
		if (_pduP->get_message_type() != ACKNOWLEDGEMENT
			|| _mid != _pduP->get_message_messageId()) {
			LOG(logging::ERROR, "There was received an unknown packet");
			retry();
			return;
		}
		message_code_t mc = (message_code_t)_pduP->get_message_code();
		if (mc == NOT_FOUND) {
			LOG(logging::ERROR, "The target URI is not found");
			_error.set_code(URI_NOT_FOUND);
			_next_state = ERROR;
			return;			
		}
		else if (mc != CONTENT) {
			LOG(logging::ERROR, "Response code ", mc, " was received");
			_error.set_code(SERVER_CODE);
			_next_state = ERROR;
			return;
		}
		LOG(DEBUGGING, "Get header of BLOCK2");
		if (!_block2->get_header(*_pduP)) {
			LOG(logging::ERROR, "Unable to decode BLOCK2 option");
			_error.set_code(DECODE_BLOCK_OPTION);
			_next_state = ERROR;
			return;			
		}
		LOG(DEBUGGING, "Store a data chunk");
		// store a data chunk
		(*_downloaded_file).write((const char *)_pduP->get_message_payload().data(), _pduP->get_message_payload().size());

		_attempts = 0;
		LOG(DEBUGGING, "Analizing of more bit");
		if (_block2->get_more() == true) {
			_block2->set_more(false);
			_block2->set_number(_block2->get_number() + 1);
			_mid = _pduP->get_message_messageId() + 1;
			LOG(DEBUGGING, "mid was set : value: ",_mid);
			_next_state = SEND_REQUEST;
			set_tv_sec(0);
		}
		else {
			_next_state = COMPLETE;
		}
	}
	else {
		retry();
	}
}

void uriClientEndpoint::error_handler()
{
	LOG(DEBUGGING,"error_handler");
	_state = ERROR;
	LOG(logging::ERROR, _error);
	std::cerr << "An error occurred while receiving the file\n";
	_next_state = IDLE;
}

void uriClientEndpoint::complete_handler()
{
	LOG(DEBUGGING,"complete_handler");
	_state = COMPLETE;
	LOG(INFO,"Block-wise transfer completed");
	std::cout << "Block-wise transfer completed\n";
	_next_state = IDLE;
}

}//coap