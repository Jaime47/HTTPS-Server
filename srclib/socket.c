
#include "socket.h"


int server_ini(socklen_t * addrlen, cfg_t * conf){

    int sockvalue;
    struct sockaddr_in Dir;
    
    if((sockvalue = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        exit(EXIT_FAILURE);
    }

    Dir.sin_family = AF_INET;
    Dir.sin_port = cfg_getstr(conf, "listen_port");
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

void process_request(int connfd){
    char * buffer[3000];
    int read_control;
    //Leemos el mensaje que tiene el cliente.
    read_control = recv(connfd, buffer, 3000, 0);
    if (read_control == 0){
        // Desconectado
        fflush(stdout);
        return;
    }
    else if(read_control == -1){
        //Error
        perror("Failed recv");
    }

    // Ahora hay que parsear y llamar a la funcion que corresponda para responder
}