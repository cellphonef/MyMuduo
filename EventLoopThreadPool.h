#ifndef MYMUDUO_EVENTLOOPTHREADPOLL_H
#define MYMUDUO_EVENTLOOPTHREADPOLL_H

#include <vector>
#include <memory>
#include <functional>
#include <string>

class EventLoop;
class EventLoopThread;

// IO线程池
// 
class EventLoopThreadPool {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    EventLoopThreadPool(EventLoop* mainLoop, const std::string& name);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads);  // 设置线程池中线程的数量
    void start(const ThreadInitCallback& cb = ThreadInitCallback());  // 启动线程池
    EventLoop* getNextLoop();  // 获取下一个事件循环
    std::vector<EventLoop*> getAllLoops();

private:
    EventLoop* mainLoop_;
    std::string name_;
    bool started_;
    int numThreads_;  // 保存线程池中线程数量
    std::vector<std::unique_ptr<EventLoopThread>> threads_;  // 保存线程句柄
                                                             // 每个线程都用unique_ptr管理
    std::vector<EventLoop*> loops_;  // 保存所有的EventLoop
                                     // 保存的是raw pointer
    int next_;  // 下一个返回的EventLoop索引
};




#endif  // MYMUDUO_EVENTLOOPTHREADPOLL_H