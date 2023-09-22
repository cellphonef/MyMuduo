#ifndef MYMUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
#define MYMUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H

#include "TcpServer.h"



class EchoServer {

public:
    EchoServer(EventLoop* loop, const InetAddress& listenAddr);

    void start();


private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);


    TcpServer server_;

};


#endif  // MYMUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H