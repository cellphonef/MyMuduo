#ifndef MYMUDUO_EPOLLPOLLER_H
#define MYMUDUO_EPOLLPOLLER_H

#include <sys/epoll.h>
#include <vector>

#include "Poller.h"

// EPoll多路分发器
// 
// 
class EpollPoller: public Poller {
public:
    EpollPoller(EventLoop* loop);
    ~EpollPoller() = default;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;

private:
    void fillActiveChannel(int numEvents, ChannelList* activeChannels) const;  // 内部使用，用于
    void update(int operation, Channel* channel);  // 内部函数，用于

    int epollfd_;  // epoll文件描述符，用于epoll_xxx函数
    static const int kInitEventListSize = 16;  // 设置接收发生的事件数量，初始为16
    using EventList = std::vector<epoll_event>;
    EventList events_;  // 用于接收发生的事件，用于epoll_wait
};



#endif  // MYMUDUO_EPOLLER_H