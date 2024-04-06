#include "InetAddress.h"
#include "util.h"
#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>

InetAddress::InetAddress(int lFd) : m_fd(lFd), m_sockLen(sizeof(m_sockAddr)) { bzero(&m_sockAddr, sizeof(m_sockAddr)); }

InetAddress::InetAddress(int lFd, const char *ip, uint16_t port) : m_fd(lFd), m_sockLen(sizeof(m_sockAddr)) {
  bzero(&m_sockAddr, sizeof(m_sockAddr));
  m_sockAddr.sin_family = AF_INET;
  m_sockAddr.sin_addr.s_addr = inet_addr(ip);
  m_sockAddr.sin_port = htons(port);
  m_sockLen = sizeof(m_sockAddr);
}

InetAddress::~InetAddress() {}

void InetAddress::bind() {
  errif(::bind(m_fd, reinterpret_cast<sockaddr *>(&m_sockAddr), m_sockLen) == -1, "bind error");
}

int InetAddress::accpet() {
  int ret = ::accept(m_fd, reinterpret_cast<sockaddr *>(&m_sockAddr), &m_sockLen);
  errif(ret == -1, "accpet error");
  return ret;
}

void InetAddress::connect() {
  errif(::connect(m_fd, reinterpret_cast<sockaddr *>(&m_sockAddr), m_sockLen) == -1, "connect error");
}