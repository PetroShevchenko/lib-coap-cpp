#include "lwm2mData.h"

lwm2mData::lwm2mData(std::string &value): _type(lwm2mData::STRING), _id(0)
{
	_value.asBuffer.length = value.size() + sizeof(char);
	_value.asBuffer.buffer = new std::uint8_t [_value.asBuffer.length];
	std::memcpy(_value.asBuffer.buffer, value.c_str(), value.size());
}

lwm2mData::lwm2mData(std::vector<std::uint8_t> &value): _type(lwm2mData::OPAQUE), _id(0)
{
	_value.asBuffer.length = value.length();
	_value.asBuffer.buffer = new std::uint8_t [_value.asBuffer.length];
	std::memcpy(_value.asBuffer.buffer, value.data(), value.length());	
}

lwm2mData::lwm2mData(std::vector<lwm2mData> &value): _type(DATA_VECTOR), _id(0)
{
	_value.asChildren.count = value.length();
	size_t i = 0;
	for(auto data:value)
	{
		_value.asChildren.array[i] = new lwm2mData();
		*_value.asChildren.array[i] = data;
		i++;
	}
}

lwm2mData::lwm2mData(lwm2mData::object_link_t &value): _type(OBJECT_LINK), _id(0)
{
	_value.asObjectLink.objectId = value.objectId;
	_value.asObjectLink.objectInstanceId = value.objectInstanceId;
}

lwm2mData::~lwm2mData()
{
	if (_type == STRING	|| _type == OPAQUE) {
		delete _value.asBuffer.buffer;
	}
	else if (_type == DATA_VECTOR) {
		for(size_t i = 0; i < _value.asChildren.count; i++)
		{
			delete _value.asChildren.array[i];
		}
	}
}
