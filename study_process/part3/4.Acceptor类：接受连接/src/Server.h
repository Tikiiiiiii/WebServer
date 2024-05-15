/*

Server类：
服务器主体类

*/

#pragma once

class EventLoop;
class Socket;
class Acceptor;
class Server
{
private:
    EventLoop *m_loop;
    Acceptor *m_acceptor;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};