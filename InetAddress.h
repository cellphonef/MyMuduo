#ifndef MYMUDUO_INETADDRESS_H
#define MYMUDUO_INETADDRESS_H

#include <string>
#include <netinet/in.h>  // for sockaddr_in

// IPv4地址的包装类
//
// sockaddr是通用的地址结构
// 而sockaddr_in是IPv4的专用结构，可以转换成sockaddr
class InetAddress {
public:
    InetAddress() = default;
    explicit InetAddress(uint16_t port, const std::string& ip = "127.0.0.1");
    explicit InetAddress(const sockaddr_in& addr);
    ~InetAddress() = default;

    const sockaddr_in* getSockAddr() const;
    void setSockAddr(const sockaddr_in& addr);

private:
    sockaddr_in addr_;

};



#endif  // MYMUDUO_INETADDRESS_H