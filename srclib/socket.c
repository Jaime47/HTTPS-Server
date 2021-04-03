
#include "socket.h"

/* ---- Defines ---- */
#define ARG_ERROR "ERROR(%s): Los argumentos de la funcion son erroneos\n"
#define METHOD_ERROR "ERROR(%d): Metodo \"%s\" no soportado\n"

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

void process_request(int connfd, cfg_t * conf){
    char * buffer[3000];
    int read_control, resultado;
    const int iguales = 0;
    char * server_name;
    char * server_root;

    //Leemos el mensaje que tiene el cliente.
    read_control = recv(connfd, buffer, 3000, 0);
    if (read_control == 0){
        // Desconectado
        fflush(stdout);
        return;
    }
    else if(read_control == -1){
        //Error
        perror("Failed recv");
    }

    /**
     * PARSEO DE LA PETICION;
     * IMPORTANTE Para que el codigo sea compacto los campos del parseo deben ir en una estructura, lista, o similar
     **/
    
    /**
     * EJECUCIÓN DE LA PETICION 
     **/
    server_name = cfg_getstr(conf,"server_signature");
    server_root = cfg_getstr(conf,"server_root");
    /* Comprobamos que el metodo de la peticion sea valido */
	if (strcmp(metodo, "GET") == iguales) {
		resultado = GET(); /* TODO Procesar GET */
	} else if (strcmp(metodo, "POST") == iguales) {
		resultado = POST(); /* TODO Procesar POST */
	} else if (strcmp(metodo, "OPTIONS") == iguales) {
		resultado = OPTIONS(,server_name,server_root,connfd); /* TODO Procesar OPTIONS */
	} else {
		fprintf(stderr, METHOD_ERROR, __func__, metodo);
		resultado = 400;
	}



    /* En caso de error, enviar mensaje al cliente */
	if (resultado==301) { /* Moved Permanently */
		/* Enviar mensaje */
		/* Liberar memoria */
	} else if (resultado==400) { /* Bad Request */
		/* Enviar mensaje */
		/* Liberar memoria */
	} else if (resultado==404) { /* Not Found */
		/* Enviar mensaje */
		/* Liberar memoria */
	} else if (resultado==505) { /* HTTP Version Not Supported */
		/* Enviar mensaje */
		/* Liberar memoria */
	}
}