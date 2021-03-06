#ifndef URI_SERVER_ENDPOINT_H
#define URI_SERVER_ENDPOINT_H
#include "log.h"
#include "uri.h"
#include "endpoint.h"
#include "packet.h"
#include <functional>
#include <string>

LOG_USING_NAMESPACE
LOG_EXTERN_DECLARE

namespace coap {


class uriServerEndpoint : public endpoint {

public:

	using callback_t = std::function<void(void)>;

	using handler_t =
	struct handler_s {
		method_code_t _code;
		callback_t _callback;
		std::string _attribute;
		handler_s(method_code_t code, callback_t callback, std::string attribute ):
		_code(code), _callback(callback), _attribute(attribute){}
		handler_s(): _code(METHOD_MIN), _callback(nullptr), _attribute(""){}
		~handler_s(){}
	};

	void registration_step();
	void transaction_step();

private:

	uri _uri;
	handler_t _handler[METHODS_COUNT];

public:

	uriServerEndpoint (std::string path, callback_t callbacks[METHODS_COUNT], std::string attributes[METHODS_COUNT]):
	endpoint (path), _uri(path), _handler()
	{
		LOG(DEBUGGING,"Constructor");
		for(size_t i = 0; i < METHODS_COUNT; i++)
		{
			_handler[i]._code = static_cast<method_code_t>(i);
			_handler[i]._callback = std::move(callbacks[i]);
			_handler[i]._attribute = std::move(attributes[i]);
		}
	}

	~uriServerEndpoint()
	{
	}
	uriServerEndpoint(const uriServerEndpoint &) = delete;
	uriServerEndpoint & operator=(const uriServerEndpoint &) = delete;

	uri & get_uri()
	{
		return _uri;
	}
};

}//coap
#endif