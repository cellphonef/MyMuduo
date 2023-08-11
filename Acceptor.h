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
    Channel acceptChannel_;  // 管理监听描述符以及可读事件
    NewConnectionCallback newConnectionCallback_;






};


#endif  // MYMUDU_ACCEPTOR_H