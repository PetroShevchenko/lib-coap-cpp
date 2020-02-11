#ifndef URI_CLIENT_ENDPOINT_H
#define URI_CLIENT_ENDPOINT_H
#include "endpoint.h"
#include "uri.h"

namespace coap {

class uriClientEndpoint: public endpoint {

private:
	uri _destUri; // destination URI

public:
	bool request(method_code_t method, std::string params);
	void registration_step();
	void transaction_step();

private:
	LOG_CREATE(ALL,std::clog);

public:
	uriClientEndpoint(std::string path) : 
	endpoint(path), _destUri(path)
	{}
	~uriClientEndpoint()
	{
		LOG_DELETE;
	}
	uriClientEndpoint(const uriClientEndpoint &) = delete;
	uriClientEndpoint & operator=(const uriClientEndpoint &) = delete;
};

}//coap
#endif
