
#include "socket.h"

//MODIFICACION NECESARIA
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

    return sockvalue;
}

