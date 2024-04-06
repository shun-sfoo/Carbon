#include "InetAddress.h"
#include "util.h"
#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

// for server
InetAddress::InetAddress(int servFd, const char *ip, uint16_t port) : m_fd(servFd), m_sockLen(sizeof(m_sockAddr)) {
  bzero(&m_sockAddr, sizeof(m_sockAddr));
  m_sockAddr.sin_family = AF_INET;
  m_sockAddr.sin_addr.s_addr = inet_addr(ip);
  m_sockAddr.sin_port = htons(port);
  m_sockLen = sizeof(m_sockAddr);
}

InetAddress::InetAddress(int clntFd) : m_fd(clntFd), m_sockLen(sizeof(m_sockAddr)) {
  bzero(&m_sockAddr, sizeof(m_sockAddr));
  m_sockLen = sizeof(m_sockAddr);
}

void InetAddress::bind() {
  errif(::bind(m_fd, reinterpret_cast<sockaddr *>(&m_sockAddr), m_sockLen) == -1, "bind error");
}

// for clent connect
int InetAddress::accept() {
  int clntSockFd = ::accept(m_fd, reinterpret_cast<sockaddr *>(&m_sockAddr), &m_sockLen);
  errif(clntSockFd == -1, "accept error");
  return clntSockFd;
}

void InetAddress::connect() {
  errif(::connect(m_fd, reinterpret_cast<sockaddr *>(&m_sockAddr), m_sockLen) == -1, "connect error");
}
