#ifndef LOG_H
#define LOG_H

#define LOG_ENABLE
#define LOG_PRINT_TIMESTAMP
#define LOG_PRINT_LEVEL
#define LOG_PRINT_FUNCTION_NAME
#define LOG_PRINT_LINE_OF_CODE

#ifndef LOG_ENABLE
#define LOG_USING_NAMESPACE
#define LOG_DECLARE
#define LOG_STATIC_DECLARE
#define LOG_GLOBAL_DECLARE
#define LOG_EXTERN_DECLARE
#define LOG_CREATE(loggingLevels, outputStream)
#define LOG_DELETE
#define LOG(level, ...)
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
#define LOG_USING_NAMESPACE using namespace logging;
#define LOG_DECLARE logging::log * instanceP;
#define LOG_STATIC_DECLARE static LOG_DECLARE
#define LOG_GLOBAL_DECLARE LOG_DECLARE
#define LOG_EXTERN_DECLARE extern LOG_DECLARE
#define LOG_CREATE(loggingLevels, outputStream) instanceP = logging::log::createInstance(loggingLevels, outputStream)
#define LOG_DELETE logging::log::deleteInstance(&instanceP)
#define LOG(level, ...) instanceP->print(level, __FILE__, __func__, __LINE__, __VA_ARGS__ )
#define LOG_SET_LEVEL(level) instanceP->set_level(level)
#define LOG_UNSET_LEVEL(level) instanceP->unset_level(level)
#define LOG_GET_LEVELS(loggingLevels) loggingLevels = instanceP->get_loggingLevels()
#define LOG_SET_STREAM_FORMAT(flags, mask) instanceP->set_stream_format(flags, mask)
#define LOG_UNUSED_PARAMETER(p) (void)(p)
/*
#define LOG_PRINT_TIMESTAMP
#define LOG_PRINT_DATE
#define LOG_PRINT_LEVEL
#define LOG_PRINT_FILE_NAME
#define LOG_PRINT_FUNCTION_NAME
#define LOG_PRINT_LINE_OF_CODE
*/


namespace logging {

using level_t = enum {
	NONE = 0,
	INFO,
	DEBUGGING,
	WARNING,
	ERROR,
	CRITICAL,
	ALL = (1 << INFO)|(1 << DEBUGGING)|(1 << WARNING)|(1 << ERROR)|(1 << CRITICAL)
};

/**
	\class log
	\brief A simple logging system
	\details This class implements the logging system
	which has five levels of logging. Each level can be disabled or
	enabled regardless of others.
*/
class log {

private:
	/**
		\brief Check if a level is printable
		\param [in] level logging level
		\return true if it is printable level
				false otherwise
	*/
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

	std::uint8_t _loggingLevels; /// a bitmap that contained all of enabled levels
	std::ostream * _stream;		/// stream to output log (for example: cout, cerr, clog , file)
	bool _print_header;			/// flag to print a log header
public:
	/**
		\brief Private constructor to create single instance
		\param [in] loggingLevels bitmap of enabled logging levels
		\param [in] outputStream  stream to output a log
	*/
	log(std::uint8_t loggingLevels, std::ostream &outputStream)
	{
		if (NONE == loggingLevels || ALL == loggingLevels)
			_loggingLevels = loggingLevels;
		else _loggingLevels = ALL;
		_stream = &outputStream;
		_print_header = true;
	}
	~log(){}
private:
	/**
		\brief Function to print head informatin on the begin of the log
		\param [in] level printable logging level
		\param [in] file name of the file in which is called a logging function
		\param [in] function  name of the function in which is called a logging function
		\param [in] line  line of a code string for logging
		\return true if header is printed
				false if logging level is not enabled, so header is not printed
	*/
	bool print_header(level_t level, const char * file, const char * func, int line)
	{
		assert(is_printable_level(level));
		if (((1 << level) & _loggingLevels) == 0) return false;
#ifdef LOG_PRINT_LEVEL
		*_stream << "[" << printableLevel[level] << "] ";
#else
		LOG_UNUSED_PARAMETER(level);
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
#else
		LOG_UNUSED_PARAMETER(file);
#endif
#ifdef LOG_PRINT_FUNCTION_NAME
		*_stream  << func;
		*_stream << ":";
#else
		LOG_UNUSED_PARAMETER(func);
#endif
#ifdef LOG_PRINT_LINE_OF_CODE
		*_stream << line ;
#else
		LOG_UNUSED_PARAMETER(line);
#endif
		*_stream	<< "] ";
		return true;
	}

public:
	log() = delete;/// default constructor is not permitted
	log(const log &) = delete; /// copy constructor is not permitted
	log & operator=(const log &) = delete; /// overloaded copy operator is not permitted
	/**
		\brief Static function to create single instance of class
		\param [in] loggingLevels bitmap of enabled logging levels
		\param [in] outputStream  stream to output a log
		\return link on a created class instance
	*/

	static log * createInstance(std::uint8_t loggingLevels, std::ostream & outputStream)
	{
		return new log(loggingLevels, outputStream);
	}

	static bool deleteInstance(log ** instanceP)
	{
		if (instanceP == nullptr || *instanceP == nullptr) return false;		
		delete *instanceP;
		*instanceP = nullptr;
		return true;
	}

	void set_stream_format(std::ios_base::fmtflags flags, std::ios_base::fmtflags mask)
	{
		 _stream->setf(flags, mask);
	}

	void set_level(level_t level)
	{
		assert((level <= CRITICAL && level >= NONE) || level == ALL);
		if (ALL == level || NONE == level)
		{
			_loggingLevels = level;
		}
		else _loggingLevels |= (1 << level);
	}

	void unset_level(level_t level)
	{
		assert((level <= CRITICAL && level >= NONE) || level == ALL);
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
	void print(level_t level, const char * file, const char * func, int line, T &t)
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
	void print(level_t level, const char * file, const char * func, int line, T &t, Args... args)
	{
		if(_print_header) {
			if (!print_header(level, file, func, line)) return;
			_print_header = false;
		}
		*_stream << t;
		print(level, file, func, line, args...);
	}
};//log
} //logging
#endif//LOG_ENABLE
#endif//LOG_H
