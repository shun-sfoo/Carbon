#pragma once

class Epoll;
class Channel;

class EventLoop {
public:
  EventLoop();
  ~EventLoop();
  void loop();
  void updateChannel(Channel *);

private:
  Epoll *m_epoll;
  bool m_quit;
};
