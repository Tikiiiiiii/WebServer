/*

合并Socket类和InetAddress类：
两类公用Socket
*/

#pragma once
#include <arpa/inet.h>

class InetAddress
{
private:
    struct sockaddr_in addr;
public:
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();

    void setInetAddr(sockaddr_in _addr);
    sockaddr_in getAddr();
    char* getIp();
    uint16_t getPort();
};

class Socket{
public:
    Socket();                   //构造函数
    Socket(int);                //带参数构造函数
    ~Socket();                  //析构函数
    void bind(InetAddress*);    //把socket和ip地址绑定
    void listen();              //监听
    void setnonblocking();      //把socket设置非阻塞
    int accept(InetAddress*);   //接受ip连接
    void connect(InetAddress*);
    int getFd();                //返回当前描述符
private:
    int m_fd;                   
};