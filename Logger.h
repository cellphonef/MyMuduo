#ifndef MYMUDUO_LOGGER_H
#define MYMUDUO_LOGGER_H

#include "noncopyable.h"


//  日志类
// 
//  实现和设计要点：
//    ①日志类应该是一个单例（自然不可拷贝），全局使用一个。
//    ②日志的输出
//      （1）可以定制格式
//      （2）且具有不同级别（如INFO、DEBUG、ERROR、FATAL等）
//      （3）可以定制不同的输出位置（如文件、控制台等） 
class Logger: noncopyable {
public:
    enum class Level {
        INFO,
        DEBUG,
        ERROR,
        FATAL,
    };
 
    

    // 单例模式
    static Logger& getInstance();

    // 设置日志级别
    void setLevel(Level level);

    // 主要方法进行log
    void log();

private:

    Logger() = default;
    ~Logger() = default;
    

    Level level_;



};

#endif  // MYMUDUO_LOGGER_H