#include "log.h"
#include "clientConnection.h"

LOG_USING_NAMESPACE


int main()
{
	LOG_CREATE(ALL, std::clog);

	auto test_address_resolving = [&](clientConnection & conn)
	{
		LOG(DEBUG, "hostname = ", conn.get_hostname());
		if (conn.hostname2IPAddress()) {
			LOG(DEBUG, "IPv6Address = ", conn.get_IPv6Address());
			LOG(DEBUG, "IPv4Address = ", conn.get_IPv4Address());
			LOG(DEBUG, "OK");
		}
		else {
			LOG(DEBUG, "FAIL");
		}
	};

	{
		clientConnection udpConn("localhost",5683);
		LOG(DEBUG, "Test 1 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("cxemotexnika.org",5683);
		LOG(DEBUG, "Test 2 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("google.com.ua",5683);
		LOG(DEBUG, "Test 3 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("192.168.0.100",5683);
		LOG(DEBUG, "Test 4 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("2001:67c:2e5c:2033:30b9:b34d:ff24:7850",5683);
		LOG(DEBUG, "Test 5 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	LOG_DELETE;
	return 0;
}