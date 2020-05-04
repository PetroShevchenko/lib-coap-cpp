#include <arpa/inet.h>
#include "packet.h"
#include "cstring"
#include "log.h"

using namespace coap;
LOG_USING_NAMESPACE
LOG_GLOBAL_DECLARE


int main(int argc, char ** argv)
{
	(void)argc;
	(void)argv;
	try {
		LOG_CREATE(ALL, std::clog);

		std::uint8_t opt_value[32];
		static packet & coap = new_packet();

		LOG(DEBUGGING, "Creating of COAP packet");

		coap.clean_options();

		memcpy(opt_value, "3", sizeof(char));
		coap.add_option(URI_PATH, opt_value, sizeof(char));

		memcpy(opt_value, "0", sizeof(char));
		coap.add_option(URI_PATH, opt_value, sizeof(char));

		memcpy(opt_value, "0", sizeof(char));
		coap.add_option(URI_PATH, opt_value, sizeof(char));
		uint16_t opt = TLV;
		opt = htons(opt);
		memcpy(opt_value, &opt, sizeof(uint16_t));
		coap.add_option(ACCEPT, opt_value, sizeof(uint16_t));

		coap.make_request(CONFIRMABLE, GET,	nullptr, 0);

		size_t length = 0;

		if (!coap.serialize (nullptr, &length, true)) {
			LOG(ERROR, "unable to check buffer size, corrupted message");
			return 1;
		}

		LOG(DEBUGGING, "length : ", length);

		std::uint8_t * bufferP = new std::uint8_t [length];

		if (!coap.serialize (bufferP, &length, false)) {
			LOG(ERROR, "unable to serialize the coap packet");
			return 1;
		}

		LOG(DEBUGGING, "Serialized packet:");

		LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);

		for (size_t i = 0; i < length; i++)
		{
			std::clog << static_cast<short>(bufferP[i]) << " ";
			if (i >= 16 && i % 16 == 0) std::clog << "\n";			
		}
		std::clog << "\n";
		LOG_SET_STREAM_FORMAT(std::ios::dec, std::ios::basefield);
		delete [] bufferP;

		LOG(DEBUGGING, "COAP packet was formed completly. Copy it to keyboard buffer");
		LOG_DELETE;

	}
	catch(...) {
		std::cerr << "Exception was cought";
		return 1;
	}
	return 0;
}