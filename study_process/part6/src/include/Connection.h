
#pragma once
#include <functional>

#include "Macros.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection {
 public:
  Connection(EventLoop *loop, Socket *sock);
  ~Connection();

  DISALLOW_COPY_AND_MOVE(Connection);

  void echo(int sockfd);
  void setDeleteConnectionCallback(std::function<void(int)>);
  void send(int sockfd);

 private:
  EventLoop *m_loop;
  Socket *m_sock;
  Channel *m_channel;
  std::function<void(int)> deleteConnectionCallback;
  Buffer *readBuffer;
};
