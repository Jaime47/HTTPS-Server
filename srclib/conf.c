
/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */

#include "../includes/conf.h"


/**
 * @brief conf_parser : Lee el archivo de configuracion y parsea sus elementos clave-valor
 * @param
 * @return Structure with server configuration
 **/

cfg_t *conf_parser()
{
  static char *server_root = NULL;
  static char *server_signature = NULL;
  static long int max_clients;
  static long int listen_port;
  cfg_opt_t opts[] = {
      CFG_SIMPLE_STR("server_root", &server_root),
      CFG_SIMPLE_INT("max_clients", &max_clients),
      CFG_SIMPLE_INT("listen_port", &listen_port),
      CFG_SIMPLE_STR("server_signature", &server_signature),
      CFG_END()};
  cfg_t *cfg;
  cfg = cfg_init(opts, 0);
  cfg_parse(cfg, "server.conf");
  return cfg;
}