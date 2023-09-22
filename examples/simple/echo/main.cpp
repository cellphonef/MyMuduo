#include "examples/simple/echo/echo.h"

#include "EventLoop.h"
#include "InetAddress.h"

int main() {

    EventLoop loop;
    InetAddress listenAddr(12345);
    EchoServer server(&loop, listenAddr);
    server.start();  // 启动
    loop.loop();  // 开启事件循环
}