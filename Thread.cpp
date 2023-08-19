#include "Thread.h"
#include "CurrentThread.h"



Thread::Thread(ThreadFunc func, const std::string& name) 
    : func_(std::move(func)),
      started_(false),
      joined_(false),
      tid_(0),
      name_(name) {
    setDefaultName();    
}

Thread::~Thread() {
    if (started_ && !joined_) {
        thread_->detach();
    }
}

void Thread::start() {
    started_ = true;

    thread_ = std::unique_ptr<std::thread>(new std::thread([&](){
        {
            std::unique_lock<std::mutex> lock(mtx_);
            tid_ = CurrentThread::tid();
            cond_.notify_one();
        }
        // 确保start返回后，tid_已经初始化
        func_();
    }));  // 会进行移动

    {
        std::unique_lock<std::mutex> lock(mtx_);
        while(tid_ == 0) {
            cond_.wait(lock);
        }
    }
}

void Thread::join() {
    joined_ = true;
    thread_->join();
}


bool Thread::started() const {
    return started_;
}

pid_t Thread::tid() const {
    return tid_;
}
   
const std::string& Thread::name() const {
    return name_;
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if (name_.empty()) {  // 如果没有提供名字，则使用默认的名字
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

