/*

EventLoop类：
负责主要的事件循环主体，对Epoll中的每个Channel进行询问

*/

#pragma once
#include <functional>

#include "Macros.h"
class Epoll;
class Channel;
class EventLoop {
 public:
  EventLoop();
  ~EventLoop();

  DISALLOW_COPY_AND_MOVE(EventLoop);

  void loop();
  void updateChannel(Channel *);

 private:
  Epoll *m_ep;
  bool m_quit;
};
