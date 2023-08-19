#include "Logger.h"
#include "Timestamp.h"

#include <iostream>

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

void Logger::setLevel(Level level) {
    level_ = level;
}

// [级别信息] 时间戳 : msg 
void Logger::log(const std::string& msg) {
    std::string pre = "";
    switch(level_) {
        case Level::INFO:
            pre = "[INFO]";
            break;
        case Level::DEBUG:
            pre = "[DEBUG]";
            break;
        case Level::ERROR:
            pre = "[ERROR]";
            break;
        case Level::FATAL:
            pre = "[FATAL]";
            break;
        default:
            break;
    }
    std::cout << pre + Timestamp::now().toString() << " : " << msg << std::endl;
}