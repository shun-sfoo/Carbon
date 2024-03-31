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

  errif(connect(sockFd, reinterpret_cast<sockaddr *>(&servAddr), sizeof(servAddr)), "socket connect error");

  while (true) {
    char buf[1024];                                       // 定义缓冲区
    bzero(&buf, sizeof(buf));                             // 清空缓冲区
    scanf("%s", buf);                                     // 从键盘输入要传到服务器的数据
    ssize_t writeBytes = write(sockFd, buf, sizeof(buf)); // 发送缓冲区中的数据到服务器socket，返回已发送数据大小
    if (writeBytes == -1) {                               // write返回-1，表示发生错误
      printf("socket already disconnected, can't write any more!\n");
      break;
    }
    bzero(&buf, sizeof(buf));                           // 清空缓冲区
    ssize_t readBytes = read(sockFd, buf, sizeof(buf)); // 从服务器socket读到缓冲区，返回已读数据大小
    if (readBytes > 0) {
      printf("message form server: %s\n", buf);
    } else if (readBytes == 0) { // read返回0，表示EOF，通常是服务器断开链接，等会儿进行测试
      printf("server socket disconnected!\n");
      break;
    } else if (readBytes == -1) { // read返回-1，表示发生错误，按照上文方法进行错误处理)
      close(sockFd);
      errif(true, "socket read error");
    }
  }

  close(sockFd);
  return 0;
}
