#include "packet.h"
#include "log.h"

LOG_USING_NAMESPACE

namespace coap{

packet * packet::_instanceP = 0;
packetDestroyer packet::_destroyer;

packet & packet::createInstance()
{
	LOG_INIT(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);

	LOG(DEBUG,"Entering");
	if (!_instanceP) {
		LOG(DEBUG,"Create a new packet");
		_instanceP = new packet();
		LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
		LOG(DEBUG,"A new packet was created, instanceP = ",_instanceP);
		_destroyer.initialize(_instanceP);
		LOG(DEBUG,"Exit");
	}
	LOG(DEBUG,"Exit");
	return *_instanceP;
}

bool packet::clearInstance(packet & instance)
{
	LOG_INIT(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);

	LOG(DEBUG,"Entering");

	if (&instance != _instanceP) {
		LOG(DEBUG,"Instances are not compared");
		return false;
	}
	//TODO
	//clear all fields of instance
	_instanceP->_message.headerInfo.asByte = 0;
	_instanceP->_message.code.asByte = 0;
	_instanceP->_message.messageId = 0;
	LOG(DEBUG,"Clearing a token");
	_instanceP->_message.token.clear();
	LOG(DEBUG,"Clearing a payload");
	_instanceP->_message.payload.clear();
	//LOG(DEBUG,"token = ", instanceP->message.token);
	//LOG(DEBUG,"payload = ", instanceP->message.payload);
	LOG(DEBUG,"Exit");
	return true;
}

packet::packet()
{
	LOG_INIT(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);
	LOG(DEBUG,"Entering");

	_message.headerInfo.asByte = 0;
	_message.code.asByte = 0;
	_message.messageId = 0;
	_error = new error;
}

packet::~packet()
{
	delete _error;
}

packetDestroyer::~packetDestroyer()
{
	delete packetDestroyer::_instanceP;
}
void packetDestroyer::initialize(packet * p)
{
	packetDestroyer::_instanceP = p;
}

std::ostream & operator<<(std::ostream & os,const packet::option_t &option)
{
	static unsigned short optionsCounter = 0;
	os << "option #" << ++optionsCounter <<
		" delta: " << static_cast<unsigned short>(option.header.asBitfield.delta) <<
		" length: " << static_cast<unsigned short>(option.header.asBitfield.length) <<
		" value: " << option.value
		;
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet::header_t &header)
{
	os << 	" version: " << static_cast<unsigned short>(header.asBitfield.version) <<
		" type: " << static_cast<unsigned short>(header.asBitfield.type) <<
		" tokenLength: " << static_cast<unsigned short>(header.asBitfield.tokenLength)
		;
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet::code_t &code)
{
	os << 	" codeClass: " << static_cast<unsigned short>(code.asBitfield.codeClass) <<
		" codeDetail: " << static_cast<unsigned short>(code.asBitfield.codeDetail)
		;
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet::message_t &message)
{
	os << message.headerInfo << message.code <<
		" messageId: " << message.messageId <<
		" token: " << message.token <<
		" options: " << message.options <<
		" payload: " << message.payload
		;
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet &object)
{
	//os << *_error << object._message;
	os << object._message;
	return os;
}

}//coap
