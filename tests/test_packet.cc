#include "packet.h"
#include "log.h"

LOG_USING_NAMESPACE

int main()
{
	LOG_CREATE(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);

	try {

		static packet & instance = new_packet();

		instance.set_message_version(COAP_VERSION);
		instance.set_message_type(NON_CONFIRMABLE);
		instance.set_message_tokenLength(4);
		instance.set_message_code(GET);
		instance.set_message_messageId(1);

		//std::cout << __func__<< __FILE__ << std::endl;

		//std::uint8_t tmp = instance.get_message_headerInfo();
		//LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
		LOG(DEBUG, instance);

		if (!delete_packet(instance))
			LOG(DEBUG, "Packet was not deleted");
	}
	catch(...)
	{
		LOG(DEBUG,"It was cought an exception");
		LOG_DELETE;
		exit(1);
	}
	LOG_DELETE;
	return 0;
}