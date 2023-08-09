#include "EventLoop.h"
#include "CurrentThread.h"
#include "EPollPoller.h"

#include <sys/eventfd.h>

// 线程局部存储，当不同线程访问该变量时会对应不同的实例
__thread EventLoop* t_loopInThisThread = nullptr;  // 每个线程所对应的EventLoop对象

const int kPollTimeoutMs = 10000;

int createEventfd() {
    int evfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evfd < 0) {
        // 错误处理
    }
    return evfd;
}

EventLoop::EventLoop()
    : looping_(false), 
      quit_(false),
      callingPendingFunctors_(false),
      threadId_(CurrentThread::tid()),
      poller_(new EpollPoller(this)),
      wakeupFd_(createEventfd()),
      wakeupChannel_(this, wakeupFd_) {
    // 检查所属线程是否已经存在EventLoop对象
    if (t_loopInThisThread) {
        // 退出程序
    } else {
        t_loopInThisThread = this;
        wakeupChannel_.setReadCallback(std::bind(&EventLoop::handleRead, this));
        wakeupChannel_.enableReading();
    }

}

EventLoop::~EventLoop() {
    looping_ = false;
    t_loopInThisThread = nullptr;
    wakeupChannel_.disableAll();
    close(wakeupFd_);
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

        doPendingFunctors();
    }
    
 

}

void EventLoop::updateChannel(Channel* channel) {
    poller_->updateChannel(channel);
}

void EventLoop::quit() {
    quit_ = true;
    // EventLoop其所在的IO线程自始至终在执行loop，如果此时在调用quit
    // 则说明不在阻塞，则直接设置quit即可，下次循环检查就会检查到quit并退出 
    if (!isInLoopThread()) {
         // 唤醒
    }   
    
}

// 可能跨线程调用，需要加锁
void EventLoop::runInLoop(const Functor& cb) {
    if (isInLoopThread()) {
        cb();
    } else {
        queueInLoop(cb);
    }
}

// 可能跨线程调用，需要加锁
void EventLoop::queueInLoop(const Functor& cb) {
    {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        pendingFunctors_.emplace_back(cb);
    }

     // 每个EventLoop都在执行loop函数，通常情况下阻塞在poll方法，不然就在执行poll后的操作
     // 调用queueInLoop可能有三种情况：
     // (1)其他线程调用queueInLoop (2)handleEvent执行用户回调时调用queueInLoop (3)doPendingFunctors执行functors调用queueInLoop
     // (1)(3)情况下需要唤醒，来保证及时执行新添加的functors (2)不需要唤醒因为接下来马上会执行doPendingFunctors
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        functors.swap(pendingFunctors_);
    }
    for (auto& functor: functors) {
        functor();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof(one));

    if (n != sizeof(one)) {
        // 错误处理
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        // 错误处理
    }
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