#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <iostream>
#include <sstream>


enum loglevel_e
    {logNOTHING, logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

class logIt
{
public:
    static std::string levelToString(loglevel_e level){
        switch (level)
        {
        case logNOTHING:
            return "";
        case logERROR:
            return "ERROR";
        case logWARNING:
            return "WARNING";
        case logINFO:
            return "INFO";
        case logDEBUG:
            return "DEBUG";
        case logDEBUG1:
            return "DEBUG1";
        case logDEBUG2:
            return "DEBUG2";
        case logDEBUG3:
            return "DEBUG3";
        case logDEBUG4:
            return "DEBUG4";
        default:
            return std::to_string(level);
        }
    }

    logIt(loglevel_e _loglevel = logERROR) {
        this->_log_level = _loglevel;
        if(_log_level == logNOTHING) return;
        _buffer << levelToString(_loglevel) << "(pid: " << std::to_string(::getpid()) << ")" <<  " :" 
            << std::string(
                _log_level > logDEBUG
                ? (_log_level - logDEBUG) * 4
                : 1
                , ' ');
    }

    template <typename T>
    logIt & operator<<(T const & value)
    {
        if(_log_level == logNOTHING) return *this;
        _buffer << value;
        return *this;
    }

    ~logIt()
    {
        _buffer << std::endl;
        // This is atomic according to the POSIX standard
        // http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
        std::cerr << _buffer.str();
    }

private:
    loglevel_e _log_level;
    std::ostringstream _buffer;
};

extern loglevel_e loglevel;

#define log(level) \
if (level > loglevel) ; \
else logIt(level)

#define log_(level, addon) \
if(level > loglevel);      \
else logIt(level) << "[" << addon << "]"

#endif