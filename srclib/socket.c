#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>





//MODIFICACION NECESARIA
int server_ini(){

    int sockvalue;
    struct sockaddr_in Dir;
    
    if((sockvalue = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        exit(EXIT_FAILURE);
    }

    Dir.sin_family = AF_INET;
    //Dir.sin_port = htons(NFC_SERVER_PORT);
    Dir.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero((void*)&(Dir.sin_zero), 8);


    if(bind(sockvalue, (struct sockaddr *)&Dir, sizeof(Dir) < 0)){
        exit(EXIT_FAILURE);
    }

    return sockvalue;
}

void accept_connection (int sockval){
    int desc;
    struct sockaddr Link;
    socklen_t len;
    
    len = sizeof(Link);

    if(desc = accept(sockval,&Link,&len) <0){
        exit(EXIT_FAILURE);
    }

    launch_service(desc);
    //wait();
    return;
}

void launch_service(int connval){
    int pid;
    long type, aux;
    pid = fork();
    if(pid < 0) exit (EXIT_FAILURE);
    if (pid == 0) return;

    syslog (LOG_INFO, "New access");
    recv(connval, &aux, sizeof(long), 0);
    type = ntohl(aux);


    //database_access(connval, type, NULL);
    close(connval);
    syslog (LOG_INFO, "Exiting service");
    exit(EXIT_SUCCESS);
}