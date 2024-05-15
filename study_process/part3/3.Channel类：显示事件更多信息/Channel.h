/*

Channel类：
一个Channel对应一个事件，负责管理更多事件的信息，由epoll_data的ptr指向

*/

#pragma once
#include <sys/epoll.h>

class Epoll;
class Channel
{
private:
    Epoll * m_ep;
    int m_fd;
    uint32_t m_events;
    uint32_t m_revents;
    bool m_inEpoll;
public:
    Channel(Epoll *ep, int fd);
    ~Channel();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    // void setEvents(uint32_t);
    void setRevents(uint32_t);
};