#ifndef MYMUDUO_TCPSERVER_H
#define MYMUDUO_TCPSERVER_H

#include "Acceptor.h"

class TcpServer {
public:
    TcpServer(/* args */);
    ~TcpServer();

private:
    Acceptor acceptor_;

};


#endif  // MYMUDUO_TCPSERVER_H