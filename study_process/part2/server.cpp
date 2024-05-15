/*

使用epoll来建立服务器：
前4个步骤不变
5.创建epoll的描述符，也就是创建epoll的监听队列，max_event表示完全连接的最大连接数
6.初始化epoll，指定IO口，触发模式，和触发事件的类型
7.添加服务器socket到epoll监听
8.不断监听epoll事件的情况
9.根据业务类型进行不同处理
10.关闭连接（服务器很少用到）

*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>
#include "util.h"

#define MAX_LISTEN_LEN 128 		//最大监听队列长度
#define MAX_EVENTS 1024
#define BUFF_SIZE 1024			//缓冲区大小

int main(){

	//1.创建socket套接字，其本质是一个文件描述符file_description
	int socket_fd=socket(AF_INET,SOCK_STREAM,0);	
	errif(socket_fd==-1,"Socket create failed");
	
	//2.初始化服务器地址
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));				//清零
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");		//127.0.0.1表示本机
	server_addr.sin_port=htons(8888);	
	
	//3.绑定服务器地址和socket
	errif(bind(socket_fd,(sockaddr*)&server_addr,sizeof(server_addr))==-1,"Socket bind error");

	//4.监听
	errif(listen(socket_fd,MAX_LISTEN_LEN)==-1,"Listen error");

	//5.创建epoll描述符
	int epoll_fd=epoll_create1(0);
	errif(epoll_fd==-1,"Epoll create error");

	//6.初始化epoll
	struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.data.fd = socket_fd;								//设置IO口为服务器socket fd
    ev.events = EPOLLIN | EPOLLET;						//事件为数据可读事件，ET表示边缘触发LT表示水平触发

	//7.添加服务器socket到epoll监听
    setnonblocking(socket_fd);							//设置sock为非阻塞模式
    errif(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev)==-1,"Epoll control error");
	
	//8.不断监听epoll事件
	while(true){	

		//等待事件发生
		int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "Epoll wait error");

		//9.根据业务类型进行不同处理（不准确，连接准确来说不是业务处理，根据事件类型进行不同处理
		for(int i = 0; i < nfds; ++i){
            if(events[i].data.fd == socket_fd){       	 //有新客户端连接
                struct sockaddr_in clnt_addr;
                bzero(&clnt_addr, sizeof(clnt_addr));
                socklen_t clnt_addr_len = sizeof(clnt_addr);
                int clnt_sockfd = accept(socket_fd, (sockaddr*)&clnt_addr, &clnt_addr_len);
                errif(clnt_sockfd == -1, "Socket accept error");
                printf("New client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
				
				//
                bzero(&ev, sizeof(ev));
                ev.data.fd = clnt_sockfd;	
                ev.events = EPOLLIN | EPOLLET;			//对于客户端连接，使用ET模式，可以让epoll更加高效，支持更多并发，后续改为LT是因为不会有客户持续连接，设置为LT不影响效率
                setnonblocking(clnt_sockfd);			//ET需要搭配非阻塞式socket使用
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
            } else if(events[i].events & EPOLLIN){      //有新的可读事件
                char buf[BUFF_SIZE];
                while(true){    						//由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if(bytes_read > 0){
                        printf("Message from client fd %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if(bytes_read == -1 && errno == EINTR){   							//客户端正常中断、继续读取
                        printf("Continue reading");
                        continue;
                    } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                        printf("Finish reading once, errno: %d\n", errno);
                        break;
                    } else if(bytes_read == 0){  		 //EOF，客户端断开连接
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);  		 //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                }
            } else{         //其他事件，之后的版本实现
                printf("Something else happened\n");
			}
		}
	}
	
	//10.关闭连接
	close(socket_fd);
	return 0;
}

