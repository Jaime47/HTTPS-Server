/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include "../includes/socket.h"
#include "../includes/conf.h"
#include "../includes/my_lock.h"
volatile sig_atomic_t END_FLAG = 1;

/**
 * @brief Signal function that sets to zero global flag when (Ctrl+C)
 * @param int Not used
 * 
 */
void end_server(int sig)
{               // can be called asynchronously
  END_FLAG = 0; // set flag
}

/**
 * @brief La subrutina principal de cada proceso hijo, todos concurren a un semaforo globar para ir aceptando peticiones y luego las sirven.
 * @param listenfd Puerto de escucha del servidor necesario para Accept
 * @param conf Configuracion del server
 */
void child_main(int listenfd, cfg_t *conf)
{
  int connfd;
  int P_FLAG = 1;

  while (1)
  {

    if(END_FLAG == 0){
      return;
    }

    my_lock_wait();
    connfd = accept(listenfd, NULL, NULL);
    setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    my_lock_release();

    while (P_FLAG != 0)
    {
      if (!END_FLAG)
        break;
      P_FLAG = process_request(connfd, conf);
    }

    close(connfd);
  }
  return;
}

/**
 * 
 *@brief Main: Proceso principal que inicializa el servidor, lee el archivo de configuracion y comienza a crear los procesos hijos de este. 
 * 
 * 
 */
int main()
{
  /*Declaracion de variables*/
  int listenfd, i = 0;
  //cfg_t * conf;
  /*Obtenemos configuracion y numero de procesos hijos*/
  //conf = conf_parser();

  char *server_root = NULL;
  char *server_signature = NULL;
  long int max_clients;
  long int listen_port;

  cfg_t *conf;

  cfg_opt_t opts[] = {
      CFG_SIMPLE_STR("server_root", &server_root),
      CFG_SIMPLE_INT("max_clients", &max_clients),
      CFG_SIMPLE_INT("listen_port", &listen_port),
      CFG_SIMPLE_STR("server_signature", &server_signature),
      CFG_END()};
  conf = cfg_init(opts, 0);
  if (cfg_parse(conf, "server.conf") == CFG_FILE_ERROR)
  {
    syslog(LOG_INFO, "File reading error");
  };

  /*Inicializamos el server*/
  int NCHILDS = cfg_getint(conf, "max_clients");
  pid_t childpids[NCHILDS];

  listenfd = server_ini(conf);
  signal(SIGINT, end_server);

  my_lock_init();

  for (i = 0; i < NCHILDS; i++)
  {
    childpids[i] = fork();
    if (childpids[i] == 0)
    {
      child_main(listenfd, conf);
      exit(EXIT_SUCCESS);
    }
  }
  while (1)
  {

    if (!END_FLAG)
    {
      sleep(2);
      close(listenfd);
      for (i = 0; i < NCHILDS; i++)
      {
        
        kill(childpids[i], SIGTERM);
      }
      free(conf);
      return EXIT_SUCCESS;
    }
  }

  close(listenfd);
  return EXIT_SUCCESS;
}
