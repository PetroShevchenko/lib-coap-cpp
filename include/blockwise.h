#ifndef BLOCKWISE_H
#define BLOCKWISE_H
#include <vector>
#include <cstdint>
//#include "log.h"
#include "error.h"
#include "packet.h"

namespace coap {

class blockwise {
private:
	//LOG_CREATE(ALL,std::clog);
	std::vector<std::uint8_t> _data;

protected:
	std::uint32_t _number;
	std::uint32_t _offset;
	std::uint16_t _size;
	bool _more;
	bool decode_block_option(const packet::option_t * optionP);
	bool decode_size_option(const packet::option_t * optionP);

public:
	blockwise():_number(0),_offset(0),_size(0),_more(false){}
	~blockwise();
	virtual bool get_header (packet & pack) = 0;
	virtual bool set_header (packet & pack) = 0;
};


class block1 : public blockwise {
private:
	//LOG_CREATE(ALL,std::clog);
	bool get_block1_option(packet & pack, size_t * optionQuantityP, const packet::option_t ** optionPP);
public:
	bool get_header (packet & pack);
	bool set_header (packet & pack);
};

class block2: public blockwise {
private:
	//LOG_CREATE(ALL,std::clog);
public:
	bool get_header (packet & pack);
	bool set_header (packet & pack);
};
}//coap

#endif