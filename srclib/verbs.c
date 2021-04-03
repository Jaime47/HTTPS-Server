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
/**
 * 
 * 
 * 
 */
void OPTIONS(, char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    char* time[50];
    url = server_root + urlParsed;
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);

    // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }
    //Allow
    strcat(response,"Allow: GET, POST, OPTIONS\n");
    //Last-Modified
    struct stat attrib;
    stat(url, &attrib);
    strftime(time, 50, "%Y-%m-%d %H:%M:%S\n", localtime(&attrib.st_mtime));
    strcat(response,"Last-Modified: %s", time);
    // Date
    strcat(response,"Date: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    // Content-type
    strcat(response, "File-Type: %s\n", attrib.st_mode);
    //Longitud del archivo(Bytes)
    strcat(response,"Content-Lenght: %d\n", attrib.st_size);
    //Server name
    strcat(response, "Server: %s\n", server_name);
    //Sending response
    write(socket, response, sizeof(response));
    return;
}

void GET( ,char *server_name, char * server_root, int socket){

    char * url;
    char * response;
    char* time[50];
    url = server_root + urlParsed;
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);

    // HTTP version and code
    if (access(url, F_OK ) != -1){
        response = "HTTP/1.1 200 OK\n";
    }
    else{
        response = "HTTP/1.1 404 NOT FOUND\n";
    }
    //Last-Modified
    struct stat attrib;
    stat(url, &attrib);
    strftime(time, 50, "%Y-%m-%d %H:%M:%S\n", localtime(&attrib.st_mtime));
    strcat(response,"Last-Modified: %s", time);
    // Date
    strcat(response,"Date: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    // Content-type
    strcat(response, "File-Type: %s\n", attrib.st_mode);
    //Longitud del archivo(Bytes)
    strcat(response,"Content-Lenght: %d\n", attrib.st_size);
    //Server name
    strcat(response, "Server: %s\n", server_name);
    //Body of the mesaage
    strcat(response, "\n");
    
}