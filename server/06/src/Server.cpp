#include "Server.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <strings.h>
#include <unistd.h>

namespace {
constexpr int READ_BUFFER = 1024;
} // namespace

Server::Server(EventLoop *lLoop) : m_loop(lLoop) {
  Socket *servSock = new Socket();
  InetAddress *servAddr = new InetAddress("127.0.0.1", 8888);
  servSock->bind(servAddr);
  servSock->listen();
  servSock->setNonBlocking();

  Channel *servChannel = new Channel(m_loop, servSock->getFd());
  std::function<void()> cb = std::bind(&Server::newConnection, this, servSock);
  servChannel->setCallback(cb);
  servChannel->enableReading();
}

Server::~Server() {}

void Server::handleReadEvent(int sockfd) {
  char buf[READ_BUFFER];
  while (true) { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    bzero(&buf, sizeof(buf));
    ssize_t bytesRead = read(sockfd, buf, sizeof(buf));
    if (bytesRead > 0) {
      printf("message from client fd %d: %s\n", sockfd, buf);
      write(sockfd, buf, sizeof(buf));
    } else if (bytesRead == -1 && errno == EINTR) { // 客户端正常中断、继续读取
      printf("continue reading");
      continue;
    } else if (bytesRead == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
      printf("finish reading once, errno: %d\n", errno);
      break;
    } else if (bytesRead == 0) { // EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", sockfd);
      close(sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
      break;
    }
  }
}

void Server::newConnection(Socket *servSock) {
  InetAddress *clntAddr = new InetAddress();                 // 会发生内存泄露！没有delete
  Socket *clntSock = new Socket(servSock->accept(clntAddr)); // 会发生内存泄露！没有delete
  printf("new client fd %d! IP: %s Port: %d\n", clntSock->getFd(), inet_ntoa(clntAddr->m_sockAddr.sin_addr),
         ntohs(clntAddr->m_sockAddr.sin_port));
  clntSock->setNonBlocking();
  Channel *clntChannel = new Channel(m_loop, clntSock->getFd());
  std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clntSock->getFd());
  clntChannel->setCallback(cb);
  clntChannel->enableReading();
}
