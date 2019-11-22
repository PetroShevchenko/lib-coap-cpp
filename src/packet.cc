#include "packet.h"
#include "log.h"
#include "error.h"
#include <cstring>

namespace coap{

packet * packet::_instanceP = 0;
packetDestroyer packet::_destroyer;
error packet::_error;

packet & packet::createInstance()
{
	if (!_instanceP) {
		_instanceP = new packet();
		_destroyer.initialize(_instanceP);
	}
	return *_instanceP;
}

bool packet::clearInstance(packet & instance)
{
	if (&instance != _instanceP) {
		return false;
	}
	//TODO: clear all fields of instance
	_instanceP->_message.headerInfo.asByte = 0;
	_instanceP->_message.code.asByte = 0;
	_instanceP->_message.messageId = 0;
	std::memset(_instanceP->_message.token, 0, TOKEN_MAX_LENGTH);
	_instanceP->_message.payload.clear();
	return true;
}

packet::packet()
{
	LOG_SET_LEVEL(ALL);
	LOG(DEBUG,"Entering");

	_message.headerInfo.asByte = 0;
	_message.code.asByte = 0;
	_message.messageId = 0;
	_is_little_endian = is_little_endian_byte_order();
	LOG(INFO,"Arhitecture of the CPU is (1 - Little Endian, 0 - Big Endian): ", _is_little_endian);
	LOG(DEBUG,"Leaving");
}

packet::~packet()
{
	LOG_DELETE;
}

packetDestroyer::~packetDestroyer()
{
	delete packetDestroyer::_instanceP;
}
void packetDestroyer::initialize(packet * p)
{
	packetDestroyer::_instanceP = p;
}

std::ostream & operator<<(std::ostream & os,const packet::option_t &option)
{
	static unsigned short optionsCounter = 0;
	os << "option #" << ++optionsCounter <<
		" delta: " << static_cast<unsigned short>(option.header.asBitfield.delta) <<
		" length: " << static_cast<unsigned short>(option.header.asBitfield.length) <<
		" number: " << static_cast<unsigned short>(option.number) <<
		" value: ";
	for(auto v:option.value)
		os << static_cast<int>(v) << " ";
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet::header_t &header)
{
	os << 	" version: " << static_cast<unsigned short>(header.asBitfield.version) <<
		" type: " << static_cast<unsigned short>(header.asBitfield.type) <<
		" tokenLength: " << static_cast<unsigned short>(header.asBitfield.tokenLength)
		;
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet::code_t &code)
{
	os << 	" codeClass: " << static_cast<unsigned short>(code.asBitfield.codeClass) <<
		" codeDetail: " << static_cast<unsigned short>(code.asBitfield.codeDetail)
		;
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet::message_t &message)
{
	os << message.headerInfo << message.code <<
		" messageId: " << message.messageId <<
		" token: ";

	for(auto i : message.token)
		os << static_cast<int>(i) << " ";

	os <<
		" options: " << message.options <<
		" \npayloadOffset: " << message.payloadOffset <<
		" \npayload: \n";

	for(auto i : message.payload)
		os << static_cast<int>(i) << " ";
	os <<
		" \npayload length: " << message.payload.size();
	return os;
}

std::ostream & operator<<(std::ostream & os,const packet &object)
{
	os << object._message;
	return os;
}

bool packet::parse_header(const std::uint8_t * buffer, const size_t length)
{
	LOG(DEBUG, "Entering");
	if (length < PACKET_MIN_LENGTH) {
		_error.set_code(WRONG_ARGUMENT);
		LOG(ERROR, "Length of buffer is too short, must be at least ", PACKET_MIN_LENGTH, " bytes, but equals ", length);
		return false;
	}
	set_message_headerInfo(buffer[0]);
	LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
	LOG(DEBUG,"buffer[0]=",static_cast<int>(buffer[0]));
	LOG(DEBUG,"get_message_version() =",static_cast<int>(get_message_version()));
	LOG(DEBUG,"get_message_type() =",static_cast<int>(get_message_type()));
	LOG(DEBUG,"get_message_tokenLength() =",static_cast<int>(get_message_tokenLength()));

	if (COAP_VERSION != get_message_version()) {
		_error.set_code(PROTOCOL_VERSION);
		LOG(ERROR, _error.get_message());
		return false;
	}
	set_message_code(buffer[1]);
	if (get_is_little_endian()) {
		set_message_messageId(static_cast<std::uint16_t>(buffer[2] | (buffer[3] << 8)));
	}
	else {
		set_message_messageId(static_cast<std::uint16_t>(buffer[3] | (buffer[2] << 8)));
	}
	LOG(DEBUG, "Leaving");
	return true;
}

bool packet::parse_token(const std::uint8_t * buffer, const size_t length)
{
	std::memset(_message.token, 0, TOKEN_MAX_LENGTH);
	if (0 == get_message_tokenLength()) {
		LOG(DEBUG,"Token is not presented into the packet");
		return true;
	}
	if (get_message_tokenLength() > TOKEN_MAX_LENGTH ||
		length < PACKET_HEADER_SIZE + static_cast<size_t>(get_message_tokenLength())) {
		_error.set_code(TOKEN_LENGTH);
		LOG(ERROR, "Wrong the token lenght, that equals ", get_message_tokenLength(), "(0...8), and a packet length is ", length);
		return false;
	}
	std::memcpy(_message.token, static_cast<const void *>(buffer + PACKET_HEADER_SIZE), get_message_tokenLength());
	return true;
}

bool packet::parse_options(const std::uint8_t * buffer, const size_t length)
{
	const std::uint8_t * startOptions = buffer + PACKET_HEADER_SIZE + _message.headerInfo.asBitfield.tokenLength;
	const size_t optionsOffset = static_cast<size_t>(startOptions - buffer);
	size_t i = 0;
	option_t opt = {0};
	uint16_t optDelta = 0;
	uint16_t optLength = 0;
	LOG(DEBUG,"optionsOffset = ", optionsOffset);

	for (i = optionsOffset; buffer[i] != _message.payloadMarker && i < length; i += optLength)
	{
		opt.header.asByte = buffer[i];
		optDelta = opt.header.asBitfield.delta ;
		optLength = opt.header.asBitfield.length;

		LOG(DEBUG,"opt.header.asBitfield.delta = ", static_cast<int>(opt.header.asBitfield.delta));
		LOG(DEBUG,"opt.header.asBitfield.length = ", static_cast<int>(opt.header.asBitfield.length));

		auto option_parser = [&](std::uint8_t parsing, std::uint16_t * modifying )->bool
		{
			if (parsing == MINUS_THIRTEEN) {
				if (i + 1 > length) return false;
				*modifying = buffer[i + 1] + MINUS_THIRTEEN_OPT_VALUE;
				i += sizeof(std::uint8_t);
			}
			else if (parsing == MINUS_TWO_HUNDRED_SIXTY_NINE) {
				if (i + 2 > length) return false;
				if (get_is_little_endian())
				{
					*modifying = buffer[i + 1] | (buffer[i + 2] << 8);
				}
				else {
					*modifying = buffer[i + 2] | (buffer[i + 1] << 8);
				}
				*modifying += MINUS_TWO_HUNDRED_SIXTY_NINE_OPT_VALUE;
				i += sizeof(std::uint16_t);
			}
			else if (parsing == RESERVED_FOR_FUTURE) return false;
			return true;
		};

		if ( !option_parser (opt.header.asBitfield.delta, &optDelta) ) {
			_error.set_code(WRONG_OPTION_DELTA);
			LOG(ERROR, _error.get_message());
			return false;
		}

		if ( !option_parser (opt.header.asBitfield.length, &optLength) ) {
			_error.set_code(WRONG_OPTION_LENGTH);
			LOG(ERROR, _error.get_message());
			return false;
		}

		i++;

		if ((buffer + i + optLength) > (buffer + length)) {
			_error.set_code(WRONG_OPTION_LENGTH);
			LOG(ERROR,"Length of option is too long");
			return false;
		}

		opt.number = optDelta;

		LOG(DEBUG,"optLength = ", optLength);
		for (int j = 0; j < optLength; j++)
		{
			LOG(DEBUG,"buffer[i + j] = ", static_cast<int>(buffer[i + j]));
			opt.value.push_back( buffer[i + j] );
		}

		_message.options.push_back(opt);
		opt = {0};
	}
	_message.payloadOffset = i + sizeof(_message.payloadMarker);
	return true;
}

bool packet::parse_payload(const std::uint8_t * buffer, const size_t length)
{
	const size_t maxPayloadOffset = length - _message.payloadOffset;
	for (size_t i = 0; i < maxPayloadOffset; i++)
		_message.payload.push_back(buffer[_message.payloadOffset + i]);
	return true;
}

bool packet::parse(const std::uint8_t * buffer, const size_t length)
{
	bool returnCode = false;
	assert(buffer != nullptr);
	assert(length != 0);
	returnCode = parse_header(buffer, length);
	if (!returnCode) return returnCode;
	returnCode = parse_token(buffer, length);
	if (!returnCode) return returnCode;
	returnCode = parse_options(buffer, length);
	if (!returnCode) return returnCode;
	returnCode = parse_payload(buffer, length);
	return returnCode;
}

}//coap
