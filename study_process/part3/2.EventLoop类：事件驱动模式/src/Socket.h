/*

Socket类：
主要负责socket的建立

*/

#pragma once

class InetAddress;
class Socket{
public:
    Socket();                   //构造函数
    Socket(int);                //带参数构造函数
    ~Socket();                  //析构函数
    void bind(InetAddress*);    //把socket和ip地址绑定
    void listen();              //监听
    void setnonblocking();      //把socket设置非阻塞
    int accept(InetAddress*);   //接受ip连接
    int getFd();                //返回当前描述符
private:
    int m_fd;                   
};