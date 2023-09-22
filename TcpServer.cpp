#include "TcpServer.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "SocketOps.h"

#include <stdio.h>  // for snprintf 


TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr,
                     const std::string& name, Option option)
    : mainLoop_(loop),
      ipPort_(listenAddr.toIpPort()),
      name_(name),
      acceptor_(new Acceptor(loop, listenAddr, true)),
      threadPool_(new EventLoopThreadPool(loop, name_)),
      nextConnId_(1) {
    
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, 
                                        std::placeholders::_1, std::placeholders::_2));  // 设置acceptor新连接建立回调
}

TcpServer::~TcpServer() {
    for (auto& item : connections_) {
        TcpConnectionPtr conn(item.second);  // 拷贝一份，离开作用域后释放
        item.second.reset();  // 释放原来的

        // conn参数拷贝一份，使TcpConnection的生命周期能够延长到connectDestoryed执行完
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));  
    }
}

void TcpServer::setConnectionCallback(const ConnectionCallback& cb) {
    connectionCallback_ = cb;
}

void TcpServer::setMessageCallback(const MessageCallback& cb) {
    messageCallback_ = cb;
}

void TcpServer::setWriteCompleteCallback(const WriteCompleteCallback& cb) {
    writeCompleteCallback_ = cb;
}

void TcpServer::start() {
    threadPool_->start(threadInitCallback_);  // 启动subLoop
    mainLoop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::setThreadNum(int numThreads) {
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::setThreadInitCallback(const ThreadInitCallback& cb) {
    threadInitCallback_ = cb;
}

// 注：该函数通常在mainLoop执行
void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr) {
    // 准备TcpConnection的构造参数，包括连接所属的EventLoop、连接的名字、连接的本端和对端地址
    EventLoop* ioLoop = threadPool_->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof(buf), "-%s#%d", ipPort_.c_str(), nextConnId_);  // -ip:port#1
    ++nextConnId_;
    std::string connName = name_ + buf;  // 最终的连接名称为：serverName-ip:port#1
    InetAddress localAddr(sockets::getLocalAddr(sockfd));

    // 创建一个新的TcpConnection并用shared_ptr进行管理，同时添加到连接集合中
    TcpConnectionPtr conn(
        new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;  

    // 设置回调，TcpConnection的上层TcpServer负责设置
    conn->setConnectionCallback(connectionCallback_);  // 将用户回调同步给TcpConneciton
    conn->setMessageCallback(messageCallback_);  // 将用户回调同步给TcpConneciton
    conn->setWriteCompleteCallback(writeCompleteCallback_);  // 将用户回调同步给TcpConnection
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, 
                           this, std::placeholders::_1));  // 设置TcpConnection关闭时，用于通知TcpServer的回调

    // 最后在TcpConnection所属的EventLoop中调用connectEstablished
    // 此时处在mainLoop所在的线程，而TcpConnection会分配到subLoop，
    // 以完成TcpConnection自己可以完成的创建收尾工作
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));

}

// 注：该函数由TcpConnection所在的EventLoop对应的线程执行
void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
    // FIXME: unsafe why?
    mainLoop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));  // 转到mainLoop_执行
}

// 注：该函数在mainLoop执行
void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn) {
    connections_.erase(conn->name());  // 删除保存的连接

    // 回到TcpConnection所在的线程执行收尾工作
    conn->getLoop()->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));  
}

std::shared_ptr<EventLoopThreadPool> TcpServer::threadPool() {
    return threadPool_;
}

EventLoop* TcpServer::getLoop() const {
    return mainLoop_;
}

const std::string& TcpServer::ipPort() const {
    return ipPort_;
}

const std::string& TcpServer::name() const { 
    return name_;
}