#include "uri.h"
#include "log.h"
#include <cstring>
#include <cstdlib>

LOG_USING_NAMESPACE
LOG_EXTERN_DECLARE

namespace coap {

error uri::_error;

uri::uri(std::string path): _path(std::move(path)), _uri()
{
	if (!path_to_uri()) {
		_error.set_code(WRONG_URI_PATH);
		throw &_error;
	}
}

uri::uri(uri::uri_t URI): _path(""), _uri(std::move(URI))
{
	uri_to_path();
}

bool uri::path_to_uri()
{
	LOG(DEBUGGING,"Entering");
	char * token = nullptr;

	token = new char [_path.size() + 1];

	strncpy(token, _path.c_str(), _path.size() + 1);

	token = strtok(token, "/");

	if (token == nullptr) {
		LOG(ERROR,"Wrong value of variable _patch");
		return false;
	}

	if (((token[0] & 0xF0)^0x30) == 0) {
		_uri.type = URI_TYPE_INTEGER;
	}
	else {
		_uri.type = URI_TYPE_STRING;
	}

	do {
		LOG(DEBUGGING,"token =", token);
		if(_uri.type == URI_TYPE_STRING) {
			_uri.asString.push_back(token);
		}
		else if (_uri.type == URI_TYPE_INTEGER){
			_uri.asInteger.push_back(strtol (token, NULL, 10));
		}
		token = strtok(NULL, "/");
	} while (token != NULL);

	delete [] token;
	LOG(DEBUGGING,"Leaving");
	return true;
}

void uri::uri_to_path()
{
	if (_uri.type == URI_TYPE_STRING) {
		for (size_t i = 0; i < _uri.asString.size(); i++)
		{
			_path += "/";
			_path += _uri.asString[i];
		}
	}
	else if (_uri.type == URI_TYPE_INTEGER) {
		for (size_t i =0; i < _uri.asInteger.size(); i++)
		{
			_path += "/";
			_path += std::to_string(_uri.asInteger[i]);
		}
	}
}

std::ostream & operator<<(std::ostream & os,const uri::uri_t &uri)
{
	os << " type : " << uri.type <<
		  " segments : ";

	if (uri.type == uri::URI_TYPE_STRING) {
		for(size_t i = 0; i < uri.asString.size(); i++)
		{
			os << uri.asString[i] << " ";
		}
	}
	else {
		for(size_t i = 0; i < uri.asInteger.size(); i++)
		{
			os << uri.asInteger[i] << " ";
		}
	}
	return os;
}

std::ostream & operator<<(std::ostream & os,const uri &object)
{
	os << " path : " << object._path << " uri : " << object._uri;
	return os;
}


}//coap
