#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include <vector>

EventLoop::EventLoop() : m_quit(false) { m_epoll = new Epoll(); }
EventLoop::~EventLoop() { delete[] m_epoll; }

void EventLoop::loop() {
  std::vector<Channel *> chs = m_epoll->poll();
  for (auto *it : chs) {
    it->handleEvent();
  }
}

void EventLoop::updateChannel(Channel *lChannel) { m_epoll->updateChannel(lChannel); }
