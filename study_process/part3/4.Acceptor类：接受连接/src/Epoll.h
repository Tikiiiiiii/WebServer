/*

Epoll类：
负责epoll对象的创建，监听，事件队列的管理等等
修改poll函数为返回Channel对象

*/

#pragma once
#include <vector>
#include <sys/epoll.h>
class Channel;
class Epoll{
private:
    int m_epfd;
    struct epoll_event * m_events;
public:
    Epoll();
    ~Epoll();
    void addFd(int fd,uint32_t op);
    void updateChannel(Channel*);   
    // std::vector<epoll_event> poll(int timeout = -1);
    std::vector<Channel*> poll(int timeout = -1);
};