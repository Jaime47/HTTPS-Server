/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
#include "../includes/verbs.h"



char * fileTypeSwitch(HttpPetition * parser);

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
char * HEAD (HttpPetition * parser,char*server_name, char* server_root, int socket){
    char  * url;
    char timechar[50];
    time_t t;
    char * aux = NULL;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    char * response = NULL;
    
    url = strcat(server_root,parser->path);

    //Last-Modified
    struct stat attrib;
    stat(url, &attrib);
    strftime(timechar, 50, "%Y-%m-%d %H:%M:%S\n", localtime(&attrib.st_mtime));
    sprintf(response,"Last-Modified: %s", timechar);
    // Date
    sprintf(aux,"Date: %d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    strcat(response, aux);
    // Content-type
    sprintf(aux,"File-Type: %s\n",fileTypeSwitch(parser));
    strcat(response, aux);
    //Server name
    sprintf(aux, "Server: %s", server_name);
    strcat(response, aux);
    //Content length
    sprintf(aux, "File-length: %ld", attrib.st_size);
    strcat(response,aux);

    return response;

    }


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
void OPTIONS(HttpPetition * parser, char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    char * head;

    url = strcat(server_root,parser->path);

    // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }
    //Allow
    strcat(response,"Allow: GET, POST, OPTIONS\n");
    head = HEAD(parser, server_name, server_root,socket);
    strcat(response, head);
    //Sending response

    write(socket, response, sizeof(response));
    return;
}
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
void GET(HttpPetition * parser,char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    char * head;
    char * body;
    
    url = strcat(server_root,parser->path);
    char * ot = fileTypeSwitch(parser);

    // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }

    //HEAD
    head = HEAD(parser, server_name, server_root,socket);

    strcat(response, head);

    strcat(response, "<html><body><h1>");

    if(strcmp(ot, "py") == 0 ||strcmp(ot, "py") == 0){
    //BODY OBTENTION
    body = scriptInterpreter(parser);
    }
    else{
    //BODY OBTENTION
    body = readFile(url);
    }


    strcat(response, body);
    strcat(response, "</h1,/body,/html>");
    write(socket, response, sizeof(response));
    free(body);
    
    return;
}
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
void POST(HttpPetition * parser,char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    char * head;
    char * body;
    
    url = strcat(server_root, parser->path);
    //url = strcat(url, parser->headers[0].value); DONDE INTRODUCES LAS VARIABLES DEL CUERPO

    char * ot = fileTypeSwitch(parser);

        // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }

    //HEAD
    head = HEAD(parser, server_name, server_root,socket);

    strcat(response, head);

    strcat(response, "<html><body><h1>");

    if(strcmp(ot, "py") == 0 ||strcmp(ot, "py") == 0){
    //BODY OBTENTION
    body = scriptInterpreter(parser);
    }
    else{
    //BODY OBTENTION
    body = readFile(url);
    }


    strcat(response, body);
    strcat(response, "</h1,/body,/html>");
    write(socket, response, sizeof(response));
    free(body);
    return;

    


}

/**
 * @brief La funcion recibe una localización local del archivo a nivel local, lo abre y lo lee devolviendolo como puntero a char.
 * 
 * @param location Localizacion del archivo a nivel local
 * @return Archivo leido
 */
char * readFile(char * location){

    struct stat attrib;
    stat(location, &attrib);
    int i;
    int length;
    char * buffer;
    length = attrib.st_size;
    FILE *file = fopen(location, "r");

    buffer = malloc(sizeof(length));


    if (file == NULL)
    return NULL; //could not open file


    while(i=fread(buffer,1,length,file), length >0 && i != 0 ){
       if(i == -1){
           free(buffer);
        //   free(attrib);
           return NULL;
       }
       buffer += i;
       length -= i;
   }

    return buffer;
}


/**
 * @brief La funcion interpreta scripts .py y .php, los ejecuta por terminal y lee su salida estandar.
 * 
 * @param parser Parseo de la peticion http asociada
 * 
 * @return Cadena de caracteres de la estandar output del servidor.
 */
char * scriptInterpreter(HttpPetition * parser){
    char * command;
    char reader[1024];
    char * output;
    int i = 0;
    char * ot;
    ot = fileTypeSwitch(parser);

    
    if(strcmp(ot, "py") == 0){

        command = "python3 ";
        strcat(command, parser->path);
        system(command);

    }

    else if(strcmp(ot, "php") == 0){

        command = "php ";
        strcat(command, parser->path);
        system(command);

    }

    FILE *fp = popen(command, "r");
    if (!fp)
    {
        return NULL;
    }

    output = malloc(sizeof(reader) * LINEARRAYSIZE);
    
    while (strcmp(fgets(reader, sizeof(reader), fp), "\r\n") != 0)
    {
        strcat(output, reader);
        if (i >= LINEARRAYSIZE){
            output = realloc(output, 1024 * (LINEARRAYSIZE+i));
        }
        i++;
    }

    return output;


}



char * fileTypeSwitch(HttpPetition * parser){

    char * ext = parser->objectType;

    if (strcmp(ext, "txt") == 0) 
    {
        return "text/plain";
    } 
else if (strcmp(ext, "htm") == 0)
    {
        return "text/html";
    }
else if (strcmp(ext, "gif") == 0)
    {
        return "image/gif";
    }
else if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpe") == 0) 
    {
        return "image/jpeg";
    }
else if (strcmp(ext, "mpe") == 0 || strcmp(ext, "mpg") == 0) 
    {
        return "video/mpeg";
    }
else if (strcmp(ext, "doc") == 0)
    {
        return "application/msword";
    }
else if (strcmp(ext, "pdf") == 0)
    {
        return "application/pdf";
    }
else if (strcmp(ext, ".py") == 0)
{
        return "py";
}
else if (strcmp(ext, "php") == 0)
    {
        return "php";
    }
else 
    {
        return "ERROR";
    }

}