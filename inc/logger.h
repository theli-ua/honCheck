/* This is a reused code */
#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <sstream>
#include <fstream>
#include <string>

#include "ansi_colors.h"

namespace TC = TermAnsiColor;

namespace
{
    const TC::AnsiColor VerboseColor = TC::GREEN;
    const TC::AnsiColor ErrorColor = TC::RED + TC::BOLD;
    const TC::AnsiColor TraceColor = TC::MAGENTA;
} // namespace

class Logger
{
public:
    /* class for streaming anything */
    class Stream
    {
    public:
        Stream();
        std::string str() const;
        void clear();
        template<class T> Logger::Stream& add(const T& t);
        template<class T> Logger::Stream& operator<<(const T& t);
        inline Logger::Stream& operator<<(Logger::Stream& (*pf)(Logger::Stream&));
    private:
        friend class Logger;
        Stream(const Logger::Stream&);
        Stream& operator=(const Stream&);
        std::ostringstream _sstream;
        std::ostream *_current_output;
    };

    /* get static Logger object */
    static Logger& get_instance();

    /* stream to suppress output */
    inline void set_null(std::ostream& stream);

    /* stream to verbose output */
    inline Logger::Stream& verbose(int level = 0);
    inline void set_verbose(std::ostream& stream, int min_level = 0);

    /* stream to error output */
    inline Logger::Stream& error(int level = 0);
    inline void set_error(std::ostream& stream, int min_level = 0);

    /* stream to trace output */
    inline Logger::Stream& trace(int level = 0);
    inline void set_trace(std::ostream& stream, int min_level = 0);

    /* finish formatting string for output */
    inline static Logger::Stream& end(Logger::Stream& s);
    inline static Logger::Stream& rend(Logger::Stream& s);

private:
    Logger(); // this is a singleton
    ~Logger();
    Logger(const Logger&); // dissallow copy c'tor
    const Logger& operator=(const Logger&); // disallow copy operator

    std::ostream *_null; // stream to suppress output
    std::ostream *_verbose; // stream to verbose output
    int _verbose_level;
    std::ostream *_error; // stream to error output
    int _error_level;
    std::ostream *_trace; // stream to trace output
    int _trace_level;

    Stream _stream; // stream that generates string for output
};

inline Logger::Stream& Logger::Stream::operator<<(Logger::Stream& (*pf)(Logger::Stream&))
{
    return pf(*this);
}

template<class T>
Logger::Stream& Logger::Stream::add(const T& t)
{
#pragma omp critical (logger_add)
    _sstream << t;
    return *this;
}

template<class T>
Logger::Stream& Logger::Stream::operator<<(const T& t)
{
    return add(t);
}

inline void Logger::set_null(std::ostream& stream)
{
    _null = &stream;
}

inline Logger::Stream& Logger::verbose(int level)
{
    _stream._current_output = level >= _verbose_level ? _verbose : _null;
    _stream << VerboseColor;
    return _stream;
}

inline void Logger::set_verbose(std::ostream& stream, int min_level)
{
    _verbose = &stream;
    _verbose_level = min_level;
}

inline Logger::Stream& Logger::error(int level)
{
    _stream._current_output = level >= _error_level ? _error : _null;
    _stream << ErrorColor;
    return _stream;
}

inline void Logger::set_error(std::ostream& stream, int min_level)
{
    _error = &stream;
    _error_level = min_level;
}

inline Logger::Stream& Logger::trace(int level)
{
    _stream._current_output = level >= _trace_level ? _trace : _null;
    _stream << TraceColor;
    return _stream;
}

inline void Logger::set_trace(std::ostream& stream, int min_level)
{
    _trace = &stream;
    _trace_level = min_level;
}

inline Logger::Stream& Logger::end(Logger::Stream& s)
{
#pragma omp critical(loggerend)
    {
    s << TC::DEFAULT;
    if (s._current_output)
        *s._current_output << s.str() << std::endl;
    s.clear();
    }
    return s;
}
inline Logger::Stream& Logger::rend(Logger::Stream& s)
{
    s << TC::DEFAULT;
    if (s._current_output)
        *s._current_output << s.str() << '\r';
    s.clear();
    return s;
}

#endif /* __LOGGER_H_ */

