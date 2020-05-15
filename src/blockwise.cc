#include <cmath>
#include <cassert>
#ifdef STM32H747I_DISCO
#include "lwip.h"
#else
#include <arpa/inet.h>
#endif
#include "blockwise.h"
#include "log.h"

LOG_USING_NAMESPACE
LOG_EXTERN_DECLARE

namespace coap {

enum {
	BLOCK_SZX_MASK 	= 0x7,
	BLOCK_M_BIT		= 3,
	BLOCK_NUM_SHIFT	= 4,
	BLOCK_MAX_SIZE 	= 2048,
	BLOCK_SIZE_DEFAULT = BLOCK_MAX_SIZE
};


unsigned int blockwise::size_to_sizeoption(unsigned int size)
{
	const unsigned int size_options [8] = {
		16, 32, 64, 128, 256, 512, 1024, 2048
	};
	for (int opt = 0; opt < 8; opt++)
	{
		if (size_options[opt] == size)
			return opt;
	}
	return 2;
}

bool blockwise::decode_block_option(const packet::option_t * optionP)
{
	bool status = false;
	if (optionP->number != BLOCK_1
		&& optionP->number != BLOCK_2) {
		LOG(DEBUGGING, "There are no any BLOCK options");
		return false;
	}
	std::uint8_t szx;
	bool little_endian = packet::is_little_endian_byte_order();
	switch(optionP->value.size())
	{
		case 1:
			szx = optionP->value[0] & BLOCK_SZX_MASK;
			_size = static_cast<std::uint16_t>(pow(2, szx + 4));
			_more = optionP->value[0] & (1 << BLOCK_M_BIT);
			_number = static_cast<std::uint32_t>(optionP->value[0] >> BLOCK_NUM_SHIFT);
			status = true;
			break;

		case 2:
			szx = optionP->value[1] & BLOCK_SZX_MASK;
			_size = static_cast<std::uint16_t>(pow(2, szx + 4));
			_more = optionP->value[1] & (1 << BLOCK_M_BIT);
			if (little_endian == true) {
				_number = static_cast<std::uint32_t>(optionP->value[1] >> BLOCK_NUM_SHIFT
													| optionP->value[0] << BLOCK_NUM_SHIFT);//Little endian
			}
			else {
				_number = static_cast<std::uint32_t>(optionP->value[1] << BLOCK_NUM_SHIFT
													| optionP->value[0] >> BLOCK_NUM_SHIFT);//Big endian
			}
			status = true;
			break;

		case 3:
			szx = optionP->value[2] & BLOCK_SZX_MASK;
			_size = static_cast<std::uint16_t>(pow(2, szx + 4));
			_more = optionP->value[2] & (1 << BLOCK_M_BIT);
			if (little_endian == true) {
				_number = static_cast<std::uint32_t>(optionP->value[2] >> BLOCK_NUM_SHIFT
																| optionP->value[1] << BLOCK_NUM_SHIFT
																| optionP->value[0] << (BLOCK_NUM_SHIFT + 8));//Little endian
			}
			else {
				_number = static_cast<std::uint32_t>(optionP->value[2] << BLOCK_NUM_SHIFT
																| optionP->value[1] >> BLOCK_NUM_SHIFT
																| optionP->value[0] >> (BLOCK_NUM_SHIFT + 8));//Big endian
			}
			status = true;
			break;

		default:
			LOG(DEBUGGING, "Wrong size of option");
			status = false;
			break;
	}
	return status;
}

bool blockwise::encode_block_option(packet::option_t * optionP)
{
	assert(optionP != nullptr);
	std::uint8_t tmp = 0;
	unsigned int size_opt = 0;

	LOG(DEBUGGING,"Entering");
	
	if (optionP->number != BLOCK_1
		&& optionP->number != BLOCK_2) {
		LOG(DEBUGGING, "There are no any BLOCK options");
		return false;
	}
	if (_size % 16 || _size > BLOCK_MAX_SIZE) {
		LOG(DEBUGGING,"Invalid block size value");
		return false;
	}
	size_opt = size_to_sizeoption((unsigned int)_size);
	if (_number < 16) {
		tmp = (std::uint8_t)(size_opt) & BLOCK_SZX_MASK;
		if (_more) {
			tmp |= (std::uint8_t)(1 << BLOCK_M_BIT);	
		}
		else {
			tmp &= ~((std::uint8_t)(1 << BLOCK_M_BIT));
		}
		tmp |= (std::uint8_t)(_number << BLOCK_NUM_SHIFT);
		optionP->value.push_back(tmp);
	}
	else {
		bool little_endian = packet::is_little_endian_byte_order();
		if (_number < 4096) {
			if (little_endian == true) {
				tmp = (std::uint8_t)(_number & 0xFF) >> BLOCK_NUM_SHIFT;
				tmp |= ((std::uint8_t)((_number >> 8) & 0xFF)) << BLOCK_NUM_SHIFT;//Little endian
				optionP->value.push_back(tmp);
				tmp = (std::uint8_t)(_number & 0xF) << BLOCK_NUM_SHIFT;
			}
			else {
				tmp = (std::uint8_t)((_number >> 24) & 0xFF) >> BLOCK_NUM_SHIFT;				
				tmp |= ((std::uint8_t)((_number >> 16) & 0xFF)) << BLOCK_NUM_SHIFT;//Big endian
				optionP->value.push_back(tmp);
				tmp = (std::uint8_t)((_number >> 24) & 0xF) << BLOCK_NUM_SHIFT;
			}
			tmp |= (std::uint8_t)(size_opt) & BLOCK_SZX_MASK;
			if (_more) {
				tmp |= (std::uint8_t)(1 << BLOCK_M_BIT);	
			}
			else {
				tmp &= ~((std::uint8_t)(1 << BLOCK_M_BIT));
			}			
			optionP->value.push_back(tmp);
		}
		else if (_number < 1048576) {//TODO
			if (little_endian == true) {
				tmp = (std::uint8_t)(_number & 0xFF);
				optionP->value.push_back(tmp);
				tmp = (std::uint8_t)((_number >> 8) & 0xFF) >> BLOCK_NUM_SHIFT;
				tmp |= ((std::uint8_t)((_number >> 16) & 0xFF)) << BLOCK_NUM_SHIFT;//Little endian
				optionP->value.push_back(tmp);
				tmp = (std::uint8_t)((_number >> 8) & 0xF) << BLOCK_NUM_SHIFT;
			}
			else {
				tmp = (std::uint8_t)((_number >> 24) & 0xFF);
				optionP->value.push_back(tmp);
				tmp = (std::uint8_t)((_number >> 16) & 0xFF) >> BLOCK_NUM_SHIFT;
				tmp |= ((std::uint8_t)((_number >> 8) & 0xFF)) << BLOCK_NUM_SHIFT;//Big endian
				optionP->value.push_back(tmp);
				tmp = (std::uint8_t)((_number >> 16) & 0xF) << BLOCK_NUM_SHIFT;
			}
			tmp |= (std::uint8_t)(size_opt) & BLOCK_SZX_MASK;
			if (_more) {
				tmp |= (std::uint8_t)(1 << BLOCK_M_BIT);	
			}
			else {
				tmp &= ~((std::uint8_t)(1 << BLOCK_M_BIT));
			}
			optionP->value.push_back(tmp);
		}
		else {
			LOG(DEBUGGING, "Invalid block number value");
			return false;
		}
	}
	LOG(DEBUGGING,"Leaving");
	return true;
}

bool blockwise::decode_size_option(const packet::option_t * optionP)
{
	bool status = false;
	if (optionP->number != SIZE_1
		&& optionP->number != SIZE_2) {
		LOG(DEBUGGING, "There are no any SIZE options");
		return false;
	}
	bool little_endian = packet::is_little_endian_byte_order();
	switch(optionP->value.size())
	{
		case 1:
			_total = optionP->value[0];
			break;

		case 2:
			if (little_endian == true) {
				_total = optionP->value[0] << 8 | optionP->value[1];//Little endian
			}
			else {
				_total = optionP->value[0] >> 8 | optionP->value[1];//Big endian
			}
			break;

		case 3:
			if (little_endian == true) {
				_total = optionP->value[0] << 16 | optionP->value[1] << 8 | optionP->value[2];//Little endian
			}
			else {
				_total = optionP->value[0] >> 16 | optionP->value[1] >> 8 | optionP->value[2];//Big endian
			}
			break;

		case 4:
			if (little_endian == true) {
				_total = optionP->value[0] << 24 | optionP->value[1] << 16
							| optionP->value[2] << 8 | optionP->value[3];//Little endian
			}
			else {
				_total = optionP->value[0] >> 24 | optionP->value[1] >> 16
							| optionP->value[2] >> 8 | optionP->value[3];//Big endian
			}
			break;

		default:
			LOG(DEBUGGING, "Wrong size of option");
			status = false;
			break;
	}

	return status;
}

bool block1::get_block1_option(packet & pack, size_t * optionQuantityP, const packet::option_t ** optionPP)
{
	assert(optionQuantityP != nullptr);
	assert(optionPP != nullptr);
	*optionPP = pack.find_options(BLOCK_1, optionQuantityP);
	if (*optionPP == nullptr) {
		return false;
	}
	return true;
}

bool block1::get_header (packet & pack)
{
	size_t optionQuantity;
	const packet::option_t * optionP;
	if (get_block1_option(pack, &optionQuantity, &optionP) == false) {
		LOG(DEBUGGING, "There is no BLOCK 1 option in the packet");
		return false;
	}
	if (decode_block_option(optionP) == false) {
		LOG(DEBUGGING, "Unable to decode BLOCK1 option");
		return false;
	}
	return true;
}

bool block1::set_header (std::uint16_t port, uri & uri, packet & pack)
{
	// clean all options
	pack.clean_options();
	// add option URI_PORT
	port = htons(port);
	pack.add_option(URI_PORT, (const std::uint8_t *)&port, sizeof(std::uint16_t));
	// add options URI_PATH
	for (auto opt : uri.get_uri().asString)
	{
		pack.add_option(URI_PATH, (const std::uint8_t *)opt.c_str(), opt.length());
	}
	// if it is the first block
	if (_number == 0) {
		set_number(0);
		set_more(false);
		set_size(BLOCK_SIZE_DEFAULT);
	}
	packet::option_t option;
	option.number = BLOCK_1;
	if (!encode_block_option(&option)) {
		LOG(DEBUGGING,"Unable to encode BLOCK1 option");
		return false;
	}
	pack.add_option ((option_number_t)option.number, (const std::uint8_t *)option.value.data(), option.value.size());
	return true;
}

bool block2::get_block2_option(packet & pack, size_t * optionQuantityP, const packet::option_t ** optionPP)
{
	assert(optionQuantityP != nullptr);
	assert(optionPP != nullptr);
	*optionPP = pack.find_options(BLOCK_2, optionQuantityP);
	if (*optionPP == nullptr) {
		return false;
	}
	return true;
}

bool block2::get_header (packet & pack)
{
	size_t optionQuantity;
	const packet::option_t * optionP;
	if (get_block2_option(pack, &optionQuantity, &optionP) == false) {
		LOG(DEBUGGING, "There is no BLOCK 2 option in the packet");
		return false;
	}
	if (decode_block_option(optionP) == false) {
		LOG(DEBUGGING, "Unable to decode BLOCK2 option");
		return false;
	}
	return true;
}

bool block2::set_header (std::uint16_t port, uri & uri, packet & pack)
{
	LOG(DEBUGGING,"Entering");
	// clean all options
	pack.clean_options();
	// add option URI_PORT
	port = htons(port);
	pack.add_option(URI_PORT, (const std::uint8_t *)&port, sizeof(std::uint16_t));
	// add options URI_PATH
	for (auto opt : uri.get_uri().asString)
	{
		pack.add_option(URI_PATH, (const std::uint8_t *)opt.c_str(), opt.length());
	}
	// if it is the first block
	if (_number == 0) {
		set_number(0);
		set_more(false);
		set_size(BLOCK_SIZE_DEFAULT);
	}
	packet::option_t option;
	option.number = BLOCK_2;
	if (!encode_block_option(&option)) {
		LOG(DEBUGGING,"Unable to encode BLOCK2 option");
		return false;
	}
	pack.add_option ((option_number_t)option.number, (const std::uint8_t *)option.value.data(), option.value.size());

	LOG(DEBUGGING,"Leaving");
	return true;
}

}//coap