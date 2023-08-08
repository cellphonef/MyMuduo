#ifndef MYMUDUO_EVENTLOOP_H
#define MYMUDUO_EVENTLOOP_H

#include "Channel.h"
#include "Poller.h"

#include <vector>
#include <memory>
#include <mutex>


// 事件循环类（Reactor）
// 
// 设计：
// - 构造函数：每个线程只能有一个EventLoop对象，因此构造函数需要检查当前线程是否已经创建了其他EventLoop对象。
//
class EventLoop {
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();  // 运行事件循环

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

    void quit();

    void runInLoop(const Functor& cb);  // 在该EventLoop所属的IO线程执行用户任务回调cb
                                        // 该成员函数可能跨线程调用

    void queueInLoop(const Functor& cb);  // 将用户任务回调cb放入队列中，并唤醒其执行
                                          // 该成员函数可能跨线程调用

    void assertInLoopThread();  // 断言在EventLoop所属线程，用于保证操作安全
    bool isInLoopThread();  // 判断当前线程是否EventLoop所在线程


private:
    void abortNotInLoopThread();  // 终止程序（当不在EventLoop对象所属线程）
    void doPendingFunctors();  // 执行用户提交的回调
    void wakeup();

    // 以下定义的成员中使用const来保证对象在构造时就要初始化且后续不可改变
    // 如EventLoop所属的线程，在创建EventLoop时就必须初始化且后续不可改变
    const pid_t threadId_;  // EventLoop所属线程

    using ChannelList = std::vector<Channel*>;
    ChannelList activeChannels_;  // 活跃通道

    std::unique_ptr<Poller> poller_;  // 多路分发器
    bool looping_;
    bool quit_;

    bool callingPendingFunctors_;  // 是否正在执行Functors，可能多个线程访问需要保护
    std::vector<Functor> pendingFunctors_;  // 存储需要执行的Functors，可能多个线程访问需要保护
    std::mutex mutex_;  // 用于保护

};



#endif  // MYMUDUO_EVENTLOOP_H