/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */

/* --- Includes --- */
#include "../includes/verbs.h"


/**
 * @brief Funcion que define el tipo de archivo a partir de la extension
 * @param parser Parser de peticion
 * @return Puntero a char conteniendo el tipo de archivo p.e "text/plain"
 * 
 */
char *fileTypeSwitch(HttpPetition *parser);

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
char *HEAD(HttpPetition *parser, char *server_name, char *server_root, off_t file_size)
{
    char url[300];
    char *response;
    char lmodified[100];
    char requestDate[100];
    time_t tim = time(0);

    response = (char *)calloc(500, 1);
    struct stat attrib;
    sprintf(url, "%s%s", server_root, parser->path);
    stat(url, &attrib);

    //Last-Modified
    strftime(lmodified, 100, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&attrib.st_mtime));
    // Date
    /*Obtenemos la fecha de la request*/
    struct tm *getTime = gmtime(&tim);
    strftime(requestDate, 100, "%a, %d %b %Y %H:%M:%S %Z", getTime);
    /*Standard header for GET/POST files*/
    sprintf(response, "HTTP/1.%d 200 OK\r\nDate: %s\r\nServer: %s\r\nContent-Length: %ld\r\nLast-Modified: %s\r\nContent-Type: %s\r\n\r\n",
            parser->minorVersion, requestDate, server_name, file_size, lmodified, fileTypeSwitch(parser));

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
void OPTIONS(HttpPetition *parser, char *server_name, char *server_root, int socket)
{

    char response[500];
    char requestDate[100];
    time_t tim = time(0);
    struct tm *getTime = gmtime(&tim);
    /*Obtenemos la fecha de la request*/
    strftime(requestDate, 100, "%a, %d %b %Y %H:%M:%S %Z", getTime);
    //url = strcat(server_root,parser->path);
    /* Ahora nos toca montar la respuesta de Options*/
    sprintf(response, "HTTP/1.%d 200 OK\r\nDate: %s\r\nServer: %s\r\nContent-Length: 0\r\nAllow: GET, POST, OPTIONS\r\n", parser->minorVersion, requestDate, server_name);
    //Sending response
    write(socket, response, strlen(response));
    /*Resetear el buffer de respuesta para las siguientes peticiones*/
    memset(response, 0, strlen(response));
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
void GET(HttpPetition *parser, char *server_name, char *server_root, int socket)
{
    char imputScript[400];
    char *head;
    char url[200];
    FILE *outputScript;
    FILE *file;
    int fileLinuxDescriptor;
    char *buffer = NULL;
    int size;
    int i = 1;
    snprintf(url, 200, "%s%s", server_root, parser->path);

    /** Si get tiene argumentos deberia ser un script asi que en ese caso comprobamos el parser*/
    if (parser->path_list_size != 0)
    {
        if (strstr(parser->objectType, "py"))
        {
            sprintf(imputScript, "python3 %s %s", url, parser->pathList);
        }

        else if (strstr(parser->objectType, "php"))
        {
            sprintf(imputScript, "php %s \"%s\"", url, parser->pathList);
        }

        /** Como metodo para obtener el script lo almacenamos en un archivo con descriptor generado automaticamente*/
        outputScript = popen(imputScript, "r");
        if (outputScript == NULL)
        {
            syslog(LOG_INFO, "ERROR: Script exec\n");
            error404(socket, parser, server_name);
            close(socket);
        }

        /**Toca mandar una request a head para que nos de la cabecera, para ello leemos el file_size*/
        fileLinuxDescriptor = fileno(outputScript);
        sleep(3);

        do
        {
            buffer = realloc(buffer, 2000 * i);
            memset(buffer, 0, 2000);
            size += read(fileLinuxDescriptor, buffer, 2000);
            i++;
        } while (size == 2000*(i-1));

        head = HEAD(parser, server_name, server_root, size);

        /**Mensaje ensamblado*/
        write(socket, head, strlen(head));
        write(socket, buffer, size);

        pclose(outputScript);
    }

    /**If there are no arguments in path we may suppose we have normal get request*/
    else
    {
        file = fopen(url, "r");
        /**Checkeos principales*/
        if (!file)
        {
            error404(socket, parser, server_name);
            //close(socket);
            return;
        }

        char *ext;
        ext = fileTypeSwitch(parser);
        if (strcmp(ext, "ERROR") == 0)
        {
            syslog(LOG_INFO, "ERROR: Tipo de archivo sin soporte o ");
            close(socket);
            return;
        }

        /**Ensamblamos la respuesta*/
        struct stat s;
        stat(url, &s);
        /*Cabecera*/
        head = HEAD(parser, server_name, server_root, s.st_size);
        write(socket, head, strlen(head));
        /**Mensaje ensamblado*/
        fileLinuxDescriptor = fileno(file);

        sendfile(socket, fileLinuxDescriptor, 0, s.st_size);
        fclose(file);
    }

    free(head);
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
void POST(HttpPetition *parser, char *server_name, char *server_root, int socket)
{

    char url[200];
    char imputScript[600];
    FILE *outputScript;
    char *head;
    int fileLinuxDescriptor;
    char *buffer;
    int size;
    int i = 1;
    sprintf(url, "%s%s", server_root, parser->path);

    if (parser->path_list_size == 0)
    {
        if (strstr(parser->objectType, "py"))
        {
            sprintf(imputScript, "echo %s | python3 %s", parser->bodyList, url);
        }
        else
        {
            sprintf(imputScript, "echo %s | php %s", parser->bodyList, url);
        }

        /** Como metodo para obtener el script lo almacenamos en un archivo con descriptor generado automaticamente*/
        outputScript = popen(imputScript, "r");
        if (outputScript == NULL)
        {
            syslog(LOG_INFO, "ERROR: Script exec\n");
            error404(socket, parser, server_name);
            close(socket);
        }

        /**Toca mandar una request a head para que nos de la cabecera, para ello leemos el file_size*/
        fileLinuxDescriptor = fileno(outputScript);

        sleep(3);

        do
        {
            buffer = realloc(buffer, 2000 * i);
            size += read(fileLinuxDescriptor, buffer, 2000);
            i++;
        } while (size == 2000 *(i-1));

        head = HEAD(parser, server_name, server_root, size);

        /**Mensaje ensamblado*/
        write(socket, head, strlen(head));
        write(socket, buffer, size);

        pclose(outputScript);
        free(buffer);
    }
    else
    {
        if (strstr(parser->objectType, "py"))
        {
            sprintf(imputScript, "echo %s | python3 %s %s", parser->bodyList, url, parser->pathList);
        }
        else
        {
            sprintf(imputScript, "echo %s | php %s %s", parser->bodyList, url, parser->pathList);
        }

        /** Como metodo para obtener el script lo almacenamos en un archivo con descriptor generado automaticamente*/
        outputScript = popen(imputScript, "r");
        if (outputScript == NULL)
        {
            syslog(LOG_INFO, "ERROR: Script exec\n");
            error404(socket, parser, server_name);
            close(socket);
        }

        /**Toca mandar una request a head para que nos de la cabecera, para ello leemos el file_size*/
        fileLinuxDescriptor = fileno(outputScript);
        sleep(3);
        do
        {
            buffer = realloc(buffer, 2000 * i);
            size += read(fileLinuxDescriptor, buffer, 2000);
            i++;
        } while (size == 2000*(i-1));

        head = HEAD(parser, server_name, server_root, size);

        /**Mensaje ensamblado*/
        write(socket, head, strlen(head));
        write(socket, buffer, size);

        pclose(outputScript);
        free(buffer);
    }
    free(head);

    return;
}

char *fileTypeSwitch(HttpPetition *parser)
{

    char *ext = parser->objectType;

    if (strcmp(ext, "txt") == 0)
    {
        return "text/plain";
    }
    else if (strcmp(ext, "html") == 0)
    {
        return "text/html";
    }
    else if (strcmp(ext, "gif") == 0)
    {
        return "image/gif";
    }
    else if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcmp(ext, "mpeg") == 0 || strcmp(ext, "mpg") == 0)
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
    else if (strcmp(ext, "py") == 0)
    {
        return "text/plain";
    }
    else if (strcmp(ext, "php") == 0)
    {
        return "text/plain";
    }
    else
    {
        return "ERROR";
    }
}

void error400(int socket, char *server_name)
{
    char requestDate[100];
    char response[500];
    time_t tim = time(0);
    struct tm *getTime = gmtime(&tim);
    /*Obtenemos la fecha de la request*/
    strftime(requestDate, 100, "%a, %d %b %Y %H:%M:%S %Z", getTime);

    sprintf(response, "HTTP/1.1 400 Bad Request\r\nDate: %s\r\nServer: %s\r\nContent-Length: 0\r\nContent-Type: text/plain\r\n\r\n", requestDate, server_name);
    write(socket, response, strlen(response));
    memset(response, 0, strlen(response));
}

void error404(int socket, HttpPetition *parser, char *server_name)
{
    char requestDate[100];
    char response[500];
    time_t tim = time(0);
    struct tm *getTime = gmtime(&tim);
    /*Obtenemos la fecha de la request*/
    strftime(requestDate, 100, "%a, %d %b %Y %H:%M:%S %Z", getTime);

    sprintf(response, "HTTP/1.%d 404 Not Found\r\nDate: %s\r\nServer: %s\r\nContent-Length: 0\r\nContent-Type: text/plain\r\n\r\n",
            parser->minorVersion, requestDate, server_name);
    write(socket, response, strlen(response));
    memset(response, 0, strlen(response));
}

void error500(int socket, char *server_name)
{
    char requestDate[100];
    char response[500];
    time_t tim = time(0);
    struct tm *getTime = gmtime(&tim);
    /*Obtenemos la fecha de la request*/
    strftime(requestDate, 100, "%a, %d %b %Y %H:%M:%S %Z", getTime);

    sprintf(response, "HTTP/1.1 500 Internal Error\r\nDate: %s\r\nServer: %s\r\nContent-Length: 0\r\nContent-Type: text/plain\r\n\r\n", requestDate, server_name);
    write(socket, response, strlen(response));
    memset(response, 0, strlen(response));
}