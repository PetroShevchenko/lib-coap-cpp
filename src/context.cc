#include "context.h"

namespace coap{

context * context::_instanceP = 0;
contextDestroyer context::_destroyer;
error context::_error;

context & context::createInstance()
{
	if (!_instanceP) {
		_instanceP = new context();
		_destroyer.initialize(_instanceP);
	}
	return *_instanceP;
}

bool context::clearInstance(context & instance)
{
	if (&instance != _instanceP) {
		return false;
	}
	//TODO
	return true;
}

context::context()
{
	LOG(DEBUG,"Entering");
	//TODO
	LOG(DEBUG,"Leaving");
}

context::~context()
{
	LOG_DELETE;
}

contextDestroyer::~contextDestroyer()
{
	delete contextDestroyer::_instanceP;
}

void contextDestroyer::initialize(context * p)
{
	contextDestroyer::_instanceP = p;
}

}//coap