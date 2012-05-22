/* This is a reused code */
#include "logger.h"

Logger::Logger() :
	_null(0), _verbose(0), _verbose_level(0), _error(0), _error_level(0), _trace(0), _trace_level(0), _stream()
{
}

Logger::~Logger()
{
}

Logger& Logger::get_instance()
{
	static Logger logger;
	return logger;
}

Logger::Stream::Stream() :
	_sstream(), _current_output(0)
{
}

std::string Logger::Stream::str() const
{
	return _sstream.str();
}

void Logger::Stream::clear()
{
	_sstream.str("");
}

