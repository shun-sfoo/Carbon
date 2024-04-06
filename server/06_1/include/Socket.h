#pragma once

class Socket {
public:
  Socket();
  Socket(int lFd);
  ~Socket();
  int getFd();
private:
  int m_fd;
};


