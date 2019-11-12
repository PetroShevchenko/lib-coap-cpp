#ifndef PACKET_H
#define PACKET_H
#include <cstdint>
#include <vector>
#include <iostream>
//#include <arpa/inet.h>
#include "log.h"
#include "error.h"

namespace coap {

enum {
	COAP_VERSION = 0x1,
	PACKET_HEADER_SIZE = 4,
	PACKET_MIN_LENGTH = PACKET_HEADER_SIZE,
	TOKEN_MAX_LENGTH = 8
};

using message_type_t =
enum {
	CONFIRMABLE 	= 0x0,
	NON_CONFIRMABLE = 0x1,
	ACKNOWLEDGEMENT = 0x2,
	RESET 			= 0x3
};

using message_code_class_t =
enum {
	METHOD 			= (0x0 << 5),
	SUCCESS 		= (0x2 << 5),
	CLIENT_ERROR 	= (0x4 << 5),
	SERVER_ERROR 	= (0x5 << 5),
	SIGNALING_CODES = (0x7 << 5)
};

using message_code_t =
enum {
	EMPTY 	= METHOD | 0x0,
	GET 	= METHOD | 0x1,
	POST	= METHOD | 0x2,
	PUT		= METHOD | 0x3,
	DELETE 	= METHOD | 0x4,
	FETCH 	= METHOD | 0x5,
	PATCH 	= METHOD | 0x6,
	IPATCH 	= METHOD | 0x7,
	CREATED		= SUCCESS | 0x1,
	DELETED		= SUCCESS | 0x2,
	VALID		= SUCCESS | 0x3,
	CHANGED		= SUCCESS | 0x4,
	CONTENT		= SUCCESS | 0x5,
	CONTINUE 	= SUCCESS | 0x3F,
	BAD_REQUEST		= CLIENT_ERROR | 0x0,
	UNAUTHORIZED 	= CLIENT_ERROR | 0x1,
	BAD_OPTION 		= CLIENT_ERROR | 0x2,
	FORBIDDED		= CLIENT_ERROR | 0x3,
	NOT_FOUND		= CLIENT_ERROR | 0x4,
	METHOD_NOT_ALLOWED 	= CLIENT_ERROR | 0x5,
	NOT_ACCEPTABLE		= CLIENT_ERROR | 0x6,
	REQUEST_ENTITY_INCOMPLETE 	= CLIENT_ERROR | 0x8,
	CONFLICT 			= CLIENT_ERROR | 0x9,
	PRECONDITION_FAILED	= CLIENT_ERROR | 0xC,
	REQUEST_ENTITY_TOO_LARGE 	= CLIENT_ERROR | 0xD,
	UNSUPPORTED_CONTENT_FORMAT 	= CLIENT_ERROR | 0xF,
	INTERNAL_SERVER_ERROR		= SERVER_ERROR | 0x0,
	NOT_IMPLEMENTED				= SERVER_ERROR | 0x1,
	BAD_GATEWAY					= SERVER_ERROR | 0x2,
	SERVICE_UNAVAILABLE			= SERVER_ERROR | 0x3,
	GATEWAY_TIMEOUT				= SERVER_ERROR | 0x4,
	PROXYING_NOT_SUPPORTED		= SERVER_ERROR | 0x5,
	UNASSIGNED 	= SIGNALING_CODES  | 0x0,
	CSM		 	= SIGNALING_CODES  | 0x1,
	PING 		= SIGNALING_CODES  | 0x2,
	PONG 		= SIGNALING_CODES  | 0x3,
	RELEASE		= SIGNALING_CODES  | 0x4,
	ABORT 		= SIGNALING_CODES  | 0x5
};

using option_number_t =
enum {
	IF_MATCH 		= 0,
	URI_HOST 		= 3,
	ETAG     		= 4,
	IF_NONE_MATCH 	= 5,
	URI_PORT 		= 7,
	LOCATION_PATH 	= 8,
	URI_PATH 		= 11,
	CONTENT_FORMAT 	= 12,
	MAX_AGE 		= 14,
	URI_QUERY 		= 15,
	ACCEPT 			= 17,
	LOCATION_QUERY 	= 20,
	PROXY_URI 		= 35,
	PROXY_SCHEME 	= 39,
	SIZE_1 			= 60
};

using media_type_t =
enum {
	TEXT_PLAIN 		= 0,
	LINK_FORMAT 	= 40,
	XML 			= 41,
	OCTET_STREAM 	= 42,
	EXI 			= 47,
	JSON 			= 50
};

class packet;

class packetDestroyer {
private:
	packet * _instanceP;

public:
	~packetDestroyer();
	void initialize(packet * p);
};

class packet {

protected:
	packet();
	~packet();

	friend class packetDestroyer;

private:
	LOG_INIT(NONE);
	static packet * _instanceP;
	static packetDestroyer _destroyer;

	error * _error;
	bool _is_little_endian;

	using option_t =
	struct {
		union {
			std::uint8_t asByte;
			#pragma pack(push,1)
			struct {
				std::uint8_t delta : 4;
				std::uint8_t length : 4;
			} asBitfield;
			#pragma pack(pop)
		} header;
		std::vector<std::uint8_t> value;
	};

	using header_t =
	union {
		std::uint8_t asByte;
		#pragma pack(push,1)
		struct {
			std::uint8_t version: 2;
			std::uint8_t type: 2;
			std::uint8_t tokenLength: 4;
		} asBitfield;
		#pragma pack(pop)
	};

	using code_t =
	union {
		std::uint8_t asByte;
		#pragma pack(push,1)
		struct {
			std::uint8_t codeClass: 3;
			std::uint8_t codeDetail: 5;
		} asBitfield;
		#pragma pack(pop)
	};

	using message_t =
	struct {
		header_t headerInfo;
		code_t code;
		std::uint16_t messageId;
		std::uint8_t token[TOKEN_MAX_LENGTH];
		std::vector <option_t> options;
		const std::uint8_t payloadMarker = 0xFF;
		std::vector <uint8_t> payload;
	};
	message_t _message;

	bool parse_header(const std::uint8_t * buffer, const size_t length);
	bool parse_token(const std::uint8_t * buffer, const size_t length);
	bool parse_options(const std::uint8_t * buffer, const size_t length);
	bool parse_payload(const std::uint8_t * buffer, const size_t length);

public:

	packet(const packet &) = delete;
	packet & operator=(const packet &) = delete;

	friend std::ostream & operator<<(std::ostream & os,const option_t &option);
	friend std::ostream & operator<<(std::ostream & os,const header_t &header);
	friend std::ostream & operator<<(std::ostream & os,const code_t &code);
	friend std::ostream & operator<<(std::ostream & os,const message_t &message);
	friend std::ostream & operator<<(std::ostream & os,const packet &object);

	static bool is_little_endian_byte_order()
	{
		const uint16_t test = 0x005A;
		//test = htons(test);
		return (test & 0xFF) == 0x5A ? true : false;
	}

	bool get_is_little_endian() const
	{
		return _is_little_endian;
	}

	bool parse(const std::uint8_t * buffer, const size_t length);

	void set_message_headerInfo(std::uint8_t headerInfo)
	{
		_message.headerInfo.asByte = headerInfo;
	}
	void set_message_version(std::uint8_t version)
	{
		_message.headerInfo.asBitfield.version = version;
	}
	void set_message_type(std::uint8_t type)
	{
		_message.headerInfo.asBitfield.type = type;
	}
	void set_message_tokenLength(std::uint8_t tokenLength)
	{
		_message.headerInfo.asBitfield.tokenLength = tokenLength;
	}
	void set_message_code(std::uint8_t code)
	{
		_message.code.asByte = code;
	}
	void set_message_codeClass(std::uint8_t codeClass)
	{
		_message.code.asBitfield.codeClass = codeClass;
	}
	void set_message_codeDetail(std::uint8_t codeDetail)
	{
		_message.code.asBitfield.codeDetail = codeDetail;
	}
	void set_message_messageId(std::uint16_t messageId)
	{
		_message.messageId = messageId;
	}
	std::uint8_t get_message_headerInfo() const
	{
		return _message.headerInfo.asByte;
	}
	std::uint8_t get_message_tokenLength() const
	{
		return _message.headerInfo.asBitfield.tokenLength;
	}
	std::uint8_t get_message_version() const
	{
		return _message.headerInfo.asBitfield.version;
	}
	std::uint8_t get_message_code() const
	{
		return _message.code.asByte;
	}
	std::uint16_t get_message_messageId() const
	{
		return _message.messageId;
	}
	std::uint8_t * get_message_token()
	{
		return _message.token;
	}
	std::vector <uint8_t> get_message_payload() const
	{
		return _message.payload;
	}
	static packet & createInstance();
	static bool clearInstance(packet &);
};

#define new_packet() packet::createInstance()
#define delete_packet(instance) packet::clearInstance(instance)

}//coap

#endif