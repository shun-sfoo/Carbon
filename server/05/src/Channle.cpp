#include "Channel.h"
#include "Epoll.h"
#include <cstdint>
#include <sys/epoll.h>

Channel::Channel(Epoll *lEp, int lFd) : m_ep(lEp), m_fd(lFd), m_inEPoll(false), m_events(0), m_revents(0) {}

Channel::~Channel() {}

void Channel::enableReading() {
  m_events = EPOLLIN | EPOLLET;
  m_ep->updateChannel(this);
}

int Channel::getFd() { return m_fd; }

uint32_t Channel::getEvents() { return m_events; }

uint32_t Channel::getRevents() { return m_revents; }

bool Channel::getInEpoll() { return m_inEPoll; }

void Channel::setInEpoll() { m_inEPoll = true; }

void Channel::setRevents(uint32_t lRevents) { m_revents = lRevents; }
