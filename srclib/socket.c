
#include "socket.h"


int server_ini(socklen_t * addrlen){

    int sockvalue;
    struct sockaddr_in Dir;
    
    if((sockvalue = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        exit(EXIT_FAILURE);
    }

    Dir.sin_family = AF_INET;
    //Dir.sin_port = htons(NFC_SERVER_PORT);
    Dir.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero((void*)&(Dir.sin_zero), 8);

    *addrlen = sizeof(Dir.sin_addr);


    if(bind(sockvalue, (struct sockaddr *)&Dir, sizeof(Dir) < 0)){
        exit(EXIT_FAILURE);
    }

    if(listen(sockvalue, NULL) < 0){
        exit(EXIT_FAILURE);
    }

    return sockvalue;
}

void conf_parser(){
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
}