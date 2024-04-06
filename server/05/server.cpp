#include "Channel.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <strings.h>
#include <unistd.h>
#include <vector>

namespace {
constexpr int READ_BUFFER = 1024;
void handleReadEvent(int sockfd) {
  char buf[READ_BUFFER];
  while (true) { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      printf("message from client fd %d: %s\n", sockfd, buf);
      write(sockfd, buf, sizeof(buf));
    } else if (bytes_read == -1 && errno == EINTR) { // 客户端正常中断、继续读取
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
      printf("finish reading once, errno: %d\n", errno);
      break;
    } else if (bytes_read == 0) { // EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", sockfd);
      close(sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
      break;
    }
  }
}
} // namespace

int main() {
  Socket *servSock = new Socket();
  InetAddress *servAddr = new InetAddress("127.0.0.1", 8888);
  servSock->bind(servAddr);
  servSock->listen();
  Epoll *ep = new Epoll();
  servSock->setNonBlocking();
  Channel *servChannel = new Channel(ep, servSock->getFd());
  servChannel->enableReading();
  while (true) {
    std::vector<Channel *> activeChannels = ep->poll();
    int nFds = activeChannels.size();
    for (int i = 0; i < nFds; ++i) {
      int chfd = activeChannels[i]->getFd();
      if (chfd == servSock->getFd()) {                             // 新客户端连接
        InetAddress *clntAddr = new InetAddress();                 // 会发生内存泄露！没有delete
        Socket *clntSock = new Socket(servSock->accept(clntAddr)); // 会发生内存泄露！没有delete
        printf("new client fd %d! IP: %s Port: %d\n", clntSock->getFd(), inet_ntoa(clntAddr->m_sockAddr.sin_addr),
               ntohs(clntAddr->m_sockAddr.sin_port));
        clntSock->setNonBlocking();
        Channel *clntChannel = new Channel(ep, clntSock->getFd());
        clntChannel->enableReading();
      } else if (activeChannels[i]->getRevents() & EPOLLIN) { // 可读事件
        handleReadEvent(activeChannels[i]->getFd());
      } else { // 其他事件，之后的版本实现
        printf("something else happened\n");
      }
    }
  }
  delete servSock;
  delete servAddr;
  return 0;
}
