#include "utils.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace {
constexpr int MAX_EVENTS = 1024;
constexpr int READ_BUFFER = 1024;
} // namespace

void setnonblocking(int fd) { fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK); }

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

  int epFd = epoll_create1(0);
  errif(epFd == -1, "epoll create error");

  epoll_event events[MAX_EVENTS], ev;
  bzero(&events, sizeof(events));
  bzero(&ev, sizeof(ev));

  ev.data.fd = sockFd; // 该IO口为服务器socket fd
  ev.events = EPOLLIN | EPOLLET;
  setnonblocking(sockFd);
  epoll_ctl(epFd, EPOLL_CTL_ADD, sockFd, &ev); // 将服务器socket fd添加到epoll

  while (true) {                                         // 不断监听epoll上的事件并处理
    int nFds = epoll_wait(epFd, events, MAX_EVENTS, -1); // 有nfds个fd发生事件
    errif(nFds == -1, "epoll wait error");
    for (int i = 0; i < nFds; ++i) {     // 处理这nfds个事件
      if (events[i].data.fd == sockFd) { // 发生事件的fd是服务器socket fd，表示有新客户端连接
        sockaddr_in clntAddr;
        bzero(&clntAddr, sizeof(clntAddr));
        socklen_t clntAddrLen = sizeof(clntAddr);

        int clntSockFd = accept(sockFd, reinterpret_cast<sockaddr *>(&clntAddr), &clntAddrLen);
        errif(clntSockFd == -1, "socket accept error");
        printf("new client fd %d! IP: %s Port: %d\n", clntSockFd, inet_ntoa(clntAddr.sin_addr),
               ntohs(clntAddr.sin_port));

        bzero(&ev, sizeof(ev));
        ev.data.fd = clntSockFd;
        ev.events = EPOLLIN | EPOLLET; // 对于客户端连接，使用ET模式，可以让epoll更加高效，支持更多并发
        setnonblocking(clntSockFd);                      // ET需要搭配非阻塞式socket使用
        epoll_ctl(epFd, EPOLL_CTL_ADD, clntSockFd, &ev); // 将该客户端的socket fd添加到epoll
      } else if (events[i].events & EPOLLIN) { // 发生事件的是客户端，并且是可读事件（EPOLLIN）
        char buf[READ_BUFFER];
        while (true) { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
          bzero(&buf, sizeof(buf));
          ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
          if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", events[i].data.fd, buf);
            write(events[i].data.fd, buf, sizeof(buf));
          } else if (bytes_read == -1 && errno == EINTR) { // 客户端正常中断、继续读取
            printf("continue reading");
            continue;
          } else if (bytes_read == -1 &&
                     ((errno == EAGAIN) || (errno == EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
          } else if (bytes_read == 0) { // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", events[i].data.fd);
            close(events[i].data.fd); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
          }
        }
      } else {
        printf("something else happened\n");
      }
    }
  }

  close(sockFd);
  return 0;
}
