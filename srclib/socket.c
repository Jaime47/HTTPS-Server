
#include "socket.h"

/* ---- Defines ---- */
#define ARG_ERROR "ERROR(%s): Los argumentos de la funcion son erroneos\n"
#define METHOD_ERROR "ERROR(%d): Metodo \"%s\" no soportado\n"
#define RECV_ERROR "ERROR(%d): La lectura de la peticion del cliente ha fallado\n"
#define MEM_ERROR "ERROR(%s): No ha sido posible reservar memoria\n"
#define MAX_BUFF 3000
#define MAX_CHAR 128
#define DEBUG_MODE FALSE

/* ---- ED Peticion Http ---- */
typedef struct _HttpPetition {
  char method[MAX_CHAR];
  char urn[MAX_CHAR];
  char http_version[MAX_CHAR];
} HttpPetition;

/* ---- Private Methods ---- */

/**
  * @brief Inicializa la estructura de datos HttpPetition
  *
  * @param metodo Metodo de la peticion
  * @param urn Direccion del recurso solicitado
  * @param http_version Verion del protocolo Http
  *
  * @return puntero a estructura inicializada, NULL en caso de error
  */
HttpPetition *httpPetition_ini(char *metodo, char* urn, char* httpVersion);

HttpPetition *httpPetition_ini(char *metodo, char* urn, char* httpVersion) {

  HttpPetition *parser = NULL;

  /* CdE sobre los argumentos de la funcion */
  if (!metodo || !urn || !httpVersion) {
    if (DEBUG_MODE==TRUE) fprintf(stderr, ARG_ERROR, __func__);
    return NULL;
  }

  /* Reserva memoria para la estructura. CdE sobre malloc */
  if (!(parser = (HttpPetition*)malloc(sizeof(HttpPetition)))) {
    if (DEBUG_MODE==TRUE) fprintf(stderr, MEM_ERROR, __func__);
    return NULL;
  }

  /* Inicializa los atributos de la ED. CdE sobre strcpy */
  if (strcpy(parser->method, "")==NULL) { /* Metodo http */
    if (DEBUG_MODE==TRUE) fprintf(stderr, MEM_ERROR, __func__);
    free(parser); return NULL;
  }
  if (strcpy(parser->urn, "")==NULL) { /* URN de la peticion */
    if (DEBUG_MODE==TRUE) fprintf(stderr, MEM_ERROR, __func__);
    free(parser); return NULL;
  }
  if (strcpy(parser->http_version, "")==NULL) { /* Version http */
    if (DEBUG_MODE==TRUE) fprintf(stderr, MEM_ERROR, __func__);
    free(parser); return NULL;
  }

  return parser;
}

/* ------------------------- */

int server_ini(socklen_t * addrlen, cfg_t * conf){

    int sockvalue;
    struct sockaddr_in Dir;

    if((sockvalue = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        exit(EXIT_FAILURE);
    }

    Dir.sin_family = AF_INET;
    Dir.sin_port = cfg_getstr(conf, "listen_port");
    Dir.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero((void*)&(Dir.sin_zero), 8);

    *addrlen = sizeof(Dir.sin_addr);

    if(bind(sockvalue, (struct sockaddr *)&Dir, sizeof(Dir) < 0)){
        exit(EXIT_FAILURE);
    }

    if(listen(sockvalue, NULL) < 0){
        exit(EXIT_FAILURE);
    }

    return sockvalue;
}

void process_request(int connfd, cfg_t * conf) {

  char * buffer[MAX_BUFF];
  int read_control, resultado;
  const int iguales = 0;
  char * server_name;
  char * server_root;
  HttpPetition *parser = NULL;

  // Leemos el mensaje que tiene el cliente.
  if ((read_control = recv(connfd, buffer, MAX_BUFF, 0))==0) {
  // Desconectado
  fflush(stdout);
    return;
  }
  else if (read_control==-1){
    // Error
    perror("Failed recv");
  }

  /**
    * PARSEO DE LA PETICION;
    * IMPORTANTE Para que el codigo sea compacto los campos del parseo deben ir en una estructura, lista, o similar
  **/
  parser = httpPetition_parser(read_control);

  /**
    * EJECUCIÓN DE LA PETICION
  **/
  server_name = cfg_getstr(conf,"server_signature");
  server_root = cfg_getstr(conf,"server_root");

  /* Comprobamos que el metodo de la peticion sea valido */
	if (strcmp(parser->method, "GET") == iguales) {
		resultado = GET(parser,server_name,server_root,connfd); /* TODO Procesar GET */
	} else if (strcmp(parser->method, "POST") == iguales) {
		resultado = POST(parser,server_name,server_root,connfd); /* TODO Procesar POST */
	} else if (strcmp(parser->method, "OPTIONS") == iguales) {
		resultado = OPTIONS(parser,server_name,server_root,connfd); /* TODO Procesar OPTIONS */
	} else {
		fprintf(stderr, METHOD_ERROR, __func__, metodo);
		resultado = 400;
	}


  free(parser);
  //  /* En caso de error, enviar mensaje al cliente */
	//if (resultado==301) { /* Moved Permanently */
	//	/* Enviar mensaje */
	//	/* Liberar memoria */
	//} else if (resultado==400) { /* Bad Request */
	//	/* Enviar mensaje */
	//	/* Liberar memoria */
	//} else if (resultado==404) { /* Not Found */
	//	/* Enviar mensaje */
	//	/* Liberar memoria */
	//} else if (resultado==505) { /* HTTP Version Not Supported */
	//	/* Enviar mensaje */
	//	/* Liberar memoria */
	//}
}

HttpPetition *httpPetition_parser(char *petition_message) {

  HttpPetition *parser = NULL;
  char *blankSpace = " ", *crlf = "\r\n", *token = NULL;
  char *metodo = NULL, *urn = NULL, *version = NULL;

  if (!petition_message) { /* CdE sobre los argumentos de la funcion */
    if (DEBUG_MODE==TRUE) fprintf(stderr, ARG_ERROR, __func__);
    return NULL;
  }

  /* Obtiene el metodo, la URN y la version http */
	token = strtok(parser, blankSpace);
	metodo = token;
	token = strtok(NULL, blankSpace);
	urn = token; /* TODO: Obtener argumentos */
	token = strtok(NULL, blankSpace);
	toke = strtok(token, crlf)
	version = token;
	token = strtok(NULL, blankSpace);

  /* Crea la peticion. CdE sobre http_petition_ini */
  if ((parser = httpPetition_ini(&metodo, &urn, &version))==NULL) {
    if (DEBUG_MODE==TRUE) fprintf(stderr, MEM_ERROR, __func__);
    return NULL;
  }

  return parser;
}
