#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket():m_fd(-1){
    m_fd=socket(AF_INET,SOCK_STREAM,0);
    errif(m_fd==-1,"Socket create error");
}

Socket::Socket(int fd):m_fd(fd){
    errif(m_fd==-1,"Socket create error");
}

Socket::~Socket(){
    if(m_fd!=-1){
        close(m_fd);
        m_fd=-1;
    }
}

void Socket::bind(InetAddress* ipaddr){
    errif(::bind(m_fd,(sockaddr*)&ipaddr->addr,ipaddr->addr_len),"Socket bind error");
}

void Socket::listen(){
    errif(::listen(m_fd, SOMAXCONN) == -1, "Socket listen error");
}

void Socket::setnonblocking(){
    //先获取原先的状态，再添加nonblock状态
    fcntl(m_fd,F_SETFL,fcntl(m_fd,F_GETFL)|O_NONBLOCK);
}

int Socket::accept(InetAddress* ipaddr){
    int clnt_sockfd = ::accept(m_fd, (sockaddr*)&ipaddr->addr, &ipaddr->addr_len);
    errif(clnt_sockfd == -1, "Socket accept error");
    return clnt_sockfd;
}

int Socket::getFd(){
    return m_fd;
}