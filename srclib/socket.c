
/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
#include "../includes/socket.h"

/* ---- Defines ---- */

int DEBUG_FLAG = 0;

/* ---- Private Methods ---- */
/**
 * @brief Metodo que reemplaza todos los chars de una string 
 * @param str String para reemplazo
 * @param find Caracter a reemplazar
 * @param replace Caracter por el que reemplazar a find
 * @return String con el caracter reemplazado
 * @author https://stackoverflow.com/users/3537677/superlokkus
 */
char *replace_char(char *str, char find, char replace);

/* ------------------------- */
/**
 * @brief  server_ini : Inicializa un servidor asignandole socket y comenzando el proceso de recepcion
 * 
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server
 * 
 * @return
 **/
int server_ini(cfg_t *conf)
{

  int sockvalue;
  struct sockaddr_in Dir;

  if ((sockvalue = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("Error open socket");
    syslog(LOG_INFO, "Socket error");
    exit(EXIT_FAILURE);
  }
  printf("Este es el listen port %ld\n", cfg_getint(conf, "listen_port"));
  bzero(&(Dir), sizeof(Dir));
  Dir.sin_family = AF_INET;
  Dir.sin_port = htons(cfg_getint(conf, "listen_port"));
  Dir.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockvalue, (struct sockaddr *)&Dir, sizeof(Dir)) < 0)
  {
    printf("Error binding, please try another port\n");
    syslog(LOG_INFO, "Binding error");
    close(sockvalue);
    exit(EXIT_FAILURE);
  }

  listen(sockvalue, cfg_getint(conf, "max_clients"));
  printf("Socket escuchando: %d\n", htons(Dir.sin_port));

  return sockvalue;
}
/**
 * @brief Recibe un descriptor de fichero y procesa el mensaje en el servidor
 * @param connfd Descriptor de fichero
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server 
 * @return
 *
 **/
int process_request(int connfd, cfg_t *conf)
{
  const int iguales = 0;
  char *server_name;
  char *server_root;
  HttpPetition *parser = NULL;

  /**
    * EJECUCIÓN DE LA PETICION
  **/
  server_name = cfg_getstr(conf, "server_signature");
  server_root = cfg_getstr(conf, "server_root");

  /**
    * PARSEO DE LA PETICION;
    * IMPORTANTE Para que el codigo sea compacto los campos del parseo deben ir en una estructura, lista, o similar
  **/
  parser = httpPetition_parser(connfd, server_name);
  if (parser == NULL)
  {
    return 0;
  }

  /* Comprobamos que el metodo de la peticion sea valido */
  if (strcmp(parser->method, "GET") == iguales)
  {
    GET(parser, server_name, server_root, connfd); /* TODO Procesar GET */
  }
  else if (strcmp(parser->method, "POST") == iguales)
  {
    POST(parser, server_name, server_root, connfd); /* TODO Procesar POST */
  }
  else if (strcmp(parser->method, "OPTIONS") == iguales)
  {
    OPTIONS(parser, server_name, server_root, connfd); /* TODO Procesar OPTIONS */
  }
  else
  {
    error400(connfd, server_name);
  }
  if (parser->closed == 0)
  {
    freeParser(parser);
    return 1;
  }
  freeParser(parser);
  return 0;
}
/**
  * @brief Parsea una cadena de texto para ser procesada por el servidor.
  * Inicializa la estructura HttpPetition.
  *
  * @param socket Socket attached to process
  * @param server_name Alias of the server
  * 
  * @return estructura en caso de exito, NULL en caso de error
  */
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
HttpPetition *httpPetition_parser(int socket, char *server_name)
{
  /*Variables*/
  HttpPetition *parser = NULL;
  char buf[4096];
  const char *method;
  const char *path;
  char *modifPath;
  int pret, minor_version;
  struct phr_header headers[100];
  size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
  ssize_t rret;
  char *objectType;

  char *body;
  char auxpath[400];
  char *piece;
  char *dot;
  /*Obtencion de la request*/
  while (1)
  {
    /* read the request */
    memset(buf, 0, 4096);
    while ((rret = read(socket, buf + buflen, sizeof(buf) - buflen)) == -1)
      ;
    if (rret < 0)
      return NULL;
    /*If someone return 0 it means they want conexion closed*/
    if (rret == 0)
    {
      error500(socket, server_name);
      return NULL;
    }

    prevbuflen = buflen;
    buflen += rret;
    /* parse the request */
    num_headers = sizeof(headers) / sizeof(headers[0]);
    pret = phr_parse_request(buf, buflen, &method, &method_len, &path, &path_len,
                             &minor_version, headers, &num_headers, prevbuflen);
    if (pret > 0)
      break; /* successfully parsed the request */
    else if (pret == -1)
    {
      error400(socket, server_name);
      return NULL;
    }
    /* request is incomplete, continue the loop */
    assert(pret == -2);
    if (buflen == sizeof(buf))
    {
      error400(socket, server_name);
      return NULL;
    }
  }

  /*Comenzamos a guardar datos relevantes en la estructura*/
  /*Comenzamos inicializando la estructura*/
  parser = (HttpPetition *)calloc(1, sizeof(HttpPetition));

  /*Parseo metodo*/
  strncpy(parser->method, method, method_len);
  /*Parseo longitud metodo*/
  parser->methodLength = method_len;
  /*Parseo longitud path*/
  parser->path_len = path_len;
  /*Parseo version*/
  parser->minorVersion = minor_version;

  /*Realizamos copia auxiliar del path extendido*/
  memset(auxpath, 0, 400);
  strncpy(auxpath, path, path_len);
  modifPath = auxpath;

  /* Obtener argumentos del path*/

  /*Reusamos dot*/
  /*Parte del path que no interesa*/
  dot = strtok_r(modifPath, "?", &modifPath);
  /*Aprovechamos que hemos cortado los argumentos para obtener path corto*/
  if (strcmp(dot, "/") == 0)
  {
    strcpy(parser->path, "/index.html");
    strcpy(parser->objectType, "html");
  }
  else
  {
    strcpy(parser->path, dot);

    /*Obtener extension del archivo*/
    objectType = strrchr(dot, '.');
    objectType = objectType + 1;
    strcpy(parser->objectType, objectType);
  }

  /** Reseteamos memoria*/

  memset(parser->pathList, 0, 150);
  memset(parser->pathList, 0, 150);

  /*Parte de los argumentos en PATH*/
  while (1)
  {
    dot = strtok_r(NULL, "=", &modifPath);
    piece = strtok_r(NULL, "&", &modifPath);
    if (piece == NULL)
      break;
    strcat(parser->pathList, piece);
    strcat(parser->pathList, " "); /*Se queda un espacio de mas al final*/
  }

  /* Obtener argumentos del BODY*/
  body = buf + pret;
  dot = strtok_r(body, "=", &body);
  while (1)
  {
    piece = strtok_r(NULL, "&", &body);
    if (piece == NULL)
      break;
    strcat(parser->bodyList, piece);
    strcat(parser->bodyList, " ");
    dot = strtok_r(NULL, "=", &body);
  }

  /*Reemplazamos "+"" por " "*/
  replace_char(parser->pathList, '+', ' ');
  replace_char(parser->bodyList, '+', ' ');
  parser->path_list_size = strlen(parser->pathList);
  parser->body_list_size = strlen(parser->bodyList);

  /*Por ultimo asignamos cabeceras y longitud de las mismas*/
  if (strstr(headers[0].value, "keep-alive"))
  {
    parser->closed = 0;
  }
  else
  {
    parser->closed = 1;
  }

  return parser;
}
/**
 *@brief Libera el parser
 * @param parser Parser a liberar
 */
void freeParser(HttpPetition *parser)
{
  free(parser);
  return;
}

char *replace_char(char *str, char find, char replace)
{
  char *current_pos = strchr(str, find);
  while (current_pos)
  {
    *current_pos = replace;
    current_pos = strchr(current_pos, find);
  }
  return str;
}
