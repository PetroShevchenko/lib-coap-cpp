#ifndef ENDPOINTS_H
#define ENDPOINTS_H
#include <functional>
#include <map>
#include "packet.h"
#include "uri.h"
#include "log.h"
#include "error.h"

namespace coap {

class endpoints;

class endpointsDestroyer {
private:
	endpoints * _instanceP;

public:
	~endpointsDestroyer();
	void initialize(endpoints * p);
};

class endpoints {

public:
	using key_t = const char *;
	using handler_t = std::function<void(void)>;

	using method_handler_t =
	struct method_handler_s {
		method_code_t method_code;
		handler_t handler;
		const char * attribute;
	};

	using endpoint_t =
	struct endpoint_s {
		const uri_t * uriP;
		method_handler_t method_handler [METHODS_COUNT];
	};

protected:
	friend class endpointsDestroyer;
	endpoints();
	~endpoints();

private:
	LOG_CREATE(NONE,std::clog);

	static endpoints * _instanceP;
	static endpointsDestroyer _destroyer;

	std::map <key_t, endpoint_t> _context;

	error _error;

	static key_t create_key(const uri_t * uriP);

public:

	friend std::ostream & operator<<(std::ostream & os,const endpoint_t &ep);
	friend std::ostream & operator<<(std::ostream & os,const std::map <key_t, endpoint_t> &context);
	friend std::ostream & operator<<(std::ostream & os,const endpoints &object);

	static bool fill_endpoint (uri_t * uriP, endpoints::handler_t handlers[METHODS_COUNT], const char * attributes[METHODS_COUNT], endpoints::endpoint_t * ep);
	static bool string_to_uri(const char * uri_str, uri_t * uriP);

	bool add(endpoint_t & ep);
	bool del(key_t key);
	bool find(key_t key, endpoint_t *ep);

	std::map <key_t, endpoint_t> &get_context()
	{
		return _context;
	}

	endpoints(const endpoints &) = delete;
	endpoints & operator=(const endpoints &) = delete;
	static endpoints & createInstance();

};

}//coap
#endif