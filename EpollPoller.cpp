#include "EpollPoller.h"
#include "Channel.h"

#include <string.h>

namespace {  // 无名命名空间，以下内容只在该文件可见

const int kNew = -1;  // channel还未添加到Poller中（默认状态）
const int kAdded = 1;  // channel已添加到Poller中
const int kDeleted = 2;  // channel已从Poller中删除
}


EpollPoller::EpollPoller(EventLoop* loop)
    : Poller(loop), 
      epollfd_(epoll_create(5)),
      events_(kInitEventListSize) {}

Timestamp EpollPoller::poll(int timeoutMs, ChannelList* activeChannels) {
    int numEvents = epoll_wait(epollfd_, &*events_.begin(), events_.size(), timeoutMs);
    Timestamp now(Timestamp::now());
    if (numEvents > 0) {  // 正常情况
        fillActiveChannel(numEvents, activeChannels);
    } else if (numEvents == 0) {  // 超时

    } else {  // 错误情况，需要判断是否会被信号中断？

    }

    return now;
}

void EpollPoller::updateChannel(Channel* channel) {
    // 更新通道，两种情况（1）可能是新增（2）也可能是更新已有（包括更新事件或删除）
    // 通过在Channel类中的index_变量来区分
    const int index = channel->index();
    if (index == kNew || index == kDeleted) {  // 新增
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    } else {  // 更新
        if (channel->isNoneEvent()) {  // 没有关注的事件，表明需要删除
            channel->set_index(kDeleted);
            update(EPOLL_CTL_DEL, channel);
        } else {  // 仍然有关注的事件，表明需要更新
            update(EPOLL_CTL_MOD, channel);
        }
    }
}


void EpollPoller::fillActiveChannel(int numEvents, ChannelList* activeChannels) const {
    for (int i = 0; i < numEvents; ++i) {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

void EpollPoller::update(int operation, Channel* channel) {
    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = channel->events();
    event.data.ptr = channel;

    int fd = channel->fd();
    int ret = epoll_ctl(epollfd_, operation, fd, &event);
    if (ret < 0) {  // 错误处理
        
    }
    
}