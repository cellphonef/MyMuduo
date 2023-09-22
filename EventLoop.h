#ifndef MYMUDUO_EVENTLOOP_H
#define MYMUDUO_EVENTLOOP_H

#include <vector>
#include <memory>
#include <mutex>

class Poller;
class Channel;

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

    void loop();  // 运行事件循环，应该在EventLoop所属线程运行

    void wakeup();  // 唤醒
    void updateChannel(Channel* channel);  // 更新所监听的对应通道
    void removeChannel(Channel* channel);  // 移除所监听的对应通道

    void quit();  // 停止事件循环

    void runInLoop(const Functor& cb);  // 在该EventLoop所属的IO线程执行用户任务回调cb
                                        // 该成员函数可能跨线程调用

    void queueInLoop(const Functor& cb);  // 将用户任务回调cb放入队列中，并唤醒其执行
                                          // 该成员函数可能跨线程调用

    void assertInLoopThread();  // 断言在EventLoop所属线程，如果不满足则终止程序，用于保证操作安全
    bool isInLoopThread();  // 判断当前线程是否EventLoop所在线程


private:
    void abortNotInLoopThread();  // 终止程序（当不在EventLoop对象所属线程）
    void handleRead();  // 内部使用，用于处理wakeupFd可读事件
    void doPendingFunctors();  // 执行用户提交的回调
    
    // 以下定义的成员中使用const来保证对象在构造时就要初始化且后续不可改变
    // 如EventLoop所属的线程，在创建EventLoop时就必须初始化且后续不可改变
    const pid_t threadId_;  // EventLoop所属线程

    using ChannelList = std::vector<Channel*>;
    ChannelList activeChannels_;  // 用于保存活跃通道

    std::unique_ptr<Poller> poller_;  // 多路分发器
    bool looping_;
    bool quit_;

    bool callingPendingFunctors_;  // 是否正在执行Functors，可能多个线程访问需要保护，对其操作应该加锁
    std::vector<Functor> pendingFunctors_;  // 存储需要执行的Functors，可能多个线程访问需要保护，对其操作应该是原子的
    std::mutex mutex_;  // 用于保护

    int wakeupFd_;  // 唤醒文件描述符
    std::unique_ptr<Channel> wakeupChannel_;  // wakeupChannel生命周期由EventLoop管理
};


#endif  // MYMUDUO_EVENTLOOP_H