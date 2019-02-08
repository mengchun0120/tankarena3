#ifndef INCLUDE_LOG
#define INCLUDE_LOG

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum LogLevelType {
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_WARN,
    LEVEL_ERROR
} LogLevel;

extern const char *g_level_str[];

typedef struct LoggerType {
    FILE *fp;
    LogLevel min_level;
} Logger;

extern Logger g_logger;

int setup_log(Logger *logger, const char *log_file, LogLevel min_level);

int write_log(Logger *logger, const char *format, ...);

int log_time(Logger *logger);

int close_log(Logger *logger);

#ifdef ENABLE_LOG
#define LOG_WRITE(level, format, ...) \
    { \
        if(g_logger.fp != NULL && level >= g_logger.min_level) { \
            log_time(&g_logger); \
            write_log(&g_logger, " %s:%d %s ", __FILE__, __LINE__, g_level_str[level]); \
            write_log(&g_logger, format, __VA_ARGS__); \
            write_log(&g_logger, "\n"); \
            fflush(g_logger.fp); \
        } \
    }
#else
#define LOG_WRITE(level, format, ...)
#endif

#define LOG_DEBUG(format, ...) \
    LOG_WRITE(LEVEL_DEBUG, format, __VA_ARGS__)

#define LOG_INFO(format, ...) \
    LOG_WRITE(LEVEL_INFO, format, __VA_ARGS__)

#define LOG_WARN(format, ...) \
    LOG_WRITE(LEVEL_WARN, format, __VA_ARGS__)

#define LOG_ERROR(format, ...) \
    LOG_WRITE(LEVEL_ERROR, format, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
