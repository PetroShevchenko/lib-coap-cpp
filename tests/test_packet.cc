#include "packet.h"
#include "log.h"

LOG_USING_NAMESPACE

int main()
{
	LOG_INIT(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);

	try {
	
		static packet & instance = new_packet();
	
		instance.set_message_version(0b11);
		instance.set_message_type(0b11);
		instance.set_message_tokenLength(1);
		//instance.set_message_headerInfo(0x43);

		std::uint8_t tmp = instance.get_message_headerInfo();

		LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
		LOG(DEBUG,"header info = ", static_cast<unsigned short>(tmp));

		if (!delete_packet(instance))
			LOG(DEBUG, "Packet was not deleted");
	}
	catch(...)
	{
		LOG(DEBUG,"It was cought an exception");
		exit(1);
	}

	return 0;
}