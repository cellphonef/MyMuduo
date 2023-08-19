#include "TcpConnection.h"

#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Logger.h"

TcpConnection::TcpConnection(EventLoop* loop, 
                             const std::string& name,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    : loop_(),
      name_(name),
      state_(kConnecting),
      socket_(),
      channel_(),
      localAddr_(localAddr),
      peerAddr_(peerAddr) {
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    socket_->setKeepAlive(true);
}


void TcpConnection::setWriteCompleteCallback(WriteCompleteCallback cb) {
    writeCompleteCallback_ = std::move(cb);
}


void TcpConnection::setConnectionCallback(ConnectionCallback cb) {
    connectionCallback_ = std::move(cb);
}

void TcpConnection::setMessageCallback(MessageCallback cb) {
    messageCallback_ = std::move(cb);
}

void TcpConnection::setCloseCallback(CloseCallback cb) {


}

void TcpConnection::handleRead(Timestamp receiveTime) {
    int savedErrno = 0;
    // 1. 从socket中读取数据到inputBuffer中
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);  
    
    // 2. 根据返回情况进行处理
    if (n > 0) {  // 正常情况
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    } else if (n == 0) {  // 被动关闭
        handleClose();
    } else {  // 错误
        errno = savedErrno;
        LOG_DEBUG("TcpConnection::handleRead");
        handleError();
    }

}

void TcpConnection::handleClose() {

    channel_->disableAll();  
    closeCallback_(shared_from_this());  // 通知上层，如TcpServer或TcpClient，该TcpConnection要关闭了

}

void TcpConnection::handleError() {
    // 错误处理
}


void TcpConnection::connectEstablished() {
    // 1. 设置状态为已连接
    setState(kConnected);
    
    // 2. 
    channel_->tie(shared_from_this());

    // 3. 开启可读事件监听
    channel_->enableReading();

    // 4. 通知用户连接建立
    connectionCallback_(shared_from_this());

}


void TcpConnection::connectDestroyed() {
    // 1. 设置状态为已断开连接
    setState(KDisConnected);

    // 关闭所有事件监听
    channel_->disableAll();

    // 通知用户连接关闭
    connectionCallback_(shared_from_this());  

    // 从EventLoop中移除
    channel_->remove();
}

void TcpConnection::send(const std::string& message) {
    loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message));
}

void TcpConnection::sendInLoop(const std::string& message) {


}


void TcpConnection::shutdown() {
    loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
}

void TcpConnection::shutdownInLoop() {
    if (!channel_->isWriting()) {  // 如果还在写，此时不能关闭，
        socket_->shutdownWrite();
    }
}

void TcpConnection::setKeepAlive(bool on) {
    socket_->setKeepAlive(on);
}

void TcpConnection::setTcpNoDelay(bool on) {
    socket_->setTcpNoDelay(on);
}