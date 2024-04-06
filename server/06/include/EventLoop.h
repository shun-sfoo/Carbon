#pragma once

class Epoll;
class Channel;
class EventLoop {
private:
  Epoll *m_ep;
  bool m_quit;

public:
  EventLoop();
  ~EventLoop();
  void loop();
  void updateChannel(Channel *);
};
