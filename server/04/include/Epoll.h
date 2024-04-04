#pragma once
#include <cstdint>
#include <sys/epoll.h>
#include <vector>

class Socket;

class Epoll {
public:
  Epoll();
  ~Epoll();
  void addFd(int fd, uint32_t op);
  std::vector<epoll_event> poll(int timout = -1);

private:
  epoll_event *m_events;
  int m_fd;
};
