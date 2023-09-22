
#include "Channel.h"
#include "EventLoop.h"


#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLRDHUP;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : ownLoop_(loop),
      fd_(fd),
      events_(kNoneEvent),
      revents_(kNoneEvent),
      index_(-1) {}


void Channel::handleEvent(Timestamp receviceTime) {
    if (tied_) {
        std::shared_ptr<void> guard = tie_.lock();  // 提升
        if (guard) {  // 提升成功
            handleEventWithGuard(receviceTime);  // 能够保证handleEvent的执行过程中，TcpConnection不会析构
        }
    } else {
        handleEventWithGuard(receviceTime);
    }

    // 当handleEvent执行完后，对应的shared_ptr析构，TcpConnection析构
}

void Channel::handleEventWithGuard(Timestamp receiveTime) {

    // EPOLLRDHUP: Stream socket peer closed connection, or shut down writing half of connection.
    // EPOLLPRI: There is an exceptional condition on the file descriptor.
    // EPOLLERR: Error condition happened on the associated file descriptor.

    if ((revents_ & EPOLLRDHUP) && !(revents_ & EPOLLIN)) {
        if (closeCallback_) {
            closeCallback_();
        }
    }

    if (revents_ & EPOLLERR) {  // 错误
        if (errorCallback_) {
            errorCallback_();
        }
    }

    if (revents_ & (EPOLLPRI | EPOLLIN)) {  // 可读
        if (readCallback_) {
            readCallback_(receiveTime);
        }
    }

    if (revents_ & EPOLLOUT) {  // 可写
        if (writeCallback_) {
            writeCallback_();
        }
    }
}

void Channel::setReadCallback(ReadEventCallback cb) {
    readCallback_ = std::move(cb);
}

void Channel::setWriteCallback(EventCallback cb) {
    writeCallback_ = std::move(cb);
}

void Channel::setCloseCallback(EventCallback cb) {
    closeCallback_ = std::move(cb);
}

void Channel::setErrorCallback(EventCallback cb) {
    errorCallback_ = std::move(cb);
}

void Channel::enableReading() {
    events_ |= kReadEvent;
    ownLoop_->updateChannel(this);
}

void Channel::disableReading() {
    events_ &= ~kReadEvent;
    ownLoop_->updateChannel(this);
}

void Channel::enableWriting() {
    events_ |= kWriteEvent;
    ownLoop_->updateChannel(this);
}

void Channel::disableWriting() {
    events_ &= ~kWriteEvent;
    ownLoop_->updateChannel(this);
}

void Channel::disableAll() {
    events_ = kNoneEvent;
}

bool Channel::isWriting() const {
    return events_ & kWriteEvent;
}

bool Channel::isReading() const {
    return events_ & kReadEvent;
}

void Channel::tie(const std::shared_ptr<void>& obj) {
    tie_ = obj;
    tied_ = true;
}

void Channel::remove() {
    ownLoop_->removeChannel(this);
}


bool Channel::isNoneEvent() const {
    return events_ == kNoneEvent;
}

int Channel::events() const {
    return events_;
}

void Channel::set_revents(int revents) {
    revents_ = revents;
}

int Channel::fd() const {
    return fd_;
}

int Channel::index() const {
    return index_;
}

void Channel::set_index(int index) {
    index_ = index;
}