#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include <stdio.h>

Acceptor::Acceptor(EventLoop *loop) : m_loop(loop), m_sock(nullptr), m_acceptChannel(nullptr){
    m_sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 1234);
    m_sock->bind(addr);
    m_sock->listen(); 
    // m_sock->setnonblocking();    //acceptor使用阻塞式IO比较好
    m_acceptChannel = new Channel(m_loop, m_sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    m_acceptChannel->setReadCallback(cb);
    m_acceptChannel->enableRead();
    delete addr;
}

Acceptor::~Acceptor(){
    delete m_sock;
    delete m_acceptChannel;
}

void Acceptor::acceptConnection(){
    InetAddress *clnt_addr = new InetAddress();      
    Socket *clnt_sock = new Socket(m_sock->accept(clnt_addr));      
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), clnt_addr->getIp(), clnt_addr->getPort());
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> cb_f){
    newConnectionCallback = cb_f;
}