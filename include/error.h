#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <string>
#include <vector>

namespace coap {

using error_code_t =
enum
{
	OK = 0,
	WRONG_ARGUMENT,
	PROTOCOL_VERSION,
	TOKEN_LENGTH,
	WRONG_OPTION_DELTA,
	WRONG_OPTION_LENGTH,
	WRONG_URI_PATH,
	BUFFER_LENGTH,
	CREATE_SOCKET,
	INCOMPLETE_SEND,
	RECEIVE,
	RESOLVE_ADDRESS,
	PORT_NUMBER,
	CREATE_BLOCK_OPTION,
	COAP_SERIALIZE,
	COAP_SEND,
	COAP_TIMEOUT,
	COAP_RECEIVE,
	RECEIVED_PACKET,
	URI_NOT_FOUND,
	SERVER_CODE,
	DECODE_BLOCK_OPTION,
	UNKNOWN
};
#define ERROR_CODE_MIN OK
#define ERROR_CODE_MAX UNKNOWN
#define IS_ERROR_CODE(code) ((code) <= ERROR_CODE_MAX && (code) >= ERROR_CODE_MIN)

class error {

private:
	int _code;

protected:
	const std::vector <std::string> _message = {
		"Success",
		"Wrong a function argument",
		"That CoAP version is not supported",
		"Wrong the token length",
		"Wrong the option parameter delta",
		"Wrong the option parameter length",
		"Wrong URI path use something like this /0/1 or /first/second",
		"Wrong buffer length",
		"Can not create socket",
		"The buffer was incompletely sent",
		"Receive error",
		"Unable to resolve IP address",
		"Wrong port number",
		"Unable to create block-wise option",
		"Unable to serialize the COAP packet",
		"Unable to send the COAP packet",
		"The COAP server does not response",
		"Unable to receive the COAP packet",
		"Unable to deserialize the received packet",
		"URI is not found",
		"The COAP server sent error code",
		"Unable to decode block-wise option",
		"Unknown error"
	};

public:
	error(const error &) = delete; 				/// copy constructor is not permitted
	error & operator=(const error &) = delete; 	/// overloaded copy operator is not permitted

	error():_code(UNKNOWN){}
	~error(){}

	friend std::ostream & operator<<(std::ostream &os, const error & er)
	{
		os << "code: " << er.get_code() << " message : " << er.get_message();
		return os;
	}

	void set_code(const int code)
	{
		_code = code;
	}

	int get_code() const
	{
		return _code;
	}
	const std::string & get_message() const
	{
		if (!IS_ERROR_CODE(_code)) return _message[UNKNOWN];
		return _message[_code];
	}
};

}//coap
#endif
