#pragma once
#include <netinet/in.h>

class InetAddress {
public:
  InetAddress(int lFd, const char *ip, uint16_t port);
  InetAddress(int lFd);
  ~InetAddress();
  socklen_t m_sockLen;
  sockaddr_in m_sockAddr;
  void bind();
  int accpet();
  void connect();

private:
  int m_fd;
};
