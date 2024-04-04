#include "Epoll.h"
#include "util.h"
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace {
constexpr int MAX_EVENTS = 1000;
} // namespace

Epoll::Epoll() : m_fd(-1), m_events(nullptr) {
  m_fd = epoll_create1(0);
  errif(m_fd == -1, "create epoll failed");
  m_events = new epoll_event[MAX_EVENTS];
  bzero(m_events, sizeof(*m_events) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (m_fd != -1) {
    close(m_fd);
    m_fd = -1;
  }
  delete[] m_events;
}

void Epoll::addFd(int lFd, uint32_t lOp) {
  epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.fd = lFd;
  ev.events = lOp;
  errif(epoll_ctl(m_fd, EPOLL_CTL_ADD, lFd, &ev) == -1, "add event error");
}

std::vector<epoll_event> Epoll::poll(int timeout) {
  int nFds = epoll_wait(m_fd, m_events, MAX_EVENTS, timeout);
  errif(nFds == -1, "epoll wait error");
  std::vector<epoll_event> activeEvents;
  for (int i = 0; i < nFds; ++i) {
    activeEvents.push_back(m_events[i]);
  }
  return activeEvents;
}
