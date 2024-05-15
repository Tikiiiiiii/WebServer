/*

抽象出Epoll和Channel之后，我们需要继续抽象服务器Server类
Server通过初始化后主要由一个事件循环组成，不断执行while中的内容，因此把循环抽离出来成EventLoop类
把Server抽象成封装着EventLoop类的类，通过封装我们设计成事件驱动模式

*/

#include "src/EventLoop.h"
#include "src/Server.h"

int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();
    return 0;
}
