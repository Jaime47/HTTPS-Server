/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
#ifndef _socket
#define _socket
#define _GNU_SOURCE
#define ARG_ERROR "ERROR(%s): Los argumentos de la funcion son erroneos\n"
#define METHOD_ERROR "ERROR(%d): Metodo \"%s\" no soportado\n"
#define RECV_ERROR "ERROR(%d): La lectura de la peticion del cliente ha fallado\n"
#define MEM_ERROR "ERROR(%s): No ha sido posible reservar memoria\n"
#define MAX_BUFF 3000
#define MAX_CHAR 128
#define DEBUG_MODE 0
#define TRUE 1
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
#include <conf.h>
#include "picohttpparser.h"




/* ---- ED Peticion Http ---- */
typedef struct _HttpPetition {
  char * method;
  char * path;
  int petitionLength;
  int minorVetsion;
  int methodLength;
  int path_len;
  int num_headers;
  struct phr_header * headers;
  char * objectType;
  char * pathList;
  char * bodyList;
  int path_list_size;
  int body_list_size;
} HttpPetition;










/**
  * @brief Parsea una cadena de texto para ser procesada por el servidor.
  * Inicializa la estructura HttpPetition.
  *
  * @param petition_message Puntero al mensaje con la peticion http del cliente
  *
  * @return estructura en caso de exito, NULL en caso de error
  */
HttpPetition *httpPetition_parser(int socket);

/**
 * @brief  server_ini : Inicializa un servidor asignandole socket y comenzando el proceso de recepcion
 * 
 * @param addrlen Longitud de la direccion del socket
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server
 * 
 * @return
 **/
int server_ini(socklen_t * addrlen, cfg_t * conf);

/**
 * @brief Recibe un descriptor de fichero y procesa el mensaje en el servidor
 * @param connfd Descriptor de fichero
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server 
 * @return
 *
 **/
void process_request(int connfd, cfg_t * conf);

/**
 * @brief conf_parser : Lee el archivo de configuracion y parsea sus elementos clave-valor
 * @param
 * @return Structure with server configuration
 **/

cfg_t * conf_parser();
void freeParser(HttpPetition * parser);
HttpPetition * httpPetition_ini(char * method, char * path, int petitionLength, int minorVersion, int methodLength, int path_len, int num_headers, struct phr_headers * headers, char * objectType, int path_List_size, char * pathList, int body_list_size, char * bodylist);
#endif
