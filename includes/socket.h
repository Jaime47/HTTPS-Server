#ifndef _socket

#define _socket


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>

//Funcion de encapsulamiento (utlizando sys/socket.h)
int socket(int family, int type, int protocol);
//Funcion de encapsulamiento (utlizando sys/socket.h)
int bind(int socket, struct sockaddr * localaddr, int addrlen);
//Funcion de encapsulamiento (utlizando sys/socket.h)
int listen(int socket, int queue_size);
//Funcion de encapsulamiento (utlizando sys/socket.h)
//int accept( int socket, struct sockaddr * foreignaddr, int addrlen);
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
 * 
 * Retorno:
 **/
int server_ini(socklen_t * addrlen);
/**
 * Funcion: accept_connection : Acepta una peticion de conexion al server
 * Argumentos:
 * sockval: Identificador de un socket
 * Retorno:
 **/
void accept_connection (int sockval);
/**
 * Funcion: launch_service: Una vez que se acepta una conexion, se lanza para recibir el mensaje en el servidor
 * Argumentos:
 * connval: Retorno de la funcion accept (accept_connection)
 * Retorno:
 **/
void launch_service(int connval);

#endif