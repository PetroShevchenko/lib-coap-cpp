#ifndef CONTEXT_H
#define CONTEXT_H
#include "endpoint.h"
#include "connection.h"
#include "error.h"
#include <vector>
#include <map>

namespace coap {

class context;

class contextDestroyer {
private:
	context * _instanceP;

public:
	~contextDestroyer();
	void initialize(context * p);
};

class context {

protected:
	context();
	~context();

	friend class contextDestroyer;

private:
	LOG_CREATE(ALL,std::clog);

	static context * _instanceP;
	static contextDestroyer _destroyer;
	static error _error;

	std::vector<endpoint *> _endpoints;

public:
	context(const context &) = delete;
	context & operator=(const context &) = delete;

	static context & createInstance();
	static bool clearInstance(context &);

	void add_endpoint(endpoint *);
	void registration_process();
	void transaction_process();
};


#define new_context() context::createInstance()
#define delete_context(instance) context::clearInstance(instance)

}//coap

#endif