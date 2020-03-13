#ifndef BLOCKWISE_H
#define BLOCKWISE_H
#include <vector>
#include <cstdint>
#include "log.h"
#include "error.h"
#include "packet.h"

namespace coap {

class blockwise {
private:
	LOG_CREATE(ALL,std::clog);
	std::vector<std::uint8_t> _data;
	bool decode_block_option(const packet::option_t * optionP);
	bool decode_size_option(const packet::option_t * optionP);

protected:
	std::uint32_t _number;
	std::uint32_t _offset;
	std::uint16_t _size;
	bool _more;

public:
	blockwise();
	~blockwise();
	virtual bool get_header (packet & pack) = 0;
	virtual bool set_header (packet & pack) = 0;
};


class block1 : public blockwise {
private:
	LOG_CREATE(ALL,std::clog);
	bool has_block1_option(packet & pack, size_t * optionQuantityP);
public:
	bool get_header (packet & pack);
	bool set_header (packet & pack);
};

class block2: public blockwise {
private:
	LOG_CREATE(ALL,std::clog);
public:
	bool get_header (packet & pack);
	bool set_header (packet & pack);
};
}//coap

#endif