#include "AsyncLogging.h"
#include "LogFile.h"


AsyncLogging::AsyncLogging(const std::string& basename, size_t rollSize, int flushInterval)
    : running_(false),
      basename_(basename),
      rollSize_(rollSize),
      flushInterval_(flushInterval),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer),
      buffers_() {
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);  // 预留16个Buffer
}

void AsyncLogging::append(const char* logLine, size_t len) {
    std::lock_guard<std::mutex> lockGuard(mtx_);
    
    if (currentBuffer_->avail() > len) {  // 当前缓冲区未满
        currentBuffer_->append(logLine, len);  // 直接追加
    } else {  // 当前缓冲区写满
        buffers_.emplace_back(currentBuffer_.release());  // 首先将当前缓冲区移入前端缓冲区队列

        // 判断预备缓冲区是否满
        if (nextBuffer_) {  // 未满，使用该缓冲区
            currentBuffer_ = std::move(nextBuffer_);  // nextBuffer_ == nullptr
        } else {  // 已满
            currentBuffer_.reset(new FixedBuffer<kLargeBuffer>);
        }
    }
    currentBuffer_->append(logLine, len);  // 追加消息
    cond_.notify_one();  // 唤醒
}


void AsyncLogging::threadFunc() {
    LogFile output(basename_, rollSize_);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);

    while (running_) {
        {
            std::unique_lock<std::mutex> uniGuard(mtx_);
            if (buffers_.empty()) {  // 没有日志信息需要写
                cond_.wait_for(uniGuard, std::chrono::seconds(flushInterval_));
            }
            buffers_.emplace_back(currentBuffer_.release());
            currentBuffer_ = std::move(newBuffer1);
            buffersToWrite.swap(buffers_);
            if (!nextBuffer_) {
                nextBuffer_ = std::move(newBuffer2);  // >=2
            }
        }

        for (const auto& b : buffersToWrite) {
            output.append(b->data(), b->length());
        }
        
        // 写完后
        if (buffersToWrite.size() > 2) {
            buffersToWrite.resize(2);
        }

        if (!newBuffer1) {  // newBuffer1为空
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2) {  // newBuffer2为空 说明 newBuffer2 给了 nextBuffer_, buffersToWrite 必然还有 1 个 buffer
        
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();  // 只是清空元素，但不会回收内存
        output.flush();  // 刷新
    }
    output.flush();
}