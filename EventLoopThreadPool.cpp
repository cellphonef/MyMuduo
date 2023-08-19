#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* mainLoop, const std::string& name)
    : mainLoop_(mainLoop),
      name_(name),
      started_(false),
      numThreads_(0),
      next_(0) {}

EventLoopThreadPool::~EventLoopThreadPool() {}

void EventLoopThreadPool::setThreadNum(int numThreads) {
    numThreads_ = numThreads;
}
    
void EventLoopThreadPool::start(const ThreadInitCallback& cb) {
    started_ = true;
    for (int i = 0; i < numThreads_; ++i) {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof(buf), "%s%d", name_.c_str(), i);
        EventLoopThread* t = new EventLoopThread(cb, buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }

    if (numThreads_ == 0 && cb) {
        cb(mainLoop_);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    EventLoop* loop = mainLoop_;
    if (!loops_.empty()) {
        // round-robin，此处可以优化
        loop = loops_[next_];
        next_ = (next_ + 1) % loops_.size();
    }
    return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops() {
    if (!loops_.empty()) {
        return loops_;
    } else {
        return std::vector<EventLoop*> (1, mainLoop_);
    }
}
