#pragma once

class EventLoop;
class Socket;

class Server {
public:
  Server(EventLoop *);
  ~Server();
  void handleReadEvent(int);
  void newConnect(Socket *servSock);

private:
  EventLoop* m_loop;

};

