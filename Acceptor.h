#ifndef MYMUDUO_ACCEPTOR_H
#define MYMUDUO_ACCEPTOR_H

#include "Channel.h"
#include "Socket.h"

class EventLoop;
class InetAddress;

// 连接接收器

class Acceptor {
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;
    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reusePort);
    ~Acceptor();

    void listen();
    void setNewConnectionCallback(const NewConnectionCallback& cb);

private:
    void handleRead();  // 可读回调

    EventLoop* loop_;  // 所属EventLoop
    Socket acceptSocket_;  // 拥有监听文件描述符
    Channel acceptChannel_;  // 管理监听文件描述符以及可读事件
    NewConnectionCallback newConnectionCallback_;  // 新连接到来时的回调函数，用于通知上层如TcpServer，由TcpServer设置
    int idleFd_;  // 空文件描述符，用于处理文件描述符耗尽的情况






};


#endif  // MYMUDU_ACCEPTOR_H