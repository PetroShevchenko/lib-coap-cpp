#include "log.h"
#include "uriServerEndpoint.h"

using namespace coap;
LOG_USING_NAMESPACE
LOG_GLOBAL_DECLARE

int main()
{
	LOG_CREATE(ALL, std::clog);

	auto test_func = [&]()->bool
	{
		LOG(DEBUGGING,"It is test_func");
		return true;
	};

	uriServerEndpoint::callback_t callbacks [METHODS_COUNT] = {
		test_func,
		test_func,
		test_func,
		test_func
	};

	std::string attributes[METHODS_COUNT] = {
		"CI=10",
		"CI=20",
		"CI=30",
		"CI=40"
	};

	try {
		uriServerEndpoint ep1("/first/second/third/forth",callbacks, attributes);
		LOG(DEBUGGING, ep1.get_uri());
	}
	catch(...)
	{
		LOG(DEBUGGING,"It was cought an exception");
		LOG_DELETE;
		exit(1);
	}


	LOG_DELETE;
	return 0;
}
