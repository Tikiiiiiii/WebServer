/*

重构bind，accept函数

*/

#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>

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

void Socket::bind(InetAddress *_addr){
    struct sockaddr_in addr = _addr->getAddr();
    socklen_t addr_len = _addr->getAddr_len();
    errif(::bind(m_fd, (sockaddr*)&addr, addr_len) == -1, "socket bind error");
    _addr->setInetAddr(addr, addr_len);
}

void Socket::listen(){
    errif(::listen(m_fd, SOMAXCONN) == -1, "Socket listen error");
}

void Socket::setnonblocking(){
    //先获取原先的状态，再添加nonblock状态
    fcntl(m_fd,F_SETFL,fcntl(m_fd,F_GETFL)|O_NONBLOCK);
}

int Socket::accept(InetAddress *_addr){
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    bzero(&addr, sizeof(addr));
    int clnt_sockfd = ::accept(m_fd, (sockaddr*)&addr, &addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    _addr->setInetAddr(addr, addr_len);
    return clnt_sockfd;
}

int Socket::getFd(){
    return m_fd;
}