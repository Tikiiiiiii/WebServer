
#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection
{
private:
    EventLoop *m_loop;
    Socket *m_sock;
    Channel *m_channel;
    std::function<void(int)> deleteConnectionCallback;
    Buffer *readBuffer;
public:
    Connection(EventLoop *loop, Socket *sock);
    ~Connection();
    
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)>);
    void send(int sockfd);
};