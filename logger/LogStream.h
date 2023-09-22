#ifndef MYMUDUO_LOGGER_LOGSTREAM_H
#define MYMUDUO_LOGGER_LOGSTREAM_H

#include "FixedBuffer.h"

#include <string>

// 提供C++流式风格的日志方法
// 
class LogStream {
public:
    LogStream();
    ~LogStream();

    // 重载各种数据类型的operator<<
    LogStream& operator<<(bool);
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(float);
    LogStream& operator<<(double);
    LogStream& operator<<(char);
    LogStream& operator<<(const char*);
    LogStream& operator<<(const void*);
    LogStream& operator<<(const std::string&);

    void append(const char* buf, size_t len);



private:
    static const int kMaxNumbericSize = 48;
    FixedBuffer<kSmallBuffer> buffer_;
};






#endif  // MYMUDUO_LOGGER_LOGSTREAM_H