#pragma once

class Socket {
public:
  Socket();
  Socket(int fd);
  ~Socket();
  void listen();
  int getFd();
  void setNonBlocking();

private:
  int m_fd;
};
