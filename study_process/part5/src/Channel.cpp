/*

完善Channel实现，配合线程

*/

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <unistd.h>
#include <sys/epoll.h>

Channel::Channel(EventLoop *loop, int fd) : m_loop(loop), m_fd(fd), m_events(0), m_ready(0), m_inEpoll(false){

}

Channel::~Channel(){
    if(m_fd != -1){
        close(m_fd);
        m_fd = -1;
    }
}

void Channel::handleEvent(){
    if(m_ready & (EPOLLIN | EPOLLPRI)){
        readCallback();
    }
    if(m_ready & (EPOLLOUT)){
        writeCallback();
    }
}

//修复为保留原始标志
void Channel::enableRead(){
    m_events |= EPOLLIN | EPOLLET;
    m_loop->updateChannel(this);
}

void Channel::useET(){
    m_events |= EPOLLET;
    m_loop->updateChannel(this);
}

int Channel::getFd(){
    return m_fd;
}

uint32_t Channel::getEvents(){
    return m_events;
}
uint32_t Channel::getReady(){
    return m_ready;
}

bool Channel::getInEpoll(){
    return m_inEpoll;
}

void Channel::setInEpoll(bool in){
    m_inEpoll = in;
}

void Channel::setReady(uint32_t ev){
    m_ready = ev;
}

void Channel::setReadCallback(std::function<void()> cb_f){
    readCallback = cb_f;
}
