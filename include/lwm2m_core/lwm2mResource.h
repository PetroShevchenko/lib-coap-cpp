#ifndef LWM2M_RESOURCE_H
#define LWM2M_RESOURCE_H
#include <cstdint>
#include "lwm2mObject.h"
#include "lwm2mData.h"

namespace lwm2m {

class lwm2mResource : public lwm2mObject {
private:
	lwm2mData _data;
public:
	lwm2mResource():lwm2mObject();
	~lwm2mResource();
};

}//lwm2m

#endif