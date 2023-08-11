#include "../InetAddress.h"
#include "../EventLoop.h"
#include "../Acceptor.h"

#include <stdio.h>
#include <unistd.h>

void newConnection(int sockfd, const InetAddress& peerAddr) {
    printf("newConnection(): accepted a new connection from %d\n", sockfd);
    write(sockfd, "how are you?\n", 13);
    close(sockfd);
}


int main() {
    printf("main(): pid = %d\n", getpid());
    
    InetAddress listenAddr(9981);
    EventLoop loop;

    Acceptor acceptor(&loop, listenAddr, true);
    acceptor.setNewConnectionCallback(newConnection);
    acceptor.listen();
    loop.loop();

}