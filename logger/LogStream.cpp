#include "LogStream.h"

LogStream::LogStream() {

}

LogStream::~LogStream() {

}

LogStream& LogStream::operator<<(bool v) {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
}


void LogStream::append(const char* data, size_t len) {
    buffer_.append(data, len);
}

