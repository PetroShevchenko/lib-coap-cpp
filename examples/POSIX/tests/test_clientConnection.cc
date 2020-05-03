#include "coapcpp.h"
#include "clientConnection.h"
#include "log.h"

using namespace coap;
LOG_USING_NAMESPACE
LOG_GLOBAL_DECLARE

int main()
{
	LOG_CREATE(ALL, std::clog);

	auto test_address_resolving = [&](clientConnection & conn)
	{
		LOG(DEBUGGING, "hostname = ", conn.get_hostname());
		if (conn.hostname2IPAddress()) {
			LOG(DEBUGGING, "IPv6Address = ", conn.get_IPv6Address());
			LOG(DEBUGGING, "IPv4Address = ", conn.get_IPv4Address());
			LOG(DEBUGGING, "OK");
		}
		else {
			LOG(DEBUGGING, "FAIL");
		}
	};

	{
		clientConnection udpConn("localhost",5683);
		LOG(DEBUGGING, "Test 1 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("cxemotexnika.org",5683);
		LOG(DEBUGGING, "Test 2 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("google.com.ua",5683);
		LOG(DEBUGGING, "Test 3 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("192.168.0.100",5683);
		LOG(DEBUGGING, "Test 4 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("2001:67c:2e5c:2033:30b9:b34d:ff24:7850",5683);
		LOG(DEBUGGING, "Test 5 for IP address resolving :");
		test_address_resolving(udpConn);
	}
	{
		clientConnection udpConn("localhost",5683);

		LOG(DEBUGGING, "hostname = ", udpConn.get_hostname());
		LOG(DEBUGGING, "port = ", udpConn.get_port());
		LOG(DEBUGGING, "length = ", udpConn.get_length());
		LOG(DEBUGGING, "buffer = ", (long)udpConn.get_buffer());
		LOG(DEBUGGING, "state = ", udpConn.get_state());

		LOG(DEBUGGING, "Test 1 for IPv4 0.0.0.0 : ", udpConn.isIPv4Address("0.0.0.0"));
		LOG(DEBUGGING, "Test 2 for IPv4 127.0.0.1 : ", udpConn.isIPv4Address("127.0.0.1"));
		LOG(DEBUGGING, "Test 3 for IPv4 192.168.0.1 : ", udpConn.isIPv4Address("192.168.0.1"));
		LOG(DEBUGGING, "Test 4 for IPv4 10.10.254.254 : ", udpConn.isIPv4Address("10.10.254.254"));
		LOG(DEBUGGING, "Test 5 for IPv4 .100.254.254 : ", udpConn.isIPv4Address(".100.254.254"));
		LOG(DEBUGGING, "Test 6 for IPv4 0.1000.254.254 : ", udpConn.isIPv4Address("0.1000.254.254"));
		LOG(DEBUGGING, "Test 7 for IPv4 111.111.25.25.98 : ", udpConn.isIPv4Address("111.111.25.25.98"));
		LOG(DEBUGGING, "Test 8 for IPv4 0.0.0.0.0 : ", udpConn.isIPv4Address("0.0.0.0.0"));
		LOG(DEBUGGING, "Test 9 for IPv4 1.1.1.1000 : ", udpConn.isIPv4Address("1.1.1.1000"));
		LOG(DEBUGGING, "Test 10 for IPv4 1.1.1.10a : ", udpConn.isIPv4Address("1.1.1.10a"));


		LOG(DEBUGGING, "Test 1 for IPv6 [::78a] : ", udpConn.isIPv6Address("::78a"));
		LOG(DEBUGGING, "Test 2 for IPv6 [::78g] : ", udpConn.isIPv6Address("::78g"));
		LOG(DEBUGGING, "Test 3 for IPv6 [::89888:9000:ffff] : ", udpConn.isIPv6Address("::89888:9000:ffff"));
		LOG(DEBUGGING, "Test 4 for IPv6 [78::89:9000:ffff] : ", udpConn.isIPv6Address("78::89:9000:ffff"));
		LOG(DEBUGGING, "Test 5 for IPv6 [78::89:9000:fffff] : ", udpConn.isIPv6Address("78::89:9000:fffff"));
		LOG(DEBUGGING, "Test 6 for IPv6 [7899::89:9000:ffff:66:77:77:] : ", udpConn.isIPv6Address("7899::89:9000:ffff:66:77:77:"));
		LOG(DEBUGGING, "Test 7 for IPv6 [7899::89:9000:ffff:66:77:77:0000] : ", udpConn.isIPv6Address("7899::89:9000:ffff:66:77:77:0000"));
		LOG(DEBUGGING, "Test 8 for IPv6 [7899::89:9000:ffff::66:77] : ", udpConn.isIPv6Address("7899::89:9000:ffff::66:77"));
		LOG(DEBUGGING, "Test 9 for IPv6 [7899::] : ", udpConn.isIPv6Address("7899::"));
	}
	LOG_DELETE;
	return 0;
}