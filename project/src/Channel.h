/*

Channel类：
一个Channel对应一个事件，负责管理更多事件的信息，由epoll_data的ptr指向
把Epoll改为EventLoop，对Epoll即对EventLoop

*/

#pragma once
#include <sys/epoll.h>
#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
    EventLoop * m_loop;
    int m_fd;
    uint32_t m_events;
    uint32_t m_ready;
    bool m_useThreadPool;
    bool m_inEpoll;
    //下面的对象用于bind一个回调函数，当执行到对应的函数时，会跳回某个对应类执行相应的函数
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();

    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};