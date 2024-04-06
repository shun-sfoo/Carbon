#pragma once

#include <cstdint>
#include <functional>
class EventLoop;

class Channel {
private:
  int m_fd;
  EventLoop *m_loop;
  int32_t m_events;
  int32_t m_revents;
  bool m_inEPoll;
  std::function<void()> m_callback;

public:
  Channel(EventLoop *lLoop, int lFd);
  ~Channel();

  void handleEvent();
  void enableReading();

  int getFd();
  uint32_t getEvents();
  uint32_t getRevents();
  bool getInEpoll();
  void setInEpoll();
  void setRevents(uint32_t);
  void setCallback(std::function<void()>);
};
