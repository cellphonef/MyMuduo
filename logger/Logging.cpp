#include "Logging.h"

class Logger::Impl {
public:





private:
    LogStream stream_;


}


Logger::Logger(const char* file, int line) {

}

Logger::Logger(const char* file, int line, LogLevel level) {

}

Logger::~Logger() {
    stream() << "\n";
    
}

LogStream& Logger::stream() {
    return impl_->stream_;
}