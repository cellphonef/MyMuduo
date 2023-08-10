#ifndef MYMUDUO_SOCKETOPS_H
#define MYMUDUO_SOCKETOPS_H

#include <sys/types.h>
#include <sys/socket.h>

int createNonBlockingOrDie();

void bindOrDie(int sockfd, const sockaddr* addr);

void listenOrDie(int sockfd);

int accpet(int sockfd, struct sockaddr* addr);

void shutdownWrite(int sockfd);


#endif  // MYMUDUO_SOCKETOPS_H