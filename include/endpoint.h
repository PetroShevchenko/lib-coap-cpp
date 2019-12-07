#ifndef ENDPOINT_H
#define ENDPOINT_H
#include "log.h"
#include "uri.h"
#include "packet.h"
#include <functional>
#include <string>

namespace coap {

class endpoint {

public:
	using callback_t = std::function<void(void)>;

protected:
	using handler_t =
	struct handler_s {
		method_code_t code;
		callback_t callback;
		std::string attribute;
	};

	LOG_CREATE(ALL,std::clog);

	std::string _name;
	handler_t _handler[METHODS_COUNT];

public:
	endpoint (std::string name, endpoint::callback_t callbacks[METHODS_COUNT], std::string attributes[METHODS_COUNT] ):  _name(name)
	{
		for(size_t i = 0; i < METHODS_COUNT; i++)
		{
			_handler[i].code = static_cast<method_code_t>(i);
			_handler[i].callback = std::move(callbacks[i]);
			_handler[i].attribute = std::move(attributes[i]);
		}
	}

	virtual ~endpoint()
	{
		LOG_DELETE;
	}
	endpoint(const endpoint &) = delete;
	endpoint & operator=(const endpoint &) = delete;
};

}//coap
#endif