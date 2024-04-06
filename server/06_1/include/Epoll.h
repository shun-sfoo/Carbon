#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
public:
  Epoll();
  ~Epoll();
  void updateChannel(Channel *);
  std::vector<Channel *> poll(int timeout = -1);

private:
  int m_epFd;
  epoll_event *m_events;
};
