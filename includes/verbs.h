
/* --- Includes ---*/
#ifndef _verbs
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "sys/stat.h"
#include <sys/sendfile.h>
#include "../includes/socket.h"
#include "../includes/http.h"
#include "errno.h"
#include <fcntl.h>
/* --- Defines --- */
#define MAX_CHAR 128
#define LINEARRAYSIZE 20
#define _verbs

/**
 * @brief La funcion devuelve la cabezera asociada a un elemento HTTP
 * 
 * @param parser Parseo de la petición HTTTP asociada
 * @param server_name Alias del servidor
 * @param server_root Raiz del directorio del servidor en local
 * @param file_size Tamaño del archivo a transmitir
 * 
 * @return Cadena de caracteres con la cabecera.
 */
char *HEAD(HttpPetition *parser, char *server_name, char *server_root, off_t file_size);
/**
 * @brief La funcion devuelve la cabezera de un fichero HTTP más las opciones de verbos aplicables al mismo en el servidor y la manda a traves del socket
 * 
 * @param parser Parseo de la petición HTTTP asociada
 * @param server_name Alias del servidor
 * @param server_root Raiz del directorio del servidor en local
 * @param socket Socket asociado a la conexion
 * 
 * @return 
 */
void OPTIONS(HttpPetition *parser, char *server_name, char *server_root, int socket);
/**
 * @brief La funcion devuelve la cabezera de un fichero HTTP más el cuerpo del objeto requerido y los manda a traves del socket. Funciona para archivos y scripts
 * 
 * @param parser Parseo de la petición HTTTP asociada
 * @param server_name Alias del servidor
 * @param server_root Raiz del directorio del servidor en local
 * @param socket Socket asociado a la conexion
 * 
 * @return 
 */
void GET(HttpPetition *parser, char *server_name, char *server_root, int socket);
/**
 * @brief La funcion devuelve la cabezera de un fichero HTTP más el cuerpo del objeto requerido y los manda a traves del socket, la diferencia con get reside en que los argumentos
 * de la peticion vienen dados en el cuerpo de la misma. Funciona para archivos y scripts
 * 
 * @param parser Parseo de la petición HTTTP asociada
 * @param server_name Alias del servidor
 * @param server_root Raiz del directorio del servidor en local
 * @param socket Socket asociado a la conexion
 * 
 * @return 
 */
void POST(HttpPetition *parser, char *server_name, char *server_root, int socket);

/**
 * @brief ERROR 400 Bad Request 
 * @param socket Socket donde transmitir el error
 * @param server_name Alias del server
 */
void error400(int socket, char *server_name);
/**
 * @brief ERROR 404 Not Found
 * @param socket Socket donde transmitir el error
 * @param parser Petition parser
 * @param server_name Alias del server
 */
void error404(int socket, HttpPetition *parser, char *server_name);
/**
 * @brief ERROR 500 Internal Error 
 * @param socket Socket donde transmitir el error
 * @param server_name Alias del server
 */
void error500(int socket, char *server_name);

#endif