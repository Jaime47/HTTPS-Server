/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
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
#include <conf.h>




/**
 * @brief La funcion se encarga de definir el comportamiento de cada proceso hijo de modo que reciba conexiones desde su socket asignado
 * 
 * @param i Numero del subproceso hijo
 * @param listenfd Identificador de la conexion
 * @param addrlen Longitud de la address
 * @param conf Archivo de configuracion del server
 * 
 * @return
 */
void child_main(int i, int listenfd, int addrlen, cfg_t * conf){
    int clilen, connfd;
    
    while(1){
            clilen = addrlen;
            my_lock_wait();
            connfd = accept(listenfd,NULL ,NULL);
            my_lock_release();

            process_request(connfd, conf);
            close(connfd);
            
    }
}

/**
 * 
 *@brief Main: Proceso principal que inicializa el servidor, lee el archivo de configuracion y comienza a crear los procesos hijos de este. 
 * 
 * 
 */
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
            child_main(i, listenfd,addrlen,conf);
        }

    }

    while(1){
        pause();
    }
}