#include <cmath>

#include "blockwise.h"

#if 0
int coap_get_header_block1(void *packet, uint32_t *num, uint8_t *more, uint16_t *size, uint32_t *offset)
{
	coap_packet_t *const coap_pkt = (coap_packet_t *) packet;
	if (!IS_OPTION(coap_pkt, COAP_OPTION_BLOCK1)) return 0;
	/* pointers may be NULL to get only specific block parameters */
	if (num!=NULL) *num = coap_pkt->block1_num;
  	if (more!=NULL) *more = coap_pkt->block1_more;
  	if (size!=NULL) *size = coap_pkt->block1_size;
  	if (offset!=NULL) *offset = coap_pkt->block1_offset;
  	return 1;
}
#endif

namespace coap {

enum {
	BLOCK_SZX_MASK 	= 0x7,
	BLOCK_M_BIT		= 3,
	BLOCK_NUM_SHIFT	= 4
};

bool blockwise::decode_block_option(const packet::option_t * optionP)
{
	bool status = false;
	if (optionP->number != BLOCK_1
		|| optionP->number != BLOCK_2) {
		LOG(DEBUG, "There are no any BLOCK options");
		return false;
	}
	std::uint8_t szx;
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
			_number = static_cast<std::uint32_t>(optionP->value[1] >> BLOCK_NUM_SHIFT | optionP->value[0] << BLOCK_NUM_SHIFT);
			status = true;
			break;

		case 3:
			szx = optionP->value[2] & BLOCK_SZX_MASK;
			_size = static_cast<std::uint16_t>(pow(2, szx + 4));
			_more = optionP->value[2] & (1 << BLOCK_M_BIT);
			_number = static_cast<std::uint32_t>(optionP->value[2] >> BLOCK_NUM_SHIFT 
																| optionP->value[1] << BLOCK_NUM_SHIFT
																| optionP->value[0] << (BLOCK_NUM_SHIFT + 8));
			status = true;
			break;

		default:
			LOG(DEBUG, "Wrong size of option");
			status = false;
			break;	
	}
	return status;
}

bool blockwise::decode_size_option(const packet::option_t * optionP)
{
	//TODO
	return false;
}

bool block1::has_block1_option(packet & pack, size_t * optionQuantityP)
{
	const packet::option_t * optionP = pack.find_options(BLOCK_1, optionQuantityP);
	if (optionP == nullptr) {
		return false;
	}
	return true;
}

bool block1::get_header (packet & pack)
{
  	size_t optionQuantity;
  	if (has_block1_option(pack, &optionQuantity) == false) {
		LOG(DEBUG, "There is no BLOCK 1 option in the packet");
  		return false;
  	}
  	//TODO
}

}//coap