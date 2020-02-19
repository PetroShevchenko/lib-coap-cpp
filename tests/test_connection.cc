#include "log.h"
#include "connection.h"

LOG_USING_NAMESPACE


int main()
{
	LOG_CREATE(ALL, std::clog);

	connection udpConn("localhost",5683);
	LOG(DEBUG, "hostname = ", udpConn.get_hostname());
	LOG(DEBUG, "port = ", udpConn.get_port());
	LOG(DEBUG, "length = ", udpConn.get_length());
	LOG(DEBUG, "buffer = ", (long)udpConn.get_buffer());
	LOG(DEBUG, "state = ", udpConn.get_state());

	LOG(DEBUG, "Test 1 for IPv4 0.0.0.0 : ", udpConn.isIPv4Address("0.0.0.0"));
	LOG(DEBUG, "Test 2 for IPv4 127.0.0.1 : ", udpConn.isIPv4Address("127.0.0.1"));
	LOG(DEBUG, "Test 3 for IPv4 192.168.0.1 : ", udpConn.isIPv4Address("192.168.0.1"));
	LOG(DEBUG, "Test 4 for IPv4 10.10.254.254 : ", udpConn.isIPv4Address("10.10.254.254"));
	LOG(DEBUG, "Test 5 for IPv4 .100.254.254 : ", udpConn.isIPv4Address(".100.254.254"));
	LOG(DEBUG, "Test 6 for IPv4 0.1000.254.254 : ", udpConn.isIPv4Address("0.1000.254.254"));
	LOG(DEBUG, "Test 7 for IPv4 111.111.25.25.98 : ", udpConn.isIPv4Address("111.111.25.25.98"));
	LOG(DEBUG, "Test 8 for IPv4 0.0.0.0.0 : ", udpConn.isIPv4Address("0.0.0.0.0"));
	LOG(DEBUG, "Test 9 for IPv4 1.1.1.1000 : ", udpConn.isIPv4Address("1.1.1.1000"));
	LOG(DEBUG, "Test 10 for IPv4 1.1.1.10a : ", udpConn.isIPv4Address("1.1.1.10a"));


	LOG(DEBUG, "Test 1 for IPv6 [::78a] : ", udpConn.isIPv6Address("::78a"));
	LOG(DEBUG, "Test 2 for IPv6 [::78g] : ", udpConn.isIPv6Address("::78g"));
	LOG(DEBUG, "Test 3 for IPv6 [::89888:9000:ffff] : ", udpConn.isIPv6Address("::89888:9000:ffff"));
	LOG(DEBUG, "Test 4 for IPv6 [78::89:9000:ffff] : ", udpConn.isIPv6Address("78::89:9000:ffff"));
	LOG(DEBUG, "Test 5 for IPv6 [78::89:9000:fffff] : ", udpConn.isIPv6Address("78::89:9000:fffff"));
	LOG(DEBUG, "Test 6 for IPv6 [7899::89:9000:ffff:66:77:77:] : ", udpConn.isIPv6Address("7899::89:9000:ffff:66:77:77:"));
	LOG(DEBUG, "Test 7 for IPv6 [7899::89:9000:ffff:66:77:77:0000] : ", udpConn.isIPv6Address("7899::89:9000:ffff:66:77:77:0000"));
	LOG(DEBUG, "Test 8 for IPv6 [7899::89:9000:ffff::66:77] : ", udpConn.isIPv6Address("7899::89:9000:ffff::66:77"));
	LOG(DEBUG, "Test 9 for IPv6 [7899::] : ", udpConn.isIPv6Address("7899::"));

	LOG_DELETE;
	return 0;
}