/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file conf.h
  * @date 11 Mar 2021
  * @brief
  */

#ifndef conf_h
/* --- Includes --- */
#include <syslog.h>
#include <confuse.h>
#define conf_h

/**
 * @brief conf_parser : Lee el archivo de configuracion y parsea sus elementos clave-valor
 * @param
 * @return Structure with server configuration
 **/

cfg_t *conf_parser();

#endif