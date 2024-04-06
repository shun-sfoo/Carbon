#include "Epoll.h"
#include "Channel.h"
#include "util.h"
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace {
constexpr int MAX_EVENTS = 1000;
} // namespace

Epoll::Epoll() : m_epfd(-1), m_events(nullptr) {
  m_epfd = epoll_create1(0);
  errif(m_epfd == -1, "epoll create error");
  m_events = new epoll_event[MAX_EVENTS];
  bzero(m_events, sizeof(&m_events) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (m_epfd != -1) {
    close(m_epfd);
  }
  delete[] m_events;
}

void Epoll::updateChannel(Channel *lChannel) {
  int fd = lChannel->getFd();
  epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = lChannel;
  ev.events = lChannel->getEvents();
  if (!lChannel->getInEpoll()) {
    errif(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    lChannel->setInEpoll();
  } else {
    errif(epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}

std::vector<Channel *> Epoll::poll(int timeout) {
  int nFds = epoll_wait(m_epfd, m_events, MAX_EVENTS, timeout);
  errif(nFds == -1, "epoll wait error");
  std::vector<Channel *> activeChannels;
  for (int i = 0; i < nFds; ++i) {
    Channel *ch = reinterpret_cast<Channel *>(m_events[i].data.ptr);
    ch->setRevents(m_events[i].events);
    activeChannels.push_back(ch);
  }
  return activeChannels;
}
