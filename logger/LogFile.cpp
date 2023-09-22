#include "LogFile.h"

#include <stdio.h>

class LogFile::File {
public:
    explicit File(const std::string& filename);
    ~File();

    void append(const char* logLine, size_t len);  // 添加数据到缓冲区
    void flush();  // 刷新缓冲区

    size_t writtenBytes() const;

private:
    size_t write(const char* logLine, size_t len); 
    FILE* fp_;  // 文件句柄
    char buffer_[64 * 1024];  // 缓冲区
    size_t writtenBytes_;  // 已写字节

};

LogFile::File::File(const std::string& filename)
    : fp_(::fopen(filename.c_str(), "ae")),  // e for CLOEXEC
      writtenBytes_(0) {
    ::setbuffer(fp_, buffer_, sizeof(buffer_));  // 设置缓冲区，详情见：https://man7.org/linux/man-pages/man3/setbuf.3.html
} 

LogFile::File::~File() {
    ::fclose(fp_);
}

size_t LogFile::File::write(const char* logLine, size_t len) {
    return ::fwrite_unlocked(logLine, 1, len, fp_);  // 使用fwrite_unlocked来提高性能
}

void LogFile::File::append(const char* logLine, size_t len) {
    size_t written;
    while (written != len) {  // 不断写直到写完
        size_t n = write(logLine + written, len - written);
        if (n == 0) {  // 错误
            
        }
        written += n;
    }
    writtenBytes_ += len;
}

void LogFile::File::flush() {
    ::fflush(fp_);
}

LogFile::LogFile(std::string basename, size_t rollSize, int flushInterval)
    : basename_(),
      rollSize_(rollSize),
      flushInterval_(flushInterval) {

}

LogFile::LogFile(std::string basename, size_t rollSize)
    : basename_(basename),
      rollSize_(rollSize) {}

LogFile::~LogFile()
{
}

// 
void LogFile::append(const char* logLine, size_t len) {
    file_->append(logLine, len);

}