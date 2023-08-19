#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const std::string& name)
    : loop_(nullptr),
      thread_(std::bind(&EventLoopThread::threadFunc, this), name),
      callback_(cb) {}

EventLoopThread::~EventLoopThread() {
    if (loop_ != nullptr) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    // 解释一下这段代码的逻辑：
    // 首先需要了解startLoop方法的作用是启动事件循环，并返回对应的EventLoop指针，
    // 关键就在于返回对应EventLoop指针的时机，此时对应的EventLoop应该已经初始化完成。
    // 
    // 具体来说，首先调用start函数启动线程，启动后线程将执行EventLoopThread::threadFunc方法
    // EventLoopThread::threadFunc方法：
    //   1. ThreadInitCallback初始化线程
    //   2. 初始化完成后会唤醒该方法继续执行
    //   3. 开启事件循环
    // 因此startLoop方法只需要等待在loop_这里即可，当loop_改变后代表初始化完成。
    
    thread_.start();
    EventLoop* loop = nullptr;

    {
        std::unique_lock<std::mutex> lock(mtx_);
        while (loop_ == nullptr) {  // while防止虚假唤醒
            cond_.wait(lock);   
        }
        loop = loop_;
    }
    return loop;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;  // 栈上对象

    if (callback_) {
        callback_(&loop);
    }

    {
        std::unique_lock<std::mutex> lock(mtx_);
        loop_ = &loop;
        cond_.notify_one();
    }
    loop.loop();  // 开启事件循环，当该函数返回时，代表结束
    
    std::unique_lock<std::mutex> lock(mtx_);
    loop_ = nullptr;  // 重新置nullptr


}