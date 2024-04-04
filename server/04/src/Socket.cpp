#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket() : m_fd(-1) {
  m_fd = socket(AF_INET, SOCK_STREAM, 0);
  errif(m_fd == -1, "create socket error");
}

Socket::Socket(int fd) : m_fd(fd) { errif(m_fd == -1, "socket creat error"); }

Socket::~Socket() {
  if (m_fd != -1) {
    close(m_fd);
    m_fd = -1;
  }
}

void Socket::bind(InetAddress *addr) {
  errif(::bind(m_fd, reinterpret_cast<sockaddr *>(&addr->m_sockAddr), addr->m_sockLen) == -1, "socket bind error");
}

void Socket::listen() { errif(::listen(m_fd, SOMAXCONN) == -1, "socket listen error"); }

int Socket::accpet(InetAddress *addr) {
  int clntSockFd = ::accept(m_fd, reinterpret_cast<sockaddr *>(&addr->m_sockAddr), &addr->m_sockLen);
  errif(clntSockFd == -1, "socket acept error");
  return clntSockFd;
}

void Socket::connect(InetAddress *addr) {
  errif(::connect(m_fd, reinterpret_cast<sockaddr *>(&addr->m_sockAddr), addr->m_sockLen) == -1, "connect error");
}

void Socket::setNonBlocking() { fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK); }

int Socket::getFd() { return m_fd; }
