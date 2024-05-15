/*

合并Socket类和InetAddress类：
两类公用Socket
*/

#pragma once
#include <arpa/inet.h>

#include "Macros.h"

class InetAddress {
 public:
  InetAddress();
  InetAddress(const char* ip, uint16_t port);
  ~InetAddress();

  DISALLOW_COPY_AND_MOVE(InetAddress);

  void setInetAddr(sockaddr_in _addr);
  sockaddr_in getAddr();
  char* getIp();
  uint16_t getPort();

 private:
  struct sockaddr_in addr;
};

class Socket {
 public:
  Socket();              // 构造函数
  explicit Socket(int);  // 带参数构造函数
  ~Socket();             // 析构函数

  DISALLOW_COPY_AND_MOVE(Socket);

  void bind(InetAddress*);   // 把socket和ip地址绑定
  void listen();             // 监听
  void setnonblocking();     // 把socket设置非阻塞
  int accept(InetAddress*);  // 接受ip连接
  void connect(InetAddress*);
  int getFd();  // 返回当前描述符

 private:
  int m_fd;
};
