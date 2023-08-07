#include "EventLoop.h"
#include "CurrentThread.h"
#include "EPollPoller.h"

// 线程局部存储，当不同线程访问该变量时会对应不同的实例
__thread EventLoop* t_loopInThisThread = nullptr;  // 每个线程所对应的EventLoop对象

const int kPollTimeoutMs = 10000;

EventLoop::EventLoop(): 
    looping_(false), 
    threadId_(CurrentThread::tid()),
    poller_(new EpollPoller(this)) {
    // 检查所属线程是否已经存在EventLoop对象
    if (t_loopInThisThread) {
        // 退出程序
    } else {
        t_loopInThisThread = this;
    }

}

EventLoop::~EventLoop() {
    looping_ = false;
    t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    while (!quit_) {
        activeChannels_.clear();
        Timestamp now = poller_->poll(kPollTimeoutMs, &activeChannels_);

        // 开始处理事件
        for (auto channel: activeChannels_) {
            channel->handleEvent();
        }

        // doPendingFunctors
    }
    
 

}

void EventLoop::updateChannel(Channel* channel) {
    poller_->updateChannel(channel);
}

void EventLoop::quit() {
    quit_ = true;
    // 唤醒
}


bool EventLoop::isInLoopThread() {
    return CurrentThread::tid() == threadId_;
}


void EventLoop::assertInLoopThread() {
    if (!isInLoopThread()) {
        abortNotInLoopThread();
    }
}

void EventLoop::abortNotInLoopThread() {
    exit(1);
}