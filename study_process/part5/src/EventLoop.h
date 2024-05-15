/*

EventLoop类：
负责主要的事件循环主体，对Epoll中的每个Channel进行询问

*/

#pragma once
#include <functional>
class Epoll;
class Channel;
class EventLoop
{
private:
    Epoll *m_ep;
    bool m_quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);
};