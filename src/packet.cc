#include "packet.h"
#include "log.h"

LOG_USING_NAMESPACE

namespace coap{

packet * packet::instanceP = 0;
packetDestroyer packet::destroyer;

packet & packet::createInstance()
{
	LOG_INIT(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);
	
	LOG(DEBUG,"Entering");
	if (!instanceP) {
		LOG(DEBUG,"Create a new packet");
		instanceP = new packet();
		LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
		LOG(DEBUG,"A new packet was created, instanceP = ",instanceP);
		destroyer.initialize(instanceP);
		LOG(DEBUG,"Exit");
	}
	LOG(DEBUG,"Exit");
	return *instanceP;
}

bool packet::clearInstance(packet & instance)
{
	LOG_INIT(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);
	
	LOG(DEBUG,"Entering");

	if (&instance != instanceP) {
		LOG(DEBUG,"Instances are not compared");
		return false;
	}
	//TODO
	//clear all fields of instance
	instanceP->message.headerInfo.asByte = 0;
	instanceP->message.code.asByte = 0;
	instanceP->message.messageId = 0;
	LOG(DEBUG,"Clearing a token");
	instanceP->message.token.clear();
	LOG(DEBUG,"Clearing a payload");
	instanceP->message.payload.clear();
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

	message.headerInfo.asByte = 0;
	message.code.asByte = 0;
	message.messageId = 0;
}

packetDestroyer::~packetDestroyer()
{
	delete instanceP;
}
void packetDestroyer::initialize(packet * p)
{
	instanceP = p;
}

}//coap
