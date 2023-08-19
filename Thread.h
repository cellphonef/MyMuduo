#ifndef MYMUDUO_THREAD_H
#define MYMUDUO_THREAD_H

#include <functional>
#include <thread>
#include <memory>
#include <string>
#include <mutex>
#include <condition_variable>

class Thread {
public:
    using ThreadFunc = std::function<void()>;  
    Thread(ThreadFunc func, const std::string& name = std::string());  // 传入线程运行函数以及线程名
    ~Thread();

    void start();  // 启动线程
    void join();  // 线程汇合

    // getter
    bool started() const;
    pid_t tid() const;
    const std::string& name() const;


private:
    void setDefaultName();

    ThreadFunc func_;  // 线程执行的具体函数 
    bool started_;  // 是否启动
    bool joined_;  // 是否汇合
    std::unique_ptr<std::thread> thread_;  // 线程句柄
    pid_t tid_;  // 线程真实ID
    std::string name_;  // 线程名

    std::mutex mtx_;
    std::condition_variable cond_;

    static int numCreated_;

};


#endif  // MYMUDUO_THREAD_H