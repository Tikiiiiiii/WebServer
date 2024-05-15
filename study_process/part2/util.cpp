#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(1);
    }
}
void setnonblocking(int fd){
    //利用fcntl先获取再设置
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}