#include "Socket.h"
#include "InetAddress.h"
#include "SocketOps.h"

#include <unistd.h>

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
    int connfd = accpet(sockfd_, (sockaddr*)peerAddr->getSockAddr());
    
    return connfd;


}



