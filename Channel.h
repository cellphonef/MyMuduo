#ifndef MYMUDUO_CHANNEL_H
#define MYMUDUO_CHANNEL_H

#include <functional>

class EventLoop;

class Channel {
public:
    using ReadEventCallback = std::function<void()> ;
    using EventCallback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent();  // 处理发生的事件
    void setReadCallback(ReadEventCallback cb);
    void setWriteCallback();
    void setCloseCallback();
    void setErrorCallback();
    
    void enableReading();  // 
    void disableReading();  //
    void enableWriting();  // 
    void disableWriting();  //
    void disableAll();  // 

    bool isNoneEvent() const;
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
    ReadEventCallback readCallback_;  // 连接可读回调
    EventCallback closeCallback_;  // 连接关闭回调
    EventCallback writeCallback_;  // 连接可写回调
    EventCallback errorCallback_;  // 连接错误回调
    EventLoop* ownLoop_;  // 所属EventLoop，每个Channel实例自始至终只属于一个EventLoop

};




#endif  // MYMUDUO_CHANNEL_H