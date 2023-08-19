#include "SocketOps.h"
#include "Logger.h"

#include <unistd.h>
#include <sys/uio.h>
#include <string.h>

namespace sockets {

int createNonblockingOrDie() {
    // Since Linux 2.6.27, the type argument serves a second purpose: in
    // addition to specifying a socket type, it may include the bitwise
    // OR of any of the following values: SOCK_NONBLOCK, SOCK_CLOEXEC
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0) {
        LOG_ERROR("sockets::createNonblockingOrDie");
    }
    return sockfd;
}


void bindOrDie(int sockfd, const sockaddr* addr) {
    int ret = bind(sockfd, addr, static_cast<socklen_t>(sizeof(*addr)));
    if (ret < 0) {
        LOG_ERROR("sockets::bindOrDie");
    }

}

void listenOrDie(int sockfd) {
    int ret = listen(sockfd, 5);
    if (ret < 0) {
        LOG_ERROR("sockets::listenOrDie");
    }
}

int accpet(int sockfd, struct sockaddr* addr) {
    // If flags is 0, then accept4() is the same as accept().  The
    // following values can be bitwise ORed in flags to obtain different
    // behavior: SOCK_NONBLOCK, SOCK_CLOEXEC
    socklen_t addrLen = static_cast<socklen_t>(sizeof(*addr));
    int connfd = accept4(sockfd, addr, &addrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        LOG_ERROR("sockets::accept");
    }

}

ssize_t read(int sockfd, void* buf, size_t count) {
    return ::read(sockfd, buf, count);
}

ssize_t readv(int sockfd, const iovec* iov, int iovCount) {
    return ::readv(sockfd, iov, iovCount);

}

ssize_t write(int sockfd, const void* buf, size_t count) {
    return ::write(sockfd, buf, count);

}

void close(int sockfd) {
    if (::close(sockfd) < 0) {
        LOG_ERROR("sockets::close");
    }
}

void shutdownWrite(int sockfd) {
    if (shutdown(sockfd, SHUT_WR) < 0) {
        LOG_ERROR("sockets::shutdownWrite");
    }
}

void toIpPort(char* buf, size_t size, const sockaddr_in* addr) {
    toIp(buf, size, addr);
    size_t end = ::strlen(buf);
    uint16_t port = ::ntohs(addr->sin_port);
    snprintf(buf+end, size-end, ":%u", port);
}

void toIp(char* buf, size_t size, const sockaddr_in* addr) {
    // 利用inet_ntop进行转换
    ::inet_ntop(AF_INET, &addr->sin_addr, buf, static_cast<socklen_t>(size));
}

void fromIpPort(const char* ip, uint16_t port, sockaddr_in* addr) {
    // 利用inet_pton进行转换

}

sockaddr_in getLocalAddr(int sockfd) {
    sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    socklen_t  addrLen = static_cast<socklen_t>(sizeof(localAddr));
    if (::getsockname(sockfd, static_cast<sockaddr*>(static_cast<void*>(&localAddr)), &addrLen) < 0) {
        LOG_ERROR("sockets::getLocalAddr");
    }

    return localAddr;
}


sockaddr_in getPeerAddr(int sockfd) {
    sockaddr_in peerAddr;
    memset(&peerAddr, 0, sizeof(peerAddr));
    socklen_t  addrLen = static_cast<socklen_t>(sizeof(peerAddr));
    if (::getpeername(sockfd, static_cast<sockaddr*>(static_cast<void*>(&peerAddr)), &addrLen) < 0) {
        LOG_ERROR("sockets::getPeerAddr");
    }
    return peerAddr;
}



}  // namespace sockets


