#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>

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

bool connection::hostname2IPAddress()
{
	struct addrinfo hints;
	struct addrinfo * servinfo = nullptr;
	char port_str[8];
	char address_str[40];
	int result;
	bool status = false;

	if (_hostname.size() == 0) return false;
	if (isIPv6Address(_hostname)) {
		_IPv6Address = _hostname;
		return true;
	}
	if (isIPv4Address(_hostname)) {
		_IPv4Address = _hostname;
		return true;
	}
	memset(address_str, 0, sizeof(address_str));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	snprintf((char *)port_str, sizeof(port_str), "%d", _port);

	result = getaddrinfo(_hostname.c_str(), port_str, &hints, &servinfo);

	if (result == 0) {
		for (struct addrinfo * p = servinfo; p != nullptr; p = p->ai_next)
		{
			inet_ntop(p->ai_family, p->ai_family == AF_INET ?
					(void *) & ((struct sockaddr_in *) p->ai_addr)->sin_addr :
					(void *) & ((struct sockaddr_in6 *) p->ai_addr)->sin6_addr, address_str, sizeof(address_str));
			if (p->ai_family == AF_INET6) {
				_IPv6Address = address_str;
				status = true;
			}
			else if (p->ai_family == AF_INET) {
				_IPv4Address = address_str;
				status = true;
			}
		}
	}
	else status = false;

	if (servinfo != nullptr) {
		freeaddrinfo(servinfo);
	}
	return status;
}

}//coap