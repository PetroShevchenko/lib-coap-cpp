#ifndef URI_ENDPOINT_H
#define URI_ENDPOINT_H
#include "log.h"
#include "uri.h"
#include "endpoint.h"
#include "packet.h"
#include <functional>
#include <string>

namespace coap {


class uriEndpoint : private endpoint {

private:
	LOG_CREATE(ALL,std::clog);

	uri _uri;

public:

	uriEndpoint (std::string path, endpoint::callback_t callbacks[METHODS_COUNT], std::string attributes[METHODS_COUNT]):
	endpoint(path, callbacks, attributes), _uri(path)
	{
		LOG(DEBUG,"Constructor");
	}

	~uriEndpoint()
	{
		LOG_DELETE;
	}
	uriEndpoint(const uriEndpoint &) = delete;
	uriEndpoint & operator=(const uriEndpoint &) = delete;

	uri & get_uri()
	{
		return _uri;
	}
};

}//coap
#endif