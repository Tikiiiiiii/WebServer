/*

建立连接和业务处理是不同的工作
对于每一个连接，不管需要什么服务，都要调用accept()函数接受TCP连接
分离接受连接这一模块为Acceptor类：
负责管理连接，每个连接对应一个Acceptor对象

*/
#pragma once
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor
{
private:
    EventLoop *m_loop;
    Socket *m_sock;
    InetAddress *m_addr;
    Channel *m_acceptChannel;
public:
    Acceptor(EventLoop *loop);
    ~Acceptor();
    void acceptConnection();
    //回调函数
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};