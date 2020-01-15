#ifndef TLV_H
#define TLV_H

namespace lwm2m {

class tlv {
private:
	using type_t = 
	union type_u {
		std::uint8_t asByte;
		#pragma pack(push,1)
		struct {
			std::uint8_t length : 3;
			std::uint8_t lengthType : 2;
			std::uint8_t identifierLength : 1
			std::uint8_t identifierType : 2;
		} asBitfield;
		#pragma pack(pop)
	};
	
	type_t _type;
	std::uint16_t _identifier;
	std::uint32_t _length;
	std::vector _value;

public:
	tlv(){}
	~tlv(){}
	tlv(const tlv &) = delete;
	tlv & operator=(const tlv &) = delete;
};

}//lwm2m

#endif


  