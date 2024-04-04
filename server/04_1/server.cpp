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
  Socket *servSocket = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 8888);
  addr->bind(servSocket);
  servSocket->listen();

  Epoll *ep = new Epoll();
  servSocket->setNonBlocking();
  ep->addFd(servSocket->getFd(), EPOLLIN | EPOLLET);
  while (true) {
    std::vector<epoll_event> events = ep->poll();
    int nFds = events.size();
    for (int i = 0; i < nFds; ++i) {
      if (events[i].data.fd == servSocket->getFd()) {                // 新客户断连接
        InetAddress *clntAddr = new InetAddress();                   // 会发生内存泄漏，没有delete
        Socket *clntSock = new Socket(clntAddr->accpet(servSocket)); // 会发生内存泄漏，没有delete
        printf("new client fd %d! IP: %s Port: %d\n", clntSock->getFd(), inet_ntoa(clntAddr->m_sockAddr.sin_addr),
               ntohs(clntAddr->m_sockAddr.sin_port));
        clntSock->setNonBlocking();
        ep->addFd(clntSock->getFd(), EPOLLIN | EPOLLET);
      } else if (events[i].events & EPOLLIN) { // 可读事件
        handleReadEvent(events[i].data.fd);
      } else {
        printf("something else happend\n");
      }
    }
  }
  delete ep;
  delete addr;
  delete servSocket;
}
