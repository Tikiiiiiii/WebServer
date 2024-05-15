/*

修改内存泄露问题，在删去channel对象时释放文件描述符fd

*/

#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>

Channel::Channel(EventLoop *loop, int fd) : m_loop(loop), m_fd(fd), m_events(0), m_revents(0), m_inEpoll(false){

}

Channel::~Channel(){
    if(m_fd != -1){
        close(m_fd);
        m_fd = -1;
    }
}

void Channel::handleEvent(){
    callback();
}

void Channel::enableReading(){
    m_events = EPOLLIN | EPOLLET;
    m_loop->updateChannel(this);
}

int Channel::getFd(){
    return m_fd;
}

uint32_t Channel::getEvents(){
    return m_events;
}
uint32_t Channel::getRevents(){
    return m_revents;
}

bool Channel::getInEpoll(){
    return m_inEpoll;
}

void Channel::setInEpoll(){
    m_inEpoll = true;
}

// void Channel::setEvents(uint32_t _ev){
//     events = _ev;
// }

void Channel::setRevents(uint32_t ev){
    m_revents = ev;
}

void Channel::setCallback(std::function<void()> cb_f){
    callback = cb_f;
}