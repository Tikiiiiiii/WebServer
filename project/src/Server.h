/*

Server类：
服务器主体类

*/

#pragma once
#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *m_loop;
    Acceptor *m_acceptor;
    std::map<int, Connection*> m_connections;
public:
    Server(EventLoop*);
    ~Server();

    void newConnection(Socket *sock);
    void deleteConnection(int sockfd);
};