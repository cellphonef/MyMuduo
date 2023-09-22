#ifndef MYMUDUO_TCPSERVER_H
#define MYMUDUO_TCPSERVER_H

#include "TcpConnection.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <memory>

class EventLoop;
class Acceptor;
class EventLoopThreadPool;


class TcpServer {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    enum class Option {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop* loop, const InetAddress& listenAddr,
              const std::string& name, Option option = Option::kReusePort);  // 
    ~TcpServer();

    void setConnectionCallback(const ConnectionCallback& cb);  // 设置连接建立和结束回调
    void setMessageCallback(const MessageCallback& cb);  // 设置消息到达回调
    void setWriteCompleteCallback(const WriteCompleteCallback& cb);  // 设置写完成回调
    void start();  // 启动Tcp server，本质上就是启动mainReactor以及subReactor

    EventLoop* getLoop() const;
    const std::string& ipPort() const;
    const std::string& name() const;
    std::shared_ptr<EventLoopThreadPool> threadPool();


    void setThreadNum(int numThreads);  // 设置线程池数量，本质上就是subReactor数量
    void setThreadInitCallback(const ThreadInitCallback& cb);  // 设置每个Reactor初始化函数


private:
    void newConnection(int sockfd, const InetAddress& peerAddr);  // Acceptor回调该函数，用于通知TcpServer处理连接到来
    void removeConnection(const TcpConnectionPtr& conn);  // TcpConnection回调该函数，用于通知TcpServer处理连接关闭
    void removeConnectionInLoop(const TcpConnectionPtr& conn);
    
    using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;

    EventLoop* mainLoop_;  // mainLoop，即acceptor所在的EventLoop
    const std::string ipPort_;  // 服务器监听的ip以及端口号
    const std::string name_;  // 服务器名称
    std::unique_ptr<Acceptor> acceptor_;  // 接收器，用于接收连接，
    
    std::shared_ptr<EventLoopThreadPool> threadPool_;  // subLoop所在的线程池

    MessageCallback messageCallback_;  // 保存用户连接消息到达回调，当连接消息到达时需要回调该函数
                                       // 由上层设置，并最终传递到TcpConnection类中，由TcpConnection来执行
    ConnectionCallback connectionCallback_;  // 保存用户连接建立和关闭回调，当连接建立或关闭时需要回调该函数
                                             // 由上层设置，并最终传递到TcpConnection类中，由TcpConnection来执行
    WriteCompleteCallback writeCompleteCallback_;  // 保存用户写完成回调，当写完成时需要回调该函数
                                                   // 由上层设置，并最终传递到TcpConnection类中，由TcpConnection来执行
    ThreadInitCallback threadInitCallback_;  // 线程初始化回调
                                             // 由上层设置，并逐层传递到最终的Thread类，由Thread类来执行
                                             // YourServer->TcpServer->EventLoopThreadPool->EventLoopThread

    int nextConnId_;  // 下一个Tcp连接编号    

    ConnectionMap connections_;  // 保存服务端所有连接，连接名称<-->连接




};


#endif  // MYMUDUO_TCPSERVER_H