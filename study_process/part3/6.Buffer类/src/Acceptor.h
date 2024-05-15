#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Acceptor
{
private:
    EventLoop *m_loop;
    Socket *m_sock;
    Channel *m_acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;
public:
    Acceptor(EventLoop *loop);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);
};