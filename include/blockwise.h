#ifndef BLOCKWISE_H
#define BLOCKWISE_H
#include <vector>
#include <cstdint>
#include "error.h"
#include "packet.h"
#include "uri.h"

namespace coap {

class blockwise {
private:
	std::vector<std::uint8_t> _data;

protected:
	std::uint32_t _number;	// block number
	std::uint32_t _offset;	// offset of stored file
	std::uint16_t _size; 	// block size
	std::uint32_t _total;	// total file size
	bool _more;				// more bit
	bool decode_block_option(const packet::option_t * optionP);
	bool decode_size_option(const packet::option_t * optionP);
	bool encode_block_option(packet::option_t * optionP);

public:
	blockwise():_number(0),_offset(0),_size(0),_total(0), _more(false){}
	virtual ~blockwise(){}
	virtual bool get_header (packet & pack) = 0;
	virtual bool set_header (std::uint16_t port, uri & uri, packet & pack) = 0;
	std::uint32_t get_number() const
	{
		return _number;
	}
	std::uint32_t get_offset() const
	{
		return _offset;
	}
	std::uint16_t get_size() const
	{
		return _size;
	}
	std::uint32_t get_total() const
	{
		return _total;
	}
	bool get_more() const
	{
		return _more;
	}
	void set_number(std::uint32_t number)
	{
		_number = number;
	}
	void set_offset(std::uint32_t offset)
	{
		_offset = offset;
	}
	void set_size(std::uint16_t size)
	{
		_size = size;
	}
	void set_total(std::uint32_t total)
	{
		_total = total;
	}
	void set_more(bool more)
	{
		_more = more;
	}
};


class block1 : public blockwise {
private:
	bool get_block1_option(packet & pack, size_t * optionQuantityP, const packet::option_t ** optionPP);
public:
	block1():blockwise(){}
	~block1(){}
	bool get_header (packet & pack);
	bool set_header (std::uint16_t port, uri & uri, packet & pack);
};

class block2: public blockwise {
private:
	bool get_block2_option(packet & pack, size_t * optionQuantityP, const packet::option_t ** optionPP);
public:
	block2():blockwise(){}
	~block2(){}
	bool get_header (packet & pack);
	bool set_header (std::uint16_t port, uri & uri, packet & pack);
};
}//coap

#endif