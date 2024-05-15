#pragma once
#include <functional>

#include "Macros.h"

class EventLoop;
class Socket;
class Channel;
class Acceptor {
 public:
  explicit Acceptor(EventLoop *loop);
  ~Acceptor();

  DISALLOW_COPY_AND_MOVE(Acceptor);

  void acceptConnection();
  void setNewConnectionCallback(std::function<void(Socket *)>);

 private:
  EventLoop *m_loop;
  Socket *m_sock;
  Channel *m_acceptChannel;
  std::function<void(Socket *)> newConnectionCallback;
};
