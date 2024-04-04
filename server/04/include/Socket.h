#pragma once
class InetAddress;

class Socket {
public:
  Socket();
  Socket(int fd);
  ~Socket();
  void bind(InetAddress *);
  void listen();
  int accpet(InetAddress *);
  int getFd();
  void setNonBlocking();
  void connect(InetAddress *);

private:
  int m_fd;
};
