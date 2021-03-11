#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>



void child_main(int i, int listenfd, int addrlen){
    int clilen, connfd;
    
    while(1){
            clilen = addrlen;
            my_lock_wait();
            connfd = accept(listenfd,NULL ,NULL);
            my_lock_release();

            process_request(connfd);
            close(connfd);
            
    }
}


int main(int argc, char *argv)
{

int listenfd, i, childpid;
socklen_t addrlen;
int nchildren = 50;

//listenfd = server_ini(argv[1], argv[2], &addrlen);
listenfd = server_ini(&addrlen);

my_lock_init(NULL);

for(i = 0; i < nchildren; i++)
    {

        if((childpid = fork()) == 0){
            child_main(i, listenfd,addrlen);
        }

    }

    while(1){
        pause();
    }
}