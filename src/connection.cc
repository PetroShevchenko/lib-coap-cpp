#include "connection.h"

namespace coap {

bool connection::isIPv4Address(std::string address)
{
	std::size_t position = 0, prev_pos = 0;
	std::size_t iterations = 3;
	auto is_dec = [&](std::size_t start_index, std::size_t end_index)->bool
	{
		bool result;
		const std::string pattern = "0123456789";
		for(size_t i = start_index; i <= end_index; i++)
		{
			result = false;
			for(auto sym : pattern)
			{
				if (sym == address[i]) {
					std::clog << "sym =" << sym << "\n";
					result = true;
					break;
				}
			}
			if(!result) return result;
		}
		return result;
	};
	if (address.size() > 15) return false;
	while (iterations--)
	{
		position = address.find(".", position ? prev_pos = position + 1 : prev_pos = position);
		if (position == std::string::npos ||
			position == 0 ||
			position > (prev_pos + 3) ||
			!is_dec(prev_pos, position - 1)) return false;
	}
	if (address.size() - position > 4 ||
		std::string::npos != address.find(".", position + 1 ) ||
		!is_dec(position, address.size() - 1)) return false;
	return true;
}

bool connection::isIPv6Address(std::string address)
{
	std::size_t position = 0, prev_pos = 0;
	std::size_t colons = 0, double_colons = 0;

	auto is_hex = [&](std::size_t start_index, std::size_t end_index)->bool
	{
		bool result;
		const std::string pattern = "0123456789aAbBcCdDeEfF";
		for(size_t i = start_index; i <= end_index; i++)
		{
			result = false;
			for(auto sym : pattern)
			{
				if (sym == address[i]) {
					result = true;
					break;
				}
			}
			if(!result) return result;
		}
		return result;
	};

	if (address.size() > 39) return false;

	while(position < address.size())
	{
		position = address.find(":", prev_pos);
		if (position == std::string::npos) {
			if (double_colons > 1 ||
				(double_colons == 1 && colons > 5) ||
				(double_colons == 0 && colons > 7) ||
				(address.size() - prev_pos > 4) ||
				!is_hex(prev_pos, address.size() - 1))
				return false;
			else
				break;
		}
		else {
			if (position > prev_pos) {
				if ((position - prev_pos > 4) ||
					!is_hex(prev_pos, position - 1))
					return false;
			}
			position++;
			if(address[position] == ':') {
				double_colons++;
				position++;
				if (position == address.size()) break;
			}
			else {
				colons++;
				if (position == address.size())
					return false;
			}

			prev_pos = position;
		}
	}
	return true;
}

}//coap