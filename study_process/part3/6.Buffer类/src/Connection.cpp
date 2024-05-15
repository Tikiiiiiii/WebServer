#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include "Buffer.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

Connection::Connection(EventLoop *loop, Socket *sock) : m_loop(loop), m_sock(sock), m_channel(nullptr),inBuffer(new std::string()), readBuffer(nullptr){
    m_channel = new Channel(m_loop, m_sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, m_sock->getFd());
    m_channel->setCallback(cb);
    m_channel->enableReading();
    readBuffer = new Buffer();
}

Connection::~Connection(){
    delete m_channel;
    delete m_sock;
}

void Connection::echo(int sockfd){
    char buf[1024];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            readBuffer->append(buf, bytes_read);
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once\n");
            printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
            errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            readBuffer->clear();
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(m_sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> cb_f){
    deleteConnectionCallback = cb_f;
}