#ifndef INCLUDE_LOG
#define INCLUDE_LOG

#include <stdio.h>

namespace ta3 {

class Logger {
public:
    enum LogLevel {
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARN,
        LEVEL_ERROR
    };

    static Logger& getSingleton();

    static bool initSingleton(const char* logFile, LogLevel minLevel);

    static const char* levelString(LogLevel level);

    ~Logger();

    void write(const char *format, ...);

    void logTime();

    LogLevel minLevel() const
    {
        return m_minLevel;
    }

    bool valid() const
    {
        return m_fp != nullptr;
    }

    void flush();

    void close();

private:
    Logger(const char* logFile, LogLevel minLevel);

    FILE *m_fp;
    LogLevel m_minLevel;
};

} // end of namespace ta3

#ifdef ENABLE_LOG
#define LOG_WRITE(level, ...) \
    { \
        ta3::Logger& logger = ta3::Logger::getSingleton(); \
        if(logger.valid() && level >= logger.minLevel()) { \
            logger.logTime(); \
            logger.write(" %s:%d %s ", __FILE__, __LINE__, ta3::Logger::levelString(level)); \
            logger.write(__VA_ARGS__); \
            logger.write("\n"); \
            logger.flush(); \
        } \
    }
#else
#define LOG_WRITE(level, ...)
#endif

#define LOG_DEBUG(...) \
    LOG_WRITE(ta3::Logger::LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(...) \
    LOG_WRITE(ta3::Logger::LEVEL_INFO, __VA_ARGS__)

#define LOG_WARN(...) \
    LOG_WRITE(ta3::Logger::LEVEL_WARN, __VA_ARGS__)

#define LOG_ERROR(...) \
    LOG_WRITE(ta3::Logger::LEVEL_ERROR, __VA_ARGS__)

#endif
