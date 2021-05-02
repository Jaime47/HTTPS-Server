/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
#include<stdio.h>
#include<unistd.h>
#include <time.h>
#include"sys/stat.h"
#define MAX_CHAR 128
#define LINEARRAYSIZE 20


/* ---- ED Peticion Http ---- */
typedef struct _HttpPetition {
  char method[MAX_CHAR];
  char urn[MAX_CHAR];
  char http_version[MAX_CHAR];
} HttpPetition;

/**
 * Metodo HEAD
 * 
 * 
 */

char * HEAD (HttpPetition * parser,char*server_name, char* server_root, int socket){
    char  * url;
    char* time[50];
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    char * response;
    url = server_root + urlParsed;

    //Last-Modified
    struct stat attrib;
    stat(url, &attrib);
    strftime(time, 50, "%Y-%m-%d %H:%M:%S\n", localtime(&attrib.st_mtime));
    strcat(response,"Last-Modified: %s", time);
    // Date
    strcat(response,"Date: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    // Content-type
    strcat(response, "File-Type: %s\n", attrib.st_mode);
           free(buffer);
           free(attrib);
           return NULL;
       }
       buffer += i;
       length -= i;





void OPTIONS(HttpPetition * parser, char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    url = server_root + urlParsed;
    char * head;

    // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }
    //Allow
    strcat(response,"Allow: GET, POST, OPTIONS\n");
    head = HEAD();
    strcat(response, head);
    //Sending response

    write(socket, response, sizeof(response));
    return;
}

void GET(HttpPetition * Parser,char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    char * head;
    char * body;
    
    url = server_root + urlParsed;
    

    // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }

    //HEAD
    head = HEAD();

    strcat(response, head);

    strcat(response, "<html><body><h1>");

    if(Parser.objectType == ".py" ||Parser.objectType == ".php"){
    //BODY OBTENTION
    body = scriptInterpreter(Parser);
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

void POST(HttpPetition * parser,char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    char * head;
    char * body;
    char * hiddenParam;
    url = server_root + urlParsed + hiddenParam;

        // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }

    //HEAD
    head = HEAD();

    strcat(response, head);

    strcat(response, "<html><body><h1>");

    if(Parser.objectType == ".py" ||Parser.objectType == ".php"){
    //BODY OBTENTION
    body = scriptInterpreter(Parser);
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
           free(attrib);
           return NULL;
       }
       buffer += i;
       length -= i;
   }

    return buffer;
}


//pseudocodigo
char * scriptInterpreter(HttpPetition * parser){
    char * command;
    char reader[1024];
    char * output;
    int i = 0;

    
    if(parser.objetcType = ".py"){

        strcpy(command, "pyhton3 ");
        strcat(command, parser.url);
        system(command);

    }

    else if(parser.objetcType = ".php"){

        strcpy(command, "php ");
        strcat(command, parser.url);
        system(command);

    }

    FILE *fp = popen(command, "r");
    if (!fp)
    {
        return NULL;
    }

    output = malloc(sizeof(reader) * LINEARRAYSIZE);
    
    while (fgets(reader, sizeof(reader), fp) != "\r\n")
    {
        strcat(output, reader);
        if (i >= LINEARRAYSIZE){
            realloc(output, 1024 * (LINEARRAYSIZE+i));
        }
        i++;
    }

    return output;


}