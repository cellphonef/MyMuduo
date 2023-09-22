#ifndef MYMUDUO_LOGGER_LOGFILE_H
#define MYMUDUO_LOGGER_LOGFILE_H

#include <string>
#include <memory>

// 日志信息文件输出类
class LogFile {
public:
    LogFile(std::string basename, size_t rollSize, int flushInterval=3);
    ~LogFile();

    void append();  // 往文件中写数据
    void flush();  // 刷新缓冲


private:
    const std::string basename_;
    const size_t rollSize_;
    const int flushInterval_;

    class File;
    std::unique_ptr<File> file_;

    

};







#endif  // MYMUDUO_LOGGER_LOGFILE_H

