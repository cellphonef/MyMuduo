#ifndef MYMUDUO_POLLER_H
#define MYMUDUO_POLLER_H

#include <vector>

#include "Timestamp.h"

#include <map>

class EventLoop;
class Channel;

class Poller {
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    virtual ~Poller() = default;
    virtual Timestamp poll(int timeoutMs, ChannelList* activeChannel) = 0;
    virtual void updateChannel(Channel*) = 0;
    virtual void removeChannel(Channel*) = 0;

    void assertNonInLoopThread() const;  // 什么类需要这个方法？

protected:
    using ChannelMap = std::map<int, Channel*>;
    ChannelMap channels_;  // 保存fd到Channel的映射

private:
    EventLoop* ownLoop_;  // 所属的EventLoop
    

};



#endif  // MYMUDUO_POLLER_H