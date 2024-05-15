/*

创建简单的回声服务器的服务器端（添加错误处理）：
1.利用socket创建socket套接字
2.初始化服务器地址结构
3.把服务器地址绑定到socket上
4.调用listen创建监听队列，监听服务器socket
5.接受一个来自远程的连接
6.业务处理（读取来自客户端的数据并回送）
	（1）定义缓冲区
	（2）从客户端的socket读取数据
	（3）回送数据
7.关闭连接（基本用不到）

*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include "util.h"

#define MAX_LISTEN_LEN 128 		//最大监听队列长度
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

	//5.接受一个连接
	struct sockaddr_in client_addr;
	socklen_t client_addr_len=sizeof(client_addr);
	bzero(&client_addr,sizeof(client_addr));
	int client_socket_fd=accept(socket_fd,(sockaddr*)&client_addr,&client_addr_len);
	errif(client_socket_fd==-1,"Accept error");
	
	//6.业务处理
	while(true){
		//（1）.定义并初始化缓冲区
		char buf[BUFF_SIZE];
		bzero(&buf,sizeof(buf));

		//（2）.从文件描述符读取数据
		ssize_t read_bytes=recv(client_socket_fd,buf,sizeof(buf),0);
		if(read_bytes>0){
			printf("Read message from client fd %d: %s\n",client_socket_fd,buf);
			//（3）.回送
			errif(send(client_socket_fd,buf,sizeof(buf),0)==-1,"Server send message failed");
		}else if(read_bytes==0){
			printf("client fd %d disconnected\n", client_socket_fd);		//read返回0表示连接关闭
        	close(client_socket_fd);
       		break;
		}else if(read_bytes == -1){        							//read返回-1表示发生错误
			close(client_socket_fd);
			errif(true, "Server socket read error");
		}
	}
	
	//7.关闭连接
	close(socket_fd);
	return 0;
}

