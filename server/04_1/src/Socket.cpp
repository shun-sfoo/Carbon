#include "Socket.h"
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

void Socket::listen() { errif(::listen(m_fd, SOMAXCONN) == -1, "socket listen error"); }

void Socket::setNonBlocking() { fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK); }

int Socket::getFd() { return m_fd; }
