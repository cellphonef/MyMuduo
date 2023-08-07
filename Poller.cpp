#include "Poller.h"
#include "EventLoop.h"


Poller::Poller(EventLoop* loop)
    : ownLoop_(loop) {}

void Poller::assertNonInLoopThread() const {
    ownLoop_->assertInLoopThread();
}