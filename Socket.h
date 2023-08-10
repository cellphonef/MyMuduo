#ifndef MYMUDUO_SOCKET_H
#define MYMUDUO_SOCKET_H

class InetAddress;

// 文件描述符的包装类
// 
// 该类拥有文件描述符fd，其析构时会关闭对应的文件描述符
// 对于TCP网络编程中一共就两类fd，一类是监听fd，另一类是连接fd
// 该类封装了监听fd
class Socket {
public:
    explicit Socket(int sockfd);
    ~Socket();

    void bindAddress(const InetAddress& localAddr);
    void listen();
    int accept(InetAddress* peerAddr);

    int fd() const;


private:
    const int sockfd_;


};
#endif  // MYMUDUO_SOCKET_H