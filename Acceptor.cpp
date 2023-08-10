#include "Acceptor.h"
#include "SocketOps.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr) 
    : loop_(loop), 
      acceptSocket_(createNonBlockingOrDie()),
      acceptChannel_(loop, acceptSocket_.fd()) {
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));

}



void Acceptor::listen() {

    acceptChannel_.enableReading();  // 将监听文件描述符注册到多路复用中

}