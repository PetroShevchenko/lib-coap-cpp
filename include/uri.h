#ifndef URI_H
#define URI_H
#include <cstdint>
#include <string>
#include <vector>
//#include "log.h"
#include "error.h"

namespace coap {

class uri {

public:
	enum {
		URI_MAX_SEGMENTS = 4 // for example: 4 segments equals '/first/second/third/forth' or '/0/1/2/3'
	};

	using uri_type_t =
	enum uri_type_e {
		URI_TYPE_INTEGER = 0,
		URI_TYPE_STRING
	};

	using uri_t =
	struct uri_s {
		uri_type_t type;
		std::vector <std::string> asString;
		std::vector <long int> asInteger;
	};

protected:
	std::string _path;
	uri_t _uri;
	static error _error;

	bool path_to_uri();
	void uri_to_path();

public:

	friend std::ostream & operator<<(std::ostream & os,const uri_t &uri);
	friend std::ostream & operator<<(std::ostream & os,const uri &object);

	uri(std::string);
	uri(uri_t);

	virtual ~uri()
	{
		//LOG_DELETE;
	}
	std::string get_path() const {
		return _path;
	}
	uri_t & get_uri() {
		return _uri;
	}

};

}//coap

#endif