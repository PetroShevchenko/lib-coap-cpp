#ifndef ENDPOINT_H
#define ENDPOINT_H
#include <vector>
#include <functional>
#include <string>
#include <cassert>
#include "uri.h"
#include "packet.h"
#include "connection.h"

namespace coap {

class endpoint {

public:
	virtual void registration_step() = 0;
	virtual void transaction_step() = 0;

protected:
	std::string _name;
	std::vector<connection *> _connections;

public:
	endpoint (std::string name):  _name(name)
	{
	}

	virtual ~endpoint()
	{
	}
	endpoint(const endpoint &) = delete;
	endpoint & operator=(const endpoint &) = delete;
	void add_connection(connection * newConnection)
	{
		assert(newConnection);
		_connections.push_back(newConnection);
	}
};

}//coap
#endif