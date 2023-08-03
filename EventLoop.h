#ifndef MYMUDUO_EVENTLOOP_H
#define MYMUDUO_EVENTLOOP_H

#include <vector>

#include "Channel.h"
#include "Poller.h"

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();  // 事件循环


private:
    std::vector<Channel> channelList_;  // 监听的
    Poller poller_;  // 多路分发器

};



#endif  // MYMUDUO_EVENTLOOP_H