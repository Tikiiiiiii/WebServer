#include "Epoll.h"
#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>

#define MAX_EVENTS 1024

Epoll::Epoll():m_epfd(-1),m_events(nullptr){
    m_epfd=epoll_create1(0);
    errif(m_epfd==-1,"Epoll create error");
    m_events=new epoll_event[MAX_EVENTS];
    bzero(m_events,sizeof(*m_events)*MAX_EVENTS);
}

Epoll::~Epoll(){
    if(m_epfd!=-1){
        close(m_epfd);
        m_epfd=-1;
    }
    delete[] m_events;
}

void Epoll::addFd(int fd,uint32_t op){
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.data.fd=fd;
    ev.events=op;
    errif(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev)==-1,"Epoll add event error");
}

// std::vector<epoll_event> Epoll::poll(int timeout){
//     std::vector<epoll_event> activeEvents;
//     int nfds = epoll_wait(m_epfd, m_events, MAX_EVENTS, timeout);
//     errif(nfds == -1, "Epoll wait error");
//     for(int i = 0; i < nfds; ++i){
//         activeEvents.push_back(m_events[i]);
//     }
//     return activeEvents;
// }
std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(m_epfd, m_events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        Channel *ch = (Channel*)m_events[i].data.ptr;
        ch->setRevents(m_events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoll::updateChannel(Channel *channel){
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->getInEpoll()){
        errif(epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->setInEpoll();
        // debug("Epoll: add Channel to epoll tree success, the Channel's fd is: ", fd);
    } else{
        errif(epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
        // debug("Epoll: modify Channel in epoll tree success, the Channel's fd is: ", fd);
    }
}