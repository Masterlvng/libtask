#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int port, rport;
int srv_fd, cli_fd;
char remote[16];

enum
{
    STACK = 32768
};

void echotask(void* fd)
{
    int n;
    char buf[128];

    int cfd = (int)fd;
    fdnoblock(cfd);
    while((n = fdread(cfd, buf, sizeof buf)) > 0)
        fdwrite(cfd, buf, 128);
    close(cfd);
}

void
taskmain(int argc, char **argv)
{
    if(argc != 2)
    {
       fprintf(stderr, "usage: echo-server port\n");
       taskexitall(1);
    }
    port = atoi(argv[1]);
    srv_fd = netannounce(1, "localhost", port);
    fdnoblock(srv_fd);
    while((cli_fd = netaccept(srv_fd, remote, &rport)) >= 0){
        taskcreate(echotask, (void*)cli_fd, STACK);
    }
}
