#ifndef PACKET_H
#define PACKET_H
#include <cstdint>
#include <vector>
#include <iostream>
#include "log.h"

namespace coap {

enum {
	COAP_VERSION = 0b01
};

using message_type_t = 
enum {
	CONFIRMABLE 	= 0b00,
	NON_CONFIRMABLE = 0b01,
	ACKNOWLEDGEMENT = 0b10,
	RESET 			= 0b11
};	

using message_code_class_t = 
enum {
	METHOD 			= (0b000 << 5),
	SUCCESS 		= (0b010 << 5),
	CLIENT_ERROR 	= (0b100 << 5),
	SERVER_ERROR 	= (0b101 << 5),
	SIGNALING_CODES = (0b111 << 5)
};

using message_code_t = 
enum {
	EMPTY 	= METHOD | 0b00000,
	GET 	= METHOD | 0b00001,
	POST	= METHOD | 0b00010,
	PUT		= METHOD | 0b00011,
	DELETE 	= METHOD | 0b00100,
	FETCH 	= METHOD | 0b00101,
	PATCH 	= METHOD | 0b00110,
	IPATCH 	= METHOD | 0b00111,
	CREATED		= SUCCESS | 0b00001,
	DELETED		= SUCCESS | 0b00010,
	VALID		= SUCCESS | 0b00011,
	CHANGED		= SUCCESS | 0b00100,
	CONTENT		= SUCCESS | 0b00101,
	CONTINUE 	= SUCCESS | 0b111111,
	BAD_REQUEST		= CLIENT_ERROR | 0b00000,
	UNAUTHORIZED 	= CLIENT_ERROR | 0b00001,
	BAD_OPTION 		= CLIENT_ERROR | 0b00010,
	FORBIDDED		= CLIENT_ERROR | 0b00011,
	NOT_FOUND		= CLIENT_ERROR | 0b00100,
	METHOD_NOT_ALLOWED 	= CLIENT_ERROR | 0b00101,
	NOT_ACCEPTABLE		= CLIENT_ERROR | 0b00110,
	REQUEST_ENTITY_INCOMPLETE 	= CLIENT_ERROR | 0b01000,
	CONFLICT 			= CLIENT_ERROR | 0b01001,
	PRECONDITION_FAILED	= CLIENT_ERROR | 0b01100,
	REQUEST_ENTITY_TOO_LARGE 	= CLIENT_ERROR | 0b01101,
	UNSUPPORTED_CONTENT_FORMAT 	= CLIENT_ERROR | 0b01111,
	INTERNAL_SERVER_ERROR		= SERVER_ERROR | 0b00000,
	NOT_IMPLEMENTED				= SERVER_ERROR | 0b00001,
	BAD_GATEWAY					= SERVER_ERROR | 0b00010,
	SERVICE_UNAVAILABLE			= SERVER_ERROR | 0b00011,
	GATEWAY_TIMEOUT				= SERVER_ERROR | 0b00100,
	PROXYING_NOT_SUPPORTED		= SERVER_ERROR | 0b00101,
	UNASSIGNED 	= SIGNALING_CODES  | 0b00000,
	CSM		 	= SIGNALING_CODES  | 0b00001,
	PING 		= SIGNALING_CODES  | 0b00010,
	PONG 		= SIGNALING_CODES  | 0b00011,	
	RELEASE		= SIGNALING_CODES  | 0b00100,	
	ABORT 		= SIGNALING_CODES  | 0b00101
};

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
	packet();
	~packet(){}

	friend class packetDestroyer;

private:
	static packet * instanceP;
	static packetDestroyer destroyer;

	using message_t = 
	struct {
		union {
			std::uint8_t asByte;
			#pragma pack(push,1)
			struct {
				std::uint8_t version: 2;
				std::uint8_t type: 2;
				std::uint8_t tokenLength: 4;
			} asBitmap;
			#pragma pack(pop)
		} headerInfo;
		
		union {
			std::uint8_t asByte;
			#pragma pack(push,1)
			struct {
				std::uint8_t codeClass: 3;
				std::uint8_t codeDetail: 5;
			} asBitmap;
			#pragma pack(pop)
		} code;

		std::uint16_t messageId;
		std::vector <uint8_t> token;
		const std::uint8_t payloadMarker = 0xFF;
		std::vector <uint8_t> payload;
	};
	message_t message;

public:

	packet(const packet &) = delete;
	packet & operator=(const packet &) = delete;

	void set_message_headerInfo(std::uint8_t headerInfo)
	{
		message.headerInfo.asByte = headerInfo;
	}
	void set_message_version(std::uint8_t version)
	{
		message.headerInfo.asBitmap.version = version;
	}
	void set_message_type(std::uint8_t type)
	{
		message.headerInfo.asBitmap.type = type;
	}
	void set_message_tokenLength(std::uint8_t tokenLength)
	{
		message.headerInfo.asBitmap.tokenLength = tokenLength;
	}
	void set_message_code(std::uint8_t code)
	{
		message.code.asByte = code;
	}
	void set_message_codeClass(std::uint8_t codeClass)
	{
		message.code.asBitmap.codeClass = codeClass;
	}
	void set_message_codeDetail(std::uint8_t codeDetail)
	{
		message.code.asBitmap.codeDetail = codeDetail;
	}
	void set_message_messageId(std::uint8_t messageId)
	{
		message.messageId = messageId;
	}
	std::uint8_t get_message_headerInfo() const
	{
		return message.headerInfo.asByte;
	}		
	std::uint8_t get_message_code() const
	{
		return message.code.asByte;
	}
	std::uint16_t get_message_messageId() const
	{
		return message.messageId;
	}
	std::vector <uint8_t> get_message_token() const
	{
		return message.token;
	}
	std::vector <uint8_t> get_message_payload() const
	{
		return message.payload;
	}
	static packet & createInstance();
	static bool clearInstance(packet &);
};

#define new_packet() packet::createInstance() 
#define delete_packet(instance) packet::clearInstance(instance) 

}//coap

#endif