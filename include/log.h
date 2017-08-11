#ifndef LOG_H
#define LOG_H

#include <cstdio>
#include <ctime>

enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

void log_line(LogLevel log_level, const char* format, ...);

#endif
