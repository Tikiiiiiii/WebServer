#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop() : m_ep(nullptr), m_quit(false){
    m_ep = new Epoll();
}

EventLoop::~EventLoop()
{
    delete m_ep;
}


void EventLoop::loop(){
    while(!m_quit){
    std::vector<Channel*> chs;
        chs = m_ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it){
            (*it)->handleEvent();
        }
    }
}

//通过回调的方式调用更新
void EventLoop::updateChannel(Channel *ch){
    m_ep->updateChannel(ch);
}