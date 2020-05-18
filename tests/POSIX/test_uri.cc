#include "uri.h"
#include "log.h"
#include "error.h"

using namespace coap;
LOG_USING_NAMESPACE
LOG_GLOBAL_DECLARE

int main()
{
	LOG_CREATE(ALL, std::clog);

	try {
		//uri urin("/0/1/2/3");
		uri::uri_t Uri;

		Uri.type = uri::URI_TYPE_INTEGER;
		Uri.asInteger = {0, 10, 20, 30};
		//, asInteger = array };
		uri urin(Uri);
		//uri urin("/first/second/third/forth");
		//uri urin("bla-bla-bla");
		//uri urin("");
		LOG(DEBUGGING, urin);
	}
	catch(error * er) {
		LOG(ERROR, er->get_message());
		exit(1);
	}

	LOG_DELETE;
	return 0;
}