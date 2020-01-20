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
	_option_bitmap[0] = 0;
	_option_bitmap[1] = 0;
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
	os << "option #" << static_cast<unsigned short>(option.number) <<
		" delta: " << static_cast<unsigned short>(option.header.asBitfield.delta) <<
		" length: " << static_cast<unsigned short>(option.header.asBitfield.length) <<
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
	assert(buffer != nullptr);
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
	assert(buffer != nullptr);
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
	assert(buffer != nullptr);
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
	assert(buffer != nullptr);
	assert(length >= _message.payloadOffset);
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

const packet::option_t * packet::find_options(const std::uint8_t number, size_t * quantity)
{
	int min = 0 , max = _message.options.size(), mid  = 0;
	assert(quantity != nullptr);

	*quantity = 0;

	while (min <= max)
	{
		mid = (min + max) >> 1;

		if ( _message.options[mid].number < number) {
			max = mid + 1;
		}
		else if (_message.options[mid].number > number) {
			max = mid - 1;
		}
		else {
			int index = mid;
			while (index >= min)
			{
				if (_message.options[--index].number != number) {
					min = index + 1;
					break;
				}
			}
			while (_message.options[++index].number == number)
			{
				(*quantity)++;
			}
			return &_message.options[min];
		}
	}
	return nullptr;
}

std::uint8_t packet::get_option_nibble(uint32_t value)
{
	std::uint8_t nibble = 0;
	if (value < MINUS_THIRTEEN_OPT_VALUE) {
		nibble = value & 0xFF;
	}
	else if (value <= 0xFF + MINUS_THIRTEEN_OPT_VALUE) {
		nibble = MINUS_THIRTEEN;
	}
	else if (value <= 0xFFFF + MINUS_TWO_HUNDRED_SIXTY_NINE_OPT_VALUE) {
		nibble = MINUS_TWO_HUNDRED_SIXTY_NINE;
	}
	return nibble;
}

bool packet::serialize(std::uint8_t * buffer, size_t * length, bool checkBufferSizeOnly)
{
	size_t offset = 0;
	assert(length != nullptr);

	if (!checkBufferSizeOnly) {
		assert(buffer != nullptr);
		assert(*length >= static_cast<unsigned>(PACKET_MIN_LENGTH + _message.headerInfo.asBitfield.tokenLength));
	}

	if (!checkBufferSizeOnly) {
		buffer [HEADER_OFFSET]  = _message.headerInfo.asByte;
		buffer [CODE_OFFSET] 	= _message.code.asByte;
	}
	offset = MESSAGE_ID_OFFSET;

	if (!checkBufferSizeOnly) {
		if (get_is_little_endian()) {
			buffer [offset] 	= _message.messageId & 0xFF;
			buffer [offset + 1]	= (_message.messageId >> 8 ) & 0xFF;
		}
		else {
			buffer [offset] 	= (_message.messageId >> 8 ) & 0xFF;
			buffer [offset + 1] = _message.messageId & 0xFF;
		}
	}

	offset = TOKEN_OFFSET;

	size_t tokenLength = static_cast<size_t>(_message.headerInfo.asBitfield.tokenLength);

	if (tokenLength > TOKEN_MAX_LENGTH) {
		LOG(DEBUG,"Token length is wrong");
		_error.set_code(TOKEN_LENGTH);
		return false;
	}
	if (tokenLength != 0 && !checkBufferSizeOnly) std::memcpy (buffer + offset, _message.token, tokenLength);

	offset += tokenLength;

	if (!checkBufferSizeOnly)	assert(offset < *length);

	for (auto opt : _message.options)
	{
		std::uint32_t optDelta = 0;
		std::uint8_t optNumDelta = 0;
		std::uint8_t lengthNibble = 0;
		std::uint8_t deltaNibble = 0;

		auto option_maker = [&](std::uint8_t firstParsing, std::uint8_t secondParsing)
		{
			if (firstParsing == MINUS_THIRTEEN) {
				if (checkBufferSizeOnly) offset++;
				else buffer [offset++] = secondParsing - MINUS_THIRTEEN_OPT_VALUE;
			}
			else if (firstParsing == MINUS_TWO_HUNDRED_SIXTY_NINE) {
				if (checkBufferSizeOnly) offset += 2;
				else {
					buffer [offset++] = (secondParsing - MINUS_TWO_HUNDRED_SIXTY_NINE_OPT_VALUE) >> 8;
					buffer [offset++] = (secondParsing - MINUS_TWO_HUNDRED_SIXTY_NINE_OPT_VALUE) & 0xFF;
				}
			}
			if (!checkBufferSizeOnly)	assert(offset < *length);
		};

		if (!checkBufferSizeOnly && offset > *length) {
			LOG(DEBUG, "The buffer length is too small");
			_error.set_code(BUFFER_LENGTH);
			return false;
		}

		optDelta = opt.number - optNumDelta;

		deltaNibble = get_option_nibble(optDelta);
		lengthNibble = get_option_nibble(static_cast<std::uint32_t>(opt.value.size()));

		if (!checkBufferSizeOnly) {
        	buffer [offset++] = (deltaNibble << 4 | lengthNibble) & 0xFF;
        	assert(offset < *length);
    	}
    	else offset++;

		option_maker (deltaNibble, optDelta);
		option_maker (lengthNibble, static_cast<std::uint8_t>(opt.value.size()));

		if (!checkBufferSizeOnly) std::memcpy (buffer + offset, opt.value.data(), opt.value.size());
		offset += opt.value.size();
		if (!checkBufferSizeOnly) assert(offset < *length);
		optNumDelta = opt.number;
	}

	size_t optionsLength = offset - tokenLength - PACKET_HEADER_SIZE;

	if (_message.payload.size() == 0) {
		*length = offset;
	}
	else {
		if (!checkBufferSizeOnly && *length < PACKET_HEADER_SIZE + tokenLength + optionsLength
										 + sizeof(_message.payloadMarker) + _message.payload.size()) {
			LOG(DEBUG, "The buffer length is too small");
			_error.set_code(BUFFER_LENGTH);
			return false;
		}
		offset = PACKET_HEADER_SIZE + tokenLength + optionsLength;
		if (!checkBufferSizeOnly) {
			assert(offset < *length);
			buffer [offset] = _message.payloadMarker;
		}
		offset += sizeof(_message.payloadMarker);

		if (!checkBufferSizeOnly) {
			assert(offset < *length);
			assert(offset + _message.payload.size() <= *length);
			std::memcpy (buffer + offset, _message.payload.data(), _message.payload.size());
		}
		*length = offset + _message.payload.size();
	}
	return true;
}

void packet::prepare_answer(message_type_t messageType,
						std::uint16_t messageId,
						message_code_t responseCode,
						const std::uint8_t * payload,
						const size_t payloadLength,
						media_type_t payloadType)
{
	assert(payload != nullptr);
	set_message_version(COAP_VERSION);
	set_message_type(static_cast<std::uint8_t>(messageType));
	set_message_messageId(messageId);
	set_message_code(static_cast<std::uint8_t>(responseCode));
	_message.options.clear();
	_message.options[0].number = static_cast<std::uint8_t>(CONTENT_FORMAT);
	_message.options[0].value[0] = (static_cast<std::uint16_t>(payloadType) & 0xFF00) >> 8;
	_message.options[0].value[1] = (static_cast<std::uint16_t>(payloadType) & 0x00FF);
	std::memcpy(_message.payload.data(), payload, payloadLength);
}

}//coap
