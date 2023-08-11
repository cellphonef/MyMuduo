#include "Socket.h"
#include "InetAddress.h"
#include "SocketOps.h"

#include <unistd.h>
#include <string.h>  // for memset
#include <netinet/in.h>  // for IPPROTO_xxx
#include <netinet/tcp.h>  // for TCP_xxx

Socket::Socket(int sockfd)
    : sockfd_(sockfd) {}

Socket::~Socket() {
    close(sockfd_);
}

int Socket::fd() const {
    return sockfd_;
}

void Socket::bindAddress(const InetAddress& localAddr) {
    bindOrDie(sockfd_, (sockaddr*)localAddr.getSockAddr());  // FIXME: use implicit_cast
}

void Socket::listen() {
    listenOrDie(sockfd_);
}


int Socket::accept(InetAddress* peerAddr) {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    int connfd = accpet(sockfd_, (sockaddr*)&addr);
    if (connfd > 0) {
        peerAddr->setSockAddr(addr);
    }
    
    return connfd;

}


void Socket::shutdownWrite() {
    ::shutdownWrite(sockfd_);
}

void Socket::setReuseAddr(bool on) {
    int optval = on? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval)));

    // 错误处理
}


void Socket::setReusePort(bool on) {
    int optval = on? 1 : 0;
    // >=Linux 3.9才支持SO_REUSEPORT
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof(optval)));

}


void Socket::setKeepAlive(bool on) {
    int optval = on? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof(optval)));

}

void Socket::setTcpNoDelay(bool on) {
    int optval = on? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof(optval)));
}


