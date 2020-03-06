#include "blockwise.h"



int
coap_get_header_block1(void *packet, uint32_t *num, uint8_t *more, uint16_t *size, uint32_t *offset)
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

namespace coap {

bool block1::get_header (packet & pack)
{
	size_t optionQuantity;
	const packet::option_t * optionP = pack.find_options(BLOCK_1, &optionQuantity);
	if (optionP == nullptr) {
		LOG(DEBUG, "There is no BLOCK 1 option in the packet");
		return false;
	}
	//TODO
}




}//coap