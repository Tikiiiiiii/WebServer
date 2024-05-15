/*

Epoll类：
负责epoll对象的创建，监听，事件队列的管理等等

*/

#pragma once
#include <vector>
#include <sys/epoll.h>
class Epoll{
private:
    int m_epfd;
    struct epoll_event * m_events;
public:
    Epoll();
    ~Epoll();
    void addFd(int fd,uint32_t op);
    std::vector<epoll_event> poll(int timeout=1);
};