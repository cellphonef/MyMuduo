#include "Acceptor.h"
#include "SocketOps.h"
#include "InetAddress.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reusePort) 
    : loop_(loop), 
      acceptSocket_(sockets::createNonBlockingOrDie()),
      acceptChannel_(loop, acceptSocket_.fd()) {
    acceptSocket_.setReuseAddr(true); 
    acceptSocket_.setReusePort(reusePort);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));

}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();

}

void Acceptor::listen() {
    acceptSocket_.listen();
    acceptChannel_.enableReading();  // 将监听文件描述符注册到多路复用中

}

void Acceptor::handleRead() {
    InetAddress peerAddr;

    int connfd = acceptSocket_.accept(&peerAddr);  // FIXME: 考虑文件描述符耗尽的情况

    if (connfd > 0) {
        newConnectionCallback_(connfd, peerAddr);  // FIXME: 直接传递int不理想
    }
    else {
        // 错误处理
    }
}

void Acceptor::setNewConnectionCallback(const NewConnectionCallback& cb) {
    newConnectionCallback_ = cb;
}