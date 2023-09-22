#ifndef MYMUDUO_LOGGER_LOGGING_H
#define MYMUDUO_LOGGER_LOGGING_H

#include "LogStream.h"

#include <memory>

class Logger {
public:

    enum class LogLevel {
        DEBUG,
        INFO,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    }; 

    Logger(const char* file, int line);
    Logger(const char* file, int line, LogLevel level);
    ~Logger();
    LogStream& stream();

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    using OutputFunc = void(*)(const char*, size_t len);
    using FlushFunc = void(*)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);


private:
    static LogLevel g_logLevel;  // 全局共享一个日志级别
    static OutputFunc g_output;  // 全局共享一个日志输出函数
    static FlushFunc g_flush;  // 全局共享一个刷新函数

    // PImpl惯例
    class Impl;
    std::unique_ptr<Impl> impl_;
};

// 使用示例：LOG_DEBUG << "My log information";
// 等价于Logger(__FILE__, __LINE__, Logger::LogLevel::DEBUG).stream() << "My log information";
#define LOG_DEBUG if(Logger::logLevel() <= Logger::LogLevel::DEBUG) \
    Logger(__FILE__, __LINE__, Logger::LogLevel::DEBUG).stream()
#define LOG_INFO if(Logger::logLevel() <= Logger::LogLevel::INFO) \
    Logger(__FILE__, __LINE__, Logger::LogLevel::INFO).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::LogLevel::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::LogLevel::FATAL).stream() 





#endif  // MYMUDUO_LOGGER_LOGGING_H