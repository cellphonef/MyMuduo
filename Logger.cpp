#include "Logger.h"

// 多个函数同时调用该方法时，只有一个会成功。
Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

void Logger::setLevel(Level level) {
    level_ = level;
}


void Logger::log() {


}