#include "Socket.h"
#include "util.h"
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket() : m_fd(-1) {
  m_fd = socket(AF_INET, SOCK_STREAM, 0);
  errif(m_fd == -1, "socket create error");
}

Socket::Socket(int lFd) : m_fd(lFd) {}

Socket::~Socket() {
  if (m_fd != -1) {
    close(m_fd);
  }
}

int Socket::getFd() { return m_fd; }
