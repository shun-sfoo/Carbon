#pragma once

#include <cstdint>
class Epoll;

class Channel {
private:
  int m_fd;
  Epoll *m_ep;
  int32_t m_events;
  int32_t m_revents;
  bool m_inEPoll;

public:
  Channel(Epoll *lEp, int lFd);
  ~Channel();
  void enableReading();
  int getFd();
  uint32_t getEvents();
  uint32_t getRevents();
  bool getInEpoll();
  void setInEpoll();
  void setRevents(uint32_t);
};
