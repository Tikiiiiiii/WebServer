/*

回声服务器的客户端：
1.利用socket创建socket套接字
2.初始化服务器地址
3.主动发起连接
4.向服务器发送数据
5.关闭连接

*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "util.h"

#define BUFF_SIZE 1024			//缓冲区大小

int main(){

	//1.创建socket
	int socket_fd=socket(AF_INET,SOCK_STREAM,0);

	//2.初始化服务端地址
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_addr.sin_port=htons(8888);	
	
	//3.发起连接
	errif(connect(socket_fd,(sockaddr*)&server_addr,sizeof(server_addr))==-1,"Connect start error");

	//4.向服务器发送数据
	while(true){
		//（1）.定义并初始化缓冲区
		char buf[BUFF_SIZE];     
		bzero(&buf, sizeof(buf));     

		//（2）.向文件描述符写入  
		scanf("%s", buf);   										//键入          
		ssize_t write_bytes = send(socket_fd, buf, sizeof(buf),0);     
		if(write_bytes == -1){          							//send返回-1，表示发生错误
			perror("Socket disconnected");
			break;
		}

		//（3）.读取数据
		bzero(&buf, sizeof(buf));       							 
		ssize_t read_bytes = recv(socket_fd, buf, sizeof(buf),0);    	
		if(read_bytes > 0){
			printf("Receive message from server: %s\n", buf);
		}else if(read_bytes == 0){      							//recv返回0，通常是服务器断开链接
			printf("Server socket disconnected!\n");
			break;
		}else if(read_bytes == -1){     							//read返回-1，表示发生错误
			close(socket_fd);
			errif(true, "Client socket read error");
		}
	}

	//5.关闭连接
	close(socket_fd);
	return 0;
}

