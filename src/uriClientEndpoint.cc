#include "endpoint.h"
#include "uriClientEndpoint.h"

namespace coap {

bool uriClientEndpoint::request(method_code_t method, std::string params)
{
	return false;
}

void uriClientEndpoint::registration_step()
{

}

void uriClientEndpoint::transaction_step()
{

}
}//coap