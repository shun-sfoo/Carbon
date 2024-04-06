#include "Server.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

Server::Server(EventLoop *lLoop) : m_loop(lLoop) {
  Socket *servSock = new Socket();
  InetAddress *servAddr = new InetAddress(servSock->getFd(), "127.0.0.1", 8888);
  servAddr->bind();
  servAddr->accept();
}

Server::~Server() {}
