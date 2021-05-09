#ifndef _verbs
#define _verbs
#include<stdio.h>
#include<unistd.h>
#include <time.h>
#include"sys/stat.h"
#include "../includes/socket.h"
#include "../includes/http.h"
#define MAX_CHAR 128
#define LINEARRAYSIZE 20



/**
 * @brief La funcion devuelve la cabezera asociada a un elemento HTTP
 * 
 * @param parser Parseo de la petición HTTTP asociada
 * @param server_name Alias del servidor
 * @param server_root Raiz del directorio del servidor en local
 * @param socket Socket asociado a la conexion
 * 
 * @return Cadena de caracteres con la cabecera.
 */
char * HEAD (HttpPetition * parser,char*server_name, char* server_root, int socket);
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
void OPTIONS(HttpPetition * parser, char *server_name, char * server_root, int socket);
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
void GET(HttpPetition * parser,char *server_name, char * server_root, int socket);
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
void POST(HttpPetition * parser,char *server_name, char * server_root, int socket);
/**
 * @brief La funcion recibe una localización local del archivo a nivel local, lo abre y lo lee devolviendolo como puntero a char.
 * 
 * @param location Localizacion del archivo a nivel local
 * @return Archivo leido
 */
char * readFile(char * location);
/**
 * @brief La funcion interpreta scripts .py y .php, los ejecuta por terminal y lee su salida estandar.
 * 
 * @param parser Parseo de la peticion http asociada
 * 
 * @return Cadena de caracteres de la estandar output del servidor.
 */
char * scriptInterpreter(HttpPetition * parser);

#endif