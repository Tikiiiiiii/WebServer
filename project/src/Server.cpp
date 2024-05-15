
#include "Server.h"

#include <unistd.h>

#include <functional>

#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"

#define READ_BUFFER 1024
#define THREADNUM 8

Server::Server(EventLoop *loop) : mainReactor(loop), m_acceptor(nullptr) {
  m_acceptor = new Acceptor(mainReactor);
  std::function<void(Socket *)> cb =
      std::bind(&Server::newConnection, this, std::placeholders::_1);
  m_acceptor->setNewConnectionCallback(cb);

  // int size=std::thread::hardware_concurrency();//几核CPU使用几个线程
  int size = THREADNUM;
  threadpool = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    subReactors.push_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop =
        std::bind(&EventLoop::loop, subReactors[i]);
    threadpool->add(sub_loop);
  }
}

Server::~Server() {
  delete m_acceptor;
  delete threadpool;
}

void Server::newConnection(Socket *sock) {
  if (sock->getFd() != -1) {
    // 该分配策略对于少量连接来说会频繁使用前几个线程
    int random = sock->getFd() % subReactors.size();
    Connection *conn = new Connection(subReactors[random], sock);
    std::function<void(int)> cb =
        std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    m_connections[sock->getFd()] = conn;
  }
}

void Server::deleteConnection(int sockfd) {
  if (sockfd != -1) {
    auto it = m_connections.find(sockfd);
    if (it != m_connections.end()) {
      Connection *conn = m_connections[sockfd];
      m_connections.erase(sockfd);
      // close(sockfd);       //正常
      delete conn;
    }
  }
}
