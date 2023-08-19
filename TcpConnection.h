#ifndef MUMUDUO_TCPCONNECTION_H
#define MUMUDUO_TCPCONNECTION_H

#include "Callback.h"
#include "InetAddress.h"
#include "Buffer.h"

#include <memory>
#include <string>


class EventLoop;
class Channel;
class Socket;

// 表示一次Tcp连接 
//
//
class TcpConnection: public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop* loop, 
                  const std::string& name,
                  int sockfd,
                  const InetAddress& localAddr,
                  const InetAddress& peerAddr);
    ~TcpConnection();


    // getter
    EventLoop* getLoop() const;
    const std::string& name() const;
    const InetAddress& localAddress() const;
    const InetAddress& peerAddress() const;
    Buffer* inputBuffer();
    Buffer* outputBuffer();

    // setter 
    // 设置回调
    void setConnectionCallback(ConnectionCallback cb);
    void setMessageCallback(MessageCallback cb);
    void setWriteCompleteCallback(WriteCompleteCallback cb);
    void setCloseCallback(CloseCallback cb);
    
    // 设置连接套接字选项
    void setTcpNoDelay(bool on);
    void setKeepAlive(bool on);


    enum StateE {
        KDisConnected,
        kConnecting,
        kConnected,
        kDisConnecting
    };
    

    void setState(StateE state);

    // 发送数据，可跨线程调用
    void send(const void* message, size_t len);  // 任意类型
    void send(const std::string& message);  // string类型

    // 关闭，可跨线程调用
    void shutdown();

    
    bool connected() const;  // 判断是否已经完成连接


    void connectEstablished();  // 需要调用用户回调ConnectionCallback（如有），通知连接建立
    void connectDestroyed();  // 需要调用用户回调ConnectionCallback（如有），通知连接断开（如有）


private:
     // 处理事件
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void sendInLoop(const std::string& message); // send具体实现，确保线程安全
    void shutdownInLoop();  // shutdown的具体实现，确保线程安全
   
    StateE state_;

    EventLoop* loop_;  // 所属的Eventloop
    std::unique_ptr<Socket> socket_;  // 用于管理该连接对应的套接字生命周期
    std::unique_ptr<Channel> channel_;  // 用于管理该连接对应的事件以及事件处理
                                        // ？？？疑问：为什么不直接保存对象？
    
    std::string name_;
 
    InetAddress localAddr_;  // 本端地址
    InetAddress peerAddr_;  // 远程地址

    ConnectionCallback connectionCallback_;  // 连接建立或断开的用户回调，从Accetor继承
    MessageCallback messageCallback_;  // 连接可读的用户回调，从Acceptor继承
    WriteCompleteCallback writeCompleteCallback_;  // ？？？
    CloseCallback closeCallback_;  // 内部使用，用于通知上层连接关闭

    Buffer inputBuffer_;  // 输入缓冲区，缓存连接待处理数据避免阻塞
    Buffer outputBuffer_;  // 输出缓冲区，缓存连接待发送数据避免阻塞

};



#endif  // MUMUDUO_TCPCONNECTION_H