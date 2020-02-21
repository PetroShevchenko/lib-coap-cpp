#include "connection.h"

namespace coap {

bool connection::checkNumberSystem (std::size_t start_index, std::size_t end_index,
									std::string &number_string, const std::string &pattern)
{
	bool result;
	for(size_t i = start_index; i <= end_index; i++)
	{
		result = false;
		for(auto sym : pattern)
		{
			if (sym == number_string[i]) {
				result = true;
				break;
			}
		}
		if(!result) return result;
	}
	return result;
}

bool connection::isIPv4Address(std::string address)
{
	std::size_t position = 0, prev_pos = 0;
	std::size_t iterations = 3;
	auto is_dec = [&](std::size_t start_index, std::size_t end_index)->bool
	{
		return checkNumberSystem(start_index, end_index, address, "0123456789");
	};
	if (address.size() > 15) return false;
	while (iterations--)
	{
		position = address.find(".", prev_pos);
		if (position == std::string::npos ||
			position == 0 ||
			position > (prev_pos + 3) ||
			!is_dec(prev_pos, position - 1)) return false;
		prev_pos = ++position;
	}
	if (address.size() - prev_pos > 3 ||
		std::string::npos != address.find(".", prev_pos ) ||
		!is_dec(prev_pos, address.size() - 1)) return false;
	return true;
}

bool connection::isIPv6Address(std::string address)
{
	std::size_t position = 0, prev_pos = 0;
	std::size_t colons = 0, double_colons = 0;
	auto is_hex = [&](std::size_t start_index, std::size_t end_index)->bool
	{
		return checkNumberSystem(start_index, end_index, address, "0123456789aAbBcCdDeEfF");
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