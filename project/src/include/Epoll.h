/*

Epoll类：
负责epoll对象的创建，监听，事件队列的管理等等
修改poll函数为返回Channel对象

*/

#pragma once
#include <vector>

#include "Macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

class Channel;
class Epoll {
 public:
  Epoll();
  ~Epoll();

  DISALLOW_COPY_AND_MOVE(Epoll);

  void updateChannel(Channel*);
  void deleteChannel(Channel*);
  std::vector<Channel*> poll(int timeout = -1);

 private:
  int m_epfd;
  struct epoll_event* m_events;
};
