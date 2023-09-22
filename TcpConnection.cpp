#include "TcpConnection.h"

#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Logger.h"
#include "SocketOps.h"


TcpConnection::TcpConnection(EventLoop* loop, 
                             const std::string& name,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    : loop_(loop),
      name_(name),
      state_(StateE::kConnecting),
      socket_(new Socket(sockfd)),
      channel_(new Channel(loop, sockfd)),
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
    closeCallback_ = std::move(cb);
}

void TcpConnection::setHighWaterMarkCallback(HighWaterMarkCallback cb) {
    highWaterMarkCallback_ = std::move(cb);
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

// 调用该函数，说明触发了可写事件
void TcpConnection::handleWrite() {
    if (channel_->isWriting()) {  // 正在监听写事件，说明还有数据未发送完毕
        // 1. 写数据
        int savedErrno = 0;
        ssize_t n = outputBuffer_.writeFd(channel_->fd(), &savedErrno);
        
        if (n > 0) {  // 正常情况
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {  // 所有数据都发送完毕
                channel_->disableWriting();  // 关闭写监听，由于采用LT模式避免频繁触发
                if (writeCompleteCallback_) {  // 回调写完成回调函数
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));  // 为什么要queueInLoop
                }
                if (state_ == StateE::kDisConnecting) {  // 如果处于这个状态，表示需要关闭此连接
                    shutdownInLoop();
                }
            }
        } else {  // 错误情况
            LOG_ERROR("TcpConenction::handleWrite");
        }
    } else {  // ？
        LOG_ERROR("");
    }
}

// 调用该函数，说明连接被动关闭
void TcpConnection::handleClose() {
    channel_->disableAll();  // 取消所有监听的事件
    // connectionCallback_(shared_from_this());  // 这里也调用通知用户连接关闭？
    closeCallback_(shared_from_this());  // 通知上层，如TcpServer或TcpClient，该TcpConnection要关闭了
                                         // 注：调用该回调函数的线程是当前TcpConnection的所属的线程
}

// 调用该函数，说明触发了错误
void TcpConnection::handleError() {
    
}


void TcpConnection::connectEstablished() {
    setState(StateE::kConnected);  // 设置状态为已连接
    channel_->tie(shared_from_this());  // channel绑定该TcpConnection
    channel_->enableReading();  // 开启可读事件监听
    connectionCallback_(shared_from_this());  // 通知用户连接建立
}

void TcpConnection::connectDestroyed() {
    setState(StateE::KDisConnected);  // 设置状态为已断开连接
    channel_->disableAll();  // 关闭所有事件监听
    connectionCallback_(shared_from_this());   // 通知用户连接关闭（第二次调用？）
    channel_->remove();  // 从EventLoop中移除

}

void TcpConnection::send(const std::string& message) {
    loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message));  
}

void TcpConnection::sendInLoop(const std::string& message) {


}


void TcpConnection::shutdown() {
    if (state_ == StateE::kConnected) {
        setState(StateE::kDisConnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop() {
    // 如果还在监听写事件，代表数据还没发送完毕，此时不能关闭，应该发送完数据再关闭
    // 什么时候数据发完？数据发没发送完只有handleWrite知道，因此此时我们应该在handleWrite中来处理shutdown
    if (!channel_->isWriting()) {  
        socket_->shutdownWrite();
    }
}

void TcpConnection::setKeepAlive(bool on) {
    socket_->setKeepAlive(on);
}

void TcpConnection::setTcpNoDelay(bool on) {
    socket_->setTcpNoDelay(on);
}

bool TcpConnection::connected() const {
    return state_ == StateE::kConnected;
}