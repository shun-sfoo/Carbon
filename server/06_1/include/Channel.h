#pragma once

#include <cstdint>
#include <functional>
class EventLoop;

class Channel {
public:
  Channel(EventLoop *lLoop, int lFd)
      : m_fd(lFd), m_loop(lLoop), m_events(0), m_revents(0), m_inEpoll(false), m_callback(nullptr) {}

  ~Channel() {}

  void handleEvent() { m_callback(); };
  void enableReading();

  int getFd() { return m_fd; }
  uint32_t getEvents() { return m_events; }
  uint32_t getRevents() { return m_revents; }
  bool getInPoll() { return m_inEpoll; }
  void setInEpoll() { m_inEpoll = true; }
  void setRevents(uint32_t events) { m_events = events; }
  void setCallback(std::function<void()> lCallback) { m_callback = lCallback; }

private:
  int m_fd;
  EventLoop *m_loop;
  uint32_t m_events;
  uint32_t m_revents;
  bool m_inEpoll;
  std::function<void()> m_callback;
};
