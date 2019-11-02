#ifndef PACKET_H
#define PACKET_H
#include "log.h"
#include <cstdint>
#include <vector>

namespace coap {

class packet;

class packetDestroyer {
private:
	packet * instanceP;

public:
	~packetDestroyer();
	void initialize(packet * p);
};

class packet {

protected:
	packet(){}
	~packet(){}
	/*packet(const packet &);
	packet & operator=(const packet &);*/
	friend class packetDestroyer;

private:
	static packet * instanceP;
	static packetDestroyer destroyer;

	using message_t = 
	struct {
		union {
			std::uint8_t headerInfo;
			std::uint8_t version: 2;
			std::uint8_t type: 2;
			std::uint8_t tokenLength: 4;
		};
		union {
			std::uint8_t code;
			std::uint8_t codeClass: 3;
			std::uint8_t codeDetail: 5;
		};
		std::uint16_t messageId;
		std::vector <uint8_t> token;
		const std::uint8_t payloadMarker = 0xFF;
		std::vector <uint8_t> payload;
	};
	message_t message;

public:
	//packet() = delete;
	//~packet() = delete;
	packet(const packet &) = delete;
	packet & operator=(const packet &) = delete;

	static packet & createInstance();
};

}//coap

#endif