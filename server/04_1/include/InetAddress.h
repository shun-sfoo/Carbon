#pragma once
#include <netinet/in.h>

class Socket;
class InetAddress {
public:
  InetAddress(const char *ip, uint16_t port);
  InetAddress();
  ~InetAddress();
  socklen_t m_sockLen;
  sockaddr_in m_sockAddr;
  void bind(Socket *);
  int accpet(Socket *);
  void connect(Socket *);
};
