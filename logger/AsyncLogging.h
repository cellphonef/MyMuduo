#ifndef MYMUDUO_LOGGER_ASYNCLOGGING_H
#define MYMUDUO_LOGGER_ASYNCLOGGING_H

#include "FixedBuffer.h"

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>


// 异步写日志线程类
//
// 
class AsyncLogging {
public:

    AsyncLogging(const std::string& basename, size_t rollSize, int flushInterval=3);
    ~AsyncLogging();

    void append();  // 前端日志写入
    void start();  // 启动异步写日志
    void stop();  // 停止异步写日志


private:
    using Buffer = FixedBuffer<kLargeBuffer>;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;

    void threadFunc();  // 后端日志落盘

    
    bool running_;
    std::string basename_;
    size_t rollSize_;
    const int flushInterval_;

    std::unique_ptr<std::thread> thread_;
    std::mutex mtx_;
    std::condition_variable cond_;

    BufferPtr currentBuffer_;  // 当前缓冲 
    BufferPtr nextBuffer_;  // 预备缓冲
    BufferVector buffers_;  // 前端缓冲队列    


};





#endif  // MYMUDUO_LOGGER_ASYNCLOGGING_H