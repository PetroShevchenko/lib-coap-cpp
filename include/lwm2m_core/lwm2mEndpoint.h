#ifndef LWM2M_ENDPOINT_H
#define LWM2M_ENDPOINT_H
#include "endpoint.h"
#include <vector>

namespace lwm2m {


class lwm2mEndpoint : private endpoint {

private:
	std::vector<lwm2mObject> objects;
	
//TODO

};



}// lwm2m

#endif