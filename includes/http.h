#ifndef HTTP_H
#define HTTP_H
 

 
/* ---- Estructura Peticion Http ---- */
typedef struct _HttpPetition {
  char * method;
  char * path;
  int petitionLength;
  int minorVetsion;
  int methodLength;
  int path_len;
  int num_headers;
  struct phr_header * headers;
  char * objectType;
  char ** pathList;
  char ** bodyList;
  int path_list_size;
  int body_list_size;
} HttpPetition;
 
#endif 