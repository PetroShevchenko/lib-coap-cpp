#include "packet.h"
#include "log.h"

LOG_USING_NAMESPACE

int main()
{
	LOG_CREATE(NONE, std::clog);
	LOG_SET_LEVEL(DEBUG);

	std::uint8_t test1_coap_packet[] = {// in network order

	   0x68, 0x45, 0x1a, 0xb1, 0x74, 0xa5, 0xe2, 0xc5, 0x10, 0x93, 0xb3, 0xd8, 0xc2, 0x2d, 0x16, 0xff,
	   0xc8, 0x00, 0x14, 0x4f, 0x70, 0x65, 0x6e, 0x20, 0x4d, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x20, 0x41,
	   0x6c, 0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0xc8, 0x01, 0x16, 0x4c, 0x69, 0x67, 0x68, 0x74, 0x77,
	   0x65, 0x69, 0x67, 0x68, 0x74, 0x20, 0x4d, 0x32, 0x4d, 0x20, 0x43, 0x6c, 0x69, 0x65, 0x6e, 0x74,
	   0xc8, 0x02, 0x09, 0x33, 0x34, 0x35, 0x30, 0x30, 0x30, 0x31, 0x32, 0x33, 0xc3, 0x03, 0x31, 0x2e,
	   0x30, 0x86, 0x06, 0x41, 0x00, 0x01, 0x41, 0x01, 0x05, 0x88, 0x07, 0x08, 0x42, 0x00, 0x0e, 0xd8,
	   0x42, 0x01, 0x13, 0x88, 0x87, 0x08, 0x41, 0x00, 0x7d, 0x42, 0x01, 0x03, 0x84, 0xc1, 0x09, 0x64,
	   0xc1, 0x0a, 0x0f, 0x83, 0x0b, 0x41, 0x00, 0x00, 0xc8, 0x0d, 0x08, 0x00, 0x00, 0x00, 0x00, 0xaf,
	   0x52, 0x25, 0xa5, 0xc6, 0x0e, 0x2b, 0x30, 0x31, 0x3a, 0x30, 0x30, 0xc8, 0x0f, 0x0d, 0x45, 0x75,
	   0x72, 0x6f, 0x70, 0x65, 0x2f, 0x42, 0x65, 0x72, 0x6c, 0x69, 0x6e, 0xc1, 0x10, 0x55
	};

	std::uint8_t test2_coap_packet[] = {// in network order

		0x44, 0x02, 0x13, 0xe9, 0xe9, 0x13, 0xa3, 0x3f, 0xb2, 0x72, 0x64, 0x11, 0x28, 0x39, 0x6c, 0x77,
		0x6d, 0x32, 0x6d, 0x3d, 0x31, 0x2e, 0x31, 0x0d, 0x01, 0x65, 0x70, 0x3d, 0x74, 0x65, 0x73, 0x74,
   		0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x03, 0x62, 0x3d, 0x55, 0x06, 0x6c, 0x74, 0x3d, 0x33,
   		0x36, 0x30, 0xff, 0x3c, 0x2f, 0x3e, 0x3b, 0x72, 0x74, 0x3d, 0x22, 0x6f, 0x6d, 0x61, 0x2e, 0x6c,
   		0x77, 0x6d, 0x32, 0x6d, 0x22, 0x3b, 0x63, 0x74, 0x3d, 0x31, 0x31, 0x30, 0x2c, 0x3c, 0x2f, 0x31,
   		0x2f, 0x30, 0x3e, 0x2c, 0x3c, 0x2f, 0x32, 0x2f, 0x30, 0x3e, 0x2c, 0x3c, 0x2f, 0x33, 0x2f, 0x30,
   		0x3e, 0x2c, 0x3c, 0x2f, 0x34, 0x2f, 0x30, 0x3e, 0x2c, 0x3c, 0x2f, 0x35, 0x2f, 0x30, 0x3e, 0x2c,
   		0x3c, 0x2f, 0x36, 0x2f, 0x30, 0x3e, 0x2c, 0x3c, 0x2f, 0x37,	0x2f, 0x30, 0x3e, 0x2c, 0x3c, 0x2f,
   		0x33, 0x31, 0x30, 0x32, 0x34, 0x2f, 0x31, 0x30, 0x3e, 0x2c, 0x3c, 0x2f, 0x33, 0x31, 0x30, 0x32,
   		0x34, 0x2f, 0x31, 0x31, 0x3e, 0x2c, 0x3c, 0x2f, 0x33, 0x31, 0x30, 0x32, 0x34, 0x2f, 0x31, 0x32,
   		0x3e
   	};

	try {

		static packet & instance = new_packet();

		if (!instance.parse(test2_coap_packet, sizeof(test2_coap_packet))) {
			LOG(ERROR,"parse is failure");
			throw;
		}

		LOG(DEBUG, "CoAP Version: ", static_cast<int>(instance.get_message_version()));
		LOG(DEBUG, "Message type: ", static_cast<int>(instance.get_message_type()));
		LOG(DEBUG, "Token length: ", static_cast<int>(instance.get_message_tokenLength()));
		LOG(DEBUG, "Command code: ", static_cast<int>(instance.get_message_code()));
		LOG(DEBUG, "Message ID: ", instance.get_message_messageId());

		//LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
		LOG(DEBUG, instance);

		const packet::option_t * optP;
		size_t quantity;

		optP = instance.find_options (0, &quantity);
		LOG(DEBUG, *optP);
		LOG(DEBUG, "Number of options #0 : ",quantity);

		LOG_SET_STREAM_FORMAT(std::ios::dec, std::ios::basefield);

		size_t length = 0;

		if (!instance.serialize (nullptr, &length, true)) {
			LOG(ERROR, "unable to check buffer size, corrupted message");
			throw;
		}

		LOG(DEBUG, "buffer length is ", length);

		std::uint8_t * buffer = new unsigned char [length];

		instance.set_option(IF_MATCH);
		instance.set_option(URI_HOST);
		instance.set_option(ETAG);
		instance.set_option(IF_NONE_MATCH);
		instance.set_option(URI_PORT);
		instance.set_option(LOCATION_PATH);
		instance.set_option(URI_PATH);
		instance.set_option(CONTENT_FORMAT);
		instance.set_option(MAX_AGE);
		instance.set_option(URI_QUERY);
		instance.set_option(ACCEPT);
		instance.set_option(LOCATION_QUERY);
		instance.set_option(BLOCK_2);
		instance.set_option(BLOCK_1);
		instance.set_option(SIZE_2);
		instance.set_option(PROXY_URI);
		instance.set_option(PROXY_SCHEME);
		instance.set_option(SIZE_1);

		LOG(DEBUG, "option_bitmap[0] = ", instance.get_option_bitmap()[0] );
		LOG(DEBUG, "option_bitmap[1] = ", instance.get_option_bitmap()[1] );

		if (!instance.serialize(buffer, &length, false)) {
			LOG(ERROR, "serialize is failure");
		}
		else {
			LOG(DEBUG,"\nbuffer[ ", length, " ] = ");
			LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
			for(size_t i = 0; i < length; i++)
			{
				std::clog << static_cast<unsigned short>(buffer [i]) << " ";
				if ( i % 16 == 0) std::clog << "\n";
			}
		}

		delete [] buffer;

		std::uint16_t messageId = instance.get_message_messageId();

		const std::uint8_t payload[] = "{""result"": ""Hello JSON-RPC"", ""error"": null, ""id"":1}";

		instance.prepare_answer (ACKNOWLEDGEMENT, ++messageId, CONTENT, payload, sizeof(payload), JSON);

		if (!instance.serialize (nullptr, &length, true)) {
			LOG(ERROR, "unable to check buffer size, corrupted message");
			throw;
		}

		LOG(DEBUG, "ANSWER :");

		LOG(DEBUG, "buffer length is ", length);

		buffer = new unsigned char [length];

		if (!instance.serialize(buffer, &length, false)) {
			LOG(ERROR, "serialize is failure");
		}
		else {
			LOG(DEBUG,"\nbuffer[ ", length, " ] = ");
			LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
			for(size_t i = 0; i < length; i++)
			{
				std::clog << static_cast<unsigned short>(buffer [i]) << " ";
				if ( i % 16 == 0) std::clog << "\n";
			}
		}

		delete [] buffer;


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