#ifndef MYMUDUO_CHANNEL_H
#define MYMUDUO_CHANNEL_H

#include "Timestamp.h"

#include <functional>
#include <memory>

class EventLoop;

class Channel {
public:
    using ReadEventCallback = std::function<void(Timestamp)> ;
    using EventCallback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    ~Channel() = default;

    // 处理发生的事件
    void handleEvent(Timestamp receiveTime);  
    void handleEventWithGuard(Timestamp receiveTime);  // 

    // 设置回调
    void setReadCallback(ReadEventCallback cb);
    void setWriteCallback(EventCallback cb);
    void setCloseCallback(EventCallback cb);
    void setErrorCallback(EventCallback cb);
    
    void enableReading();  // 开启可读事件监听
    void disableReading();  // 关闭可读事件监听
    void enableWriting();  // 开启可写事件监听
    void disableWriting();  // 关闭可写事件监听
    void disableAll();  // 关闭所有事件监听
    bool isWriting() const;  // 是否正在监听可写事件
    bool isReading() const;  // 是否正在监听可读事件

    void tie(const std::shared_ptr<void>&);  // 绑定

    void remove();  // 从

    // getter/setter
    bool isNoneEvent() const;  // 是否未在监听任何事件
    int events() const;  
    void set_revents(int revents);
    int fd() const;
    int index() const;
    void set_index(int index);

private:
    // 为什么不在这里直接初始化？我认为是为了减少需要包含的头文件，不在这里初始化可以不用包含epoll.h。
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    int fd_;  // 文件描述符
    int events_;  // 监听的事件
    int revents_;  // 发生的事件，由Poller设置
    int index_;  // 用于标识该Channel在Poller中的状态（包括初始状态、已添加到Poller中、已从Poller中删除）

    std::weak_ptr<void> tie_;  // 
    bool tied_;  // 是否已经绑定

    ReadEventCallback readCallback_;  // 连接可读回调
    EventCallback closeCallback_;  // 连接关闭回调
    EventCallback writeCallback_;  // 连接可写回调
    EventCallback errorCallback_;  // 连接错误回调
    EventLoop* ownLoop_;  // 所属EventLoop，每个Channel实例自始至终只属于一个EventLoop

};


#endif  // MYMUDUO_CHANNEL_H