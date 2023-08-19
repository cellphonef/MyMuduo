#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // 绑定本地地址
    sockaddr_in localAddress;
    localAddress.sin_family = AF_INET;
    localAddress.sin_addr.s_addr = INADDR_ANY;
    localAddress.sin_port = htons(8888);  // 指定一个端口号

    if (bind(sockfd, (struct sockaddr*)&localAddress, sizeof(localAddress)) == -1) {
        std::cerr << "Failed to bind socket." << std::endl;
        close(sockfd);
        return 1;
    }

    // 开始监听
    if (listen(sockfd, 5) == -1) {
        std::cerr << "Failed to listen on socket." << std::endl;
        close(sockfd);
        return 1;
    }

    // 接受连接
    int clientSock = accept(sockfd, nullptr, nullptr);
    if (clientSock == -1) {
        std::cerr << "Failed to accept connection." << std::endl;
        close(sockfd);
        return 1;
    }

    // 获取本地地址
    sockaddr_in localAddr;
    socklen_t localAddrLen = sizeof(localAddr);
    if (getsockname(clientSock, (struct sockaddr*)&localAddr, &localAddrLen) == -1) {
        std::cerr << "Failed to get local address." << std::endl;
        close(clientSock);
        close(sockfd);
        return 1;
    }

    // 获取远程地址
    sockaddr_in remoteAddr;
    socklen_t remoteAddrLen = sizeof(remoteAddr);
    if (getpeername(clientSock, (struct sockaddr*)&remoteAddr, &remoteAddrLen) == -1) {
        std::cerr << "Failed to get remote address." << std::endl;
        close(clientSock);
        close(sockfd);
        return 1;
    }

    // 打印地址信息
    char localIP[INET_ADDRSTRLEN];
    char remoteIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(localAddr.sin_addr), localIP, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(remoteAddr.sin_addr), remoteIP, INET_ADDRSTRLEN);

    std::cout << "Local Address: " << localIP << ":" << ntohs(localAddr.sin_port) << std::endl;
    std::cout << "Remote Address: " << remoteIP << ":" << ntohs(remoteAddr.sin_port) << std::endl;

    // 关闭套接字
    close(clientSock);
    close(sockfd);

    return 0;
}