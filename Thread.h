#ifndef MYMUDUO_THREAD_H
#define MYMUDUO_THREAD_H

#include <functional>
#include <thread>
#include <memory>

class Thread {
public:
    using ThreadFunc = std::function<void()>;  // 最终保存Eventloop::threadFunc()函数
    Thread(ThreadFunc func);
    ~Thread();

    void start();                              // 启动线程
    void join();                               // 线程汇合

private:
    ThreadFunc func_;                          // 线程执行的具体函数 
    bool started_;                             // 是否启动
    bool joined_;                              // 是否汇合
    std::thread* thread_;                      // 线程句柄
    pid_t tid;                                 // 线程真实ID

};


#endif  // MYMUDUO_THREAD_H