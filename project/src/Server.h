/*

使用主从Reactor模式：
在Server中有一个主事件循环来监听并接受连接
线程池由Server管理
Server分出多个从Reactor事件循环来处理每个连接
单个从Reactor占一个线程

*/

#pragma once
#include <map>
#include <vector>
class EventLoop;
class Socket;
class ThreadPool;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *mainReactor;
    std::vector<EventLoop*> subReactors;
    ThreadPool* threadpool;
    Acceptor *m_acceptor;
    std::map<int, Connection*> m_connections;
public:

    Server(EventLoop*);
    ~Server();

    void newConnection(Socket *sock);
    void deleteConnection(int sockfd);
};