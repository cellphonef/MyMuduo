#ifndef MYMUDUO_EVENTLOOPTHREAD_H
#define MYMUDUO_EVENTLOOPTHREAD_H

#include "Thread.h"

#include <functional>
#include <condition_variable>
#include <mutex>

class EventLoop;

// IO线程
// 
// 通过包含一个Thread类使其成为一个线程，与此同时实现自己的threadFunc使其成为一个具有自定义功能的线程，这里是IO线程。
// 
class EventLoopThread {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    
    EventLoopThread(const ThreadInitCallback& cb,
                    const std::string& name);
    ~EventLoopThread();

    EventLoop* startLoop();  // 启动EventLoop，并返回对应的指针

private:
    void threadFunc();  // 线程需要运行的函数

    EventLoop* loop_;  // 线程运行的EventLoop
    Thread thread_;  // 线程
                     // 为什么这里又不用unique_ptr了？

    std::mutex mtx_;  // 锁
    std::condition_variable cond_;  // 条件变量
    
    ThreadInitCallback callback_;  // 每个线程执行前的初始化操作，由上层设置（这里是EventLoopThreadPool）



};


#endif  // MYMUDUO_EVENTLOOPTHREAD_H