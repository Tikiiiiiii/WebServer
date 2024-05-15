/*

对于TCP协议，三次握手新建连接后，这个连接将会一直存在，直到我们四次挥手断开连接。因此，我们也可以把TCP连接抽象成一个Connection类
在server中用map来映射文件描述符和connection之间的关系

*/

#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Connection
{
private:
    EventLoop *m_loop;
    Socket *m_sock;
    Channel *m_channel;
    std::function<void(Socket*)> deleteConnectionCallback;
public:
    Connection(EventLoop *loop, Socket *sock);
    ~Connection();
    
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};