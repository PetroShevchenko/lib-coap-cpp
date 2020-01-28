#include "log.h"
#include "connection.h"

LOG_USING_NAMESPACE


int main()
{
	LOG_CREATE(ALL, std::clog);

	connection udpConn("127.0.0.1",5683);
	LOG(DEBUG, "address = ", udpConn.get_address());
	LOG(DEBUG, "port = ", udpConn.get_port());
	LOG(DEBUG, "length = ", udpConn.get_length());
	LOG(DEBUG, "buffer = ", (long)udpConn.get_buffer());
	LOG(DEBUG, "state = ", udpConn.get_state());
	LOG_DELETE;
	return 0;
}
