#include "InetAddress.h"
#include <arpa/inet.h>
#include <strings.h>

InetAddress::InetAddress() : m_sockLen(sizeof(m_sockAddr)) { bzero(&m_sockAddr, sizeof(m_sockAddr)); }

InetAddress::InetAddress(const char *ip, uint16_t port) : m_sockLen(sizeof(m_sockAddr)) {
  bzero(&m_sockAddr, sizeof(m_sockAddr));
  m_sockAddr.sin_family = AF_INET;
  m_sockAddr.sin_addr.s_addr = inet_addr(ip);
  m_sockAddr.sin_port = htons(port);
  m_sockLen = sizeof(m_sockAddr);
}

InetAddress::~InetAddress() {}
