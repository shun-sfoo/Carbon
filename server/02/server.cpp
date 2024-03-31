#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <unistd.h>

void errif(bool condition, const char *errmsg) {
  if (condition) {
    perror(errmsg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  int sockFd = socket(AF_INET, SOCK_STREAM, 0);
  errif(sockFd == -1, "socket create error");

  sockaddr_in servAddr;
  bzero(&servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(8888);

  errif(bind(sockFd, reinterpret_cast<sockaddr *>(&servAddr), sizeof(servAddr)) == -1, "socket bind error");

  errif(listen(sockFd, SOMAXCONN) == -1, "socket listen error");

  sockaddr_in clntAddr;
  socklen_t clntAddrLen = sizeof(clntAddr);
  bzero(&clntAddr, sizeof(clntAddr));
  int clntSockFd = accept(sockFd, reinterpret_cast<sockaddr *>(&clntAddr), &clntAddrLen);
  errif(clntSockFd == -1, "socket accept error");

  printf("new client fd %d! IP: %s Port: %d\n", clntSockFd, inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));

  while (true) {
    char buf[1024];                                         // 定义缓冲区
    bzero(&buf, sizeof(buf));                               // 清空缓冲区
    ssize_t readBytes = read(clntSockFd, buf, sizeof(buf)); // 从客户端socket读到缓冲区，返回已读数据大小
    if (readBytes > 0) {
      printf("message from client fd %d: %s\n", clntSockFd, buf);
      write(clntSockFd, buf, sizeof(buf)); // 将相同的数据写回到客户端
    } else if (readBytes == 0) {           // read返回0，表示EOF
      printf("client fd %d disconnected\n", clntSockFd);
      close(clntSockFd);
      break;
    } else if (readBytes == -1) { // read返回-1，表示发生错误，按照上文方法进行错误处理
      close(clntSockFd);
      errif(true, "socket read error");
    }
  }

  close(sockFd);
  return 0;
}
