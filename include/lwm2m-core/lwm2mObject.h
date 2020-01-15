#ifndef LWM2M_OBJECT_H
#define LWM2M_OBJECT_H
#include <cstdint>
#include <vector>
#include <functional>
#include "uri.h"
#include "lwm2mResource.h"

namespace lwm2m {

class lwm2mObject : public uri {
private:
	std::uint16_t _id;
	std::vector<lwm2mResource> _resources;

public:
	lwm2mObject(std::string &URI) : uri(URI);
	lwm2mObject(coap::uri::uri_t &URI) : uri(URI);
	~lwm2mObject();
	virtual bool write();
	virtual bool read();
	virtual bool execute();
	virtual bool create();
	virtual bool remove();
	virtual bool discover();
};


}//lwm2m

#endif