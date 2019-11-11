#ifndef ERROR_H
#define ERROR_H
#include <iostream>

namespace coap {

using error_code_t = 
enum
{
	OK = 0,
	WRONG_ARGUMENT,
	PROTOCOL_VERSION,
	TOKEN_LENGTH,
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
