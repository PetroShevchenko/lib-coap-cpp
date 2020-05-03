#include <cmath>
#include <cassert>
#include "blockwise.h"
#include "log.h"

LOG_USING_NAMESPACE
LOG_EXTERN_DECLARE

namespace coap {

enum {
	BLOCK_SZX_MASK 	= 0x7,
	BLOCK_M_BIT		= 3,
	BLOCK_NUM_SHIFT	= 4
};
#if 0
blockwise::blockwise():_number(0),_offset(0),_size(0),_more(false)
{
}
#endif
bool blockwise::decode_block_option(const packet::option_t * optionP)
{
	bool status = false;
	if (optionP->number != BLOCK_1
		|| optionP->number != BLOCK_2) {
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

bool blockwise::decode_size_option(const packet::option_t * optionP)
{
	bool status = false;
	if (optionP->number != SIZE_1
		|| optionP->number != SIZE_2) {
		LOG(DEBUGGING, "There are no any SIZE options");
		return false;
	}
	bool little_endian = packet::is_little_endian_byte_order();
	switch(optionP->value.size())
	{
		case 1:
			_offset += optionP->value[0];
			break;

		case 2:
			if (little_endian == true) {
				_offset += optionP->value[0] << 8 | optionP->value[1];//Little endian
			}
			else {
				_offset += optionP->value[0] >> 8 | optionP->value[1];//Big endian
			}
			break;

		case 3:
			if (little_endian == true) {
				_offset += optionP->value[0] << 16 | optionP->value[1] << 8 | optionP->value[2];//Little endian
			}
			else {
				_offset += optionP->value[0] >> 16 | optionP->value[1] >> 8 | optionP->value[2];//Big endian
			}
			break;

		case 4:
			if (little_endian == true) {
				_offset += optionP->value[0] << 24 | optionP->value[1] << 16
							| optionP->value[2] << 8 | optionP->value[3];//Little endian
			}
			else {
				_offset += optionP->value[0] >> 24 | optionP->value[1] >> 16
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

}//coap