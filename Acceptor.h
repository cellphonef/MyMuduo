#ifndef MYMUDUO_ACCEPTOR_H
#define MYMUDUO_ACCEPTOR_H

#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"


// 连接接收器

class Acceptor {
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;
    Acceptor(EventLoop* loop, const InetAddress& listenAddr);
    ~Acceptor();

    void listen();
    void setNewConnectionCallback(const NewConnectionCallback& cb);

private:
    void handleRead();  // 可读回调

    EventLoop* loop_;  // 所属EventLoop
    Socket acceptSocket_;  // 拥有监听文件描述符
    Channel acceptChannel_;  // 管理监听描述符以及可读事件






};


#endif  // MYMUDU_ACCEPTOR_H