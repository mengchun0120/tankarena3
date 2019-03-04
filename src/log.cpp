#include <cstdarg>
#include <ctime>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "log.h"

namespace ta3 {

std::shared_ptr<Logger> g_logger;

Logger& Logger::getSingleton()
{
    return *g_logger;
}

bool Logger::initSingleton(const char* logFile, LogLevel minLevel)
{
    if(g_logger) {
        std::cerr << "Logger has already been initialized";
        return false;
    }

    Logger* logger = new Logger(logFile, minLevel);
    if(!logger->valid()) {
        LOG_ERROR("Exception happened while constructing logger");
        delete logger;
        return false;
    }

    g_logger.reset(logger);
    return true;
}

const char* Logger::levelString(LogLevel level)
{
    switch(level) {
    case LEVEL_DEBUG:
        return "DEBUG";
    case LEVEL_INFO:
        return "INFO";
    case LEVEL_WARN:
        return "WARN";
    case LEVEL_ERROR:
        return "ERROR";
    }
    return "UNKNOWN";
}

Logger::Logger(const char* logFile, LogLevel minLevel)
: m_fp(nullptr)
, m_minLevel(minLevel)
{
    m_fp = fopen(logFile, "w");
    if(m_fp == nullptr) {
        std::cerr << "Failed to open log file " << logFile << std::endl;
    }
}

Logger::~Logger()
{
    close();
}

void Logger::write(const char *format, ...)
{
    if(valid()) {
        va_list args;
        va_start(args, format);
        vfprintf(m_fp, format, args);
        va_end(args);
    }
}

void Logger::logTime()
{
    struct timespec tp;
    if(clock_gettime(CLOCK_REALTIME, &tp) == -1) {
        return;
    }

    struct tm t;
    if(localtime_r(&tp.tv_sec, &t) == NULL) {
        return;
    }

    int ms = tp.tv_nsec / 1e6;
    write("%04d-%02d-%02d %02d:%02d:%02d.%03d",
          (t.tm_year+1900), t.tm_mon, t.tm_mday, t.tm_hour,
          t.tm_min, t.tm_sec, ms);
}

void Logger::flush()
{
    if(valid()) {
        fflush(m_fp);
    }
}

void Logger::close()
{
    if(valid()) {
        fclose(m_fp);
        m_fp = nullptr;
    }
}

} // end of namespace ta3
