#include "packet.h"

namespace coap{

packet * packet::instanceP = 0;
packetDestroyer packet::destroyer;

packet & packet::createInstance()
{
	if (!instanceP) {
		instanceP = new packet();
		destroyer.initialize(instanceP);
	}
	return *instanceP;
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