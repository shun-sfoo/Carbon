#pragma once

#include <sys/epoll.h>
#include <vector>
class Channel;

class Epoll {
public:
  Epoll();
  ~Epoll();
  std::vector<Channel *> poll(int timeout = -1);
  void updateChannel(Channel *);

private:
  int m_epfd;
  epoll_event *m_events;
};
