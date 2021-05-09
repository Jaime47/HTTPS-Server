
/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
#include "../includes/socket.h"

/* ---- Defines ---- */




/* ---- Private Methods ---- */
HttpPetition * httpPetition_ini(char * method, char * path, int petitionLength, int minorVersion, int methodLength, int path_len, int num_headers, struct phr_header * headers, char * objectType, int path_List_size, char ** pathList, int body_list_size, char ** bodyList);
/**
  * @brief Inicializa la estructura de datos HttpPetition
  *
  * @param metodo Metodo de la peticion
  * @param urn Direccion del recurso solicitado
  * @param http_version Verion del protocolo Http
  *
  * @return puntero a estructura inicializada, NULL en caso de error
  */
HttpPetition * httpPetition_ini(char * method, char * path, int petitionLength, int minorVersion, int methodLength, int path_len, int num_headers, struct phr_header * headers, char * objectType, int path_List_size, char ** pathList, int body_list_size, char ** bodyList){


  HttpPetition *parser = NULL;

  /* CdE sobre los argumentos de la funcion */
  if (!method || !path || !petitionLength || !minorVersion || !methodLength || !path_len || !num_headers || !headers || !objectType)
  {
    if (DEBUG_MODE == TRUE)
      fprintf(stderr, ARG_ERROR, __func__);
    return NULL;
  }

  /* Reserva memoria para la estructura. CdE sobre malloc */
  if (!(parser = (HttpPetition *)malloc(sizeof(HttpPetition))))
  {
    if (DEBUG_MODE == TRUE)
      fprintf(stderr, MEM_ERROR, __func__);
    return NULL;
  }

  /* Inicializa los atributos de la ED. CdE sobre strcpy */
  strcpy(parser->method, method);

  strcpy(parser->path, path);

  parser->minorVetsion = minorVersion;

  parser->methodLength = methodLength;

  parser->path_len = path_len ;

  parser->num_headers = num_headers;


  parser->headers = headers;


  strcpy(parser->objectType,objectType);


    parser->path_list_size = path_List_size;

    parser->body_list_size = body_list_size;


    parser->pathList = pathList;

    parser->bodyList = bodyList;



  return parser;
}

/* ------------------------- */
/**
 * @brief  server_ini : Inicializa un servidor asignandole socket y comenzando el proceso de recepcion
 * 
 * @param addrlen Longitud de la direccion del socket
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server
 * 
 * @return
 **/
int server_ini(socklen_t *addrlen, cfg_t *conf)
{

  int sockvalue;
  struct sockaddr_in Dir;

  if ((sockvalue = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    exit(EXIT_FAILURE);
  }

  Dir.sin_family = AF_INET;
  Dir.sin_port = cfg_getint(conf, "listen_port");
  Dir.sin_addr.s_addr = htonl(INADDR_ANY);
  bzero((void *)&(Dir.sin_zero), 8);

  *addrlen = sizeof(Dir.sin_addr);

  if (bind(sockvalue, (struct sockaddr *)&Dir, sizeof(Dir) < 0))
  {
    exit(EXIT_FAILURE);
  }
  // REVISAR NUMERO DE CONEX PARALELAS
  if (listen(sockvalue, 10) < 0)
  {
    exit(EXIT_FAILURE);
  }

  return sockvalue;
}
/**
 * @brief Recibe un descriptor de fichero y procesa el mensaje en el servidor
 * @param connfd Descriptor de fichero
 * @param conf Estructura del tipo cfg_t con los datos de configuración del server 
 * @return
 *
 **/
void process_request(int connfd, cfg_t *conf)
{
  const int iguales = 0;
  char *server_name;
  char *server_root;
  HttpPetition *parser = NULL;
  /**
    * PARSEO DE LA PETICION;
    * IMPORTANTE Para que el codigo sea compacto los campos del parseo deben ir en una estructura, lista, o similar
  **/
  parser = httpPetition_parser(connfd);

  /**
    * EJECUCIÓN DE LA PETICION
  **/
  server_name = cfg_getstr(conf, "server_signature");
  server_root = cfg_getstr(conf, "server_root");

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
    write(connfd, METHOD_ERROR, sizeof(METHOD_ERROR));
  }

  freeParser(parser);
}
/**
  * @brief Parsea una cadena de texto para ser procesada por el servidor.
  * Inicializa la estructura HttpPetition.
  *
  * @param petition_message Puntero al mensaje con la peticion http del cliente
  *
  * @return estructura en caso de exito, NULL en caso de error
  */
HttpPetition *httpPetition_parser(int socket)
{

  HttpPetition *parser = NULL;
  char buf[4096];
  const char * method;
  const char * path;

  char * modifPath;
  char * modifMethod;


  int pret, minor_version;
  struct phr_header headers[100];
  size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
  ssize_t rret;
  char * objectType;
  char * pathList[20];
  char * bodyList[20];
  int path_list_size;
  int body_list_size;
  int i = 0;
  char * body;
  char * piece;
  
  while (1)
  {
    /* read the request */
    while ((rret = read(socket, buf + buflen, sizeof(buf) - buflen)) == -1);
    if (rret <= 0)
      return NULL;
    prevbuflen = buflen;
    buflen += rret;
    /* parse the request */
    num_headers = sizeof(headers) / sizeof(headers[0]);
    pret = phr_parse_request(buf, buflen, &method, &method_len, &path, &path_len,
                             &minor_version, headers, &num_headers, prevbuflen);


  modifPath = path;
  modifMethod = method;


  
  char *dot = strrchr(modifPath, '.');
  objectType = dot + 1;


  /* Obtener argumentos del path*/
  dot = strtok_r(modifPath,"?", &modifPath);

 while((piece = strtok_r(dot, "&",&dot)) != NULL){
    dot = strtok_r(dot, "&", &dot);
    strtok_r(piece, "=", &piece);
    pathList[i] = strtok_r(piece,"=", &piece);
    i++;
  }
  path_list_size = i;
  i = 0;

  /* Obtener argumentos del cuerpo*/
  body = buf + pret;
  
  while((piece = strtok_r(body, "&", &body)) != NULL){
    body = strtok_r(body, "&", &body);
    strtok_r(piece, "=", &piece);
    bodyList[i] = strtok_r(piece,"=",&piece);
    i++;
  }
  body_list_size = i;

    /* Ahora insertamos todos los elementos en nuestra estructura HttpPetition*/
    parser = httpPetition_ini(modifMethod, modifPath, pret, minor_version, method_len, path_len, num_headers, headers, objectType, path_list_size, pathList, body_list_size, bodyList);

    if (pret > 0)
      break; /* successfully parsed the request */
    else if (pret == -1)
      return NULL;
    /* request is incomplete, continue the loop */
    assert(pret == -2);
    if (buflen == sizeof(buf))
      return NULL;
  
  }
return parser;
}


void freeParser(HttpPetition * parser){
  free(parser->method);
  free(parser->objectType);
  free(parser->path);
  free(parser->bodyList);
  free(parser->pathList);
  free(parser->headers);
  free(parser);
}


 // Acordarse de libera : cfg, server_root y server_signature
