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
	//*_error = error::createInstance();
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

}//coap
