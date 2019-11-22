#include "log.h"
#include "endpoints.h"

LOG_USING_NAMESPACE


int main()
{
	LOG_CREATE(ALL, std::clog);

	auto test_func = [&]()->bool
	{
		LOG(DEBUG,"It is test_func");
	};

	uri_t uri = {
		URI_TYPE_STRING,
		4,
		"first",
		"second",
		"third",
		"forth"
	};

	uri_t uri1, uri2;

	endpoints::string_to_uri("/0/1/2/3", &uri1);
	endpoints::string_to_uri("/10/11/12/13", &uri2);

	endpoints::handler_t handlers[METHODS_COUNT] = {
		test_func,
		test_func,
		test_func,
		test_func
	};

	const char * attributes[METHODS_COUNT] = {
		"CI=10",
		"CI=20",
		"CI=30",
		"CI=40"
	};

	endpoints::endpoint_t ep1, ep2;

	if (!endpoints::fill_endpoint (&uri1, handlers, attributes, &ep1)) {
		LOG(DEBUG, "");
		exit(1);
	}
	if (!endpoints::fill_endpoint (&uri2, handlers, attributes, &ep2)) {
		LOG(DEBUG, "");
		exit(1);
	}


#if 0
	endpoints::endpoint_t ep = {
		&uri,
		{
		{	METHOD_GET,
			test_func,
			"CI=10"	},
		{	METHOD_POST,
			test_func,
			"CI=20"	},
		{	METHOD_PUT,
			test_func,
			"CI=30"	},
		{	METHOD_DELETE,
			test_func,
			"CI=40"	}
		}
	};
#endif

	try {

		static endpoints & instance = endpoints::createInstance();

		instance.add(ep1);
		instance.add(ep2);
		LOG(DEBUG, instance.get_context());
	}
	catch(...)
	{
		LOG(DEBUG,"It was cought an exception");
		LOG_DELETE;
		exit(1);
	}


	LOG_DELETE;
	return 0;
}
