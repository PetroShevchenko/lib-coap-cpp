#ifndef LOG_H
#define LOG_H

#define LOG_ENABLE
#define LOG_PRINT_TIMESTAMP
#define LOG_PRINT_LEVEL
#define LOG_PRINT_FUNCTION_NAME
#define LOG_PRINT_LINE_OF_CODE

#ifndef LOG_ENABLE
#define LOG_USING_NAMESPACE
#define LOG_INIT(loggingLevels, outputStream)
#define LOG(level, ...)
#define LOG_SET_STREAM(stream)
#define LOG_SET_LEVEL(level)
#define LOG_UNSET_LEVEL(level)
#define LOG_GET_LEVELS(loggingLevels)
#define LOG_SET_STREAM_FORMAT(flags, mask)
#else
#include <iostream>
#include <ios>
#include <cstdint>
#include <string>
#include <vector>
#include <cassert>
#ifdef LOG_PRINT_TIMESTAMP
#include <chrono>
#include <ctime>
#include <iomanip>
#endif
#define LOG_USING_NAMESPACE using namespace coap;
#define LOG_INIT(loggingLevels, outputStream) coap::log & instanceLink = coap::log::createInstance(loggingLevels, outputStream)
#define LOG(level, ...) {instanceLink.print(level, __FILE__, __func__, __LINE__, __VA_ARGS__ );}
#define LOG_SET_STREAM(stream) {instanceLink.set_stream(stream);}
#define LOG_SET_LEVEL(level) {instanceLink.set_level(level);}
#define LOG_UNSET_LEVEL(level) {instanceLink.unset_level(level);}
#define LOG_GET_LEVELS(loggingLevels) {loggingLevels = instanceLink.get_loggingLevels();}
#define LOG_SET_STREAM_FORMAT(flags, mask) {instanceLink.set_stream_format(flags, mask);}

/*
#define LOG_PRINT_TIMESTAMP
#define LOG_PRINT_DATE
#define LOG_PRINT_LEVEL
#define LOG_PRINT_FILE_NAME
#define LOG_PRINT_FUNCTION_NAME
#define LOG_PRINT_LINE_OF_CODE
*/

namespace coap {

using level_t = enum {
	NONE = 0,
	INFO,
	DEBUG,
	WARNING,
	ERROR,
	CRITICAL,
	ALL = (1 << INFO)|(1 << DEBUG)|(1 << WARNING)|(1 << ERROR)|(1 << CRITICAL)
};


/**
*/
class log {

private:

	bool is_printable_level(level_t level)
	{
		return level > NONE && level <= CRITICAL;  
	}

	const std::vector<std::string> printableLevel = {
		"",
		"INFO",
		"DEBUG",
		"WARNING",
		"ERROR",
		"CRITICAL"
	};

	std::uint8_t _loggingLevels;
	std::ostream * _stream;
	bool _print_header;

	log(std::uint8_t loggingLevels, std::ostream &outputStream)
	{
		if (NONE == loggingLevels || ALL == loggingLevels)
			_loggingLevels = loggingLevels;
		else _loggingLevels = ALL;
		_stream = &outputStream;
		_print_header = true;
	}

	bool print_header(level_t level, const char * file, const char * func, int line)
	{
		assert(is_printable_level(level));
		if (((1 << level) & _loggingLevels) == 0) return false;		
#ifdef LOG_PRINT_LEVEL		
		*_stream << "[" << printableLevel[level] << "] ";
#endif
#ifdef LOG_PRINT_TIMESTAMP
		std::chrono::time_point<std::chrono::system_clock> now;
		now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
#ifdef LOG_PRINT_DATE		
		*_stream << std::put_time(std::localtime(&now_c), "%F %T"); 
#else
		*_stream << std::put_time(std::localtime(&now_c), "%T");
#endif
#endif		
		*_stream << " >| " << "[";
#ifdef LOG_PRINT_FILE_NAME	
		*_stream << file;
		*_stream << ":";
#endif
#ifdef LOG_PRINT_FUNCTION_NAME	
		*_stream  << func;
		*_stream << ":";
#endif
#ifdef LOG_PRINT_LINE_OF_CODE
		*_stream << line ;
#endif
		*_stream	<< "] ";
		return true;	 
	}

public:
	log() = delete;
	log(const log &) = delete;
	log & operator=(const log &) = delete;

	static log& createInstance(std::uint8_t loggingLevels, std::ostream &outputStream)
	{
		static log instance(loggingLevels, outputStream);
		return instance;
	}

	void set_stream(std::ostream & stream)
	{
		log::_stream = &stream;
	}

	void set_stream_format(std::ios_base::fmtflags flags, std::ios_base::fmtflags mask)
	{
		 _stream->setf(flags, mask);
	}

	void set_level(level_t level)
	{
		assert(level <= CRITICAL && level >= NONE || level == ALL);
		if (ALL == level || NONE == level)
		{		
			_loggingLevels = level;
		}
		else _loggingLevels |= (1 << level);
	}

	void unset_level(level_t level)
	{
		assert(level <= CRITICAL && level >= NONE || level == ALL);
		if (NONE == level)
			_loggingLevels = ALL;
		else if (ALL == level)
			_loggingLevels = NONE; 
		else _loggingLevels &= ~(1 << level);
	}

	std::uint8_t get_loggingLevels() const
	{
		return _loggingLevels;
	}
	
	template <typename T>
	void print(level_t level, const char * file, const char * func, int line, T t) 
	{
		if(_print_header) {
			if (!print_header(level, file, func, line)) return;
		}
		else {
			_print_header = true;
		}
    	*_stream << t << std::endl;
    	_stream->flush();
	}

	template <typename T, typename... Args>
	void print(level_t level, const char * file, const char * func, int line, T t, Args... args)
	{
		if(_print_header) {
			if (!print_header(level, file, func, line)) return;
			_print_header = false;
		}
		*_stream << t;
		print(level, file, func, line, args...);
	}
};//log
} //coap
#endif//LOG_ENABLE
#endif//LOG_H
