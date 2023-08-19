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
      nextConnId_(1),
      acceptor_(new Acceptor(loop, listenAddr, true)),
      threadPool_(new EventLoopThreadPool(loop, name_)) {
    
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, 
                                       std::placeholders::_1, std::placeholders::_2));
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
    threadPool_->start(threadInitCallback_);
    mainLoop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::setThreadNum(int numThreads) {
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::setThreadInitCallback(const ThreadInitCallback& cb) {
    threadInitCallback_ = cb;
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr) {
    // 1. 选择该连接所属的EventLoop，并设置名字
    EventLoop* ioLoop = threadPool_->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof(buf), "-%s#%d", ipPort_.c_str(), nextConnId_);  // -ip:port#1
    ++nextConnId_;
    std::string connName = name_ + buf;  // 最终的连接名称为：serverName-ip:port#1

    InetAddress localAddr(sockets::getLocalAddr(sockfd));

    // 1. 创建一个新的TcpConnection并用shared_ptr进行管理，同时添加到连接集合中
    TcpConnectionPtr conn(
        new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;

    // 2. 设置回调，TcpServer负责设置
    conn->setConnectionCallback(connectionCallback_);  // 将用户回调同步给TcpConneciton
    conn->setMessageCallback(messageCallback_);  // 将用户回调同步给TcpConneciton
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, 
                           this, std::placeholders::_1));  // 设置连接关闭时，通知TcpServer的回调

    // 3. 最后在TcpConnection所属的EventLoop中调用connectEstablished，完成TcpConnection自己完成的一些工作
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));

}

void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
    mainLoop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

