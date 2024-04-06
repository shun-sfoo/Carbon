#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop *lLoop, int lFd)
    : m_loop(lLoop), m_fd(lFd), m_events(0), m_revents(0), m_inEPoll(false), m_callback(nullptr) {}

Channel::~Channel() {}

void Channel::handleEvent() { m_callback(); }

void Channel::enableReading() {
  m_events = EPOLLIN | EPOLLET;
  m_loop->updateChannel(this);
}

int Channel::getFd() { return m_fd; }

uint32_t Channel::getEvents() { return m_events; }

uint32_t Channel::getRevents() { return m_revents; }

bool Channel::getInEpoll() { return m_inEPoll; }

void Channel::setInEpoll() { m_inEPoll = true; }

void Channel::setRevents(uint32_t lEv) { m_revents = lEv; }

void Channel::setCallback(std::function<void()> lCb) { m_callback = lCb; }
