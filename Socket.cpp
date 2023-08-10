#include "Socket.h"
#include "InetAddress.h"

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
    
}

void Socket::listen() {

}



int Socket::accept(InetAddress* peerAddr) {

}



