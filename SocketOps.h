#ifndef MYMUDUO_SOCKETOPS_H
#define MYMUDUO_SOCKETOPS_H

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>


// 该命名空间将常用的socket API进行的整合
namespace sockets {

// 监听套接字相关操作
int createNonBlockingOrDie();
void bindOrDie(int sockfd, const sockaddr* addr);
void listenOrDie(int sockfd);
int accpet(int sockfd, struct sockaddr* addr);

// 读写数据
ssize_t read(int sockfd, void* buf, size_t count);
ssize_t readv(int sockfd, const iovec* iov, int iovCount);
ssize_t write(int sockfd, const void* buf, size_t count);
void close(int sockfd);

void shutdownWrite(int sockfd);

// 将sockaddr地址与成字符串之间转换
void toIpPort(char* buf, size_t size,
              const sockaddr_in* addr);
void toIp(char* buf, size_t size,
          const sockaddr_in* addr);
void fromIpPort(const char* ip, uint16_t port,
                sockaddr_in* addr);

sockaddr_in getLocalAddr(int sockfd);
sockaddr_in getPeerAddr(int sockfd);


}  // namespace sockets


#endif  // MYMUDUO_SOCKETOPS_H