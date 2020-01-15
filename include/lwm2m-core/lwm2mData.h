#ifndef LWM2M_DATA_H
#define LWM2M_DATA_H
#include <vector>

namespace lwm2m {

class lwm2mData {

public:

using type_t =
enum type_e {
    UNDEFINED = 0,
    STRING,
    OPAQUE,
    INTEGER,
    UNSIGNED_INTEGER,
    FLOAT,
    BOOLEAN,
    DATA_VECTOR,
    OBJECT_LINK,
};

using buffer_t =
struct buffer_s {
	size_t length;
	std::uint8_t * buffer;
};

using children_t =
struct children_s {
	size_t count;
	lwm2mData ** array;
};

using object_link_t =
struct object_link_s {
	std::uint16_t objectId;
	std::uint16_t objectInstanceId; 
};

private:
	type_t _type;
	std::uint16_t _id;
	union {
		std::bool asBoolean;
		std::int64_t asInteger;
		std::uint64_t asUnsigned;
		doudle asFloat;
		buffer_t asBuffer;
		children_t asChildren;
		object_link_t asObjectLink;
	} _value;

public:
	lwm2mData():_type(UNDEFINED), _id(0), _value(0){};
	lwm2mData(std::bool &value): _type(BOOLEAN), _id(0), _value(std::move(value)){}
	lwm2mData(std::int64_t &value): _type(INTEGER), _id(0), _value(std::move(value)){}
	lwm2mData(std::uint64_t &value): _type(UNSIGNED_INTEGER), _id(0), _value(std::move(value)){}
	lwm2mData(double &value): _type(FLOAT), _id(0), _value(std::move(value)){}
	lwm2mData(std::string &value);
	lwm2mData(std::vector<std::uint8_t> &value);
	lwm2mData(std::vector<lwm2mData> &value);
	lwm2mData(object_link_t &value);
	~lwm2mData(); 
};


}//lwm2m
#endif