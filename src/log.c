#include <stdarg.h>
#include <time.h>
#include "log.h"

Logger g_logger;

const char *g_level_str[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

int setup_log(Logger *logger, const char *log_file, LogLevel min_level)
{
    logger->fp = fopen(log_file, "a");
    if(logger->fp == NULL) {
        return -1;
    }

    logger->min_level = min_level;
    return 0;
}

int write_log(Logger *logger, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(logger->fp, format, args);
    va_end(args);
}

int log_time(Logger *logger)
{
    struct timespec tp;
    if(clock_gettime(CLOCK_REALTIME, &tp) == -1) {
        return -1;
    }

    struct tm t;
    if(localtime_r(&tp.tv_sec, &t) == NULL) {
        return -1;
    }

    int ms = tp.tv_nsec / 1e6;
    return write_log(logger, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                    (t.tm_year+1900), t.tm_mon, t.tm_mday, t.tm_hour,
                    t.tm_min, t.tm_sec, ms);
}

int close_log(Logger *logger)
{
    fclose(logger->fp);
}
