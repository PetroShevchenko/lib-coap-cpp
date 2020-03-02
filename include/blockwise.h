#ifndef BLOCKWISE_H
#define BLOCKWISE_H
#include "log.h"
#include "error.h"
#include <vector>

namespace coap {

class blockwise {
private:
	LOG_CREATE(ALL,std::clog);
	std::vector<std::uint8_t> _data;

public:
	blockwise();
	~blockwise();

};


class block1 : public blockwise {
private:
	LOG_CREATE(ALL,std::clog);
public:

};

class block2: public blockwise {
private:
	LOG_CREATE(ALL,std::clog);
public:

};
}//coap

#endif