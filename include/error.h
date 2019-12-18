#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <string>

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
