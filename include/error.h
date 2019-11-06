#ifndef ERROR_H
#define ERROR_H

namespace coap {

using error_code_t = 
enum
{
	OK = 0,
	WRONG_ARGUMENT,
	UNKNOWN
};
#define ERROR_CODE_MIN OK
#define ERROR_CODE_MAX UNKNOWN
#define IS_ERROR_CODE(code) ((code) <= ERROR_CODE_MAX && (code) >= ERROR_CODE_MIN)

class error {

private:
	int _code;
/*	error()
	{
		_code = UNKNOWN;
	}
	~error(){}
*/
protected:
	const std::vector <std::string> _message = {
		"Success",
		"Wrong a function argument",
		"Unknown error"
	};

public:
	error(const error &) = delete; 				/// copy constructor is not permitted
	error & operator=(const error &) = delete; 	/// overloaded copy operator is not permitted 

	error():_code(UNKNOWN)
	{
		//_code = UNKNOWN;
	}
	~error(){}

/*	static error& createInstance()
	{
		static error instance;
		return instance;
	}
*/
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
		if IS_ERROR_CODE(_code) return _message[UNKNOWN];
		return _message[_code];
	}
};

}//coap
#endif
