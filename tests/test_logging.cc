#include "log.h"
#include <fstream>

LOG_USING_NAMESPACE


int main()
{
	std::fstream file("default.log", std::ios::out|std::ios::app);
	std::uint8_t loggingLevels;
	//LOG_INIT((1 << ERROR), std::clog);
	LOG_INIT(NONE, std::clog);
	LOG_SET_STREAM(file);
	//LOG_UNSET_LEVEL(INFO);
	LOG_SET_LEVEL(DEBUG);
	LOG_SET_LEVEL(ERROR);
	LOG_GET_LEVELS(loggingLevels);
	LOG(INFO, 10000, "\t",3.14, " test");
	LOG(DEBUG, 10000, "\t",3.14, " test");
	LOG(WARNING, "it is warning");
	LOG(ERROR, " it is error");
	LOG_SET_STREAM_FORMAT(std::ios::hex, std::ios::basefield);
	LOG(DEBUG,"logging levels: 0x", static_cast<unsigned short>(loggingLevels));
	file.close();

	return 0;
}

