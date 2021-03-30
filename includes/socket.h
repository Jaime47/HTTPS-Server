#ifndef _socket

#define _socket
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include <confuse.h>

// LAS SIGUIENTES TRES FUNCIONES ESTAN ENCAPSULADAS EN LA FUNCION server_ini

//Funcion de encapsulamiento (utlizando sys/socket.h)
//int socket(int family, int type, int protocol);
//Funcion de encapsulamiento (utlizando sys/socket.h)
//int bind(int socket, struct sockaddr * localaddr, int addrlen);
//Funcion de encapsulamiento (utlizando sys/socket.h)
//int listen(int socket, int queue_size);
//Funcion de encapsulamiento (utlizando sys/socket.h)


int accept( int socket, struct sockaddr * foreignaddr, int addrlen);
//Funcion de encapsulamiento (utlizando sys/socket.h)
int connect(int sockfd, struct sockaddr * foreignaddrr, int addrlen);
//Funcion de encapsulamiento (utlizando sys/socket.h)
int close(int socket);
//Funcion de encapsulamiento (utlizando sys/socket.h)
int send(int socket, void *buf, size_t len, int flags);
//Funcion de encapsulamiento (utlizando sys/socket.h)
int read(int fd, void *buf, size_t count);
/**
 * Funcion: httpParser : Ajusta una cadena de texto para ser procesada por el servidor
 * Argumentos:
 * 
 * Retorno:
**/

/**
 * Funcion: server_ini : Inicializa un servidor asignandole socket y comenzando el proceso de recepcion
 * Argumentos:
 * addrlen: Puntero que registra el tamaño de la direccion del socket, se usa en el main.
 * Retorno:
 **/
int server_ini(socklen_t * addrlen, cfg_t * conf);

/**
 * Funcion: process_request : Recibe un descriptor de fichero y procesa el mensaje en el servidor
 * Argumentos:
 *  connfd: Descriptor de fichero 
 * Retorno:
 * 
 **/
void process_request(int connfd);

/**
 * Funcion: conf_parser : Lee el archivo de configuracion y parsea sus elementos clave-valor
 * Argumentos:
 * Retorno:
 **/

void conf_parser();

#endif