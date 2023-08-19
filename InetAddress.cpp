#include "InetAddress.h"
#include "SocketOps.h"

#include <string.h>  // for memset
#include <arpa/inet.h>  // for inet_addr

InetAddress::InetAddress(uint16_t port, const std::string& ip) {
    memset(&addr_, 0, sizeof(addr_));

    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
}

InetAddress::InetAddress(const sockaddr_in& addr)
    : addr_(addr) {}

const sockaddr_in* InetAddress::getSockAddr() const {
    return &addr_;
}

void InetAddress::setSockAddr(const sockaddr_in& addr) {
    addr_ = addr;
}

std::string InetAddress::toIp() const {
    char buf[64] = "";
    sockets::toIp(buf, sizeof(buf), getSockAddr());
    return buf;
}

std::string InetAddress::toIpPort() const {
    char buf[64] = "";
    sockets::toIpPort(buf, sizeof(buf), getSockAddr());
    return buf;
}