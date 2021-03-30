#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include "socket.h"


cfg_t * conf_parser(){
    static char * server_root = NULL;
    static char * server_signature = NULL;    
    static long int max_clients;
    static long int listen_port;
    cfg_opt_t opts[] = {
        CFG_SIMPLE_STR("server_root", &server_root),
        CFG_SIMPLE_INT("max_clients", &max_clients),
        CFG_SIMPLE_INT("listen_port", &listen_port),
        CFG_SIMPLE_STR("server_signature", &server_signature),
        CFG_END()
    };
    cfg_t *cfg;
    cfg_init(opts, 0);
    cfg_parse(cfg, "server.conf");
    return cfg;
} // Acordarse de libera : cfg, server_root y server_signature



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
cfg_t * conf;
int nchildren;

conf = conf_parser();

nchildren = cfg_getint(conf, "nchildren");

//listenfd = server_ini(argv[1], argv[2], &addrlen);
listenfd = server_ini(&addrlen, conf);

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