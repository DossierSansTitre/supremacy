#include <cstdarg>
#include <log.h>

static const char* log_level_tags[] = {
    "[DEBUG]",
    "[INFO ]",
    "[WARN ]",
    "[ERROR]",
    "[FATAL]"
};

void log_line(LogLevel log_level, const char* format, ...)
{
    static const size_t timebuf_size = 1024;
    char timebuf[timebuf_size];
    time_t raw_time;
    tm* timeinfo;
    va_list args;

    va_start(args, format);

    time(&raw_time);
    timeinfo = localtime(&raw_time);
    strftime(timebuf, timebuf_size, "%d/%m/%Y %H:%M:%S", timeinfo);

    printf("%s %s - ", log_level_tags[static_cast<size_t>(log_level)], timebuf);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}
