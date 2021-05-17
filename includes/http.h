/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file http.h
  * @date 18 Mar 2021
  * @brief
  */
#ifndef HTTP_H
#define HTTP_H

/* ---- Estructura Peticion Http ---- */
typedef struct _HttpPetition
{
  char method[10];
  char path[100];
  int petitionLength;
  int minorVersion;
  int methodLength;
  int path_len;
  int num_headers;
  int closed;
  char objectType[5];
  char pathList[150];
  char bodyList[150];
  int path_list_size;
  int body_list_size;
} HttpPetition;

#endif