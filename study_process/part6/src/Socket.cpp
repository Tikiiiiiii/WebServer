/*

重构bind，accept函数
去掉inetaddress的addr_len对象，冗余项

*/

#include "Socket.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "util.h"

Socket::Socket() : m_fd(-1) {
  m_fd = socket(AF_INET, SOCK_STREAM, 0);
  errif(m_fd == -1, "Socket create error");
}

Socket::Socket(int fd) : m_fd(fd) { errif(m_fd == -1, "Socket create error"); }

Socket::~Socket() {
  if (m_fd != -1) {
    close(m_fd);
    m_fd = -1;
  }
}

void Socket::bind(InetAddress *_addr) {
  struct sockaddr_in addr = _addr->getAddr();
  errif(::bind(m_fd, (sockaddr *)&addr, sizeof(addr)) == -1,
        "socket bind error");
}

void Socket::listen() {
  errif(::listen(m_fd, SOMAXCONN) == -1, "Socket listen error");
}

void Socket::setnonblocking() {
  // 先获取原先的状态，再添加nonblock状态
  fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *_addr) {
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  memset(&addr, 0, sizeof(addr));
  int clnt_sockfd = -1;
  if (fcntl(m_fd, F_GETFL) & O_NONBLOCK) {
    while (true) {
      clnt_sockfd = ::accept(m_fd, (sockaddr *)&addr, &addr_len);
      if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        // printf("no connection yet\n");
        continue;
      } else if (clnt_sockfd == -1) {
        errif(true, "socket accept error");
      } else {
        break;
      }
    }
  } else {
    clnt_sockfd = ::accept(m_fd, (sockaddr *)&addr, &addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
  }
  _addr->setInetAddr(addr);
  return clnt_sockfd;
}

void Socket::connect(InetAddress *_addr) {
  struct sockaddr_in addr = _addr->getAddr();
  if (fcntl(m_fd, F_GETFL) & O_NONBLOCK) {
    while (true) {
      int ret = ::connect(m_fd, (sockaddr *)&addr, sizeof(addr));
      if (ret == 0) {
        break;
      } else if (ret == -1 && (errno == EINPROGRESS)) {
        continue;
        /* 连接非阻塞式sockfd建议的做法：
            The socket is nonblocking and the connection cannot be
          completed immediately.  (UNIX domain sockets failed with
          EAGAIN instead.)  It is possible to select(2) or poll(2)
          for completion by selecting the socket for writing.  After
          select(2) indicates writability, use getsockopt(2) to read
          the SO_ERROR option at level SOL_SOCKET to determine
          whether connect() completed successfully (SO_ERROR is
          zero) or unsuccessfully (SO_ERROR is one of the usual
          error codes listed here, explaining the reason for the
          failure).
          这里为了简单、不断连接直到连接完成，相当于阻塞式
        */
      } else if (ret == -1) {
        errif(true, "socket connect error");
      }
    }
  } else {
    errif(::connect(m_fd, (sockaddr *)&addr, sizeof(addr)) == -1,
          "socket connect error");
  }
}

int Socket::getFd() { return m_fd; }

InetAddress::InetAddress() { memset(&addr, 0, sizeof(addr)); }
InetAddress::InetAddress(const char *ip, uint16_t port) {
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(port);
}

InetAddress::~InetAddress() {}

void InetAddress::setInetAddr(sockaddr_in _addr) { addr = _addr; }

sockaddr_in InetAddress::getAddr() { return addr; }

char *InetAddress::getIp() { return inet_ntoa(addr.sin_addr); }

uint16_t InetAddress::getPort() { return ntohs(addr.sin_port); }
