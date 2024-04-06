#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
class InetAddress {
public:
  // for server
  InetAddress(int servFd, const char *ip, uint16_t port);
  // for client
  InetAddress(int clntFd);
  socklen_t m_sockLen;
  sockaddr_in m_sockAddr;
  void bind();
  int accept();
  void connect();

private:
  int m_fd;
};
