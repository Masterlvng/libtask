#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum{
    STACK = 65535
};

int port;

void send(void* v)
{
    int fd, n;
    char buf[128];
    for(;;){
        if((fd = netdial(1, "localhost", port)) < 0){
            continue;
        }
        snprintf(buf, sizeof buf, "hello");
        fdwrite(fd, buf, sizeof buf);
        while((n = fdread(fd, buf, 128)) > 0)
            ;
        close(fd);
        write(1, buf, sizeof buf);
    }
}

void taskmain(int argc, char** argv)
{
    if(argc != 2)
    {
        taskexitall(1);
    }
    port = atoi(argv[1]);
    taskcreate(send, 0, STACK);
}
