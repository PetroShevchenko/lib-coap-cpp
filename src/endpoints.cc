#include "endpoints.h"
#include <cstring>

namespace coap {

endpoints * endpoints::_instanceP = nullptr;
endpointsDestroyer endpoints::_destroyer;

endpoints & endpoints::createInstance()
{
	if (!_instanceP) {
		_instanceP = new endpoints();
		_destroyer.initialize(_instanceP);
	}
	return *_instanceP;
}

endpointsDestroyer::~endpointsDestroyer()
{
	delete endpointsDestroyer::_instanceP;
}

void endpointsDestroyer::initialize(endpoints * p)
{
	endpointsDestroyer::_instanceP = p;
}

endpoints::endpoints()
{
	LOG_SET_LEVEL(ALL);
	//_context;
	//_error;
}
endpoints::~endpoints()
{
	LOG_DELETE;
}

endpoints::key_t endpoints::create_key(const uri_t * uriP)
{
	key_t key = nullptr;
	size_t length = 0;

	if (uriP->type == URI_TYPE_STRING)
	{
		for (size_t i = 0; i < uriP->count; i++)
		{
			length += strlen("/") + strlen(uriP->path.asString[i]);
		}

		key = new char [length];

		for (size_t i = 0; i < uriP->count; i++)
		{
			std::sprintf((char *)key, "/%s", (char *)uriP->path.asString[i]);
		}
	}
	else if (uriP->type == URI_TYPE_INTEGER)
	{
		length = uriP->count * (1 + 10); //  max size equals (strlen("/4294967296") * count)

		key = new char [length];

		for (size_t i = 0; i < uriP->count; i++)
		{
			std::sprintf((char *)key, "/%zu", (char *)uriP->path.asInteger[i]);
		}
	}
	return key;
}

bool endpoints::string_to_uri(const char * uri_str, uri_t * uriP)
{
	if (uri_str == nullptr||uriP == nullptr) return false;

	LOG_CREATE(ALL, std::clog);
	LOG(DEBUG,"CP #1");
	char * p;

	int length = strlen(uri_str);
	LOG(DEBUG,"CP #2");
	char * token = new char [length + 1];

	strncpy (token, uri_str, length);
	LOG(DEBUG,"CP #3");

	uriP->count = 0;
	token = strtok_r (token, "/", &p);
	LOG(DEBUG,"token =", token);
	strcpy(const_cast<char *>(uriP->path.asString[uriP->count]), token);
	uriP->count++;

	LOG(DEBUG,"CP #4");
	while (token != NULL)
	{
		LOG(DEBUG,"CP #", (5 + uriP->count));
		token = strtok_r(NULL, "/", &p);
		LOG(DEBUG,"token =", token);
		strcpy(const_cast<char *>(uriP->path.asString[uriP->count]), token);
		uriP->count++;
	}
	LOG(DEBUG,"CP #9");
	if ((uriP->path.asString[0][0] & 0xF0)^0x30 == 0) {
		uriP->type = URI_TYPE_INTEGER;
		int path[4];
		LOG(DEBUG,uriP->count);
		for (int i = 0; i < uriP->count; i++)
		{
			LOG(DEBUG,"CP #10");
			LOG(DEBUG,uriP->path.asString[i]);
			strcpy(const_cast<char *>(uriP->path.asString[uriP->count]), token);
			uriP->path.asInteger[i] = path[i];
			LOG(DEBUG,"CP #11");
		}
	}
	else {
		uriP->type = URI_TYPE_STRING;
	}
	delete [] token;
	LOG_DELETE;
	return true;
}

bool endpoints::add(endpoint_t & new_ep)
{
	key_t key = nullptr;
	size_t length = 0;

	key = create_key(new_ep.uriP);

	if (key == nullptr) return false;

	_context.insert({key, new_ep});

	delete [] key;
	return true;
}

bool endpoints::del(key_t key)
{
	if (_context.erase(key) == 0) return false;
	return true;
}

bool endpoints::find(key_t key, endpoint_t * found)
{
	auto context = _context.find (key);
	if (context == _context.end()) return false;
	std::memmove(found, &context, sizeof(context));
	return true;
}

std::ostream & operator<<(std::ostream & os,const endpoints::endpoint_t &ep)
{
	static size_t count = 0;
	os << "Endpoint #" << ++count << " : count: " << ep.uriP->count;
	for (size_t i = 0; i < ep.uriP->count; i++)
	{
		if (ep.uriP->type == URI_TYPE_STRING) {
			os << " : " << ep.uriP->path.asString[i];
		}
		else if (ep.uriP->type == URI_TYPE_INTEGER) {
			os << " : " << ep.uriP->path.asInteger[i];
		}
	}
	os << " method";
	for (size_t code = METHOD_MIN; code < METHODS_COUNT; code++)
	{
		os << " code : " << static_cast<int>(ep.method_handler[code].method_code) <<
		//" handler : " << ep.method_handler[code].handler <<
			" attribute : " << ep.method_handler[code].attribute;
	}
	return os;
}

std::ostream & operator<<(std::ostream & os,const std::map <endpoints::key_t, endpoints::endpoint_t> &context)
{
	for (const auto &current : context)
	{
		os << static_cast<endpoints::endpoint_t>(current.second);
	}
	return os;
}

std::ostream & operator<<(std::ostream & os,const endpoints &object)
{
	os << object._context;
	return os;
}

bool endpoints::fill_endpoint (uri_t * uriP, endpoints::handler_t handlers[METHODS_COUNT], const char * attributes[METHODS_COUNT], endpoints::endpoint_t * ep)
{
	if (uriP == nullptr || attributes == nullptr || ep == nullptr) return false;

	ep->uriP = uriP;

	for (size_t i = 0; i < METHODS_COUNT; i++)
	{
		ep->method_handler[i].method_code = static_cast<method_code_t>(i);
		ep->method_handler[i].handler = handlers[i];
		ep->method_handler[i].attribute = attributes[i];
	}
	return true;
}

}//coap