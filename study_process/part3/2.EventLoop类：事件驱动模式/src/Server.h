/*

Server类：
服务器主体类

*/

#pragma once

class EventLoop;
class Socket;
class Server
{
private:
    EventLoop *m_loop;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};