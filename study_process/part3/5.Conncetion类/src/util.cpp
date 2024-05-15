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