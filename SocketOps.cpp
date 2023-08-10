#include "SocketOps.h"


int createNonBlockingOrDie() {
    // Since Linux 2.6.27, the type argument serves a second purpose: in
    // addition to specifying a socket type, it may include the bitwise
    // OR of any of the following values: SOCK_NONBLOCK, SOCK_CLOEXEC
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0) {
        // 错误处理
    }
    return sockfd;
}


void bindOrDie(int sockfd, const sockaddr* addr) {
    int ret = bind(sockfd, addr, static_cast<socklen_t>(sizeof(*addr)));
    if (ret < 0) {
        // 错误处理
    }

}

void listenOrDie(int sockfd) {
    int ret = listen(sockfd, 5);
    if (ret < 0) {
        // 错误处理
    }
}

int accpet(int sockfd, struct sockaddr* addr) {
    // If flags is 0, then accept4() is the same as accept().  The
    // following values can be bitwise ORed in flags to obtain different
    // behavior: SOCK_NONBLOCK, SOCK_CLOEXEC
    socklen_t addrLen = static_cast<socklen_t>(sizeof(*addr));
    int connfd = accept4(sockfd, addr, &addrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        // 错误处理
    }

}

void shutdownWrite(int sockfd) {
    if (shutdown(sockfd, SHUT_WR) < 0) {
        // 错误处理
    }
}
