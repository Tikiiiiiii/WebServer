#pragma once
#include <vector>

#include "Macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#ifdef OS_MACOS
#include <sys/event.h>
#endif

class Channel;
class Poller {
 public:
  Poller();
  ~Poller();

  DISALLOW_COPY_AND_MOVE(Poller);

  void UpdateChannel(Channel *ch);
  void DeleteChannel(Channel *ch);

  std::vector<Channel *> Poll(int timeout = -1);

 private:
  int fd_{1};
#ifdef OS_LINUX
  struct epoll_event *events_{nullptr};
#endif

#ifdef OS_MACOS
  struct kevent *events_{nullptr};
#endif
};
