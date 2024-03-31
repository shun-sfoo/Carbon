#include <arpa/inet.h>
#include <cstdio>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

int main() {
  int sockFd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in servAddr;
  bzero(&servAddr, sizeof(servAddr));

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(8888);

  bind(sockFd, reinterpret_cast<sockaddr *>(&servAddr), sizeof(servAddr));
  listen(sockFd, SOMAXCONN);

  sockaddr_in clntAddr;
  socklen_t clntAddrLen = sizeof(clntAddr);
  bzero(&clntAddr, sizeof(clntAddr));
  int clntSockFd = accept(sockFd, reinterpret_cast<sockaddr *>(&clntAddr), &clntAddrLen);
  printf("new client fd %d! IP: %s Port %d\n", clntSockFd, inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));
}
