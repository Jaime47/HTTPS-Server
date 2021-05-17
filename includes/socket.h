/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file socket.h
  * @date 11 Mar 2021
  * @brief
  */
 /* ---- Defines ---- */
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
/* --- Includes --- */
#include <errno.h>
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
#include <assert.h>
#include "../includes/conf.h"
#include "../includes/picohttpparser.h"
#include "../includes/verbs.h"
#include "../includes/http.h"

/**
  * @brief Parsea una cadena de texto para ser procesada por el servidor.
  * Inicializa la estructura HttpPetition.
  *
  * @param socket Socket attached to process
  * @param server_name Alias of the server
  * 
  * @return estructura en caso de exito, NULL en caso de error
  */
HttpPetition *httpPetition_parser(int socket, char *server_name);

/**
 * @brief  server_ini : Inicializa un servidor asignandole socket y comenzando el proceso de recepcion
 * 
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server
 * 
 * @return
 **/
int server_ini(cfg_t *conf);

/**
 * @brief Recibe un descriptor de fichero y procesa el mensaje en el servidor
 * @param connfd Descriptor de fichero
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server 
 * @return
 *
 **/
int process_request(int connfd, cfg_t *conf);

/**
 * @brief conf_parser : Lee el archivo de configuracion y parsea sus elementos clave-valor
 * @param
 * @return Structure with server configuration
 **/

cfg_t *conf_parser();
/**
 *@brief Libera el parser
 * @param parser Parser a liberar
 */
void freeParser(HttpPetition *parser);

#endif
