#include "log.h"
#include <fstream>

LOG_USING_NAMESPACE
LOG_GLOBAL_DECLARE

int main()
{
	std::fstream file("default.log", std::ios::out|std::ios::app);
	std::uint8_t loggingLevels;
	//LOG_INIT((1 << ERROR), std::clog);
	LOG_CREATE(NONE, file);
	//LOG_UNSET_LEVEL(INFO);
	LOG_SET_LEVEL(DEBUGGING);
	LOG_SET_LEVEL(ERROR);
	LOG_GET_LEVELS(loggingLevels);
	int a = 10000;
	double b = 3.14;
	LOG(INFO, a, "\t", b, " test");
	LOG(DEBUGGING, a, "\t", b, " test");
	LOG(WARNING, "it is warning");
	LOG(ERROR, " it is error");
	//LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
	LOG(DEBUGGING,"logging levels: 0x", static_cast<unsigned short>(loggingLevels));
	file.close();
	LOG_DELETE;

	return 0;
}

