/*

引入线程池

*/

#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop() : m_ep(nullptr), m_quit(false){
    m_ep = new Epoll();
    m_threadPool = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete m_ep;
    delete m_threadPool;
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

void EventLoop::addThread(std::function<void()> func){
    m_threadPool->add(func);
}