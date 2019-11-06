#include "packet.h"
#include "log.h"

LOG_USING_NAMESPACE

int main()
{
	LOG_INIT(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);

	try {
	
		static packet & instance = new_packet();
	
		instance.set_message_version(COAP_VERSION);
		instance.set_message_type(NON_CONFIRMABLE);
		instance.set_message_tokenLength(4);
		instance.set_message_code(GET);
		instance.set_message_messageId(1);
		



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