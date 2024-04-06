#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>

void Channel::enableReading() {
  m_events = EPOLLIN | EPOLLET;
  m_loop->updateChannel(this);
}
